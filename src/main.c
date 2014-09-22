#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/inet.h>
#include <linux/ip.h>
#include <linux/socket.h>
#include <net/sock.h>

#include <linux/netlink.h>
#include <linux/skbuff.h>

#include "broadcast.h"
#include "communicate.h"
#include "capture.h"
#include "datastore.h"


int init_module()
{
    int returnCode = 0;
    //init Datastore
    datastore_init();
    
    //Start Netlink Broadcast for LogMessages
    printk(KERN_DEBUG "Starting Netlink Broadcast\n");
    returnCode = init_Broadcast();
    
    printk(KERN_DEBUG "Checking Broadcast Return Value\n");
    if(returnCode >= 300) {
        return returnCode;
    }    

    //Start Netlink for Communication
    printk(KERN_DEBUG "Starting Netlink Communcation\n");
    returnCode = init_Communicate();
    
    printk(KERN_DEBUG "Checking Communcation Return Value\n");
    if(returnCode >= 300) {
        return returnCode;
    }   
    
    //Netfilter
    printk(KERN_DEBUG "Starting Capture\n");
    returnCode = init_Capture();
    
    printk(KERN_DEBUG "Checking Capture Return Code\n");
    if(returnCode >= 300) {        
        return returnCode;
    }
    
    
    return 0;
}


void cleanup_module()
{
    dest_Capture();
    dest_Communicate();
    dest_Broadcast();
    
    datastore_dest();
    return;    
}

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Dan Goldsmith <d2g.org.uk>");
MODULE_DESCRIPTION("Cloud Pathway Kernel Module");