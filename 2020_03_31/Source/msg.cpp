#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"
#include "../DiabloUI/diabloui.h"

int sgdwOwnerWait; // weak
int sgdwRecvOffset; // idb
int sgnCurrMegaPlayer; // weak
DLevel sgLevels[NUMLEVELS];
TMegaPkt *sgpCurrPkt;
BYTE sgRecvBuf[4722];
unsigned char sgbRecvCmd; // idb
LocalLevel sgLocals[NUMLEVELS];
DJunk sgJunk;
TMegaPkt *sgpMegaPkt;
char sgbDeltaChanged; // weak
BYTE sgbDeltaChunks; // weak
int deltaload; // weak
char gbBufferMsgs; // weak
int dwRecCount; // weak
int msg_err_timer; // weak

void msg_send_drop_pkt(int pnum, DWORD reason)
{
	TFakeDropPlr cmd;

	cmd.bCmd = FAKE_CMD_DROPID;
	cmd.bPlr = pnum;
	cmd.dwReason = reason;
	msg_send_packet(pnum, &cmd, sizeof(cmd));
}

void msg_send_packet(int pnum, const void *pMsg, DWORD dwLen)
{
	TFakeCmdPlr cmd;

	/// ASSERT: assert((DWORD) pnum < MAX_PLRS);
	/// ASSERT: assert(pMsg);
	/// ASSERT: assert(dwLen <= gdwLargestMsgSize);
	if(pnum != sgnCurrMegaPlayer) {
		sgnCurrMegaPlayer = pnum;
		cmd.bCmd = FAKE_CMD_SETID;
		cmd.bPlr = pnum;
		msg_send_packet(pnum, &cmd, sizeof(cmd));
	}

	/// ASSERT: assert(sgpCurrPkt);
	if(sgpCurrPkt->dwSpaceLeft < dwLen) {
		msg_get_next_packet();
		/// ASSERT: assert(sgpCurrPkt->dwSpaceLeft >= dwLen);
	}

	memcpy((char *)&sgpCurrPkt[1] - sgpCurrPkt->dwSpaceLeft, pMsg, dwLen);
	sgpCurrPkt->dwSpaceLeft -= dwLen;
}

void msg_get_next_packet()
{
	TMegaPkt *p;

	sgpCurrPkt = (TMegaPkt *)DiabloAllocPtr(sizeof(*sgpCurrPkt));
	sgpCurrPkt->pNext = NULL;
	sgpCurrPkt->dwSpaceLeft = sizeof(sgpCurrPkt->data);

	p = (TMegaPkt *)&sgpMegaPkt;
	while(p->pNext != NULL) {
		p = p->pNext;
	}
	p->pNext = sgpCurrPkt;
}

BOOL msg_wait_resync()
{
	BOOL bSuccess;

	/// ASSERT: assert(ghMainWnd);
	/// ASSERT: assert(! sgpMegaPkt);
	/// ASSERT: assert(! sgpCurrPkt);

	msg_get_next_packet();
	sgbDeltaChunks = 0;
	sgnCurrMegaPlayer = -1;
	sgbRecvCmd = CMD_DLEVEL_END;
	gbBufferMsgs = 1;
	sgdwOwnerWait = GetTickCount();
	bSuccess = UiProgressDialog(ghMainWnd, "Waiting for game data...", 1, msg_wait_for_turns, 20);
	gbBufferMsgs = 0;

	if(!bSuccess) {
		msg_free_packets();
		return FALSE;
	}
	if(gbGameDestroyed) {
		DrawDlg("The game ended");
		msg_free_packets();
		return FALSE;
	}
	if(sgbDeltaChunks != 21) {
		DrawDlg("Unable to get level data");
		msg_free_packets();
		return FALSE;
	}

	return TRUE;
}

void msg_free_packets()
{
	while(sgpMegaPkt != NULL) {
		sgpCurrPkt = sgpMegaPkt->pNext;
		MemFreeDbg(sgpMegaPkt);
		sgpMegaPkt = sgpCurrPkt;
	}
}

int msg_wait_for_turns()
{
	DWORD dwTurns;
	BOOL fSendAsync;

	if(sgbDeltaChunks == 0) {
		nthread_send_and_recv_turn(0, 0);
		if(!SNetGetOwnerTurnsWaiting(&dwTurns) && SErrGetLastError() == STORM_ERROR_NOT_IN_GAME) {
			return 100;
		}
		if(GetTickCount() - sgdwOwnerWait <= 2000 && dwTurns < gdwTurnsInTransit) {
			return 0;
		}
		sgbDeltaChunks++;
	}

	multi_process_network_packets();
	nthread_send_and_recv_turn(0, 0);

	if(nthread_has_500ms_passed(FALSE)) {
		nthread_recv_turns(&fSendAsync);
	}
	if(gbGameDestroyed) {
		return 100;
	}
	if(gbDeltaSender >= 4) {
		sgbDeltaChunks = 0;
		sgbRecvCmd = CMD_DLEVEL_END;
		gbDeltaSender = myplr;
		nthread_set_turn_upper_bit();
	}
	if(sgbDeltaChunks == 20) {
		sgbDeltaChunks++;
		return 99;
	}

	return 100 * sgbDeltaChunks / 21;
}

void run_delta_info()
{
	if(gbMaxPlayers == 1) {
		return;
	}

	/// ASSERT: assert(currlevel == 0);
	/// ASSERT: assert(plr[myplr].plrlevel == 0);

	gbBufferMsgs = 2;
	msg_pre_packet();
	gbBufferMsgs = 0;
	msg_free_packets();
}

void msg_pre_packet()
{
	int nPlayer;
	DWORD dwLen, dwSize;
	TMegaPkt *p;
	BYTE *pbMsg;
	TFakeCmdPlr *pFake;
	TFakeDropPlr *pDrop;
	TCmd *pCmd;

	nPlayer = -1;
	for(p = sgpMegaPkt; p != NULL; p = p->pNext) {
		pbMsg = p->data;
		dwLen = sizeof(p->data);
		while(dwLen != p->dwSpaceLeft) {
			if(*pbMsg == FAKE_CMD_SETID) {
				pFake = (TFakeCmdPlr *)pbMsg;
				pbMsg += sizeof(*pFake);
				dwLen -= sizeof(*pFake);
				nPlayer = pFake->bPlr;
			} else if(*pbMsg == FAKE_CMD_DROPID) {
				pDrop = (TFakeDropPlr *)pbMsg;
				pbMsg += sizeof(*pDrop);
				dwLen -= sizeof(*pDrop);
				multi_player_left(pDrop->bPlr, pDrop->dwReason);
			} else {
				/// ASSERT: assert((DWORD) nPlayer < MAX_PLRS);
				pCmd = (TCmd *)pbMsg;
				dwSize = ParseCmd(nPlayer, pCmd);
				pbMsg += dwSize;
				dwLen -= dwSize;
			}
		}
	}
}

void DeltaExportData(int pnum)
{
	int i;
	DWORD dwLen;
	BYTE bSrc;
	BYTE *p, *dst;

	if(sgbDeltaChanged) {
		p = DiabloAllocPtr(sizeof(*sgLevels) + 1);
		for(i = 0; i < NUMLEVELS; i++) {
			dst = p + 1;
			dst = DeltaExportItem(dst, sgLevels[i].item);
			dst = DeltaExportObject(dst, sgLevels[i].object);
			dst = DeltaExportMonster(dst, sgLevels[i].monster);
			dwLen = msg_comp_level(p, dst);
			dthread_send_delta(pnum, i + CMD_DLEVEL_0, p, dwLen);
		}
		dst = DeltaExportJunk(p + 1);
		dwLen = msg_comp_level(p, dst);
		dthread_send_delta(pnum, CMD_DLEVEL_JUNK, p, dwLen);
		MemFreeDbg(p);
	}

	bSrc = 0;
	dthread_send_delta(pnum, CMD_DLEVEL_END, &bSrc, sizeof(bSrc));
}

BYTE *DeltaExportItem(BYTE *dst, TCmdPItem *src)
{
	int i;

	i = 0;
	while(i < MAXITEMS) {
		if(src->bCmd == 255) {
			*dst = 255;
			dst++;
		} else {
			memcpy(dst, src, sizeof(*src));
			dst += sizeof(*src);
		}
		i++;
		src++;
	}

	return dst;
}

BYTE *DeltaExportObject(BYTE *dst, DObjectStr *src)
{
	memcpy(dst, src, sizeof(*src) * MAXOBJECTS);
	return dst + sizeof(*src) * MAXOBJECTS;
}

BYTE *DeltaExportMonster(BYTE *dst, DMonsterStr *src)
{
	int i;

	i = 0;
	while(i < MAXMONSTERS) {
		if(src->_mx == 255) {
			*dst = 255;
			dst++;
		} else {
			memcpy(dst, src, sizeof(*src));
			dst += sizeof(*src);
		}
		i++;
		src++;
	}

	return dst;
}

BYTE *DeltaExportJunk(BYTE *dst)
{
	int i, deltaq;

	for(i = 0; i < MAXPORTAL; i++) {
		if(sgJunk.portal[i].x == 255) {
			*dst = 255;
			dst++;
		} else {
			memcpy(dst, &sgJunk.portal[i], sizeof(*sgJunk.portal));
			dst += sizeof(*sgJunk.portal);
		}
	}

	deltaq = 0;
	for(i = 0; i < MAXQUESTS; i++) {
		if(questlist[i]._qflags & 1) {
			sgJunk.quests[deltaq].qlog = quests[i]._qlog;
			sgJunk.quests[deltaq].qstate = quests[i]._qactive;
			sgJunk.quests[deltaq].qvar1 = quests[i]._qvar1;
			memcpy(dst, &sgJunk.quests[deltaq], sizeof(*sgJunk.quests));
			dst += sizeof(*sgJunk.quests);
			deltaq++;
		}
	}

	return dst;
}

DWORD msg_comp_level(BYTE *p, BYTE *dst)
{
	DWORD dwSize, dwLen;

	dwSize = dst - p - 1;
	dwLen = PkwareCompress(p + 1, dwSize);
	*p = dwSize != dwLen;
	dwLen++;
	return dwLen;
}

void delta_init()
{
	sgbDeltaChanged = 0;
	memset(&sgJunk, 255, sizeof(sgJunk));
	memset(&sgLevels, 255, sizeof(sgLevels));
	memset(&sgLocals, 0, sizeof(sgLocals));
	deltaload = 0;
}

void delta_kill_monster(int mi, BYTE x, BYTE y, BYTE bLevel)
{
	DMonsterStr *p;

	if(gbMaxPlayers == 1) {
		return;
	}

	/// ASSERT: assert((DWORD)mi < MAXMONSTERS);
	/// ASSERT: assert(x < DMAXX);
	/// ASSERT: assert(y < DMAXY);
	/// ASSERT: assert(bLevel < NUMLEVELS);

	sgbDeltaChanged = 1;
	p = &sgLevels[bLevel].monster[mi];
	p->_mx = x;
	p->_my = y;
	p->_mdir = monster[mi]._mdir;
	p->_mhitpoints = 0;
}

void delta_monster_hp(int mi, long hp, BYTE bLevel)
{
	DMonsterStr *p;

	if(gbMaxPlayers == 1) {
		return;
	}

	/// ASSERT: assert((DWORD)mi < MAXMONSTERS);
	/// ASSERT: assert(bLevel < NUMLEVELS);

	sgbDeltaChanged = 1;
	p = &sgLevels[bLevel].monster[mi];
	if(p->_mhitpoints > hp) {
		p->_mhitpoints = hp;
	}
}

void delta_sync_monster(const TSyncMonster *pSync, BYTE bLevel)
{
	DMonsterStr *pD;

	if(gbMaxPlayers == 1) {
		return;
	}

	/// ASSERT: assert(pSync != NULL);
	/// ASSERT: assert(bLevel < NUMLEVELS);

	sgbDeltaChanged = 1;
	pD = &sgLevels[bLevel].monster[pSync->_mndx];
	if(pD->_mhitpoints != 0) {
		pD->_mx = pSync->_mx;
		pD->_my = pSync->_my;
		pD->_mactive = 255;
		pD->_menemy = pSync->_menemy;
	}
}

void delta_sync_golem(const TCmdGolem *pG, int pnum, BYTE bLevel)
{
	DMonsterStr *pD;

	if(gbMaxPlayers == 1) {
		return;
	}

	/// ASSERT: assert(bLevel < NUMLEVELS);
	/// ASSERT: assert(bLevel != 0);

	sgbDeltaChanged = 1;
	pD = &sgLevels[bLevel].monster[pnum];
	pD->_mx = pG->_mx;
	pD->_my = pG->_my;
	pD->_mactive = 255;
	pD->_menemy = pG->_menemy;
	pD->_mdir = pG->_mdir;
	pD->_mhitpoints = pG->_mhitpoints;
}

void delta_leave_sync(BYTE bLevel)
{
	int i, ii;
	DMonsterStr *pD;

	if(gbMaxPlayers == 1) {
		return;
	}

	if(currlevel == 0) {
		glSeedTbl[0] = GetRndSeed();
	}
	if(currlevel <= 0) {
		return;
	}

	/// ASSERT: assert(bLevel < NUMLEVELS);

	for(i = 0; i < nummonsters; i++) {
		ii = monstactive[i];
		if(monster[ii]._mhitpoints != 0) {
			sgbDeltaChanged = 1;
			pD = &sgLevels[bLevel].monster[ii];
			pD->_mx = monster[ii]._mx;
			pD->_my = monster[ii]._my;
			pD->_mdir = monster[ii]._mdir;
			pD->_menemy = encode_enemy(ii);
			pD->_mhitpoints = monster[ii]._mhitpoints;
			pD->_mactive = monster[ii]._msquelch;
		}
	}

	memcpy(sgLocals[bLevel].automapsv, automapview, sizeof(automapview));
}

BOOL delta_portal_inited(int i)
{
	/// ASSERT: assert((DWORD)i < MAXPORTAL);
	return sgJunk.portal[i].x == 255;
}

BOOL delta_quest_inited(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMULTIQUESTS);
	return sgJunk.quests[i].qstate != 255;
}

void DeltaAddItem(int ii)
{
	int i;
	TCmdPItem *pD;

	if(gbMaxPlayers == 1) {
		return;
	}

	/// ASSERT: assert((DWORD)ii < MAXITEMS);

	pD = sgLevels[currlevel].item;
	i = 0;
	while(i < MAXITEMS) {
		if(pD->bCmd != 255) {
			if(pD->wIndx == item[ii].IDidx
			&& pD->wCI == item[ii]._iCreateInfo
			&& pD->dwSeed == item[ii]._iSeed
			&& (pD->bCmd == CMD_WALKXY || pD->bCmd == CMD_STAND)) {
				return;
			}
		}
		i++;
		pD++;
	}

	pD = sgLevels[currlevel].item;
	i = 0;
	while(i < MAXITEMS) {
		if(pD->bCmd == 255) {
			sgbDeltaChanged = 1;
			pD->bCmd = CMD_STAND;
			pD->x = item[ii]._ix;
			pD->y = item[ii]._iy;
			pD->wIndx = item[ii].IDidx;
			pD->wCI = item[ii]._iCreateInfo;
			pD->dwSeed = item[ii]._iSeed;
			pD->bId = item[ii]._iIdentified;
			pD->bDur = item[ii]._iDurability;
			pD->bMDur = item[ii]._iMaxDur;
			pD->bCh = item[ii]._iCharges;
			pD->bMCh = item[ii]._iMaxCharges;
			pD->wValue = item[ii]._ivalue;
			return;
		}
		i++;
		pD++;
	}
}

void DeltaSaveLevel()
{
	int i;

	if(gbMaxPlayers == 1) {
		return;
	}

	for(i = 0; i < MAX_PLRS; i++) {
		if(i != myplr) {
			plr[i]._pGFXLoad = 0;
		}
	}

	/// ASSERT: assert((DWORD) currlevel < NUMLEVELS);
	plr[myplr]._pLvlVisited[currlevel] = 1;
	delta_leave_sync(currlevel);
}

void DeltaLoadLevel()
{
	int i, ii, ox, oy, xx, iz, yy, j, l;
	BOOL done;

	if(gbMaxPlayers == 1) {
		return;
	}

	deltaload = 1;

	if(currlevel != 0) {
		for(i = 0; i < nummonsters; i++) {
			if(sgLevels[currlevel].monster[i]._mx == 255) {
				continue;
			}
			M_ClearSquares(i);
			monster[i]._mx = sgLevels[currlevel].monster[i]._mx;
			monster[i]._my = sgLevels[currlevel].monster[i]._my;
			monster[i]._moldx = monster[i]._mx;
			monster[i]._moldy = monster[i]._my;
			monster[i]._mfutx = monster[i]._mx;
			monster[i]._mfuty = monster[i]._my;
			if(sgLevels[currlevel].monster[i]._mhitpoints != -1) {
				monster[i]._mhitpoints = sgLevels[currlevel].monster[i]._mhitpoints;
			}
			if(sgLevels[currlevel].monster[i]._mhitpoints == 0) {
				monster[i]._moldx = sgLevels[currlevel].monster[i]._mx; /// BUGFIX: leftover, already set oldx/y above, remove
				monster[i]._moldy = sgLevels[currlevel].monster[i]._my;
				M_ClearSquares(i);
				if(monster[i]._mAi != AI_DIABLO) {
					if(monster[i]._uniqtype == 0) {
						/// ASSERT: assert(monster[i].MType != NULL);
						AddDead(monster[i]._mx, monster[i]._my, monster[i].MType->mdeadval, monster[i]._mdir);
					} else {
						AddDead(monster[i]._mx, monster[i]._my, monster[i]._udeadval, monster[i]._mdir);
					}
				}
				monster[i]._mDelFlag = 1;
				M_UpdateLeader(i);
			} else {
				decode_enemy(i, sgLevels[currlevel].monster[i]._menemy);
				if(monster[i]._mx != 0 && monster[i]._mx != 1 || monster[i]._my != 0) {
					dMonster[monster[i]._mx][monster[i]._my] = i + 1;
				}
				if(i < 4) {
					MAI_Golum(i);
					monster[i]._mFlags |= 0x30;
				} else {
					M_StartStand(i, monster[i]._mdir);
				}
				monster[i]._msquelch = sgLevels[currlevel].monster[i]._mactive;
			}
		}
		memcpy(automapview, sgLocals[currlevel].automapsv, sizeof(automapview));
	}

	for(i = 0; i < MAXITEMS; i++) {
		if(sgLevels[currlevel].item[i].bCmd == 255) {
			continue;
		}
		if(sgLevels[currlevel].item[i].bCmd == CMD_WALKXY) {
			ii = FindGetItem(
				sgLevels[currlevel].item[i].wIndx,
				sgLevels[currlevel].item[i].wCI,
				sgLevels[currlevel].item[i].dwSeed);
			if(ii != -1) {
				if(dItem[item[ii]._ix][item[ii]._iy] == ii + 1) {
					dItem[item[ii]._ix][item[ii]._iy] = 0;
				}
				DeleteItem(ii, i);
			}
		}
		if(sgLevels[currlevel].item[i].bCmd == CMD_ACK_PLRINFO) {
			ii = itemavail[0];
			itemavail[0] = itemavail[MAXITEMS - numitems - 1];
			itemactive[numitems] = ii;
			if(sgLevels[currlevel].item[i].wIndx == IDI_EAR) {
				RecreateEar(
					ii,
					sgLevels[currlevel].item[i].wCI,
					sgLevels[currlevel].item[i].dwSeed,
					sgLevels[currlevel].item[i].bId,
					sgLevels[currlevel].item[i].bDur,
					sgLevels[currlevel].item[i].bMDur,
					sgLevels[currlevel].item[i].bCh,
					sgLevels[currlevel].item[i].bMCh,
					sgLevels[currlevel].item[i].wValue,
					sgLevels[currlevel].item[i].dwBuff);
			} else {
				RecreateItem(
					ii,
					sgLevels[currlevel].item[i].wIndx,
					sgLevels[currlevel].item[i].wCI,
					sgLevels[currlevel].item[i].dwSeed,
					sgLevels[currlevel].item[i].wValue);
				if(sgLevels[currlevel].item[i].bId) {
					item[ii]._iIdentified = 1;
				}
				item[ii]._iDurability = sgLevels[currlevel].item[i].bDur;
				item[ii]._iMaxDur = sgLevels[currlevel].item[i].bMDur;
				item[ii]._iCharges = sgLevels[currlevel].item[i].bCh;
				item[ii]._iMaxCharges = sgLevels[currlevel].item[i].bMCh;
			}
			ox = sgLevels[currlevel].item[i].x;
			oy = sgLevels[currlevel].item[i].y;
			if(!CanPut(ox, oy)) {
				done = FALSE;
				for(l = 1; l < 50 && !done; l++) {
					for(j = -l; j <= l && !done; j++) {
						yy = j + oy;
						for(iz = -l; iz <= l && !done; iz++) {
							xx = iz + ox;
							if(CanPut(xx, yy)) {
								done = TRUE;
								ox = xx;
								oy = yy;
							}
						}
					}
				}
			}
			item[ii]._ix = ox;
			item[ii]._iy = oy;
			dItem[item[ii]._ix][item[ii]._iy] = ii + 1;
			RespawnItem(ii, FALSE);
			numitems++;
		}
	}

	if(currlevel != 0) {
		for(i = 0; i < MAXOBJECTS; i++) {
			switch(sgLevels[currlevel].object[i].bCmd) {
			case 255:
				break;
			case CMD_OPENDOOR:
			case CMD_CLOSEDOOR:
			case CMD_OPERATEOBJ:
			case CMD_PLROPOBJ:
				SyncOpObject(-1, sgLevels[currlevel].object[i].bCmd, i);
				break;
			case CMD_BREAKOBJ:
				SyncBreakObj(-1, i);
				break;
			}
		}
		for(i = 0; i < nobjects; i++) {
			ii = objectactive[i];
			if(object[ii]._otype == OBJ_TRAPL || object[ii]._otype == OBJ_TRAPR) {
				Obj_Trap(ii);
			}
		}
	}

	deltaload = 0;
}

void NetSendCmd(BOOL bHiPri, BYTE bCmd)
{
	TCmd cmd;

	cmd.bCmd = bCmd;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdGolem(BYTE mx, BYTE my, BYTE dir, BYTE menemy, long hp, BYTE cl)
{
	TCmdGolem cmd;

	/// ASSERT: assert(mx < DMAXX);
	/// ASSERT: assert(my < DMAXY);
	cmd.bCmd = CMD_AWAKEGOLEM;
	cmd._mx = mx;
	cmd._my = my;
	cmd._mdir = dir;
	cmd._menemy = menemy;
	cmd._mhitpoints = hp;
	cmd._currlevel = cl;

	NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
}

void NetSendCmdLoc(BOOL bHiPri, BYTE bCmd, BYTE x, BYTE y)
{
	__declspec(align(1)) TCmdLoc cmd;

	/// ASSERT: assert(x < DMAXX);
	/// ASSERT: assert(y < DMAXY);
	cmd.bCmd = bCmd;
	cmd.x = x;
	cmd.y = y;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdLocParam1(BOOL bHiPri, BYTE bCmd, BYTE x, BYTE y, WORD wParam1)
{
	TCmdLocParam1 cmd;

	/// ASSERT: assert(x < DMAXX);
	/// ASSERT: assert(y < DMAXY);
	cmd.bCmd = bCmd;
	cmd.x = x;
	cmd.y = y;
	cmd.wParam1 = wParam1;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdLocParam2(BOOL bHiPri, BYTE bCmd, BYTE x, BYTE y, WORD wParam1, WORD wParam2)
{
	TCmdLocParam2 cmd;

	/// ASSERT: assert(x < DMAXX);
	/// ASSERT: assert(y < DMAXY);
	cmd.bCmd = bCmd;
	cmd.x = x;
	cmd.y = y;
	cmd.wParam1 = wParam1;
	cmd.wParam2 = wParam2;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdLocParam3(BOOL bHiPri, BYTE bCmd, BYTE x, BYTE y, WORD wParam1, WORD wParam2, WORD wParam3)
{
	TCmdLocParam3 cmd;

	/// ASSERT: assert(x < DMAXX);
	/// ASSERT: assert(y < DMAXY);
	cmd.bCmd = bCmd;
	cmd.x = x;
	cmd.y = y;
	cmd.wParam1 = wParam1;
	cmd.wParam2 = wParam2;
	cmd.wParam3 = wParam3;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdParam1(BOOL bHiPri, BYTE bCmd, WORD wParam1)
{
	__declspec(align(1)) TCmdParam1 cmd;

	cmd.bCmd = bCmd;
	cmd.wParam1 = wParam1;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdParam2(BOOL bHiPri, BYTE bCmd, WORD wParam1, WORD wParam2)
{
	TCmdParam2 cmd;

	cmd.bCmd = bCmd;
	cmd.wParam1 = wParam1;
	cmd.wParam2 = wParam2;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdParam3(BOOL bHiPri, BYTE bCmd, WORD wParam1, WORD wParam2, WORD wParam3)
{
	TCmdParam3 cmd;

	cmd.bCmd = bCmd;
	cmd.wParam1 = wParam1;
	cmd.wParam2 = wParam2;
	cmd.wParam3 = wParam3;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdQuest(BOOL bHiPri, BYTE q)
{
	TCmdQuest cmd;

	cmd.bCmd = CMD_SYNCQUEST;
	cmd.q = q;
	cmd.qstate = quests[q]._qactive;
	cmd.qlog = quests[q]._qlog;
	cmd.qvar1 = quests[q]._qvar1;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdGItem(BOOL bHiPri, BYTE bCmd, BYTE mast, BYTE pnum, BYTE ii)
{
	TCmdGItem cmd;

	/// ASSERT: assert(ii < MAXITEMS);
	/// ASSERT: assert(pnum < MAX_PLRS);
	cmd.bCmd = bCmd;
	cmd.bPnum = pnum;
	cmd.bMaster = mast;
	cmd.bLevel = currlevel;
	cmd.bCursitem = ii;
	cmd.dwTime = 0;
	cmd.x = item[ii]._ix;
	cmd.y = item[ii]._iy;
	cmd.wIndx = item[ii].IDidx;

	if(item[ii].IDidx == IDI_EAR) {
		cmd.wCI = item[ii]._iName[7] << 8 | item[ii]._iName[8];
		cmd.dwSeed = item[ii]._iName[9] << 24 | item[ii]._iName[10] << 16 | item[ii]._iName[11] << 8 | item[ii]._iName[12];
		cmd.bId = item[ii]._iName[13];
		cmd.bDur = item[ii]._iName[14];
		cmd.bMDur = item[ii]._iName[15];
		cmd.bCh = item[ii]._iName[16];
		cmd.bMCh = item[ii]._iName[17];
		cmd.wValue = item[ii]._ivalue | item[ii]._iName[18] << 8 | ((item[ii]._iCurs - 19) << 6);
		cmd.dwBuff = item[ii]._iName[19] << 24 | item[ii]._iName[20] << 16 | item[ii]._iName[21] << 8 | item[ii]._iName[22];
	} else {
		cmd.wCI = item[ii]._iCreateInfo;
		cmd.dwSeed = item[ii]._iSeed;
		cmd.bId = item[ii]._iIdentified;
		cmd.bDur = item[ii]._iDurability;
		cmd.bMDur = item[ii]._iMaxDur;
		cmd.bCh = item[ii]._iCharges;
		cmd.bMCh = item[ii]._iMaxCharges;
		cmd.wValue = item[ii]._ivalue;
	}

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdGItem2(BOOL usonly, BYTE bCmd, BYTE mast, BYTE pnum, const TCmdGItem *p)
{
	int nTicks;
	TCmdGItem cmd;

	/// ASSERT: assert(pnum < MAX_PLRS);
	/// ASSERT: assert(p != NULL);
	memcpy(&cmd, p, sizeof(cmd));
	cmd.bCmd = bCmd;
	cmd.bPnum = pnum;
	cmd.bMaster = mast;

	if(!usonly) {
		cmd.dwTime = 0;
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
		return;
	}

	nTicks = GetTickCount();
	if(cmd.dwTime == 0) {
		cmd.dwTime = nTicks;
	} else if(nTicks - cmd.dwTime > 5000) {
		return;
	}

	multi_msg_add((BYTE *)&cmd, sizeof(cmd));
}

BOOL NetSendCmdReq2(BYTE bCmd, BYTE mast, BYTE pnum, const TCmdGItem *p)
{
	int nTicks;
	TCmdGItem cmd;

	/// ASSERT: assert(pnum < MAX_PLRS);
	/// ASSERT: assert(p != NULL);
	memcpy(&cmd, p, sizeof(cmd));
	cmd.bCmd = bCmd;
	cmd.bPnum = pnum;
	cmd.bMaster = mast;

	nTicks = GetTickCount();
	if(cmd.dwTime == 0) {
		cmd.dwTime = nTicks;
	} else if(nTicks - cmd.dwTime > 5000) {
		return FALSE;
	}

	multi_msg_add((BYTE *)&cmd, sizeof(cmd));
	return TRUE;
}

void NetSendCmdExtra(const TCmdGItem *p)
{
	TCmdGItem cmd;

	/// ASSERT: assert(p != NULL);
	memcpy(&cmd, p, sizeof(cmd));
	cmd.bCmd = CMD_ITEMEXTRA;
	cmd.dwTime = 0;

	NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
}

void NetSendCmdPItem(BOOL bHiPri, BYTE bCmd, BYTE x, BYTE y)
{
	TCmdPItem cmd;

	/// ASSERT: assert(x < DMAXX);
	/// ASSERT: assert(y < DMAXY);
	cmd.bCmd = bCmd;
	cmd.x = x;
	cmd.y = y;
	cmd.wIndx = plr[myplr].HoldItem.IDidx;

	if(plr[myplr].HoldItem.IDidx == IDI_EAR) {
		cmd.wCI = plr[myplr].HoldItem._iName[7] << 8 | plr[myplr].HoldItem._iName[8];
		cmd.dwSeed = plr[myplr].HoldItem._iName[9] << 24
			| plr[myplr].HoldItem._iName[10] << 16
			| plr[myplr].HoldItem._iName[11] << 8
			| plr[myplr].HoldItem._iName[12];
		cmd.bId = plr[myplr].HoldItem._iName[13];
		cmd.bDur = plr[myplr].HoldItem._iName[14];
		cmd.bMDur = plr[myplr].HoldItem._iName[15];
		cmd.bCh = plr[myplr].HoldItem._iName[16];
		cmd.bMCh = plr[myplr].HoldItem._iName[17];
		cmd.wValue = plr[myplr].HoldItem._ivalue | plr[myplr].HoldItem._iName[18] << 8 | ((plr[myplr].HoldItem._iCurs - 19) << 6);
		cmd.dwBuff = plr[myplr].HoldItem._iName[19] << 24
			| plr[myplr].HoldItem._iName[20] << 16
			| plr[myplr].HoldItem._iName[21] << 8
			| plr[myplr].HoldItem._iName[22];
	} else {
		cmd.wCI = plr[myplr].HoldItem._iCreateInfo;
		cmd.dwSeed = plr[myplr].HoldItem._iSeed;
		cmd.bId = plr[myplr].HoldItem._iIdentified;
		cmd.bDur = plr[myplr].HoldItem._iDurability;
		cmd.bMDur = plr[myplr].HoldItem._iMaxDur;
		cmd.bCh = plr[myplr].HoldItem._iCharges;
		cmd.bMCh = plr[myplr].HoldItem._iMaxCharges;
		cmd.wValue = plr[myplr].HoldItem._ivalue;
	}

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdChItem(BOOL bHiPri, BYTE bLoc)
{
	TCmdChItem cmd;

	cmd.bCmd = CMD_CHANGEPLRITEMS;
	cmd.bLoc = bLoc;
	cmd.wIndx = plr[myplr].HoldItem.IDidx;
	cmd.wCI = plr[myplr].HoldItem._iCreateInfo;
	cmd.dwSeed = plr[myplr].HoldItem._iSeed;
	cmd.bId = plr[myplr].HoldItem._iIdentified;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdDelItem(BOOL bHiPri, BYTE bLoc)
{
	TCmdDelItem cmd;

	cmd.bCmd = CMD_DELPLRITEMS;
	cmd.bLoc = bLoc;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdDItem(BOOL bHiPri, int ii)
{
	TCmdPItem cmd;

	/// ASSERT: assert((DWORD)ii < MAXITEMS);
	cmd.bCmd = CMD_DROPITEM;
	cmd.x = item[ii]._ix;
	cmd.y = item[ii]._iy;
	cmd.wIndx = item[ii].IDidx;

	if(item[ii].IDidx == IDI_EAR) {
		cmd.wCI = item[ii]._iName[7] << 8 | item[ii]._iName[8];
		cmd.dwSeed = item[ii]._iName[9] << 24 | item[ii]._iName[10] << 16 | item[ii]._iName[11] << 8 | item[ii]._iName[12];
		cmd.bId = item[ii]._iName[13];
		cmd.bDur = item[ii]._iName[14];
		cmd.bMDur = item[ii]._iName[15];
		cmd.bCh = item[ii]._iName[16];
		cmd.bMCh = item[ii]._iName[17];
		cmd.wValue = item[ii]._ivalue | item[ii]._iName[18] << 8 | ((item[ii]._iCurs - 19) << 6);
		cmd.dwBuff = item[ii]._iName[19] << 24 | item[ii]._iName[20] << 16 | item[ii]._iName[21] << 8 | item[ii]._iName[22];
	} else {
		cmd.wCI = item[ii]._iCreateInfo;
		cmd.dwSeed = item[ii]._iSeed;
		cmd.bId = item[ii]._iIdentified;
		cmd.bDur = item[ii]._iDurability;
		cmd.bMDur = item[ii]._iMaxDur;
		cmd.bCh = item[ii]._iCharges;
		cmd.bMCh = item[ii]._iMaxCharges;
		cmd.wValue = item[ii]._ivalue;
	}

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdDamage(BOOL bHiPri, BYTE bPlr, DWORD dwDam)
{
	TCmdDamage cmd;

	/// ASSERT: assert(bPlr < MAX_PLRS);
	cmd.bCmd = CMD_PLRDAMAGE;
	cmd.bPlr = bPlr;
	cmd.dwDam = dwDam;

	if(bHiPri) {
		NetSendHiPri((BYTE *)&cmd, sizeof(cmd));
	} else {
		NetSendLoPri((BYTE *)&cmd, sizeof(cmd));
	}
}

void NetSendCmdString(int msk, const char *pszStr)
{
	DWORD dwStrLen;
	TCmdString cmd;

	/// ASSERT: assert(pszStr);
	dwStrLen = strlen(pszStr);
	/// ASSERT: assert(dwStrLen < sizeof(cmd.str));
	cmd.bCmd = CMD_STRING;
	strcpy(cmd.str, pszStr);

	multi_send_msg_packet(msk, (BYTE *)&cmd, dwStrLen + 2);
}

void delta_close_portal(int pnum)
{
	/// ASSERT: assert((DWORD)pnum < MAX_PLRS);
	memset(&sgJunk.portal[pnum], 255, sizeof(sgJunk.portal[pnum]));
	sgbDeltaChanged = 1;
}

DWORD ParseCmd(int pnum, const TCmd *pCmd)
{
	static BYTE sbLastCmd;

	/// ASSERT: assert((DWORD)pnum < MAX_PLRS);
	/// ASSERT: assert(pCmd != NULL);

	sbLastCmd = pCmd->bCmd;

	if(sgwPackPlrOffsetTbl[pnum] != 0 && sbLastCmd != CMD_ACK_PLRINFO && sbLastCmd != CMD_SEND_PLRINFO) {
		return 0;
	}

	switch(pCmd->bCmd) {
	case CMD_SYNCDATA:
		return On_SYNCDATA(pCmd, pnum);
	case CMD_WALKXY:
		return On_WALKXY(pCmd, pnum);
	case CMD_ADDSTR:
		return On_ADDSTR(pCmd, pnum);
	case CMD_ADDDEX:
		return On_ADDDEX(pCmd, pnum);
	case CMD_ADDMAG:
		return On_ADDMAG(pCmd, pnum);
	case CMD_ADDVIT:
		return On_ADDVIT(pCmd, pnum);
	case CMD_SBSPELL:
		return On_SBSPELL(pCmd, pnum);
	case CMD_GOTOGETITEM:
		return On_GOTOGETITEM(pCmd, pnum);
	case CMD_REQUESTGITEM:
		return On_REQUESTGITEM(pCmd, pnum);
	case CMD_GETITEM:
		return On_GETITEM(pCmd, pnum);
	case CMD_GOTOAGETITEM:
		return On_GOTOAGETITEM(pCmd, pnum);
	case CMD_REQUESTAGITEM:
		return On_REQUESTAGITEM(pCmd, pnum);
	case CMD_AGETITEM:
		return On_AGETITEM(pCmd, pnum);
	case CMD_ITEMEXTRA:
		return On_ITEMEXTRA(pCmd, pnum);
	case CMD_PUTITEM:
		return On_PUTITEM(pCmd, pnum);
	case CMD_SYNCPUTITEM:
		return On_SYNCPUTITEM(pCmd, pnum);
	case CMD_RESPAWNITEM:
		return On_RESPAWNITEM(pCmd, pnum);
	case CMD_ATTACKXY:
		return On_ATTACKXY(pCmd, pnum);
	case CMD_SATTACKXY:
		return On_SATTACKXY(pCmd, pnum);
	case CMD_RATTACKXY:
		return On_RATTACKXY(pCmd, pnum);
	case CMD_SPELLXYD:
		return On_SPELLXYD(pCmd, pnum);
	case CMD_SPELLXY:
		return On_SPELLXY(pCmd, pnum);
	case CMD_TSPELLXY:
		return On_TSPELLXY(pCmd, pnum);
	case CMD_OPOBJXY:
		return On_OPOBJXY(pCmd, pnum);
	case CMD_DISARMXY:
		return On_DISARMXY(pCmd, pnum);
	case CMD_OPOBJT:
		return On_OPOBJT(pCmd, pnum);
	case CMD_ATTACKID:
		return On_ATTACKID(pCmd, pnum);
	case CMD_ATTACKPID:
		return On_ATTACKPID(pCmd, pnum);
	case CMD_RATTACKID:
		return On_RATTACKID(pCmd, pnum);
	case CMD_RATTACKPID:
		return On_RATTACKPID(pCmd, pnum);
	case CMD_SPELLID:
		return On_SPELLID(pCmd, pnum);
	case CMD_SPELLPID:
		return On_SPELLPID(pCmd, pnum);
	case CMD_TSPELLID:
		return On_TSPELLID(pCmd, pnum);
	case CMD_TSPELLPID:
		return On_TSPELLPID(pCmd, pnum);
	case CMD_KNOCKBACK:
		return On_KNOCKBACK(pCmd, pnum);
	case CMD_RESURRECT:
		return On_RESURRECT(pCmd, pnum);
	case CMD_HEALOTHER:
		return On_HEALOTHER(pCmd, pnum);
	case CMD_TALKXY:
		return On_TALKXY(pCmd, pnum);
	case CMD_DEBUG:
		return On_DEBUG(pCmd, pnum);
	case CMD_NEWLVL:
		return On_NEWLVL(pCmd, pnum);
	case CMD_WARP:
		return On_WARP(pCmd, pnum);
	case CMD_MONSTDEATH:
		return On_MONSTDEATH(pCmd, pnum);
	case CMD_KILLGOLEM:
		return On_KILLGOLEM(pCmd, pnum);
	case CMD_AWAKEGOLEM:
		return On_AWAKEGOLEM(pCmd, pnum);
	case CMD_MONSTDAMAGE:
		return On_MONSTDAMAGE(pCmd, pnum);
	case CMD_PLRDEAD:
		return On_PLRDEAD(pCmd, pnum);
	case CMD_PLRDAMAGE:
		return On_PLRDAMAGE(pCmd, pnum);
	case CMD_OPENDOOR:
		return On_OPENDOOR(pCmd, pnum);
	case CMD_CLOSEDOOR:
		return On_CLOSEDOOR(pCmd, pnum);
	case CMD_OPERATEOBJ:
		return On_OPERATEOBJ(pCmd, pnum);
	case CMD_PLROPOBJ:
		return On_PLROPOBJ(pCmd, pnum);
	case CMD_BREAKOBJ:
		return On_BREAKOBJ(pCmd, pnum);
	case CMD_CHANGEPLRITEMS:
		return On_CHANGEPLRITEMS(pCmd, pnum);
	case CMD_DELPLRITEMS:
		return On_DELPLRITEMS(pCmd, pnum);
	case CMD_PLRLEVEL:
		return On_PLRLEVEL(pCmd, pnum);
	case CMD_DROPITEM:
		return On_DROPITEM(pCmd, pnum);
	case CMD_ACK_PLRINFO:
		return On_ACK_PLRINFO(pCmd, pnum);
	case CMD_SEND_PLRINFO:
		return On_SEND_PLRINFO(pCmd, pnum);
	case CMD_PLAYER_JOINLEVEL:
		return On_PLAYER_JOINLEVEL(pCmd, pnum);
	case CMD_ACTIVATEPORTAL:
		return On_ACTIVATEPORTAL(pCmd, pnum);
	case CMD_DEACTIVATEPORTAL:
		return On_DEACTIVATEPORTAL(pCmd, pnum);
	case CMD_RETOWN:
		return On_RETOWN(pCmd, pnum);
	case CMD_SETSTR:
		return On_SETSTR(pCmd, pnum);
	case CMD_SETMAG:
		return On_SETMAG(pCmd, pnum);
	case CMD_SETDEX:
		return On_SETDEX(pCmd, pnum);
	case CMD_SETVIT:
		return On_SETVIT(pCmd, pnum);
	case CMD_STRING:
		return On_STRING(pCmd, pnum);
	case CMD_SYNCQUEST:
		return On_SYNCQUEST(pCmd, pnum);
	case CMD_ENDSHIELD:
		return On_ENDSHIELD(pCmd, pnum);
	case CMD_CHEAT_EXPERIENCE:
		return On_CHEAT_EXPERIENCE(pCmd, pnum);
	case CMD_CHEAT_SPELL_LEVEL:
		return On_CHEAT_SPELL_LEVEL(pCmd, pnum);
	case CMD_NOVA:
		return On_NOVA(pCmd, pnum);
	case CMD_SETSHIELD:
		return On_SETSHIELD(pCmd, pnum);
	case CMD_REMSHIELD:
		return On_REMSHIELD(pCmd, pnum);
	}

	if(pCmd->bCmd >= CMD_DLEVEL_0 && pCmd->bCmd <= CMD_DLEVEL_END) {
		return On_DLEVEL(pnum, pCmd);
	}

	SNetDropPlayer(pnum, 0x40000006);
	return 0;
}

DWORD On_DLEVEL(int pnum, const TCmd *pCmd)
{
	TCmdPlrInfoHdr *p;

	p = (TCmdPlrInfoHdr *)pCmd;

	if(gbDeltaSender != pnum) {
		if(p->bCmd == CMD_DLEVEL_END) {
			gbDeltaSender = pnum;
			sgbRecvCmd = CMD_DLEVEL_END;
		} else if(p->bCmd == CMD_DLEVEL_0 && p->wOffset == 0) {
			gbDeltaSender = pnum;
			sgbRecvCmd = CMD_DLEVEL_END;
		} else {
			return p->wBytes + sizeof(*p);
		}
	}
	if(sgbRecvCmd == CMD_DLEVEL_END) {
		if(p->bCmd == CMD_DLEVEL_END) {
			sgbDeltaChunks = 20;
			return p->wBytes + sizeof(*p);
		} else if(p->bCmd == CMD_DLEVEL_0 && p->wOffset == 0) {
			sgdwRecvOffset = 0;
			sgbRecvCmd = p->bCmd;
		} else {
			return p->wBytes + sizeof(*p);
		}
	} else if(sgbRecvCmd != p->bCmd) {
		DeltaImportData(sgbRecvCmd, sgdwRecvOffset);
		if(p->bCmd == CMD_DLEVEL_END) {
			sgbDeltaChunks = 20;
			sgbRecvCmd = CMD_DLEVEL_END;
			return p->wBytes + sizeof(*p);
		} else {
			sgdwRecvOffset = 0;
			sgbRecvCmd = p->bCmd;
		}
	}

	/// ASSERT: assert(p->wOffset == sgdwRecvOffset);
	memcpy(&sgRecvBuf[p->wOffset], &p[1], p->wBytes);
	sgdwRecvOffset += p->wBytes;
	return p->wBytes + sizeof(*p);
}

void DeltaImportData(BYTE bCmd, DWORD dwReceive)
{
	BYTE bLevel;
	BYTE *p;

	if(sgRecvBuf[0] != 0) {
		PkwareDecompress(&sgRecvBuf[1], dwReceive, sizeof(sgRecvBuf) - 1);
	}

	p = &sgRecvBuf[1];
	if(bCmd == CMD_DLEVEL_JUNK) {
		DeltaImportJunk(p);
	} else if(bCmd >= CMD_DLEVEL_0 && bCmd <= CMD_DLEVEL_16) {
		bLevel = bCmd - CMD_DLEVEL_0;
		p = DeltaImportItem(p, sgLevels[bLevel].item);
		p = DeltaImportObject(p, sgLevels[bLevel].object);
		p = DeltaImportMonster(p, sgLevels[bLevel].monster);
	} else {
		app_fatal("msg:1");
	}

	sgbDeltaChanged = 1;
	sgbDeltaChunks++;
}

BYTE *DeltaImportItem(BYTE *src, TCmdPItem *dst)
{
	int i;

	i = 0;
	while(i < MAXITEMS) {
		if(*src == 255) {
			memset(dst, 255, sizeof(*dst));
			src++;
		} else {
			memcpy(dst, src, sizeof(*dst));
			src += sizeof(*dst);
		}
		i++;
		dst++;
	}

	return src;
}

BYTE *DeltaImportObject(BYTE *src, DObjectStr *dst)
{
	memcpy(dst, src, sizeof(*dst) * MAXOBJECTS);
	return src + sizeof(*dst) * MAXOBJECTS;
}

BYTE *DeltaImportMonster(BYTE *src, DMonsterStr *dst)
{
	int i;

	i = 0;
	while(i < MAXMONSTERS) {
		if(*src == 255) {
			memset(dst, 255, sizeof(*dst));
			src++;
		} else {
			memcpy(dst, src, sizeof(*dst));
			src += sizeof(*dst);
		}
		i++;
		dst++;
	}

	return src;
}

void DeltaImportJunk(BYTE *src)
{
	int i, deltaq;

	for(i = 0; i < MAXPORTAL; i++) {
		if(*src == 255) {
			memset(&sgJunk.portal[i], 255, sizeof(sgJunk.portal[i]));
			src++;
			SetPortalStats(i, FALSE, 0, 0, 0, 0);
		} else {
			memcpy(&sgJunk.portal[i], src, sizeof(sgJunk.portal[i]));
			src += sizeof(sgJunk.portal[i]);
			SetPortalStats(i, TRUE, sgJunk.portal[i].x, sgJunk.portal[i].y, sgJunk.portal[i].level, sgJunk.portal[i].ltype);
		}
	}

	deltaq = 0;
	for(i = 0; i < MAXQUESTS; i++) {
		if(questlist[i]._qflags & 1) {
			memcpy(&sgJunk.quests[deltaq], src, sizeof(sgJunk.quests[deltaq]));
			quests[i]._qlog = sgJunk.quests[deltaq].qlog;
			quests[i]._qactive = sgJunk.quests[deltaq].qstate;
			quests[i]._qvar1 = sgJunk.quests[deltaq].qvar1;
			src += sizeof(sgJunk.quests[deltaq]);
			deltaq++;
		}
	}
}

DWORD On_SYNCDATA(const TCmd *pCmd, int pnum)
{
	return sync_update(pnum, (const BYTE *)pCmd);
}

DWORD On_WALKXY(const TCmd *pCmd, int pnum)
{
	TCmdLoc *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLoc *)pCmd;
		ClrPlrPath(pnum);
		MakePlrPath(pnum, p->x, p->y, TRUE);
		plr[pnum].destAction = -1;
	}

	return sizeof(*p);
}

DWORD On_ADDSTR(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(p->wParam1 <= 256) {
			ModifyPlrStr(pnum, p->wParam1);
		}
	}

	return sizeof(*p);
}

DWORD On_ADDMAG(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(p->wParam1 <= 256) {
			ModifyPlrMag(pnum, p->wParam1);
		}
	}

	return sizeof(*p);
}

DWORD On_ADDDEX(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(p->wParam1 <= 256) {
			ModifyPlrDex(pnum, p->wParam1);
		}
	}

	return sizeof(*p);
}

DWORD On_ADDVIT(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(p->wParam1 <= 256) {
			ModifyPlrVit(pnum, p->wParam1);
		}
	}

	return sizeof(*p);
}

DWORD On_SBSPELL(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1) {
		p = (TCmdParam1 *)pCmd;
		if(currlevel != 0 || spelldata[p->wParam1].sTownSpell) {
			plr[pnum]._pSpell = p->wParam1;
			plr[pnum]._pSplType = plr[pnum]._pSBkSplType;
			plr[pnum]._pSplFrom = 1;
			plr[pnum].destAction = 12;
		} else {
			msg_errorf("%s has cast an illegal spell.", plr[pnum]._pName);
		}
	}

	return sizeof(*p);
}

void __cdecl msg_errorf(const char *pszFmt, ...)
{
	DWORD dwTicks;
	char szStr[256];
	va_list va;

	va_start(va, pszFmt);

	dwTicks = GetTickCount();
	if(dwTicks - msg_err_timer >= 5000) {
		msg_err_timer = dwTicks;
		vsprintf(szStr, pszFmt, va);
		ErrorPlrMsg(szStr);
	}
}

DWORD On_GOTOGETITEM(const TCmd *pCmd, int pnum)
{
	TCmdLocParam1 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLocParam1 *)pCmd;
		MakePlrPath(pnum, p->x, p->y, FALSE);
		plr[pnum].destAction = 15;
		plr[pnum].destParam1 = p->wParam1;
	}

	return sizeof(*p);
}

DWORD On_REQUESTGITEM(const TCmd *pCmd, int pnum)
{
	int ii;
	TCmdGItem *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1) {
		p = (TCmdGItem *)pCmd;
		if(i_own_level(plr[pnum].plrlevel)) {
			/// ASSERT: assert(currlevel == plr[myplr].plrlevel);
			if(GetItemRecord(p->dwSeed, p->wCI, p->wIndx)) {
				ii = FindGetItem(p->wIndx, p->wCI, p->dwSeed);
				if(ii != -1) {
					NetSendCmdGItem2(FALSE, CMD_GETITEM, myplr, p->bPnum, p);
					if(p->bPnum != myplr) {
						SyncGetItem(p->x, p->y, p->wIndx, p->wCI, p->dwSeed);
					} else {
						InvGetItem(myplr, ii);
					}
					SetItemRecord(p->dwSeed, p->wCI, p->wIndx);
				} else if(!NetSendCmdReq2(CMD_REQUESTGITEM, myplr, p->bPnum, p)) {
					NetSendCmdExtra(p);
				}
			}
		}
	}

	return sizeof(*p);
}

BOOL i_own_level(int nReqLevel)
{
	int i;

	for(i = 0; i < MAX_PLRS; i++) {
		if(!plr[i].plractive || plr[i]._pLvlChanging || plr[i].plrlevel != nReqLevel) {
			continue;
		}
		if(i != myplr || gbBufferMsgs == 0) {
			break;
		}
	}

	return i == myplr;
}

DWORD On_GETITEM(const TCmd *pCmd, int pnum)
{
	int nIndex, hitem;
	TCmdGItem *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdGItem *)pCmd;
		nIndex = FindGetItem(p->wIndx, p->wCI, p->dwSeed);
		if(delta_get_item(p, p->bLevel)) {
			if((currlevel == p->bLevel || p->bPnum == myplr) && p->bMaster != myplr) {
				if(p->bPnum == myplr) {
					if(currlevel != p->bLevel) {
						hitem = SyncPutItem(
							myplr,
							plr[myplr].WorldX,
							plr[myplr].WorldY,
							p->wIndx,
							p->wCI,
							p->dwSeed,
							p->bId,
							p->bDur,
							p->bMDur,
							p->bCh,
							p->bMCh,
							p->wValue,
							p->dwBuff);
						if(hitem != -1) {
							InvGetItem(myplr, hitem);
						}
					} else {
						InvGetItem(myplr, nIndex);
					}
#if 0
					item_droplog_debug(
						"%d: local plr get at (%d,%d) ndx %d ci %d seed %d\r\n",
						myplr,
						p->x,
						p->y,
						p->wIndx,
						p->wCI,
						p->dwSeed);
#endif
				} else {
					SyncGetItem(p->x, p->y, p->wIndx, p->wCI, p->dwSeed);
#if 0
					item_droplog_debug(
						"%d: remote plr %d get at (%d,%d) ndx %d ci %d seed %d\r\n",
						myplr,
						p->bPnum,
						p->x,
						p->y,
						p->wIndx,
						p->wCI,
						p->dwSeed);
#endif
				}
			}
		} else {
			NetSendCmdGItem2(TRUE, CMD_GETITEM, p->bMaster, p->bPnum, p);
#if 0
			item_droplog_debug(
				"%d: repost get at (%d,%d) ndx %d ci %d seed %d master\r\n",
				myplr,
				p->x,
				p->y,
				p->wIndx,
				p->wCI,
				p->dwSeed,
				p->bMaster);
#endif
		}
	}

	return sizeof(*p);
}

BOOL delta_get_item(const TCmdGItem *pI, BYTE bLevel)
{
	int i;
	TCmdPItem *pD;

	if(gbMaxPlayers == 1) {
		return TRUE;
	}

	/// ASSERT: assert(pI != NULL);
	/// ASSERT: assert(bLevel < NUMLEVELS);

	pD = sgLevels[bLevel].item;
	i = 0;
	while(i < MAXITEMS) {
		if(pD->bCmd != 255 && pD->wIndx == pI->wIndx && pD->wCI == pI->wCI && pD->dwSeed == pI->dwSeed) {
			if(pD->bCmd == CMD_WALKXY) {
				return TRUE;
			} else if(pD->bCmd == CMD_STAND) {
				sgbDeltaChanged = 1;
				pD->bCmd = CMD_WALKXY;
				return TRUE;
			} else if(pD->bCmd == CMD_ACK_PLRINFO) {
				sgbDeltaChanged = 1;
				pD->bCmd = 255;
				return TRUE;
			} else {
				app_fatal("delta:1");
			}
			break;
		}
		i++;
		pD++;
	}

	if(!(pI->wCI & 0x8000)) {
		return FALSE;
	}

	pD = sgLevels[bLevel].item;
	i = 0;
	while(i < MAXITEMS) {
		if(pD->bCmd == 255) {
			sgbDeltaChanged = 1;
			pD->bCmd = CMD_WALKXY;
			pD->x = pI->x;
			pD->y = pI->y;
			pD->wIndx = pI->wIndx;
			pD->wCI = pI->wCI;
			pD->dwSeed = pI->dwSeed;
			pD->bId = pI->bId;
			pD->bDur = pI->bDur;
			pD->bMDur = pI->bMDur;
			pD->bCh = pI->bCh;
			pD->bMCh = pI->bMCh;
			pD->wValue = pI->wValue;
			pD->dwBuff = pI->dwBuff;
			return TRUE;
		}
		i++;
		pD++;
	}

	return TRUE;
}

DWORD On_GOTOAGETITEM(const TCmd *pCmd, int pnum)
{
	TCmdLocParam1 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLocParam1 *)pCmd;
		MakePlrPath(pnum, p->x, p->y, FALSE);
		plr[pnum].destAction = 16;
		plr[pnum].destParam1 = p->wParam1;
	}

	return sizeof(*p);
}

DWORD On_REQUESTAGITEM(const TCmd *pCmd, int pnum)
{
	int ii;
	TCmdGItem *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1) {
		p = (TCmdGItem *)pCmd;
		if(i_own_level(plr[pnum].plrlevel)) {
			/// ASSERT: assert(currlevel == plr[myplr].plrlevel);
			if(GetItemRecord(p->dwSeed, p->wCI, p->wIndx)) {
				ii = FindGetItem(p->wIndx, p->wCI, p->dwSeed);
				if(ii != -1) {
					NetSendCmdGItem2(FALSE, CMD_AGETITEM, myplr, p->bPnum, p);
					if(p->bPnum != myplr) {
						SyncGetItem(p->x, p->y, p->wIndx, p->wCI, p->dwSeed);
					} else {
						AutoGetItem(myplr, p->bCursitem);
					}
					SetItemRecord(p->dwSeed, p->wCI, p->wIndx);
				} else if(!NetSendCmdReq2(CMD_REQUESTAGITEM, myplr, p->bPnum, p)) {
					NetSendCmdExtra(p);
				}
			}
		}
	}

	return sizeof(*p);
}

DWORD On_AGETITEM(const TCmd *pCmd, int pnum)
{
	int nIndex, hitem;
	TCmdGItem *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdGItem *)pCmd;
		nIndex = FindGetItem(p->wIndx, p->wCI, p->dwSeed); /// BUGFIX: useless call, remove
		if(delta_get_item(p, p->bLevel)) {
			if((currlevel == p->bLevel || p->bPnum == myplr) && p->bMaster != myplr) {
				if(p->bPnum == myplr) {
					if(currlevel != p->bLevel) {
						hitem = SyncPutItem(
							myplr,
							plr[myplr].WorldX,
							plr[myplr].WorldY,
							p->wIndx,
							p->wCI,
							p->dwSeed,
							p->bId,
							p->bDur,
							p->bMDur,
							p->bCh,
							p->bMCh,
							p->wValue,
							p->dwBuff);
						if(hitem != -1) {
							AutoGetItem(myplr, hitem);
						}
					} else {
						AutoGetItem(myplr, p->bCursitem);
					}
				} else {
					SyncGetItem(p->x, p->y, p->wIndx, p->wCI, p->dwSeed);
				}
			}
		} else {
			NetSendCmdGItem2(TRUE, CMD_AGETITEM, p->bMaster, p->bPnum, p);
		}
	}

	return sizeof(*p);
}

DWORD On_ITEMEXTRA(const TCmd *pCmd, int pnum)
{
	TCmdGItem *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdGItem *)pCmd;
		delta_get_item(p, p->bLevel);
		if(currlevel == plr[pnum].plrlevel) {
			SyncGetItem(p->x, p->y, p->wIndx, p->wCI, p->dwSeed);
		}
	}

	return sizeof(*p);
}

DWORD On_PUTITEM(const TCmd *pCmd, int pnum)
{
	int ii;
	TCmdPItem *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdPItem *)pCmd;
		if(currlevel == plr[pnum].plrlevel) {
			if(pnum == myplr) {
				ii = InvPutItem(pnum, p->x, p->y);
			} else {
				ii = SyncPutItem(
					pnum,
					p->x,
					p->y,
					p->wIndx,
					p->wCI,
					p->dwSeed,
					p->bId,
					p->bDur,
					p->bMDur,
					p->bCh,
					p->bMCh,
					p->wValue,
					p->dwBuff);
			}
			if(ii != -1) {
				PutItemRecord(p->dwSeed, p->wCI, p->wIndx);
				delta_put_item(p, item[ii]._ix, item[ii]._iy, plr[pnum].plrlevel);
				check_update_plr(pnum);
			}
			return sizeof(*p);
		}
		PutItemRecord(p->dwSeed, p->wCI, p->wIndx);
		delta_put_item(p, p->x, p->y, plr[pnum].plrlevel);
		check_update_plr(pnum);
	}

	return sizeof(*p);
}

void delta_put_item(const TCmdPItem *pI, int x, int y, BYTE bLevel)
{
	int i;
	TCmdPItem *pD;

	if(gbMaxPlayers == 1) {
		return;
	}

	/// ASSERT: assert(pI != NULL);
	/// ASSERT: assert(x < DMAXX);
	/// ASSERT: assert(y < DMAXY);
	/// ASSERT: assert(bLevel < NUMLEVELS);

	pD = sgLevels[bLevel].item;
	i = 0;
	while(i < MAXITEMS) {
		if(pD->bCmd != CMD_WALKXY && pD->bCmd != 255) {
			if(pD->wIndx == pI->wIndx && pD->wCI == pI->wCI && pD->dwSeed == pI->dwSeed) {
				if(pD->bCmd == CMD_ACK_PLRINFO) {
					return;
				}
				app_fatal("Trying to drop a floor item?");
			}
		}
		i++;
		pD++;
	}

	pD = sgLevels[bLevel].item;
	i = 0;
	while(i < MAXITEMS) {
		if(pD->bCmd == 255) {
			sgbDeltaChanged = 1;
			memcpy(pD, pI, sizeof(*pI));
			pD->bCmd = CMD_ACK_PLRINFO;
			pD->x = x;
			pD->y = y;
			return;
		}
		i++;
		pD++;
	}
}

void check_update_plr(int pnum)
{
	if(gbMaxPlayers == 1) {
		return;
	}

	/// ASSERT: assert((DWORD)pnum < MAX_PLRS);
	if(pnum == myplr) {
		pfile_update(TRUE);
	}
}

DWORD On_SYNCPUTITEM(const TCmd *pCmd, int pnum)
{
	int ii;
	TCmdPItem *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdPItem *)pCmd;
		if(currlevel == plr[pnum].plrlevel) {
			ii = SyncPutItem(
				pnum,
				p->x,
				p->y,
				p->wIndx,
				p->wCI,
				p->dwSeed,
				p->bId,
				p->bDur,
				p->bMDur,
				p->bCh,
				p->bMCh,
				p->wValue,
				p->dwBuff);
			if(ii != -1) {
				PutItemRecord(p->dwSeed, p->wCI, p->wIndx);
				delta_put_item(p, item[ii]._ix, item[ii]._iy, plr[pnum].plrlevel);
				check_update_plr(pnum);
			}
			return sizeof(*p);
		}
		PutItemRecord(p->dwSeed, p->wCI, p->wIndx);
		delta_put_item(p, p->x, p->y, plr[pnum].plrlevel);
		check_update_plr(pnum);
	}

	return sizeof(*p);
}

DWORD On_RESPAWNITEM(const TCmd *pCmd, int pnum)
{
	int ii;
	TCmdPItem *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdPItem *)pCmd;
		if(currlevel == plr[pnum].plrlevel && pnum != myplr) {
			ii = SyncPutItem(
				pnum,
				p->x,
				p->y,
				p->wIndx,
				p->wCI,
				p->dwSeed,
				p->bId,
				p->bDur,
				p->bMDur,
				p->bCh,
				p->bMCh,
				p->wValue,
				p->dwBuff);
		}
		PutItemRecord(p->dwSeed, p->wCI, p->wIndx);
		delta_put_item(p, p->x, p->y, plr[pnum].plrlevel);
	}

	return sizeof(*p);
}

DWORD On_ATTACKXY(const TCmd *pCmd, int pnum)
{
	TCmdLoc *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLoc *)pCmd;
		MakePlrPath(pnum, p->x, p->y, FALSE);
		plr[pnum].destAction = 9;
		plr[pnum].destParam1 = p->x;
		plr[pnum].destParam2 = p->y;
	}

	return sizeof(*p);
}

DWORD On_SATTACKXY(const TCmd *pCmd, int pnum)
{
	TCmdLoc *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLoc *)pCmd;
		ClrPlrPath(pnum);
		plr[pnum].destAction = 9;
		plr[pnum].destParam1 = p->x;
		plr[pnum].destParam2 = p->y;
	}

	return sizeof(*p);
}

DWORD On_RATTACKXY(const TCmd *pCmd, int pnum)
{
	TCmdLoc *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLoc *)pCmd;
		ClrPlrPath(pnum);
		plr[pnum].destAction = 10;
		plr[pnum].destParam1 = p->x;
		plr[pnum].destParam2 = p->y;
	}

	return sizeof(*p);
}

DWORD On_SPELLXYD(const TCmd *pCmd, int pnum)
{
	TCmdLocParam3 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLocParam3 *)pCmd;
		if(currlevel != 0 || spelldata[p->wParam1].sTownSpell) {
			ClrPlrPath(pnum);
			plr[pnum].destAction = 26;
			plr[pnum].destParam1 = p->x;
			plr[pnum].destParam2 = p->y;
			plr[pnum].destParam3 = p->wParam2;
			plr[pnum].destParam4 = p->wParam3;
			plr[pnum]._pSpell = p->wParam1;
			plr[pnum]._pSplType = plr[pnum]._pRSplType;
			plr[pnum]._pSplFrom = 0;
		} else {
			msg_errorf("%s has cast an illegal spell.", plr[pnum]._pName);
		}
	}

	return sizeof(*p);
}

DWORD On_SPELLXY(const TCmd *pCmd, int pnum)
{
	TCmdLocParam2 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLocParam2 *)pCmd;
		if(currlevel != 0 || spelldata[p->wParam1].sTownSpell) {
			ClrPlrPath(pnum);
			plr[pnum].destAction = 12;
			plr[pnum].destParam1 = p->x;
			plr[pnum].destParam2 = p->y;
			plr[pnum].destParam3 = p->wParam2;
			plr[pnum]._pSpell = p->wParam1;
			plr[pnum]._pSplType = plr[pnum]._pRSplType;
			plr[pnum]._pSplFrom = 0;
		} else {
			msg_errorf("%s has cast an illegal spell.", plr[pnum]._pName);
		}
	}

	return sizeof(*p);
}

DWORD On_TSPELLXY(const TCmd *pCmd, int pnum)
{
	TCmdLocParam2 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLocParam2 *)pCmd;
		if(currlevel != 0 || spelldata[p->wParam1].sTownSpell) {
			ClrPlrPath(pnum);
			plr[pnum].destAction = 12;
			plr[pnum].destParam1 = p->x;
			plr[pnum].destParam2 = p->y;
			plr[pnum].destParam3 = p->wParam2;
			plr[pnum]._pSpell = p->wParam1;
			plr[pnum]._pSplType = plr[pnum]._pTSplType;
			plr[pnum]._pSplFrom = 2;
		} else {
			msg_errorf("%s has cast an illegal spell.", plr[pnum]._pName);
		}
	}

	return sizeof(*p);
}

DWORD On_OPOBJXY(const TCmd *pCmd, int pnum)
{
	TCmdLocParam1 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLocParam1 *)pCmd;
		if(!object[p->wParam1]._oSolidFlag && !object[p->wParam1]._oDoorFlag) {
			MakePlrPath(pnum, p->x, p->y, TRUE);
		} else {
			MakePlrPath(pnum, p->x, p->y, FALSE);
		}
		plr[pnum].destAction = 13;
		plr[pnum].destParam1 = p->wParam1;
	}

	return sizeof(*p);
}

DWORD On_DISARMXY(const TCmd *pCmd, int pnum)
{
	TCmdLocParam1 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLocParam1 *)pCmd;
		if(!object[p->wParam1]._oSolidFlag && !object[p->wParam1]._oDoorFlag) {
			MakePlrPath(pnum, p->x, p->y, TRUE);
		} else {
			MakePlrPath(pnum, p->x, p->y, FALSE);
		}
		plr[pnum].destAction = 14;
		plr[pnum].destParam1 = p->wParam1;
	}

	return sizeof(*p);
}

DWORD On_OPOBJT(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdParam1 *)pCmd;
		plr[pnum].destAction = 18;
		plr[pnum].destParam1 = p->wParam1;
	}

	return sizeof(*p);
}

DWORD On_ATTACKID(const TCmd *pCmd, int pnum)
{
	int dx, dy;
	TCmdParam1 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdParam1 *)pCmd;
		dx = abs(plr[pnum].WorldX - monster[p->wParam1]._mfutx);
		dy = abs(plr[pnum].WorldY - monster[p->wParam1]._mfuty);
		if(dx > 1 || dy > 1) {
			MakePlrPath(pnum, monster[p->wParam1]._mfutx, monster[p->wParam1]._mfuty, FALSE);
		}
		plr[pnum].destAction = 20;
		plr[pnum].destParam1 = p->wParam1;
	}

	return sizeof(*p);
}

DWORD On_ATTACKPID(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdParam1 *)pCmd;
		MakePlrPath(pnum, plr[p->wParam1]._px, plr[p->wParam1]._py, FALSE);
		plr[pnum].destAction = 21;
		plr[pnum].destParam1 = p->wParam1;
	}

	return sizeof(*p);
}

DWORD On_RATTACKID(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdParam1 *)pCmd;
		ClrPlrPath(pnum);
		plr[pnum].destAction = 22;
		plr[pnum].destParam1 = p->wParam1;
	}

	return sizeof(*p);
}

DWORD On_RATTACKPID(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdParam1 *)pCmd;
		ClrPlrPath(pnum);
		plr[pnum].destAction = 23;
		plr[pnum].destParam1 = p->wParam1;
	}

	return sizeof(*p);
}

DWORD On_SPELLID(const TCmd *pCmd, int pnum)
{
	TCmdParam3 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdParam3 *)pCmd;
		if(currlevel != 0 || spelldata[p->wParam2].sTownSpell) {
			ClrPlrPath(pnum);
			plr[pnum].destAction = 24;
			plr[pnum].destParam1 = p->wParam1;
			plr[pnum].destParam2 = p->wParam3;
			plr[pnum]._pSpell = p->wParam2;
			plr[pnum]._pSplType = plr[pnum]._pRSplType;
			plr[pnum]._pSplFrom = 0;
		} else {
			msg_errorf("%s has cast an illegal spell.", plr[pnum]._pName);
		}
	}

	return sizeof(*p);
}

DWORD On_SPELLPID(const TCmd *pCmd, int pnum)
{
	TCmdParam3 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdParam3 *)pCmd;
		if(currlevel != 0 || spelldata[p->wParam2].sTownSpell) {
			ClrPlrPath(pnum);
			plr[pnum].destAction = 25;
			plr[pnum].destParam1 = p->wParam1;
			plr[pnum].destParam2 = p->wParam3;
			plr[pnum]._pSpell = p->wParam2;
			plr[pnum]._pSplType = plr[pnum]._pRSplType;
			plr[pnum]._pSplFrom = 0;
		} else {
			msg_errorf("%s has cast an illegal spell.", plr[pnum]._pName);
		}
	}

	return sizeof(*p);
}

DWORD On_TSPELLID(const TCmd *pCmd, int pnum)
{
	TCmdParam3 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdParam3 *)pCmd;
		if(currlevel != 0 || spelldata[p->wParam2].sTownSpell) {
			ClrPlrPath(pnum);
			plr[pnum].destAction = 24;
			plr[pnum].destParam1 = p->wParam1;
			plr[pnum].destParam2 = p->wParam3;
			plr[pnum]._pSpell = p->wParam2;
			plr[pnum]._pSplType = plr[pnum]._pTSplType;
			plr[pnum]._pSplFrom = 2;
		} else {
			msg_errorf("%s has cast an illegal spell.", plr[pnum]._pName);
		}
	}

	return sizeof(*p);
}

DWORD On_TSPELLPID(const TCmd *pCmd, int pnum)
{
	TCmdParam3 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdParam3 *)pCmd;
		if(currlevel != 0 || spelldata[p->wParam2].sTownSpell) {
			ClrPlrPath(pnum);
			plr[pnum].destAction = 25;
			plr[pnum].destParam1 = p->wParam1;
			plr[pnum].destParam2 = p->wParam3;
			plr[pnum]._pSpell = p->wParam2;
			plr[pnum]._pSplType = plr[pnum]._pTSplType;
			plr[pnum]._pSplFrom = 2;
		} else {
			msg_errorf("%s has cast an illegal spell.", plr[pnum]._pName);
		}
	}

	return sizeof(*p);
}

DWORD On_KNOCKBACK(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs != 1) {
		p = (TCmdParam1 *)pCmd;
		if(currlevel == plr[pnum].plrlevel) {
			M_GetKnockback(p->wParam1);
			M_StartHit(p->wParam1, pnum, 0);
		}
	}

	return sizeof(*p);
}

DWORD On_RESURRECT(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		DoResurrect(pnum, p->wParam1);
		check_update_plr(pnum);
	}

	return sizeof(*p);
}

DWORD On_HEALOTHER(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdParam1 *)pCmd;
		DoHealOther(pnum, p->wParam1);
	}

	return sizeof(*p);
}

DWORD On_TALKXY(const TCmd *pCmd, int pnum)
{
	TCmdLocParam1 *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel) {
		p = (TCmdLocParam1 *)pCmd;
		MakePlrPath(pnum, p->x, p->y, FALSE);
		plr[pnum].destAction = 17;
		plr[pnum].destParam1 = p->wParam1;
	}

	return sizeof(*p);
}

DWORD On_NEWLVL(const TCmd *pCmd, int pnum)
{
	TCmdParam2 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else if(pnum != myplr) {
		p = (TCmdParam2 *)pCmd;
		StartNewLvl(pnum, p->wParam1, p->wParam2);
	}

	return sizeof(*p);
}

DWORD On_WARP(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		StartWarpLvl(pnum, p->wParam1);
		if(pnum == myplr && pcurs >= CURSOR_FIRSTITEM) {
			item[MAXITEMS] = plr[myplr].HoldItem;
			AutoGetItem(myplr, MAXITEMS);
		}
	}

	return sizeof(*p);
}

DWORD On_MONSTDEATH(const TCmd *pCmd, int pnum)
{
	TCmdLocParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else if(pnum != myplr) {
		p = (TCmdLocParam1 *)pCmd;
		if(currlevel == plr[pnum].plrlevel) {
			M_SyncStartKill(p->wParam1, p->x, p->y, pnum);
		}
		delta_kill_monster(p->wParam1, p->x, p->y, plr[pnum].plrlevel);
	}

	return sizeof(*p);
}

DWORD On_KILLGOLEM(const TCmd *pCmd, int pnum)
{
	TCmdLocParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else if(pnum != myplr) {
		p = (TCmdLocParam1 *)pCmd;
		if(currlevel == p->wParam1) {
			M_SyncStartKill(pnum, p->x, p->y, pnum);
		}
		delta_kill_monster(pnum, p->x, p->y, plr[pnum].plrlevel);
	}

	return sizeof(*p);
}

DWORD On_AWAKEGOLEM(const TCmd *pCmd, int pnum)
{
	int i, mi;
	BOOL addok;
	TCmdGolem *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdGolem *)pCmd;
		if(currlevel != plr[pnum].plrlevel) {
			delta_sync_golem(p, pnum, p->_currlevel);
		} else if(pnum != myplr) {
			addok = TRUE;
			for(i = 0; i < nummissiles; i++) {
				mi = missileactive[i];
				if(missile[mi]._mitype == MIS_GOLEM && missile[mi]._misource == pnum) {
					addok = FALSE;
				}
			}
			if(addok) {
				AddMissile(plr[pnum].WorldX, plr[pnum].WorldY, p->_mx, p->_my, p->_mdir, MIS_GOLEM, 0, pnum, 0, 1);
			}
		}
	}

	return sizeof(*p);
}

DWORD On_MONSTDAMAGE(const TCmd *pCmd, int pnum)
{
	TCmdParam2 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else if(pnum != myplr) {
		p = (TCmdParam2 *)pCmd;
		if(currlevel == plr[pnum].plrlevel) {
			monster[p->wParam1].mWhoHit |= 1 << pnum;
			if(monster[p->wParam1]._mhitpoints != 0) {
				monster[p->wParam1]._mhitpoints -= p->wParam2;
				if(monster[p->wParam1]._mhitpoints >> 6 < 1) {
					monster[p->wParam1]._mhitpoints = 64;
				}
				delta_monster_hp(p->wParam1, monster[p->wParam1]._mhitpoints, plr[pnum].plrlevel);
			}
		}
	}

	return sizeof(*p);
}

DWORD On_PLRDEAD(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(pnum != myplr) {
			StartPlayerKill(pnum, p->wParam1);
		} else {
			check_update_plr(pnum);
		}
	}

	return sizeof(*p);
}

DWORD On_PLRDAMAGE(const TCmd *pCmd, int pnum)
{
	TCmdDamage *p;

	/// ASSERT: assert(gbBufferMsgs != BUFFER_PROCESS);

	p = (TCmdDamage *)pCmd;
	if(p->bPlr == myplr && currlevel != 0 && gbBufferMsgs != 1) {
		if(currlevel == plr[pnum].plrlevel && p->dwDam <= 192000 && plr[myplr]._pHitPoints >> 6 > 0) {
			drawhpflag = 1;
			plr[myplr]._pHitPoints -= p->dwDam;
			plr[myplr]._pHPBase -= p->dwDam;
			if(plr[myplr]._pHitPoints > plr[myplr]._pMaxHP) {
				plr[myplr]._pHitPoints = plr[myplr]._pMaxHP;
				plr[myplr]._pHPBase = plr[myplr]._pMaxHPBase;
			}
			if(plr[myplr]._pHitPoints >> 6 <= 0) {
				SyncPlrKill(myplr, 1);
			}
		}
	}

	return sizeof(*p);
}

DWORD On_OPENDOOR(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(currlevel == plr[pnum].plrlevel) {
			SyncOpObject(pnum, CMD_OPENDOOR, p->wParam1);
		}
		delta_sync_object(p->wParam1, CMD_OPENDOOR, plr[pnum].plrlevel);
	}

	return sizeof(*p);
}

void delta_sync_object(int oi, BYTE bCmd, BYTE bLevel)
{
	DObjectStr *p;

	if(gbMaxPlayers == 1) {
		return;
	}

	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
	/// ASSERT: assert(bLevel < NUMLEVELS);

	sgbDeltaChanged = 1;
	p = &sgLevels[bLevel].object[oi];
	p->bCmd = bCmd;
}

DWORD On_CLOSEDOOR(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(currlevel == plr[pnum].plrlevel) {
			SyncOpObject(pnum, CMD_CLOSEDOOR, p->wParam1);
		}
		delta_sync_object(p->wParam1, CMD_CLOSEDOOR, plr[pnum].plrlevel);
	}

	return sizeof(*p);
}

DWORD On_OPERATEOBJ(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(currlevel == plr[pnum].plrlevel) {
			SyncOpObject(pnum, CMD_OPERATEOBJ, p->wParam1);
		}
		delta_sync_object(p->wParam1, CMD_OPERATEOBJ, plr[pnum].plrlevel);
	}

	return sizeof(*p);
}

DWORD On_PLROPOBJ(const TCmd *pCmd, int pnum)
{
	TCmdParam2 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam2 *)pCmd;
		if(currlevel == plr[pnum].plrlevel) {
			SyncOpObject(p->wParam1, CMD_PLROPOBJ, p->wParam2);
		}
		delta_sync_object(p->wParam2, CMD_PLROPOBJ, plr[pnum].plrlevel);
	}

	return sizeof(*p);
}

DWORD On_BREAKOBJ(const TCmd *pCmd, int pnum)
{
	TCmdParam2 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam2 *)pCmd;
		if(currlevel == plr[pnum].plrlevel) {
			SyncBreakObj(p->wParam1, p->wParam2);
		}
		delta_sync_object(p->wParam2, CMD_BREAKOBJ, plr[pnum].plrlevel);
	}

	return sizeof(*p);
}

DWORD On_CHANGEPLRITEMS(const TCmd *pCmd, int pnum)
{
	TCmdChItem *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdChItem *)pCmd;
		if(pnum != myplr) {
			CheckInvSwap(pnum, p->bLoc, p->wIndx, p->wCI, p->dwSeed, p->bId);
		}
	}

	return sizeof(*p);
}

DWORD On_DELPLRITEMS(const TCmd *pCmd, int pnum)
{
	TCmdDelItem *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdDelItem *)pCmd;
		if(pnum != myplr) {
			inv_update_rem_item(pnum, p->bLoc);
		}
	}

	return sizeof(*p);
}

DWORD On_PLRLEVEL(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(p->wParam1 <= 51 && pnum != myplr) {
			plr[pnum]._pLevel = p->wParam1;
		}
	}

	return sizeof(*p);
}

DWORD On_DROPITEM(const TCmd *pCmd, int pnum)
{
	TCmdPItem *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdPItem *)pCmd;
		delta_put_item(p, p->x, p->y, plr[pnum].plrlevel);
	}

	return sizeof(*p);
}

DWORD On_SEND_PLRINFO(const TCmd *pCmd, int pnum)
{
	TCmdPlrInfoHdr *p;

	p = (TCmdPlrInfoHdr *)pCmd;
	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, p->wBytes + sizeof(*p));
	} else {
		recv_plrinfo(pnum, p, p->bCmd == CMD_ACK_PLRINFO);
	}

	return p->wBytes + sizeof(*p);
}

DWORD On_ACK_PLRINFO(const TCmd *pCmd, int pnum)
{
	return On_SEND_PLRINFO(pCmd, pnum);
}

DWORD On_PLAYER_JOINLEVEL(const TCmd *pCmd, int pnum)
{
	TCmdLocParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		/// ASSERT: assert((DWORD)pnum < MAX_PLRS);
		plr[pnum]._pLvlChanging = 0;
		if(plr[pnum]._pName[0] == '\0') {
#ifdef _DEBUG
			dumphist("(%d) received %d joinlevel before plrdata", myplr, pnum);
#endif
		} else if(!plr[pnum].plractive) {
			plr[pnum].plractive = 1;
			gbActivePlayers++;
			EventPlrMsg("Player '%s' (level %d) just joined the game", plr[pnum]._pName, plr[pnum]._pLevel);
#ifdef _DEBUG
			dumphist("(%d) activating %d on joinlevel", myplr, pnum);
#endif
		}
		if(plr[pnum].plractive && myplr != pnum) {
			p = (TCmdLocParam1 *)pCmd;
			plr[pnum].WorldX = p->x;
			plr[pnum].WorldY = p->y;
			plr[pnum].plrlevel = p->wParam1;
			plr[pnum]._pGFXLoad = 0;
			if(currlevel == plr[pnum].plrlevel) {
				LoadPlrGFX(pnum, 1);
				SyncInitPlr(pnum);
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
				plr[pnum]._pvid = AddVision(plr[pnum].WorldX, plr[pnum].WorldY, plr[pnum]._pLightRad, pnum == myplr);
				plr[pnum]._plid = -1;
			}
		}
	}

	return sizeof(*p);
}

DWORD On_ACTIVATEPORTAL(const TCmd *pCmd, int pnum)
{
	int i, mi;
	BOOL addok;
	TCmdLocParam3 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdLocParam3 *)pCmd;
		ActivatePortal(pnum, p->x, p->y, p->wParam1, p->wParam2, p->wParam3);
		if(pnum != myplr) {
			if(currlevel == 0) {
				AddInTownPortal(pnum);
			} else if(currlevel == plr[pnum].plrlevel) {
				addok = TRUE;
				for(i = 0; i < nummissiles; i++) {
					mi = missileactive[i];
					if(missile[mi]._mitype == MIS_TOWN && missile[mi]._misource == pnum) {
						addok = FALSE;
					}
				}
				if(addok) {
					AddWarpMissile(pnum, p->x, p->y);
				}
			} else {
				RemovePortalMissile(pnum);
			}
		}
		delta_open_portal(pnum, p->x, p->y, p->wParam1, p->wParam2, p->wParam3);
	}

	return sizeof(*p);
}

void delta_open_portal(int pnum, BYTE x, BYTE y, BYTE bLevel, BYTE bLType, BYTE bSetLvl)
{
	/// ASSERT: assert((DWORD)pnum < MAX_PLRS);
	/// ASSERT: assert(x < DMAXX);
	/// ASSERT: assert(y < DMAXY);
	/// ASSERT: assert(bLevel < NUMLEVELS);

	sgJunk.portal[pnum].x = x;
	sgJunk.portal[pnum].y = y;
	sgJunk.portal[pnum].level = bLevel;
	sgJunk.portal[pnum].ltype = bLType;
	sgJunk.portal[pnum].setlvl = bSetLvl;
	sgbDeltaChanged = 1;
}

DWORD On_DEACTIVATEPORTAL(const TCmd *pCmd, int pnum)
{
	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*pCmd));
	} else {
		if(PortalOnLevel(pnum)) {
			RemovePortalMissile(pnum);
		}
		DeactivatePortal(pnum);
		delta_close_portal(pnum);
	}

	return sizeof(*pCmd);
}

DWORD On_RETOWN(const TCmd *pCmd, int pnum)
{
	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*pCmd));
	} else {
		if(pnum == myplr) {
			deathflag = 0;
			gamemenu_off();
		}
		RestartTownLvl(pnum);
	}

	return sizeof(*pCmd);
}

DWORD On_SETSTR(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(p->wParam1 <= 750 && pnum != myplr) {
			SetPlrStr(pnum, p->wParam1);
		}
	}

	return sizeof(*p);
}

DWORD On_SETDEX(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(p->wParam1 <= 750 && pnum != myplr) {
			SetPlrDex(pnum, p->wParam1);
		}
	}

	return sizeof(*p);
}

DWORD On_SETMAG(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(p->wParam1 <= 750 && pnum != myplr) {
			SetPlrMag(pnum, p->wParam1);
		}
	}

	return sizeof(*p);
}

DWORD On_SETVIT(const TCmd *pCmd, int pnum)
{
	TCmdParam1 *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdParam1 *)pCmd;
		if(p->wParam1 <= 750 && pnum != myplr) {
			SetPlrVit(pnum, p->wParam1);
		}
	}

	return sizeof(*p);
}

DWORD On_STRING(const TCmd *pCmd, int pnum)
{
	TCmdString *p;

	p = (TCmdString *)pCmd;
	return On_STRING2(pnum, p);
}

DWORD On_STRING2(int pnum, const TCmdString *p)
{
	int len;

	/// ASSERT: assert((DWORD)pnum < MAX_PLRS);
	/// ASSERT: assert(p != NULL);

	len = strlen(p->str);
	if(gbBufferMsgs == 0) {
		SendPlrMsg(pnum, p->str);
	}

	return len + 2;
}

DWORD On_SYNCQUEST(const TCmd *pCmd, int pnum)
{
	TCmdQuest *p;

	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*p));
	} else {
		p = (TCmdQuest *)pCmd;
		if(pnum != myplr) {
			SetMultiQuest(p->q, p->qstate, p->qlog, p->qvar1);
		}
		sgbDeltaChanged = 1;
	}

	return sizeof(*p);
}

DWORD On_ENDSHIELD(const TCmd *pCmd, int pnum)
{
	int i, mi;

	if(gbBufferMsgs != 1 && pnum != myplr && currlevel == plr[pnum].plrlevel) {
		for(i = 0; i < nummissiles; i++) {
			mi = missileactive[i];
			if(missile[mi]._mitype == MIS_MANASHIELD && missile[mi]._misource == pnum) {
				ClearMissileSpot(mi);
				DeleteMissile(mi, i);
			}
		}
	}

	return sizeof(*pCmd);
}

DWORD On_CHEAT_EXPERIENCE(const TCmd *pCmd, int pnum)
{
#ifdef _DEBUG
	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*pCmd));
	} else if(plr[pnum]._pLevel < 50) {
		plr[pnum]._pExperience = plr[pnum]._pNextExper;
		NextPlrLevel(pnum);
	}
#endif
	return sizeof(*pCmd);
}

DWORD On_CHEAT_SPELL_LEVEL(const TCmd *pCmd, int pnum)
{
#ifdef _DEBUG
	if(gbBufferMsgs == 1) {
		msg_send_packet(pnum, pCmd, sizeof(*pCmd));
	} else {
		plr[pnum]._pSplLvl[plr[pnum]._pRSpell]++;
	}
#endif
	return sizeof(*pCmd);
}

DWORD On_DEBUG(const TCmd *pCmd, int pnum)
{
	return sizeof(*pCmd);
}

DWORD On_NOVA(const TCmd *pCmd, int pnum)
{
	TCmdLoc *p;

	if(gbBufferMsgs != 1 && currlevel == plr[pnum].plrlevel && pnum != myplr) {
		p = (TCmdLoc *)pCmd;
		ClrPlrPath(pnum);
		plr[pnum]._pSpell = SPL_NOVA;
		plr[pnum]._pSplType = 4;
		plr[pnum]._pSplFrom = 3;
		plr[pnum].destAction = 12;
		plr[pnum].destParam1 = p->x;
		plr[pnum].destParam2 = p->y;
	}

	return sizeof(*p);
}

DWORD On_SETSHIELD(const TCmd *pCmd, int pnum)
{
	if(gbBufferMsgs != 1) {
		plr[pnum].pManaShield = 1;
	}

	return sizeof(*pCmd);
}

DWORD On_REMSHIELD(const TCmd *pCmd, int pnum)
{
	if(gbBufferMsgs != 1) {
		plr[pnum].pManaShield = 0;
	}

	return sizeof(*pCmd);
}
