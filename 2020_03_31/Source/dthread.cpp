#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

static CCritSect sgMemCrit; // idb
static unsigned int sgnThreadId; // idb
TDeltaInfo *sgpInfoHead;
static BOOLEAN sgbThreadIsActive; // weak
HANDLE sghWorkToDoEvent; // idb

/* rdata */
static HANDLE sghThread = INVALID_HANDLE_VALUE; // idb

void dthread_remove_player(int pnum)
{
	TDeltaInfo *p;

	sgMemCrit.Enter();

	for(p = sgpInfoHead; p != NULL; p = p->pNext) {
		if(p->pnum == pnum) {
			p->pnum = 4;
		}
	}

	sgMemCrit.Leave();
}

void dthread_send_delta(int pnum, BYTE cmd, const BYTE *pbSrc, DWORD dwLen)
{
	TDeltaInfo *pCurr, *p;

	/// ASSERT: assert((DWORD) pnum < MAX_PLRS);
	/// ASSERT: assert(pnum != myplr);
	/// ASSERT: assert(pbSrc);
	/// ASSERT: assert(dwLen);

	if(gbMaxPlayers == 1) {
		return;
	}

	pCurr = (TDeltaInfo *)DiabloAllocPtr(sizeof(*pCurr) + dwLen);
	pCurr->pNext = NULL;
	pCurr->pnum = pnum;
	pCurr->cmd = cmd;
	pCurr->size = dwLen;
	memcpy(pCurr->data, pbSrc, dwLen);

	sgMemCrit.Enter();

	p = (TDeltaInfo *)&sgpInfoHead;
	while(p->pNext != NULL) {
		p = p->pNext;
	}
	p->pNext = pCurr;

	SetEvent(sghWorkToDoEvent);

	sgMemCrit.Leave();
}

void dthread_start()
{
	/// ASSERT: assert(sghThread == INVALID_HANDLE_VALUE);

	if(gbMaxPlayers == 1) {
		return;
	}

	/// ASSERT: assert(! sgpInfoHead);
	/// ASSERT: assert(! sghWorkToDoEvent);
	sghWorkToDoEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(sghWorkToDoEvent == NULL) {
		app_fatal("dthread:1\n%s", TraceLastError());
	}

	sgbThreadIsActive = TRUE;

	/// ASSERT: assert(sghThread == INVALID_HANDLE_VALUE);
	sghThread = (HANDLE)_beginthreadex(NULL, 0, dthread_handler, NULL, 0, &sgnThreadId);
	if(sghThread == INVALID_HANDLE_VALUE) {
		app_fatal("dthread2:\n%s", TraceLastError());
	}
}

unsigned int __stdcall dthread_handler(void *dummy)
{
	DWORD dwTime;
	TDeltaInfo *p;

	while(sgbThreadIsActive) {
		if(sgpInfoHead == NULL && WaitForSingleObject(sghWorkToDoEvent, INFINITE) == WAIT_FAILED) {
			app_fatal("dthread4:\n%s", TraceLastError());
		}
		sgMemCrit.Enter();
		p = sgpInfoHead;
		if(p != NULL) {
			sgpInfoHead = p->pNext;
		} else {
			ResetEvent(sghWorkToDoEvent);
		}
		sgMemCrit.Leave();
		if(p != NULL) {
			if(p->pnum != 4) {
				multi_send_zero_packet(p->pnum, p->cmd, p->data, p->size);
			}
			/// ASSERT: assert(gdwDeltaBytesSec);
			dwTime = 1000 * p->size / gdwDeltaBytesSec;
			if(dwTime >= 1) {
				dwTime = 1;
			}
			MemFreeDbg(p);
			if(dwTime != 0) {
				Sleep(dwTime);
			}
		}
	}

	return 0;
}

void dthread_cleanup()
{
	TDeltaInfo *p;

	if(sghWorkToDoEvent == NULL) {
		return;
	}

	sgbThreadIsActive = FALSE;
	SetEvent(sghWorkToDoEvent);

	if(sghThread != INVALID_HANDLE_VALUE && sgnThreadId != GetCurrentThreadId()) {
		if(WaitForSingleObject(sghThread, INFINITE) == WAIT_FAILED) {
			app_fatal("dthread3:\n(%s)", TraceLastError());
		}
		CloseHandle(sghThread);
		sghThread = INVALID_HANDLE_VALUE;
	}

	CloseHandle(sghWorkToDoEvent);
	sghWorkToDoEvent = NULL;

	while(sgpInfoHead != NULL) {
		p = sgpInfoHead->pNext;
		MemFreeDbg(sgpInfoHead);
		sgpInfoHead = p;
	}
}
