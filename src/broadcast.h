#ifndef BROADCAST_H
#define	BROADCAST_H

#define NETLINK_BROADCAST_CHANNEL 31

int init_Broadcast(void);

int dest_Broadcast(void);

void broadcast(struct sk_buff*);

#endif	/* LOGMESSAGEBROADCAST_H */