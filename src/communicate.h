#ifndef COMMUNICATE_H
#define	COMMUNICATE_H

#define NETLINK_COMMUNICATE_CHANNEL 30

int init_Communicate(void);

int dest_Communicate(void);

void communicate(struct sk_buff*);

#endif