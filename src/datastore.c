#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

#include "datastore.h"


struct Datastore *datastore = NULL;


void datastore_init() {
    datastore = kmalloc(sizeof(struct Datastore),GFP_KERNEL);
    datastore->Records = kmalloc(1000 * sizeof(struct Record),GFP_KERNEL);
    if (datastore->Records == NULL) {
        printk(KERN_INFO "Exception Allocating Memory For Datastore Records\n");
        datastore->capacity = 0;
    } else {
        datastore->capacity = 1000;
    }
    datastore->size = 0;
}

void datastore_dest() {
    if (datastore->capacity > 0) {
        kfree(datastore->Records);
    }
    kfree(datastore);
}

void datastore_Clear() {
    //We don't free the memory we just say we've got no records.
    datastore->size = 0;
}

void datastore_ClearSingle(__u32 *ipaddress) {
    
}

void datastore_BlockRecords(__u8 count,struct Record *Records) {
    int i;
    for (i = 0; i < count; i++){
        datastore_BlockRecord(&Records[i]);
    }
}

void datastore_BulkBlockRecords(__u32 *ipaddress,__u8 count, __u32 *ipaddresses) {
    printk(KERN_DEBUG "ERROR: TODO: Implement\n");
}

void datastore_UnblockRecords(__u8 count,struct Record *Records) {
    int i;
    for (i = 0; i < count; i++){
        datastore_UnblockRecord(&Records[i]);
    }
}

void datastore_BulkUnblockRecords(__u32 *ipaddress,__u8 count, __u32 *ipaddresses) {
    printk(KERN_DEBUG "ERROR: TODO: Implement\n");
}

void datastore_BlockRecord(struct Record *record){
    //TODO:
    //Does Record Already Exists?
    if (datastore->size == datastore->capacity) {
        //TODO:
        printk(KERN_DEBUG "ERROR: TODO: Expand Capacity, Discarding Record...\n");
    } else {
        //TODO: Do some optimisation on how we store records (i.e. in some order)
        memcpy(&datastore->Records[datastore->size],record,sizeof(struct Record));        
        datastore->size++;
    }    
}

void datastore_UnblockRecord(struct Record *record){
    int position = 0;
    position = datastore_IsBlocked(record);
    
    if (position > 0) {
        //Our index Starts @ 0 are position starts @ 1.
        position--;
        
        //copy the last record over our position.
        if (datastore->size > 1) {
            memcpy(&datastore->Records[position],&datastore->Records[(datastore->size -1)],sizeof(struct Record));
        }
        //reduce the size by 1
        datastore->size--;
    } else {
        printk(KERN_DEBUG "ERROR: Unblocking Record Which isn't blocked\n");
    }
}

//Return the position (0 = 1, 1 = 2 etc, 0 = Not Found)
int datastore_IsBlocked(struct Record *record){
    int i;
    for (i =0 ; i < datastore->size; i++) {
        if (record->source == datastore->Records[i].source && record->destination == datastore->Records[i].destination) {
            return (i+1);
        }
    }
    return 0;
}