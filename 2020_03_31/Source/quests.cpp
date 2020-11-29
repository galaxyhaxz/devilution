#include "diablo.h"

int qtopline; // idb
int questlog; // weak
void *pQLogCel;
QuestStruct quests[MAXQUESTS];
int qline; // weak
int qlist[MAXQUESTS];
int numqlines; // weak
int WaterDone; // idb
int ReturnLvlY; // idb
int ReturnLvlX; // idb
int ReturnLvlT; // idb
int ALLQUESTS; // idb
int ReturnLvl; // idb

QuestData questlist[MAXQUESTS] =
{
#include "Data/xl_quest.cpp"
};
char questxoff[7] = { 0, -1, 0, -1, -2, -1, -2 };
char questyoff[7] = { 0, 0, -1, -1, -1, -2, -2 };
char *questtrigstr[5] =
{
  "King Leoric's Tomb",
  "The Chamber of Bone",
  "Maze",
  "A Dark Passage",
  "Unholy Altar"
};
int QuestGroup1[3] = { Q_BUTCHER, Q_LTBANNER, Q_GARBUD };
int QuestGroup2[3] = { Q_BLIND, Q_ROCK, Q_BLOOD };
int QuestGroup3[3] = { Q_MUSHROOM, Q_ZHAR, Q_ANVIL };
int QuestGroup4[2] = { Q_VEIL, Q_WARLORD };

void InitQuests()
{
	int i, gri, deltaq;

	if(gbMaxPlayers == 1) {
		for(i = 0; i < MAXQUESTS; i++) {
			quests[i]._qactive = 0;
		}
	} else {
		for(i = 0; i < MAXQUESTS; i++) {
			if(!(questlist[i]._qflags & 1)) {
				quests[i]._qactive = 0;
			}
		}
	}

	questlog = 0;
	ALLQUESTS = 1;
	WaterDone = 0;

	deltaq = 0;
	for(i = 0; (DWORD)i < MAXQUESTS; i++) {
		if(gbMaxPlayers > 1 && !(questlist[i]._qflags & 1)) {
			continue;
		}
		quests[i]._qtype = questlist[i]._qdtype;
		if(gbMaxPlayers > 1) {
			quests[i]._qlevel = questlist[i]._qdmultlvl;
			if(!delta_quest_inited(deltaq)) {
				quests[i]._qactive = 1;
				quests[i]._qvar1 = 0;
				quests[i]._qlog = FALSE;
			}
			deltaq++;
		} else {
			quests[i]._qactive = 1;
			quests[i]._qlevel = questlist[i]._qdlvl;
			quests[i]._qvar1 = 0;
			quests[i]._qlog = FALSE;
		}
		quests[i]._qslvl = questlist[i]._qslvl;
		quests[i]._qtx = 0;
		quests[i]._qty = 0;
		quests[i]._qidx = i;
		quests[i]._qlvltype = questlist[i]._qlvlt;
		quests[i]._qvar2 = 0;
		quests[i]._qmsg = questlist[i]._qdmsg;
	}

	if(gbMaxPlayers == 1) {
		SetRndSeed(glSeedTbl[15]);
		if(random(0, 2) != 0) {
			quests[Q_PWATER]._qactive = 0;
		} else {
			quests[Q_SKELKING]._qactive = 0;
		}
		gri = random(0, 3);
		quests[QuestGroup1[gri]]._qactive = 0;
		gri = random(0, 3);
		quests[QuestGroup2[gri]]._qactive = 0;
		gri = random(0, 3);
		quests[QuestGroup3[gri]]._qactive = 0;
		gri = random(0, 2);
		quests[QuestGroup4[gri]]._qactive = 0;
	}
#ifdef _DEBUG
	if(questdebug != -1) {
		quests[questdebug]._qactive = 2;
	}
#endif
	if(quests[Q_SKELKING]._qactive == 0) {
		quests[Q_SKELKING]._qvar2 = 2;
	}
	if(quests[Q_ROCK]._qactive == 0) {
		quests[Q_ROCK]._qvar2 = 2;
	}

	quests[Q_LTBANNER]._qvar1 = 1;

	if(gbMaxPlayers != 1) {
		quests[Q_BETRAYER]._qvar1 = 2;
	}
}

void CheckQuests()
{
	int i, rportx, rporty;

	if(QuestStatus(Q_BETRAYER) && gbMaxPlayers != 1 && quests[Q_BETRAYER]._qvar1 == 2) {
		AddObject(OBJ_ALTBOY, 2 * setpc_x + 20, 2 * setpc_y + 22);
		quests[Q_BETRAYER]._qvar1 = 3;
		NetSendCmdQuest(TRUE, Q_BETRAYER);
	}

	if(gbMaxPlayers != 1) {
		return;
	}

	if(currlevel == quests[Q_BETRAYER]._qlevel
	&& !setlevel
	&& quests[Q_BETRAYER]._qvar1 >= 2
	&& (quests[Q_BETRAYER]._qactive == 2 || quests[Q_BETRAYER]._qactive == 3)
	&& (quests[Q_BETRAYER]._qvar2 == 0 || quests[Q_BETRAYER]._qvar2 == 2)) {
		quests[Q_BETRAYER]._qtx = 2 * quests[Q_BETRAYER]._qtx + 16;
		quests[Q_BETRAYER]._qty = 2 * quests[Q_BETRAYER]._qty + 16;
		rportx = quests[Q_BETRAYER]._qtx;
		rporty = quests[Q_BETRAYER]._qty;
		AddMissile(rportx, rporty, rportx, rporty, 0, MIS_RPORTAL, 0, myplr, 0, 0);
		quests[Q_BETRAYER]._qvar2 = 1;
		if(quests[Q_BETRAYER]._qactive == 2) {
			quests[Q_BETRAYER]._qvar1 = 3;
		}
	}

	if(quests[Q_BETRAYER]._qactive == 3
	&& setlevel
	&& setlvlnum == SL_VILEBETRAYER
	&& quests[Q_BETRAYER]._qvar2 == 4) {
		rportx = 35;
		rporty = 32;
		AddMissile(rportx, rporty, rportx, rporty, 0, MIS_RPORTAL, 0, myplr, 0, 0);
		quests[Q_BETRAYER]._qvar2 = 3;
	}

	if(setlevel) {
		if(setlvlnum == quests[Q_PWATER]._qslvl
		&& quests[Q_PWATER]._qactive != 1
		&& leveltype == quests[Q_PWATER]._qlvltype
		&& nummonsters == 4
		&& quests[Q_PWATER]._qactive != 3) {
			quests[Q_PWATER]._qactive = 3;
			PlaySfxLoc(IS_QUESTDN, plr[myplr].WorldX, plr[myplr].WorldY);
			LoadPalette("Levels\\L3Data\\L3pwater.pal");
			WaterDone = 32;
		}
		if(WaterDone > 0) {
			Lava2Water(WaterDone);
			WaterDone--;
		}
	} else if(plr[myplr]._pmode == PM_STAND) {
		for(i = 0; i < MAXQUESTS; i++) {
			if(currlevel == quests[i]._qlevel
			&& quests[i]._qslvl != 0
			&& quests[i]._qactive != 0
			&& plr[myplr].WorldX == quests[i]._qtx
			&& plr[myplr].WorldY == quests[i]._qty) {
				if(quests[i]._qlvltype != 255) {
					setlvltype = quests[i]._qlvltype;
				}
				StartNewLvl(myplr, WM_DIABSETLVL, quests[i]._qslvl);
			}
		}
	}
}

BOOL ForceQuests()
{
	int i, j, ql, qx, qy;

	if(gbMaxPlayers != 1) {
		return FALSE;
	}

	for(i = 0; i < MAXQUESTS; i++) {
		if(i != Q_BETRAYER && currlevel == quests[i]._qlevel && quests[i]._qslvl != 0) {
			ql = quests[quests[i]._qidx]._qslvl - 1;
			qx = quests[i]._qtx;
			qy = quests[i]._qty;
			for(j = 0; j < 7; j++) {
				if(qx + questxoff[j] == cursmx && qy + questyoff[j] == cursmy) {
					sprintf(infostr, "To %s", questtrigstr[ql]);
					cursmx = qx;
					cursmy = qy;
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL QuestStatus(int i)
{
	if(setlevel) {
		return FALSE;
	}
	/// ASSERT: assert((DWORD)i < MAXQUESTS);
	if(currlevel != quests[i]._qlevel) {
		return FALSE;
	}
	if(quests[i]._qactive == 0) {
		return FALSE;
	}
	if(gbMaxPlayers != 1 && !(questlist[i]._qflags & 1)) {
		return FALSE;
	}

	return TRUE;
}

void CheckQuestKill(int m, BOOL sendmsg)
{
	int i, j;

	/// ASSERT: assert((DWORD)m < MAXMONSTERS);
	if(monster[m].MType->mtype == MT_SKING) {
		quests[Q_SKELKING]._qactive = 3;
		sfxdelay = 30;
		if(plr[myplr]._pClass == PC_WARRIOR) {
			sfxdnum = PS_WARR82;
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			sfxdnum = PS_ROGUE82;
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			sfxdnum = PS_MAGE82;
		}
		if(sendmsg) {
			NetSendCmdQuest(TRUE, Q_SKELKING);
		}
	} else if(monster[m].MType->mtype == MT_CLEAVER) {
		quests[Q_BUTCHER]._qactive = 3;
		sfxdelay = 30;
		if(plr[myplr]._pClass == PC_WARRIOR) {
			sfxdnum = PS_WARR80;
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			sfxdnum = PS_ROGUE80;
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			sfxdnum = PS_MAGE80;
		}
		if(sendmsg) {
			NetSendCmdQuest(TRUE, Q_BUTCHER);
		}
	} else if(monster[m].mName == UniqMonst[0].mName) {
		quests[Q_GARBUD]._qactive = 3;
		sfxdelay = 30;
		if(plr[myplr]._pClass == PC_WARRIOR) {
			sfxdnum = PS_WARR61;
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			sfxdnum = PS_ROGUE61;
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			sfxdnum = PS_MAGE61;
		}
	} else if(monster[m].mName == UniqMonst[2].mName) {
		quests[Q_ZHAR]._qactive = 3;
		sfxdelay = 30;
		if(plr[myplr]._pClass == PC_WARRIOR) {
			sfxdnum = PS_WARR62;
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			sfxdnum = PS_ROGUE62;
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			sfxdnum = PS_MAGE62;
		}
	} else if(monster[m].mName == UniqMonst[4].mName && gbMaxPlayers != 1) {
		quests[Q_BETRAYER]._qactive = 3;
		quests[Q_BETRAYER]._qvar1 = 7;
		sfxdelay = 30;
		quests[Q_DIABLO]._qactive = 2;
		for(j = 0; j < MAXDUNY; j++) {
			for(i = 0; i < MAXDUNX; i++) {
				if(dPiece[i][j] == 370) {
					trigs[numtrigs]._tx = i;
					trigs[numtrigs]._ty = j;
					trigs[numtrigs]._tmsg = WM_DIABNEXTLVL;
					numtrigs++;
				}
			}
		}
		if(plr[myplr]._pClass == PC_WARRIOR) {
			sfxdnum = PS_WARR83;
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			sfxdnum = PS_ROGUE83;
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			sfxdnum = PS_MAGE83;
		}
		if(sendmsg) {
			NetSendCmdQuest(TRUE, Q_BETRAYER);
			NetSendCmdQuest(TRUE, Q_DIABLO);
		}
	} else if(monster[m].mName == UniqMonst[4].mName && gbMaxPlayers == 1) {
		quests[Q_BETRAYER]._qactive = 3;
		sfxdelay = 30;
		InitVPTriggers();
		quests[Q_BETRAYER]._qvar1 = 7;
		quests[Q_BETRAYER]._qvar2 = 4;
		quests[Q_DIABLO]._qactive = 2;
		AddMissile(35, 32, 35, 32, 0, MIS_RPORTAL, 0, myplr, 0, 0);
		if(plr[myplr]._pClass == PC_WARRIOR) {
			sfxdnum = PS_WARR83;
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			sfxdnum = PS_ROGUE83;
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			sfxdnum = PS_MAGE83;
		}
	} else if(monster[m].mName == UniqMonst[8].mName) {
		/// ASSERT: assert(gbMaxPlayers == 1);
		quests[Q_WARLORD]._qactive = 3;
		sfxdelay = 30;
		if(plr[myplr]._pClass == PC_WARRIOR) {
			sfxdnum = PS_WARR94;
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			sfxdnum = PS_ROGUE94;
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			sfxdnum = PS_MAGE94;
		}
	}
}

void DrawButcher()
{
	int xx, yy;

	xx = 2 * setpc_x + 16;
	yy = 2 * setpc_y + 16;

	DRLG_RectTrans(xx + 3, yy + 3, xx + 10, yy + 10);
}

void DrawSkelKing(int q, int x, int y)
{
	/// ASSERT: assert((DWORD)q < MAXQUESTS);
	quests[q]._qtx = 2 * x + 28;
	quests[q]._qty = 2 * y + 23;
}

void DrawWarLord(int x, int y)
{
	int rw, rh, i, j;
	BYTE *sp, *setp;

	setp = DiabLoad("Levels\\L4Data\\Warlord2.DUN", NULL, 'QSTt');
	sp = setp;
	rw = *sp;
	sp += 2;
	rh = *sp;
	sp += 2;

	setpc_w = rw;
	setpc_h = rh;
	setpc_x = x;
	setpc_y = y;

	/// ASSERT: assert((DWORD)(rw + x - 1) < MDMAXX);
	/// ASSERT: assert((DWORD)(rh + y - 1) < MDMAXY);
	for(j = y; j < y + rh; j++) {
		for(i = x; i < x + rw; i++) {
			dungeon[i][j] = *sp != 0 ? *sp : 6;
			sp += 2;
		}
	}

	MemFreeDbg(setp);
}

void DrawSChamber(int q, int x, int y)
{
	int i, j, rw, rh, xx, yy;
	BYTE *sp, *setp;

	setp = DiabLoad("Levels\\L2Data\\Bonestr1.DUN", NULL, 'QSTt');
	sp = setp;
	rw = *sp;
	sp += 2;
	rh = *sp;
	sp += 2;

	setpc_w = rw;
	setpc_h = rh;
	setpc_x = x;
	setpc_y = y;

	/// ASSERT: assert((DWORD)(rw + x - 1) < MDMAXX);
	/// ASSERT: assert((DWORD)(rh + y - 1) < MDMAXY);
	for(j = y; j < y + rh; j++) {
		for(i = x; i < x + rw; i++) {
			dungeon[i][j] = *sp != 0 ? *sp : 3;
			sp += 2;
		}
	}

	xx = 2 * x + 22;
	yy = 2 * y + 23;
	/// ASSERT: assert((DWORD)q < MAXQUESTS);
	quests[q]._qtx = xx;
	quests[q]._qty = yy;

	MemFreeDbg(setp);
}

void DrawLTBanner(int x, int y)
{
	int rw, rh, i, j;
	BYTE *sp, *setp;

	setp = DiabLoad("Levels\\L1Data\\Banner1.DUN", NULL, 'QSTt');
	sp = setp;
	rw = *sp;
	sp += 2;
	rh = *sp;
	sp += 2;

	setpc_w = rw;
	setpc_h = rh;
	setpc_x = x;
	setpc_y = y;

	/// ASSERT: assert((DWORD)(x+rw-1) < MDMAXX);
	/// ASSERT: assert((DWORD)(y+rh-1) < MDMAXY);
	for(j = 0; j < rh; j++) {
		for(i = 0; i < rw; i++) {
			if(*sp != 0) {
				pdungeon[i + x][j + y] = *sp;
			}
			sp += 2;
		}
	}

	MemFreeDbg(setp);
}

void DrawBlind(int x, int y)
{
	int rw, rh, i, j;
	BYTE *sp, *setp;

	setp = DiabLoad("Levels\\L2Data\\Blind1.DUN", NULL, 'QSTt');
	sp = setp;
	rw = *sp;
	sp += 2;
	rh = *sp;
	sp += 2;

	setpc_x = x;
	setpc_y = y;
	setpc_w = rw;
	setpc_h = rh;

	/// ASSERT: assert((DWORD)(x+rw-1) < MDMAXX);
	/// ASSERT: assert((DWORD)(y+rh-1) < MDMAXY);
	for(j = 0; j < rh; j++) {
		for(i = 0; i < rw; i++) {
			if(*sp != 0) {
				pdungeon[i + x][j + y] = *sp;
			}
			sp += 2;
		}
	}

	MemFreeDbg(setp);
}

void DrawBlood(int x, int y)
{
	int rw, rh, i, j;
	BYTE *sp, *setp;

	setp = DiabLoad("Levels\\L2Data\\Blood2.DUN", NULL, 'QSTt');
	sp = setp;
	rw = *sp;
	sp += 2;
	rh = *sp;
	sp += 2;

	setpc_x = x;
	setpc_y = y;
	setpc_w = rw;
	setpc_h = rh;

	/// ASSERT: assert((DWORD)(rw + x - 1) < MDMAXX);
	/// ASSERT: assert((DWORD)(rh + y - 1) < MDMAXY);
	for(j = 0; j < rh; j++) {
		for(i = 0; i < rw; i++) {
			if(*sp != 0) {
				dungeon[i + x][j + y] = *sp;
			}
			sp += 2;
		}
	}

	MemFreeDbg(setp);
}

void DRLG_CheckQuests(int x, int y)
{
	int i;

	for(i = 0; i < MAXQUESTS; i++) {
		if(QuestStatus(i)) {
			switch(quests[i]._qtype) {
			case Q_BUTCHER:
				DrawButcher();
				break;
			case Q_SKELKING:
				DrawSkelKing(i, x, y);
				break;
			case Q_SCHAMB:
				DrawSChamber(i, x, y);
				break;
			case Q_BLIND:
				DrawBlind(x, y);
				break;
			case Q_BLOOD:
				DrawBlood(x, y);
				break;
			case Q_LTBANNER:
				DrawLTBanner(x, y);
				break;
			case Q_WARLORD:
				DrawWarLord(x, y);
				break;
			}
		}
	}
}

void SetReturnLvlPos()
{
	switch(setlvlnum) {
	case SL_SKELKING:
		ReturnLvlX = quests[Q_SKELKING]._qtx + 1;
		ReturnLvlY = quests[Q_SKELKING]._qty;
		ReturnLvl = quests[Q_SKELKING]._qlevel;
		ReturnLvlT = DTYPE_CATHEDRAL;
		break;
	case SL_BONECHAMB:
		ReturnLvlX = quests[Q_SCHAMB]._qtx + 1;
		ReturnLvlY = quests[Q_SCHAMB]._qty;
		ReturnLvl = quests[Q_SCHAMB]._qlevel;
		ReturnLvlT = DTYPE_CATACOMBS;
		break;
	case SL_POISONWATER:
		ReturnLvlX = quests[Q_PWATER]._qtx;
		ReturnLvlY = quests[Q_PWATER]._qty + 1;
		ReturnLvl = quests[Q_PWATER]._qlevel;
		ReturnLvlT = DTYPE_CATHEDRAL;
		break;
	case SL_VILEBETRAYER:
		ReturnLvlX = quests[Q_BETRAYER]._qtx + 1;
		ReturnLvlY = quests[Q_BETRAYER]._qty - 1;
		ReturnLvl = quests[Q_BETRAYER]._qlevel;
		ReturnLvlT = DTYPE_HELL;
		break;
	}
}

void GetReturnLvlPos()
{
	if(quests[Q_BETRAYER]._qactive == 3) {
		quests[Q_BETRAYER]._qvar2 = 2;
	}

	ViewX = ReturnLvlX;
	ViewY = ReturnLvlY;
	currlevel = ReturnLvl;
	leveltype = ReturnLvlT;
}

void ResyncMPQuests()
{
	if(quests[Q_SKELKING]._qactive == 1 && currlevel >= quests[Q_SKELKING]._qlevel - 1 && currlevel <= quests[Q_SKELKING]._qlevel + 1) {
		quests[Q_SKELKING]._qactive = 2;
		NetSendCmdQuest(TRUE, Q_SKELKING);
	}
	if(quests[Q_BUTCHER]._qactive == 1 && currlevel >= quests[Q_BUTCHER]._qlevel - 1 && currlevel <= quests[Q_BUTCHER]._qlevel + 1) {
		quests[Q_BUTCHER]._qactive = 2;
		NetSendCmdQuest(TRUE, Q_BUTCHER);
	}
	if(quests[Q_BETRAYER]._qactive == 1 && currlevel == quests[Q_BETRAYER]._qlevel - 1) {
		quests[Q_BETRAYER]._qactive = 2;
		NetSendCmdQuest(TRUE, Q_BETRAYER);
	}
	if(QuestStatus(Q_BETRAYER)) {
		AddObject(OBJ_ALTBOY, 2 * setpc_x + 20, 2 * setpc_y + 22);
	}
}

void ResyncQuests()
{
	int i, tren;

	if(setlevel && setlvlnum == quests[Q_PWATER]._qslvl && quests[Q_PWATER]._qactive != 1 && leveltype == quests[Q_PWATER]._qlvltype) {
		if(quests[Q_PWATER]._qactive == 3) {
			LoadPalette("Levels\\L3Data\\L3pwater.pal");
		} else {
			LoadPalette("Levels\\L3Data\\L3pfoul.pal");
		}
		for(i = 0; i <= 32; i++) {
			Lava2Water(i);
		}
	}
	if(QuestStatus(Q_LTBANNER)) {
		if(quests[Q_LTBANNER]._qvar1 == 1) {
			ObjChangeMapResync(setpc_x + setpc_w - 2, setpc_y + setpc_h - 2, setpc_x + setpc_w + 1, setpc_y + setpc_h + 1);
		}
		if(quests[Q_LTBANNER]._qvar1 == 2) {
			ObjChangeMapResync(setpc_x + setpc_w - 2, setpc_y + setpc_h - 2, setpc_x + setpc_w + 1, setpc_y + setpc_h + 1);
			ObjChangeMapResync(setpc_x, setpc_y, setpc_x + (setpc_w >> 1) + 2, setpc_y + (setpc_h >> 1) - 2);
			for(i = 0; i < nobjects; i++) {
				SyncObjectAnim(objectactive[i]);
			}
			tren = TransVal;
			TransVal = 9;
			DRLG_MRectTrans(setpc_x, setpc_y, setpc_x + (setpc_w >> 1) + 4, setpc_y + (setpc_h >> 1));
			TransVal = tren;
		}
		if(quests[Q_LTBANNER]._qvar1 == 3) {
			ObjChangeMapResync(setpc_x, setpc_y, setpc_x + setpc_w + 1, setpc_y + setpc_h + 1);
			for(i = 0; i < nobjects; i++) {
				SyncObjectAnim(objectactive[i]);
			}
			tren = TransVal;
			TransVal = 9;
			DRLG_MRectTrans(setpc_x, setpc_y, setpc_x + (setpc_w >> 1) + 4, setpc_y + (setpc_h >> 1));
			TransVal = tren;
		}
	}
	if(currlevel == quests[Q_MUSHROOM]._qlevel) {
		if(quests[Q_MUSHROOM]._qactive == 1) {
			if(quests[Q_MUSHROOM]._qvar1 == 0) {
				SpawnQuestItem(IDI_FUNGALTM, 0, 0, 5, 1);
				quests[Q_MUSHROOM]._qvar1 = 1;
			}
		} else if(quests[Q_MUSHROOM]._qactive == 2) {
			if(quests[Q_MUSHROOM]._qvar1 >= 5) {
				Qtalklist[TOWN_HEALER][Q_MUSHROOM] = TEXT_MUSH3;
				Qtalklist[TOWN_WITCH][Q_MUSHROOM] = -1;
			} else if(quests[Q_MUSHROOM]._qvar1 >= 7) {
				Qtalklist[TOWN_HEALER][Q_MUSHROOM] = -1;
			}
		}
	}
	if(currlevel == quests[Q_VEIL]._qlevel + 1 && quests[Q_VEIL]._qactive == 2 && quests[Q_VEIL]._qvar1 == 0) {
		quests[Q_VEIL]._qvar1 = 1;
		SpawnQuestItem(IDI_GLDNELIX, 0, 0, 5, 1);
	}
	if(setlevel && setlvlnum == SL_VILEBETRAYER) {
		if(quests[Q_BETRAYER]._qvar1 >= 4) {
			ObjChangeMapResync(1, 11, 20, 18);
		}
		if(quests[Q_BETRAYER]._qvar1 >= 6) {
			ObjChangeMapResync(1, 18, 20, 24);
		}
		if(quests[Q_BETRAYER]._qvar1 >= 7) {
			InitVPTriggers();
		}
		for(i = 0; i < nobjects; i++) {
			SyncObjectAnim(objectactive[i]);
		}
	}
	if(currlevel == quests[Q_BETRAYER]._qlevel
	&& !setlevel
	&& (quests[Q_BETRAYER]._qvar2 == 1 || quests[Q_BETRAYER]._qvar2 >= 3)
	&& (quests[Q_BETRAYER]._qactive == 2 || quests[Q_BETRAYER]._qactive == 3)) {
		quests[Q_BETRAYER]._qvar2 = 2;
	}
}

void PrintQLString(int x, int y, BOOL cjustflag, char *str, int col)
{
	int sx, sy, len, l, i, w, No;
	BYTE c;

	sy = SStringY[y];
	No = x + PitchTbl[sy + 204] + 96;
	len = strlen(str);

	w = 0;
	if(cjustflag) {
		l = 0;
		for(i = 0; i < len; i++) {
			c = fontframe[gbFontTransTbl[(BYTE)str[i]]];
			l += fontkern[c] + 1;
		}
		if(l < 257) {
			w = (257 - l) >> 1;
		}
		No += w;
	}
	if(qline == y) {
		if(cjustflag) {
			sx = x + w + 76;
		} else {
			sx = x + 76;
		}
		CelDecodeOnly(sx, sy + 205, (BYTE *)pSPentSpnCels, ALLQUESTS, 12);
	}

	for(i = 0; i < len; i++) {
		c = fontframe[gbFontTransTbl[(BYTE)str[i]]];
		w += fontkern[c] + 1;
		if(c != 0 && w <= 257) {
			PrintChar(No, c, col);
		}
		No += fontkern[c] + 1;
	}

	if(qline == y) {
		if(cjustflag) {
			sx = x + w + 100;
		} else {
			sx = 340 - x;
		}
		CelDecodeOnly(sx, sy + 205, (BYTE *)pSPentSpnCels, ALLQUESTS, 12);
	}
}

void DrawQuestLog()
{
	int i, l, q;

	PrintQLString(0, 2, TRUE, "Quest Log", COL_GOLD);
	CelDecodeOnly(64, 511, (BYTE *)pQLogCel, 1, 320);

	l = qtopline;
	for(i = 0; i < numqlines; i++) {
		q = qlist[i];
		PrintQLString(0, l, TRUE, questlist[q]._qlstr, COL_WHITE);
		l += 2;
	}

	PrintQLString(0, 22, TRUE, "Close Quest Log", COL_WHITE);
	ALLQUESTS = (ALLQUESTS & 7) + 1;
}

void StartQuestlog()
{
	int i;

	numqlines = 0;
	for(i = 0; (DWORD)i < MAXQUESTS; i++) {
		if(quests[i]._qactive == 2 && quests[i]._qlog) {
			qlist[numqlines] = i;
			numqlines++;
		}
	}

	if(numqlines > 5) {
		qtopline = 5 - (numqlines >> 1);
	} else {
		qtopline = 8;
	}

	if(numqlines == 0) {
		qline = 22;
	} else {
		qline = qtopline;
	}

	questlog = 1;
	ALLQUESTS = 1;
}

void QuestlogUp()
{
	if(numqlines != 0) {
		if(qline == qtopline) {
			qline = 22;
		} else if(qline == 22) {
			qline = qtopline + 2 * numqlines - 2;
		} else {
			qline -= 2;
		}
		PlaySFX(IS_TITLEMOV);
	}
}

void QuestlogDown()
{
	if(numqlines != 0) {
		if(qline == 22) {
			qline = qtopline;
		} else if(qline == qtopline + 2 * numqlines - 2) {
			qline = 22;
		} else {
			qline += 2;
		}
		PlaySFX(IS_TITLEMOV);
	}
}

void QuestlogEnter()
{
	int q;

	PlaySFX(IS_TITLSLCT);

	if(numqlines != 0 && qline != 22) {
		q = qlist[(qline - qtopline) >> 1];
		InitQTextMsg(quests[q]._qmsg);
	}

	questlog = 0;
}

void QuestlogESC()
{
	int i, l;

	l = (MouseY - 32) / 12;
	if(numqlines != 0) {
		for(i = 0; i < numqlines; i++) {
			if(l == qtopline + 2 * i) {
				qline = l;
				QuestlogEnter();
			}
		}
	}
	if(l == 22) {
		qline = 22;
		QuestlogEnter();
	}
}

void SetMultiQuest(int q, int s, BOOL l, int v1)
{
	/// ASSERT: assert((DWORD)q < MAXQUESTS);
	/// ASSERT: assert(quests[q]._qactive != QUEST_NOTAVAIL);
	if(quests[q]._qactive == 3) {
		return;
	}

	if(s > quests[q]._qactive) {
		quests[q]._qactive = s;
	}

	quests[q]._qlog |= l;

	if(v1 > quests[q]._qvar1) {
		quests[q]._qvar1 = v1;
	}
}
