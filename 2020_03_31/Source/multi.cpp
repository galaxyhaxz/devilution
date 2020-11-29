#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"
#include "../DiabloUI/diabloui.h"

char gbSomebodyWonGameKludge; // weak
#ifdef _DEBUG
DWORD gdwHistTicks;
#endif
TBuffer sgHiPriBuf;
char szPlayerDescript[128];
WORD sgwPackPlrOffsetTbl[MAX_PLRS];
PkPlayerStruct netplr[MAX_PLRS];
BYTE sgbPlayerTurnBitTbl[MAX_PLRS];
char sgbPlayerLeftGameTbl[MAX_PLRS];
int sgbSentThisCycle; // idb
int dword_678628; // weak
char gbActivePlayers; // weak
char gbGameDestroyed; // weak
char sgbSendDeltaTbl[MAX_PLRS];
_gamedata sgGameInitInfo;
char gbSelectProvider; // weak
int sglTimeoutStart; // weak
int sgdwPlayerLeftReasonTbl[MAX_PLRS];
TBuffer sgLoPriBuf;
unsigned int sgdwGameLoops; // idb
unsigned char gbMaxPlayers; // weak
char sgbTimeout; // weak
char szPlayerName[128];
BYTE gbDeltaSender; // weak
int sgbNetInited; // weak
DWORD player_state[MAX_PLRS];

const int event_types[3] =
{
  EVENT_TYPE_PLAYER_LEAVE_GAME,
  EVENT_TYPE_PLAYER_CREATE_GAME,
  EVENT_TYPE_PLAYER_MESSAGE
};

#ifdef _DEBUG
void __cdecl dumphist(const char *pszFmt, ...)
{
	static FILE *sgpHistFile = NULL;
	DWORD dwTicks;
	va_list va;

	va_start(va, pszFmt);

	if(sgpHistFile == NULL) {
		sgpHistFile = fopen("c:\\dumphist.txt", "wb");
		if(sgpHistFile == NULL) {
			return;
		}
	}

	dwTicks = GetTickCount();
	fprintf(sgpHistFile, "%4u.%02u  ", (dwTicks - gdwHistTicks) / 1000, (dwTicks - gdwHistTicks) % 1000 / 10);
	vfprintf(sgpHistFile, pszFmt, va);
	fprintf(
		sgpHistFile,
		"\r\n          (%d,%d)(%d,%d)(%d,%d)(%d,%d)\r\n",
		plr[0].plractive,
		player_state[0],
		plr[1].plractive,
		player_state[1],
		plr[2].plractive,
		player_state[2],
		plr[3].plractive,
		player_state[3]);
	fflush(sgpHistFile);
}
#endif

void multi_msg_add(const BYTE *pbMsg, BYTE bLen)
{
	/// ASSERT: assert(sgbNetInited);

	if(pbMsg != NULL && bLen != 0) {
		/// ASSERT: assert(bLen <= gdwNormalMsgSize - sizeof(TPktHdr));
		tmsg_add(pbMsg, bLen);
	}
}

void NetSendLoPri(const BYTE *pbMsg, BYTE bLen)
{
	/// ASSERT: assert(sgbNetInited);

	if(pbMsg != NULL && bLen != 0) {
		/// ASSERT: assert(bLen <= gdwNormalMsgSize - sizeof(TPktHdr));
		multi_copy_packet(&sgLoPriBuf, pbMsg, bLen);
		multi_send_packet(pbMsg, bLen);
	}
}

void multi_copy_packet(TBuffer *pBuf, const BYTE *pbMsg, BYTE bLen)
{
	BYTE *p;

	if(pBuf->dwNextWriteOffset + bLen + 2 > sizeof(pBuf->bData)) {
#ifdef _DEBUG
		app_fatal("msg buffer failure");
#endif
		return;
	}

	p = &pBuf->bData[pBuf->dwNextWriteOffset];
	pBuf->dwNextWriteOffset += bLen + 1;
	*p = bLen;
	p++;
	memcpy(p, pbMsg, bLen);
	p += bLen;
	*p = 0;
}

void multi_send_packet(const BYTE *pbMsg, BYTE bLen)
{
	TPkt pkt;

	/// ASSERT: assert(sgbNetInited);
	/// ASSERT: assert(pbMsg);
	/// ASSERT: assert(bLen);
	/// ASSERT: assert(bLen <= gdwNormalMsgSize - sizeof(TPktHdr));

	NetRecvPlrData(&pkt);
	pkt.hdr.wLen = bLen + sizeof(TPktHdr);
	memcpy(pkt.body, pbMsg, bLen);

	if(!SNetSendMessage(myplr, &pkt.hdr, pkt.hdr.wLen)) {
		nthread_terminate_game("SNetSendMessage0");
	}
#if 0
	debug_plr_tbl[myplr]++;
#endif
}

void NetRecvPlrData(TPkt *pkt)
{
	/// ASSERT: assert(pkt);
	pkt->hdr.wCheck = 'ip';
	pkt->hdr.px = plr[myplr].WorldX;
	pkt->hdr.py = plr[myplr].WorldY;
	pkt->hdr.targx = plr[myplr]._ptargx;
	pkt->hdr.targy = plr[myplr]._ptargy;
	pkt->hdr.php = plr[myplr]._pHitPoints;
	pkt->hdr.pmhp = plr[myplr]._pMaxHP;
	pkt->hdr.bstr = plr[myplr]._pBaseStr;
	pkt->hdr.bmag = plr[myplr]._pBaseMag;
	pkt->hdr.bdex = plr[myplr]._pBaseDex;
}

void NetSendHiPri(const BYTE *pbMsg, BYTE bLen)
{
	DWORD dwLeft, dwLen;
	BYTE *p;
	TPkt pkt;

	/// ASSERT: assert(sgbNetInited);

	if(pbMsg != NULL && bLen != 0) {
		/// ASSERT: assert(bLen <= gdwNormalMsgSize - sizeof(TPktHdr));
		multi_copy_packet(&sgHiPriBuf, pbMsg, bLen);
		multi_send_packet(pbMsg, bLen);
	}

	if(dword_678628) {
		return;
	}

	dword_678628 = 1;
	/// ASSERT: assert((DWORD) myplr < MAX_PLRS);
	NetRecvPlrData(&pkt);
	dwLeft = gdwNormalMsgSize - sizeof(TPktHdr);
	p = multi_recv_packet(&sgHiPriBuf, pkt.body, &dwLeft);
	p = multi_recv_packet(&sgLoPriBuf, p, &dwLeft);
	dwLeft = sync_all_monsters(p, dwLeft);
	dwLen = gdwNormalMsgSize - dwLeft;
	pkt.hdr.wLen = dwLen;

	if(!SNetSendMessage(SNPLAYER_OTHERS, &pkt.hdr, dwLen)) {
		nthread_terminate_game("SNetSendMessage");
	}
#if 0
	if(myplr != 0) {
		debug_plr_tbl[0]++;
	}
	if(myplr != 1) {
		debug_plr_tbl[1]++;
	}
	if(myplr != 2) {
		debug_plr_tbl[2]++;
	}
	if(myplr != 3) {
		debug_plr_tbl[3]++;
	}
#endif
}

BYTE *multi_recv_packet(TBuffer *pBuf, BYTE *p, DWORD *dwSize)
{
	BYTE bLen;
	BYTE *pbMsg;

	if(pBuf->dwNextWriteOffset == 0) {
		return p;
	}

	pbMsg = pBuf->bData;
	while(1) {
		bLen = *pbMsg;
		if(bLen == 0 || bLen > *dwSize) {
			break;
		}
		pbMsg++;
		memcpy(p, pbMsg, bLen);
		pbMsg += bLen;
		p += bLen;
		*dwSize -= bLen;
	}

	memcpy(pBuf->bData, pbMsg, pBuf->dwNextWriteOffset - (pbMsg - pBuf->bData) + 1);
	pBuf->dwNextWriteOffset -= pbMsg - pBuf->bData;
	return p;
}

void multi_send_msg_packet(int msk, const BYTE *pbMsg, BYTE bLen)
{
	int m, pnum;
	DWORD dwSendBytes;
	TPkt pkt;

	/// ASSERT: assert(sgbNetInited);
	/// ASSERT: assert(pbMsg);
	/// ASSERT: assert(bLen);

	NetRecvPlrData(&pkt);

	dwSendBytes = bLen + sizeof(TPktHdr);
	/// ASSERT: assert(dwSendBytes < gdwNormalMsgSize);
	pkt.hdr.wLen = dwSendBytes;
	memcpy(pkt.body, pbMsg, bLen);

	m = 1;
	pnum = 0;
	while((DWORD)pnum < MAX_PLRS) {
		if(m & msk) {
			if(SNetSendMessage(pnum, &pkt.hdr, dwSendBytes)) {
#if 0
				debug_plr_tbl[pnum]++;
#endif
			} else if(SErrGetLastError() != STORM_ERROR_INVALID_PLAYER) {
				nthread_terminate_game("SNetSendMessage");
				return;
			}
		}
		pnum++;
		m <<= 1;
	}
}

void multi_msg_countdown()
{
	int i;

	for(i = 0; i < MAX_PLRS; i++) {
		if(player_state[i] & 0x20000) {
			/// ASSERT: assert(glpMsgTbl[i]);
			if(gdwMsgLenTbl[i] == sizeof(DWORD)) {
				multi_parse_turn(i, *glpMsgTbl[i]);
			}
		}
	}
}

void multi_parse_turn(int pnum, DWORD turn)
{
	if(turn & 0x80000000) {
		multi_handle_turn_upper_bit(pnum);
	}

	turn &= ~0x80000000;

	if(sgbSentThisCycle < gdwTurnsInTransit + turn) {
		if(turn >= ~0x80000000) {
			turn &= 0xFFFF;
		}
		sgbSentThisCycle = turn + gdwTurnsInTransit;
		turn *= gbDeltaTurnsSec;
		turn <<= 2;
		sgdwGameLoops = turn;
	}
}

void multi_handle_turn_upper_bit(int pnum)
{
	int i;

	for(i = 0; i < MAX_PLRS; i++) {
		if(player_state[i] & 0x10000 && i != pnum) {
			break;
		}
	}

	if(myplr == i) {
		sgbSendDeltaTbl[pnum] = 1;
	} else if(myplr == pnum) {
		gbDeltaSender = i;
	}
}

void multi_player_left(int pnum, DWORD reason)
{
	sgbPlayerLeftGameTbl[pnum] = 1;
	sgdwPlayerLeftReasonTbl[pnum] = reason;
	multi_clear_left_tbl();
}

void multi_clear_left_tbl()
{
	int i;

	for(i = 0; i < MAX_PLRS; i++) {
		if(sgbPlayerLeftGameTbl[i]) {
			if(gbBufferMsgs == 1) {
#ifdef _DEBUG
				dumphist("(%d) buffering -- player %d left game due to %d", myplr, i, sgdwPlayerLeftReasonTbl[i]);
#endif
				msg_send_drop_pkt(i, sgdwPlayerLeftReasonTbl[i]);
			} else {
#ifdef _DEBUG
				dumphist("(%d) player %d left game due to %d", myplr, i, sgdwPlayerLeftReasonTbl[i]);
#endif
				multi_player_left_msg(i, TRUE);
			}
			sgbPlayerLeftGameTbl[i] = 0;
			sgdwPlayerLeftReasonTbl[i] = 0;
		}
	}
}

void multi_player_left_msg(int pnum, BOOL left)
{
	char *pszLeft;

	if(!plr[pnum].plractive) {
		return;
	}

#ifdef _DEBUG
	dumphist("(%d) player %d --> inactive", myplr, pnum);
#endif
	RemovePlrFromMap(pnum);
	RemovePortalMissile(pnum);
	DeactivatePortal(pnum);
	delta_close_portal(pnum);
	RemovePlrMissiles(pnum);

	if(left) {
		pszLeft = "Player '%s' just left the game";
		switch(sgdwPlayerLeftReasonTbl[pnum]) {
		case 0x40000006:
			pszLeft = "Player '%s' dropped due to timeout";
			break;
		case 0x40000004:
			pszLeft = "Player '%s' killed Diablo and left the game!";
			gbSomebodyWonGameKludge = 1;
			break;
		}
		EventPlrMsg(pszLeft, plr[pnum]._pName);
	}

	plr[pnum].plractive = 0;
	plr[pnum]._pName[0] = '\0';
	gbActivePlayers--;
}

void multi_net_ping()
{
	sgbTimeout = 1;
	sglTimeoutStart = GetTickCount();
}

BOOL multi_handle_delta()
{
	int i;
	BOOL fSendAsync;

	/// ASSERT: assert(sgbNetInited);

	if(gbGameDestroyed) {
		gbRunGame = 0;
		return FALSE;
	}

	for(i = 0; i < MAX_PLRS; i++) {
		if(sgbSendDeltaTbl[i]) {
			sgbSendDeltaTbl[i] = 0;
			DeltaExportData(i);
		}
	}

	sgbSentThisCycle = nthread_send_and_recv_turn(sgbSentThisCycle, 1);

	if(!nthread_recv_turns(&fSendAsync)) {
		multi_begin_timeout();
		return FALSE;
	}

	sgbTimeout = 0;

	if(fSendAsync) {
		if(!dword_678628) {
			NetSendHiPri(NULL, 0);
			dword_678628 = 0;
		} else {
			dword_678628 = 0;
			if(!multi_check_pkt_valid(&sgHiPriBuf)) {
				NetSendHiPri(NULL, 0);
			}
		}
	}

	multi_mon_seeds();
	return TRUE;
}

BOOL multi_check_pkt_valid(TBuffer *pBuf)
{
	return pBuf->dwNextWriteOffset == 0;
}

void multi_mon_seeds()
{
	int i;
	DWORD s;

	sgdwGameLoops++;
	s = _rotr(sgdwGameLoops, 8);

	for(i = 0; i < MAXMONSTERS; i++) {
		monster[i]._mAISeed = i + s;
	}
}

void multi_begin_timeout()
{
	int i, nTicks, nState, nLowestActive, nLowestPlayer;
	BYTE bGroupPlayers, bGroupCount;

	if(!sgbTimeout) {
		return;
	}
#ifdef _DEBUG
	if(debug_mode_key_i) {
		return;
	}
#endif

	nTicks = GetTickCount() - sglTimeoutStart;
	if(nTicks > 20000) {
		gbRunGame = FALSE;
		return;
	}
	if(nTicks < 10000) {
		return;
	}

	nLowestActive = -1;
	nLowestPlayer = -1;
	bGroupPlayers = 0;
	bGroupCount = 0;
	for(i = 0; i < MAX_PLRS; i++) {
		nState = player_state[i];
		if(nState & 0x10000) {
			if(nLowestPlayer == -1) {
				nLowestPlayer = i;
			}
			if(nState & 0x40000) {
				bGroupPlayers++;
				if(nLowestActive == -1) {
					nLowestActive = i;
				}
			} else {
				bGroupCount++;
			}
		}
	}

	/// ASSERT: assert(bGroupPlayers);
	/// ASSERT: assert(nLowestActive != -1);
	/// ASSERT: assert(nLowestPlayer != -1);

#ifdef _DEBUG
	dumphist(
		"(%d) grp:%d ngrp:%d lowp:%d lowa:%d",
		myplr,
		bGroupPlayers,
		bGroupCount,
		nLowestPlayer,
		nLowestActive);
#endif

	if(bGroupPlayers < bGroupCount) {
		gbGameDestroyed = TRUE;
	} else if(bGroupPlayers == bGroupCount) {
		if(nLowestPlayer != nLowestActive) {
			gbGameDestroyed = TRUE;
		} else if(nLowestActive == myplr) {
			multi_check_drop_player();
		}
	} else if(nLowestActive == myplr) {
		multi_check_drop_player();
	}
}

void multi_check_drop_player()
{
	int i;

	for(i = 0; i < MAX_PLRS; i++) {
		if(!(player_state[i] & 0x40000) && player_state[i] & 0x10000) {
#ifdef _DEBUG
			dumphist("(%d) dropping player %d state %d", myplr, i, player_state[i]);
#endif
			SNetDropPlayer(i, 0x40000006);
		}
	}
}

void multi_process_network_packets()
{
	int dx, dy;
	DWORD dwID, dwMsgSize;
	char *p;
	TPkt *pkt;

	multi_clear_left_tbl();
	multi_process_tmsgs();

	/// ASSERT: assert(sgbNetInited);

	while(SNetReceiveMessage(&dwID, &p, &dwMsgSize)) {
		dwRecCount++; /* unused, probably debug to track total packets received */
		multi_clear_left_tbl();
		pkt = (TPkt *)p;
		/// ASSERT: assert(dwMsgSize >= sizeof(TPktHdr));
		/// ASSERT: assert(dwID < MAX_PLRS);
		/// ASSERT: assert(pkt->hdr.wCheck == 0x6970);
		/// ASSERT: assert(pkt->hdr.wLen == dwMsgSize);
		if(dwMsgSize < sizeof(TPktHdr)) {
			continue;
		}
		if(dwID >= MAX_PLRS) {
			continue;
		}
		if(pkt->hdr.wCheck != 'ip') {
			continue;
		}
		if(pkt->hdr.wLen != dwMsgSize) {
			continue;
		}
		plr[dwID]._pownerx = pkt->hdr.px;
		plr[dwID]._pownery = pkt->hdr.py;
#if 0
		debug_act_plrs[dwID]++;
#endif
		if(dwID != myplr) {
			/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);
			plr[dwID]._pHitPoints = pkt->hdr.php;
			plr[dwID]._pMaxHP = pkt->hdr.pmhp;
			plr[dwID]._pBaseStr = pkt->hdr.bstr;
			plr[dwID]._pBaseMag = pkt->hdr.bmag;
			plr[dwID]._pBaseDex = pkt->hdr.bdex;
			if(gbBufferMsgs != 1 && plr[dwID].plractive && plr[dwID]._pHitPoints != 0) {
				if(currlevel == plr[dwID].plrlevel && !plr[dwID]._pLvlChanging) {
					dx = abs(plr[dwID].WorldX - pkt->hdr.px);
					dy = abs(plr[dwID].WorldY - pkt->hdr.py);
					if((dx > 3 || dy > 3) && dPlayer[pkt->hdr.px][pkt->hdr.py] == 0) {
						FixPlrWalkTags(dwID);
						plr[dwID]._poldx = plr[dwID].WorldX;
						plr[dwID]._poldy = plr[dwID].WorldY;
						FixPlrWalkTags(dwID);
						plr[dwID].WorldX = pkt->hdr.px;
						plr[dwID].WorldY = pkt->hdr.py;
						plr[dwID]._px = pkt->hdr.px;
						plr[dwID]._py = pkt->hdr.py;
						dPlayer[plr[dwID].WorldX][plr[dwID].WorldY] = dwID + 1;
					}
					dx = abs(plr[dwID]._px - plr[dwID].WorldX);
					dy = abs(plr[dwID]._py - plr[dwID].WorldY);
					if(dx > 1 || dy > 1) {
						plr[dwID]._px = plr[dwID].WorldX;
						plr[dwID]._py = plr[dwID].WorldY;
					}
					MakePlrPath(dwID, pkt->hdr.targx, pkt->hdr.targy, TRUE);
				} else {
					plr[dwID].WorldX = pkt->hdr.px;
					plr[dwID].WorldY = pkt->hdr.py;
					plr[dwID]._px = pkt->hdr.px;
					plr[dwID]._py = pkt->hdr.py;
					plr[dwID]._ptargx = pkt->hdr.targx;
					plr[dwID]._ptargy = pkt->hdr.targy;
				}
			}
		}
		multi_handle_all_packets(dwID, pkt->body, dwMsgSize - sizeof(TPktHdr));
	}

	if(SErrGetLastError() != STORM_ERROR_NO_MESSAGES_WAITING) {
		nthread_terminate_game("SNetReceiveMsg");
	}
}

void multi_handle_all_packets(int pnum, BYTE *pData, DWORD dwSize)
{
	DWORD dwLen;

	while(dwSize != 0) {
		dwLen = ParseCmd(pnum, (TCmd *)pData);
		if(dwLen == 0) {
			break;
		}
		pData += dwLen;
		dwSize -= dwLen;
	}
}

void multi_process_tmsgs()
{
	DWORD dwSize;
	TPkt pkt;

	while(dwSize = tmsg_get((BYTE *)&pkt, sizeof(pkt))) {
		multi_handle_all_packets(myplr, (BYTE *)&pkt, dwSize);
	}
}

void multi_send_zero_packet(int pnum, BYTE bCmd, BYTE *pbSrc, DWORD dwLen)
{
	DWORD dwOffset, dwBody, dwMsg;
	TCmdPlrInfoHdr *p;
	TPkt pkt;

	/// ASSERT: assert(pnum != myplr);
	/// ASSERT: assert(pbSrc);
	/// ASSERT: assert(dwLen <= 0x0ffff);

	dwOffset = 0;
	while(dwLen != 0) {
		pkt.hdr.wCheck = 'ip';
		pkt.hdr.px = 0;
		pkt.hdr.py = 0;
		pkt.hdr.targx = 0;
		pkt.hdr.targy = 0;
		pkt.hdr.php = 0;
		pkt.hdr.pmhp = 0;
		pkt.hdr.bstr = 0;
		pkt.hdr.bmag = 0;
		pkt.hdr.bdex = 0;
		p = (TCmdPlrInfoHdr *)pkt.body;
		p->bCmd = bCmd;
		p->wOffset = dwOffset;
		dwBody = gdwLargestMsgSize - sizeof(TPktHdr) - sizeof(*p);
		if(dwLen < dwBody) {
			dwBody = dwLen;
		}
		/// ASSERT: assert(dwBody <= 0x0ffff);
		p->wBytes = dwBody;
		memcpy(&pkt.body[sizeof(*p)], pbSrc, p->wBytes);
		dwMsg = sizeof(TPktHdr);
		dwMsg += sizeof(*p);
		dwMsg += p->wBytes;
		pkt.hdr.wLen = dwMsg;
		if(!SNetSendMessage(pnum, &pkt, dwMsg)) {
			nthread_terminate_game("SNetSendMessage2");
			return;
		}
#if 0
		if((DWORD)pnum < MAX_PLRS) {
			debug_plr_tbl[pnum]++;
		} else {
			if(myplr != 0) {
				debug_plr_tbl[0]++;
			}
			if(myplr != 1) {
				debug_plr_tbl[1]++;
			}
			if(myplr != 2) {
				debug_plr_tbl[2]++;
			}
			if(myplr != 3) {
				debug_plr_tbl[3]++;
			}
		}
#endif
		pbSrc += p->wBytes;
		dwLen -= p->wBytes;
		dwOffset += p->wBytes;
	}
}

void NetClose()
{
	if(sgbNetInited) {
		sgbNetInited = 0;
		nthread_cleanup();
		dthread_cleanup();
		tmsg_cleanup();
		multi_event_handler(FALSE);
		SNetLeaveGame(3);
		msgcmd_cmd_cleanup();
#ifdef _DEBUG
		dumphist("(%d) NetClose", myplr);
#endif
		if(gbMaxPlayers > 1) {
			Sleep(2000);
		}
	}
}

void multi_event_handler(BOOL reg)
{
	int i;
	BOOL (__stdcall *func)(int, void (__stdcall *)(_SNETEVENT *));

	if(reg) {
		func = SNetRegisterEventHandler;
	} else {
		func = SNetUnregisterEventHandler;
	}

	for(i = 0; (DWORD)i < 3; i++) {
		if(!func(event_types[i], multi_handle_events) && reg) {
			app_fatal("SNetRegisterEventHandler:\n%s", TraceLastError());
		}
	}
}

void __stdcall multi_handle_events(_SNETEVENT *pEvt)
{
	DWORD reason;
	_gamedata *pGame;

	switch(pEvt->eventid) {
	case EVENT_TYPE_PLAYER_CREATE_GAME:
		/// ASSERT: assert(pEvt->data);
		/// ASSERT: assert(pEvt->databytes >= sizeof(DWORD));
		pGame = (_gamedata *)pEvt->data;
		sgGameInitInfo.dwSeed = pGame->dwSeed;
		sgGameInitInfo.bDiff = pGame->bDiff;
		sgbPlayerTurnBitTbl[pEvt->playerid] = 1;
		break;
	case EVENT_TYPE_PLAYER_LEAVE_GAME:
		/// ASSERT: assert(pEvt->playerid >= 0 && pEvt->playerid < MAX_PLRS);
		sgbPlayerLeftGameTbl[pEvt->playerid] = 1;
		sgbPlayerTurnBitTbl[pEvt->playerid] = 0;
		reason = 0;
		if(pEvt->data != NULL && pEvt->databytes >= 4) {
			reason = ((DWORD *)pEvt->data)[0];
		}
		sgdwPlayerLeftReasonTbl[pEvt->playerid] = reason;
		if(reason == 0x40000004) {
			gbSomebodyWonGameKludge = 1;
		}
		sgbSendDeltaTbl[pEvt->playerid] = 0;
		dthread_remove_player(pEvt->playerid);
		if(gbDeltaSender == pEvt->playerid) {
			gbDeltaSender = 4;
		}
#ifdef _DEBUG
		dumphist("(%d) callback: player %d left game due to %d", myplr, pEvt->playerid, reason);
#endif
		break;
	case EVENT_TYPE_PLAYER_MESSAGE:
		/// ASSERT: assert(pEvt->data);
		ErrorPlrMsg((char *)pEvt->data);
		break;
	}
}

BOOL NetInit(BOOL bSinglePlayer, BOOL *pfExitProgram)
{
	int i;
	DWORD dwBytes;
	_SNETPROGRAMDATA progdata;
	_SNETPLAYERDATA plrdata;
	_SNETUIDATA uidata;

	while(1) {
		/// ASSERT: assert(pfExitProgram);
		*pfExitProgram = FALSE;
		SetRndSeed(0);
		sgGameInitInfo.dwSeed = time(NULL);
		sgGameInitInfo.bDiff = gnDifficulty;
		memset(&progdata, 0, sizeof(progdata));
		progdata.size = sizeof(progdata);
		progdata.programname = "Diablo Retail";
		progdata.programdescription = gszVersionNumber;
		progdata.programid = 'DRTL';
		progdata.versionid = 42;
		progdata.maxplayers = MAX_PLRS;
		progdata.initdata = &sgGameInitInfo;
		progdata.initdatabytes = sizeof(sgGameInitInfo);
		progdata.optcategorybits = 15;
		progdata.lcid = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);
		memset(&plrdata, 0, sizeof(plrdata));
		plrdata.size = sizeof(plrdata);
		memset(&uidata, 0, sizeof(uidata));
		uidata.size = sizeof(uidata);
		uidata.parentwindow = SDrawGetFrameWindow(0);
		uidata.artcallback = (void (*)())UiArtCallback;
		uidata.createcallback = (void (*)())UiCreateGameCallback;
		uidata.drawdesccallback = (void (*)())UiDrawDescCallback;
		uidata.messageboxcallback = (void (*)())UiMessageBoxCallback;
		uidata.soundcallback = (void (*)())UiSoundCallback;
		uidata.authcallback = (void (*)())UiAuthCallback;
		uidata.getdatacallback = (void (*)())UiGetDataCallback;
		uidata.categorycallback = (void (*)())UiCategoryCallback;
		uidata.selectnamecallback = (void (*)())mainmenu_select_hero_dialog;
		uidata.changenamecallback = (void (*)())mainmenu_create_hero;
		uidata.profilebitmapcallback = (void (*)())UiProfileDraw;
		uidata.profilecallback = (void (*)())UiProfileCallback;
		uidata.profilefields = UiProfileGetString();
		memset(sgbPlayerTurnBitTbl, 0, sizeof(sgbPlayerTurnBitTbl));
		gbGameDestroyed = 0;
		memset(sgbPlayerLeftGameTbl, 0, sizeof(sgbPlayerLeftGameTbl));
		memset(sgdwPlayerLeftReasonTbl, 0, sizeof(sgdwPlayerLeftReasonTbl));
		memset(sgbSendDeltaTbl, 0, sizeof(sgbSendDeltaTbl));
		memset(plr, 0, sizeof(plr));
		memset(sgwPackPlrOffsetTbl, 0, sizeof(sgwPackPlrOffsetTbl));
		SNetSetBasePlayer(0);
		if(bSinglePlayer) {
			if(!multi_init_single(&progdata, &plrdata, &uidata)) {
				return FALSE;
			}
		} else {
			if(!multi_init_multi(&progdata, &plrdata, &uidata, pfExitProgram)) {
				return FALSE;
			}
		}
#ifdef _DEBUG
		gdwHistTicks = GetTickCount();
		dumphist("(%d) new game started", myplr);
#endif
		sgbNetInited = 1;
		sgbTimeout = 0;
		delta_init();
		InitPlrMsg();
		buffer_init(&sgHiPriBuf);
		buffer_init(&sgLoPriBuf);
		dword_678628 = 0;
		sync_init();
		nthread_start(sgbPlayerTurnBitTbl[myplr]);
		dthread_start();
		tmsg_start();
		sgdwGameLoops = 0;
		sgbSentThisCycle = 0;
		gbDeltaSender = myplr;
		gbSomebodyWonGameKludge = 0;
		nthread_send_and_recv_turn(0, 0);
		SetupLocalCoords();
		multi_send_pinfo(SNPLAYER_OTHERS, CMD_SEND_PLRINFO);
		plr[myplr].plractive = 1;
		gbActivePlayers = 1;
		if(sgbPlayerTurnBitTbl[myplr] == 0 || msg_wait_resync()) {
			break;
		}
		NetClose();
		gbSelectProvider = 0;
	}

	gnDifficulty = sgGameInitInfo.bDiff;
	SetRndSeed(sgGameInitInfo.dwSeed);

	for(i = 0; i < NUMLEVELS; i++) {
		glSeedTbl[i] = GetRndSeed();
		gnLevelTypeTbl[i] = InitLevelType(i);
	}

	if(!SNetGetGameInfo(GAMEINFO_NAME, szPlayerName, sizeof(szPlayerName), &dwBytes)) {
		nthread_terminate_game("SNetGetGameInfo1");
	}
	if(!SNetGetGameInfo(GAMEINFO_PASSWORD, szPlayerDescript, sizeof(szPlayerDescript), &dwBytes)) {
		nthread_terminate_game("SNetGetGameInfo2");
	}

	return TRUE;
}

void buffer_init(TBuffer *pBuf)
{
	pBuf->dwNextWriteOffset = 0;
	pBuf->bData[0] = 0;
}

void multi_send_pinfo(int pnum, BYTE cmd)
{
	PkPlayerStruct pkplr;

	PackPlayer(&pkplr, myplr, TRUE);
	dthread_send_delta(pnum, cmd, (BYTE *)&pkplr, sizeof(pkplr));
}

int InitLevelType(int l)
{
	if(l == 0) {
		return DTYPE_TOWN;
	}
	if(l >= 1 && l <= 4) {
		return DTYPE_CATHEDRAL;
	}
	if(l >= 5 && l <= 8) {
		return DTYPE_CATACOMBS;
	}
	if(l >= 9 && l <= 12) {
		return DTYPE_CAVES;
	}

	return DTYPE_HELL;
}

void SetupLocalCoords()
{
	int x, y;

	if(!leveldebug || gbMaxPlayers > 1) {
		currlevel = 0;
		leveltype = 0;
		setlevel = 0;
	}

	x = 75;
	y = 68;
#ifdef _DEBUG
	if(debug_mode_key_inverted_v || debug_mode_key_d) {
		x = 49;
		y = 23;
	}
#endif
	x += plrxoff[myplr];
	y += plryoff[myplr];

	plr[myplr].WorldX = x;
	plr[myplr].WorldY = y;
	plr[myplr]._px = x;
	plr[myplr]._py = y;
	plr[myplr]._ptargx = x;
	plr[myplr]._ptargy = y;
	plr[myplr].plrlevel = currlevel;
	plr[myplr]._pLvlChanging = 1;
	plr[myplr].pLvlLoad = 0;
	plr[myplr]._pmode = PM_NEWLVL;
	plr[myplr].destAction = -1;
}

BOOL multi_init_single(_SNETPROGRAMDATA *progdata, _SNETPLAYERDATA *plrdata, _SNETUIDATA *uidata)
{
	DWORD dwID;

	if(!SNetInitializeProvider(0, progdata, plrdata, uidata, &fileinfo)) {
		SErrGetLastError();
		return FALSE;
	}

	dwID = 0;
	if(!SNetCreateGame("local", "local", "local", 0, &sgGameInitInfo, sizeof(sgGameInitInfo), 1, "local", "local", &dwID)) {
		app_fatal("SNetCreateGame1:\n%s", TraceLastError());
	}
	/// ASSERT: assert(dwID == 0);

	myplr = 0;
	gbMaxPlayers = 1;
	return TRUE;
}

BOOL multi_init_multi(_SNETPROGRAMDATA *progdata, _SNETPLAYERDATA *plrdata, _SNETUIDATA *uidata, BOOL *pfExitProgram)
{
	DWORD dwProvider, dwID;
	BOOL fUpgrade;

	fUpgrade = TRUE;
	while(1) {
		dwProvider = 0;
		if(gbSelectProvider) {
			if(!UiSelectProvider(0, progdata, plrdata, uidata, &fileinfo, &dwProvider)) {
				if(!fUpgrade) {
					return FALSE;
				}
				if(SErrGetLastError() != STORM_ERROR_REQUIRES_UPGRADE) {
					return FALSE;
				}
				if(!multi_upgrade(pfExitProgram)) {
					return FALSE;
				}
			}
			if(dwProvider == 'BNET') {
				plr[0].pBattleNet = 1;
			}
		}
		multi_event_handler(TRUE);
		if(UiSelectGame(1, progdata, plrdata, uidata, &fileinfo, &dwID)) {
			break;
		}
		/// ASSERT: assert(! *pfExitProgram);
		gbSelectProvider = 1;
		fUpgrade = FALSE;
	}

	if(dwID >= MAX_PLRS) {
		return FALSE;
	} else {
		myplr = dwID;
		gbMaxPlayers = MAX_PLRS;
		pfile_read_player_from_save();
		if(dwProvider == 'BNET') {
			plr[myplr].pBattleNet = 1;
		}
		return TRUE;
	}
}

BOOL multi_upgrade(BOOL *pfExitProgram)
{
	DWORD dwStatus;

	SNetPerformUpgrade(&dwStatus);

	switch(dwStatus) {
	case 0xFFFFFFFF:
		DrawDlg("Network upgrade failed");
		break;
	case 0:
		return TRUE;
	case 1:
		return TRUE;
	case 2:
		*pfExitProgram = TRUE;
		break;
	}

	return FALSE;
}

void recv_plrinfo(int pnum, TCmdPlrInfoHdr *p, BOOL recv)
{
	char *szEvent;

	if(myplr == pnum) {
		return;
	}
	/// ASSERT: assert((DWORD)pnum < MAX_PLRS);

	if(sgwPackPlrOffsetTbl[pnum] != p->wOffset) {
		sgwPackPlrOffsetTbl[pnum] = 0;
		if(p->wOffset != 0) {
			return;
		}
	}
	if(!recv && sgwPackPlrOffsetTbl[pnum] == 0) {
		multi_send_pinfo(pnum, CMD_ACK_PLRINFO);
	}

	memcpy((char *)&netplr[pnum] + p->wOffset, &p[1], p->wBytes); /* todo: cast? */
	sgwPackPlrOffsetTbl[pnum] += p->wBytes;
	if(sgwPackPlrOffsetTbl[pnum] != sizeof(*netplr)) {
		return;
	}

	sgwPackPlrOffsetTbl[pnum] = 0;
	multi_player_left_msg(pnum, 0);
	plr[pnum]._pGFXLoad = 0;
	UnPackPlayer(&netplr[pnum], pnum, 1);

	if(!recv) {
#ifdef _DEBUG
		dumphist("(%d) received all %d plrinfo", myplr, pnum);
#endif
		return;
	}

	plr[pnum].plractive = 1;
	gbActivePlayers++;

	if(sgbPlayerTurnBitTbl[pnum] != 0) {
		szEvent = "Player '%s' (level %d) just joined the game";
	} else {
		szEvent = "Player '%s' (level %d) is already in the game";
	}
	EventPlrMsg(szEvent, plr[pnum]._pName, plr[pnum]._pLevel);

	LoadPlrGFX(pnum, 1);
	SyncInitPlr(pnum);

	if(plr[pnum].plrlevel == currlevel) {
		if(plr[pnum]._pHitPoints >> 6 > 0) {
			StartStand(pnum, 0);
		} else {
			plr[pnum]._pgfxnum = 0;
			LoadPlrGFX(pnum, 128);
			plr[pnum]._pmode = PM_DEATH;
			NewPlrAnim(pnum, plr[pnum]._pDAnim[0], plr[pnum]._pDFrames, 1, plr[pnum]._pDWidth);
			plr[pnum]._pAnimFrame = plr[pnum]._pAnimLen - 1;
			plr[pnum]._pVar8 = 2 * plr[pnum]._pAnimLen;
			dFlags[plr[pnum].WorldX][plr[pnum].WorldY] |= 4;
		}
	}
#ifdef _DEBUG
	dumphist("(%d) making %d active -- recv_plrinfo", myplr, pnum);
#endif
}
