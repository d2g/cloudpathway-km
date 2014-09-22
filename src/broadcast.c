#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

#include <linux/inet.h>
#include <linux/ip.h>
#include <linux/socket.h>
#include <net/sock.h>

#include <linux/netlink.h>
#include <linux/skbuff.h>

#include "broadcast.h"

struct sock *netlink_broadcast_socket = NULL;

int init_Broadcast() {
    
    //create Netlink Socket So We Can receive updates from Go Lang :D
    netlink_broadcast_socket = netlink_kernel_create(&init_net, NETLINK_BROADCAST_CHANNEL, NULL); 
    if(!netlink_broadcast_socket)
    {
        printk(KERN_INFO "Error creating Netlink Broadcast Socket.\n");
        return 591;
    }

    return 200;
}

int dest_Broadcast(){
    netlink_kernel_release(netlink_broadcast_socket);
    return 200;
}

void broadcast(struct sk_buff *netfilter_socket_buffer){
    int res;

    struct sk_buff *broadcastSocketBuffer;
    struct nlmsghdr *netlink_header;
    
    broadcastSocketBuffer = nlmsg_new(netfilter_socket_buffer->len,0);
    
    if(!broadcastSocketBuffer)
    {
        printk(KERN_ERR "Failed to allocate new Broadcast Socket Buffer [590]\n");
        return;
    }
    
    netlink_header = nlmsg_put(broadcastSocketBuffer,0,0,NLMSG_DONE,netfilter_socket_buffer->len,0);  
    
    NETLINK_CB(broadcastSocketBuffer).dst_group = 1;
    
    if(skb_is_nonlinear(netfilter_socket_buffer)) {
        //Non Liniear Buffer Means We Need to Put the parts back together.
        skb_linearize(netfilter_socket_buffer);
    }
    
    memcpy(nlmsg_data(netlink_header), netfilter_socket_buffer->data, netfilter_socket_buffer->len);
    
    res = netlink_broadcast(netlink_broadcast_socket, broadcastSocketBuffer, 0, 1, GFP_ATOMIC);

    if(res < 0) {
        printk(KERN_ERR "Error (%d) while sending broadcast message. [590]\n",res);
    }    
}
