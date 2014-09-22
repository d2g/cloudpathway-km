#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/time.h>

#include <linux/ip.h>
#include <linux/tcp.h>


#include "capture.h"
#include "broadcast.h"
#include "datastore.h"

struct nf_hook_ops netfilterOptions;

unsigned int capture  (
                        unsigned int hooknum, 
                        struct sk_buff *netfilter_socket_buffer, 
                        const struct net_device *networkDeviceIn, 
                        const struct net_device *networkDeviceOut, 
                        int (*okfn)(struct sk_buff *)
                        )
{
    struct iphdr  *ip_header;
    struct Record *blockRecord;
    
    if(!netfilter_socket_buffer){
        return NF_ACCEPT;
    }
    
    ip_header = (struct iphdr *)skb_network_header(netfilter_socket_buffer);
	    
    if(ip_header->protocol == IPPROTO_TCP){ //|| ip_header->protocol == IPPROTO_UDP) {                
        broadcast(netfilter_socket_buffer);
        
        blockRecord = kmalloc(sizeof(struct Record),GFP_ATOMIC);
        if (blockRecord == NULL) {
            printk(KERN_DEBUG "ERROR: Unable to Create Block Record Accepting Traffic!\n");            
        } else {
            blockRecord->source = (*ip_header).saddr;
            blockRecord->destination = (*ip_header).daddr;

            if (datastore_IsBlocked(blockRecord) > 0) {
                kfree(blockRecord);
                return NF_DROP;
            }

            //Reverse the source and destination and test that also.
            blockRecord->destination = (*ip_header).saddr;
            blockRecord->source = (*ip_header).daddr;
            
            if (datastore_IsBlocked(blockRecord) > 0) {
                kfree(blockRecord);
                return NF_DROP;
            }
                        
            kfree(blockRecord);
        }
    }
        
    return NF_ACCEPT;
}

int init_Capture() {
    
    netfilterOptions.hook = capture;
    netfilterOptions.hooknum = NF_INET_PRE_ROUTING;
    netfilterOptions.pf = PF_INET;
    netfilterOptions.priority = NF_IP_PRI_FIRST;
    nf_register_hook(&netfilterOptions);
    
    return 200;
}

int dest_Capture() {

    nf_unregister_hook(&netfilterOptions);
    return 200;
}
