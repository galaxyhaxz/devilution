//HEADER_GOES_HERE
#ifndef __TMSG_H__
#define __TMSG_H__

extern TMsg *sgpTimedMsgHead;

int tmsg_get(unsigned char *pbMsg, unsigned int dwMaxLen);
void tmsg_add(const BYTE *pbMsg, unsigned char bLen);
void tmsg_start();
void tmsg_cleanup();

#endif /* __TMSG_H__ */
