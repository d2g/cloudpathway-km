#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

#include <linux/inet.h>
#include <linux/ip.h>
#include <linux/socket.h>
#include <net/sock.h>

#include <linux/netlink.h>
#include <linux/skbuff.h>

#include "communicate.h"
#include "datastore.h"

struct sock *netlink_communicate_socket = NULL;

int init_Communicate() {
    
    struct netlink_kernel_cfg netlink_config = {
        .input = communicate,
    };
    
    //create Netlink Socket So We Can receive updates from Go Lang :D
    netlink_communicate_socket = netlink_kernel_create(&init_net, NETLINK_COMMUNICATE_CHANNEL, &netlink_config);
    if(!netlink_communicate_socket)
    {
        printk(KERN_INFO "Error creating Netlink Socket.\n");
        return 591;
    }

    return 200;
}

int dest_Communicate(){
    netlink_kernel_release(netlink_communicate_socket);
    return 200;
}

void communicate(struct sk_buff *netfilter_socket_buffer){
    struct nlmsghdr *netlink_header;
    struct MsgHeader *message_header;
    struct Record *first_record;
    
    //int senders_pid;
    //int tmp_status;
    
    netlink_header = (struct nlmsghdr*)netfilter_socket_buffer->data;
    //senders_pid = netlink_header->nlmsg_pid; /*pid of sending process */
    message_header = (struct MsgHeader*)nlmsg_data(netlink_header);
    
    switch(message_header->type) {
        case 0:
            printk(KERN_DEBUG "Clear All Records\n");
            datastore_Clear();
            break;
        case 1:
            printk(KERN_DEBUG "Clear Records For Single IP\n");
            break;
        case 10:
            printk(KERN_DEBUG "Block Record\n");    
            first_record = (struct Record*)nlmsg_data(netlink_header);
            first_record = first_record + 2; //Add 2 to the pointer there must be a better solution...
            datastore_BlockRecords(message_header->records,first_record);
            break;
        case 11:
            printk(KERN_DEBUG "Debug: Bulk Block Record\n");    
            break;
        case 20:
            printk(KERN_DEBUG "Debug: Unblock Record\n");    
            first_record = (struct Record*)nlmsg_data(netlink_header);
            first_record = first_record + 2; //Add 2 to the pointer there must be a better solution...
            datastore_UnblockRecords(message_header->records,first_record);
            break;
        case 21:
            printk(KERN_DEBUG "Debug: Bulk Unblock Record\n");    
            break;            
    }    
}
