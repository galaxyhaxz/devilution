#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

int plr_lframe_size; // idb
int plr_wframe_size; // idb
BYTE plr_gfx_flag; // weak
int plr_aframe_size; // idb
int myplr;
PlayerStruct plr[MAX_PLRS];
int plr_fframe_size; // idb
int plr_qframe_size; // idb
int deathflag; // idb
int plr_hframe_size; // idb
int plr_bframe_size; // idb
char plr_gfx_bflag; // weak
int plr_sframe_size; // idb
int deathdelay; // weak
int plr_dframe_size; // idb

const char ArmourChar[4] = { 'L', 'M', 'H', 0 };
const char WepChar[10] = { 'N', 'U', 'S', 'D', 'B', 'A', 'M', 'H', 'T', 0 };
const char CharChar[4] = { 'W', 'R', 'S', 0 };

/* data */

int plrxoff[9] = { 0, 2, 0, 2, 1, 0, 1, 2, 1 };
int plryoff[9] = { 0, 2, 2, 0, 1, 1, 0, 1, 2 };
int plrxoff2[9] = { 0, 1, 0, 1, 2, 0, 1, 2, 2 };
int plryoff2[9] = { 0, 0, 1, 1, 0, 2, 2, 1, 2 };
char PlrGFXAnimLens[3][11] =
{
  { 10, 16, 8, 2, 20, 20, 6, 20, 8, 9, 14 },
  { 8, 18, 8, 4, 20, 16, 7, 20, 8, 10, 12 },
  { 8, 16, 8, 6, 20, 12, 8, 20, 8, 12, 8 }
};
int PWVel[3][3] = { { 2048, 1024, 512 }, { 2048, 1024, 512 }, { 2048, 1024, 512 } };
int WalkAnimTbl[3] = { 8, 8, 8 };
int StrengthTbl[3] = { 30, 20, 15 };
int MagicTbl[3] = { 10, 15, 35 };
int DexterityTbl[3] = { 20, 30, 15 };
int VitalityTbl[3] = { 25, 20, 20 };
int ToBlkTbl[3] = { 30, 20, 10 };
char *ClassStrTblOld[3] = { "Warrior", "Rogue", "Sorceror" }; // unused
int MaxStats[3][4] = { { 250, 50, 60, 100 }, { 55, 70, 250, 80 }, { 45, 250, 85, 80 } };
int ExpLvlsTbl[51] =
{
  0,
  2000,
  4620,
  8040,
  12489,
  18258,
  25712,
  35309,
  47622,
  63364,
  83419,
  108879,
  141086,
  181683,
  231075,
  313656,
  424067,
  571190,
  766569,
  1025154,
  1366227,
  1814568,
  2401895,
  3168651,
  4166200,
  5459523,
  7130496,
  9281874,
  12042092,
  15571031,
  20066900,
  25774405,
  32994399,
  42095202,
  53525811,
  67831218,
  85670061,
  107834823,
  135274799,
  169122009,
  210720231,
  261657253,
  323800420,
  399335440,
  490808349,
  601170414,
  733825617,
  892680222,
  1082908612,
  1310707109,
  1583495809
};
char *ClassStrTbl[3] = { "Warrior", "Rogue", "Sorceror" };

void SetPlayerGPtrs(BYTE *pData, BYTE **pAnim)
{
	int i;
	DWORD *pFrameTable;

	pFrameTable = (DWORD *)pData;

	for(i = 0; i < 8; i++) {
		pAnim[i] = &pData[pFrameTable[i]];
	}
}

void LoadPlrGFX(int pnum, DWORD dwMask)
{
	PlayerStruct *pPlayer;
	DWORD i;
	const char *c, *Suffix;
	BYTE *pData, **pAnim;
	char FileName[256];
	char Type[16];

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("LoadPlrGFX: illegal player %d", pnum);
	}

	pPlayer = &plr[pnum];
	/// ASSERT: assert((DWORD) pPlayer->_pClass <= NUM_CLASSES);
	sprintf(Type, "%c%c%c", CharChar[pPlayer->_pClass], ArmourChar[pPlayer->_pgfxnum >> 4], WepChar[pPlayer->_pgfxnum & 0xF]);

	c = ClassStrTbl[pPlayer->_pClass];

	for(i = 1; i <= 0x17F; i <<= 1) {
		if(!(i & dwMask)) {
			continue;
		}
		switch(i) {
		case 1:
			Suffix = "AS";
			if(leveltype == DTYPE_TOWN) {
				Suffix = "ST";
			}
			pData = pPlayer->_pNData;
			pAnim = pPlayer->_pNAnim;
			break;
		case 2:
			Suffix = "AW";
			if(leveltype == DTYPE_TOWN) {
				Suffix = "WL";
			}
			pData = pPlayer->_pWData;
			pAnim = pPlayer->_pWAnim;
			break;
		case 4:
			if(leveltype == DTYPE_TOWN) {
				continue;
			}
			Suffix = "AT";
			pData = pPlayer->_pAData;
			pAnim = pPlayer->_pAAnim;
			break;
		case 8:
			if(leveltype == DTYPE_TOWN) {
				continue;
			}
			Suffix = "HT";
			pData = pPlayer->_pHData;
			pAnim = pPlayer->_pHAnim;
			break;
		case 0x10:
			if(leveltype == DTYPE_TOWN) {
				continue;
			}
			Suffix = "LM";
			pData = pPlayer->_pLData;
			pAnim = pPlayer->_pLAnim;
			break;
		case 0x20:
			if(leveltype == DTYPE_TOWN) {
				continue;
			}
			Suffix = "FM";
			pData = pPlayer->_pFData;
			pAnim = pPlayer->_pFAnim;
			break;
		case 0x40:
			if(leveltype == DTYPE_TOWN) {
				continue;
			}
			Suffix = "QM";
			pData = pPlayer->_pTData;
			pAnim = pPlayer->_pTAnim;
			break;
		case 0x80:
			if(pPlayer->_pgfxnum & 0xF) {
				continue;
			}
			Suffix = "DT";
			pData = pPlayer->_pDData;
			pAnim = pPlayer->_pDAnim;
			break;
		case 0x100:
			if(leveltype == DTYPE_TOWN || !pPlayer->_pBlockFlag) {
				continue;
			}
			Suffix = "BL";
			pData = pPlayer->_pBData;
			pAnim = pPlayer->_pBAnim;
			break;
		default:
			app_fatal("PLR:2");
			break;
		}
		sprintf(FileName, "PlrGFX\\%s\\%s\\%s%s.CL2", c, Type, Type, Suffix);
		/// ASSERT: assert(pData);
		LoadFileWithMem(FileName, pData);
		SetPlayerGPtrs(pData, pAnim);
		pPlayer->_pGFXLoad |= i;
	}
}

void InitPlayerGFX(int pnum)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("InitPlayerGFX: illegal player %d", pnum);
	}

	if(plr[pnum]._pHitPoints >> 6 == 0) {
		plr[pnum]._pgfxnum = 0;
		LoadPlrGFX(pnum, 0x80);
	} else {
		LoadPlrGFX(pnum, 0x17F);
	}
}

void InitPlrGFXMem(int pnum)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("InitPlrGFXMem: illegal player %d", pnum);
	}
	/// ASSERT: assert(! plr[pnum]._pNData);

	if(!(plr_gfx_flag & 1)) {
		plr_gfx_flag |= 1;
		if(GetPlrGFXSize("AS") > GetPlrGFXSize("ST")) {
			plr_sframe_size = GetPlrGFXSize("AS");
		} else {
			plr_sframe_size = GetPlrGFXSize("ST");
		}
	}
	plr[pnum]._pNData = DiabloAllocPtr(plr_sframe_size);

	if(!(plr_gfx_flag & 2)) {
		plr_gfx_flag |= 2;
		if(GetPlrGFXSize("AW") > GetPlrGFXSize("WL")) {
			plr_wframe_size = GetPlrGFXSize("AW");
		} else {
			plr_wframe_size = GetPlrGFXSize("WL");
		}
	}
	plr[pnum]._pWData = DiabloAllocPtr(plr_wframe_size);

	if(!(plr_gfx_flag & 4)) {
		plr_gfx_flag |= 4;
		plr_aframe_size = GetPlrGFXSize("AT");
	}
	plr[pnum]._pAData = DiabloAllocPtr(plr_aframe_size);

	if(!(plr_gfx_flag & 8)) {
		plr_gfx_flag |= 8;
		plr_hframe_size = GetPlrGFXSize("HT");
	}
	plr[pnum]._pHData = DiabloAllocPtr(plr_hframe_size);

	if(!(plr_gfx_flag & 0x10)) {
		plr_gfx_flag |= 0x10;
		plr_lframe_size = GetPlrGFXSize("LM");
	}
	plr[pnum]._pLData = DiabloAllocPtr(plr_lframe_size);

	if(!(plr_gfx_flag & 0x20)) {
		plr_gfx_flag |= 0x20;
		plr_fframe_size = GetPlrGFXSize("FM");
	}
	plr[pnum]._pFData = DiabloAllocPtr(plr_fframe_size);

	if(!(plr_gfx_flag & 0x40)) {
		plr_gfx_flag |= 0x40;
		plr_qframe_size = GetPlrGFXSize("QM");
	}
	plr[pnum]._pTData = DiabloAllocPtr(plr_qframe_size);

	if(!(plr_gfx_flag & 0x80)) {
		plr_gfx_flag |= 0x80;
		plr_dframe_size = GetPlrGFXSize("DT");
	}
	plr[pnum]._pDData = DiabloAllocPtr(plr_dframe_size);

	if(!(plr_gfx_bflag & 1)) {
		plr_gfx_bflag |= 1;
		plr_bframe_size = GetPlrGFXSize("BL");
	}
	plr[pnum]._pBData = DiabloAllocPtr(plr_bframe_size);

	plr[pnum]._pGFXLoad = 0;
}

DWORD GetPlrGFXSize(char *szCel)
{
	int i, a, w;
	DWORD dwSize, dwMaxSize;
	HANDLE hsFile;
	char FileName[256];
	char Type[16];

	dwMaxSize = 0;

	for(i = 0; (DWORD)i < NUM_CLASSES; i++) {
		for(a = 0; ArmourChar[a] != '\0'; a++) {
			for(w = 0; WepChar[w] != '\0'; w++) {
				sprintf(Type, "%c%c%c", CharChar[i], ArmourChar[a], WepChar[w]);
				sprintf(FileName, "PlrGFX\\%s\\%s\\%s%s.CL2", ClassStrTbl[i], Type, Type, szCel);
				if(WOpenFile(FileName, &hsFile, 1)) {
					/// ASSERT: assert(hsFile);
					dwSize = WGetFileSize(hsFile, NULL);
					WCloseFile(hsFile);
					if(dwMaxSize <= dwSize) {
						dwMaxSize = dwSize;
					}
				}
			}
		}
	}

	return dwMaxSize;
}

void FreePlayerGFX(int pnum)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("FreePlayerGFX: illegal player %d", pnum);
	}

	MemFreeDbg(plr[pnum]._pNData);
	MemFreeDbg(plr[pnum]._pWData);
	MemFreeDbg(plr[pnum]._pAData);
	MemFreeDbg(plr[pnum]._pHData);
	MemFreeDbg(plr[pnum]._pLData);
	MemFreeDbg(plr[pnum]._pFData);
	MemFreeDbg(plr[pnum]._pTData);
	MemFreeDbg(plr[pnum]._pDData);
	MemFreeDbg(plr[pnum]._pBData);

	plr[pnum]._pGFXLoad = 0;
}

void NewPlrAnim(int pnum, unsigned char *Peq, int numFrames, int Delay, int width)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("NewPlrAnim: illegal player %d", pnum);
	}

	plr[pnum]._pAnimData = Peq;
	plr[pnum]._pAnimLen = numFrames;
	plr[pnum]._pAnimFrame = 1;
	plr[pnum]._pAnimCnt = 0;
	plr[pnum]._pAnimDelay = Delay;
	plr[pnum]._pAnimWidth = width;
	plr[pnum]._pAnimWidth2 = (width - 64) >> 1;
}

void ClearPlrPVars(int pnum)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("ClearPlrPVars: illegal player %d", pnum);
	}

	plr[pnum]._pVar1 = 0;
	plr[pnum]._pVar2 = 0;
	plr[pnum]._pVar3 = 0;
	plr[pnum]._pVar4 = 0;
	plr[pnum]._pVar5 = 0;
	plr[pnum]._pVar6 = 0;
	plr[pnum]._pVar7 = 0;
	plr[pnum]._pVar8 = 0;
}

void SetPlrAnims(int pnum)
{
	int gn, pc;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("SetPlrAnims: illegal player %d", pnum);
	}

	plr[pnum]._pNWidth = 96;
	plr[pnum]._pWWidth = 96;
	plr[pnum]._pAWidth = 128;
	plr[pnum]._pHWidth = 96;
	plr[pnum]._pSWidth = 96;
	plr[pnum]._pDWidth = 128;
	plr[pnum]._pBWidth = 96;

	pc = plr[pnum]._pClass;

	if(leveltype == DTYPE_TOWN) {
		plr[pnum]._pNFrames = PlrGFXAnimLens[pc][7];
		plr[pnum]._pWFrames = PlrGFXAnimLens[pc][8];
		plr[pnum]._pDFrames = PlrGFXAnimLens[pc][4];
		plr[pnum]._pSFrames = PlrGFXAnimLens[pc][5];
		plr[pnum]._pSFNum = PlrGFXAnimLens[pc][10];
	} else {
		plr[pnum]._pNFrames = PlrGFXAnimLens[pc][0];
		plr[pnum]._pWFrames = PlrGFXAnimLens[pc][2];
		plr[pnum]._pAFrames = PlrGFXAnimLens[pc][1];
		plr[pnum]._pHFrames = PlrGFXAnimLens[pc][6];
		plr[pnum]._pSFrames = PlrGFXAnimLens[pc][5];
		plr[pnum]._pDFrames = PlrGFXAnimLens[pc][4];
		plr[pnum]._pBFrames = PlrGFXAnimLens[pc][3];
		plr[pnum]._pAFNum = PlrGFXAnimLens[pc][9];
		plr[pnum]._pSFNum = PlrGFXAnimLens[pc][10];
	}

	gn = plr[pnum]._pgfxnum & 0xF;

	if(pc == PC_WARRIOR) {
		if(gn == 4) {
			if(leveltype != DTYPE_TOWN) {
				plr[pnum]._pNFrames = 8;
			}
			plr[pnum]._pAWidth = 96;
			plr[pnum]._pAFNum = 11;
		} else if(gn == 5) {
			plr[pnum]._pAFrames = 20;
			plr[pnum]._pAFNum = 10;
		} else if(gn == 8) {
			plr[pnum]._pAFrames = 16;
			plr[pnum]._pAFNum = 11;
		}
	} else if(pc == PC_ROGUE) {
		if(gn == 5) {
			plr[pnum]._pAFrames = 22;
			plr[pnum]._pAFNum = 13;
		} else if(gn == 4) {
			plr[pnum]._pAFrames = 12;
			plr[pnum]._pAFNum = 7;
		} else if(gn == 8) {
			plr[pnum]._pAFrames = 16;
			plr[pnum]._pAFNum = 11;
		}
	} else if(pc == PC_SORCERER) {
		plr[pnum]._pSWidth = 128;
		if(gn == 0) {
			plr[pnum]._pAFrames = 20;
		} else if(gn == 1) {
			plr[pnum]._pAFNum = 9;
		} else if(gn == 4) {
			plr[pnum]._pAFrames = 20;
			plr[pnum]._pAFNum = 16;
		} else if(gn == 5) {
			plr[pnum]._pAFrames = 24;
			plr[pnum]._pAFNum = 16;
		}
	}
}

void ClearPlrRVars(PlayerStruct *p)
{
	/// ASSERT: assert(p != NULL);

	p->bReserved[0] = 0;
	p->bReserved[1] = 0;
	p->bReserved[2] = 0;
	p->wReserved[0] = 0;
	p->wReserved[1] = 0;
	p->wReserved[2] = 0;
	p->wReserved[3] = 0;
	p->wReserved[4] = 0;
	p->wReserved[5] = 0;
	p->wReserved[6] = 0;
	p->wReserved[7] = 0;
	p->dwReserved[0] = 0;
	p->dwReserved[1] = 0;
	p->dwReserved[2] = 0;
	p->dwReserved[3] = 0;
	p->dwReserved[4] = 0;
	p->dwReserved[5] = 0;
	p->dwReserved[6] = 0;
}

void CreatePlayer(int pnum, char c)
{
	int i;
	char vc;

	ClearPlrRVars(&plr[pnum]);
	SetRndSeed(GetTickCount());

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("CreatePlayer: illegal player %d", pnum);
	}

	plr[pnum]._pClass = c;

	vc = StrengthTbl[c];
	if(vc < 0) {
		vc = 0;
	}
	plr[pnum]._pStrength = vc;
	plr[pnum]._pBaseStr = vc;
	vc = MagicTbl[c];
	if(vc < 0) {
		vc = 0;
	}
	plr[pnum]._pMagic = vc;
	plr[pnum]._pBaseMag = vc;
	vc = DexterityTbl[c];
	if(vc < 0) {
		vc = 0;
	}
	plr[pnum]._pDexterity = vc;
	plr[pnum]._pBaseDex = vc;
	vc = VitalityTbl[c];
	if(vc < 0) {
		vc = 0;
	}
	plr[pnum]._pVitality = vc;
	plr[pnum]._pBaseVit = vc;

	plr[pnum]._pStatPts = 0;
	plr[pnum].pTownWarps = 0;
	plr[pnum].pDungMsgs = 0;
	plr[pnum].pLvlLoad = 0;
	plr[pnum].pDiabloKillLevel = 0;

	if(plr[pnum]._pClass == PC_ROGUE) {
		plr[pnum]._pDamageMod = plr[pnum]._pLevel * (plr[pnum]._pStrength + plr[pnum]._pDexterity) / 200;
	} else {
		plr[pnum]._pDamageMod = plr[pnum]._pLevel * plr[pnum]._pStrength / 100;
	}

	plr[pnum]._pBaseToBlk = ToBlkTbl[c];
	plr[pnum]._pHitPoints = (plr[pnum]._pVitality + 10) << 6;
	if(plr[pnum]._pClass == PC_WARRIOR) {
		plr[pnum]._pHitPoints *= 2;
	}
	if(plr[pnum]._pClass == PC_ROGUE) {
		plr[pnum]._pHitPoints += plr[pnum]._pHitPoints >> 1;
	}

	plr[pnum]._pMaxHP = plr[pnum]._pHitPoints;
	plr[pnum]._pHPBase = plr[pnum]._pHitPoints;
	plr[pnum]._pMaxHPBase = plr[pnum]._pHitPoints;

	plr[pnum]._pMana = plr[pnum]._pMagic << 6;
	if(plr[pnum]._pClass == PC_SORCERER) {
		plr[pnum]._pMana *= 2;
	}
	if(plr[pnum]._pClass == PC_ROGUE) {
		plr[pnum]._pMana += plr[pnum]._pMana >> 1;
	}

	plr[pnum]._pMaxMana = plr[pnum]._pMana;
	plr[pnum]._pManaBase = plr[pnum]._pMana;
	plr[pnum]._pMaxManaBase = plr[pnum]._pMana;

	plr[pnum]._pLevel = 1;
	plr[pnum]._pMaxLvl = plr[pnum]._pLevel;
	plr[pnum]._pExperience = 0;
	plr[pnum]._pMaxExp = plr[pnum]._pExperience;
	plr[pnum]._pNextExper = ExpLvlsTbl[1];
	plr[pnum]._pArmorClass = 0;
	plr[pnum]._pMagResist = 0;
	plr[pnum]._pFireResist = 0;
	plr[pnum]._pLghtResist = 0;
	plr[pnum]._pLightRad = 10;
	plr[pnum]._pInfraFlag = 0;

	if(c == PC_WARRIOR) {
		plr[pnum]._pAblSpells64 = (__int64)1 << (SPL_REPAIR - 1);
	} else if(c == PC_ROGUE) {
		plr[pnum]._pAblSpells64 = (__int64)1 << (SPL_DISARM - 1);
	} else if(c == PC_SORCERER) {
		plr[pnum]._pAblSpells64 = (__int64)1 << (SPL_RECHARGE - 1);
	}

	if(c == PC_SORCERER) {
		plr[pnum]._pMemSpells64 = (__int64)1 << (SPL_FIREBOLT - 1);
	} else {
		plr[pnum]._pMemSpells64 = 0;
	}

	for(i = 0; i < 64; i++) {
		plr[pnum]._pSplLvl[i] = 0;
	}

	plr[pnum]._pSpellFlags = 0;

	if(plr[pnum]._pClass == PC_SORCERER) {
		plr[pnum]._pSplLvl[SPL_FIREBOLT] = 2;
	}

	for(i = 0; i < 3; i++) { /// BUGFIX: clear all 4 hotkeys instead of 3 (demo leftover)
		plr[pnum]._pSplHotKey[i] = -1;
	}

	if(c == PC_WARRIOR) {
		plr[pnum]._pgfxnum = 3;
	} else if(c == PC_ROGUE) {
		plr[pnum]._pgfxnum = 4;
	} else if(c == PC_SORCERER) {
		plr[pnum]._pgfxnum = 8;
	}

	for(i = 0; i < NUMLEVELS; i++) {
		plr[pnum]._pLvlVisited[i] = 0;
	}

	for(i = 0; i < 10; i++) { /// BUGFIX: clear all 17 bytes
		plr[pnum]._pSLvlVisited[i] = 0;
	}

	plr[pnum]._pLvlChanging = 0;
	plr[pnum].pTownWarps = 0;
	plr[pnum].pLvlLoad = 0;
	plr[pnum].pBattleNet = 0;
	plr[pnum].pManaShield = 0;
	InitDungMsgs(pnum);
	CreatePlrItems(pnum);
	SetRndSeed(0);
}

int CalcStatDiff(int pnum)
{
	int c, d;

	c = plr[pnum]._pClass;
	d = MaxStats[c][ATTRIB_STR] - plr[pnum]._pBaseStr;
	d += MaxStats[c][ATTRIB_MAG] - plr[pnum]._pBaseMag;
	d += MaxStats[c][ATTRIB_DEX] - plr[pnum]._pBaseDex;
	d += MaxStats[c][ATTRIB_VIT] - plr[pnum]._pBaseVit;

	return d;
}

void NextPlrLevel(int pnum)
{
	long l;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("NextPlrLevel: illegal player %d", pnum);
	}

	plr[pnum]._pLevel++;
	plr[pnum]._pMaxLvl++;

	if(CalcStatDiff(pnum) < 5) {
		plr[pnum]._pStatPts = CalcStatDiff(pnum);
	} else {
		plr[pnum]._pStatPts += 5;
	}

	plr[pnum]._pNextExper = ExpLvlsTbl[plr[pnum]._pLevel];

	l = plr[pnum]._pClass == PC_SORCERER ? 64 : 128;
	if(gbMaxPlayers == 1) {
		l++;
	}
	plr[pnum]._pMaxHP += l;
	plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
	plr[pnum]._pMaxHPBase += l;
	plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;

	if(pnum == myplr) {
		drawhpflag = 1;
	}

	l = plr[pnum]._pClass == PC_WARRIOR ? 64 : 128;
	if(gbMaxPlayers == 1) {
		l++;
	}
	plr[pnum]._pMaxMana += l;
	plr[pnum]._pMaxManaBase += l;

	if(!(plr[pnum]._pIFlags & ISPL_NOMANA)) {
		plr[pnum]._pMana = plr[pnum]._pMaxMana;
		plr[pnum]._pManaBase = plr[pnum]._pMaxManaBase;
	}
	if(pnum == myplr) {
		drawmanaflag = 1;
	}
}

void AddPlrExperience(int pnum, int lvl, long exp)
{
	int i, l; /* omp */
	long e, lLevel, lMax;

	if(pnum != myplr) {
		return;
	}
	if((DWORD)myplr >= MAX_PLRS) {
		app_fatal("AddPlrExperience: illegal player %d", myplr);
	}
	if(plr[myplr]._pHitPoints <= 0) {
		return;
	}

	e = exp * (((double)lvl - (double)plr[pnum]._pLevel) / 10 + 1);
	if(e < 0) {
		e = 0;
	}
	if(gbMaxPlayers > 1) {
		lLevel = plr[pnum]._pLevel < 0 ? 0 : plr[pnum]._pLevel;
		if(lLevel >= 50) {
			lLevel = 50;
		}
		lMax = ExpLvlsTbl[lLevel] / 20;
		if(e >= lMax) {
			e = lMax;
		}
		lMax = 200 * lLevel;
		if(e >= lMax) {
			e = lMax;
		}
	}
	plr[pnum]._pExperience += e;
	if((DWORD)plr[pnum]._pExperience > MAXEXP) {
		plr[pnum]._pExperience = MAXEXP;
	}

	if(plr[pnum]._pExperience >= ExpLvlsTbl[49]) {
		plr[pnum]._pLevel = 50;
	} else {
		l = 0;
		while(plr[pnum]._pExperience >= ExpLvlsTbl[l]) {
			l++;
		}
		if(l != plr[pnum]._pLevel) {
			for(i = l - plr[pnum]._pLevel; i > 0; i--) {
				NextPlrLevel(pnum);
			}
		}
		NetSendCmdParam1(FALSE, CMD_PLRLEVEL, plr[myplr]._pLevel);
	}
}

void AddPlrMonstExper(int lvl, long exp, char pmask)
{
	int totplrs, i;

	totplrs = 0;
	for(i = 0; i < MAX_PLRS; i++) {
		if(pmask & (1 << i)) {
			totplrs++;
		}
	}
	if(totplrs != 0 && pmask & (1 << myplr)) {
		AddPlrExperience(myplr, lvl, exp / totplrs);
	}
}

void InitPlayer(int pnum, BOOL FirstTime)
{
	int i;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("InitPlayer: illegal player %d", pnum);
	}

	ClearPlrRVars(&plr[pnum]);

	if(FirstTime) {
		plr[pnum]._pRSplType = RSPLTYPE_INVALID;
		plr[pnum]._pRSpell = -1;
		plr[pnum]._pSBkSpell = -1;
		plr[pnum]._pSpell = plr[pnum]._pRSpell;
		plr[pnum]._pSplType = plr[pnum]._pRSplType;
		if((plr[pnum]._pgfxnum & 0xF) == 4) {
			plr[pnum]._pwtype = 1;
		} else {
			plr[pnum]._pwtype = 0;
		}
		plr[pnum].pManaShield = 0;
	}
	if(plr[pnum].plrlevel == currlevel || leveldebug) {
		SetPlrAnims(pnum);
		plr[pnum]._pxoff = 0;
		plr[pnum]._pyoff = 0;
		plr[pnum]._pxvel = 0;
		plr[pnum]._pyvel = 0;
		ClearPlrPVars(pnum);
		if(plr[pnum]._pHitPoints >> 6 > 0) {
			plr[pnum]._pmode = PM_STAND;
			NewPlrAnim(pnum, plr[pnum]._pNAnim[0], plr[pnum]._pNFrames, 3, plr[pnum]._pNWidth);
			plr[pnum]._pAnimFrame = random(2, plr[pnum]._pNFrames - 1) + 1;
			plr[pnum]._pAnimCnt = random(2, 3);
		} else {
			plr[pnum]._pmode = PM_DEATH;
			NewPlrAnim(pnum, plr[pnum]._pDAnim[0], plr[pnum]._pDFrames, 1, plr[pnum]._pDWidth);
			plr[pnum]._pAnimFrame = plr[pnum]._pAnimLen - 1;
			plr[pnum]._pVar8 = 2 * plr[pnum]._pAnimLen;
		}
		plr[pnum]._pdir = 0;
		plr[pnum]._peflag = 0;
		if(pnum == myplr) {
			if(!FirstTime || currlevel != 0) {
				plr[pnum].WorldX = ViewX;
				plr[pnum].WorldY = ViewY;
			}
			plr[pnum]._ptargx = plr[pnum].WorldX;
			plr[pnum]._ptargy = plr[pnum].WorldY;
		} else {
			plr[pnum]._ptargx = plr[pnum].WorldX;
			plr[pnum]._ptargy = plr[pnum].WorldY;
			for(i = 0; (DWORD)i < 8; i++) {
				if(PosOkPlayer(pnum, plr[pnum].WorldX + plrxoff2[i], plr[pnum].WorldY + plryoff2[i])) {
					break;
				}
			}
			plr[pnum].WorldX += plrxoff2[i];
			plr[pnum].WorldY += plryoff2[i];
		}
		plr[pnum]._px = plr[pnum].WorldX;
		plr[pnum]._py = plr[pnum].WorldY;
		plr[pnum].walkpath[0] = -1;
		plr[pnum].destAction = -1;
		if(pnum == myplr) {
			plr[pnum]._plid = AddLight(plr[pnum].WorldX, plr[pnum].WorldY, plr[pnum]._pLightRad);
		} else {
			plr[pnum]._plid = -1;
		}
		plr[pnum]._pvid = AddVision(plr[pnum].WorldX, plr[pnum].WorldY, plr[pnum]._pLightRad, pnum == myplr);
	}

	if(plr[pnum]._pClass == PC_WARRIOR) {
		plr[pnum]._pAblSpells64 = (__int64)1 << (SPL_REPAIR - 1);
	} else if(plr[pnum]._pClass == PC_ROGUE) {
		plr[pnum]._pAblSpells64 = (__int64)1 << (SPL_DISARM - 1);
	} else if(plr[pnum]._pClass == PC_SORCERER) {
		plr[pnum]._pAblSpells64 = (__int64)1 << (SPL_RECHARGE - 1);
	}

#ifdef _DEBUG
	if(debug_mode_dollar_sign && FirstTime) {
		plr[pnum]._pMemSpells64 |= (__int64)1 << SPL_TELEPORT; /// BUGFIX: `(SPL_TELEPORT - 1)`
		if(plr[myplr]._pSplLvl[SPL_TELEPORT] == 0) { /// BUGFIX: use `pnum` instead of `myplr`
			plr[myplr]._pSplLvl[SPL_TELEPORT] = 1;
		}
	}
	if(debug_mode_key_inverted_v && FirstTime) {
		plr[pnum]._pMemSpells64 = 0xFFFFFFFFFFFFFFF; /// BUGFIX: `0xFFFFFFFFFFFFFFFF`
	}
#endif

	plr[pnum]._pNextExper = ExpLvlsTbl[plr[pnum]._pLevel];
	plr[pnum]._pInvincible = 0;

	if(pnum == myplr) {
		deathdelay = 0;
		deathflag = 0;
		ScrollInfo._sxoff = 0;
		ScrollInfo._syoff = 0;
		ScrollInfo._sdir = 0;
	}
}

void InitMultiView()
{
	if((DWORD)myplr >= MAX_PLRS) {
		app_fatal("InitPlayer: illegal player %d", myplr); /// BUGFIX: wrong function name in string
	}

	ViewX = plr[myplr].WorldX;
	ViewY = plr[myplr].WorldY;
}

void CheckEFlag(int pnum, BOOL flag)
{
	int i, x, y, m;
	MICROS *pMicro;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("InitPlayer: illegal player %d", pnum); /// BUGFIX: wrong function name in string
	}

	x = plr[pnum].WorldX - 1;
	y = plr[pnum].WorldY + 1;
	m = 0;
	pMicro = &dpiece_defs_map_1[IsometricCoord(x, y)];

	for(i = 2; i < 10; i++) {
		m |= pMicro->mt[i];
	}

	if(m | dSpecial[x][y] | nSolidTable[dPiece[x][y]]) {
		plr[pnum]._peflag = 1;
	} else {
		plr[pnum]._peflag = 0;
	}

	if(flag == TRUE && plr[pnum]._peflag == 1) {
		x = plr[pnum].WorldX;
		y = plr[pnum].WorldY + 2;
		m = 0;
		pMicro = &dpiece_defs_map_1[IsometricCoord(x, y)];

		for(i = 2; i < 10; i++) {
			m |= pMicro->mt[i];
		}

		if(!(m | dSpecial[x][y])) {
			x = plr[pnum].WorldX - 2;
			y = plr[pnum].WorldY + 1;
			m = 0;
			pMicro = &dpiece_defs_map_1[IsometricCoord(x, y)];

			for(i = 2; i < 10; i++) {
				m |= pMicro->mt[i];
			}

			if(m | dSpecial[x][y]) {
				plr[pnum]._peflag = 2;
			}
		}
	}
}

BOOL SolidLoc(int x, int y)
{
	if(x < 0 || y < 0 || x >= MAXDUNX || y >= MAXDUNY) {
		return FALSE;
	}

	return nSolidTable[dPiece[x][y]];
}

BOOL PlrDirOK(int pnum, int dir)
{
	int px, py;
	BOOL rv;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PlrDirOK: illegal player %d", pnum);
	}

	px = plr[pnum].WorldX + offset_x[dir];
	py = plr[pnum].WorldY + offset_y[dir];

	if(px < 0) {
		return FALSE;
	}
	if(dPiece[px][py] == 0) {
		return FALSE;
	}
	if(!PosOkPlayer(pnum, px, py)) {
		return FALSE;
	}

	rv = TRUE;
	if(rv && dir == DIR_E) {
		rv = !SolidLoc(px, py + 1) && !(dFlags[px][py + 1] & 0x20);
	}
	if(rv && dir == DIR_W) {
		rv = !SolidLoc(px + 1, py) && !(dFlags[px + 1][py] & 0x20);
	}

	return rv;
}

void PlrClrTrans(int x, int y)
{
	int i, j;
	char v;

	for(j = y - 1; j <= y + 1; j++) {
		for(i = x - 1; i <= x + 1; i++) {
			v = dTransVal[i][j];
			TransList[v] = 0;
		}
	}
}

void PlrDoTrans(int x, int y)
{
	int i, j;
	char v;

	if(leveltype != DTYPE_CATHEDRAL && leveltype != DTYPE_CATACOMBS) {
		TransList[1] = 1;
	} else {
		for(j = y - 1; j <= y + 1; j++) {
			for(i = x - 1; i <= x + 1; i++) {
				if(!nSolidTable[dPiece[i][j]]) {
					v = dTransVal[i][j];
					if(v != 0) {
						TransList[v] = 1;
					}
				}
			}
		}
	}
}

void SetPlayerOld(int pnum)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("SetPlayerOld: illegal player %d", pnum);
	}

	plr[pnum]._poldx = plr[pnum].WorldX;
	plr[pnum]._poldy = plr[pnum].WorldY;
}

void FixPlayerLocation(int pnum, int bDir)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("FixPlayerLocation: illegal player %d", pnum);
	}

	plr[pnum]._px = plr[pnum].WorldX;
	plr[pnum]._py = plr[pnum].WorldY;
	plr[pnum]._ptargx = plr[pnum].WorldX;
	plr[pnum]._ptargy = plr[pnum].WorldY;
	plr[pnum]._pxoff = 0;
	plr[pnum]._pyoff = 0;
	CheckEFlag(pnum, FALSE);
	plr[pnum]._pdir = bDir;

	if(pnum == myplr) {
		ScrollInfo._sxoff = 0;
		ScrollInfo._syoff = 0;
		ScrollInfo._sdir = 0;
		ViewX = plr[pnum].WorldX;
		ViewY = plr[pnum].WorldY;
	}
}

void StartStand(int pnum, int dir)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("StartStand: illegal player %d", pnum);
	}

	if(!plr[pnum]._pInvincible || plr[pnum]._pHitPoints != 0 || pnum != myplr) {
		if(!(plr[pnum]._pGFXLoad & 1)) {
			LoadPlrGFX(pnum, 1);
		}
		NewPlrAnim(pnum, plr[pnum]._pNAnim[dir], plr[pnum]._pNFrames, 3, plr[pnum]._pNWidth);
		plr[pnum]._pmode = PM_STAND;
		FixPlayerLocation(pnum, dir);
		FixPlrWalkTags(pnum);
		dPlayer[plr[pnum].WorldX][plr[pnum].WorldY] = pnum + 1;
		SetPlayerOld(pnum);
	} else {
		SyncPlrKill(pnum, -1);
	}
}

void StartWalkStand(int pnum)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("StartWalkStand: illegal player %d", pnum);
	}

	plr[pnum]._pmode = PM_STAND;
	plr[pnum]._px = plr[pnum].WorldX;
	plr[pnum]._py = plr[pnum].WorldY;
	plr[pnum]._pxoff = 0;
	plr[pnum]._pyoff = 0;
	CheckEFlag(pnum, FALSE);

	if(pnum == myplr) {
		ScrollInfo._sxoff = 0;
		ScrollInfo._syoff = 0;
		ScrollInfo._sdir = 0;
		ViewX = plr[pnum].WorldX;
		ViewY = plr[pnum].WorldY;
	}
}

void PM_ChangeLightOff(int pnum)
{
	int x, y, xmul, ymul, px, py, lx, ly;
	LightListStruct *l;
	static unsigned char fix[9] = { 0, 0, 3, 3, 3, 6, 6, 6, 8 };

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PM_ChangeLightOff: illegal player %d", pnum);
	}

	l = &LightList[plr[pnum]._plid];
	x = 2 * plr[pnum]._pyoff + plr[pnum]._pxoff;
	y = 2 * plr[pnum]._pyoff - plr[pnum]._pxoff;

	if(x < 0) {
		xmul = -1;
		x = -x;
	} else {
		xmul = 1;
	}

	if(y < 0) {
		ymul = -1;
		y = -y;
	} else {
		ymul = 1;
	}

	x >>= 3;
	y >>= 3;
	x *= xmul;
	y *= ymul;
	px = (l->_lx << 3) + x;
	py = (l->_ly << 3) + y;
	lx = (l->_lx << 3) + l->_xoff;
	ly = (l->_ly << 3) + l->_yoff;

	if(abs(px - lx) >= 3 || abs(py - ly) >= 3) {
		ChangeLightOff(plr[pnum]._plid, x, y);
	}
}

void PM_ChangeOffset(int pnum)
{
	int px, py;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PM_ChangeOffset: illegal player %d", pnum);
	}

	plr[pnum]._pVar8++;
	px = plr[pnum]._pVar6 >> 8;
	py = plr[pnum]._pVar7 >> 8;
	plr[pnum]._pVar6 += plr[pnum]._pxvel;
	plr[pnum]._pVar7 += plr[pnum]._pyvel;
	plr[pnum]._pxoff = plr[pnum]._pVar6 >> 8;
	plr[pnum]._pyoff = plr[pnum]._pVar7 >> 8;
	px -= plr[pnum]._pVar6 >> 8;
	py -= plr[pnum]._pVar7 >> 8;

	if(pnum == myplr && ScrollInfo._sdir != 0) {
		ScrollInfo._sxoff += px;
		ScrollInfo._syoff += py;
	}

	PM_ChangeLightOff(pnum);
}

void StartWalk(int pnum, int xvel, int yvel, int xadd, int yadd, int EndDir, int sdir)
{
	int px, py;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("StartWalk: illegal player %d", pnum);
	}

	if(plr[pnum]._pInvincible && plr[pnum]._pHitPoints == 0 && pnum == myplr) {
		SyncPlrKill(pnum, -1);
		return;
	}

	SetPlayerOld(pnum);
	px = xadd + plr[pnum].WorldX;
	py = yadd + plr[pnum].WorldY;

	if(!PlrDirOK(pnum, EndDir)) {
		return;
	}

	plr[pnum]._px = px;
	plr[pnum]._py = py;

	if(pnum == myplr) {
		ScrollInfo._sdx = plr[pnum].WorldX - ViewX;
		ScrollInfo._sdy = plr[pnum].WorldY - ViewY;
	}

	dPlayer[px][py] = -(pnum + 1);
	plr[pnum]._pmode = PM_WALK;
	plr[pnum]._pxvel = xvel;
	plr[pnum]._pyvel = yvel;
	plr[pnum]._pxoff = 0;
	plr[pnum]._pyoff = 0;
	plr[pnum]._pVar1 = xadd;
	plr[pnum]._pVar2 = yadd;
	plr[pnum]._pVar3 = EndDir;

	if(!(plr[pnum]._pGFXLoad & 2)) {
		LoadPlrGFX(pnum, 2);
	}

	NewPlrAnim(pnum, plr[pnum]._pWAnim[EndDir], plr[pnum]._pWFrames, 0, plr[pnum]._pWWidth);
	plr[pnum]._pdir = EndDir;
	plr[pnum]._pVar6 = 0;
	plr[pnum]._pVar7 = 0;
	plr[pnum]._pVar8 = 0;
	CheckEFlag(pnum, FALSE);

	if(pnum == myplr) {
		if(zoomflag) {
			if(abs(ScrollInfo._sdx) >= 3 || abs(ScrollInfo._sdy) >= 3) {
				ScrollInfo._sdir = 0;
			} else {
				ScrollInfo._sdir = sdir;
			}
		} else {
			if(abs(ScrollInfo._sdx) >= 2 || abs(ScrollInfo._sdy) >= 2) {
				ScrollInfo._sdir = 0;
			} else {
				ScrollInfo._sdir = sdir;
			}
		}
	}
}

void StartWalk2(int pnum, int xvel, int yvel, int xoff, int yoff, int xadd, int yadd, int EndDir, int sdir)
{
	int v9; // edi
	int v10; // esi
	int v11; // ebx
	int v12; // edi
	_bool v13; // zf
	int v14; // eax
	int v15; // ecx
	int v16; // ecx
	int v17; // ecx
	int v18; // ST08_4
	_bool v19; // edx
	int v20; // eax
	_bool v21; // sf
	unsigned char v22; // of
	int v23; // eax
	int v24; // [esp+Ch] [ebp-8h]
	int arglist; // [esp+10h] [ebp-4h]
	int x; // [esp+28h] [ebp+14h]

	v9 = pnum;
	v24 = xvel;
	arglist = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("StartWalk2: illegal player %d", pnum);
	v10 = v9;
	if ( plr[v9]._pInvincible && !plr[v10]._pHitPoints && v9 == myplr )
	{
		SyncPlrKill(v9, -1);
		return;
	}
	SetPlayerOld(v9);
	v11 = xadd + plr[v10].WorldX;
	v12 = yadd + plr[v10].WorldY;
	x = xadd + plr[v10].WorldX;
	if ( PlrDirOK(arglist, EndDir) )
	{
		plr[v10]._px = v11;
		v13 = arglist == myplr;
		plr[v10]._py = v12;
		if ( v13 )
		{
			ScrollInfo._sdx = plr[v10].WorldX - ViewX;
			ScrollInfo._sdy = plr[v10].WorldY - ViewY;
		}
		v14 = plr[v10].WorldY;
		v15 = plr[v10].WorldX;
		plr[v10]._pVar2 = v14;
		dPlayer[v15][v14] = -1 - arglist;
		v16 = plr[v10].WorldX;
		plr[v10].WorldX = v11;
		dPlayer[v11][v12] = arglist + 1;
		plr[v10]._pVar1 = v16;
		v17 = plr[v10]._plid;
		plr[v10].WorldY = v12;
		plr[v10]._pxoff = xoff;
		plr[v10]._pyoff = yoff;
		ChangeLightXY(v17, x, v12);
		PM_ChangeLightOff(arglist);
		plr[v10]._pxvel = v24;
		plr[v10]._pyvel = yvel;
		plr[v10]._pVar6 = xoff << 8;
		v13 = (plr[v10]._pGFXLoad & 2) == 0;
		plr[v10]._pmode = PM_WALK2;
		plr[v10]._pVar7 = yoff << 8;
		plr[v10]._pVar3 = EndDir;
		if ( v13 )
			LoadPlrGFX(arglist, PM_WALK2);
		v18 = plr[v10]._pWWidth;
		NewPlrAnim(arglist, plr[0]._pWAnim[EndDir + 5430 * arglist], plr[v10]._pWFrames, 0, v18);
		plr[v10]._pVar8 = 0;
		v19 = 0;
		plr[v10]._pdir = EndDir;
		if ( EndDir == 7 )
			v19 = 1;
		CheckEFlag(arglist, v19);
		if ( arglist == myplr )
		{
			if ( zoomflag )
			{
				if ( abs(ScrollInfo._sdx) < 3 )
				{
					v20 = abs(ScrollInfo._sdy);
					v22 = __OFSUB__(v20, 3);
					v21 = v20 - 3 < 0;
					goto LABEL_20;
				}
			}
			else if ( abs(ScrollInfo._sdx) < PM_WALK2 )
			{
				v23 = abs(ScrollInfo._sdy);
				v22 = __OFSUB__(v23, 2);
				v21 = v23 - PM_WALK2 < 0;
LABEL_20:
				if ( v21 ^ v22 )
				{
					ScrollInfo._sdir = sdir;
					return;
				}
				goto LABEL_22;
			}
LABEL_22:
			ScrollInfo._sdir = 0;
			return;
		}
	}
}

void StartWalk3(int pnum, int xvel, int yvel, int xoff, int yoff, int xadd, int yadd, int mapx, int mapy, int EndDir, int sdir)
{
	int v11; // edi
	int v12; // esi
	int v13; // eax
	int v14; // ecx
	int v15; // ebx
	int v16; // edi
	_bool v17; // zf
	int v18; // edx
	int v19; // ecx
	int v20; // ST08_4
	int v21; // eax
	_bool v22; // sf
	unsigned char v23; // of
	int v24; // eax
	int v25; // [esp+10h] [ebp-8h]
	int arglist; // [esp+14h] [ebp-4h]
	int a6; // [esp+2Ch] [ebp+14h]
	int x; // [esp+30h] [ebp+18h]
	int y; // [esp+34h] [ebp+1Ch]

	v11 = pnum;
	v25 = xvel;
	arglist = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("StartWalk3: illegal player %d", pnum);
	v12 = v11;
	if ( plr[v11]._pInvincible && !plr[v12]._pHitPoints && v11 == myplr )
	{
		SyncPlrKill(v11, -1);
		return;
	}
	SetPlayerOld(v11);
	v13 = plr[v12].WorldX;
	a6 = v13 + xadd;
	v14 = plr[v12].WorldY;
	v15 = v14 + yadd;
	x = mapx + v13;
	v16 = v14 + mapy;
	y = v14 + mapy;
	if ( PlrDirOK(arglist, EndDir) )
	{
		v17 = arglist == myplr;
		plr[v12]._px = a6;
		plr[v12]._py = v15;
		if ( v17 )
		{
			ScrollInfo._sdx = plr[v12].WorldX - ViewX;
			ScrollInfo._sdy = plr[v12].WorldY - ViewY;
		}
		v18 = plr[v12].WorldY;
		v19 = plr[v12].WorldX;
		plr[v12]._pVar5 = v16;
		dPlayer[v19][v18] = -1 - arglist;
		dPlayer[a6][v15] = -1 - arglist;
		plr[v12]._pVar4 = x;
		plr[v12]._pyoff = yoff;
		dFlags[x][v16] |= 0x20u;
		v17 = leveltype == DTYPE_TOWN;
		plr[v12]._pxoff = xoff;
		if ( !v17 )
		{
			ChangeLightXY(plr[v12]._plid, x, y);
			PM_ChangeLightOff(arglist);
		}
		plr[v12]._pmode = PM_WALK3;
		plr[v12]._pxvel = v25;
		plr[v12]._pyvel = yvel;
		plr[v12]._pVar1 = a6;
		plr[v12]._pVar6 = xoff << 8;
		v17 = (plr[v12]._pGFXLoad & 2) == 0;
		plr[v12]._pVar7 = yoff << 8;
		plr[v12]._pVar2 = v15;
		plr[v12]._pVar3 = EndDir;
		if ( v17 )
			LoadPlrGFX(arglist, 2);
		v20 = plr[v12]._pWWidth;
		NewPlrAnim(arglist, plr[0]._pWAnim[EndDir + 5430 * arglist], plr[v12]._pWFrames, 0, v20);
		plr[v12]._pdir = EndDir;
		plr[v12]._pVar8 = 0;
		CheckEFlag(arglist, 0);
		if ( arglist == myplr )
		{
			if ( zoomflag )
			{
				if ( abs(ScrollInfo._sdx) < 3 )
				{
					v21 = abs(ScrollInfo._sdy);
					v23 = __OFSUB__(v21, 3);
					v22 = v21 - 3 < 0;
					goto LABEL_20;
				}
			}
			else if ( abs(ScrollInfo._sdx) < 2 )
			{
				v24 = abs(ScrollInfo._sdy);
				v23 = __OFSUB__(v24, 2);
				v22 = v24 - 2 < 0;
LABEL_20:
				if ( v22 ^ v23 )
				{
					ScrollInfo._sdir = sdir;
					return;
				}
				goto LABEL_22;
			}
LABEL_22:
			ScrollInfo._sdir = 0;
			return;
		}
	}
}

void StartAttack(int pnum, int d)
{
	int v2; // edi
	int v3; // ebp
	int v4; // esi
	int v5; // ST08_4

	v2 = pnum;
	v3 = d;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("StartAttack: illegal player %d", pnum);
	v4 = v2;
	if ( !plr[v2]._pInvincible || plr[v4]._pHitPoints || v2 != myplr )
	{
		if ( !(plr[v4]._pGFXLoad & 4) )
			LoadPlrGFX(v2, 4);
		v5 = plr[v4]._pAWidth;
		NewPlrAnim(v2, plr[0]._pAAnim[v3 + 5430 * v2], plr[v4]._pAFrames, 0, v5);
		plr[v4]._pmode = 4;
		FixPlayerLocation(v2, v3);
		SetPlayerOld(v2);
	}
	else
	{
		SyncPlrKill(v2, -1);
	}
}

void StartRangeAttack(int pnum, int d, int cx, int cy)
{
	int v4; // edi
	int v5; // esi
	int v6; // ST08_4
	int a2a; // [esp+8h] [ebp-4h]

	v4 = pnum;
	a2a = d;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("StartRangeAttack: illegal player %d", pnum);
	v5 = v4;
	if ( !plr[v4]._pInvincible || plr[v5]._pHitPoints || v4 != myplr )
	{
		if ( !(plr[v5]._pGFXLoad & 4) )
			LoadPlrGFX(v4, 4);
		v6 = plr[v5]._pAWidth;
		NewPlrAnim(v4, plr[0]._pAAnim[a2a + 5430 * v4], plr[v5]._pAFrames, 0, v6);
		plr[v5]._pmode = PM_RATTACK;
		FixPlayerLocation(v4, a2a);
		SetPlayerOld(v4);
		plr[v5]._pVar1 = cx;
		plr[v5]._pVar2 = cy;
	}
	else
	{
		SyncPlrKill(v4, -1);
	}
}

void StartPlrBlock(int pnum, int dir)
{
	int v2; // edi
	int v3; // ebx
	int v4; // esi
	int v5; // ST08_4

	v2 = pnum;
	v3 = dir;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("StartPlrBlock: illegal player %d", pnum);
	v4 = v2;
	if ( !plr[v2]._pInvincible || plr[v4]._pHitPoints || v2 != myplr )
	{
		PlaySfxLoc(IS_ISWORD, plr[v4].WorldX, plr[v4].WorldY);
		if ( !(plr[v4]._pGFXLoad & 0x100) )
			LoadPlrGFX(v2, 256);
		v5 = plr[v4]._pBWidth;
		NewPlrAnim(v2, plr[0]._pBAnim[v3 + 5430 * v2], plr[v4]._pBFrames, 2, v5);
		plr[v4]._pmode = PM_BLOCK;
		FixPlayerLocation(v2, v3);
		SetPlayerOld(v2);
	}
	else
	{
		SyncPlrKill(v2, -1);
	}
}

void StartSpell(int pnum, int d, int cx, int cy)
{
	int v4; // edi
	int v5; // esi
	unsigned char *v6; // edx
	int v7; // ST08_4
	int v8; // edx
	int a2; // [esp+Ch] [ebp-4h]

	v4 = pnum;
	a2 = d;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("StartSpell: illegal player %d", pnum);
	v5 = v4;
	if ( plr[v4]._pInvincible && !plr[v5]._pHitPoints && v4 == myplr )
	{
		SyncPlrKill(v4, -1);
		return;
	}
	if ( leveltype )
	{
		switch ( spelldata[plr[v5]._pSpell].sType )
		{
			case STYPE_FIRE:
				if ( !(plr[v5]._pGFXLoad & 0x20) )
					LoadPlrGFX(v4, 32);
				v6 = plr[0]._pFAnim[a2 + 5430 * v4];
				goto LABEL_20;
			case STYPE_LIGHTNING:
				if ( !(plr[v5]._pGFXLoad & 0x10) )
					LoadPlrGFX(v4, 16);
				v6 = plr[0]._pLAnim[a2 + 5430 * v4];
				goto LABEL_20;
			case STYPE_MAGIC:
				if ( !(plr[v5]._pGFXLoad & 0x40) )
					LoadPlrGFX(v4, 64);
				v6 = plr[0]._pTAnim[a2 + 5430 * v4];
LABEL_20:
				v7 = plr[v5]._pSWidth;
				NewPlrAnim(v4, v6, plr[v5]._pSFrames, 0, v7);
				break;
		}
	}
	PlaySfxLoc((unsigned char)spelldata[plr[v5]._pSpell].sSFX, plr[v5].WorldX, plr[v5].WorldY);
	plr[v5]._pmode = PM_SPELL;
	FixPlayerLocation(v4, a2);
	SetPlayerOld(v4);
	v8 = plr[v5]._pSpell;
	plr[v5]._pVar1 = cx;
	plr[v5]._pVar2 = cy;
	plr[v5]._pVar4 = GetSpellLevel(v4, v8);
	plr[v5]._pVar8 = 1;
}

void FixPlrWalkTags(int pnum)
{
	int v1; // esi
	int v2; // edx
	int v3; // ecx
	int v4; // eax
	int v5; // esi
	int v6; // edi
	int v7; // ebx
	int v8; // edi
	_bool v9; // zf
	_bool v10; // sf
	unsigned char v11; // of
	int v12; // eax
	int v13; // [esp+8h] [ebp-Ch]
	int v14; // [esp+Ch] [ebp-8h]
	char *v15; // [esp+10h] [ebp-4h]

	v1 = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("FixPlrWalkTags: illegal player %d", pnum);
	v13 = v1 + 1;
	v2 = -1 - v1;
	v3 = plr[v1]._poldx;
	v4 = plr[v1]._poldy;
	v5 = v4 - 1;
	if ( (unsigned char)(__OFSUB__(v4 - 1, v4 + 1) ^ 1) | (v4 - 1 == v4 + 1) )
	{
		v6 = v3 + 1;
		do
		{
			v7 = v3 - 1;
			v14 = v3 - 1;
			if ( v3 - 1 <= v6 )
			{
				v15 = &dPlayer[v7][v5];
				do
				{
					if ( v7 >= 0 && v7 < 112 && v5 >= 0 && v5 < 112 )
					{
						v8 = *v15;
						if ( v8 == v13 || v8 == v2 )
							*v15 = 0;
					}
					v15 += 112;
					v7 = v14 + 1;
					v6 = v3 + 1;
					v11 = __OFSUB__(v14 + 1, v3 + 1);
					v9 = v14 + 1 == v3 + 1;
					v10 = v14++ - v3 < 0;
				}
				while ( (unsigned char)(v10 ^ v11) | v9 );
			}
			++v5;
		}
		while ( v5 <= v4 + 1 );
	}
	if ( v3 >= 0 && v3 < 111 && v4 >= 0 && v4 < 111 )
	{
		v12 = 112 * v3 + v4;
		dFlags[1][v12] &= 0xDFu;
		dFlags[0][v12 + 1] &= 0xDFu;
	}
}

void RemovePlrFromMap(int pnum)
{
	int v1; // esi
	signed int v2; // edi
	signed int v3; // edx
	signed int v4; // ebx
	char v5; // al
	signed int v6; // edx
	_BYTE *v7; // eax
	signed int v8; // edi
	int v9; // ecx
	int v10; // [esp+Ch] [ebp-4h]

	v1 = -1 - pnum;
	v10 = pnum + 1;
	v2 = 1;
	do
	{
		v3 = v2;
		v4 = 111;
		do
		{
			if ( dPlayer[0][v3 + 111] == v1 || dPlayer[0][v3] == v1 )
			{
				v5 = dFlags[1][v3];
				if ( v5 & 0x20 )
					dFlags[1][v3] = v5 & 0xDF;
			}
			v3 += 112;
			--v4;
		}
		while ( v4 );
		++v2;
	}
	while ( v2 < 112 );
	v6 = 0;
	do
	{
		v7 = (unsigned char *)dPlayer + v6;
		v8 = 112;
		do
		{
			v9 = (char)*v7;
			if ( v9 == v10 || v9 == v1 )
				*v7 = 0;
			v7 += 112;
			--v8;
		}
		while ( v8 );
		++v6;
	}
	while ( v6 < 112 );
}

void StartPlrHit(int pnum, int dam, unsigned char forcehit)
{
	int v3; // ebx
	int v4; // edi
	int v5; // esi
	char v6; // al
	int v7; // ecx
	int v8; // eax
	int v9; // edi
	int v10; // ST08_4

	v3 = pnum;
	v4 = dam;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("StartPlrHit: illegal player %d", pnum);
	v5 = v3;
	if ( plr[v3]._pInvincible && !plr[v5]._pHitPoints && v3 == myplr )
	{
		SyncPlrKill(v3, -1);
		return;
	}
	v6 = plr[v5]._pClass;
	switch ( v6 )
	{
		case UI_WARRIOR:
			v7 = PS_WARR69;
LABEL_13:
			PlaySfxLoc(v7, plr[v5].WorldX, plr[v5].WorldY);
			break;
		case UI_ROGUE:
			v7 = PS_ROGUE69;
			goto LABEL_13;
		case UI_SORCERER:
			v7 = PS_MAGE69;
			goto LABEL_13;
	}
	v8 = plr[v5]._pLevel;
	drawhpflag = 1;
	if ( v4 >> 6 >= v8 || forcehit )
	{
		v9 = plr[v5]._pdir;
		if ( !(plr[v5]._pGFXLoad & 8) )
			LoadPlrGFX(v3, 8);
		v10 = plr[v5]._pHWidth;
		NewPlrAnim(v3, plr[0]._pHAnim[v9 + 5430 * v3], plr[v5]._pHFrames, 0, v10);
		plr[v5]._pmode = PM_GOTHIT;
		FixPlayerLocation(v3, v9);
		plr[v5]._pVar8 = 1;
		FixPlrWalkTags(v3);
		dPlayer[plr[v5].WorldX][plr[v5].WorldY] = v3 + 1;
		SetPlayerOld(v3);
	}
}

void RespawnDeadItem(ItemStruct *itm, int x, int y)
{
	int ii;

	if(numitems >= MAXITEMS) {
		return;
	}

	if(FindGetItem(itm->IDidx, itm->_iCreateInfo, itm->_iSeed) >= 0) {
		DrawInvMsg("A duplicate item has been detected.  Destroying duplicate...");
		SyncGetItem(x, y, itm->IDidx, itm->_iCreateInfo, itm->_iSeed);
	}

	ii = itemavail[0];
	dItem[x][y] = ii + 1;
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;
	item[ii] = *itm;
	item[ii]._ix = x;
	item[ii]._iy = y;
	RespawnItem(ii, TRUE);
	numitems++;
	itm->_itype = -1;
}

void StartPlayerKill(int pnum, int earflag)
{
	unsigned int v2; // edi
	unsigned int v3; // esi
	char v4; // al
	int v5; // ecx
	int v6; // ST0C_4
	_bool v7; // zf
	int *v8; // eax
	signed int v9; // ecx
	char *v10; // eax
	char v11; // al
	short v12; // cx
	short v13; // ax
	int v14; // ecx
	int v15; // eax
	signed int v17; // ebx
	int v18; // eax
	ItemStruct ear; // [esp+Ch] [ebp-178h]
	BOOL v20; // [esp+17Ch] [ebp-8h]
	struct ItemStruct *itm; // [esp+180h] [ebp-4h]

	v2 = pnum;
	v3 = 21720 * pnum;
	itm = (struct ItemStruct *)earflag;
	if ( plr[pnum]._pHitPoints <= 0 && plr[v3 / 0x54D8]._pmode == PM_DEATH )
		return;
	if ( myplr == pnum )
		NetSendCmdParam1(1u, CMD_PLRDEAD, earflag);
	v20 = (unsigned char)gbMaxPlayers > 1u && plr[v3 / 0x54D8].plrlevel == 16;
	if ( v2 >= 4 )
		app_fatal("StartPlayerKill: illegal player %d", v2);
	v4 = plr[v3 / 0x54D8]._pClass;
	if ( v4 )
	{
		if ( v4 == 1 )
		{
			v5 = PS_ROGUE71;
		}
		else
		{
			if ( v4 != 2 )
				goto LABEL_18;
			v5 = PS_MAGE71;
		}
		PlaySfxLoc(v5, plr[v3 / 0x54D8].WorldX, plr[v3 / 0x54D8].WorldY);
		goto LABEL_18;
	}
	PlaySfxLoc(PS_DEAD, plr[v3 / 0x54D8].WorldX, plr[v3 / 0x54D8].WorldY); /// BUGFIX: should use `PS_WARR71` like other classes
LABEL_18:
	if ( plr[v3 / 0x54D8]._pgfxnum )
	{
		plr[v3 / 0x54D8]._pgfxnum = 0;
		plr[v3 / 0x54D8]._pGFXLoad = 0;
		SetPlrAnims(v2);
	}
	if ( SLOBYTE(plr[v3 / 0x54D8]._pGFXLoad) >= 0 )
		LoadPlrGFX(v2, 128);
	v6 = plr[v3 / 0x54D8]._pDWidth;
	NewPlrAnim(v2, plr[0]._pDAnim[plr[v3 / 0x54D8]._pdir + v3 / 4], plr[v3 / 0x54D8]._pDFrames, 1, v6);
	plr[v3 / 0x54D8]._pBlockFlag = 0;
	plr[v3 / 0x54D8]._pmode = PM_DEATH;
	plr[v3 / 0x54D8]._pInvincible = 1;
	SetPlayerHitPoints(v2, 0);
	v7 = v2 == myplr;
	plr[v3 / 0x54D8]._pVar8 = 1;
	if ( !v7 && !itm && !v20 )
	{
		v8 = &plr[v3 / 0x54D8].InvBody[0]._itype;
		v9 = 7;
		do
		{
			*v8 = -1;
			v8 += 92;
			--v9;
		}
		while ( v9 );
		CalcPlrInv(v2, 0);
	}
	if ( plr[v3 / 0x54D8].plrlevel == currlevel )
	{
		FixPlayerLocation(v2, plr[v3 / 0x54D8]._pdir);
		RemovePlrFromMap(v2);
		v10 = &dFlags[plr[v3 / 0x54D8].WorldX][plr[v3 / 0x54D8].WorldY];
		*v10 |= 4u;
		SetPlayerOld(v2);
		if ( v2 == myplr )
		{
			drawhpflag = 1;
			deathdelay = 30;
			if ( pcurs >= CURSOR_FIRSTITEM )
			{
				PlrDeadItem(v2, &plr[v3 / 0x54D8].HoldItem, 0, 0);
				SetCursor_(CURSOR_HAND);
			}
			if ( !v20 )
			{
				DropHalfPlayersGold(v2);
				if ( itm != (struct ItemStruct *)-1 )
				{
					if ( itm )
					{
						SetPlrHandItem(&ear, IDI_EAR);
						sprintf(ear._iName, "Ear of %s", plr[v3 / 0x54D8]._pName);
						v11 = plr[v3 / 0x54D8]._pClass;
						if ( v11 == 2 )
						{
							ear._iCurs = 19;
						}
						else if ( v11 )
						{
							if ( v11 == 1 )
								ear._iCurs = 21;
						}
						else
						{
							ear._iCurs = 20;
						}
						_LOBYTE(v12) = 0;
						_HIBYTE(v12) = plr[v3 / 0x54D8]._pName[0];
						v13 = v12 | plr[v3 / 0x54D8]._pName[1];
						v14 = plr[v3 / 0x54D8]._pName[3];
						ear._iCreateInfo = v13;
						v15 = plr[v3 / 0x54D8]._pName[5] | ((plr[v3 / 0x54D8]._pName[4] | ((v14 | (plr[v3 / 0x54D8]._pName[2] << 8)) << 8)) << 8);
						ear._ivalue = plr[v3 / 0x54D8]._pLevel;
						ear._iSeed = v15;
						if ( FindGetItem(IDI_EAR, *(int *)&ear._iCreateInfo, v15) == -1 )
							PlrDeadItem(v2, &ear, 0, 0);
					}
					else
					{
						itm = plr[v3 / 0x54D8].InvBody;
						v17 = 7;
						do
						{
							v18 = ((_BYTE)--v17 + (unsigned char)plr[v3 / 0x54D8]._pdir) & 7;
							PlrDeadItem(v2, itm, offset_x[v18], offset_y[v18]);
							++itm;
						}
						while ( v17 );
						CalcPlrInv(v2, 0);
					}
				}
			}
		}
	}
	SetPlayerHitPoints(v2, 0);
}

void PlrDeadItem(int pnum, ItemStruct *itm, int xx, int yy)
{
	int i, j, l, x, y;

	if(itm->_itype == -1) {
		return;
	}
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PlrDeadItem: illegal player %d", pnum);
	}

	x = xx + plr[pnum].WorldX;
	y = yy + plr[pnum].WorldY;
	if((xx != 0 || yy != 0) && ItemSpaceOk(x, y)) {
		RespawnDeadItem(itm, x, y);
		plr[pnum].HoldItem = *itm;
		NetSendCmdPItem(FALSE, CMD_RESPAWNITEM, x, y);
	} else {
		for(l = 1; l < 50; l++) {
			for(j = -l; j <= l; j++) {
				y = j + plr[pnum].WorldY;
				for(i = -l; i <= l; i++) {
					x = i + plr[pnum].WorldX;
					if(ItemSpaceOk(x, y)) {
						RespawnDeadItem(itm, x, y);
						plr[pnum].HoldItem = *itm;
						NetSendCmdPItem(FALSE, CMD_RESPAWNITEM, x, y);
						return;
					}
				}
			}
		}
	}
}

void DropHalfPlayersGold(int pnum)
{
	int v1; // ebx
	int v2; // esi
	int v3; // edi
	int v4; // ecx
	int v5; // eax
	int v6; // ecx
	int v7; // eax
	int v8; // edx
	int v9; // ecx
	int v10; // eax
	int v11; // edx
	int v12; // ecx
	int v13; // eax
	int v14; // [esp+Ch] [ebp-8h]
	int v15; // [esp+Ch] [ebp-8h]
	int v16; // [esp+Ch] [ebp-8h]
	int v17; // [esp+Ch] [ebp-8h]
	signed int i; // [esp+10h] [ebp-4h]
	signed int ia; // [esp+10h] [ebp-4h]
	signed int ib; // [esp+10h] [ebp-4h]
	signed int ic; // [esp+10h] [ebp-4h]

	v1 = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("DropHalfPlayersGold: illegal player %d", pnum);
	v2 = v1;
	v3 = plr[v1]._pGold >> 1;
	i = 0;
	while ( v3 > 0 )
	{
		v4 = 368 * i + v2 * 21720;
		v14 = v4;
		if ( *(int *)((char *)&plr[0].SpdList[0]._itype + v4) == ITYPE_GOLD )
		{
			v5 = *(int *)((char *)&plr[0].SpdList[0]._ivalue + v4);
			if ( v5 != 5000 )
			{
				if ( v3 >= v5 )
				{
					v3 -= v5;
					RemoveSpdBarItem(v1, i);
					SetPlrHandItem(&plr[v2].HoldItem, IDI_GOLD);
					GetGoldSeed(v1, &plr[v2].HoldItem);
					SetPlrHandGoldCurs(&plr[v2].HoldItem);
					plr[v2].HoldItem._ivalue = *(int *)((char *)&plr[0].SpdList[0]._ivalue + v14);
					PlrDeadItem(v1, &plr[v2].HoldItem, 0, 0);
					i = -1;
				}
				else
				{
					*(int *)((char *)&plr[0].SpdList[0]._ivalue + v4) = v5 - v3;
					SetSpdbarGoldCurs(v1, i);
					SetPlrHandItem(&plr[v2].HoldItem, IDI_GOLD);
					GetGoldSeed(v1, &plr[v2].HoldItem);
					SetPlrHandGoldCurs(&plr[v2].HoldItem);
					plr[v2].HoldItem._ivalue = v3;
					v3 = 0;
					PlrDeadItem(v1, &plr[v2].HoldItem, 0, 0);
				}
			}
		}
		if ( ++i >= 8 )
		{
			if ( v3 > 0 )
			{
				ia = 0;
				do
				{
					if ( v3 <= 0 )
						break;
					v6 = 368 * ia + v2 * 21720;
					v15 = v6;
					if ( *(int *)((char *)&plr[0].SpdList[0]._itype + v6) == ITYPE_GOLD )
					{
						v7 = *(int *)((char *)&plr[0].SpdList[0]._ivalue + v6);
						if ( v3 >= v7 )
						{
							v3 -= v7;
							RemoveSpdBarItem(v1, ia);
							SetPlrHandItem(&plr[v2].HoldItem, IDI_GOLD);
							GetGoldSeed(v1, &plr[v2].HoldItem);
							SetPlrHandGoldCurs(&plr[v2].HoldItem);
							plr[v2].HoldItem._ivalue = *(int *)((char *)&plr[0].SpdList[0]._ivalue + v15);
							PlrDeadItem(v1, &plr[v2].HoldItem, 0, 0);
							ia = -1;
						}
						else
						{
							*(int *)((char *)&plr[0].SpdList[0]._ivalue + v6) = v7 - v3;
							SetSpdbarGoldCurs(v1, ia);
							SetPlrHandItem(&plr[v2].HoldItem, IDI_GOLD);
							GetGoldSeed(v1, &plr[v2].HoldItem);
							SetPlrHandGoldCurs(&plr[v2].HoldItem);
							plr[v2].HoldItem._ivalue = v3;
							v3 = 0;
							PlrDeadItem(v1, &plr[v2].HoldItem, 0, 0);
						}
					}
					++ia;
				}
				while ( ia < 8 );
			}
			break;
		}
	}
	v8 = 0;
	force_redraw = 255;
	if ( v3 > 0 )
	{
		ib = 0;
		if ( plr[v2]._pNumInv <= 0 )
		{
LABEL_28:
			if ( v3 > 0 )
			{
				v11 = 0;
				for ( ic = 0; ic < plr[v2]._pNumInv; v11 = ic++ + 1 )
				{
					if ( v3 <= 0 )
						break;
					v12 = 368 * v11 + v2 * 21720;
					v17 = v12;
					if ( *(int *)((char *)&plr[0].InvList[0]._itype + v12) == ITYPE_GOLD )
					{
						v13 = *(int *)((char *)&plr[0].InvList[0]._ivalue + v12);
						if ( v3 >= v13 )
						{
							v3 -= v13;
							RemoveInvItem(v1, v11);
							SetPlrHandItem(&plr[v2].HoldItem, IDI_GOLD);
							GetGoldSeed(v1, &plr[v2].HoldItem);
							SetPlrHandGoldCurs(&plr[v2].HoldItem);
							plr[v2].HoldItem._ivalue = *(int *)((char *)&plr[0].InvList[0]._ivalue + v17);
							PlrDeadItem(v1, &plr[v2].HoldItem, 0, 0);
							ic = -1;
						}
						else
						{
							*(int *)((char *)&plr[0].InvList[0]._ivalue + v12) = v13 - v3;
							SetGoldCurs(v1, v11);
							SetPlrHandItem(&plr[v2].HoldItem, IDI_GOLD);
							GetGoldSeed(v1, &plr[v2].HoldItem);
							SetPlrHandGoldCurs(&plr[v2].HoldItem);
							plr[v2].HoldItem._ivalue = v3;
							v3 = 0;
							PlrDeadItem(v1, &plr[v2].HoldItem, 0, 0);
						}
					}
				}
			}
		}
		else
		{
			while ( v3 > 0 )
			{
				v9 = 368 * v8 + v2 * 21720;
				v16 = v9;
				if ( *(int *)((char *)&plr[0].InvList[0]._itype + v9) == ITYPE_GOLD )
				{
					v10 = *(int *)((char *)&plr[0].InvList[0]._ivalue + v9);
					if ( v10 != 5000 )
					{
						if ( v3 >= v10 )
						{
							v3 -= v10;
							RemoveInvItem(v1, v8);
							SetPlrHandItem(&plr[v2].HoldItem, IDI_GOLD);
							GetGoldSeed(v1, &plr[v2].HoldItem);
							SetPlrHandGoldCurs(&plr[v2].HoldItem);
							plr[v2].HoldItem._ivalue = *(int *)((char *)&plr[0].InvList[0]._ivalue + v16);
							PlrDeadItem(v1, &plr[v2].HoldItem, 0, 0);
							ib = -1;
						}
						else
						{
							*(int *)((char *)&plr[0].InvList[0]._ivalue + v9) = v10 - v3;
							SetGoldCurs(v1, v8);
							SetPlrHandItem(&plr[v2].HoldItem, IDI_GOLD);
							GetGoldSeed(v1, &plr[v2].HoldItem);
							SetPlrHandGoldCurs(&plr[v2].HoldItem);
							plr[v2].HoldItem._ivalue = v3;
							v3 = 0;
							PlrDeadItem(v1, &plr[v2].HoldItem, 0, 0);
						}
					}
				}
				v8 = ib++ + 1;
				if ( ib >= plr[v2]._pNumInv )
					goto LABEL_28;
			}
		}
	}
	plr[v2]._pGold = CalculateGold(v1);
}

void SyncPlrKill(int pnum, int earflag)
{
	int i, mx;

	if(plr[pnum]._pHitPoints == 0 && currlevel == 0) {
		SetPlayerHitPoints(pnum, 64);
		return;
	}

	for(i = 0; i < nummissiles; i++) {
		mx = missileactive[i];
		if(missile[mx]._mitype == MIS_MANASHIELD && missile[mx]._misource == pnum && !missile[mx]._miDelFlag) {
			if(earflag != -1) {
				missile[mx]._miVar8 = earflag;
			}
			return;
		}
	}

	SetPlayerHitPoints(pnum, 0);
	StartPlayerKill(pnum, earflag);
}

void RemovePlrMissiles(int pnum)
{
	int i, mx;

	if(currlevel != 0 && pnum == myplr && (monster[myplr]._mx != 1 || monster[myplr]._my != 0)) {
		M_StartKill(myplr, myplr);
		AddDead(monster[myplr]._mx, monster[myplr]._my, monster[myplr].MType->mdeadval, monster[myplr]._mdir);
		dMonster[monster[myplr]._mx][monster[myplr]._my] = 0;
		monster[myplr]._mDelFlag = 1;
		DeleteMonsterList();
	}
	for(i = 0; i < nummissiles; i++) {
		mx = missileactive[i];
		if(missile[mx]._mitype == MIS_STONE && missile[mx]._misource == pnum) {
			monster[missile[mx]._miVar2]._mmode = missile[mx]._miVar1;
		}
		if(missile[mx]._mitype == MIS_MANASHIELD && missile[mx]._misource == pnum) {
			ClearMissileSpot(mx);
			DeleteMissile(mx, i);
		}
		if(missile[mx]._mitype == MIS_ETHEREALIZE && missile[mx]._misource == pnum) {
			ClearMissileSpot(mx);
			DeleteMissile(mx, i);
		}
	}
}

void InitLevelChange(int pnum)
{
	RemovePlrMissiles(pnum);

	if(pnum == myplr && qtextflag) {
		qtextflag = 0;
		stream_stop();
	}

	RemovePlrFromMap(pnum);
	SetPlayerOld(pnum);

	if(pnum == myplr) {
		dPlayer[plr[myplr].WorldX][plr[myplr].WorldY] = myplr + 1;
	} else {
		plr[pnum]._pLvlVisited[plr[pnum].plrlevel] = 1;
	}

	ClrPlrPath(pnum);
	plr[pnum].destAction = -1;
	plr[pnum]._pLvlChanging = 1;

	if(pnum == myplr) {
		plr[pnum].pLvlLoad = 10;
	}
}

void StartNewLvl(int pnum, int fom, int lvl)
{
	InitLevelChange(pnum);

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("StartNewLvl: illegal player %d", pnum);
	}

	switch(fom) {
	case WM_DIABNEXTLVL:
	case WM_DIABPREVLVL:
	case WM_DIABTOWNWARP:
		plr[pnum].plrlevel = lvl;
		break;
	case WM_DIABTWARPUP:
		plr[myplr].pTownWarps |= 1 << (leveltype - 2);
		plr[pnum].plrlevel = lvl;
		break;
	case WM_DIABRETOWN:
		break;
	case WM_DIABRTNLVL:
		/// ASSERT: assert(gbMaxPlayers == 1);
		plr[pnum].plrlevel = lvl;
		break;
	case WM_DIABSETLVL:
		setlvlnum = lvl;
		/// ASSERT: assert(gbMaxPlayers == 1);
		break;
	default:
		app_fatal("StartNewLvl");
		break;
	}

	if(pnum == myplr) {
		plr[pnum]._pmode = PM_NEWLVL;
		plr[pnum]._pInvincible = 1;
		PostMessage(ghMainWnd, fom, 0, 0);
		if(gbMaxPlayers > 1) {
			NetSendCmdParam2(TRUE, CMD_NEWLVL, fom, lvl);
		}
	}
}

void RestartTownLvl(int pnum)
{
	InitLevelChange(pnum);

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("RestartTownLvl: illegal player %d", pnum);
	}

	plr[pnum].plrlevel = 0;
	plr[pnum]._pInvincible = 0;
	SetPlayerHitPoints(pnum, 64);
	plr[pnum]._pMana = 0;
	plr[pnum]._pManaBase = plr[pnum]._pMaxManaBase - plr[pnum]._pMaxMana;
	CalcPlrInv(pnum, FALSE);

	if(pnum == myplr) {
		plr[pnum]._pmode = PM_NEWLVL;
		plr[pnum]._pInvincible = 1;
		PostMessage(ghMainWnd, WM_DIABRETOWN, 0, 0);
	}
}

void StartWarpLvl(int pnum, int pidx)
{
	InitLevelChange(pnum);

	if(gbMaxPlayers != 1) {
		if(plr[pnum].plrlevel != 0) {
			plr[pnum].plrlevel = 0;
		} else {
			plr[pnum].plrlevel = portal[pidx].level;
		}
	}
	if(pnum == myplr) {
		SetCurrentPortal(pidx);
		plr[pnum]._pmode = PM_NEWLVL;
		plr[pnum]._pInvincible = 1;
		PostMessage(ghMainWnd, WM_DIABWARPLVL, 0, 0);
	}
}

int PM_DoStand(int pnum)
{
	return FALSE;
}

int PM_DoWalk(int pnum)
{
	int l;
	BOOL rv;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PM_DoWalk: illegal player %d", pnum);
	}

	if(plr[pnum]._pAnimFrame == 3
	|| plr[pnum]._pWFrames == 8 && plr[pnum]._pAnimFrame == 7
	|| plr[pnum]._pWFrames != 8 && plr[pnum]._pAnimFrame == 4) {
		PlaySfxLoc(PS_WALK1, plr[pnum].WorldX, plr[pnum].WorldY);
	}

	l = 8;
	if(currlevel != 0) {
		l = WalkAnimTbl[plr[pnum]._pClass];
	}
	if(plr[pnum]._pVar8 == l) {
		dPlayer[plr[pnum].WorldX][plr[pnum].WorldY] = 0;
		plr[pnum].WorldX += plr[pnum]._pVar1;
		plr[pnum].WorldY += plr[pnum]._pVar2;
		dPlayer[plr[pnum].WorldX][plr[pnum].WorldY] = pnum + 1;
		if(leveltype != DTYPE_TOWN) {
			ChangeLightXY(plr[pnum]._plid, plr[pnum].WorldX, plr[pnum].WorldY);
			ChangeVisionXY(plr[pnum]._pvid, plr[pnum].WorldX, plr[pnum].WorldY);
		}
		if(pnum == myplr && ScrollInfo._sdir != 0) {
			ViewX = plr[pnum].WorldX - ScrollInfo._sdx;
			ViewY = plr[pnum].WorldY - ScrollInfo._sdy;
		}
		if(plr[pnum].walkpath[0] != -1) {
			StartWalkStand(pnum);
		} else {
			StartStand(pnum, plr[pnum]._pVar3);
		}
		ClearPlrPVars(pnum);
		if(leveltype != DTYPE_TOWN) {
			ChangeLightOff(plr[pnum]._plid, 0, 0);
		}
		rv = TRUE;
	} else {
		PM_ChangeOffset(pnum);
		rv = FALSE;
	}

	return rv;
}

int PM_DoWalk2(int pnum)
{
	int l;
	BOOL rv;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PM_DoWalk2: illegal player %d", pnum);
	}

	if(plr[pnum]._pAnimFrame == 3
	|| plr[pnum]._pWFrames == 8 && plr[pnum]._pAnimFrame == 7
	|| plr[pnum]._pWFrames != 8 && plr[pnum]._pAnimFrame == 4) {
		PlaySfxLoc(PS_WALK1, plr[pnum].WorldX, plr[pnum].WorldY);
	}

	l = 8;
	if(currlevel != 0) {
		l = WalkAnimTbl[plr[pnum]._pClass];
	}
	if(plr[pnum]._pVar8 == l) {
		dPlayer[plr[pnum]._pVar1][plr[pnum]._pVar2] = 0;
		if(leveltype != DTYPE_TOWN) {
			ChangeLightXY(plr[pnum]._plid, plr[pnum].WorldX, plr[pnum].WorldY);
			ChangeVisionXY(plr[pnum]._pvid, plr[pnum].WorldX, plr[pnum].WorldY);
		}
		if(pnum == myplr && ScrollInfo._sdir != 0) {
			ViewX = plr[pnum].WorldX - ScrollInfo._sdx;
			ViewY = plr[pnum].WorldY - ScrollInfo._sdy;
		}
		if(plr[pnum].walkpath[0] != -1) {
			StartWalkStand(pnum);
		} else {
			StartStand(pnum, plr[pnum]._pVar3);
		}
		ClearPlrPVars(pnum);
		if(leveltype != DTYPE_TOWN) {
			ChangeLightOff(plr[pnum]._plid, 0, 0);
		}
		rv = TRUE;
	} else {
		PM_ChangeOffset(pnum);
		rv = FALSE;
	}

	return rv;
}

int PM_DoWalk3(int pnum)
{
	int l;
	BOOL rv;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PM_DoWalk3: illegal player %d", pnum);
	}

	if(plr[pnum]._pAnimFrame == 3
	|| plr[pnum]._pWFrames == 8 && plr[pnum]._pAnimFrame == 7
	|| plr[pnum]._pWFrames != 8 && plr[pnum]._pAnimFrame == 4) {
		PlaySfxLoc(PS_WALK1, plr[pnum].WorldX, plr[pnum].WorldY);
	}

	l = 8;
	if(currlevel != 0) {
		l = WalkAnimTbl[plr[pnum]._pClass];
	}
	if(plr[pnum]._pVar8 == l) {
		dPlayer[plr[pnum].WorldX][plr[pnum].WorldY] = 0;
		dFlags[plr[pnum]._pVar4][plr[pnum]._pVar5] &= ~0x20;
		plr[pnum].WorldX = plr[pnum]._pVar1;
		plr[pnum].WorldY = plr[pnum]._pVar2;
		dPlayer[plr[pnum].WorldX][plr[pnum].WorldY] = pnum + 1;
		if(leveltype != DTYPE_TOWN) {
			ChangeLightXY(plr[pnum]._plid, plr[pnum].WorldX, plr[pnum].WorldY);
			ChangeVisionXY(plr[pnum]._pvid, plr[pnum].WorldX, plr[pnum].WorldY);
		}
		if(pnum == myplr && ScrollInfo._sdir != 0) {
			ViewX = plr[pnum].WorldX - ScrollInfo._sdx;
			ViewY = plr[pnum].WorldY - ScrollInfo._sdy;
		}
		if(plr[pnum].walkpath[0] != -1) {
			StartWalkStand(pnum);
		} else {
			StartStand(pnum, plr[pnum]._pVar3);
		}
		ClearPlrPVars(pnum);
		if(leveltype != DTYPE_TOWN) {
			ChangeLightOff(plr[pnum]._plid, 0, 0);
		}
		rv = TRUE;
	} else {
		PM_ChangeOffset(pnum);
		rv = FALSE;
	}

	return rv;
}

BOOL WeaponDur(int pnum, int durrnd)
{
	if(pnum != myplr) {
		return FALSE;
	}
	if(random(3, durrnd) != 0) {
		return FALSE;
	}

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("WeaponDur: illegal player %d", pnum);
	}

	if(plr[pnum].InvBody[4]._itype != -1 && plr[pnum].InvBody[4]._iClass == ICLASS_WEAPON) {
		if(plr[pnum].InvBody[4]._iDurability == 255) {
			return FALSE;
		}
		plr[pnum].InvBody[4]._iDurability--;
		if(plr[pnum].InvBody[4]._iDurability == 0) {
			NetSendCmdDelItem(TRUE, 4);
			plr[pnum].InvBody[4]._itype = -1;
			CalcPlrInv(pnum, TRUE);
			return TRUE;
		}
	}
	if(plr[pnum].InvBody[5]._itype != -1 && plr[pnum].InvBody[5]._iClass == ICLASS_WEAPON) {
		if(plr[pnum].InvBody[5]._iDurability == 255) {
			return FALSE;
		}
		plr[pnum].InvBody[5]._iDurability--;
		if(plr[pnum].InvBody[5]._iDurability == 0) {
			NetSendCmdDelItem(TRUE, 5);
			plr[pnum].InvBody[5]._itype = -1;
			CalcPlrInv(pnum, TRUE);
			return TRUE;
		}
	}
	if(plr[pnum].InvBody[4]._itype == -1 && plr[pnum].InvBody[5]._itype == ITYPE_SHIELD) {
		if(plr[pnum].InvBody[5]._iDurability == 255) {
			return FALSE;
		}
		plr[pnum].InvBody[5]._iDurability--;
		if(plr[pnum].InvBody[5]._iDurability == 0) {
			NetSendCmdDelItem(TRUE, 5);
			plr[pnum].InvBody[5]._itype = -1;
			CalcPlrInv(pnum, TRUE);
			return TRUE;
		}
	}
	if(plr[pnum].InvBody[5]._itype == -1 && plr[pnum].InvBody[4]._itype == ITYPE_SHIELD) {
		if(plr[pnum].InvBody[4]._iDurability == 255) {
			return FALSE;
		}
		plr[pnum].InvBody[4]._iDurability--;
		if(plr[pnum].InvBody[4]._iDurability == 0) {
			NetSendCmdDelItem(TRUE, 4);
			plr[pnum].InvBody[4]._itype = -1;
			CalcPlrInv(pnum, TRUE);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL PlrHitMonst(int pnum, int m)
{
	int v2; // ebx
	unsigned int v3; // esi
	//int v4; // ST04_4
	int v5; // ebx
	//int v7; // ST04_4
	int v8; // eax
	unsigned int v9; // esi
	int v10; // ecx
	int v11; // eax
	int v12; // edi
	int v13; // edi
	//int v14; // eax
	int v16; // edx
	int v17; // eax
	int v18; // ecx
	int v19; // edi
	int v20; // eax
	int v21; // eax
	char v22; // dl
	_bool v23; // zf
	int v24; // eax
	int v25; // ecx
	int v26; // edi
	int v27; // eax
	int v28; // edx
	int *v29; // ecx
	int v30; // edx
	int *v31; // ecx
	int v32; // ecx
	int v33; // edx
	int *v34; // ecx
	int v35; // edx
	int *v36; // ecx
	int v37; // edx
	int *v38; // ecx
	int *v39; // ecx
	int v40; // esi
	BOOL ret; // [esp+Ch] [ebp-18h]
	_bool v42; // [esp+10h] [ebp-14h]
	int v48;
	int v43; // [esp+14h] [ebp-10h]
	int pnuma; // [esp+18h] [ebp-Ch]
	int arglist; // [esp+1Ch] [ebp-8h]
	int v46; // [esp+20h] [ebp-4h]

	v2 = m;
	v3 = pnum;
	arglist = m;
	pnuma = pnum;
	if ( (unsigned int)m >= 0xC8 )
	{
		app_fatal("PlrHitMonst: illegal monster %d", m);
		//pnum = v4;
	}
	v5 = 228 * v2;
	v43 = v5;
	if ( (signed int)(*(int *)((_BYTE *)&monster[0]._mhitpoints + v5) & 0xFFFFFFC0) <= 0
	  || **(_BYTE **)((char *)&monster[0].MType + v5) == MT_ILLWEAV && *((_BYTE *)&monster[0]._mgoal + v5) == 2
	  || *(MON_MODE *)((char *)&monster[0]._mmode + v5) == MM_CHARGE )
	{
		return 0;
	}
	if ( v3 >= 4 )
	{
		app_fatal("PlrHitMonst: illegal player %d", v3);
		//pnum = v7;
	}
	v42 = 0;
	v8 = random(4, 100);
	v23 = *(MON_MODE *)((char *)&monster[0]._mmode + v5) == MM_STONE;
	v46 = v8;
	if ( v23 )
		v46 = 0;
	v9 = v3;
	v10 = plr[v9]._pLevel;
	v11 = plr[v9]._pIEnAc + (plr[v9]._pDexterity >> 1) - *((unsigned char *)&monster[0].mArmorClass + v5);
	v12 = v11 + v10 + 50;
	if ( !_LOBYTE(plr[v9]._pClass) )
		v12 = v11 + v10 + 70;
	v13 = plr[v9]._pIBonusToHit + v12;
	if ( v13 < 5 )
		v13 = 5;
	if ( v13 > 95 )
		v13 = 95;
	if ( CheckMonsterHit(arglist, ret) )
		return ret;
#ifdef _DEBUG
	if ( (signed int)v46 < v13 || debug_mode_key_inverted_v || debug_mode_dollar_sign )
#else
	if ( (signed int)v46 < v13 )
#endif
	{
		v16 = plr[v9]._pIMaxDam - plr[v9]._pIMinDam + 1;
		v48 = plr[v9]._pIMinDam;
		v17 = random(5, v16);
		v18 = 100;
		v19 = plr[v9]._pIBonusDamMod + plr[v9]._pDamageMod + (v48 + v17) * plr[v9]._pIBonusDam / 100 + v48 + v17;
		if ( !_LOBYTE(plr[v9]._pClass) )
		{
			v48 = plr[v9]._pLevel;
			v20 = random(6, 100);
			if ( v20 < v48 )
				v19 *= 2;
		}
		v21 = plr[v9].InvBody[4]._itype;
		v46 = -1;
		if ( v21 == 1 || plr[v9].InvBody[5]._itype == 1 )
			v46 = 1;
		if ( v21 == ITYPE_MACE || plr[v9].InvBody[5]._itype == ITYPE_MACE )
			v46 = ITYPE_MACE;
		v22 = (*(MonsterData **)((char *)&monster[0].MData + v5))->mMonstClass;
		if ( v22 )
		{
			if ( v22 != 2 )
				goto LABEL_40;
			if ( v46 == ITYPE_MACE )
				v19 -= v19 >> 1;
			v23 = v46 == 1;
		}
		else
		{
			if ( v46 == 1 )
				v19 -= v19 >> 1;
			v23 = v46 == ITYPE_MACE;
		}
		if ( v23 )
			v19 += v19 >> 1;
LABEL_40:
		v24 = plr[v9]._pIFlags;
		if ( v24 & 0x40000000 && v22 == 1 )
			v19 *= 3;
		v25 = pnuma;
		v26 = v19 << 6;
		if ( pnuma == myplr )
			*(int *)((char *)&monster[0]._mhitpoints + v5) -= v26;
		if ( v24 & 2 )
		{
			v27 = random(7, v26 >> 3);
			v28 = plr[v9]._pMaxHP;
			v29 = &plr[v9]._pHitPoints;
			*v29 += v27;
			if ( plr[v9]._pHitPoints > v28 )
				*v29 = v28;
			v30 = plr[v9]._pMaxHPBase;
			v31 = &plr[v9]._pHPBase;
			*v31 += v27;
			if ( plr[v9]._pHPBase > v30 )
				*v31 = v30;
			v5 = v43;
			drawhpflag = 1;
		}
		else
		{
			v27 = ret;
		}
		v46 = plr[v9]._pIFlags;
		v32 = v46;
		if ( v32 & 0x6000 && !(v46 & 0x8000000) )
		{
			if ( v32 & 0x2000 )
				v27 = 3 * v26 / 100;
			if ( v32 & 0x4000 )
				v27 = 5 * v26 / 100;
			v33 = plr[v9]._pMaxMana;
			v34 = &plr[v9]._pMana;
			*v34 += v27;
			if ( plr[v9]._pMana > v33 )
				*v34 = v33;
			v35 = plr[v9]._pMaxManaBase;
			v36 = &plr[v9]._pManaBase;
			*v36 += v27;
			if ( plr[v9]._pManaBase > v35 )
				*v36 = v35;
			v5 = v43;
			v32 = v46;
			drawmanaflag = 1;
		}
		if ( v32 & 0x18000 )
		{
			if ( (v32 & 0x8000) != 0 )
				v27 = 3 * v26 / 100;
			if ( v32 & 0x10000 )
				v27 = 5 * v26 / 100;
			v37 = plr[v9]._pMaxHP;
			v38 = &plr[v9]._pHitPoints;
			*v38 += v27;
			if ( plr[v9]._pHitPoints > v37 )
				*v38 = v37;
			v39 = &plr[v9]._pHPBase;
			v40 = plr[v9]._pMaxHPBase;
			*v39 += v27;
			if ( *v39 > v40 )
				*v39 = v40;
			BYTE1(v32) = BYTE1(v46);
			v5 = v43;
			drawhpflag = 1;
		}
		if ( v32 & 0x100 )
			*(int *)((char *)&monster[0]._mFlags + v5) |= 8u;
#ifdef _DEBUG
		if ( debug_mode_dollar_sign || debug_mode_key_inverted_v )
			monster[m]._mhitpoints = 0; /* double check */
#endif
		if ( (signed int)(*(int *)((_BYTE *)&monster[0]._mhitpoints + v5) & 0xFFFFFFC0) > 0 )
		{
			if ( *(MON_MODE *)((char *)&monster[0]._mmode + v5) != MM_STONE )
			{
				if ( v32 & 0x800 )
					M_GetKnockback(arglist);
				M_StartHit(arglist, pnuma, v26);
				goto LABEL_85;
			}
			M_StartHit(arglist, pnuma, v26);
		}
		else
		{
			if ( *(MON_MODE *)((char *)&monster[0]._mmode + v5) != MM_STONE )
			{
				M_StartKill(arglist, pnuma);
				goto LABEL_85;
			}
			M_StartKill(arglist, pnuma);
		}
		*(MON_MODE *)((char *)&monster[0]._mmode + v5) = MM_STONE;
LABEL_85:
		v42 = 1;
	}
	return v42;
}

BOOL PlrHitPlr(int pnum, char p)
{
	char v2; // bl
	unsigned int v3; // esi
	//int v4; // ST04_4
	int v5; // edi
	//int v7; // ST04_4
	unsigned int v8; // esi
	int v9; // ecx
	int v10; // eax
	int v11; // ebx
	int v12; // ebx
	int v13; // eax
	int v14; // eax
	int v15; // ecx
	int v16; // eax
	int v17; // ebx
	int v18; // eax
	int v19; // ecx
	int v20; // edi
	int v21; // ebx
	signed int v22; // edi
	int v23; // eax
	int v24; // edx
	int *v25; // ecx
	int *v26; // ecx
	int v27; // esi
	int v28; // [esp+Ch] [ebp-14h]
	int v29; // [esp+10h] [ebp-10h]
	BOOL v30; // [esp+14h] [ebp-Ch]
	int arglist; // [esp+18h] [ebp-8h]
	char bPlr; // [esp+1Ch] [ebp-4h]

	v2 = p;
	v3 = pnum;
	bPlr = p;
	v28 = pnum;
	if ( (unsigned char)p >= 4u )
	{
		app_fatal("PlrHitPlr: illegal target player %d", p);
		//pnum = v4;
	}
	arglist = v2;
	v5 = v2;
	v30 = 0;
	if ( plr[v5]._pInvincible || plr[v5]._pSpellFlags & 1 )
		return 0;
	if ( v3 >= 4 )
	{
		app_fatal("PlrHitPlr: illegal attacking player %d", v3);
		//pnum = v7;
	}
	v8 = v3;
	v29 = random(4, 100);
	v9 = (plr[v8]._pDexterity >> 1) - plr[v5]._pIBonusAC - plr[v5]._pIAC - plr[v5]._pDexterity / 5;
	v10 = plr[v8]._pLevel;
	v11 = v9 + v10 + 50;
	if ( !_LOBYTE(plr[v8]._pClass) )
		v11 = v9 + v10 + 70;
	v12 = plr[v8]._pIBonusToHit + v11;
	if ( v12 < 5 )
		v12 = 5;
	if ( v12 > 95 )
		v12 = 95;
	v13 = plr[v5]._pmode;
	if ( v13 && v13 != 4 || !plr[v5]._pBlockFlag )
	{
		v14 = 100;
	}
	else
	{
		v14 = random(5, 100);
	}
	v15 = plr[v5]._pDexterity + plr[v5]._pBaseToBlk + 2 * plr[v5]._pLevel - 2 * plr[v8]._pLevel;
	if ( v15 < 0 )
		v15 = 0;
	if ( v15 > 100 )
		v15 = 100;
	if ( v29 < v12 )
	{
		if ( v14 >= v15 )
		{
			v17 = plr[v8]._pIMinDam;
			v18 = random(5, plr[v8]._pIMaxDam - v17 + 1);
			v19 = 100;
			v20 = plr[v8]._pIBonusDamMod + plr[v8]._pDamageMod + (v17 + v18) * plr[v8]._pIBonusDam / 100 + v17 + v18;
			if ( !_LOBYTE(plr[v8]._pClass) )
			{
				v21 = plr[v8]._pLevel;
				if ( random(6, 100) < v21 )
					v20 *= 2;
			}
			v22 = v20 << 6;
			if ( plr[v8]._pIFlags & 2 )
			{
				v23 = random(7, v22 >> 3);
				v24 = plr[v8]._pMaxHP;
				v25 = &plr[v8]._pHitPoints;
				*v25 += v23;
				if ( plr[v8]._pHitPoints > v24 )
					*v25 = v24;
				v26 = &plr[v8]._pHPBase;
				v27 = plr[v8]._pMaxHPBase;
				*v26 += v23;
				if ( *v26 > v27 )
					*v26 = v27;
				drawhpflag = 1;
			}
			if ( v28 == myplr )
				NetSendCmdDamage(1u, bPlr, v22);
			StartPlrHit(arglist, v22, 0);
		}
		else
		{
			v16 = GetDirection(plr[v5].WorldX, plr[v5].WorldY, plr[v8].WorldX, plr[v8].WorldY);
			StartPlrBlock(arglist, v16);
		}
		v30 = 1;
	}
	return v30;
}

BOOL PlrHitObj(int pnum, int mx, int my)
{
	int oi; // edx

	if ( dObject[mx][my] <= 0 )
		oi = -1 - dObject[mx][my];
	else
		oi = dObject[mx][my] - 1;

	if ( object[oi]._oBreak != 1 )
		return 0;

	BreakObject(pnum, oi);
	return 1;
}

int PM_DoAttack(int pnum)
{
	int dx, dy, m, frame;
	char p;
	BOOL didhit;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PM_DoAttack: illegal player %d", pnum);
	}

	frame = plr[pnum]._pAnimFrame;
	if(plr[pnum]._pIFlags & ISPL_QUICKATTACK && frame == 1) {
		plr[pnum]._pAnimFrame++;
	}
	if(plr[pnum]._pIFlags & ISPL_FASTATTACK && (frame == 1 || frame == 3)) {
		plr[pnum]._pAnimFrame++;
	}
	if(plr[pnum]._pIFlags & ISPL_FASTERATTACK && (frame == 1 || frame == 3 || frame == 5)) {
		plr[pnum]._pAnimFrame++;
	}
	if(plr[pnum]._pIFlags & ISPL_FASTESTATTACK && (frame == 1 || frame == 4)) {
		plr[pnum]._pAnimFrame += 2;
	}

	if(plr[pnum]._pAnimFrame == plr[pnum]._pAFNum - 1) {
		PlaySfxLoc(PS_SWING, plr[pnum].WorldX, plr[pnum].WorldY);
	}

	if(plr[pnum]._pAnimFrame == plr[pnum]._pAFNum) {
		dx = plr[pnum].WorldX + offset_x[plr[pnum]._pdir];
		dy = plr[pnum].WorldY + offset_y[plr[pnum]._pdir];
		if(dMonster[dx][dy] != 0) {
			m = dMonster[dx][dy] > 0 ? dMonster[dx][dy] - 1 : -(dMonster[dx][dy] + 1);
			if(CanTalkToMonst(m)) {
				plr[pnum]._pVar1 = 0;
				return FALSE;
			}
		}
		if(plr[pnum]._pIFlags & ISPL_FIREDAM) {
			AddMissile(dx, dy, 1, 0, 0, MIS_WEAPEXP, 0, pnum, 0, 0);
		}
		if(plr[pnum]._pIFlags & ISPL_LIGHTDAM) {
			AddMissile(dx, dy, 2, 0, 0, MIS_WEAPEXP, 0, pnum, 0, 0);
		}
		didhit = FALSE;
		if(dMonster[dx][dy] != 0) {
			m = dMonster[dx][dy] > 0 ? dMonster[dx][dy] - 1 : -(dMonster[dx][dy] + 1);
			didhit = PlrHitMonst(pnum, m);
		} else if(dPlayer[dx][dy] != 0 && !FriendlyMode) {
			p = dPlayer[dx][dy] > 0 ? dPlayer[dx][dy] - 1 : -(dPlayer[dx][dy] + 1);
			didhit = PlrHitPlr(pnum, p);
		} else if(dObject[dx][dy] > 0) {
			didhit = PlrHitObj(pnum, dx, dy);
		}
		if(didhit && WeaponDur(pnum, 30)) {
			StartStand(pnum, plr[pnum]._pdir);
			ClearPlrPVars(pnum);
			return TRUE;
		}
	}
	if(plr[pnum]._pAnimFrame == plr[pnum]._pAFrames) {
		StartStand(pnum, plr[pnum]._pdir);
		ClearPlrPVars(pnum);
		return TRUE;
	} else {
		return FALSE;
	}
}

int PM_DoRangeAttack(int pnum)
{
	int mistype, frame;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PM_DoRangeAttack: illegal player %d", pnum);
	}

	frame = plr[pnum]._pAnimFrame;
	if(plr[pnum]._pIFlags & ISPL_QUICKATTACK && frame == 1) {
		plr[pnum]._pAnimFrame++;
	}
	if(plr[pnum]._pIFlags & ISPL_FASTATTACK && (frame == 1 || frame == 3)) {
		plr[pnum]._pAnimFrame++;
	}

	if(plr[pnum]._pAnimFrame == plr[pnum]._pAFNum) {
		mistype = MIS_ARROW;
		if(plr[pnum]._pIFlags & ISPL_FIRE_ARROWS) {
			mistype = MIS_FARROW;
		}
		if(plr[pnum]._pIFlags & ISPL_LIGHT_ARROWS) {
			mistype = MIS_LARROW;
		}
		AddMissile(plr[pnum].WorldX, plr[pnum].WorldY, plr[pnum]._pVar1, plr[pnum]._pVar2, plr[pnum]._pdir, mistype, 0, pnum, 4, 0);
		PlaySfxLoc(PS_BFIRE, plr[pnum].WorldX, plr[pnum].WorldY);
		if(WeaponDur(pnum, 40)) {
			StartStand(pnum, plr[pnum]._pdir);
			ClearPlrPVars(pnum);
			return TRUE;
		}
	}
	if(plr[pnum]._pAnimFrame >= plr[pnum]._pAFrames) {
		StartStand(pnum, plr[pnum]._pdir);
		ClearPlrPVars(pnum);
		return TRUE;
	} else {
		return FALSE;
	}
}

void ShieldDur(int pnum)
{
	if(pnum != myplr) {
		return;
	}

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("ShieldDur: illegal player %d", pnum);
	}

	if(plr[pnum].InvBody[4]._itype == ITYPE_SHIELD) {
		if(plr[pnum].InvBody[4]._iDurability == 255) {
			return;
		}
		plr[pnum].InvBody[4]._iDurability--;
		if(plr[pnum].InvBody[4]._iDurability == 0) {
			NetSendCmdDelItem(TRUE, 4);
			plr[pnum].InvBody[4]._itype = -1;
			CalcPlrInv(pnum, TRUE);
		}
	}
	if(plr[pnum].InvBody[5]._itype == ITYPE_SHIELD) {
		if(plr[pnum].InvBody[5]._iDurability == 255) {
			return;
		}
		plr[pnum].InvBody[5]._iDurability--;
		if(plr[pnum].InvBody[5]._iDurability == 0) {
			NetSendCmdDelItem(TRUE, 5);
			plr[pnum].InvBody[5]._itype = -1;
			CalcPlrInv(pnum, TRUE);
		}
	}
}

int PM_DoBlock(int pnum)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PM_DoBlock: illegal player %d", pnum);
	}

	if(plr[pnum]._pIFlags & ISPL_FASTBLOCK && plr[pnum]._pAnimFrame != 1) {
		plr[pnum]._pAnimFrame = plr[pnum]._pBFrames;
	}

	if(plr[pnum]._pAnimFrame >= plr[pnum]._pBFrames) {
		StartStand(pnum, plr[pnum]._pdir);
		ClearPlrPVars(pnum);
		if(random(3, 10) == 0) {
			ShieldDur(pnum);
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

int PM_DoSpell(int pnum)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PM_DoSpell: illegal player %d", pnum);
	}

	if(plr[pnum]._pVar8 == plr[pnum]._pSFNum) {
		CastSpell(
			pnum,
			plr[pnum]._pSpell,
			plr[pnum].WorldX,
			plr[pnum].WorldY,
			plr[pnum]._pVar1,
			plr[pnum]._pVar2,
			0,
			plr[pnum]._pVar4);
		if(plr[pnum]._pSplFrom == 0) {
			if(plr[pnum]._pRSplType == RSPLTYPE_SCROLL && !(plr[pnum]._pScrlSpells64 & (__int64)1 << (plr[pnum]._pRSpell - 1))) {
				plr[pnum]._pRSpell = -1;
				plr[pnum]._pRSplType = RSPLTYPE_INVALID;
				force_redraw = 255;
			}
			if(plr[pnum]._pRSplType == RSPLTYPE_CHARGES && !(plr[pnum]._pISpells64 & (__int64)1 << (plr[pnum]._pRSpell - 1))) {
				plr[pnum]._pRSpell = -1;
				plr[pnum]._pRSplType = RSPLTYPE_INVALID;
				force_redraw = 255;
			}
		}
	}

	plr[pnum]._pVar8++;

	if(leveltype == DTYPE_TOWN) {
		if(plr[pnum]._pVar8 > plr[pnum]._pSFrames) {
			StartWalkStand(pnum);
			ClearPlrPVars(pnum);
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		if(plr[pnum]._pAnimFrame == plr[pnum]._pSFrames) {
			StartStand(pnum, plr[pnum]._pdir);
			ClearPlrPVars(pnum);
			return TRUE;
		} else {
			return FALSE;
		}
	}
}

int PM_DoGotHit(int pnum)
{
	int frame;
	BOOL rv;

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PM_DoGotHit: illegal player %d", pnum);
	}

	frame = plr[pnum]._pAnimFrame;
	if(plr[pnum]._pIFlags & ISPL_FASTRECOVER && frame == 3) {
		plr[pnum]._pAnimFrame++;
	}
	if(plr[pnum]._pIFlags & ISPL_FASTERRECOVER && (frame == 3 || frame == 5)) {
		plr[pnum]._pAnimFrame++;
	}
	if(plr[pnum]._pIFlags & ISPL_FASTESTRECOVER && (frame == 1 || frame == 3 || frame == 5)) {
		plr[pnum]._pAnimFrame++;
	}

	if(plr[pnum]._pAnimFrame >= plr[pnum]._pHFrames) {
		StartStand(pnum, plr[pnum]._pdir);
		ClearPlrPVars(pnum);
		if(random(3, 4) != 0) {
			ArmorDur(pnum);
		}
		rv = TRUE;
	} else {
		rv = FALSE;
	}

	return rv;
}

void ArmorDur(int pnum)
{
	PlayerStruct *p;
	int a;
	ItemStruct *pi;

	if(pnum != myplr) {
		return;
	}

	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("ArmorDur: illegal player %d", pnum);
	}

	p = &plr[pnum];
	if(p->InvBody[6]._itype == -1 && p->InvBody[0]._itype == -1) {
		return;
	}

	a = random(8, 3);
	if(p->InvBody[6]._itype != -1 && p->InvBody[0]._itype == -1) {
		a = 1;
	}
	if(p->InvBody[6]._itype == -1 && p->InvBody[0]._itype != -1) {
		a = 0;
	}

	if(a != 0) {
		pi = &p->InvBody[6];
	} else {
		pi = &p->InvBody[0];
	}

	if(pi->_iDurability != 255) {
		pi->_iDurability--;
		if(pi->_iDurability == 0) {
			if(a != 0) {
				NetSendCmdDelItem(TRUE, 6);
			} else {
				NetSendCmdDelItem(TRUE, 0);
			}
			pi->_itype = -1;
			CalcPlrInv(pnum, TRUE);
		}
	}
}

int PM_DoDeath(int pnum)
{
	if((DWORD)pnum >= MAX_PLRS) {
		app_fatal("PM_DoDeath: illegal player %d", pnum);
	}

	if(plr[pnum]._pVar8 >= 2 * plr[pnum]._pDFrames) {
		if(deathdelay > 1 && pnum == myplr) {
			deathdelay--;
			if(deathdelay == 1) {
				deathflag = 1;
				if(gbMaxPlayers == 1) {
					gamemenu_on();
				}
			}
		}
		plr[pnum]._pAnimDelay = 10000;
		plr[pnum]._pAnimFrame = plr[pnum]._pAnimLen;
		dFlags[plr[pnum].WorldX][plr[pnum].WorldY] |= 4;
	}
	if(plr[pnum]._pVar8 < 100) {
		plr[pnum]._pVar8++;
	}

	return FALSE;
}

int PM_DoNewLvl(int pnum)
{
	return FALSE;
}

void CheckNewPath(int pnum)
{
	int v1; // edi
	int v2; // ebx
	int v3; // eax
	int v4; // ecx
	_bool v5; // zf
	int v6; // eax
	int v7; // esi
	int v8; // eax
	int v9; // edx
	int v10; // esi
	int v11; // esi
	int v12; // eax
	int v13; // eax
	int v14; // ecx
	int v15; // edx
	int v16; // eax
	int v17; // eax
	int v18; // eax
	int v19; // ecx
	int v20; // eax
	int v21; // edi
	int v22; // esi
	int v23; // ST38_4
	int v24; // eax
	int v25; // esi
	int v26; // esi
	int v27; // ST38_4
	int v28; // eax
	int v29; // ecx
	int v30; // edx
	int v31; // ecx
	int *v32; // esi
	int *v33; // edi
	int v34; // esi
	int v35; // eax
	int v36; // ecx
	int v37; // eax
	int v38; // eax
	int v39; // eax
	int v40; // eax
	int v41; // eax
	int *v42; // esi
	int *v43; // edi
	int v44; // eax
	int v45; // eax
	int v46; // esi
	int v47; // esi
	int v48; // eax
	int v49; // ecx
	int v50; // esi
	int v51; // eax
	int v52; // ecx
	int v53; // edi
	int v54; // esi
	int v55; // ST38_4
	int v56; // eax
	int v57; // edi
	int v58; // esi
	int v59; // ST38_4
	int v60; // eax
	int v61; // eax
	int v62; // ecx
	int v63; // esi
	int v64; // ST38_4
	int v65; // eax
	int v66; // esi
	int v67; // edi
	int v68; // eax
	int v69; // esi
	int v70; // esi
	int v71; // eax
	int v72; // ecx
	int v73; // eax
	int v74; // eax
	int *v75; // esi
	int *v76; // edi
	int v77; // eax
	int v78; // eax
	int v79; // eax
	int v80; // eax
	int *v81; // esi
	int *v82; // edi
	int v83; // eax
	int v84; // eax
	int v85; // eax
	int v86; // [esp-18h] [ebp-34h]
	int v87; // [esp-10h] [ebp-2Ch]
	int v88; // [esp-10h] [ebp-2Ch]
	int v89; // [esp-Ch] [ebp-28h]
	int v90; // [esp-Ch] [ebp-28h]
	int v91; // [esp-8h] [ebp-24h]
	int v92; // [esp-8h] [ebp-24h]
	int v93; // [esp-8h] [ebp-24h]
	int v94; // [esp-4h] [ebp-20h]
	int v95; // [esp-4h] [ebp-20h]
	int v96; // [esp-4h] [ebp-20h]
	signed int v97; // [esp+Ch] [ebp-10h]
	int arglist; // [esp+10h] [ebp-Ch]
	int arglista; // [esp+10h] [ebp-Ch]
	int arglistb; // [esp+10h] [ebp-Ch]
	int v101; // [esp+14h] [ebp-8h]
	int v102; // [esp+14h] [ebp-8h]
	int v103; // [esp+14h] [ebp-8h]
	int v104; // [esp+14h] [ebp-8h]
	int p; // [esp+18h] [ebp-4h]

	v1 = pnum;
	p = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("CheckNewPath: illegal player %d", pnum);
	v2 = v1;
	if ( plr[v1].destAction == 20 )
		MakePlrPath(v1, monster[plr[v2].destParam1]._mfutx, monster[plr[v2].destParam1]._mfuty, 0);
	if ( plr[v2].destAction == 21 )
		MakePlrPath(v1, plr[plr[v2].destParam1]._px, plr[plr[v2].destParam1]._py, 0);
	if ( plr[v2].walkpath[0] == -1 )
	{
		v18 = plr[v2].destAction;
		if ( v18 == -1 )
			return;
		v19 = plr[v2]._pmode;
		if ( v19 == PM_STAND )
		{
			switch ( v18 )
			{
				case 9:
					v20 = GetDirection(plr[v2].WorldX, plr[v2].WorldY, plr[v2].destParam1, plr[v2].destParam2);
					goto LABEL_52;
				case 10:
					v30 = plr[v2].WorldY;
					v31 = plr[v2].WorldX;
					v32 = &plr[v2].destParam2;
					v33 = &plr[v2].destParam1;
					goto LABEL_59;
				case 12:
					v39 = GetDirection(plr[v2].WorldX, plr[v2].WorldY, plr[v2].destParam1, plr[v2].destParam2);
					StartSpell(p, v39, plr[v2].destParam1, plr[v2].destParam2);
					v40 = plr[v2].destParam3;
					goto LABEL_66;
				case 13:
					v46 = plr[v2].destParam1;
					arglista = v46;
					v47 = v46;
					v102 = abs(plr[v2].WorldX - object[v47]._ox);
					v48 = abs(plr[v2].WorldY - object[v47]._oy);
					if ( v48 > 1 )
					{
						v49 = object[v47]._oy;
						if ( dObject[object[v47]._ox][v49-1] == -1 - arglista ) /* dungeon[39][112 * object[v47]._ox + 39 + v49] check */
							v48 = abs(plr[v2].WorldY - v49 + 1);
					}
					if ( v102 > 1 || v48 > 1 )
						break;
					if ( object[v47]._oBreak != 1 )
						goto LABEL_73;
					goto LABEL_80;
				case 14:
					v50 = plr[v2].destParam1;
					arglista = v50;
					v47 = v50;
					v103 = abs(plr[v2].WorldX - object[v47]._ox);
					v51 = abs(plr[v2].WorldY - object[v47]._oy);
					if ( v51 > 1 )
					{
						v52 = object[v47]._oy;
						if ( dObject[object[v47]._ox][v52-1] == -1 - arglista ) /* dungeon[39][112 * object[v47]._ox + 39 + v52] check */
							v51 = abs(plr[v2].WorldY - v52 + 1);
					}
					if ( v103 > 1 || v51 > 1 )
						break;
					if ( object[v47]._oBreak == 1 )
					{
LABEL_80:
						v20 = GetDirection(plr[v2].WorldX, plr[v2].WorldY, object[v47]._ox, object[v47]._oy);
LABEL_81:
						v29 = p;
LABEL_82:
						StartAttack(v29, v20);
					}
					else
					{
						TryDisarm(p, arglista);
LABEL_73:
						OperateObject(p, arglista, 0);
					}
					break;
				case 15:
					if ( v1 == myplr )
					{
						v53 = plr[v2].destParam1;
						v54 = plr[v2].destParam1;
						v55 = abs(plr[v2].WorldX - item[v54]._ix);
						v56 = abs(plr[v2].WorldY - item[v54]._iy);
						if ( v55 <= 1 && v56 <= 1 && pcurs == 1 && !item[v54]._iRequest )
						{
							NetSendCmdGItem(1u, CMD_REQUESTGITEM, myplr, myplr, v53);
							item[v54]._iRequest = 1;
						}
					}
					break;
				case 16:
					if ( v1 == myplr )
					{
						v57 = plr[v2].destParam1;
						v58 = plr[v2].destParam1;
						v59 = abs(plr[v2].WorldX - item[v58]._ix);
						v60 = abs(plr[v2].WorldY - item[v58]._iy);
						if ( v59 <= 1 && v60 <= 1 && pcurs == 1 )
							NetSendCmdGItem(1u, CMD_REQUESTAGITEM, myplr, myplr, v57);
					}
					break;
				case 17:
					if ( v1 == myplr )
						TalkToTowner(v1, plr[v2].destParam1);
					break;
				case 18:
					if ( object[plr[v2].destParam1]._oBreak != 1 )
						OperateObject(v1, plr[v2].destParam1, 1u);
					break;
				case 20:
					v21 = plr[v2].destParam1;
					v22 = plr[v2].destParam1;
					v23 = abs(plr[v2].WorldX - monster[v22]._mfutx);
					v24 = abs(plr[v2].WorldY - monster[v22]._mfuty);
					if ( v23 > 1 || v24 > 1 )
						break;
					v20 = GetDirection(plr[v2]._px, plr[v2]._py, monster[v22]._mfutx, monster[v22]._mfuty);
					v25 = monster[v22].mtalkmsg;
					if ( v25 && v25 != TEXT_VILE14 )
						goto LABEL_56;
					goto LABEL_81;
				case 21:
					v26 = plr[v2].destParam1;
					v27 = abs(plr[v2].WorldX - plr[v26]._px);
					v28 = abs(plr[v2].WorldY - plr[v26]._py);
					if ( v27 > 1 || v28 > 1 )
						break;
					v20 = GetDirection(plr[v2]._px, plr[v2]._py, plr[v26]._px, plr[v26]._py);
LABEL_52:
					v29 = v1;
					goto LABEL_82;
				case 22:
					v21 = plr[v2].destParam1;
					v34 = plr[v2].destParam1;
					v35 = GetDirection(plr[v2]._px, plr[v2]._py, monster[v34]._mfutx, monster[v34]._mfuty);
					v36 = monster[v34].mtalkmsg;
					if ( v36 && v36 != TEXT_VILE14 )
LABEL_56:
						TalktoMonster(v21);
					else
						StartRangeAttack(p, v35, monster[v34]._mfutx, monster[v34]._mfuty);
					break;
				case 23:
					v30 = plr[v2]._py;
					v37 = plr[v2].destParam1;
					v31 = plr[v2]._px;
					v32 = &plr[v37]._py;
					v33 = &plr[v37]._px;
LABEL_59:
					v38 = GetDirection(v31, v30, *v33, *v32);
					StartRangeAttack(p, v38, *v33, *v32);
					break;
				case 24:
					v41 = plr[v2].destParam1;
					v42 = &monster[v41]._mfuty;
					v43 = &monster[v41]._mfutx;
					goto LABEL_65;
				case 25:
					v44 = plr[v2].destParam1;
					v42 = &plr[v44]._py;
					v43 = &plr[v44]._px;
LABEL_65:
					v45 = GetDirection(plr[v2].WorldX, plr[v2].WorldY, *v43, *v42);
					StartSpell(p, v45, *v43, *v42);
					v40 = plr[v2].destParam2;
					goto LABEL_66;
				case 26:
					StartSpell(v1, plr[v2].destParam3, plr[v2].destParam1, plr[v2].destParam2);
					plr[v2]._pVar3 = plr[v2].destParam3;
					v40 = plr[v2].destParam4;
LABEL_66:
					plr[v2]._pVar4 = v40;
					break;
				default:
					break;
			}
			FixPlayerLocation(p, plr[v2]._pdir);
			goto LABEL_143;
		}
		if ( v19 == 4 && plr[v2]._pAnimFrame > plr[myplr]._pAFNum )
		{
			switch ( v18 )
			{
				case 9:
					v61 = GetDirection(plr[v2]._px, plr[v2]._py, plr[v2].destParam1, plr[v2].destParam2);
LABEL_105:
					v62 = v1;
LABEL_106:
					StartAttack(v62, v61);
LABEL_107:
					plr[v2].destAction = -1;
					break;
				case 20:
					v63 = plr[v2].destParam1;
					v64 = abs(plr[v2].WorldX - monster[v63]._mfutx);
					v65 = abs(plr[v2].WorldY - monster[v63]._mfuty);
					if ( v64 > 1 || v65 > 1 )
						goto LABEL_107;
					v61 = GetDirection(plr[v2]._px, plr[v2]._py, monster[v63]._mfutx, monster[v63]._mfuty);
					goto LABEL_105;
				case 21:
					v66 = plr[v2].destParam1;
					v67 = abs(plr[v2].WorldX - plr[v66]._px);
					v68 = abs(plr[v2].WorldY - plr[v66]._py);
					if ( v67 > 1 || v68 > 1 )
						goto LABEL_107;
					v61 = GetDirection(plr[v2]._px, plr[v2]._py, plr[v66]._px, plr[v66]._py);
					v62 = p;
					goto LABEL_106;
				case 13:
					v69 = plr[v2].destParam1;
					arglistb = v69;
					v70 = v69;
					v104 = abs(plr[v2].WorldX - object[v70]._ox);
					v71 = abs(plr[v2].WorldY - object[v70]._oy);
					if ( v71 > 1 )
					{
						v72 = object[v70]._oy;
						if ( dObject[object[v70]._ox][v72-1] == -1 - arglistb ) /* dungeon[39][112 * object[v70]._ox + 39 + v72] check */
							v71 = abs(plr[v2].WorldY - v72 + 1);
					}
					if ( v104 <= 1 && v71 <= 1 )
					{
						if ( object[v70]._oBreak == 1 )
						{
							v73 = GetDirection(plr[v2].WorldX, plr[v2].WorldY, object[v70]._ox, object[v70]._oy);
							StartAttack(p, v73);
						}
						else
						{
							OperateObject(p, arglistb, 0);
						}
					}
					break;
			}
		}
		if ( plr[v2]._pmode == PM_RATTACK && plr[v2]._pAnimFrame > plr[myplr]._pAFNum )
		{
			v74 = plr[v2].destAction;
			switch ( v74 )
			{
				case 10:
					v75 = &plr[v2].destParam2;
					v76 = &plr[v2].destParam1;
LABEL_133:
					v79 = GetDirection(plr[v2].WorldX, plr[v2].WorldY, *v76, *v75);
					StartRangeAttack(p, v79, *v76, *v75);
					plr[v2].destAction = -1;
					break;
				case 22:
					v77 = plr[v2].destParam1;
					v75 = &monster[v77]._mfuty;
					v76 = &monster[v77]._mfutx;
					goto LABEL_133;
				case 23:
					v78 = plr[v2].destParam1;
					v75 = &plr[v78]._py;
					v76 = &plr[v78]._px;
					goto LABEL_133;
			}
		}
		if ( plr[v2]._pmode == PM_SPELL && plr[v2]._pAnimFrame > plr[v2]._pSFNum )
		{
			v80 = plr[v2].destAction;
			switch ( v80 )
			{
				case 12:
					v81 = &plr[v2].destParam2;
					v82 = &plr[v2].destParam1;
					break;
				case 24:
					v83 = plr[v2].destParam1;
					v81 = &monster[v83]._mfuty;
					v82 = &monster[v83]._mfutx;
					break;
				case 25:
					v84 = plr[v2].destParam1;
					v81 = &plr[v84]._py;
					v82 = &plr[v84]._px;
					break;
				default:
					return;
			}
			v85 = GetDirection(plr[v2].WorldX, plr[v2].WorldY, *v82, *v81);
			StartSpell(p, v85, *v82, *v81);
			goto LABEL_143;
		}
		return;
	}
	if ( plr[v2]._pmode == PM_STAND )
	{
		if ( v1 == myplr )
		{
			v3 = plr[v2].destAction;
			if ( v3 == 20 || v3 == 21 )
			{
				v4 = plr[v2].destParam1;
				v5 = v3 == 20;
				v6 = plr[v2]._px;
				arglist = plr[v2].destParam1;
				if ( v5 )
				{
					v7 = v4;
					v101 = abs(v6 - monster[v4]._mfutx);
					v8 = abs(plr[v2]._py - monster[v7]._mfuty);
					v9 = plr[v2]._py;
					v94 = monster[v7]._mfuty;
					v91 = monster[v7]._mfutx;
				}
				else
				{
					v10 = v4;
					v101 = abs(v6 - plr[v4]._px);
					v8 = abs(plr[v2]._py - plr[v10]._py);
					v9 = plr[v2]._py;
					v94 = plr[v10]._py;
					v91 = plr[v10]._px;
				}
				v97 = v8;
				v11 = GetDirection(plr[v2]._px, v9, v91, v94);
				if ( v101 < 2 && v97 < 2 )
				{
					ClrPlrPath(p);
					v12 = monster[arglist].mtalkmsg;
					if ( v12 && v12 != TEXT_VILE14 )
						TalktoMonster(arglist);
					else
						StartAttack(p, v11);
					plr[v2].destAction = -1;
				}
			}
		}
		if ( currlevel )
		{
			v13 = SLOBYTE(plr[v2]._pClass);
			v14 = PWVel[v13][0];
			v15 = PWVel[v13][1];
			v16 = PWVel[v13][2];
		}
		else
		{
			v14 = 2048;
			v15 = 1024;
			v16 = 512;
		}
		switch ( plr[v2].walkpath[0] )
		{
			case WALK_NE:
				v95 = 2;
				v92 = DIR_NE;
				v89 = -1;
				v87 = 0;
				v17 = -v16;
				goto LABEL_37;
			case WALK_NW:
				v95 = 8;
				v92 = DIR_NW;
				v89 = 0;
				v87 = -1;
				v17 = -v16;
				v15 = -v15;
LABEL_37:
				StartWalk(p, v15, v17, v87, v89, v92, v95);
				break;
			case WALK_SE:
				v96 = 4;
				v93 = DIR_SE;
				v90 = 0;
				v88 = 1;
				v86 = -32;
				goto LABEL_32;
			case WALK_SW:
				v96 = 6;
				v93 = DIR_SW;
				v90 = 1;
				v88 = 0;
				v86 = 32;
				v15 = -v15;
LABEL_32:
				StartWalk2(p, v15, v16, v86, -16, v88, v90, v93, v96);
				break;
			case WALK_N:
				StartWalk(p, 0, -v15, -1, -1, DIR_N, 1);
				break;
			case WALK_E:
				StartWalk3(p, v14, 0, -32, -16, 1, -1, 1, 0, DIR_E, 3);
				break;
			case WALK_S:
				StartWalk2(p, 0, v15, 0, -32, 1, 1, DIR_S, 5);
				break;
			case WALK_W:
				StartWalk3(p, -v14, 0, 32, -16, -1, 1, 0, 1, DIR_W, 7);
				break;
			default:
				break;
		}
		qmemcpy(plr[v2].walkpath, &plr[v2].walkpath[1], 0x18u);
		plr[v2].walkpath[24] = -1;
		if ( plr[v2]._pmode == PM_STAND )
		{
			StartStand(p, plr[v2]._pdir);
LABEL_143:
			plr[v2].destAction = -1;
			return;
		}
	}
}

BOOL PlrDeathModeOK(int p)
{
	if(p != myplr) {
		return TRUE;
	}

	if((DWORD)p >= MAX_PLRS) {
		app_fatal("PlrDeathModeOK: illegal player %d", p);
	}

	if(plr[p]._pmode == PM_DEATH) {
		return TRUE;
	}
	if(plr[p]._pmode == PM_QUIT) {
		return TRUE;
	}
	if(plr[p]._pmode == PM_NEWLVL) {
		return TRUE;
	}

	return FALSE;
}

void ValidatePlayer()
{
	int v0; // edi
	int v1; // esi
	char *v2; // eax
	int v3; // ecx
	int v4; // ecx
	int *v5; // eax
	int v6; // eax
	int v7; // edx
	int v8; // edx
	int v9; // edx
	int v10; // eax
	int *v11; // ebx
	signed int v12; // edi
	char *v13; // eax
	__int64 v14; // [esp+Ch] [ebp-8h]

	v0 = 0;
	v14 = (__int64)0;
	if ( (unsigned int)myplr >= 4 )
		app_fatal("ValidatePlayer: illegal player %d", myplr);
	v1 = myplr;
	v2 = &plr[myplr]._pLevel;
	if ( *v2 > 50 )
		*v2 = 50;
	v3 = plr[v1]._pNextExper;
	if ( plr[v1]._pExperience > v3 )
		plr[v1]._pExperience = v3;
	v4 = 0;
	if ( plr[v1]._pNumInv > 0 )
	{
		v5 = &plr[v1].InvList[0]._ivalue;
		do
		{
			if ( *(v5 - 47) == 11 )
			{
				if ( *v5 > 5000 )
					*v5 = 5000;
				v4 += *v5;
			}
			++v0;
			v5 += 92;
		}
		while ( v0 < plr[v1]._pNumInv );
	}
	if ( v4 != plr[v1]._pGold )
		plr[v1]._pGold = v4;
	v6 = SLOBYTE(plr[v1]._pClass);
	v7 = MaxStats[v6][0];
	if ( plr[v1]._pBaseStr > v7 )
		plr[v1]._pBaseStr = v7;
	v8 = MaxStats[v6][1];
	if ( plr[v1]._pBaseMag > v8 )
		plr[v1]._pBaseMag = v8;
	v9 = MaxStats[v6][2];
	if ( plr[v1]._pBaseDex > v9 )
		plr[v1]._pBaseDex = v9;
	v10 = MaxStats[v6][3];
	if ( plr[v1]._pBaseVit > v10 )
		plr[v1]._pBaseVit = v10;
	v11 = &spelldata[1].sBookLvl;
	v12 = 1;
	do
	{
		if ( *v11 != -1 )
		{
			v14 |= (__int64)1 << ((unsigned char)v12 - 1);
			v13 = &plr[v1]._pSplLvl[v12];
			if ( *v13 > 15 )
				*v13 = 15;
		}
		v11 += 14;
		++v12;
	}
	while ( (signed int)v11 < (signed int)&spelldata[37].sBookLvl );
	*(_QWORD *)plr[v1]._pMemSpells &= v14;
}

void ProcessPlayers()
{
	int v0; // eax
	int v1; // eax
	unsigned char *v2; // ecx
	char v3; // al
	int v4; // ebp
	int *v5; // esi
	int v6; // eax
	//int v7; // eax
	int v8; // eax
	int v9; // eax
	int v10; // eax
	int v11; // edi
	int v12; // eax
	char *v13; // eax
	char *v14; // eax

	v0 = myplr;
	if ( (unsigned int)myplr >= 4 )
	{
		app_fatal("ProcessPlayers: illegal player %d", myplr);
		v0 = myplr;
	}
	v1 = v0;
	v2 = &plr[v1].pLvlLoad;
	v3 = plr[v1].pLvlLoad;
	if ( v3 )
		*v2 = v3 - 1;
	v4 = 0;
	if ( sfxdelay > 0 && !--sfxdelay )
		PlaySFX(sfxdnum);
	ValidatePlayer();
	v5 = &plr[0]._pHitPoints;
	do
	{
		v6 = (int)(v5 - 89);
		if ( *((_BYTE *)v5 - 379) && currlevel == *(_DWORD *)v6 && (v4 == myplr || !*(_BYTE *)(v6 + 267)) )
		{
			CheckCheatStats(v4);
			//_LOBYTE(v7) = PlrDeathModeOK(v4);
			if ( !PlrDeathModeOK(v4) && (signed int)(*v5 & 0xFFFFFFC0) <= 0 )
				SyncPlrKill(v4, -1);
			if ( v4 == myplr )
			{
				if ( v5[5294] & 0x40 && currlevel )
				{
					*v5 -= 4;
					v8 = *v5;
					*(v5 - 2) -= 4;
					if ( (signed int)(v8 & 0xFFFFFFC0) <= 0 )
						SyncPlrKill(v4, 0);
					drawhpflag = 1;
				}
				if ( *((_BYTE *)v5 + 21179) & 8 )
				{
					v9 = v5[3];
					if ( v9 > 0 )
					{
						v10 = v9 - v5[5];
						v5[5] = 0;
						drawmanaflag = 1;
						v5[3] = v10;
					}
				}
			}
			v11 = 0;
			do
			{
				switch ( *(v5 - 102) )
				{
					case PM_STAND:
						v12 = PM_DoStand(v4);
						goto LABEL_38;
					case PM_WALK:
						v12 = PM_DoWalk(v4);
						goto LABEL_38;
					case PM_WALK2:
						v12 = PM_DoWalk2(v4);
						goto LABEL_38;
					case PM_WALK3:
						v12 = PM_DoWalk3(v4);
						goto LABEL_38;
					case PM_ATTACK:
						v12 = PM_DoAttack(v4);
						goto LABEL_38;
					case PM_RATTACK:
						v12 = PM_DoRangeAttack(v4);
						goto LABEL_38;
					case PM_BLOCK:
						v12 = PM_DoBlock(v4);
						goto LABEL_38;
					case PM_GOTHIT:
						v12 = PM_DoGotHit(v4);
						goto LABEL_38;
					case PM_DEATH:
						v12 = PM_DoDeath(v4);
						goto LABEL_38;
					case PM_SPELL:
						v12 = PM_DoSpell(v4);
						goto LABEL_38;
					case PM_NEWLVL:
						v12 = PM_DoNewLvl(v4);
LABEL_38:
						v11 = v12;
						break;
					default:
						break;
				}
				CheckNewPath(v4);
			}
			while ( v11 );
			v13 = (char *)(v5 - 69);
			++*(_DWORD *)v13;
			if ( *(v5 - 69) > *(v5 - 70) )
			{
				*(_DWORD *)v13 = 0;
				v14 = (char *)(v5 - 67);
				++*(_DWORD *)v14;
				if ( *(v5 - 67) > *(v5 - 68) )
					*(_DWORD *)v14 = 1;
			}
		}
		v5 += 5430;
		++v4;
	}
	while ( (signed int)v5 < (signed int)&plr[MAX_PLRS]._pHitPoints );
}

void CheckCheatStats(int pnum)
{
	int v1; // ecx
	int *v2; // ecx

	v1 = pnum;
	if ( plr[v1]._pStrength > 750 )
		plr[v1]._pStrength = 750;
	if ( plr[v1]._pDexterity > 750 )
		plr[v1]._pDexterity = 750;
	if ( plr[v1]._pMagic > 750 )
		plr[v1]._pMagic = 750;
	if ( plr[v1]._pVitality > 750 )
		plr[v1]._pVitality = 750;
	if ( plr[v1]._pHitPoints > 128000 )
		plr[v1]._pHitPoints = 128000;
	v2 = &plr[v1]._pMana;
	if ( *v2 > 128000 )
		*v2 = 128000;
}

void ClrPlrPath(int pnum)
{
	int v1; // esi

	v1 = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("ClrPlrPath: illegal player %d", pnum);
	memset(plr[v1].walkpath, -1, 0x19u);
}

BOOL PosOkPlayer(int pnum, int px, int py)
{
	char v8; // cl
	unsigned int v9; // ecx
	int v10; // esi
	char v11; // al
	char v12; // cl
	_bool result; // eax

	result = 0;
	if ( px >= 0 && px < 112 && py >= 0 && py < 112 && !SolidLoc(px, py) )
	{
		if ( dPiece[px][py] )
		{
			v8 = dPlayer[px][py];
			if ( !v8 || (v8 <= 0 ? (v9 = -1 - v8) : (v9 = v8 - 1), v9 == pnum || v9 >= 4 || !plr[v9]._pHitPoints) )
			{
				v10 = dMonster[px][py];
				if ( !v10 || currlevel && v10 > 0 && (signed int)(monster[v10-1]._mhitpoints & 0xFFFFFFC0) <= 0 ) /* fix */
				{
					v11 = dObject[px][py];
					if ( !v11 || (v11 <= 0 ? (v12 = -1 - v11) : (v12 = v11 - 1), !object[v12]._oSolidFlag) )
						result = 1;
				}
			}
		}
	}
	return result;
}

void MakePlrPath(int pnum, int xx, int yy, BOOL endspace)
{
	int v4; // esi
	int v5; // ebx
	int v6; // esi
	int v7; // edi
	int v8; // eax
	int v9; // eax
	int v10; // ecx
	int a2; // [esp+Ch] [ebp-4h]

	v4 = pnum;
	v5 = xx;
	a2 = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("MakePlrPath: illegal player %d", pnum);
	v6 = v4;
	v7 = yy;
	v8 = plr[v6]._px;
	plr[v6]._ptargx = v5;
	plr[v6]._ptargy = yy;
	if ( v8 != v5 || plr[v6]._py != yy )
	{
		v9 = FindPath(PosOkPlayer, a2, v8, plr[v6]._py, v5, yy, plr[v6].walkpath);
		if ( v9 )
		{
			if ( !endspace )
			{
				v10 = plr[v6].walkpath[--v9]; /* *((char *)&plr[v6]._pmode + v9-- + 3) */
				switch ( v10 )
				{
					case 1: // N
						goto LABEL_12;
					case 2: // W
						++v5;
						break;
					case 3: // E
						--v5;
						break;
					case 4: // S
						goto LABEL_15;
					case 5: // NW
						++v5;
						goto LABEL_12;
					case 6: // NE
						--v5;
LABEL_12:
						v7 = yy + 1;
						break;
					case 7: // SE
						--v5;
						goto LABEL_15;
					case 8: // SW
						++v5;
LABEL_15:
						v7 = yy - 1;
						break;
					default: // 0/Neutral
						break;
				}
				plr[v6]._ptargx = v5;
				plr[v6]._ptargy = v7;
			}
			plr[v6].walkpath[v9] = -1;
		}
	}
}

void CheckPlrSpell()
{
	int v0; // ecx
	int v1; // eax
	int v2; // edx
	char v3; // al
	int v4; // ecx
	char v5; // al
	int v6; // eax
	int v7; // edx
	int v8; // esi
	int v9; // ST10_4
	int v10; // eax
	int v11; // eax
	int v12; // eax
	int v13; // ST10_4
	int v14; // eax
	char v15; // al

	v0 = myplr;
	if ( (unsigned int)myplr >= 4 )
	{
		app_fatal("CheckPlrSpell: illegal player %d", myplr);
		v0 = myplr;
	}
	v1 = 21720 * v0;
	v2 = plr[v0]._pRSpell;
	if ( v2 != -1 )
	{
		if ( leveltype == DTYPE_TOWN && !*(_DWORD *)&spelldata[v2].sTownSpell )
		{
			v5 = *((_BYTE *)&plr[0]._pClass + v1);
			switch ( v5 )
			{
				case UI_WARRIOR:
					v4 = PS_WARR27;
					goto LABEL_53;
				case UI_ROGUE:
					v4 = PS_ROGUE27;
					goto LABEL_53;
				case UI_SORCERER:
					v4 = PS_MAGE27;
					goto LABEL_53;
			}
			return;
		}
		if ( pcurs != CURSOR_HAND
		  || MouseY >= 352
		  || (chrflag && MouseX < 320 || invflag && MouseX > 320)
		  && v2 != 2
		  && v2 != 5
		  && v2 != 26
		  && v2 != 9
		  && v2 != 27 )
		{
			return;
		}
		_LOBYTE(v1) = *((_BYTE *)&plr[0]._pRSplType + v1);
		if ( (v1 & 0x80u) != 0 )
			goto LABEL_46;
		if ( (char)v1 <= 1 )
		{
			v6 = CheckSpell(v0, v2, v1, 0);
		}
		else
		{
			if ( (_BYTE)v1 != 2 )
			{
				if ( (_BYTE)v1 == 3 )
				{
					v6 = UseStaff();
					goto LABEL_36;
				}
LABEL_46:
				if ( plr[v0]._pRSplType == 1 )
				{
					v15 = plr[v0]._pClass;
					switch ( v15 )
					{
						case UI_WARRIOR:
							v4 = PS_WARR35;
							goto LABEL_53;
						case UI_ROGUE:
							v4 = PS_ROGUE35;
							goto LABEL_53;
						case UI_SORCERER:
							v4 = PS_MAGE35;
							goto LABEL_53;
					}
				}
				return;
			}
			v6 = UseScroll();
		}
LABEL_36:
		v0 = myplr;
		if ( v6 )
		{
			v7 = plr[myplr]._pRSpell;
			if ( v7 == 6 )
			{
				v8 = GetDirection(plr[myplr].WorldX, plr[myplr].WorldY, cursmx, cursmy);
				v9 = GetSpellLevel(myplr, plr[myplr]._pRSpell);
				v10 = 21720 * myplr;
				_LOWORD(v10) = plr[myplr]._pRSpell;
				NetSendCmdLocParam3(1u, CMD_SPELLXYD, cursmx, cursmy, v10, v8, v9);
			}
			else if ( pcursmonst == -1 )
			{
				if ( pcursplr == -1 )
				{
					v13 = GetSpellLevel(myplr, v7);
					v14 = 21720 * myplr;
					_LOWORD(v14) = plr[myplr]._pRSpell;
					NetSendCmdLocParam2(1u, CMD_SPELLXY, cursmx, cursmy, v14, v13);
				}
				else
				{
					v12 = GetSpellLevel(myplr, v7);
					NetSendCmdParam3(1u, CMD_SPELLPID, pcursplr, plr[myplr]._pRSpell, v12);
				}
			}
			else
			{
				v11 = GetSpellLevel(myplr, v7);
				NetSendCmdParam3(1u, CMD_SPELLID, pcursmonst, plr[myplr]._pRSpell, v11);
			}
			return;
		}
		goto LABEL_46;
	}
	v3 = *((_BYTE *)&plr[0]._pClass + v1);
	switch ( v3 )
	{
		case UI_WARRIOR:
			v4 = PS_WARR34;
LABEL_53:
			PlaySFX(v4);
			return;
		case UI_ROGUE:
			v4 = PS_ROGUE34;
			goto LABEL_53;
		case UI_SORCERER:
			v4 = PS_MAGE34;
			goto LABEL_53;
	}
}

void SyncPlrAnim(int pnum)
{
	int v1; // esi
	int v2; // eax
	int v3; // ecx
	unsigned char *v4; // ecx
	int v5; // edx

	v1 = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("SyncPlrAnim: illegal player %d", pnum);
	v2 = v1;
	v3 = plr[v1]._pdir;
	switch ( plr[v1]._pmode )
	{
		case PM_STAND:
		case PM_NEWLVL:
		case PM_QUIT:
			v4 = plr[0]._pNAnim[v3 + 5430 * v1];
			goto LABEL_19;
		case PM_WALK:
		case PM_WALK2:
		case PM_WALK3:
			v4 = plr[0]._pWAnim[v3 + 5430 * v1];
			goto LABEL_19;
		case PM_ATTACK:
		case PM_RATTACK:
			v4 = plr[0]._pAAnim[v3 + 5430 * v1];
			goto LABEL_19;
		case PM_BLOCK:
			v4 = plr[0]._pBAnim[v3 + 5430 * v1];
			goto LABEL_19;
		case PM_GOTHIT:
			v4 = plr[0]._pHAnim[v3 + 5430 * v1];
			goto LABEL_19;
		case PM_DEATH:
			v4 = plr[0]._pDAnim[v3 + 5430 * v1];
			goto LABEL_19;
		case PM_SPELL:
			if ( v1 == myplr )
				v5 = (unsigned char)spelldata[plr[v2]._pSpell].sType;
			else
				v5 = 0;
			if ( !v5 )
				plr[v2]._pAnimData = plr[0]._pFAnim[v3 + 5430 * v1];
			if ( v5 == STYPE_LIGHTNING )
				plr[v2]._pAnimData = plr[0]._pLAnim[v3 + 5430 * v1];
			if ( v5 == STYPE_MAGIC )
			{
				v4 = plr[0]._pTAnim[v3 + 5430 * v1];
LABEL_19:
				plr[v2]._pAnimData = v4;
			}
			break;
		default:
			app_fatal("SyncPlrAnim");
			break;
	}
}

void SyncInitPlrPos(int pnum)
{
	int v1; // esi
	_bool v2; // zf
	unsigned int v3; // eax
	int v4; // ebx
	int v5; // edi
	int v6; // eax
	signed int v7; // [esp+Ch] [ebp-18h]
	int p; // [esp+10h] [ebp-14h]
	int v9; // [esp+14h] [ebp-10h]
	signed int v10; // [esp+18h] [ebp-Ch]
	signed int v11; // [esp+1Ch] [ebp-8h]
	unsigned int i; // [esp+20h] [ebp-4h]
	signed int v13; // [esp+20h] [ebp-4h]

	p = pnum;
	v1 = pnum;
	v2 = gbMaxPlayers == 1;
	plr[v1]._ptargx = plr[pnum].WorldX;
	plr[v1]._ptargy = plr[pnum].WorldY;
	if ( !v2 && plr[v1].plrlevel == currlevel )
	{
		v3 = 0;
		for ( i = 0; ; v3 = i )
		{
			v4 = plr[v1].WorldX + *(int *)((char *)plrxoff2 + v3);
			v5 = plr[v1].WorldY + *(int *)((char *)plryoff2 + v3);
			if ( PosOkPlayer(p, v4, v5) )
				break;
			i += 4;
			if ( i >= 0x20 )
				break;
		}
		if ( !PosOkPlayer(p, v4, v5) )
		{
			v11 = 0;
			v6 = -1;
			v13 = 1;
			v7 = -1;
			do
			{
				if ( v11 )
					break;
				v9 = v6;
				while ( v6 <= v13 && !v11 )
				{
					v5 = v9 + plr[v1].WorldY;
					v10 = v7;
					do
					{
						if ( v11 )
							break;
						v4 = v10 + plr[v1].WorldX;
						if ( PosOkPlayer(p, v10 + plr[v1].WorldX, v5) && !PosOkPortal(currlevel, v4, v5) )
							v11 = 1;
						++v10;
					}
					while ( v10 <= v13 );
					v6 = ++v9;
				}
				++v13;
				v6 = v7-- - 1;
			}
			while ( v7 > -50 );
		}
		plr[v1].WorldX = v4;
		v2 = p == myplr;
		plr[v1].WorldY = v5;
		dPlayer[v4][v5] = p + 1;
		if ( v2 )
		{
			plr[v1]._px = v4;
			plr[v1]._py = v5;
			plr[v1]._ptargx = v4;
			plr[v1]._ptargy = v5;
			ViewX = v4;
			ViewY = v5;
		}
	}
}

void SyncInitPlr(int pnum)
{
	int v1; // esi

	v1 = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("SyncInitPlr: illegal player %d", pnum);
	SetPlrAnims(v1);
	SyncInitPlrPos(v1);
}

void CheckStats(int pnum)
{
	int v1; // esi
	int v2; // eax
	char v3; // cl
	signed int v4; // esi
	signed int v5; // edi
	int v6; // edx
	int v7; // ecx
	int v8; // edx
	int v9; // ecx
	int v10; // edx
	int v11; // ecx
	int v12; // edx
	int v13; // ecx
	//signed int v14; // [esp+Ch] [ebp-4h]

	v1 = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("CheckStats: illegal player %d", pnum);
	v2 = v1;
	v3 = plr[v1]._pClass;
	if ( v3 )
	{
		if ( v3 == 1 )
		{
			v4 = 1;
		}
		else if ( v3 == 2 )
		{
			v4 = 2;
		}
		/*else
		{
			v4 = v14;
		}*/
	}
	else
	{
		v4 = 0;
	}
	v5 = 0;
	do
	{
		if ( v5 )
		{
			switch ( v5 )
			{
				case ATTRIB_MAG:
					v10 = plr[v2]._pBaseMag;
					v11 = MaxStats[v4][1];
					if ( v10 <= v11 )
					{
						if ( v10 < 0 )
							plr[v2]._pBaseMag = 0;
					}
					else
					{
						plr[v2]._pBaseMag = v11;
					}
					break;
				case ATTRIB_DEX:
					v8 = plr[v2]._pBaseDex;
					v9 = MaxStats[v4][2];
					if ( v8 <= v9 )
					{
						if ( v8 < 0 )
							plr[v2]._pBaseDex = 0;
					}
					else
					{
						plr[v2]._pBaseDex = v9;
					}
					break;
				case ATTRIB_VIT:
					v6 = plr[v2]._pBaseVit;
					v7 = MaxStats[v4][3];
					if ( v6 <= v7 )
					{
						if ( v6 < 0 )
							plr[v2]._pBaseVit = 0;
					}
					else
					{
						plr[v2]._pBaseVit = v7;
					}
					break;
			}
		}
		else
		{
			v12 = plr[v2]._pBaseStr;
			v13 = MaxStats[v4][0];
			if ( v12 <= v13 )
			{
				if ( v12 < 0 )
					plr[v2]._pBaseStr = 0;
			}
			else
			{
				plr[v2]._pBaseStr = v13;
			}
		}
		++v5;
	}
	while ( v5 < 4 );
}

void ModifyPlrStr(int pnum, int l)
{
	int v2; // esi
	int v3; // edi
	int v4; // esi
	char v5; // dl
	int v6; // ecx
	int v7; // eax
	int v8; // ebx
	int v9; // eax
	signed int v10; // ecx
	int p; // [esp+8h] [ebp-4h]

	v2 = pnum;
	v3 = l;
	p = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("ModifyPlrStr: illegal player %d", pnum);
	v4 = v2;
	v5 = plr[v4]._pClass;
	v6 = plr[v4]._pBaseStr;
	v7 = MaxStats[v5][0];
	if ( v6 + v3 > v7 )
		v3 = v7 - v6;
	plr[v4]._pBaseStr = v3 + v6;
	plr[v4]._pStrength += v3;
	v8 = plr[v4]._pStrength;
	if ( v5 == 1 )
	{
		v9 = plr[v4]._pLevel * (v8 + plr[v4]._pDexterity);
		v10 = 200;
	}
	else
	{
		v9 = v8 * plr[v4]._pLevel;
		v10 = 100;
	}
	plr[v4]._pDamageMod = v9 / v10;
	CalcPlrInv(p, 1u);
	if ( p == myplr )
		NetSendCmdParam1(0, CMD_SETSTR, plr[v4]._pBaseStr);
}

void ModifyPlrMag(int pnum, int l)
{
	int v2; // esi
	int v3; // edi
	int v4; // esi
	char v5; // dl
	int v6; // ecx
	int v7; // eax
	int v8; // eax
	int v9; // edi
	int p; // [esp+8h] [ebp-4h]

	v2 = pnum;
	v3 = l;
	p = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("ModifyPlrMag: illegal player %d", pnum);
	v4 = v2;
	v5 = plr[v4]._pClass;
	v6 = MaxStats[v5][1];
	v7 = plr[v4]._pBaseMag;
	if ( v7 + v3 > v6 )
		v3 = v6 - v7;
	plr[v4]._pMagic += v3;
	v8 = v3 + v7;
	v9 = v3 << 6;
	plr[v4]._pBaseMag = v8;
	if ( v5 == 2 )
		v9 *= 2;
	plr[v4]._pMaxManaBase += v9;
	plr[v4]._pMaxMana += v9;
	if ( !(plr[v4]._pIFlags & 0x8000000) )
	{
		plr[v4]._pManaBase += v9;
		plr[v4]._pMana += v9;
	}
	CalcPlrInv(p, 1u);
	if ( p == myplr )
		NetSendCmdParam1(0, CMD_SETMAG, plr[v4]._pBaseMag);
}

void ModifyPlrDex(int pnum, int l)
{
	int v2; // ebx
	int v3; // edi
	int v4; // esi
	int v5; // ecx
	int v6; // eax

	v2 = pnum;
	v3 = l;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("ModifyPlrDex: illegal player %d", pnum);
	v4 = v2;
	v5 = MaxStats[SLOBYTE(plr[v2]._pClass)][2];
	v6 = plr[v2]._pBaseDex;
	if ( v6 + v3 > v5 )
		v3 = v5 - v6;
	plr[v4]._pDexterity += v3;
	plr[v4]._pBaseDex = v3 + v6;
	CalcPlrInv(v2, 1u);
	if ( _LOBYTE(plr[v4]._pClass) == 1 )
		plr[v4]._pDamageMod = plr[v4]._pLevel * (plr[v4]._pDexterity + plr[v4]._pStrength) / 200;
	if ( v2 == myplr )
		NetSendCmdParam1(0, CMD_SETDEX, plr[v4]._pBaseDex);
}

void ModifyPlrVit(int pnum, int l)
{
	int v2; // esi
	int v3; // edi
	int v4; // esi
	char v5; // dl
	int v6; // ecx
	int v7; // eax
	int v8; // eax
	int v9; // edi
	int p; // [esp+8h] [ebp-4h]

	v2 = pnum;
	v3 = l;
	p = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("ModifyPlrVit: illegal player %d", pnum);
	v4 = v2;
	v5 = plr[v4]._pClass;
	v6 = MaxStats[v5][3];
	v7 = plr[v4]._pBaseVit;
	if ( v7 + v3 > v6 )
		v3 = v6 - v7;
	plr[v4]._pVitality += v3;
	v8 = v3 + v7;
	v9 = v3 << 6;
	plr[v4]._pBaseVit = v8;
	if ( !v5 )
		v9 *= 2;
	plr[v4]._pHPBase += v9;
	plr[v4]._pMaxHPBase += v9;
	plr[v4]._pHitPoints += v9;
	plr[v4]._pMaxHP += v9;
	CalcPlrInv(p, 1u);
	if ( p == myplr )
		NetSendCmdParam1(0, CMD_SETVIT, plr[v4]._pBaseVit);
}

void SetPlayerHitPoints(int pnum, int newhp)
{
	int v2; // esi
	int v3; // edi
	int v4; // eax
	int v5; // ecx
	_bool v6; // zf

	v2 = pnum;
	v3 = newhp;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("SetPlayerHitPoints: illegal player %d", pnum);
	v4 = v2;
	v5 = plr[v2]._pMaxHPBase;
	plr[v4]._pHitPoints = v3;
	v6 = v2 == myplr;
	plr[v4]._pHPBase = v3 + v5 - plr[v2]._pMaxHP;
	if ( v6 )
		drawhpflag = 1;
}

void SetPlrStr(int pnum, int v)
{
	int v2; // edi
	int v3; // ebx
	int v4; // esi
	int v5; // eax
	signed int v6; // ecx

	v2 = pnum;
	v3 = v;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("SetPlrStr: illegal player %d", pnum);
	v4 = v2;
	plr[v2]._pBaseStr = v3;
	CalcPlrInv(v2, 1u);
	if ( _LOBYTE(plr[v2]._pClass) == 1 )
	{
		v5 = plr[v4]._pLevel * (plr[v4]._pStrength + plr[v4]._pDexterity);
		v6 = 200;
	}
	else
	{
		v5 = plr[v4]._pStrength * plr[v4]._pLevel;
		v6 = 100;
	}
	plr[v4]._pDamageMod = v5 / v6;
}

void SetPlrMag(int pnum, int v)
{
	int v2; // edi
	int v3; // esi
	int v4; // eax
	int v5; // esi

	v2 = pnum;
	v3 = v;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("SetPlrMag: illegal player %d", pnum);
	v4 = v2;
	plr[v2]._pBaseMag = v3;
	v5 = v3 << 6;
	if ( _LOBYTE(plr[v2]._pClass) == 2 )
		v5 *= 2;
	plr[v4]._pMaxManaBase = v5;
	plr[v4]._pMaxMana = v5;
	CalcPlrInv(v2, 1u);
}

void SetPlrDex(int pnum, int v)
{
	int v2; // edi
	int v3; // ebx
	int v4; // esi
	int v5; // eax
	signed int v6; // ecx

	v2 = pnum;
	v3 = v;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("SetPlrDex: illegal player %d", pnum);
	v4 = v2;
	plr[v2]._pBaseDex = v3;
	CalcPlrInv(v2, 1u);
	if ( _LOBYTE(plr[v2]._pClass) == 1 )
	{
		v5 = plr[v4]._pLevel * (plr[v4]._pStrength + plr[v4]._pDexterity);
		v6 = 200;
	}
	else
	{
		v5 = plr[v4]._pStrength * plr[v4]._pLevel;
		v6 = 100;
	}
	plr[v4]._pDamageMod = v5 / v6;
}

void SetPlrVit(int pnum, int v)
{
	int v2; // edi
	int v3; // esi
	int v4; // eax
	int v5; // esi

	v2 = pnum;
	v3 = v;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("SetPlrVit: illegal player %d", pnum);
	v4 = v2;
	plr[v2]._pBaseVit = v3;
	v5 = v3 << 6;
	if ( !_LOBYTE(plr[v2]._pClass) )
		v5 *= 2;
	plr[v4]._pHPBase = v5;
	plr[v4]._pMaxHPBase = v5;
	CalcPlrInv(v2, 1u);
}

void InitDungMsgs(int pnum)
{
	int v1; // esi

	v1 = pnum;
	if ( (unsigned int)pnum >= MAX_PLRS )
		app_fatal("InitDungMsgs: illegal player %d", pnum);
	plr[v1].pDungMsgs = 0;
}

void PlayDungMsgs()
{
	int v0; // eax
	int v1; // eax
	char v2; // cl
	char v3; // dl
	char v4; // cl
	char v5; // cl
	char v6; // dl
	char v7; // cl
	char v8; // dl
	char v9; // cl
	char v10; // dl
	char v11; // cl
	char v12; // dl

	v0 = myplr;
	if ( (unsigned int)myplr >= 4 )
	{
		app_fatal("PlayDungMsgs: illegal player %d", myplr);
		v0 = myplr;
	}
	switch ( currlevel )
	{
		case 1u:
			v1 = v0;
			if ( !plr[v1]._pLvlVisited[1] && gbMaxPlayers == currlevel )
			{
				v2 = plr[v1].pDungMsgs;
				if ( !(v2 & 1) )
				{
					v3 = plr[v1]._pClass;
					sfxdelay = 40;
					if ( v3 )
					{
						if ( v3 == 1 )
						{
							sfxdnum = PS_ROGUE97;
						}
						else if ( v3 == 2 )
						{
							sfxdnum = PS_MAGE97;
						}
					}
					else
					{
						sfxdnum = PS_WARR97;
					}
					v4 = v2 | 1;
LABEL_14:
					plr[v1].pDungMsgs = v4;
					return;
				}
			}
			break;
		case 5u:
			v1 = v0;
			if ( !plr[v1]._pLvlVisited[5] && gbMaxPlayers == 1 )
			{
				v5 = plr[v1].pDungMsgs;
				if ( !(v5 & 2) )
				{
					v6 = plr[v1]._pClass;
					sfxdelay = 40;
					if ( v6 )
					{
						if ( v6 == 1 )
						{
							sfxdnum = PS_ROGUE96;
						}
						else if ( v6 == 2 )
						{
							sfxdnum = PS_MAGE96;
						}
					}
					else
					{
						sfxdnum = PS_WARR96B;
					}
					v4 = v5 | 2;
					goto LABEL_14;
				}
			}
			break;
		case 9u:
			v1 = v0;
			if ( !plr[v1]._pLvlVisited[9] && gbMaxPlayers == 1 )
			{
				v7 = plr[v1].pDungMsgs;
				if ( !(v7 & 4) )
				{
					v8 = plr[v1]._pClass;
					sfxdelay = 40;
					if ( v8 )
					{
						if ( v8 == 1 )
						{
							sfxdnum = PS_ROGUE98;
						}
						else if ( v8 == 2 )
						{
							sfxdnum = PS_MAGE98;
						}
					}
					else
					{
						sfxdnum = PS_WARR98;
					}
					v4 = v7 | 4;
					goto LABEL_14;
				}
			}
			break;
		case 13u:
			v1 = v0;
			if ( !plr[v1]._pLvlVisited[13] && gbMaxPlayers == 1 )
			{
				v9 = plr[v1].pDungMsgs;
				if ( !(v9 & 8) )
				{
					v10 = plr[v1]._pClass;
					sfxdelay = 40;
					if ( v10 )
					{
						if ( v10 == 1 )
						{
							sfxdnum = PS_ROGUE99;
						}
						else if ( v10 == 2 )
						{
							sfxdnum = PS_MAGE99;
						}
					}
					else
					{
						sfxdnum = PS_WARR99;
					}
					v4 = v9 | 8;
					goto LABEL_14;
				}
			}
			break;
		case 16u:
			v1 = v0;
			if ( !plr[v1]._pLvlVisited[15] && gbMaxPlayers == 1 )
			{
				v11 = plr[v1].pDungMsgs;
				if ( !(v11 & 0x10) )
				{
					v12 = plr[v1]._pClass;
					sfxdelay = 40;
					if ( !v12 || v12 == 1 || v12 == 2 )
						sfxdnum = PS_DIABLVLINT;
					v4 = v11 | 0x10;
					goto LABEL_14;
				}
			}
			break;
	}
	sfxdelay = 0;
}
