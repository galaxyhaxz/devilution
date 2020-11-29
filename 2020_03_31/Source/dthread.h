//HEADER_GOES_HERE
#ifndef __DTHREAD_H__
#define __DTHREAD_H__

extern TDeltaInfo *sgpInfoHead;
extern HANDLE sghWorkToDoEvent; // idb

void dthread_remove_player(int pnum);
void dthread_send_delta(int pnum, BYTE cmd, const BYTE *pbSrc, DWORD dwLen);
void dthread_start();
unsigned int __stdcall dthread_handler(void *dummy);
void dthread_cleanup();

#endif /* __DTHREAD_H__ */
