#ifndef DATASTORE_H
#define	DATASTORE_H

struct MsgHeader {
    __u8            type;
    __u8            records;    
};

struct Record {
    __be32   source;
    __be32   destination;
};

struct Datastore {
    int size;
    int capacity;
    
    struct Record *Records;    
};

void datastore_init(void);

void datastore_dest(void);

void datastore_Clear(void);

void datastore_ClearSingle(__u32*);

void datastore_BlockRecords(__u8,struct Record*);

void datastore_BulkBlockRecords(__u32*,__u8, __u32*);

void datastore_UnblockRecords(__u8,struct Record*);

void datastore_BulkUnblockRecords(__u32*,__u8, __u32*);

void datastore_BlockRecord(struct Record*);

void datastore_UnblockRecord(struct Record*);

int datastore_IsBlocked(struct Record*);

#endif	/* DATASTORE_H */