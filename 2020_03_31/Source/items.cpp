#include "diablo.h"

int itemactive[MAXITEMS];
int uitemflag;
int itemavail[MAXITEMS];
ItemStruct curruitem;
ItemGetRecordStruct itemrecord[MAXITEMS];
ItemStruct item[MAXITEMS+1];
BOOL itemhold[3][3];
unsigned char *itemanims[35];
int UniqueItemFlag[128];
int numitems;
int gnNumGetRecords;

/* data */

unsigned char ItemCAnimTbl[169] =
{
   20,  16,  16,  16,   4,   4,   4,  12,  12,  12,
   12,  12,  12,  12,  12,  21,  21,  25,  12,  28,
   28,  28,   0,   0,   0,  32,   0,   0,   0,  24,
   24,  26,   2,  25,  22,  23,  24,  25,  27,  27,
   29,   0,   0,   0,  12,  12,  12,  12,  12,   0,
	8,   8,   0,   8,   8,   8,   8,   8,   8,   6,
	8,   8,   8,   6,   8,   8,   6,   8,   8,   6,
	6,   6,   8,   8,   8,   5,   9,  13,  13,  13,
	5,   5,   5,  15,   5,   5,  18,  18,  18,  30,
	5,   5,  14,   5,  14,  13,  16,  18,   5,   5,
	7,   1,   3,  17,   1,  15,  10,  14,   3,  11,
	8,   0,   1,   7,   0,   7,  15,   7,   3,   3,
	3,   6,   6,  11,  11,  11,  31,  14,  14,  14,
	6,   6,   7,   3,   8,  14,   0,  14,  14,   0,
   33,   1,   1,   1,   1,   1,   7,   7,   7,  14,
   14,  17,  17,  17,   0,  34,   1,   0,   3,  17,
	8,   8,   6,   1,   3,   3,  11,   3,   4
};
char *ItemDropStrs[35] =
{
  "Armor2",
  "Axe",
  "FBttle",
  "Bow",
  "GoldFlip",
  "Helmut",
  "Mace",
  "Shield",
  "SwrdFlip",
  "Rock",
  "Cleaver",
  "Staff",
  "Ring",
  "CrownF",
  "LArmor",
  "WShield",
  "Scroll",
  "FPlateAr",
  "FBook",
  "Food",
  "FBttleBB",
  "FBttleDY",
  "FBttleOR",
  "FBttleBR",
  "FBttleBL",
  "FBttleBY",
  "FBttleWH",
  "FBttleDB",
  "FEar",
  "FBrain",
  "FMush",
  "Innsign",
  "Bldstn",
  "Fanvil",
  "FLazStaf"
};
unsigned char ItemAnimLs[35] =
{
  15u,
  13u,
  16u,
  13u,
  10u,
  13u,
  13u,
  13u,
  13u,
  10u,
  13u,
  13u,
  13u,
  13u,
  13u,
  13u,
  13u,
  13u,
  13u,
  1u,
  16u,
  16u,
  16u,
  16u,
  16u,
  16u,
  16u,
  16u,
  13u,
  12u,
  12u,
  13u,
  13u,
  13u,
  8u
};
int ItemDropSnds[35] =
{
  IS_FHARM,
  IS_FAXE,
  IS_FPOT,
  IS_FBOW,
  IS_GOLD,
  IS_FCAP,
  IS_FSWOR,
  IS_FSHLD,
  IS_FSWOR,
  IS_FROCK,
  IS_FAXE,
  IS_FSTAF,
  IS_FRING,
  IS_FCAP,
  IS_FLARM,
  IS_FSHLD,
  IS_FSCRL,
  IS_FHARM,
  IS_FBOOK,
  IS_FLARM,
  IS_FPOT,
  IS_FPOT,
  IS_FPOT,
  IS_FPOT,
  IS_FPOT,
  IS_FPOT,
  IS_FPOT,
  IS_FPOT,
  IS_FBODY,
  IS_FBODY,
  IS_FMUSH,
  IS_ISIGN,
  IS_FBLST,
  IS_FANVL,
  IS_FSTAF
};
int ItemInvSnds[35] =
{
  IS_IHARM,
  IS_IAXE,
  IS_IPOT,
  IS_IBOW,
  IS_GOLD,
  IS_ICAP,
  IS_ISWORD,
  IS_ISHIEL,
  IS_ISWORD,
  IS_IROCK,
  IS_IAXE,
  IS_ISTAF,
  IS_IRING,
  IS_ICAP,
  IS_ILARM,
  IS_ISHIEL,
  IS_ISCROL,
  IS_IHARM,
  IS_IBOOK,
  IS_IHARM,
  IS_IPOT,
  IS_IPOT,
  IS_IPOT,
  IS_IPOT,
  IS_IPOT,
  IS_IPOT,
  IS_IPOT,
  IS_IPOT,
  IS_IBODY,
  IS_IBODY,
  IS_IMUSH,
  IS_ISIGN,
  IS_IBLST,
  IS_IANVL,
  IS_ISTAF
};
int idoppely = 16;
int premiumlvladd[6] = { -1, -1, 0, 0, 1, 2 };

void InitItemGFX()
{
	int i;
	char filestr[64];

	for(i = 0; i < 35; i++) {
		sprintf(filestr, "Items\\%s.CEL", ItemDropStrs[i]);
		/// ASSERT: assert(! itemanims[i]);
		itemanims[i] = DiabLoad(filestr, NULL, 'IGFX');
	}

	memset(UniqueItemFlag, 0, sizeof(UniqueItemFlag));
}

BOOL ItemPlace(int xp, int yp)
{
	if(dMonster[xp][yp] != 0) {
		return FALSE;
	}
	if(dPlayer[xp][yp] != 0) {
		return FALSE;
	}
	if(dItem[xp][yp] != 0) {
		return FALSE;
	}
	if(dObject[xp][yp] != 0) {
		return FALSE;
	}
	if(dFlags[xp][yp] & 8) {
		return FALSE;
	}
	if(nSolidTable[dPiece[xp][yp]]) {
		return FALSE;
	}

	return TRUE;
}

void AddInitItems()
{
	int i, j, ii, xx, yy;

	j = random(11, 3) + 3;
	for(i = 0; i < j; i++) {
		ii = itemavail[0];
		itemavail[0] = itemavail[MAXITEMS - numitems - 1];
		itemactive[numitems] = ii;
		xx = random(12, 80) + 16;
		yy = random(12, 80) + 16;
		while(!ItemPlace(xx, yy)) {
			xx = random(12, 80) + 16;
			yy = random(12, 80) + 16;
		}
		item[ii]._ix = xx;
		item[ii]._iy = yy;
		dItem[xx][yy] = ii + 1;
		item[ii]._iSeed = GetRndSeed();
		SetRndSeed(item[ii]._iSeed);
		if(random(12, 2) != 0) {
			GetItemAttrs(ii, IDI_HEAL, currlevel);
		} else {
			GetItemAttrs(ii, IDI_MANA, currlevel);
		}
		item[ii]._iCreateInfo = currlevel + 0x8000;
		SetupItem(ii);
		item[ii]._iAnimFrame = item[ii]._iAnimLen;
		item[ii]._iAnimFlag = 0;
		item[ii]._iSelFlag = 1;
		DeltaAddItem(ii);
		numitems++;
	}
}

void InitItems()
{
	int i, s;

	GetItemAttrs(0, IDI_GOLD, 1);
	golditem = item[0];
	golditem._iStatFlag = 1;
	numitems = 0;

	for(i = 0; i < MAXITEMS; i++) {
		item[i]._itype = 0;
		item[i]._ix = 0;
		item[i]._iy = 0;
		item[i]._iAnimFlag = 0;
		item[i]._iSelFlag = 0;
		item[i]._iIdentified = 0;
		item[i]._iPostDraw = 0;
	}

	for(i = 0; i < MAXITEMS; i++) {
		itemavail[i] = i;
		itemactive[i] = 0;
	}

	if(!setlevel) {
		s = GetRndSeed(); /* unused */
		if(QuestStatus(Q_ROCK)) {
			SpawnRock();
		}
		if(QuestStatus(Q_ANVIL)) {
			SpawnQuestItem(IDI_ANVIL, 2 * setpc_x + 27, 2 * setpc_y + 27, 0, 1);
		}
		if(currlevel > 0 && currlevel < 16) {
			AddInitItems();
		}
	}

	uitemflag = 0;
}

void CalcPlrItemVals(int p, BOOL Loadgfx)
{
	int i, g, d, mi, tmpac;
	__int64 t; /* todo: check usage */
	ItemStruct *itm;

	/// ASSERT: assert((DWORD) p < MAX_PLRS);

	int mind = 0;
	int maxd = 0;
	int tac = 0;
	int bdam = 0;
	int btohit = 0;
	int bac = 0;
	long iflgs = 0;
	int sadd = 0;
	int madd = 0;
	int dadd = 0;
	int vadd = 0;
	__int64 spl = 0;
	int fr = 0;
	int lr = 0;
	int mr = 0;
	int dmod = 0;
	int ghit = 0;
	int lrad = 10;
	int ihp = 0;
	int imana = 0;
	int spllvladd = 0;
	int enac = 0;
	int fmin = 0;
	int fmax = 0;
	int lmin = 0;
	int lmax = 0;

	for(i = 0; i < NUM_INVLOC; i++) {
		itm = &plr[p].InvBody[i];
		if(itm->_itype == -1 || !itm->_iStatFlag) {
			continue;
		}
		mind += itm->_iMinDam;
		maxd += itm->_iMaxDam;
		tac += itm->_iAC;
		if(itm->_iSpell != SPL_NULL) {
			t = 1;
			spl |= t << (itm->_iSpell - 1);
		}
		if(itm->_iMagical == 0 || itm->_iIdentified) {
			bdam += itm->_iPLDam;
			btohit += itm->_iPLToHit;
			if(itm->_iPLAC != 0) {
				tmpac = itm->_iPLAC * itm->_iAC / 100;
				if(tmpac == 0) {
					tmpac = 1;
				}
				bac += tmpac;
			}
			iflgs |= itm->_iFlags;
			sadd += itm->_iPLStr;
			madd += itm->_iPLMag;
			dadd += itm->_iPLDex;
			vadd += itm->_iPLVit;
			fr += itm->_iPLFR;
			lr += itm->_iPLLR;
			mr += itm->_iPLMR;
			dmod += itm->_iPLDamMod;
			ghit += itm->_iPLGetHit;
			lrad += itm->_iPLLight;
			ihp += itm->_iPLHP;
			imana += itm->_iPLMana;
			spllvladd += itm->_iSplLvlAdd;
			enac += itm->_iPLEnAc;
			fmin += itm->_iFMinDam;
			fmax += itm->_iFMaxDam;
			lmin += itm->_iLMinDam;
			lmax += itm->_iLMaxDam;
		}
	}

	if(mind == 0 && maxd == 0) {
		mind = 1;
		maxd = 1;
		if(plr[p].InvBody[4]._itype == ITYPE_SHIELD && plr[p].InvBody[4]._iStatFlag) {
			maxd = 3;
		}
		if(plr[p].InvBody[5]._itype == ITYPE_SHIELD && plr[p].InvBody[5]._iStatFlag) {
			maxd = 3;
		}
	}
	plr[p]._pIMinDam = mind;
	plr[p]._pIMaxDam = maxd;
	plr[p]._pIAC = tac;
	plr[p]._pIBonusDam = bdam;
	plr[p]._pIBonusToHit = btohit;
	plr[p]._pIBonusAC = bac;
	plr[p]._pIFlags = iflgs;
	plr[p]._pIBonusDamMod = dmod;
	plr[p]._pIGetHit = ghit;

	if(lrad < 2) {
		lrad = 2;
	}
	if(lrad > 15) {
		lrad = 15;
	}
	if(plr[p]._pLightRad != lrad && p == myplr) {
		ChangeLightRadius(plr[p]._plid, lrad);
		if(lrad >= 10) {
			ChangeVisionRadius(plr[p]._pvid, lrad);
		} else {
			ChangeVisionRadius(plr[p]._pvid, 10);
		}
		plr[p]._pLightRad = lrad;
	}

	plr[p]._pStrength = sadd + plr[p]._pBaseStr;
	if(plr[myplr]._pStrength <= 0) {
		plr[myplr]._pStrength = 0;
	}
	plr[p]._pMagic = madd + plr[p]._pBaseMag;
	if(plr[myplr]._pMagic <= 0) {
		plr[myplr]._pMagic = 0;
	}
	plr[p]._pDexterity = dadd + plr[p]._pBaseDex;
	if(plr[myplr]._pDexterity <= 0) {
		plr[myplr]._pDexterity = 0;
	}
	plr[p]._pVitality = vadd + plr[p]._pBaseVit;
	if(plr[myplr]._pVitality <= 0) {
		plr[myplr]._pVitality = 0;
	}

	if(plr[p]._pClass == PC_ROGUE) {
		plr[p]._pDamageMod = plr[p]._pLevel * (plr[p]._pStrength + plr[p]._pDexterity) / 200;
	} else {
		plr[p]._pDamageMod = plr[p]._pLevel * plr[p]._pStrength / 100;
	}

	plr[p]._pISpells64 = spl;
	t = 1;
	if(plr[p]._pRSplType == RSPLTYPE_CHARGES && !(spl & t << (plr[p]._pRSpell - 1))) {
		plr[p]._pRSpell = -1;
		plr[p]._pRSplType = RSPLTYPE_INVALID;
		force_redraw = 255;
	}
	plr[p]._pISplLvlAdd = spllvladd;
	plr[p]._pIEnAc = enac;

	if(iflgs & ISPL_ALLRESZERO) {
		mr = 0;
		fr = 0;
		lr = 0;
	}
	if(mr > 75) {
		mr = 75;
	}
	plr[p]._pMagResist = mr;
	if(fr > 75) {
		fr = 75;
	}
	plr[p]._pFireResist = fr;
	if(lr > 75) {
		lr = 75;
	}
	plr[p]._pLghtResist = lr;

	if(plr[p]._pClass == PC_WARRIOR) {
		vadd *= 2;
	}
	if(plr[p]._pClass == PC_ROGUE) {
		vadd += vadd >> 1;
	}
	ihp += vadd << 6;

	if(plr[p]._pClass == PC_SORCERER) {
		madd *= 2;
	}
	if(plr[p]._pClass == PC_ROGUE) {
		madd += madd >> 1;
	}
	imana += madd << 6;

	plr[p]._pHitPoints = ihp + plr[p]._pHPBase;
	plr[p]._pMaxHP = ihp + plr[p]._pMaxHPBase;
	if(p == myplr && plr[p]._pHitPoints >> 6 <= 0) {
		SetPlayerHitPoints(p, 0);
	}

	plr[p]._pMana = imana + plr[p]._pManaBase;
	plr[p]._pMaxMana = imana + plr[p]._pMaxManaBase;

	plr[p]._pIFMinDam = fmin;
	plr[p]._pIFMaxDam = fmax;
	plr[p]._pILMinDam = lmin;
	plr[p]._pILMaxDam = lmax;

	if(iflgs & ISPL_INFRAVISION) {
		plr[p]._pInfraFlag = 1;
	} else {
		plr[p]._pInfraFlag = 0;
	}

	plr[p]._pBlockFlag = 0;
	plr[p]._pwtype = 0;

	g = 0;
	if(plr[p].InvBody[4]._itype != -1 && plr[p].InvBody[4]._iClass == ICLASS_WEAPON && plr[p].InvBody[4]._iStatFlag) {
		g = plr[p].InvBody[4]._itype;
	}
	if(plr[p].InvBody[5]._itype != -1 && plr[p].InvBody[5]._iClass == ICLASS_WEAPON && plr[p].InvBody[5]._iStatFlag) {
		g = plr[p].InvBody[5]._itype;
	}

	switch(g) {
	case ITYPE_SWORD:
		g = 2;
		break;
	case ITYPE_MACE:
		g = 6;
		break;
	case ITYPE_BOW:
		plr[p]._pwtype = 1;
		g = 4;
		break;
	case ITYPE_AXE:
		g = 5;
		break;
	case ITYPE_STAFF:
		g = 8;
		break;
	}

	if(plr[p].InvBody[4]._itype == ITYPE_SHIELD && plr[p].InvBody[4]._iStatFlag) {
		plr[p]._pBlockFlag = 1;
		g++;
	}
	if(plr[p].InvBody[5]._itype == ITYPE_SHIELD && plr[p].InvBody[5]._iStatFlag) {
		plr[p]._pBlockFlag = 1;
		g++;
	}
	if(plr[p].InvBody[6]._itype == ITYPE_MARMOR && plr[p].InvBody[6]._iStatFlag) {
		g += 16;
	}
	if(plr[p].InvBody[6]._itype == ITYPE_HARMOR && plr[p].InvBody[6]._iStatFlag) {
		g += 32;
	}
	if(plr[p]._pgfxnum != g && Loadgfx) {
		plr[p]._pgfxnum = g;
		plr[p]._pGFXLoad = 0;
		LoadPlrGFX(p, 1);
		SetPlrAnims(p);
		d = plr[p]._pdir;
		/// ASSERT: assert(plr[p]._pNAnim[d]);
		plr[p]._pAnimData = plr[p]._pNAnim[d];
		plr[p]._pAnimLen = plr[p]._pNFrames;
		plr[p]._pAnimFrame = 1;
		plr[p]._pAnimCnt = 0;
		plr[p]._pAnimDelay = 3;
		plr[p]._pAnimWidth = plr[p]._pNWidth;
		plr[p]._pAnimWidth2 = (plr[p]._pNWidth - 64) >> 1;
	} else {
		plr[p]._pgfxnum = g;
	}

	for(i = 0; i < nummissiles; i++) {
		mi = missileactive[i];
		if(missile[mi]._mitype == MIS_MANASHIELD && missile[mi]._misource == p) {
			missile[mi]._miVar1 = plr[p]._pHitPoints;
			missile[mi]._miVar2 = plr[p]._pHPBase;
		}
	}

	drawmanaflag = 1;
	drawhpflag = 1;
}

void CalcPlrScrolls(int p)
{
	int i;
	__int64 t; /* todo: check usage */

	plr[p]._pScrlSpells64 = 0;

	for(i = 0; i < plr[p]._pNumInv; i++) {
		if(plr[p].InvList[i]._itype == -1) {
			continue;
		}
		if(plr[p].InvList[i]._iMiscId == IMISC_SCROLL || plr[p].InvList[i]._iMiscId == IMISC_SCROLLT) {
			if(plr[p].InvList[i]._iStatFlag) {
				t = 1;
				plr[p]._pScrlSpells64 |= t << (plr[p].InvList[i]._iSpell - 1);
			}
		}
	}
	for(i = 0; i < 8; i++) {
		if(plr[p].SpdList[i]._itype == -1) {
			continue;
		}
		if(plr[p].SpdList[i]._iMiscId == IMISC_SCROLL || plr[p].SpdList[i]._iMiscId == IMISC_SCROLLT) {
			if(plr[p].SpdList[i]._iStatFlag) {
				t = 1;
				plr[p]._pScrlSpells64 |= t << (plr[p].SpdList[i]._iSpell - 1);
			}
		}
	}

	/// BUGFIX: add 't = 1' and use in next line
	if(plr[p]._pRSplType == RSPLTYPE_SCROLL && !(plr[p]._pScrlSpells64 & 1 << (plr[p]._pRSpell - 1))) {
		plr[p]._pRSpell = -1;
		plr[p]._pRSplType = RSPLTYPE_INVALID;
		force_redraw = 255;
	}
}

void CalcPlrStaff(int p)
{
	__int64 t; /* todo: check usage */

	plr[p]._pISpells64 = 0;

	if(plr[p].InvBody[4]._itype != -1 && plr[p].InvBody[4]._iStatFlag && plr[p].InvBody[4]._iCharges > 0) {
		t = 1;
		plr[p]._pISpells64 = t << (plr[p].InvBody[4]._iSpell - 1);
	}
}

void CalcSelfItems(int pnum)
{
	int i, sa, ma, da;
	BOOL sf, changeflag;
	PlayerStruct *p;

	p = &plr[pnum];

	sa = 0;
	ma = 0;
	da = 0;

	for(i = 0; i < NUM_INVLOC; i++) {
		if(p->InvBody[i]._itype == -1) {
			continue;
		}
		p->InvBody[i]._iStatFlag = 1;
		if(p->InvBody[i]._iIdentified) {
			sa += p->InvBody[i]._iPLStr;
			ma += p->InvBody[i]._iPLMag;
			da += p->InvBody[i]._iPLDex;
		}
	}

	while(1) {
		changeflag = FALSE;
		for(i = 0; i < NUM_INVLOC; i++) {
			if(p->InvBody[i]._itype == -1 || !p->InvBody[i]._iStatFlag) {
				continue;
			}
			sf = TRUE;
			if(sa + p->_pBaseStr < p->InvBody[i]._iMinStr) {
				sf = FALSE;
			}
			if(ma + p->_pBaseMag < p->InvBody[i]._iMinMag) {
				sf = FALSE;
			}
			if(da + p->_pBaseDex < p->InvBody[i]._iMinDex) {
				sf = FALSE;
			}
			if(!sf) {
				changeflag = TRUE;
				p->InvBody[i]._iStatFlag = 0;
				if(p->InvBody[i]._iIdentified) {
					sa -= p->InvBody[i]._iPLStr;
					ma -= p->InvBody[i]._iPLMag;
					da -= p->InvBody[i]._iPLDex;
				}
			}
		}
		if(!changeflag) {
			break;
		}
	}
}

void CalcPlrItemMin(int pnum)
{
	int i;
	PlayerStruct *p;
	ItemStruct *pi;

	p = &plr[pnum];

	pi = p->InvList;
	i = p->_pNumInv;
	while(i--) {
		pi->_iStatFlag = ItemMinStats(p, pi);
		pi++;
	}

	pi = p->SpdList;
	i = 8;
	while(i--) {
		if(pi->_itype != -1) {
			pi->_iStatFlag = ItemMinStats(p, pi);
		}
		pi++;
	}
}

BOOL ItemMinStats(PlayerStruct *p, ItemStruct *x)
{
	if(p->_pMagic < x->_iMinMag) {
		return FALSE;
	}
	if(p->_pStrength < x->_iMinStr) {
		return FALSE;
	}
	if(p->_pDexterity < x->_iMinDex) {
		return FALSE;
	}

	return TRUE;
}

void CalcPlrBookVals(int p)
{
	int i, slvl;

	if(currlevel == 0) {
		for(i = 1; witchitem[i]._itype != -1; i++) {
			WitchBookLevel(i);
			witchitem[i]._iStatFlag = StoreStatOk(&witchitem[i]);
		}
	}

	for(i = 0; i < plr[p]._pNumInv; i++) {
		if(plr[p].InvList[i]._itype == ITYPE_MISC && plr[p].InvList[i]._iMiscId == IMISC_BOOK) {
			plr[p].InvList[i]._iMinMag = spelldata[plr[p].InvList[i]._iSpell].sMinInt;
			slvl = plr[p]._pSplLvl[plr[p].InvList[i]._iSpell];
			while(slvl != 0) {
				plr[p].InvList[i]._iMinMag += 20 * plr[p].InvList[i]._iMinMag / 100;
				slvl--;
				if(plr[p].InvList[i]._iMinMag + 20 * plr[p].InvList[i]._iMinMag / 100 > 255) {
					plr[p].InvList[i]._iMinMag = 255;
					slvl = 0;
				}
			}
			plr[p].InvList[i]._iStatFlag = ItemMinStats(&plr[p], &plr[p].InvList[i]);
		}
	}
}

void CalcPlrInv(int p, BOOL Loadgfx)
{
	CalcPlrItemMin(p);
	CalcSelfItems(p);
	CalcPlrItemVals(p, Loadgfx);
	CalcPlrItemMin(p);

	if(p == myplr) {
		CalcPlrBookVals(p);
		CalcPlrScrolls(p);
		CalcPlrStaff(p);
	}
	if(p == myplr && currlevel == 0) {
		RecalcStoreStats();
	}
}

void SetPlrHandItem(ItemStruct *h, int idata)
{
	ItemDataStruct *pAllItem;

	pAllItem = &AllItemsList[idata];
	memset(h, 0, sizeof(*h));
	h->_itype = pAllItem->itype;
	h->_iCurs = pAllItem->iCurs;
	strcpy(h->_iName, pAllItem->iName);
	strcpy(h->_iIName, pAllItem->iName);
	h->_iLoc = pAllItem->iLoc;
	h->_iClass = pAllItem->iClass;
	h->_iMinDam = pAllItem->iMinDam;
	h->_iMaxDam = pAllItem->iMaxDam;
	h->_iAC = pAllItem->iMinAC;
	h->_iMiscId = pAllItem->iMiscId;
	h->_iSpell = pAllItem->iSpell;
	if(pAllItem->iMiscId == IMISC_STAFF) {
		h->_iCharges = 40;
	}
	h->_iMaxCharges = h->_iCharges;
	h->_iDurability = pAllItem->iDurability;
	h->_iMaxDur = pAllItem->iDurability;
	h->_iMinStr = pAllItem->iMinStr;
	h->_iMinMag = pAllItem->iMinMag;
	h->_iMinDex = pAllItem->iMinDex;
	h->_ivalue = pAllItem->iValue;
	h->_iIvalue = pAllItem->iValue;
	h->_iPrePower = -1;
	h->_iSufPower = -1;
	h->IDidx = idata;
	h->_iMagical = 0;
}

void GetPlrHandSeed(ItemStruct *h)
{
	h->_iSeed = GetRndSeed();
}

void GetGoldSeed(int pnum, ItemStruct *h)
{
	int i, ii, s;
	BOOL doneflag;

	while(1) {
		doneflag = TRUE;
		s = GetRndSeed();
		for(i = 0; i < numitems; i++) {
			ii = itemactive[i];
			if(item[ii]._iSeed == s) {
				doneflag = FALSE;
			}
		}
		if(pnum == myplr) {
			for(i = 0; i < plr[pnum]._pNumInv; i++) {
				if(plr[pnum].InvList[i]._iSeed == s) {
					doneflag = FALSE;
				}
			}
		}
		if(doneflag) {
			break;
		}
	}

	h->_iSeed = s;
}

void SetPlrHandSeed(ItemStruct *h, int iseed)
{
	h->_iSeed = iseed;
}

void SetPlrHandGoldCurs(ItemStruct *h)
{
	if(h->_ivalue >= 2500) {
		h->_iCurs = 6;
	} else if(h->_ivalue <= 1000) {
		h->_iCurs = 4;
	} else {
		h->_iCurs = 5;
	}
}

void CreatePlrItems(int p)
{
	int i;
	ItemStruct *pi;

	pi = plr[p].InvBody;
	i = NUM_INVLOC;
	while(i--) {
		pi->_itype = -1;
		pi++;
	}

	memset(plr[p].InvGrid, 0, sizeof(plr[p].InvGrid));

	pi = plr[p].InvList;
	i = 40;
	while(i--) {
		pi->_itype = -1;
		pi++;
	}

	plr[p]._pNumInv = 0;

	pi = plr[p].SpdList;
	i = 8;
	while(i--) {
		pi->_itype = -1;
		pi++;
	}

	switch(plr[p]._pClass) {
	case PC_WARRIOR:
		SetPlrHandItem(&plr[p].InvBody[4], IDI_WARRIOR);
		GetPlrHandSeed(&plr[p].InvBody[4]);
		SetPlrHandItem(&plr[p].InvBody[5], IDI_WARRSHLD);
		GetPlrHandSeed(&plr[p].InvBody[5]);
#ifdef _DEBUG
		if(!debug_mode_key_w) {
#endif
			SetPlrHandItem(&plr[p].HoldItem, IDI_WARRCLUB);
			GetPlrHandSeed(&plr[p].HoldItem);
			AutoPlace(p, 0, 1, 3, TRUE);
#ifdef _DEBUG
		}
#endif
		SetPlrHandItem(&plr[p].SpdList[0], IDI_HEAL);
		GetPlrHandSeed(&plr[p].SpdList[0]);
		SetPlrHandItem(&plr[p].SpdList[1], IDI_HEAL);
		GetPlrHandSeed(&plr[p].SpdList[1]);
		break;
	case PC_ROGUE:
		SetPlrHandItem(&plr[p].InvBody[4], IDI_ROGUE);
		GetPlrHandSeed(&plr[p].InvBody[4]);
		SetPlrHandItem(&plr[p].SpdList[0], IDI_HEAL);
		GetPlrHandSeed(&plr[p].SpdList[0]);
		SetPlrHandItem(&plr[p].SpdList[1], IDI_HEAL);
		GetPlrHandSeed(&plr[p].SpdList[1]);
		break;
	case PC_SORCERER:
		SetPlrHandItem(&plr[p].InvBody[4], IDI_SORCEROR);
		GetPlrHandSeed(&plr[p].InvBody[4]);
		SetPlrHandItem(&plr[p].SpdList[0], IDI_MANA);
		GetPlrHandSeed(&plr[p].SpdList[0]);
		SetPlrHandItem(&plr[p].SpdList[1], IDI_MANA);
		GetPlrHandSeed(&plr[p].SpdList[1]);
		break;
	}

	SetPlrHandItem(&plr[p].HoldItem, IDI_GOLD);
	GetPlrHandSeed(&plr[p].HoldItem);

#ifdef _DEBUG
	if(!debug_mode_key_w) {
#endif
		plr[p].HoldItem._ivalue = 100;
		plr[p].HoldItem._iCurs = 4;
		plr[p]._pGold = plr[p].HoldItem._ivalue;
		plr[p].InvList[plr[p]._pNumInv++] = plr[p].HoldItem;
		plr[p].InvGrid[30] = plr[p]._pNumInv;
#ifdef _DEBUG
	} else {
		plr[p].HoldItem._ivalue = 5000;
		plr[p].HoldItem._iCurs = 6;
		plr[p]._pGold = plr[p].HoldItem._ivalue * 40;
		for(i = 0; i < 40; i++) {
			GetPlrHandSeed(&plr[p].HoldItem);
			plr[p].InvList[plr[p]._pNumInv++] = plr[p].HoldItem;
			plr[p].InvGrid[i] = plr[p]._pNumInv;
		}
	}
#endif

	CalcPlrItemVals(p, FALSE);
}

BOOL ItemSpaceOk(int i, int j)
{
	int pn, oi;

	if(i < 0 || i >= MAXDUNX || j < 0 || j >= MAXDUNY) {
		return FALSE;
	}
	if(dMonster[i][j] != 0) {
		return FALSE;
	}
	if(dPlayer[i][j] != 0) {
		return FALSE;
	}
	if(dItem[i][j] != 0) {
		return FALSE;
	}
	if(dObject[i][j] != 0) {
		oi = dObject[i][j] > 0 ? dObject[i][j] - 1 : -(dObject[i][j] + 1);
		if(object[oi]._oSolidFlag) {
			return FALSE;
		}
	}
	if(dObject[i + 1][j + 1] > 0) {
		oi = dObject[i + 1][j + 1] - 1;
		if(object[oi]._oSelFlag) {
			return FALSE;
		}
	}
	if(dObject[i + 1][j + 1] < 0) {
		oi = -(dObject[i + 1][j + 1] + 1);
		if(object[oi]._oSelFlag) {
			return FALSE;
		}
	}
	if(dObject[i + 1][j] > 0 && dObject[i][j + 1] > 0) {
		oi = dObject[i + 1][j] - 1;
		if(object[oi]._oSelFlag) {
			oi = dObject[i][j + 1] - 1;
			if(object[oi]._oSelFlag) {
				return FALSE;
			}
		}
	}
	pn = dPiece[i][j];
	if(nSolidTable[pn]) {
		return FALSE;
	}

	return TRUE;
}

BOOL GetItemSpace(int x, int y, char inum)
{
	int i, j, xx, yy, rs;
	BOOL savail;

	yy = 0;
	for(j = y - 1; j <= y + 1; j++) {
		xx = 0;
		for(i = x - 1; i <= x + 1; i++) {
			itemhold[xx][yy] = ItemSpaceOk(i, j);
			xx++;
		}
		yy++;
	}

	savail = FALSE;
	for(yy = 0; yy < 3; yy++) {
		for(xx = 0; xx < 3; xx++) {
			if(itemhold[xx][yy]) {
				savail = TRUE;
			}
		}
	}

	rs = random(13, 15) + 1;
	if(!savail) {
		return FALSE;
	}

	xx = 0;
	yy = 0;
	while(rs > 0) {
		if(itemhold[xx][yy]) {
			rs--;
		}
		if(rs > 0) {
			xx++;
			if(xx == 3) {
				xx = 0;
				yy++;
				if(yy == 3) {
					yy = 0;
				}
			}
		}
	}

	xx += x - 1;
	yy += y - 1;
	item[inum]._ix = xx;
	item[inum]._iy = yy;
	dItem[xx][yy] = inum + 1;
	return TRUE;
}

void GetSuperItemSpace(int x, int y, char inum)
{
	int i, j, l, xx, yy;

	if(GetItemSpace(x, y, inum)) {
		return;
	}

	for(l = 2; l < 50; l++) {
		for(j = -l; j <= l; j++) {
			yy = j + y;
			for(i = -l; i <= l; i++) {
				xx = i + x;
				if(ItemSpaceOk(xx, yy)) {
					item[inum]._ix = xx;
					item[inum]._iy = yy;
					dItem[xx][yy] = inum + 1;
					return;
				}
			}
		}
	}
}

void GetSuperItemLoc(int x, int y, int &xx, int &yy)
{
	int i, j, l;

	for(l = 1; l < 50; l++) {
		for(j = -l; j <= l; j++) {
			yy = j + y;
			for(i = -l; i <= l; i++) {
				xx = i + x;
				if(ItemSpaceOk(xx, yy)) {
					return;
				}
			}
		}
	}
}

void CalcItemValue(int i)
{
	int v;

	v = item[i]._iVMult1 + item[i]._iVMult2;
	if(v > 0) {
		v *= item[i]._ivalue;
	}
	if(v < 0) {
		v = item[i]._ivalue / v;
	}
	v += item[i]._iVAdd1 + item[i]._iVAdd2;
	if(v <= 0) {
		v = 1;
	}

	item[i]._iIvalue = v;
}

void GetBookSpell(int i, int lvl)
{
	int rv, s, bs;

	if(lvl == 0) {
		lvl = 1;
	}

	rv = random(14, 37) + 1;
	s = 1;
	while(rv > 0) {
		if(spelldata[s].sBookLvl != -1 && lvl >= spelldata[s].sBookLvl) {
			rv--;
			bs = s;
		}
		s++;
		if(gbMaxPlayers == 1 && s == SPL_RESURRECT) {
			s = SPL_TELEKINESIS;
		}
		if(gbMaxPlayers == 1 && s == SPL_HEALOTHER) {
			s = SPL_FLARE;
		}
		if(s == 37) {
			s = 1;
		}
	}

	strcat(item[i]._iName, spelldata[bs].sNameText);
	strcat(item[i]._iIName, spelldata[bs].sNameText);
	item[i]._iSpell = bs;
	item[i]._iMinMag = spelldata[bs].sMinInt;
	item[i]._ivalue += spelldata[bs].sBookCost;
	item[i]._iIvalue += spelldata[bs].sBookCost;

	if(spelldata[bs].sType == STYPE_FIRE) {
		item[i]._iCurs = 87; // Red Book
	}
	if(spelldata[bs].sType == STYPE_LIGHTNING) {
		item[i]._iCurs = 88; // Blue Book
	}
	if(spelldata[bs].sType == STYPE_MAGIC) {
		item[i]._iCurs = 86; // Black Book
	}
}

void GetStaffPower(int i, int lvl, int bs, BOOL onlygood)
{
	int nl, j, preidx, rv, idata;
	BOOL addok;
	int l[256];
	char istr[128];

	rv = random(15, 10);
	preidx = -1;
#ifdef _DEBUG
	if(rv == 0 || debug_mode_key_inverted_v || onlygood) {
#else
	if(rv == 0 || onlygood) {
#endif
		nl = 0;
		for(j = 0; PL_Prefix[j].PLPower != -1; j++) {
			if(PL_Prefix[j].PLIType & PLT_STAFF && PL_Prefix[j].PLMinLvl <= lvl) {
				addok = TRUE;
				if(onlygood && !PL_Prefix[j].PLOk) {
					addok = FALSE;
				}
				if(addok) {
					l[nl++] = j;
					if(PL_Prefix[j].PLDouble) {
						l[nl++] = j;
					}
				}
			}
		}
		if(nl != 0) {
			preidx = l[random(16, nl)];
			sprintf(istr, "%s %s", PL_Prefix[preidx].PLName, item[i]._iIName);
			strcpy(item[i]._iIName, istr);
			item[i]._iMagical = 1;
			SaveItemPower(
				i,
				PL_Prefix[preidx].PLPower,
				PL_Prefix[preidx].PLParam1,
				PL_Prefix[preidx].PLParam2,
				PL_Prefix[preidx].PLMinVal,
				PL_Prefix[preidx].PLMaxVal,
				PL_Prefix[preidx].PLMultVal);
			item[i]._iPrePower = PL_Prefix[preidx].PLPower;
		}
	}
	if(!control_WriteStringToBuffer(item[i]._iIName)) {
		idata = item[i].IDidx;
		strcpy(item[i]._iIName, AllItemsList[idata].iSName);
		if(preidx != -1) {
			sprintf(istr, "%s %s", PL_Prefix[preidx].PLName, item[i]._iIName);
			strcpy(item[i]._iIName, istr);
		}
		sprintf(istr, "%s of %s", item[i]._iIName, spelldata[bs].sNameText);
		strcpy(item[i]._iIName, istr);
		if(item[i]._iMagical == 0) {
			strcpy(item[i]._iName, item[i]._iIName);
		}
	}

	CalcItemValue(i);
}

void GetStaffSpell(int i, int lvl, BOOL onlygood)
{
	int rv, s, l, bs, maxc, minc, v;
	char istr[64];

	if(random(17, 4) == 0) {
		GetItemPower(i, lvl >> 1, lvl, PLT_STAFF, onlygood);
		return;
	}

	l = lvl >> 1;
	if(l == 0) {
		l = 1;
	}
	rv = random(18, 37) + 1;
	s = 1;
	while(rv > 0) {
		if(spelldata[s].sStaffLvl != -1 && l >= spelldata[s].sStaffLvl) {
			rv--;
			bs = s;
		}
		s++;
		if(gbMaxPlayers == 1 && s == SPL_RESURRECT) {
			s = SPL_TELEKINESIS;
		}
		if(gbMaxPlayers == 1 && s == SPL_HEALOTHER) {
			s = SPL_FLARE;
		}
		if(s == 37) {
			s = 1;
		}
	}

	sprintf(istr, "%s of %s", item[i]._iName, spelldata[bs].sNameText);
	if(!control_WriteStringToBuffer(istr)) {
		sprintf(istr, "Staff of %s", spelldata[bs].sNameText);
	}
	strcpy(item[i]._iName, istr);
	strcpy(item[i]._iIName, istr);

	item[i]._iSpell = bs;
	minc = spelldata[bs].sStaffMin;
	maxc = spelldata[bs].sStaffMax;
	item[i]._iCharges = random(19, maxc - minc + 1) + minc;
	item[i]._iMaxCharges = item[i]._iCharges;
	item[i]._iMinMag = spelldata[bs].sMinInt;
	v = item[i]._iCharges * spelldata[bs].sStaffCost / 5;
	item[i]._ivalue += v;
	item[i]._iIvalue += v;
	GetStaffPower(i, lvl, bs, onlygood);
}

void GetItemAttrs(int i, int idata, int lvl)
{
	int rndv;

	item[i]._itype = AllItemsList[idata].itype;
	item[i]._iCurs = AllItemsList[idata].iCurs;
	strcpy(item[i]._iName, AllItemsList[idata].iName);
	strcpy(item[i]._iIName, AllItemsList[idata].iName);
	item[i]._iLoc = AllItemsList[idata].iLoc;
	item[i]._iClass = AllItemsList[idata].iClass;
	item[i]._iMinDam = AllItemsList[idata].iMinDam;
	item[i]._iMaxDam = AllItemsList[idata].iMaxDam;
	item[i]._iAC = random(20, AllItemsList[idata].iMaxAC - AllItemsList[idata].iMinAC + 1) + AllItemsList[idata].iMinAC;
	item[i]._iFlags = AllItemsList[idata].iFlags;
	item[i]._iMiscId = AllItemsList[idata].iMiscId;
	item[i]._iSpell = AllItemsList[idata].iSpell;
	item[i]._iMagical = 0;
	item[i]._ivalue = AllItemsList[idata].iValue;
	item[i]._iIvalue = AllItemsList[idata].iValue;
	item[i]._iVAdd1 = 0;
	item[i]._iVMult1 = 0;
	item[i]._iVAdd2 = 0;
	item[i]._iVMult2 = 0;
	item[i]._iPLDam = 0;
	item[i]._iPLToHit = 0;
	item[i]._iPLAC = 0;
	item[i]._iPLStr = 0;
	item[i]._iPLMag = 0;
	item[i]._iPLDex = 0;
	item[i]._iPLVit = 0;
	item[i]._iCharges = 0;
	item[i]._iMaxCharges = 0;
	item[i]._iDurability = AllItemsList[idata].iDurability;
	item[i]._iMaxDur = AllItemsList[idata].iDurability;
	item[i]._iMinStr = AllItemsList[idata].iMinStr;
	item[i]._iMinMag = AllItemsList[idata].iMinMag;
	item[i]._iMinDex = AllItemsList[idata].iMinDex;
	item[i]._iPLFR = 0;
	item[i]._iPLLR = 0;
	item[i]._iPLMR = 0;
	item[i].IDidx = idata;
	item[i]._iPLDamMod = 0;
	item[i]._iPLGetHit = 0;
	item[i]._iPLLight = 0;
	item[i]._iSplLvlAdd = 0;
	item[i]._iRequest = 0;
	item[i]._iFMinDam = 0;
	item[i]._iFMaxDam = 0;
	item[i]._iLMinDam = 0;
	item[i]._iLMaxDam = 0;
	item[i]._iPLEnAc = 0;
	item[i]._iPLMana = 0;
	item[i]._iPLHP = 0;
	item[i]._iPrePower = -1;
	item[i]._iSufPower = -1;

	if(item[i]._iMiscId == IMISC_BOOK) {
		GetBookSpell(i, lvl);
	}
	if(item[i]._itype == ITYPE_GOLD) {
		if(gnDifficulty == DIFF_NORMAL) {
			rndv = 5 * currlevel + random(21, 10 * currlevel);
		}
		if(gnDifficulty == DIFF_NIGHTMARE) {
			rndv = 5 * (currlevel + 16) + random(21, 10 * (currlevel + 16));
		}
		if(gnDifficulty == DIFF_HELL) {
			rndv = 5 * (currlevel + 32) + random(21, 10 * (currlevel + 32));
		}
		if(leveltype == DTYPE_HELL) {
			rndv += rndv >> 3;
		}
		if(rndv > 5000) {
			rndv = 5000;
		}
		item[i]._ivalue = rndv;
		if(rndv >= 2500) {
			item[i]._iCurs = 6;
		} else if(rndv <= 1000) {
			item[i]._iCurs = 4;
		} else {
			item[i]._iCurs = 5;
		}
	}
}

int RndPL(int param1, int param2)
{
	return param1 + random(22, param2 - param1 + 1);
}

int PLVal(int pv, int p1, int p2, int minv, int maxv)
{
	if(p1 == p2) {
		return minv;
	}
	if(minv == maxv) {
		return minv;
	}

	return minv + (maxv - minv) * (100 * (pv - p1) / (p2 - p1)) / 100;
}

void SaveItemPower(int i, int power, int param1, int param2, int minval, int maxval, int multval)
{
	int r, r2;

	r = RndPL(param1, param2);

	switch(power) {
	case IPL_TOHIT:
		item[i]._iPLToHit += r;
		break;
	case IPL_TOHIT_CURSE:
		item[i]._iPLToHit -= r;
		break;
	case IPL_DAMP:
		item[i]._iPLDam += r;
		break;
	case IPL_DAMP_CURSE:
		item[i]._iPLDam -= r;
		break;
	case IPL_TOHIT_DAMP:
		r = RndPL(param1, param2);
		item[i]._iPLDam += r;
		if(param1 == 20) {
			r2 = RndPL(1, 5);
		}
		if(param1 == 36) {
			r2 = RndPL(6, 10);
		}
		if(param1 == 51) {
			r2 = RndPL(11, 15);
		}
		if(param1 == 66) {
			r2 = RndPL(16, 20);
		}
		if(param1 == 81) {
			r2 = RndPL(21, 30);
		}
		if(param1 == 96) {
			r2 = RndPL(31, 40);
		}
		if(param1 == 111) {
			r2 = RndPL(41, 50);
		}
		if(param1 == 126) {
			r2 = RndPL(51, 75);
		}
		if(param1 == 151) {
			r2 = RndPL(76, 100);
		}
		item[i]._iPLToHit += r2;
		break;
	case IPL_TOHIT_DAMP_CURSE:
		item[i]._iPLDam -= r;
		if(param1 == 25) {
			r2 = RndPL(1, 5);
		}
		if(param1 == 50) {
			r2 = RndPL(6, 10);
		}
		item[i]._iPLToHit -= r2;
		break;
	case IPL_ACP:
		item[i]._iPLAC += r;
		break;
	case IPL_ACP_CURSE:
		item[i]._iPLAC -= r;
		break;
	case IPL_SETAC:
		item[i]._iAC = r;
		break;
	case IPL_AC_CURSE:
		item[i]._iAC -= r;
		break;
	case IPL_FIRERES:
		item[i]._iPLFR += r;
		break;
	case IPL_LIGHTRES:
		item[i]._iPLLR += r;
		break;
	case IPL_MAGICRES:
		item[i]._iPLMR += r;
		break;
	case IPL_ALLRES:
		item[i]._iPLFR += r;
		item[i]._iPLLR += r;
		item[i]._iPLMR += r;
		if(item[i]._iPLFR < 0) {
			item[i]._iPLFR = 0;
		}
		if(item[i]._iPLLR < 0) {
			item[i]._iPLLR = 0;
		}
		if(item[i]._iPLMR < 0) {
			item[i]._iPLMR = 0;
		}
		break;
	case IPL_SPLLVLADD:
		item[i]._iSplLvlAdd = r;
		break;
	case IPL_CHARGES:
		item[i]._iCharges *= param1;
		item[i]._iMaxCharges = item[i]._iCharges;
		break;
	case IPL_SPELL:
		item[i]._iSpell = param1;
		item[i]._iCharges = param1;
		item[i]._iMaxCharges = param2;
		break;
	case IPL_FIREDAM:
		item[i]._iFlags |= ISPL_FIREDAM;
		item[i]._iFMinDam = param1;
		item[i]._iFMaxDam = param2;
		break;
	case IPL_LIGHTDAM:
		item[i]._iFlags |= ISPL_LIGHTDAM;
		item[i]._iLMinDam = param1;
		item[i]._iLMaxDam = param2;
		break;
	case IPL_STR:
		item[i]._iPLStr += r;
		break;
	case IPL_STR_CURSE:
		item[i]._iPLStr -= r;
		break;
	case IPL_MAG:
		item[i]._iPLMag += r;
		break;
	case IPL_MAG_CURSE:
		item[i]._iPLMag -= r;
		break;
	case IPL_DEX:
		item[i]._iPLDex += r;
		break;
	case IPL_DEX_CURSE:
		item[i]._iPLDex -= r;
		break;
	case IPL_VIT:
		item[i]._iPLVit += r;
		break;
	case IPL_VIT_CURSE:
		item[i]._iPLVit -= r;
		break;
	case IPL_ATTRIBS:
		item[i]._iPLStr += r;
		item[i]._iPLMag += r;
		item[i]._iPLDex += r;
		item[i]._iPLVit += r;
		break;
	case IPL_ATTRIBS_CURSE:
		item[i]._iPLStr -= r;
		item[i]._iPLMag -= r;
		item[i]._iPLDex -= r;
		item[i]._iPLVit -= r;
		break;
	case IPL_GETHIT_CURSE:
		item[i]._iPLGetHit += r;
		break;
	case IPL_GETHIT:
		item[i]._iPLGetHit -= r;
		break;
	case IPL_LIFE:
		item[i]._iPLHP += r << 6;
		break;
	case IPL_LIFE_CURSE:
		item[i]._iPLHP -= r << 6;
		break;
	case IPL_MANA:
		item[i]._iPLMana += r << 6;
		drawmanaflag = 1;
		break;
	case IPL_MANA_CURSE:
		item[i]._iPLMana -= r << 6;
		drawmanaflag = 1;
		break;
	case IPL_DUR:
		r2 = r * item[i]._iMaxDur / 100;
		item[i]._iMaxDur += r2;
		item[i]._iDurability += r2;
		break;
	case IPL_DUR_CURSE:
		item[i]._iMaxDur -= r * item[i]._iMaxDur / 100;
		if(item[i]._iMaxDur < 1) {
			item[i]._iMaxDur = 1;
		}
		item[i]._iDurability = item[i]._iMaxDur;
		break;
	case IPL_INDESTRUCTIBLE:
		item[i]._iDurability = 255;
		item[i]._iMaxDur = 255;
		break;
	case IPL_LIGHT:
		item[i]._iPLLight += param1;
		break;
	case IPL_LIGHT_CURSE:
		item[i]._iPLLight -= param1;
		break;
	case IPL_FIRE_ARROWS:
		item[i]._iFlags |= ISPL_FIRE_ARROWS;
		item[i]._iFMinDam = param1;
		item[i]._iFMaxDam = param2;
		break;
	case IPL_LIGHT_ARROWS:
		item[i]._iFlags |= ISPL_LIGHT_ARROWS;
		item[i]._iLMinDam = param1;
		item[i]._iLMaxDam = param2;
		break;
	case IPL_THORNS:
		item[i]._iFlags |= ISPL_THORNS;
		break;
	case IPL_NOMANA:
		item[i]._iFlags |= ISPL_NOMANA;
		drawmanaflag = 1;
		break;
	case IPL_NOHEALPLR:
		item[i]._iFlags |= ISPL_NOHEALPLR;
		break;
	case IPL_ABSHALFTRAP:
		item[i]._iFlags |= ISPL_ABSHALFTRAP;
		break;
	case IPL_KNOCKBACK:
		item[i]._iFlags |= ISPL_KNOCKBACK;
		break;
	case IPL_3XDAMVDEM:
		item[i]._iFlags |= ISPL_3XDAMVDEM;
		break;
	case IPL_ALLRESZERO:
		item[i]._iFlags |= ISPL_ALLRESZERO;
		break;
	case IPL_NOHEALMON:
		item[i]._iFlags |= ISPL_NOHEALMON;
		break;
	case IPL_STEALMANA:
		if(param1 == 3) {
			item[i]._iFlags |= ISPL_STEALMANA_3;
		}
		if(param1 == 5) {
			item[i]._iFlags |= ISPL_STEALMANA_5;
		}
		drawmanaflag = 1;
		break;
	case IPL_STEALLIFE:
		if(param1 == 3) {
			item[i]._iFlags |= ISPL_STEALLIFE_3;
		}
		if(param1 == 5) {
			item[i]._iFlags |= ISPL_STEALLIFE_5;
		}
		drawhpflag = 1;
		break;
	case IPL_TARGAC:
		item[i]._iPLEnAc += r;
		break;
	case IPL_FASTATTACK:
		if(param1 == 1) {
			item[i]._iFlags |= ISPL_QUICKATTACK;
		}
		if(param1 == 2) {
			item[i]._iFlags |= ISPL_FASTATTACK;
		}
		if(param1 == 3) {
			item[i]._iFlags |= ISPL_FASTERATTACK;
		}
		if(param1 == 4) {
			item[i]._iFlags |= ISPL_FASTESTATTACK;
		}
		break;
	case IPL_FASTRECOVER:
		if(param1 == 1) {
			item[i]._iFlags |= ISPL_FASTRECOVER;
		}
		if(param1 == 2) {
			item[i]._iFlags |= ISPL_FASTERRECOVER;
		}
		if(param1 == 3) {
			item[i]._iFlags |= ISPL_FASTESTRECOVER;
		}
		break;
	case IPL_FASTBLOCK:
		item[i]._iFlags |= ISPL_FASTBLOCK;
		break;
	case IPL_DAMMOD:
		item[i]._iPLDamMod += r;
		break;
	case IPL_RNDARROWVEL:
		item[i]._iFlags |= ISPL_RNDARROWVEL;
		break;
	case IPL_SETDAM:
		item[i]._iMinDam = param1;
		item[i]._iMaxDam = param2;
		break;
	case IPL_SETDUR:
		item[i]._iDurability = param1;
		item[i]._iMaxDur = param1;
		break;
	case IPL_FASTSWING:
		item[i]._iFlags |= ISPL_FASTERATTACK;
		break;
	case IPL_ONEHAND:
		item[i]._iLoc = ILOC_ONEHAND;
		break;
	case IPL_DRAINLIFE:
		item[i]._iFlags |= ISPL_DRAINLIFE;
		break;
	case IPL_RNDSTEALLIFE:
		item[i]._iFlags |= ISPL_RNDSTEALLIFE;
		break;
	case IPL_INFRAVISION:
		item[i]._iFlags |= ISPL_INFRAVISION;
		break;
	case IPL_NOMINSTR:
		item[i]._iMinStr = 0;
		break;
	case IPL_INVCURS:
		item[i]._iCurs = param1;
		break;
	case IPL_ADDACLIFE:
		item[i]._iPLHP = (plr[myplr]._pIBonusAC + plr[myplr]._pIAC + plr[myplr]._pDexterity / 5) << 6;
		break;
	case IPL_ADDMANAAC:
		item[i]._iAC += (plr[myplr]._pMaxManaBase >> 6) / 10;
		break;
	case IPL_FIRERESCLVL:
		item[i]._iPLFR = 30 - plr[myplr]._pLevel;
		if(item[i]._iPLFR < 0) {
			item[i]._iPLFR = 0;
		}
		break;
	}

	if(item[i]._iVAdd1 == 0 && item[i]._iVMult1 == 0) {
		item[i]._iVAdd1 = PLVal(r, param1, param2, minval, maxval);
		item[i]._iVMult1 = multval;
	} else {
		item[i]._iVAdd2 = PLVal(r, param1, param2, minval, maxval);
		item[i]._iVMult2 = multval;
	}
}

void GetItemPower(int i, int minlvl, int maxlvl, long flgs, BOOL onlygood)
{
	int pre, post, nl, j, preidx, sufidx, idata;
	BYTE goe;
	int l[256];
	char istr[128];

	pre = random(23, 4);
	post = random(23, 3);
	if(pre != 0 && post == 0) {
		if(random(23, 2) != 0) {
			post = 1;
		} else {
			pre = 0;
		}
	}

	preidx = -1;
	sufidx = -1;
	goe = 0;

	if(!onlygood && random(0, 3) != 0) {
		onlygood = TRUE;
	}

	if(pre == 0) {
		nl = 0;
		for(j = 0; PL_Prefix[j].PLPower != -1; j++) {
			if(PL_Prefix[j].PLIType & flgs && PL_Prefix[j].PLMinLvl >= minlvl && PL_Prefix[j].PLMinLvl <= maxlvl) {
				if(!onlygood || PL_Prefix[j].PLOk) {
					if(flgs != PLT_STAFF || PL_Prefix[j].PLPower != IPL_CHARGES) {
						l[nl++] = j;
						if(PL_Prefix[j].PLDouble) {
							l[nl++] = j;
						}
					}
				}
			}
		}
		if(nl != 0) {
			preidx = l[random(23, nl)];
			sprintf(istr, "%s %s", PL_Prefix[preidx].PLName, item[i]._iIName);
			strcpy(item[i]._iIName, istr);
			item[i]._iMagical = 1;
			SaveItemPower(
				i,
				PL_Prefix[preidx].PLPower,
				PL_Prefix[preidx].PLParam1,
				PL_Prefix[preidx].PLParam2,
				PL_Prefix[preidx].PLMinVal,
				PL_Prefix[preidx].PLMaxVal,
				PL_Prefix[preidx].PLMultVal);
			item[i]._iPrePower = PL_Prefix[preidx].PLPower;
			goe = PL_Prefix[preidx].PLGOE;
		}
	}
	if(post != 0) {
		nl = 0;
		for(j = 0; PL_Suffix[j].PLPower != -1; j++) {
			if(PL_Suffix[j].PLIType & flgs && PL_Suffix[j].PLMinLvl >= minlvl && PL_Suffix[j].PLMinLvl <= maxlvl) {
				if((goe | PL_Suffix[j].PLGOE) != 0x11) {
					if(!onlygood || PL_Suffix[j].PLOk) {
						l[nl++] = j;
					}
				}
			}
		}
		if(nl != 0) {
			sufidx = l[random(23, nl)];
			sprintf(istr, "%s of %s", item[i]._iIName, PL_Suffix[sufidx].PLName);
			strcpy(item[i]._iIName, istr);
			item[i]._iMagical = 1;
			SaveItemPower(
				i,
				PL_Suffix[sufidx].PLPower,
				PL_Suffix[sufidx].PLParam1,
				PL_Suffix[sufidx].PLParam2,
				PL_Suffix[sufidx].PLMinVal,
				PL_Suffix[sufidx].PLMaxVal,
				PL_Suffix[sufidx].PLMultVal);
			item[i]._iSufPower = PL_Suffix[sufidx].PLPower;
		}
	}

	if(!control_WriteStringToBuffer(item[i]._iIName)) {
		idata = item[i].IDidx;
		strcpy(item[i]._iIName, AllItemsList[idata].iSName);
		if(preidx != -1) {
			sprintf(istr, "%s %s", PL_Prefix[preidx].PLName, item[i]._iIName);
			strcpy(item[i]._iIName, istr);
		}
		if(sufidx != -1) {
			sprintf(istr, "%s of %s", item[i]._iIName, PL_Suffix[sufidx].PLName);
			strcpy(item[i]._iIName, istr);
		}
	}

	if(preidx != -1 || sufidx != -1) {
		CalcItemValue(i);
	}
}

void GetItemBonus(int i, int idata, int minlvl, int maxlvl, BOOL onlygood)
{
	if(item[i]._iClass == ICLASS_GOLD) {
		return;
	}

	if(minlvl > 25) {
		minlvl = 25;
	}

	switch(item[i]._itype) {
	case ITYPE_SWORD:
	case ITYPE_AXE:
	case ITYPE_MACE:
		GetItemPower(i, minlvl, maxlvl, 0x1000, onlygood);
		break;
	case ITYPE_BOW:
		GetItemPower(i, minlvl, maxlvl, 0x10, onlygood);
		break;
	case ITYPE_SHIELD:
		GetItemPower(i, minlvl, maxlvl, 0x10000, onlygood);
		break;
	case ITYPE_LARMOR:
	case ITYPE_HELM:
	case ITYPE_MARMOR:
	case ITYPE_HARMOR:
		GetItemPower(i, minlvl, maxlvl, 0x100000, onlygood);
		break;
	case ITYPE_STAFF:
		GetStaffSpell(i, maxlvl, onlygood);
		break;
	case ITYPE_RING:
	case ITYPE_AMULET:
		GetItemPower(i, minlvl, maxlvl, 1, onlygood);
		break;
	}
}

void SetupItem(int i)
{
	int it;

	it = ItemCAnimTbl[item[i]._iCurs];
	item[i]._iAnimData = itemanims[it];
	item[i]._iAnimLen = ItemAnimLs[it];
	item[i]._iAnimWidth = 96;
	item[i]._iAnimWidth2 = 16;
	item[i]._iIdentified = 0;
	item[i]._iPostDraw = 0;

	if(plr[myplr].pLvlLoad == 0) {
		item[i]._iAnimFrame = 1;
		item[i]._iAnimFlag = 1;
		item[i]._iSelFlag = 0;
	} else {
		item[i]._iAnimFrame = item[i]._iAnimLen;
		item[i]._iAnimFlag = 0;
		item[i]._iSelFlag = 1;
	}
}

int RndItem(int m)
{
	int i, ri, rv;
	int ril[512];

	if(monster[m].MData->mTreasure & 0x8000) {
		return -((monster[m].MData->mTreasure & 0xFFF) + 1);
	}
	if(monster[m].MData->mTreasure & 0x4000) {
		return 0;
	}

	rv = random(24, 100);
#ifdef _DEBUG
	if(debug_mode_key_d) {
		rv = 100;
	}
#endif
#ifdef _DEBUG
	if(!debug_mode_key_inverted_v && rv > 40) {
#else
	if(rv > 40) {
#endif
		return 0;
	}
#ifdef _DEBUG
	if(!debug_mode_key_inverted_v && random(24, 100) > 25) {
#else
	if(random(24, 100) > 25) {
#endif
		return IDI_GOLD + 1;
	}

	ri = 0;
	for(i = 0; AllItemsList[i].iLoc != -1; i++) {
		if(AllItemsList[i].iRnd == IDROP_DOUBLE && monster[m].mLevel >= AllItemsList[i].iMinMLvl) {
			ril[ri++] = i;
		}
		if(AllItemsList[i].iRnd != IDROP_NEVER && monster[m].mLevel >= AllItemsList[i].iMinMLvl) {
			ril[ri++] = i;
		}
		if(AllItemsList[i].iSpell == SPL_RESURRECT && gbMaxPlayers == 1) {
			ri--;
		}
		if(AllItemsList[i].iSpell == SPL_HEALOTHER && gbMaxPlayers == 1) {
			ri--;
		}
	}

	rv = random(24, ri);
	return ril[rv] + 1;
}

int RndUItem(int m)
{
	int i, ri;
	int ril[512];
	BOOL okflag;

	if(m != -1 && monster[m].MData->mTreasure & 0x8000 && gbMaxPlayers == 1) {
		return -((monster[m].MData->mTreasure & 0xFFF) + 1);
	}

	ri = 0;
	for(i = 0; AllItemsList[i].iLoc != -1; i++) {
		okflag = TRUE;
		if(AllItemsList[i].iRnd == IDROP_NEVER) {
			okflag = FALSE;
		}
		if(m != -1) {
			if(monster[m].mLevel < AllItemsList[i].iMinMLvl) {
				okflag = FALSE;
			}
		} else {
			if(currlevel << 1 < AllItemsList[i].iMinMLvl) {
				okflag = FALSE;
			}
		}
		if(AllItemsList[i].itype == ITYPE_MISC) {
			okflag = FALSE;
		}
		if(AllItemsList[i].itype == ITYPE_GOLD) {
			okflag = FALSE;
		}
		if(AllItemsList[i].itype == ITYPE_MEAT) {
			okflag = FALSE;
		}
		if(AllItemsList[i].iMiscId == IMISC_BOOK) {
			okflag = TRUE;
		}
		if(AllItemsList[i].iSpell == SPL_RESURRECT && gbMaxPlayers == 1) {
			okflag = FALSE;
		}
		if(AllItemsList[i].iSpell == SPL_HEALOTHER && gbMaxPlayers == 1) {
			okflag = FALSE;
		}
		if(okflag) {
			ril[ri++] = i;
		}
	}

	return ril[random(25, ri)];
}

int RndAllItems()
{
	int i, ri, rv;
	int ril[512];

#ifdef _DEBUG
	if(!debug_mode_key_inverted_v && random(26, 100) > 25) {
		return 0;
	}
#else
	if(random(26, 100) > 25) {
		return 0;
	}
#endif

	ri = 0;
	for(i = 0; AllItemsList[i].iLoc != -1; i++) {
		if(AllItemsList[i].iRnd != IDROP_NEVER && currlevel << 1 >= AllItemsList[i].iMinMLvl) {
			ril[ri++] = i;
		}
		if(AllItemsList[i].iSpell == SPL_RESURRECT && gbMaxPlayers == 1) {
			ri--;
		}
		if(AllItemsList[i].iSpell == SPL_HEALOTHER && gbMaxPlayers == 1) {
			ri--;
		}
	}

	rv = random(26, ri);
	return ril[rv];
}

int RndTypeItems(int itype, int imid)
{
	int i, ri;
	int ril[512];
	BOOL okflag;

	ri = 0;
	for(i = 0; AllItemsList[i].iLoc != -1; i++) {
		okflag = TRUE;
		if(AllItemsList[i].iRnd == IDROP_NEVER) {
			okflag = FALSE;
		}
		if(currlevel << 1 < AllItemsList[i].iMinMLvl) {
			okflag = FALSE;
		}
		if(AllItemsList[i].itype != itype) {
			okflag = FALSE;
		}
		if(imid != -1 && AllItemsList[i].iMiscId != imid) {
			okflag = FALSE;
		}
		if(okflag) {
			ril[ri++] = i;
		}
	}

	return ril[random(27, ri)];
}

int CheckUnique(int i, int lvl, int uper, BOOL recreate)
{
	int j, idata, numu, rv;
	BOOLEAN uok[128];

#ifdef _DEBUG
	if((!debug_mode_key_d || !debug_mode_key_inverted_v) && random(28, 100) > uper) {
#else
	if(random(28, 100) > uper) {
#endif
		return -1;
	}

	numu = 0;
	memset(uok, 0, sizeof(uok));

	for(j = 0; UniqueItemList[j].UIItemId != -1; j++) {
		idata = item[i].IDidx;
		if(UniqueItemList[j].UIItemId != AllItemsList[idata].iItemId) {
			continue;
		}
		if(lvl < UniqueItemList[j].UIMinLvl) {
			continue;
		}
		if(recreate || !UniqueItemFlag[j] || gbMaxPlayers != 1) {
			uok[j] = TRUE;
			numu++;
		}
	}

	if(numu == 0) {
		return -1;
	}

	rv = random(29, 10); /// BUGFIX: unused, last unique in array is always chosen
	j = 0;

	while(numu > 0) {
		if(uok[j]) {
			numu--;
		}
		if(numu > 0) {
			j++;
			if(j == sizeof(uok)) {
				j = 0;
			}
		}
	}

	return j;
}

void GetUniqueItem(int i, int uid)
{
	UniqueItemFlag[uid] = 1;
	SaveItemPower(i, UniqueItemList[uid].UIPower1, UniqueItemList[uid].UIParam1, UniqueItemList[uid].UIParam2, 0, 0, 1);

	if(UniqueItemList[uid].UINumPL > 1) {
		SaveItemPower(i, UniqueItemList[uid].UIPower2, UniqueItemList[uid].UIParam3, UniqueItemList[uid].UIParam4, 0, 0, 1);
	}
	if(UniqueItemList[uid].UINumPL > 2) {
		SaveItemPower(i, UniqueItemList[uid].UIPower3, UniqueItemList[uid].UIParam5, UniqueItemList[uid].UIParam6, 0, 0, 1);
	}
	if(UniqueItemList[uid].UINumPL > 3) {
		SaveItemPower(i, UniqueItemList[uid].UIPower4, UniqueItemList[uid].UIParam7, UniqueItemList[uid].UIParam8, 0, 0, 1);
	}
	if(UniqueItemList[uid].UINumPL > 4) {
		SaveItemPower(i, UniqueItemList[uid].UIPower5, UniqueItemList[uid].UIParam9, UniqueItemList[uid].UIParam10, 0, 0, 1);
	}
	if(UniqueItemList[uid].UINumPL > 5) {
		SaveItemPower(i, UniqueItemList[uid].UIPower6, UniqueItemList[uid].UIParam11, UniqueItemList[uid].UIParam12, 0, 0, 1);
	}

	strcpy(item[i]._iIName, UniqueItemList[uid].UIName);
	item[i]._iIvalue = UniqueItemList[uid].UIValue;
	if(item[i]._iMiscId == IMISC_UNIQUE) {
		item[i]._iSeed = uid;
	}
	item[i]._iUid = uid;
	item[i]._iMagical = 2;
	item[i]._iCreateInfo |= 0x0200;
}

void SpawnUnique(int uid, int x, int y)
{
	int ii, itype;

	if(numitems >= MAXITEMS) {
		return;
	}

	ii = itemavail[0];
	GetSuperItemSpace(x, y, ii);
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;

	itype = 0;
	while(AllItemsList[itype].iItemId != UniqueItemList[uid].UIItemId) {
		itype++;
	}

	GetItemAttrs(ii, itype, currlevel);
	GetUniqueItem(ii, uid);
	SetupItem(ii);
	numitems++;
}

void ItemRndDur(int ii)
{
	if(item[ii]._iDurability == 0 || item[ii]._iDurability == 255) {
		return;
	}

	item[ii]._iDurability = random(0, item[ii]._iMaxDur >> 1) + (item[ii]._iMaxDur >> 2) + 1;
}

void SetupAllItems(int ii, int idx, int iseed, int lvl, int uper, BOOL onlygood, BOOL recreate, BOOL pregen)
{
	int iblvl, uid;

	item[ii]._iSeed = iseed;
	SetRndSeed(iseed);
	GetItemAttrs(ii, idx, lvl >> 1);
	item[ii]._iCreateInfo = lvl;

	if(pregen) {
		item[ii]._iCreateInfo |= 0x8000;
	}
	if(onlygood) {
		item[ii]._iCreateInfo |= 0x40;
	}

	if(uper == 15) {
		item[ii]._iCreateInfo |= 0x80;
	} else if(uper == 1) {
		item[ii]._iCreateInfo |= 0x0100;
	}

	if(item[ii]._iMiscId != IMISC_UNIQUE) {
		iblvl = -1;
		if(random(32, 100) <= 10) {
			iblvl = lvl;
		} else if(random(33, 100) <= lvl) {
			iblvl = lvl;
		}
		if(iblvl == -1 && item[ii]._iMiscId == IMISC_STAFF) {
			iblvl = lvl;
		}
		if(iblvl == -1 && item[ii]._iMiscId == IMISC_RING) {
			iblvl = lvl;
		}
		if(iblvl == -1 && item[ii]._iMiscId == IMISC_AMULET) {
			iblvl = lvl;
		}
		if(onlygood) {
			iblvl = lvl;
		}
#ifdef _DEBUG
		if(debug_mode_key_inverted_v) {
			iblvl = lvl;
		}
#endif
		if(uper == 15) {
			iblvl = lvl + 4;
		}
		if(iblvl != -1) {
			uid = CheckUnique(ii, iblvl, uper, recreate);
			if(uid == -1) {
				GetItemBonus(ii, idx, iblvl >> 1, iblvl, onlygood);
			} else {
				GetUniqueItem(ii, uid);
				item[ii]._iCreateInfo |= 0x0200;
			}
		}
		if(item[ii]._iMagical != 2) {
			ItemRndDur(ii);
		}
	} else if(item[ii]._iLoc != ILOC_UNEQUIPABLE) {
		GetUniqueItem(ii, iseed);
	}

	SetupItem(ii);
}

void SpawnItem(int m, int x, int y, BOOL sendmsg)
{
	int ii, idx;
	BOOL onlygood;

	if(monster[m]._uniqtype != 0 || (monster[m].MData->mTreasure & 0x8000 && gbMaxPlayers != 1)) {
		idx = RndUItem(m);
		if(idx < 0) {
			SpawnUnique(-(idx + 1), x, y);
			return;
		}
		onlygood = TRUE;
	} else if(quests[Q_MUSHROOM]._qactive == 2 && quests[Q_MUSHROOM]._qvar1 == 5) { // QS_MUSHGIVEN
		idx = IDI_BRAIN;
		quests[Q_MUSHROOM]._qvar1 = 6; // QS_BRAINSPAWNED
	} else {
		idx = RndItem(m);
		if(idx == 0) {
			return;
		}
		if(idx > 0) {
			idx--;
			onlygood = FALSE;
		} else {
			SpawnUnique(-(idx + 1), x, y);
			return;
		}
	}

	if(numitems >= MAXITEMS) {
		return;
	}

	ii = itemavail[0];
	GetSuperItemSpace(x, y, ii);
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;

	if(monster[m]._uniqtype != 0) {
		SetupAllItems(ii, idx, GetRndSeed(), monster[m].MData->mLevel, 15, onlygood, FALSE, FALSE);
	} else {
		SetupAllItems(ii, idx, GetRndSeed(), monster[m].MData->mLevel, 1, onlygood, FALSE, FALSE);
	}

	numitems++;

	if(sendmsg) {
		NetSendCmdDItem(FALSE, ii);
	}
}

void CreateItem(int uid, int x, int y)
{
	int ii, idx;

	if(numitems >= MAXITEMS) {
		return;
	}

	ii = itemavail[0];
	GetSuperItemSpace(x, y, ii);
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;

	idx = 0;
	while(AllItemsList[idx].iItemId != UniqueItemList[uid].UIItemId) {
		idx++;
	}

	GetItemAttrs(ii, idx, currlevel);
	GetUniqueItem(ii, uid);
	SetupItem(ii);
	item[ii]._iMagical = 2;
	numitems++;
}

void CreateRndItem(int x, int y, BOOL onlygood, BOOL sendmsg, BOOL delta)
{
	int ii, idx;

	if(onlygood) {
		idx = RndUItem(-1);
	} else {
		idx = RndAllItems();
	}

	if(numitems >= MAXITEMS) {
		return;
	}

	ii = itemavail[0];
	GetSuperItemSpace(x, y, ii);
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;
	SetupAllItems(ii, idx, GetRndSeed(), currlevel << 1, 1, onlygood, FALSE, delta);

	if(sendmsg) {
		NetSendCmdDItem(FALSE, ii);
	}
	if(delta) {
		DeltaAddItem(ii);
	}

	numitems++;
}

void SetupAllUseful(int ii, int iseed, int lvl)
{
	int idx;

	item[ii]._iSeed = iseed;
	SetRndSeed(iseed);

	if(random(34, 2) != 0) {
		idx = IDI_HEAL;
	} else {
		idx = IDI_MANA;
	}
	if(lvl > 1 && random(34, 3) == 0) {
		idx = IDI_PORTAL;
	}

	GetItemAttrs(ii, idx, lvl);
	item[ii]._iCreateInfo = lvl + 0x180;
	SetupItem(ii);
}

void CreateRndUseful(int pnum, int x, int y, BOOL sendmsg)
{
	int ii;

	if(numitems >= MAXITEMS) {
		return;
	}

	ii = itemavail[0];
	GetSuperItemSpace(x, y, ii);
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;
	SetupAllUseful(ii, GetRndSeed(), currlevel);

	if(sendmsg) {
		NetSendCmdDItem(FALSE, ii);
	}

	numitems++;
}

void CreateTypeItem(int x, int y, BOOL onlygood, int itype, int imisc, BOOL sendmsg, BOOL delta)
{
	int ii, idx;

	if(itype != ITYPE_GOLD) {
		idx = RndTypeItems(itype, imisc);
	} else {
		idx = IDI_GOLD;
	}

	if(numitems >= MAXITEMS) {
		return;
	}

	ii = itemavail[0];
	GetSuperItemSpace(x, y, ii);
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;
	SetupAllItems(ii, idx, GetRndSeed(), currlevel << 1, 1, onlygood, FALSE, delta);

	if(sendmsg) {
		NetSendCmdDItem(FALSE, ii);
	}
	if(delta) {
		DeltaAddItem(ii);
	}

	numitems++;
}

void RecreateItem(int ii, int idx, WORD icreateinfo, int iseed, int ivalue)
{
	int uper;
	BOOL onlygood, uavail, pregen;

	if(idx == IDI_GOLD) {
		SetPlrHandItem(&item[ii], idx);
		item[ii]._iSeed = iseed;
		item[ii]._iCreateInfo = icreateinfo;
		item[ii]._ivalue = ivalue;
		if(ivalue >= 2500) {
			item[ii]._iCurs = 6;
		} else if(ivalue <= 1000) {
			item[ii]._iCurs = 4;
		} else {
			item[ii]._iCurs = 5;
		}
	} else {
		if(icreateinfo == 0) {
			SetPlrHandItem(&item[ii], idx);
			SetPlrHandSeed(&item[ii], iseed);
		} else if(icreateinfo & 0x7C00) {
			RecreateTownItem(ii, idx, icreateinfo, iseed, ivalue);
		} else if((icreateinfo & 0x0180) == 0x0180) {
			SetupAllUseful(ii, iseed, icreateinfo & 0x3F);
		} else {
			uper = 0;
			onlygood = FALSE;
			uavail = FALSE;
			pregen = FALSE;
			if(icreateinfo & 0x0100) {
				uper = 1;
			}
			if(icreateinfo & 0x80) {
				uper = 15;
			}
			if(icreateinfo & 0x40) {
				onlygood = TRUE;
			}
			if(icreateinfo & 0x0200) {
				uavail = TRUE;
			}
			if(icreateinfo & 0x8000) {
				pregen = TRUE;
			}
			SetupAllItems(ii, idx, iseed, icreateinfo & 0x3F, uper, onlygood, uavail, pregen);
		}
	}
}

void RecreateEar(int ii, WORD ic, int iseed, BOOL Id, int dur, int mdur, int ch, int mch, int ivalue, int ibuff)
{
	SetPlrHandItem(&item[ii], IDI_EAR);
	tempstr[0] = (ic >> 8) & 0x7F;
	tempstr[1] = ic & 0x7F;
	tempstr[2] = (iseed >> 24) & 0x7F;
	tempstr[3] = (iseed >> 16) & 0x7F;
	tempstr[4] = (iseed >> 8) & 0x7F;
	tempstr[5] = iseed & 0x7F;
	tempstr[6] = Id & 0x7F;
	tempstr[7] = dur & 0x7F;
	tempstr[8] = mdur & 0x7F;
	tempstr[9] = ch & 0x7F;
	tempstr[10] = mch & 0x7F;
	tempstr[11] = (ivalue >> 8) & 0x7F;
	tempstr[12] = (ibuff >> 24) & 0x7F;
	tempstr[13] = (ibuff >> 16) & 0x7F;
	tempstr[14] = (ibuff >> 8) & 0x7F;
	tempstr[15] = ibuff & 0x7F;
	tempstr[16] = '\0';
	sprintf(item[ii]._iName, "Ear of %s", tempstr);
	item[ii]._iCurs = ((ivalue >> 6) & 3) + 19;
	item[ii]._ivalue = ivalue & 0x3F;
	item[ii]._iCreateInfo = ic;
	item[ii]._iSeed = iseed;
}

void SpawnQuestItem(int itemid, int x, int y, int randarea, int selflag)
{
	int i, j, tries;
	BOOL failed;

	if(randarea != 0) {
		tries = 0;
		while(1) {
			tries++;
			if(tries > 1000 && randarea > 1) {
				randarea--;
			}
			x = random(0, MAXDUNX);
			y = random(0, MAXDUNY);
			failed = FALSE;
			for(i = 0; i < randarea && !failed; i++) {
				for(j = 0; j < randarea && !failed; j++) {
					failed = !ItemSpaceOk(i + x, j + y);
				}
			}
			if(!failed) {
				break;
			}
		}
	}

	if(numitems >= MAXITEMS) {
		return;
	}

	i = itemavail[0];
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = i;
	item[i]._ix = x;
	item[i]._iy = y;
	dItem[x][y] = i + 1;
	GetItemAttrs(i, itemid, currlevel);
	SetupItem(i);
	item[i]._iPostDraw = 1;

	if(selflag != 0) {
		item[i]._iSelFlag = selflag;
		item[i]._iAnimFrame = item[i]._iAnimLen;
		item[i]._iAnimFlag = 0;
	}

	numitems++;
}

void SpawnRock()
{
	int i, ii, ostand, xx, yy;
	BOOL done;

	done = FALSE;
	for(i = 0; i < nobjects && !done; i++) {
		ostand = objectactive[i];
		done = object[ostand]._otype == OBJ_STAND;
	}
	if(done) {
		ii = itemavail[0];
		itemavail[0] = itemavail[MAXITEMS - numitems - 1];
		itemactive[numitems] = ii;
		item[ii]._ix = object[ostand]._ox;
		xx = item[ii]._ix;
		item[ii]._iy = object[ostand]._oy;
		yy = item[ii]._iy;
		dItem[xx][yy] = ii + 1;
		GetItemAttrs(ii, IDI_ROCK, currlevel);
		SetupItem(ii);
		item[ii]._iSelFlag = 2;
		item[ii]._iPostDraw = 1;
		item[ii]._iAnimFrame = 11;
		numitems++;
	}
}

void RespawnItem(int i, BOOL FlipFlag)
{
	int it;

	it = ItemCAnimTbl[item[i]._iCurs];
	item[i]._iAnimData = itemanims[it];
	item[i]._iAnimLen = ItemAnimLs[it];
	item[i]._iAnimWidth = 96;
	item[i]._iAnimWidth2 = 16;
	item[i]._iPostDraw = 0;
	item[i]._iRequest = 0;

	if(FlipFlag) {
		item[i]._iAnimFrame = 1;
		item[i]._iAnimFlag = 1;
		item[i]._iSelFlag = 0;
	} else {
		item[i]._iAnimFrame = item[i]._iAnimLen;
		item[i]._iAnimFlag = 0;
		item[i]._iSelFlag = 1;
	}

	if(item[i]._iCurs == 76) { // Magic Rock
		item[i]._iSelFlag = 1;
		PlaySfxLoc(ItemDropSnds[it], item[i]._ix, item[i]._iy);
	}
	if(item[i]._iCurs == 126) { // Tavern Sign
		item[i]._iSelFlag = 1;
	}
	if(item[i]._iCurs == 140) { // Anvil of Fury
		item[i]._iSelFlag = 1;
	}
}

void DeleteItem(int ii, int i)
{
	itemavail[MAXITEMS - numitems] = ii;
	numitems--;

	if(numitems > 0 && i != numitems) {
		itemactive[i] = itemactive[numitems];
	}
}

void ItemDoppel()
{
	int idoppelx;
	ItemStruct *i;

	if(gbMaxPlayers == 1) {
		return;
	}

	for(idoppelx = 16; idoppelx < 96; idoppelx++) {
		if(dItem[idoppelx][idoppely] != 0) {
			i = &item[dItem[idoppelx][idoppely] - 1];
			if(i->_ix != idoppelx || i->_iy != idoppely) {
				dItem[idoppelx][idoppely] = 0;
			}
		}
	}

	idoppely++;
	if(idoppely == 96) {
		idoppely = 16;
	}
}

void ProcessItems()
{
	int i, ii, it;

	for(i = 0; i < numitems; i++) {
		ii = itemactive[i];
		if(item[ii]._iAnimFlag) {
			item[ii]._iAnimFrame++;
			if(item[ii]._iCurs == 76) { // Magic Rock
				if(item[ii]._iSelFlag == 1 && item[ii]._iAnimFrame == 11) {
					item[ii]._iAnimFrame = 1;
				}
				if(item[ii]._iSelFlag == 2 && item[ii]._iAnimFrame == 21) {
					item[ii]._iAnimFrame = 11;
				}
			} else {
				if(item[ii]._iAnimFrame == item[ii]._iAnimLen >> 1) {
					it = ItemCAnimTbl[item[ii]._iCurs];
					PlaySfxLoc(ItemDropSnds[it], item[ii]._ix, item[ii]._iy);
				}
				if(item[ii]._iAnimFrame >= item[ii]._iAnimLen) {
					item[ii]._iAnimFrame = item[ii]._iAnimLen;
					item[ii]._iAnimFlag = 0;
					item[ii]._iSelFlag = 1;
				}
			}
		}
	}

	ItemDoppel();
}

void FreeItemGFX()
{
	int i;

	for(i = 0; i < 35; i++) {
		MemFreeDbg(itemanims[i]);
	}
}

void GetItemFrm(int i)
{
	int it;

	it = ItemCAnimTbl[item[i]._iCurs];
	item[i]._iAnimData = itemanims[it];
}

void GetItemStr(int i)
{
	int nGold;

	if(item[i]._itype != ITYPE_GOLD) {
		if(item[i]._iIdentified) {
			strcpy(infostr, item[i]._iIName);
		} else {
			strcpy(infostr, item[i]._iName);
		}
		if(item[i]._iMagical == 1) {
			infoclr = COL_BLUE;
		}
		if(item[i]._iMagical == 2) {
			infoclr = COL_GOLD;
		}
	} else {
		nGold = item[i]._ivalue;
		sprintf(infostr, "%i gold %s", nGold, get_pieces_str(nGold));
	}
}

void CheckIdentify(int pnum, int cii)
{
	ItemStruct *pi;

	if(cii >= 7) {
		pi = &plr[pnum].InvList[cii - 7];
	} else {
		pi = &plr[pnum].InvBody[cii];
	}

	pi->_iIdentified = 1;
	CalcPlrInv(pnum, TRUE);

	if(pnum == myplr) {
		SetCursor_(CURSOR_HAND);
	}
}

void DoRepair(int pnum, int cii)
{
	PlayerStruct *p;
	ItemStruct *pi;

	p = &plr[pnum];
	PlaySfxLoc(IS_REPAIR, p->WorldX, p->WorldY);

	if(cii >= 7) {
		pi = &p->InvList[cii - 7];
	} else {
		pi = &p->InvBody[cii];
	}

	RepairItem(pi, p->_pLevel);
	CalcPlrInv(pnum, TRUE);

	if(pnum == myplr) {
		SetCursor_(CURSOR_HAND);
	}
}

void RepairItem(ItemStruct *i, int lvl)
{
	int d, rep;

	if(i->_iDurability == i->_iMaxDur) {
		return;
	}
	if(i->_iMaxDur <= 0) {
		i->_itype = -1;
		return;
	}

	rep = 0;
	while(1) {
		rep += random(37, lvl) + lvl;
		d = i->_iMaxDur / (lvl + 9);
		if(d < 1) {
			d = 1;
		}
		i->_iMaxDur -= d;
		if(i->_iMaxDur == 0) {
			i->_itype = -1;
			return;
		}
		if(rep + i->_iDurability >= i->_iMaxDur) {
			break;
		}
	}

	i->_iDurability += rep;
	if(i->_iDurability > i->_iMaxDur) {
		i->_iDurability = i->_iMaxDur;
	}
}

void DoRecharge(int pnum, int cii)
{
	int r, sn;
	PlayerStruct *p;
	ItemStruct *pi;

	p = &plr[pnum];
	if(cii >= 7) {
		pi = &p->InvList[cii - 7];
	} else {
		pi = &p->InvBody[cii];
	}

	if(pi->_itype == ITYPE_STAFF && pi->_iSpell != SPL_NULL) {
		sn = pi->_iSpell;
		r = spelldata[sn].sBookLvl;
		r = random(38, p->_pLevel / r) + 1;
		RechargeItem(pi, r);
		CalcPlrInv(pnum, TRUE);
	}

	if(pnum == myplr) {
		SetCursor_(CURSOR_HAND);
	}
}

void RechargeItem(ItemStruct *i, int r)
{
	if(i->_iCharges == i->_iMaxCharges) {
		return;
	}

	while(1) {
		i->_iMaxCharges--;
		if(i->_iMaxCharges == 0) {
			return;
		}
		i->_iCharges += r;
		if(i->_iCharges >= i->_iMaxCharges) {
			break;
		}
	}

	if(i->_iCharges > i->_iMaxCharges) {
		i->_iCharges = i->_iMaxCharges;
	}
}

void PrintItemOil(char IDidx)
{
	switch(IDidx) {
	case IMISC_FULLHEAL:
		strcpy(tempstr, "fully recover life");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_HEAL:
		strcpy(tempstr, "recover partial life");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_OLDHEAL:
		strcpy(tempstr, "recover life");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_DEADHEAL:
		strcpy(tempstr, "deadly heal");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_MANA:
		strcpy(tempstr, "recover mana");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_FULLMANA:
		strcpy(tempstr, "fully recover mana");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_ELIXSTR:
		strcpy(tempstr, "increase strength");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_ELIXMAG:
		strcpy(tempstr, "increase magic");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_ELIXDEX:
		strcpy(tempstr, "increase dexterity");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_ELIXVIT:
		strcpy(tempstr, "increase vitality");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_ELIXWEAK:
		strcpy(tempstr, "decrease strength");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_ELIXDIS:
		strcpy(tempstr, "decrease strength"); /// BUGFIX: "decrease magic"
		AddPanelString(tempstr, 1);
		break;
	case IMISC_ELIXCLUM:
		strcpy(tempstr, "decrease dexterity");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_ELIXSICK:
		strcpy(tempstr, "decrease vitality");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_REJUV:
		strcpy(tempstr, "recover life and mana");
		AddPanelString(tempstr, 1);
		break;
	case IMISC_FULLREJUV:
		strcpy(tempstr, "fully recover life and mana");
		AddPanelString(tempstr, 1);
		break;
	}
}

void PrintItemPower(char plidx, const ItemStruct *x)
{
	int v;

	switch(plidx) {
	case IPL_TOHIT:
	case IPL_TOHIT_CURSE:
		sprintf(tempstr, "chance to hit : %+i%%", x->_iPLToHit);
		break;
	case IPL_DAMP:
	case IPL_DAMP_CURSE:
		sprintf(tempstr, "%+i%% damage", x->_iPLDam);
		break;
	case IPL_TOHIT_DAMP:
	case IPL_TOHIT_DAMP_CURSE:
		sprintf(tempstr, "to hit: %+i%%, %+i%% damage", x->_iPLToHit, x->_iPLDam);
		break;
	case IPL_ACP:
	case IPL_ACP_CURSE:
		sprintf(tempstr, "%+i%% armor", x->_iPLAC);
		break;
	case IPL_SETAC:
		sprintf(tempstr, "armor class: %i", x->_iAC);
		break;
	case IPL_AC_CURSE:
		sprintf(tempstr, "armor class: %i", x->_iAC);
		break;
	case IPL_FIRERES:
		if(x->_iPLFR < 75) {
			sprintf(tempstr, "Resist Fire : %+i%%", x->_iPLFR);
		}
		if(x->_iPLFR >= 75) {
			sprintf(tempstr, "Resist Fire : 75%% MAX");
		}
		break;
	case IPL_LIGHTRES:
		if(x->_iPLLR < 75) {
			sprintf(tempstr, "Resist Lightning : %+i%%", x->_iPLLR);
		}
		if(x->_iPLLR >= 75) {
			sprintf(tempstr, "Resist Lightning : 75%% MAX");
		}
		break;
	case IPL_MAGICRES:
		if(x->_iPLMR < 75) {
			sprintf(tempstr, "Resist Magic : %+i%%", x->_iPLMR);
		}
		if(x->_iPLMR >= 75) {
			sprintf(tempstr, "Resist Magic : 75%% MAX");
		}
		break;
	case IPL_ALLRES:
		if(x->_iPLFR < 75) {
			sprintf(tempstr, "Resist All : %+i%%", x->_iPLFR);
		}
		if(x->_iPLFR >= 75) {
			sprintf(tempstr, "Resist All : 75%% MAX");
		}
		break;
	case IPL_SPLLVLADD:
		if(x->_iSplLvlAdd == 1) {
			strcpy(tempstr, "spells are increased 1 level");
		}
		if(x->_iSplLvlAdd == 2) {
			strcpy(tempstr, "spells are increased 2 levels");
		}
		if(x->_iSplLvlAdd < 1) {
			strcpy(tempstr, "spells are decreased 1 level");
		}
		break;
	case IPL_CHARGES:
		strcpy(tempstr, "Extra charges");
		break;
	case IPL_SPELL:
		sprintf(tempstr, "%i %s charges", x->_iMaxCharges, spelldata[x->_iSpell].sNameText);
		break;
	case IPL_FIREDAM:
		sprintf(tempstr, "Fire hit damage: %i-%i", x->_iFMinDam, x->_iFMaxDam);
		break;
	case IPL_LIGHTDAM:
		sprintf(tempstr, "Lightning hit damage: %i-%i", x->_iLMinDam, x->_iLMaxDam);
		break;
	case IPL_STR:
	case IPL_STR_CURSE:
		sprintf(tempstr, "%+i to strength", x->_iPLStr);
		break;
	case IPL_MAG:
	case IPL_MAG_CURSE:
		sprintf(tempstr, "%+i to magic", x->_iPLMag);
		break;
	case IPL_DEX:
	case IPL_DEX_CURSE:
		sprintf(tempstr, "%+i to dexterity", x->_iPLDex);
		break;
	case IPL_VIT:
	case IPL_VIT_CURSE:
		sprintf(tempstr, "%+i to vitality", x->_iPLVit);
		break;
	case IPL_ATTRIBS:
	case IPL_ATTRIBS_CURSE:
		sprintf(tempstr, "%+i to all attributes", x->_iPLStr);
		break;
	case IPL_GETHIT_CURSE:
	case IPL_GETHIT:
		sprintf(tempstr, "%+i damage from enemies", x->_iPLGetHit);
		break;
	case IPL_LIFE:
	case IPL_LIFE_CURSE:
		sprintf(tempstr, "Hit Points : %+i", x->_iPLHP >> 6);
		break;
	case IPL_MANA:
	case IPL_MANA_CURSE:
		sprintf(tempstr, "Mana : %+i", x->_iPLMana >> 6);
		break;
	case IPL_DUR:
		strcpy(tempstr, "high durability");
		break;
	case IPL_DUR_CURSE:
		strcpy(tempstr, "decreased durability");
		break;
	case IPL_INDESTRUCTIBLE:
		strcpy(tempstr, "indestructible");
		break;
	case IPL_LIGHT:
		v = 10 * x->_iPLLight;
		sprintf(tempstr, "+%i%% light radius", v);
		break;
	case IPL_LIGHT_CURSE:
		v = -10 * x->_iPLLight;
		sprintf(tempstr, "-%i%% light radius", v);
		break;
	case IPL_FIRE_ARROWS:
		sprintf(tempstr, "fire arrows damage: %i-%i", x->_iFMinDam, x->_iFMaxDam);
		break;
	case IPL_LIGHT_ARROWS:
		sprintf(tempstr, "lightning arrows damage %i-%i", x->_iLMinDam, x->_iLMaxDam);
		break;
	case IPL_THORNS:
		strcpy(tempstr, "attacker takes 1-3 damage");
		break;
	case IPL_NOMANA:
		strcpy(tempstr, "user loses all mana");
		break;
	case IPL_NOHEALPLR:
		strcpy(tempstr, "you can't heal");
		break;
	case IPL_ABSHALFTRAP:
		strcpy(tempstr, "absorbs half of trap damage");
		break;
	case IPL_KNOCKBACK:
		strcpy(tempstr, "knocks target back");
		break;
	case IPL_3XDAMVDEM:
		strcpy(tempstr, "+200% damage vs. demons");
		break;
	case IPL_ALLRESZERO:
		strcpy(tempstr, "All Resistance equals 0");
		break;
	case IPL_NOHEALMON:
		strcpy(tempstr, "hit monster doesn't heal");
		break;
	case IPL_STEALMANA:
		if(x->_iFlags & ISPL_STEALMANA_3) {
			strcpy(tempstr, "hit steals 3% mana");
		}
		if(x->_iFlags & ISPL_STEALMANA_5) {
			strcpy(tempstr, "hit steals 5% mana");
		}
		break;
	case IPL_STEALLIFE:
		if(x->_iFlags & ISPL_STEALLIFE_3) {
			strcpy(tempstr, "hit steals 3% life");
		}
		if(x->_iFlags & ISPL_STEALLIFE_5) {
			strcpy(tempstr, "hit steals 5% life");
		}
		break;
	case IPL_TARGAC:
		strcpy(tempstr, "damages target's armor");
		break;
	case IPL_FASTATTACK:
		if(x->_iFlags & ISPL_QUICKATTACK) {
			strcpy(tempstr, "quick attack");
		}
		if(x->_iFlags & ISPL_FASTATTACK) {
			strcpy(tempstr, "fast attack");
		}
		if(x->_iFlags & ISPL_FASTERATTACK) {
			strcpy(tempstr, "faster attack");
		}
		if(x->_iFlags & ISPL_FASTESTATTACK) {
			strcpy(tempstr, "fastest attack");
		}
		break;
	case IPL_FASTRECOVER:
		if(x->_iFlags & ISPL_FASTRECOVER) {
			strcpy(tempstr, "fast hit recovery");
		}
		if(x->_iFlags & ISPL_FASTERRECOVER) {
			strcpy(tempstr, "faster hit recovery");
		}
		if(x->_iFlags & ISPL_FASTESTRECOVER) {
			strcpy(tempstr, "fastest hit recovery");
		}
		break;
	case IPL_FASTBLOCK:
		strcpy(tempstr, "fast block");
		break;
	case IPL_DAMMOD:
		sprintf(tempstr, "adds %i points to damage", x->_iPLDamMod);
		break;
	case IPL_RNDARROWVEL:
		strcpy(tempstr, "fires random speed arrows");
		break;
	case IPL_SETDAM:
		sprintf(tempstr, "unusual item damage");
		break;
	case IPL_SETDUR:
		strcpy(tempstr, "altered durability");
		break;
	case IPL_FASTSWING:
		strcpy(tempstr, "Faster attack swing");
		break;
	case IPL_ONEHAND:
		strcpy(tempstr, "one handed sword");
		break;
	case IPL_DRAINLIFE:
		strcpy(tempstr, "constantly lose hit points");
		break;
	case IPL_RNDSTEALLIFE:
		strcpy(tempstr, "life stealing");
		break;
	case IPL_NOMINSTR:
		strcpy(tempstr, "no strength requirement");
		break;
	case IPL_INFRAVISION:
		strcpy(tempstr, "see with infravision");
		break;
	case IPL_INVCURS:
		strcpy(tempstr, " ");
		break;
	case IPL_ADDACLIFE:
		strcpy(tempstr, "Armor class added to life");
		break;
	case IPL_ADDMANAAC:
		strcpy(tempstr, "10% of mana added to armor");
		break;
	case IPL_FIRERESCLVL:
		if(x->_iPLFR <= 0) {
			sprintf(tempstr, " ");
		} else if(x->_iPLFR >= 1) {
			sprintf(tempstr, "Resist Fire : %+i%%", x->_iPLFR);
		}
		break;
	default:
		strcpy(tempstr, "Another ability (NW)");
		break;
	}
}

void DrawUTextBack()
{
	CelDecodeOnly(88, 487, (BYTE *)pSTextBoxCels, 1, 271);

	/// ASSERT: assert(gpBuffer);

#ifdef USE_ASM
	__asm {
		mov		edi, gpBuffer
		add		edi, SCREENXY(27, 324)
		xor		eax, eax
		mov		edx, 297 / 2
	label1:
		mov		ecx, 265 / 2
	label2:
		stosb
		inc		edi
		loop	label2
		stosb
		sub		edi, 768 + 265
		mov		ecx, 265 / 2
	label3:
		inc		edi
		stosb
		loop	label3
		sub		edi, 768 + (265 - 1)
		dec		edx
		jnz		label1
		mov		ecx, 265 / 2
	label4:
		stosb
		inc		edi
		loop	label4
		stosb
	}
#else
	int wdt, hgt;
	BYTE *dst;

	dst = &gpBuffer[SCREENXY(27, 324)];

	for(hgt = 297 / 2; hgt != 0; hgt--) {
		for(wdt = 265 / 2; wdt != 0; wdt--) {
			dst[0] = 0;
			dst += 2;
		}
		*dst = 0;
		dst -= 768 + (265 - 1);
		for(wdt = 265 / 2; wdt != 0; wdt--) {
			dst[1] = 0;
			dst += 2;
		}
		dst -= 768 + (265 - 1);
	}
	for(wdt = 265 / 2; wdt != 0; wdt--) {
		dst[0] = 0;
		dst += 2;
	}
	*dst = 0;
#endif
}

void PrintUString(int x, int y, int cjustflag, char *str, int col)
{
	int i, l, w, o, sy, No;
	BYTE c;

	sy = SStringY[y];
	No = x + 32 + 64 + PitchTbl[sy + 44 + 160];
	l = strlen(str);
	o = 0;

	if(cjustflag) {
		w = 0;
		for(i = 0; i < l; i++) {
			c = fontframe[gbFontTransTbl[(BYTE)str[i]]];
			w += fontkern[c] + 1;
		}
		if(w < 257) {
			o = (257 - w) >> 1;
		}
		No += o;
	}

	for(i = 0; i < l; i++) {
		c = fontframe[gbFontTransTbl[(BYTE)str[i]]];
		o += fontkern[c] + 1;
		if(c != '\0' && o <= 257) {
			PrintChar(No, c, col);
		}
		No += fontkern[c] + 1;
	}
}

void DrawULine(int y)
{
	/// ASSERT: assert(gpBuffer);

#ifdef USE_ASM
	int yy;

	yy = PitchTbl[SStringY[y] + 198] + 26 + 64;

	__asm {
		mov		esi, gpBuffer
		mov		edi, esi
		add		esi, SCREENXY(26, 25)
		add		edi, yy
		mov		ebx, 768 - 266
		mov		edx, 3
	copyline:
		mov		ecx, 266 / 4
		rep movsd
		movsw
		add		esi, ebx
		add		edi, ebx
		dec		edx
		jnz		copyline
	}
#else
	int i;
	BYTE *src, *dst;

	src = &gpBuffer[SCREENXY(26, 25)];
	dst = &gpBuffer[PitchTbl[SStringY[y] + 198] + 26 + 64];

	for(i = 0; i < 3; i++, src += 768, dst += 768)
		memcpy(dst, src, 266);
#endif
}

void DrawUniqueInfo()
{
	int uid, y;

	if(chrflag || questlog) {
		return;
	}

	uid = curruitem._iUid;
	DrawUTextBack();
	PrintUString(0, 2, 1, UniqueItemList[uid].UIName, COL_GOLD);
	DrawULine(5);
	PrintItemPower(UniqueItemList[uid].UIPower1, &curruitem);
	y = 6 - UniqueItemList[uid].UINumPL + 8;
	PrintUString(0, y, 1, tempstr, COL_WHITE);

	if(UniqueItemList[uid].UINumPL > 1) {
		PrintItemPower(UniqueItemList[uid].UIPower2, &curruitem);
		PrintUString(0, y + 2, 1, tempstr, COL_WHITE);
	}
	if(UniqueItemList[uid].UINumPL > 2) {
		PrintItemPower(UniqueItemList[uid].UIPower3, &curruitem);
		PrintUString(0, y + 4, 1, tempstr, COL_WHITE);
	}
	if(UniqueItemList[uid].UINumPL > 3) {
		PrintItemPower(UniqueItemList[uid].UIPower4, &curruitem);
		PrintUString(0, y + 6, 1, tempstr, COL_WHITE);
	}
	if(UniqueItemList[uid].UINumPL > 4) {
		PrintItemPower(UniqueItemList[uid].UIPower5, &curruitem);
		PrintUString(0, y + 8, 1, tempstr, COL_WHITE);
	}
	if(UniqueItemList[uid].UINumPL > 5) {
		PrintItemPower(UniqueItemList[uid].UIPower6, &curruitem);
		PrintUString(0, y + 10, 1, tempstr, COL_WHITE);
	}
}

void PrintItemMisc(ItemStruct *x)
{
	if(x->_iMiscId == IMISC_SCROLL) {
		strcpy(tempstr, "Right-click to read");
		AddPanelString(tempstr, 1);
	}
	if(x->_iMiscId == IMISC_SCROLLT) {
		strcpy(tempstr, "Right-click to read, then");
		AddPanelString(tempstr, 1);
		strcpy(tempstr, "left-click to target");
		AddPanelString(tempstr, 1);
	}
	if(x->_iMiscId >= IMISC_USEFIRST && x->_iMiscId <= IMISC_USELAST) {
		PrintItemOil(x->_iMiscId);
		strcpy(tempstr, "Right click to use");
		AddPanelString(tempstr, 1);
	}
	if(x->_iMiscId == IMISC_BOOK) {
		strcpy(tempstr, "Right click to read");
		AddPanelString(tempstr, 1);
	}
	if(x->_iMiscId == IMISC_MAPOFDOOM) {
		strcpy(tempstr, "Right click to view");
		AddPanelString(tempstr, 1);
	}
	if(x->_iMiscId == IMISC_EAR) {
		sprintf(tempstr, "Level : %i", x->_ivalue);
		AddPanelString(tempstr, 1);
	}
}

void PrintItemDetails(ItemStruct *x)
{
	if(x->_iClass == ICLASS_WEAPON) {
		if(x->_iMaxDur == 255) {
			sprintf(tempstr, "damage: %i-%i  Indestructible", x->_iMinDam, x->_iMaxDam);
		} else {
			sprintf(tempstr, "damage: %i-%i  Dur: %i/%i", x->_iMinDam, x->_iMaxDam, x->_iDurability, x->_iMaxDur);
		}
		AddPanelString(tempstr, 1);
	}
	if(x->_iClass == ICLASS_ARMOR) {
		if(x->_iMaxDur == 255) {
			sprintf(tempstr, "armor: %i  Indestructible", x->_iAC);
		} else {
			sprintf(tempstr, "armor: %i  Dur: %i/%i", x->_iAC, x->_iDurability, x->_iMaxDur);
		}
		AddPanelString(tempstr, 1);
	}
	if(x->_iMiscId == IMISC_STAFF && x->_iMaxCharges != 0) {
		sprintf(tempstr, "dam: %i-%i  Dur: %i/%i", x->_iMinDam, x->_iMaxDam, x->_iDurability, x->_iMaxDur);
		sprintf(tempstr, "Charges: %i/%i", x->_iCharges, x->_iMaxCharges);
		AddPanelString(tempstr, 1);
	}
	if(x->_iPrePower != -1) {
		PrintItemPower(x->_iPrePower, x);
		AddPanelString(tempstr, 1);
	}
	if(x->_iSufPower != -1) {
		PrintItemPower(x->_iSufPower, x);
		AddPanelString(tempstr, 1);
	}
	if(x->_iMagical == 2) {
		AddPanelString("unique item", 1);
		uitemflag = 1;
		curruitem = *x;
	}

	PrintItemMisc(x);

	if(x->_iMinStr + x->_iMinMag + x->_iMinDex) {
		strcpy(tempstr, "Required:");
		if(x->_iMinStr != 0) {
			sprintf(tempstr, "%s %i Str", tempstr, x->_iMinStr);
		}
		if(x->_iMinMag != 0) {
			sprintf(tempstr, "%s %i Mag", tempstr, x->_iMinMag);
		}
		if(x->_iMinDex != 0) {
			sprintf(tempstr, "%s %i Dex", tempstr, x->_iMinDex);
		}
		AddPanelString(tempstr, 1);
	}

	pinfoflag = 1;
}

void PrintItemDur(ItemStruct *x)
{
	if(x->_iClass == ICLASS_WEAPON) {
		if(x->_iMaxDur == 255) {
			sprintf(tempstr, "damage: %i-%i  Indestructible", x->_iMinDam, x->_iMaxDam);
		} else {
			sprintf(tempstr, "damage: %i-%i  Dur: %i/%i", x->_iMinDam, x->_iMaxDam, x->_iDurability, x->_iMaxDur);
		}
		AddPanelString(tempstr, 1);
		if(x->_iMiscId == IMISC_STAFF && x->_iMaxCharges != 0) {
			sprintf(tempstr, "Charges: %i/%i", x->_iCharges, x->_iMaxCharges);
			AddPanelString(tempstr, 1);
		}
		if(x->_iMagical != 0) {
			AddPanelString("Not Identified", 1);
		}
	}
	if(x->_iClass == ICLASS_ARMOR) {
		if(x->_iMaxDur == 255) {
			sprintf(tempstr, "armor: %i  Indestructible", x->_iAC);
		} else {
			sprintf(tempstr, "armor: %i  Dur: %i/%i", x->_iAC, x->_iDurability, x->_iMaxDur);
		}
		AddPanelString(tempstr, 1);
		if(x->_iMagical != 0) {
			AddPanelString("Not Identified", 1);
		}
		if(x->_iMiscId == IMISC_STAFF && x->_iMaxCharges != 0) { /// BUGFIX: Staff code is useless here, remove
			sprintf(tempstr, "Charges: %i/%i", x->_iCharges, x->_iMaxCharges);
			AddPanelString(tempstr, 1);
		}
	}
	if(x->_itype == ITYPE_RING || x->_itype == ITYPE_AMULET) {
		AddPanelString("Not Identified", 1);
	}

	PrintItemMisc(x);

	if(x->_iMinStr + x->_iMinMag + x->_iMinDex) {
		strcpy(tempstr, "Required:");
		if(x->_iMinStr != 0) {
			sprintf(tempstr, "%s %i Str", tempstr, x->_iMinStr);
		}
		if(x->_iMinMag != 0) {
			sprintf(tempstr, "%s %i Mag", tempstr, x->_iMinMag);
		}
		if(x->_iMinDex != 0) {
			sprintf(tempstr, "%s %i Dex", tempstr, x->_iMinDex);
		}
		AddPanelString(tempstr, 1);
	}

	pinfoflag = 1;
}

void UseItem(int p, int Mid, int spl)
{
	long l;
	__int64 t; /* todo: check usage */

	switch(Mid) {
	case IMISC_HEAL:
	case IMISC_MEAT:
		l = plr[p]._pMaxHP >> 8;
		l = ((l >> 1) + random(39, l)) << 6;
		if(plr[p]._pClass == PC_WARRIOR) {
			l *= 2;
		}
		if(plr[p]._pClass == PC_ROGUE) {
			l += l >> 1;
		}
		plr[p]._pHitPoints += l;
		if(plr[p]._pHitPoints > plr[p]._pMaxHP) {
			plr[p]._pHitPoints = plr[p]._pMaxHP;
		}
		plr[p]._pHPBase += l;
		if(plr[p]._pHPBase > plr[p]._pMaxHPBase) {
			plr[p]._pHPBase = plr[p]._pMaxHPBase;
		}
		drawhpflag = 1;
		break;
	case IMISC_FULLHEAL:
		plr[p]._pHitPoints = plr[p]._pMaxHP;
		plr[p]._pHPBase = plr[p]._pMaxHPBase;
		drawhpflag = 1;
		break;
	case IMISC_MANA:
		l = plr[p]._pMaxMana >> 8;
		l = ((l >> 1) + random(40, l)) << 6;
		if(plr[p]._pClass == PC_SORCERER) {
			l *= 2;
		}
		if(plr[p]._pClass == PC_ROGUE) {
			l += l >> 1;
		}
		if(!(plr[p]._pIFlags & ISPL_NOMANA)) {
			plr[p]._pMana += l;
			if(plr[p]._pMana > plr[p]._pMaxMana) {
				plr[p]._pMana = plr[p]._pMaxMana;
			}
			plr[p]._pManaBase += l;
			if(plr[p]._pManaBase > plr[p]._pMaxManaBase) {
				plr[p]._pManaBase = plr[p]._pMaxManaBase;
			}
			drawmanaflag = 1;
		}
		break;
	case IMISC_FULLMANA:
		if(!(plr[p]._pIFlags & ISPL_NOMANA)) {
			plr[p]._pMana = plr[p]._pMaxMana;
			plr[p]._pManaBase = plr[p]._pMaxManaBase;
			drawmanaflag = 1;
		}
		break;
	case IMISC_ELIXSTR:
		ModifyPlrStr(p, 1);
		break;
	case IMISC_ELIXMAG:
		ModifyPlrMag(p, 1);
		break;
	case IMISC_ELIXDEX:
		ModifyPlrDex(p, 1);
		break;
	case IMISC_ELIXVIT:
		ModifyPlrVit(p, 1);
		break;
	case IMISC_BOOK:
		t = 1;
		plr[p]._pMemSpells64 |= t << (spl - 1);
		if(plr[p]._pSplLvl[spl] < 15) {
			plr[p]._pSplLvl[spl]++;
		}
		plr[p]._pMana += spelldata[spl].sManaCost << 6;
		if(plr[p]._pMana > plr[p]._pMaxMana) {
			plr[p]._pMana = plr[p]._pMaxMana;
		}
		plr[p]._pManaBase += spelldata[spl].sManaCost << 6;
		if(plr[p]._pManaBase > plr[p]._pMaxManaBase) {
			plr[p]._pManaBase = plr[p]._pMaxManaBase;
		}
		if(p == myplr) {
			CalcPlrBookVals(p);
		}
		drawmanaflag = 1;
		break;
	case IMISC_REJUV:
		l = plr[p]._pMaxHP >> 8;
		l = ((l >> 1) + random(39, l)) << 6;
		if(plr[p]._pClass == PC_WARRIOR) {
			l *= 2;
		}
		if(plr[p]._pClass == PC_ROGUE) {
			l += l >> 1;
		}
		plr[p]._pHitPoints += l;
		if(plr[p]._pHitPoints > plr[p]._pMaxHP) {
			plr[p]._pHitPoints = plr[p]._pMaxHP;
		}
		plr[p]._pHPBase += l;
		if(plr[p]._pHPBase > plr[p]._pMaxHPBase) {
			plr[p]._pHPBase = plr[p]._pMaxHPBase;
		}
		drawhpflag = 1;
		l = plr[p]._pMaxMana >> 8;
		l = ((l >> 1) + random(40, l)) << 6;
		if(plr[p]._pClass == PC_SORCERER) {
			l *= 2;
		}
		if(plr[p]._pClass == PC_ROGUE) {
			l += l >> 1;
		}
		if(!(plr[p]._pIFlags & ISPL_NOMANA)) {
			plr[p]._pMana += l;
			if(plr[p]._pMana > plr[p]._pMaxMana) {
				plr[p]._pMana = plr[p]._pMaxMana;
			}
			plr[p]._pManaBase += l;
			if(plr[p]._pManaBase > plr[p]._pMaxManaBase) {
				plr[p]._pManaBase = plr[p]._pMaxManaBase;
			}
			drawmanaflag = 1;
		}
		break;
	case IMISC_FULLREJUV:
		plr[p]._pHitPoints = plr[p]._pMaxHP;
		plr[p]._pHPBase = plr[p]._pMaxHPBase;
		drawhpflag = 1;
		if(!(plr[p]._pIFlags & ISPL_NOMANA)) {
			plr[p]._pMana = plr[p]._pMaxMana;
			plr[p]._pManaBase = plr[p]._pMaxManaBase;
			drawmanaflag = 1;
		}
		break;
	case IMISC_SCROLL:
		if(spelldata[spl].sTargeted) {
			plr[p]._pTSpell = spl;
			plr[p]._pTSplType = 4;
			if(p == myplr) {
				SetCursor_(CURSOR_TELEPORT); /* NewCursor */
			}
		} else {
			ClrPlrPath(p);
			plr[p]._pSpell = spl;
			plr[p]._pSplType = 4;
			plr[p]._pSplFrom = 3;
			plr[p].destAction = 12;
			plr[p].destParam1 = cursmx;
			plr[p].destParam2 = cursmy;
			if(p == myplr && spl == SPL_NOVA) {
				NetSendCmdLoc(TRUE, CMD_NOVA, cursmx, cursmy);
			}
		}
		break;
	case IMISC_SCROLLT:
		if(spelldata[spl].sTargeted) {
			plr[p]._pTSpell = spl;
			plr[p]._pTSplType = 4;
			if(p == myplr) {
				SetCursor_(CURSOR_TELEPORT); /* NewCursor */
			}
		} else {
			ClrPlrPath(p);
			plr[p]._pSpell = spl;
			plr[p]._pSplType = 4;
			plr[p]._pSplFrom = 3;
			plr[p].destAction = 12;
			plr[p].destParam1 = cursmx;
			plr[p].destParam2 = cursmy;
		}
		break;
	case IMISC_MAPOFDOOM:
		doom_init();
		break;
	case IMISC_SPECELIX:
		ModifyPlrStr(p, 3);
		ModifyPlrMag(p, 3);
		ModifyPlrDex(p, 3);
		ModifyPlrVit(p, 3);
		break;
	}
}

BOOL StoreStatOk(ItemStruct *h)
{
	BOOL sf;

	sf = TRUE;
	if(plr[myplr]._pStrength < h->_iMinStr) {
		sf = FALSE;
	}
	if(plr[myplr]._pMagic < h->_iMinMag) {
		sf = FALSE;
	}
	if(plr[myplr]._pDexterity < h->_iMinDex) {
		sf = FALSE;
	}

	return sf;
}

BOOL SmithItemOk(int i)
{
	BOOL rv;

	rv = TRUE;
	if(AllItemsList[i].itype == ITYPE_MISC) {
		rv = FALSE;
	}
	if(AllItemsList[i].itype == ITYPE_GOLD) {
		rv = FALSE;
	}
	if(AllItemsList[i].itype == ITYPE_MEAT) {
		rv = FALSE;
	}
	if(AllItemsList[i].itype == ITYPE_STAFF) {
		rv = FALSE;
	}
	if(AllItemsList[i].itype == ITYPE_RING) {
		rv = FALSE;
	}
	if(AllItemsList[i].itype == ITYPE_AMULET) {
		rv = FALSE;
	}

	return rv;
}

int RndSmithItem(int lvl)
{
	int i, ri;
	int ril[512];

	ri = 0;
	for(i = 1; AllItemsList[i].iLoc != -1; i++) {
		if(AllItemsList[i].iRnd != IDROP_NEVER && SmithItemOk(i) && lvl >= AllItemsList[i].iMinMLvl) {
			ril[ri++] = i;
			if(AllItemsList[i].iRnd == IDROP_DOUBLE) {
				ril[ri++] = i;
			}
		}
	}

	return ril[random(50, ri)] + 1;
}

void BubbleSwapItem(ItemStruct *a, ItemStruct *b)
{
	ItemStruct h;

	h = *a;
	*a = *b;
	*b = h;
}

void SortSmith()
{
	int j, k;
	BOOL sorted;

	k = 0;
	for(j = 1; smithitem[j]._itype != -1; j++) {
		k++;
	}

	sorted = FALSE;
	while(k > 0 && !sorted) {
		sorted = TRUE;
		for(j = 0; j < k; j++) {
			if(smithitem[j].IDidx > smithitem[j + 1].IDidx) {
				BubbleSwapItem(&smithitem[j], &smithitem[j + 1]);
				sorted = FALSE;
			}
		}
		k--;
	}
}

void SpawnSmith(int lvl)
{
	int i, nsi, itype, maxval;

	maxval = 140000;
	nsi = random(50, 10) + 10;
	for(i = 0; i < nsi; i++) {
		do {
			item[0]._iSeed = GetRndSeed();
			SetRndSeed(item[0]._iSeed);
			itype = RndSmithItem(lvl) - 1;
			GetItemAttrs(0, itype, lvl);
		} while(item[0]._iIvalue > maxval);
		smithitem[i] = item[0];
		smithitem[i]._iCreateInfo = lvl | 0x400;
		smithitem[i]._iIdentified = 1;
		smithitem[i]._iStatFlag = StoreStatOk(&smithitem[i]);
	}
	for(i = nsi; i < 20; i++) {
		smithitem[i]._itype = -1;
	}

	SortSmith();
}

BOOL PremiumItemOk(int i)
{
	BOOL rv;

	rv = TRUE;
	if(AllItemsList[i].itype == ITYPE_MISC) {
		rv = FALSE;
	}
	if(AllItemsList[i].itype == ITYPE_GOLD) {
		rv = FALSE;
	}
	if(AllItemsList[i].itype == ITYPE_MEAT) {
		rv = FALSE;
	}
	if(AllItemsList[i].itype == ITYPE_STAFF) {
		rv = FALSE;
	}
	if(gbMaxPlayers != 1 && AllItemsList[i].itype == ITYPE_RING) {
		rv = FALSE;
	}
	if(gbMaxPlayers != 1 && AllItemsList[i].itype == ITYPE_AMULET) {
		rv = FALSE;
	}

	return rv;
}

int RndPremiumItem(int minlvl, int maxlvl)
{
	int i, ri;
	int ril[512];

	ri = 0;
	for(i = 1; AllItemsList[i].iLoc != -1; i++) {
		if(AllItemsList[i].iRnd != IDROP_NEVER
		&& PremiumItemOk(i)
		&& AllItemsList[i].iMinMLvl >= minlvl
		&& AllItemsList[i].iMinMLvl <= maxlvl) {
			ril[ri++] = i;
		}
	}

	return ril[random(50, ri)] + 1;
}

void SpawnOnePremium(int i, int plvl)
{
	int itype, maxval;
	ItemStruct holditem;

	maxval = 140000;
	holditem = item[0];

#ifdef _DEBUG
	if(plvl > 30 || debug_mode_key_w) {
#else
	if(plvl > 30) {
#endif
		plvl = 30;
	}
	if(plvl < 1) {
		plvl = 1;
	}

	do {
		item[0]._iSeed = GetRndSeed();
		SetRndSeed(item[0]._iSeed);
		itype = RndPremiumItem(plvl >> 2, plvl) - 1;
		GetItemAttrs(0, itype, plvl);
		GetItemBonus(0, itype, plvl >> 1, plvl, 1);
	} while(item[0]._iIvalue > maxval);
	premiumitem[i] = item[0];
	premiumitem[i]._iCreateInfo = plvl | 0x800;
	premiumitem[i]._iIdentified = 1;
	premiumitem[i]._iStatFlag = StoreStatOk(&premiumitem[i]);

	item[0] = holditem;
}

void SpawnPremium(int lvl)
{
	int i;

	if(numpremium < 6) {
		for(i = 0; i < 6; i++) {
			if(premiumitem[i]._itype == -1) {
				SpawnOnePremium(i, premiumlevel + premiumlvladd[i]);
			}
		}
		numpremium = 6;
	}
	while(premiumlevel < lvl) {
		premiumlevel++;
		premiumitem[0] = premiumitem[2];
		premiumitem[1] = premiumitem[3];
		premiumitem[2] = premiumitem[4];
		SpawnOnePremium(3, premiumlevel + premiumlvladd[3]);
		premiumitem[4] = premiumitem[5];
		SpawnOnePremium(5, premiumlevel + premiumlvladd[5]);
	}
}

BOOL WitchItemOk(int i)
{
	BOOL rv;

	rv = FALSE;
	if(AllItemsList[i].itype == ITYPE_MISC) {
		rv = TRUE;
	}
	if(AllItemsList[i].itype == ITYPE_STAFF) {
		rv = TRUE;
	}
	if(AllItemsList[i].iMiscId == IMISC_MANA) {
		rv = FALSE;
	}
	if(AllItemsList[i].iMiscId == IMISC_FULLMANA) {
		rv = FALSE;
	}
	if(AllItemsList[i].iSpell == SPL_TOWN) {
		rv = FALSE;
	}
	if(AllItemsList[i].iMiscId == IMISC_FULLHEAL) {
		rv = FALSE;
	}
	if(AllItemsList[i].iMiscId == IMISC_HEAL) {
		rv = FALSE;
	}
	if(AllItemsList[i].iSpell == SPL_RESURRECT && gbMaxPlayers == 1) {
		rv = FALSE;
	}
	if(AllItemsList[i].iSpell == SPL_HEALOTHER && gbMaxPlayers == 1) {
		rv = FALSE;
	}

	return rv;
}

int RndWitchItem(int lvl)
{
	int i, ri;
	int ril[512];

	ri = 0;
	for(i = 1; AllItemsList[i].iLoc != -1; i++) {
		if(AllItemsList[i].iRnd != IDROP_NEVER && WitchItemOk(i) && lvl >= AllItemsList[i].iMinMLvl) {
			ril[ri++] = i;
		}
	}

	return ril[random(51, ri)] + 1;
}

void SortWitch()
{
	int j, k;
	BOOL sorted;

	k = 3;
	for(j = 4; witchitem[j]._itype != -1; j++) {
		k++;
	}

	sorted = FALSE;
	while(k > 3 && !sorted) {
		sorted = TRUE;
		for(j = 3; j < k; j++) {
			if(witchitem[j].IDidx > witchitem[j + 1].IDidx) {
				BubbleSwapItem(&witchitem[j], &witchitem[j + 1]);
				sorted = FALSE;
			}
		}
		k--;
	}
}

void WitchBookLevel(int ii)
{
	int slvl;

	if(witchitem[ii]._iMiscId != IMISC_BOOK) {
		return;
	}

	witchitem[ii]._iMinMag = spelldata[witchitem[ii]._iSpell].sMinInt;
	slvl = plr[myplr]._pSplLvl[witchitem[ii]._iSpell];
	while(slvl != 0) {
		witchitem[ii]._iMinMag += 20 * witchitem[ii]._iMinMag / 100;
		slvl--;
		if(witchitem[ii]._iMinMag + 20 * witchitem[ii]._iMinMag / 100 > 255) {
			witchitem[ii]._iMinMag = 255;
			slvl = 0;
		}
	}
}

void SpawnWitch(int lvl)
{
	int itype, iblvl, i, nsi, maxval;

	maxval = 140000;

	GetItemAttrs(0, IDI_MANA, 1);
	witchitem[0] = item[0];
	witchitem[0]._iCreateInfo = lvl;
	witchitem[0]._iStatFlag = 1;
	GetItemAttrs(0, IDI_FULLMANA, 1);
	witchitem[1] = item[0];
	witchitem[1]._iCreateInfo = lvl;
	witchitem[1]._iStatFlag = 1;
	GetItemAttrs(0, IDI_PORTAL, 1);
	witchitem[2] = item[0];
	witchitem[2]._iCreateInfo = lvl;
	witchitem[2]._iStatFlag = 1;

	nsi = random(51, 8) + 10;
	for(i = 3; i < nsi; i++) {
		do {
			item[0]._iSeed = GetRndSeed();
			SetRndSeed(item[0]._iSeed);
			itype = RndWitchItem(lvl) - 1;
			GetItemAttrs(0, itype, lvl);
			iblvl = -1;
			if(random(51, 100) <= 5) {
				iblvl = 2 * lvl;
			}
			if(iblvl == -1 && item[0]._iMiscId == IMISC_STAFF) {
				iblvl = 2 * lvl;
			}
#ifdef _DEBUG
			if(debug_mode_key_inverted_v) {
				iblvl = 2 * lvl;
			}
#endif
			if(iblvl != -1) {
				GetItemBonus(0, itype, iblvl >> 1, iblvl, 1);
			}
		} while(item[0]._iIvalue > maxval);
		witchitem[i] = item[0];
		witchitem[i]._iCreateInfo = lvl | 0x2000;
		witchitem[i]._iIdentified = 1;
		WitchBookLevel(i);
		witchitem[i]._iStatFlag = StoreStatOk(&witchitem[i]);
	}
	for(i = nsi; i < 20; i++) {
		witchitem[i]._itype = -1;
	}

	SortWitch();
}

int RndBoyItem(int lvl)
{
	int i, ri;
	int ril[512];

	ri = 0;
	for(i = 1; AllItemsList[i].iLoc != -1; i++) {
		if(AllItemsList[i].iRnd != IDROP_NEVER && PremiumItemOk(i) && lvl >= AllItemsList[i].iMinMLvl) {
			ril[ri++] = i;
		}
	}

	return ril[random(49, ri)] + 1;
}

void SpawnBoy(int lvl)
{
	int itype, maxval;

	maxval = 90000;

	if(boylevel >= lvl >> 1 && boyitem._itype != -1) {
		return;
	}

	do {
		item[0]._iSeed = GetRndSeed();
		SetRndSeed(item[0]._iSeed);
		itype = RndBoyItem(lvl) - 1;
		GetItemAttrs(0, itype, lvl);
		GetItemBonus(0, itype, lvl, 2 * lvl, 1);
	} while(item[0]._iIvalue > maxval);
	boyitem = item[0];
	boyitem._iCreateInfo = lvl | 0x1000;
	boyitem._iIdentified = 1;
	boyitem._iStatFlag = StoreStatOk(&boyitem);
	boylevel = lvl >> 1;
}

BOOL HealerItemOk(int i)
{
	BOOL rv;

	rv = FALSE;
	if(AllItemsList[i].itype != ITYPE_MISC) {
		return FALSE;
	}
	if(AllItemsList[i].iMiscId == IMISC_SCROLL && AllItemsList[i].iSpell == SPL_HEAL) {
		rv = TRUE;
	}
	if(AllItemsList[i].iMiscId == IMISC_SCROLLT && AllItemsList[i].iSpell == SPL_RESURRECT && gbMaxPlayers != 1) {
		rv = FALSE;
	}
	if(AllItemsList[i].iMiscId == IMISC_SCROLLT && AllItemsList[i].iSpell == SPL_HEALOTHER && gbMaxPlayers != 1) {
		rv = TRUE;
	}
	if(gbMaxPlayers == 1) {
		if(AllItemsList[i].iMiscId == IMISC_ELIXSTR) {
			rv = TRUE;
		}
		if(AllItemsList[i].iMiscId == IMISC_ELIXMAG) {
			rv = TRUE;
		}
		if(AllItemsList[i].iMiscId == IMISC_ELIXDEX) {
			rv = TRUE;
		}
		if(AllItemsList[i].iMiscId == IMISC_ELIXVIT) {
			rv = TRUE;
		}
	}
	if(AllItemsList[i].iMiscId == IMISC_FULLHEAL) { /// BUGFIX: same value checked below
		rv = TRUE;
	}
	if(AllItemsList[i].iMiscId == IMISC_REJUV) {
		rv = TRUE;
	}
	if(AllItemsList[i].iMiscId == IMISC_FULLREJUV) {
		rv = TRUE;
	}
	if(AllItemsList[i].iMiscId == IMISC_HEAL) {
		rv = FALSE;
	}
	if(AllItemsList[i].iMiscId == IMISC_FULLHEAL) {
		rv = FALSE;
	}
	if(AllItemsList[i].iMiscId == IMISC_MANA) {
		rv = FALSE;
	}
	if(AllItemsList[i].iMiscId == IMISC_FULLMANA) {
		rv = FALSE;
	}

	return rv;
}

int RndHealerItem(int lvl)
{
	int i, ri;
	int ril[512];

	ri = 0;
	for(i = 1; AllItemsList[i].iLoc != -1; i++) {
		if(AllItemsList[i].iRnd != IDROP_NEVER && HealerItemOk(i) && lvl >= AllItemsList[i].iMinMLvl) {
			ril[ri++] = i;
		}
	}

	return ril[random(50, ri)] + 1;
}

void SortHealer()
{
	int j, k;
	BOOL sorted;

	k = 2;
	for(j = 3; healitem[j]._itype != -1; j++) {
		k++;
	}

	sorted = FALSE;
	while(k > 2 && !sorted) {
		sorted = TRUE;
		for(j = 2; j < k; j++) {
			if(healitem[j].IDidx > healitem[j + 1].IDidx) {
				BubbleSwapItem(&healitem[j], &healitem[j + 1]);
				sorted = FALSE;
			}
		}
		k--;
	}
}

void SpawnHealer(int lvl)
{
	int i, nsi, srnd, itype;

	GetItemAttrs(0, IDI_HEAL, 1);
	healitem[0] = item[0];
	healitem[0]._iCreateInfo = lvl;
	healitem[0]._iStatFlag = 1;
	GetItemAttrs(0, IDI_FULLHEAL, 1);
	healitem[1] = item[0];
	healitem[1]._iCreateInfo = lvl;
	healitem[1]._iStatFlag = 1;

	if(gbMaxPlayers != 1) {
		GetItemAttrs(0, IDI_RESURRECT, 1);
		healitem[2] = item[0];
		healitem[2]._iCreateInfo = lvl;
		healitem[2]._iStatFlag = 1;
		srnd = 3;
	} else {
		srnd = 2;
	}

	nsi = random(50, 8) + 10;
	for(i = srnd; i < nsi; i++) {
		item[0]._iSeed = GetRndSeed();
		SetRndSeed(item[0]._iSeed);
		itype = RndHealerItem(lvl) - 1;
		GetItemAttrs(0, itype, lvl);
		healitem[i] = item[0];
		healitem[i]._iCreateInfo = lvl | 0x4000;
		healitem[i]._iIdentified = 1;
		healitem[i]._iStatFlag = StoreStatOk(&healitem[i]);
	}
	for(i = nsi; i < 20; i++) {
		healitem[i]._itype = -1;
	}

	SortHealer();
}

void SpawnStoreGold()
{
	GetItemAttrs(0, IDI_GOLD, 1);
	golditem = item[0];
	golditem._iStatFlag = 1;
}

void RecreateSmithItem(int ii, int idx, int lvl, int iseed)
{
	int itype;

	SetRndSeed(iseed);
	itype = RndSmithItem(lvl) - 1;
	GetItemAttrs(ii, itype, lvl);
	item[ii]._iSeed = iseed;
	item[ii]._iCreateInfo = lvl | 0x400;
	item[ii]._iIdentified = 1;
}

void RecreatePremiumItem(int ii, int idx, int plvl, int iseed)
{
	int itype;

	SetRndSeed(iseed);
	itype = RndPremiumItem(plvl >> 2, plvl) - 1;
	GetItemAttrs(ii, itype, plvl);
	GetItemBonus(ii, itype, plvl >> 1, plvl, 1);
	item[ii]._iSeed = iseed;
	item[ii]._iCreateInfo = plvl | 0x800;
	item[ii]._iIdentified = 1;
}

void RecreateBoyItem(int ii, int idx, int lvl, int iseed)
{
	int itype;

	SetRndSeed(iseed);
	itype = RndBoyItem(lvl) - 1;
	GetItemAttrs(ii, itype, lvl);
	GetItemBonus(ii, itype, lvl, 2 * lvl, 1);
	item[ii]._iSeed = iseed;
	item[ii]._iCreateInfo = lvl | 0x1000;
	item[ii]._iIdentified = 1;
}

void RecreateWitchItem(int ii, int idx, int lvl, int iseed)
{
	int itype, iblvl;

	if(idx == IDI_MANA || idx == IDI_FULLMANA || idx == IDI_PORTAL) {
		GetItemAttrs(ii, idx, lvl);
	} else {
		SetRndSeed(iseed);
		itype = RndWitchItem(lvl) - 1;
		GetItemAttrs(ii, itype, lvl);
		iblvl = -1;
		if(random(51, 100) <= 5) {
			iblvl = 2 * lvl;
		}
		if(iblvl == -1 && item[ii]._iMiscId == IMISC_STAFF) {
			iblvl = 2 * lvl;
		}
#ifdef _DEBUG
		if(debug_mode_key_inverted_v) {
			iblvl = 2 * lvl;
		}
#endif
		if(iblvl != -1) {
			GetItemBonus(ii, itype, iblvl >> 1, iblvl, 1);
		}
	}

	item[ii]._iSeed = iseed;
	item[ii]._iCreateInfo = lvl | 0x2000;
	item[ii]._iIdentified = 1;
}

void RecreateHealerItem(int ii, int idx, int lvl, int iseed)
{
	int itype;

	if(idx == IDI_HEAL || idx == IDI_FULLHEAL || idx == IDI_RESURRECT) {
		GetItemAttrs(ii, idx, lvl);
	} else {
		SetRndSeed(iseed);
		itype = RndHealerItem(lvl) - 1;
		GetItemAttrs(ii, itype, lvl);
	}

	item[ii]._iSeed = iseed;
	item[ii]._iCreateInfo = lvl | 0x4000;
	item[ii]._iIdentified = 1;
}

void RecreateTownItem(int ii, int idx, WORD icreateinfo, int iseed, int ivalue)
{
	if(icreateinfo & 0x400) {
		RecreateSmithItem(ii, idx, icreateinfo & 0x3F, iseed);
	} else if(icreateinfo & 0x800) {
		RecreatePremiumItem(ii, idx, icreateinfo & 0x3F, iseed);
	} else if(icreateinfo & 0x1000) {
		RecreateBoyItem(ii, idx, icreateinfo & 0x3F, iseed);
	} else if(icreateinfo & 0x2000) {
		RecreateWitchItem(ii, idx, icreateinfo & 0x3F, iseed);
	} else if(icreateinfo & 0x4000) {
		RecreateHealerItem(ii, idx, icreateinfo & 0x3F, iseed);
	}
}

void RecalcStoreStats()
{
	int i;

	for(i = 0; i < 20; i++) {
		if(smithitem[i]._itype != -1) {
			smithitem[i]._iStatFlag = StoreStatOk(&smithitem[i]);
		}
	}
	for(i = 0; i < 6; i++) {
		if(premiumitem[i]._itype != -1) {
			premiumitem[i]._iStatFlag = StoreStatOk(&premiumitem[i]);
		}
	}
	for(i = 0; i < 20; i++) {
		if(witchitem[i]._itype != -1) {
			witchitem[i]._iStatFlag = StoreStatOk(&witchitem[i]);
		}
	}
	for(i = 0; i < 20; i++) {
		if(healitem[i]._itype != -1) {
			healitem[i]._iStatFlag = StoreStatOk(&healitem[i]);
		}
	}

	boyitem._iStatFlag = StoreStatOk(&boyitem);
}

int ItemNoFlippy()
{
	int r;

	r = itemactive[numitems - 1];
	item[r]._iAnimFrame = item[r]._iAnimLen;
	item[r]._iAnimFlag = 0;
	item[r]._iSelFlag = 1;

	return r;
}

void CreateSpellBook(int x, int y, int ispell, BOOL sendmsg, BOOL delta)
{
	int ii, idx;
	BOOL done;

	done = FALSE;
	idx = RndTypeItems(ITYPE_MISC, IMISC_BOOK);

	if(numitems >= MAXITEMS) {
		return;
	}

	ii = itemavail[0];
	GetSuperItemSpace(x, y, ii);
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;

	while(!done) {
		SetupAllItems(ii, idx, GetRndSeed(), currlevel << 1, 1, TRUE, FALSE, delta);
		if(item[ii]._iMiscId == IMISC_BOOK && item[ii]._iSpell == ispell) {
			done = TRUE;
		}
	}

	if(sendmsg) {
		NetSendCmdDItem(FALSE, ii);
	}
	if(delta) {
		DeltaAddItem(ii);
	}

	numitems++;
}

void CreateMagicArmor(int x, int y, int imisc, int icurs, BOOL sendmsg, BOOL delta)
{
	int ii, idx;
	BOOL done;

	done = FALSE;

	if(numitems >= MAXITEMS) {
		return;
	}

	ii = itemavail[0];
	GetSuperItemSpace(x, y, ii);
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;
	idx = RndTypeItems(imisc, IMISC_NONE);

	while(!done) {
		SetupAllItems(ii, idx, GetRndSeed(), currlevel << 1, 1, TRUE, FALSE, delta);
		if(item[ii]._iCurs == icurs) {
			done = TRUE;
		} else {
			idx = RndTypeItems(imisc, IMISC_NONE);
		}
	}

	if(sendmsg) {
		NetSendCmdDItem(FALSE, ii);
	}
	if(delta) {
		DeltaAddItem(ii);
	}

	numitems++;
}

void CreateMagicWeapon(int x, int y, int imisc, int icurs, BOOL sendmsg, BOOL delta)
{
	int ii, idx;
	BOOL done;

	done = FALSE;

	if(numitems >= MAXITEMS) {
		return;
	}

	ii = itemavail[0];
	GetSuperItemSpace(x, y, ii);
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;
	idx = RndTypeItems(imisc, IMISC_NONE);

	while(!done) {
		SetupAllItems(ii, idx, GetRndSeed(), currlevel << 1, 1, TRUE, FALSE, delta);
		if(item[ii]._iCurs == icurs) {
			done = TRUE;
		} else {
			idx = RndTypeItems(imisc, IMISC_NONE);
		}
	}

	if(sendmsg) {
		NetSendCmdDItem(FALSE, ii);
	}
	if(delta) {
		DeltaAddItem(ii);
	}

	numitems++;
}

BOOL GetItemRecord(int nSeed, WORD wCI, int nIndex)
{
	int i;
	DWORD dwTicks;

	dwTicks = GetTickCount();

	for(i = 0; i < gnNumGetRecords; i++) {
		if(dwTicks - itemrecord[i].dwTimestamp > 6000) {
			NextItemRecord(i);
			i--;
		} else if(nSeed == itemrecord[i].nSeed && wCI == itemrecord[i].wCI && nIndex == itemrecord[i].nIndex) {
			return FALSE;
		}
	}

	return TRUE;
}

void NextItemRecord(int i)
{
	gnNumGetRecords--;

	if(gnNumGetRecords == 0) {
		return;
	}

	itemrecord[i].dwTimestamp = itemrecord[gnNumGetRecords].dwTimestamp;
	itemrecord[i].nSeed = itemrecord[gnNumGetRecords].nSeed;
	itemrecord[i].wCI = itemrecord[gnNumGetRecords].wCI;
	itemrecord[i].nIndex = itemrecord[gnNumGetRecords].nIndex;
}

void SetItemRecord(int nSeed, WORD wCI, int nIndex)
{
	DWORD dwTicks;

	dwTicks = GetTickCount();

	if(gnNumGetRecords == MAXITEMS) {
		return;
	}

	itemrecord[gnNumGetRecords].dwTimestamp = dwTicks;
	itemrecord[gnNumGetRecords].nSeed = nSeed;
	itemrecord[gnNumGetRecords].wCI = wCI;
	itemrecord[gnNumGetRecords].nIndex = nIndex;
	gnNumGetRecords++;
}

void PutItemRecord(int nSeed, WORD wCI, int nIndex)
{
	int i;
	DWORD dwTicks;

	dwTicks = GetTickCount();

	for(i = 0; i < gnNumGetRecords; i++) {
		if(dwTicks - itemrecord[i].dwTimestamp > 6000) {
			NextItemRecord(i);
			i--;
		} else if(nSeed == itemrecord[i].nSeed && wCI == itemrecord[i].wCI && nIndex == itemrecord[i].nIndex) {
			NextItemRecord(i);
			break;
		}
	}
}
