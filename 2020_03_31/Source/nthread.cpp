#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

BYTE gbDeltaTurnsSec; // weak
DWORD gdwMsgLenTbl[4];
static CCritSect sgMemCrit;
int gdwDeltaBytesSec; // weak
static BOOLEAN sgbThreadIsActive; // weak
DWORD gdwTurnsInTransit; // weak
DWORD *glpMsgTbl[4];
static unsigned int sgnThreadId;
char sgbSyncCountdown; // weak
int dword_679754; // weak
char byte_679758; // weak
char sgbPacketCountdown; // weak
char sgbThreadIsRunning; // weak
int gdwLargestMsgSize; // weak
int gdwNormalMsgSize; // weak
int dword_679764; // weak

/* data */
static HANDLE sghThread = INVALID_HANDLE_VALUE; // idb

void nthread_terminate_game(const char *pszFcn)
{
	DWORD dwError;

	/// ASSERT: assert(pszFcn);

	dwError = SErrGetLastError();

	if(dwError == STORM_ERROR_INVALID_PLAYER) {
		return;
	}

	if(dwError == STORM_ERROR_GAME_TERMINATED) {
		gbGameDestroyed = 1;
	} else if(dwError == STORM_ERROR_NOT_IN_GAME) {
		gbGameDestroyed = 1;
	} else {
		app_fatal("%s:\n%s", pszFcn, TraceLastError());
	}
}

DWORD nthread_send_and_recv_turn(DWORD cur_turn, int turn_delta)
{
	DWORD dwCount, dwTurn;

	if(!SNetGetTurnsInTransit(&dwCount)) {
		nthread_terminate_game("SNetGetTurnsInTransit");
		return 0;
	}

	/// ASSERT: assert(gdwTurnsInTransit);
	while(dwCount++ < gdwTurnsInTransit) {
		dwTurn = dword_679754 | cur_turn & 0x7FFFFFFF;
		dword_679754 = 0;
		if(!SNetSendTurn(&dwTurn, sizeof(dwTurn))) {
			nthread_terminate_game("SNetSendTurn");
			return 0;
		}
		cur_turn += turn_delta;
		if(cur_turn >= 0x7FFFFFFF) {
			cur_turn &= 0xFFFF;
		}
	}

	return cur_turn;
}

BOOL nthread_recv_turns(BOOL *pfSendAsync)
{
	/// ASSERT: assert(pfSendAsync);
	*pfSendAsync = FALSE;

#if 0
	if(is_debug_nthread) {
		nthread_debug_413D97();
	}
#endif

	/// ASSERT: assert(sgbPacketCountdown);
	sgbPacketCountdown--;
	if(sgbPacketCountdown != 0) {
		dword_679764 += 50;
		return TRUE;
	}
	sgbPacketCountdown = gbDeltaTurnsSec;

	/// ASSERT: assert(sgbSyncCountdown);
	sgbSyncCountdown--;
	if(sgbSyncCountdown == 0) {
		if(!SNetReceiveTurns(0, 4, &glpMsgTbl, &gdwMsgLenTbl, &player_state)) {
			if(SErrGetLastError() != STORM_ERROR_NO_MESSAGES_WAITING) {
				nthread_terminate_game("SNetReceiveTurns");
			}
			sgbSyncCountdown = 1;
			sgbPacketCountdown = sgbSyncCountdown;
			byte_679758 = FALSE;
			return FALSE;
		}
		if(!byte_679758) {
			byte_679758 = TRUE;
			dword_679764 = GetTickCount();
		}
		sgbSyncCountdown = 4;
		multi_msg_countdown();
	}

	*pfSendAsync = TRUE;
	dword_679764 += 50;
	return TRUE;
}

void nthread_set_turn_upper_bit()
{
	dword_679754 = 0x80000000;
}

void nthread_start(BOOL set_turn_upper_bit)
{
	_SNETCAPS caps;

	dword_679764 = GetTickCount();
	sgbPacketCountdown = 1;
	sgbSyncCountdown = 1;
	byte_679758 = TRUE;

	if(set_turn_upper_bit) {
		nthread_set_turn_upper_bit();
	} else {
		dword_679754 = 0;
	}

	caps.size = sizeof(caps);
	if(!SNetGetProviderCaps(&caps)) {
		app_fatal("SNetGetProviderCaps:\n%s", TraceLastError());
	}

	if(caps.defaultturnsintransit != 0) {
		gdwTurnsInTransit = caps.defaultturnsintransit;
	} else {
		gdwTurnsInTransit = 1;
	}

	if(caps.defaultturnssec > 20 || caps.defaultturnssec == 0) {
		gbDeltaTurnsSec = 1;
	} else {
		gbDeltaTurnsSec = 20 / caps.defaultturnssec;
	}

	gdwLargestMsgSize = caps.maxmessagesize < 512 ? caps.maxmessagesize : 512;
	/// ASSERT: assert(gdwLargestMsgSize >= MIN_MSG_SIZE);

	gdwNormalMsgSize = caps.bytessec * gbDeltaTurnsSec / 20;
	gdwDeltaBytesSec = caps.bytessec >> 2;

	gdwNormalMsgSize *= 3;
	gdwNormalMsgSize >>= 2;

	/// ASSERT: assert(caps.maxplayers);
	if(caps.maxplayers > 4) {
		caps.maxplayers = 4;
	}

	gdwNormalMsgSize /= caps.maxplayers;

	while(gdwNormalMsgSize < MIN_MSG_SIZE) {
		gdwNormalMsgSize <<= 1;
		gbDeltaTurnsSec <<= 1;
	}

	if(gdwNormalMsgSize > gdwLargestMsgSize) {
		gdwNormalMsgSize = gdwLargestMsgSize;
	}

	if(gbMaxPlayers > 1) {
		sgbThreadIsRunning = FALSE;
		sgMemCrit.Enter();
		sgbThreadIsActive = TRUE;
		/// ASSERT: assert(sghThread == INVALID_HANDLE_VALUE);
		sghThread = (HANDLE)_beginthreadex(NULL, 0, nthread_handler, NULL, 0, &sgnThreadId);
		if(sghThread == INVALID_HANDLE_VALUE) {
			app_fatal("nthread2:\n%s", TraceLastError());
		}
		SetThreadPriority(sghThread, THREAD_PRIORITY_HIGHEST);
	}
}

unsigned int __stdcall nthread_handler(void *dummy)
{
	int nTicks;
	BOOL fSendAsync;

	while(sgbThreadIsActive) {
		sgMemCrit.Enter();
		if(!sgbThreadIsActive) {
			sgMemCrit.Leave();
			return 0;
		}
		nthread_send_and_recv_turn(0, 0);
		if(nthread_recv_turns(&fSendAsync)) {
			nTicks = dword_679764 - GetTickCount();
		} else {
			nTicks = 50;
		}
		sgMemCrit.Leave();
		if(nTicks > 0) {
			Sleep(nTicks);
		}
	}

	return 0;
}

void nthread_cleanup()
{
	sgbThreadIsActive = FALSE;
	gdwTurnsInTransit = 0;
	gdwNormalMsgSize = 0;
	gdwLargestMsgSize = 0;

	if(sghThread != INVALID_HANDLE_VALUE && sgnThreadId != GetCurrentThreadId()) {
		if(!sgbThreadIsRunning) {
			sgMemCrit.Leave();
		}
		if(WaitForSingleObject(sghThread, INFINITE) == WAIT_FAILED) {
			app_fatal("nthread3:\n(%s)", TraceLastError());
		}
		CloseHandle(sghThread);
		sghThread = INVALID_HANDLE_VALUE;
	}
}

void nthread_ignore_mutex(BOOL bStart)
{
	if(sghThread == INVALID_HANDLE_VALUE) {
		return;
	}

	/// ASSERT: assert(sgbThreadIsRunning != bStart);

	if(bStart) {
		sgMemCrit.Leave();
	} else {
		sgMemCrit.Enter();
	}

	sgbThreadIsRunning = bStart;
}

BOOL nthread_has_500ms_passed(BOOL unused)
{
	int nPassed;
	DWORD dwTicks;

	dwTicks = GetTickCount();
	nPassed = dwTicks - dword_679764;

	if(gbMaxPlayers == 1 && nPassed > 500) {
		dword_679764 = dwTicks;
		nPassed = 0;
	}

	return nPassed >= 0;
}
