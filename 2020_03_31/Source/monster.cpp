#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

int MissileFileFlag; // weak
int monstkills[MAXMONSTERS];
int monstactive[MAXMONSTERS];
int nummonsters;
BOOLEAN sgbSaveSoundOn; // weak
MonsterStruct monster[MAXMONSTERS];
int totalmonsters; // weak
CMonster Monsters[16];
BYTE GraphicTable[NUMLEVELS][MAX_LVLMTYPES]; /* unused */
// int END_Monsters_17; // weak
int monstimgtot; // weak
int uniquetrans;
int nummtypes;

/* data */

int MWVel[24][3] =
{
  { 256, 512, 1024 },
  { 128, 256, 512 },
  { 85, 170, 341 },
  { 64, 128, 256 },
  { 51, 102, 204 },
  { 42, 85, 170 },
  { 36, 73, 146 },
  { 32, 64, 128 },
  { 28, 56, 113 },
  { 26, 51, 102 },
  { 23, 46, 93 },
  { 21, 42, 85 },
  { 19, 39, 78 },
  { 18, 36, 73 },
  { 17, 34, 68 },
  { 16, 32, 64 },
  { 15, 30, 60 },
  { 14, 28, 57 },
  { 13, 26, 54 },
  { 12, 25, 51 },
  { 12, 24, 48 },
  { 11, 23, 46 },
  { 11, 22, 44 },
  { 10, 21, 42 }
};
char animletter[7] = "nwahds";
int left[8] = { 7, 0, 1, 2, 3, 4, 5, 6 };
int right[8] = { 1, 2, 3, 4, 5, 6, 7, 0 };
int opposite[8] = { 4, 5, 6, 7, 0, 1, 2, 3 };
int offset_x[8] = { 1, 0, -1, -1, -1, 0, 1, 1 };
int offset_y[8] = { 1, 1, 1, 0, -1, -1, -1, 0 };

/* unused */
int rnd5[4] = { 5, 10, 15, 20 };
int rnd10[4] = { 10, 15, 20, 30 };
int rnd20[4] = { 20, 30, 40, 50 };
int rnd60[4] = { 60, 70, 80, 90 };
//

void (*AiProc[])(int i) =
{
  &MAI_Zombie,
  &MAI_Fat,
  &MAI_SkelSd,
  &MAI_SkelBow,
  &MAI_Scav,
  &MAI_Rhino,
  &MAI_GoatMc,
  &MAI_GoatBow,
  &MAI_Fallen,
  &MAI_Magma,
  &MAI_SkelKing,
  &MAI_Bat,
  &MAI_Garg,
  &MAI_Cleaver,
  &MAI_Succ,
  &MAI_Sneak,
  &MAI_Storm,
  &MAI_Fireman,
  &MAI_Garbud,
  &MAI_Acid,
  &MAI_AcidUniq,
  &MAI_Golum,
  &MAI_Zhar,
  &MAI_SnotSpil,
  &MAI_Snake,
  &MAI_Counselor,
  &MAI_Mega,
  &MAI_Diablo,
  &MAI_Lazurus,
  &MAI_Lazhelp,
  &MAI_Lachdanan,
  &MAI_Warlord
};

void InitMonsterTRN(int monst, BOOL special)
{
	int i, a, d;
	BYTE *t;

	/// ASSERT: assert((DWORD)monst < MAX_LVLMTYPES);

	t = Monsters[monst].trans_file;
	for(i = 256; i != 0; i--) {
		if(*t == 255) {
			*t = 0;
		}
		t++;
	}

	if(special) {
		a = 6;
	} else {
		a = 5;
	}
	for(i = 0; i < a; i++) {
		if(i == 1 && Monsters[monst].mtype >= MT_COUNSLR && Monsters[monst].mtype <= MT_ADVOCATE) {
			continue;
		}
		for(d = 0; d < 8; d++) {
			Cl2ApplyTrans(Monsters[monst].Anims[i].Frames[d], Monsters[monst].trans_file, Monsters[monst].Anims[i].Rate);
		}
	}
}

void InitLevelMonsters()
{
	int i;

	nummtypes = 0;
	monstimgtot = 0;
	MissileFileFlag = 0;

	for(i = 0; i < 16; i++) {
		Monsters[i].mPlaceFlags = 0;
	}

	ClrAllMonsters();
	nummonsters = 0;
	totalmonsters = MAXMONSTERS;

	for(i = 0; i < MAXMONSTERS; i++) {
		monstactive[i] = i;
	}

	uniquetrans = 0;
}

int AddMonsterType(int type, int placeflag)
{
	int i;
	BOOL done;

	done = FALSE;
	/// ASSERT: assert((DWORD)nummtypes <= MAX_LVLMTYPES);
	for(i = 0; i < nummtypes && !done; i++) {
		done = Monsters[i].mtype == type;
	}

	i--;
	if(!done) {
		i = nummtypes++;
		Monsters[i].mtype = type;
		monstimgtot += monsterdata[type].mType;
		InitMonsterGFX(i);
		InitMonsterSND(i);
	}

	Monsters[i].mPlaceFlags |= placeflag;
	return i;
}

void GetLevelMTypes()
{
	int i, minl, maxl, mt, nt, numskeltypes, rv;
	int typelist[MAXMONSTERS];
	int skeltypes[111];
	char mamask;

	mamask = 3;
	AddMonsterType(MT_GOLEM, 2);

	if(currlevel == 16) {
		AddMonsterType(MT_ADVOCATE, 1);
		AddMonsterType(MT_RBLACK, 1);
		AddMonsterType(MT_DIABLO, 2);
		return;
	}

	if(!setlevel) {
		if(QuestStatus(Q_BUTCHER)) {
			AddMonsterType(MT_CLEAVER, 2);
		}
		if(QuestStatus(Q_GARBUD)) {
			AddMonsterType(UniqMonst[0].mtype, 4);
		}
		if(QuestStatus(Q_ZHAR)) {
			AddMonsterType(UniqMonst[2].mtype, 4);
		}
		if(QuestStatus(Q_LTBANNER)) {
			AddMonsterType(UniqMonst[3].mtype, 4);
		}
		if(QuestStatus(Q_VEIL)) {
			AddMonsterType(UniqMonst[7].mtype, 4);
		}
		if(QuestStatus(Q_WARLORD)) {
			AddMonsterType(UniqMonst[8].mtype, 4);
		}
		if(gbMaxPlayers != 1 && currlevel == quests[Q_SKELKING]._qlevel) {
			AddMonsterType(MT_SKING, 4);
			numskeltypes = 0;
			for(i = 8; i <= 27; i++) {
				if(IsSkel(i)) {
					minl = 15 * monsterdata[i].mMinDLvl / 30 + 1;
					maxl = 15 * monsterdata[i].mMaxDLvl / 30 + 1;
					if(currlevel >= minl && currlevel <= maxl && MonstAvailTbl[i] & mamask) {
						skeltypes[numskeltypes++] = i;
					}
				}
			}
			AddMonsterType(skeltypes[random(88, numskeltypes)], 1);
		}
		nt = 0;
		for(i = 0; i < 111; i++) {
			minl = 15 * monsterdata[i].mMinDLvl / 30 + 1;
			maxl = 15 * monsterdata[i].mMaxDLvl / 30 + 1;
			if(currlevel >= minl && currlevel <= maxl && MonstAvailTbl[i] & mamask) {
				typelist[nt++] = i;
			}
		}
		if(monstdebug) {
			for (i = 0; i < debugmonsttypes; i++) {
				AddMonsterType(DebugMonsters[i], 1);
			}
		} else {
			while(nt > 0 && nummtypes < 16 && monstimgtot < 4000) {
				i = 0;
				while(i < nt) {
					mt = typelist[i];
					if(monsterdata[mt].mType > 4000 - monstimgtot) {
						nt--;
						typelist[i] = typelist[nt];
					} else {
						i++;
					}
				}
				if(nt != 0) {
					rv = random(88, nt);
					mt = typelist[rv];
					AddMonsterType(mt, 1);
					nt--;
					typelist[rv] = typelist[nt];
				}
			}
		}
	} else {
		if(setlvlnum == SL_SKELKING) {
			AddMonsterType(MT_SKING, 4);
		}
	}
}

void InitMonsterGFX(int monst)
{
	int i, anim, mtype;
	char strBuff[256];
	BYTE *p;

	/// ASSERT: assert((DWORD)monst < MAX_LVLMTYPES);
	mtype = Monsters[monst].mtype;

	for(anim = 0; anim < 6; anim++) {
		if((animletter[anim] != 's' || monsterdata[mtype].has_special) && monsterdata[mtype].Frames[anim] > 0) {
			sprintf(strBuff, monsterdata[mtype].GraphicType, animletter[anim]);
			/// ASSERT: assert(! Monsters[monst].Anims[anim].CMem);
			Monsters[monst].Anims[anim].CMem = DiabLoad(strBuff, NULL, 'MONS');
			p = Monsters[monst].Anims[anim].CMem;
			if(Monsters[monst].mtype == MT_GOLEM && (animletter[anim] == 's' || animletter[anim] == 'd')) {
				for(i = 0; i < 8; i++) {
					Monsters[monst].Anims[anim].Frames[i] = p;
				}
			} else {
				for(i = 0; i < 8; i++) {
					Monsters[monst].Anims[anim].Frames[i] = &p[((DWORD *)p)[i]];
				}
			}
		}
		Monsters[monst].Anims[anim].Rate = monsterdata[mtype].Frames[anim];
		Monsters[monst].Anims[anim].Delay = monsterdata[mtype].Rate[anim];
	}

	Monsters[monst].flags_1 = monsterdata[mtype].flags;
	Monsters[monst].flags_2 = (monsterdata[mtype].flags - 64) >> 1;
	Monsters[monst].mMinHP = monsterdata[mtype].mMinHP;
	Monsters[monst].mMaxHP = monsterdata[mtype].mMaxHP;
	Monsters[monst].has_special = monsterdata[mtype].has_special;
	Monsters[monst].mAFNum = monsterdata[mtype].mAFNum;
	Monsters[monst].MData = &monsterdata[mtype];

	if(monsterdata[mtype].has_trans) {
		Monsters[monst].trans_file = DiabLoad(monsterdata[mtype].TransFile, NULL, 'MONS');
		InitMonsterTRN(monst, monsterdata[mtype].has_special);
		MemFreeDbg(Monsters[monst].trans_file);
	}

	if(mtype >= MT_NMAGMA && mtype <= MT_WMAGMA && !(MissileFileFlag & 1)) {
		MissileFileFlag |= 1;
		LoadMissileGFX(MFILE_MAGBALL);
	}
	if(mtype >= MT_STORM && mtype <= MT_MAEL && !(MissileFileFlag & 2)) {
		MissileFileFlag |= 2;
		LoadMissileGFX(MFILE_THINLGHT);
	}
	if(mtype == MT_SUCCUBUS && !(MissileFileFlag & 4)) {
		MissileFileFlag |= 4;
		LoadMissileGFX(MFILE_FLARE);
		LoadMissileGFX(MFILE_FLAREEXP);
	}
	if(mtype == MT_SNOWWICH && !(MissileFileFlag & 0x20)) {
		MissileFileFlag |= 0x20;
		LoadMissileGFX(MFILE_SCUBMISB);
		LoadMissileGFX(MFILE_SCBSEXPB);
	}
	if(mtype == MT_HLSPWN && !(MissileFileFlag & 0x40)) {
		MissileFileFlag |= 0x40;
		LoadMissileGFX(MFILE_SCUBMISD);
		LoadMissileGFX(MFILE_SCBSEXPD);
	}
	if(mtype == MT_SOLBRNR && !(MissileFileFlag & 0x80)) {
		MissileFileFlag |= 0x80;
		LoadMissileGFX(MFILE_SCUBMISC);
		LoadMissileGFX(MFILE_SCBSEXPC);
	}
	if(mtype >= MT_INCIN && mtype <= MT_HELLBURN && !(MissileFileFlag & 8)) {
		MissileFileFlag |= 8;
		LoadMissileGFX(MFILE_KRULL);
	}
	if(mtype >= MT_NACID && mtype <= MT_XACID && !(MissileFileFlag & 0x10)) {
		MissileFileFlag |= 0x10;
		LoadMissileGFX(MFILE_ACIDBF);
		LoadMissileGFX(MFILE_ACIDSPLA);
		LoadMissileGFX(MFILE_ACIDPUD);
	}
	if(mtype == MT_DIABLO) {
		LoadMissileGFX(MFILE_FIREPLAR);
	}
}

void ClearMVars(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMONSTERS);

	monster[i]._mVar1 = 0;
	monster[i]._mVar2 = 0;
	monster[i]._mVar3 = 0;
	monster[i]._mVar4 = 0;
	monster[i]._mVar5 = 0;
	monster[i]._mVar6 = 0;
	monster[i]._mVar7 = 0;
	monster[i]._mVar8 = 0;
}

void InitMonster(int i, int rd, int mtype, int x, int y)
{
	CMonster *monst;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert((DWORD)mtype < MAX_LVLMTYPES);
	monst = &Monsters[mtype];
	/// ASSERT: assert(monst->MData != NULL);

	monster[i]._mdir = rd;
	monster[i]._mx = x;
	monster[i]._my = y;
	monster[i]._mfutx = x;
	monster[i]._mfuty = y;
	monster[i]._moldx = x;
	monster[i]._moldy = y;
	monster[i]._mMTidx = mtype;
	monster[i]._mmode = MM_STAND;
	monster[i].mName = monst->MData->mName;
	monster[i].MType = monst;
	monster[i].MData = monst->MData;
	monster[i]._mAnimData = monst->Anims[0].Frames[rd];
	monster[i]._mAnimDelay = monst->Anims[0].Delay;
	monster[i]._mAnimCnt = random(88, monster[i]._mAnimDelay - 1);
	monster[i]._mAnimLen = monst->Anims[0].Rate;
	monster[i]._mAnimFrame = random(88, monster[i]._mAnimLen - 1) + 1;
	if(monst->mtype == MT_DIABLO) {
		monster[i]._mmaxhp = (random(88, 1) + 1666) << 6;
	} else {
		monster[i]._mmaxhp = (random(88, monst->mMaxHP - monst->mMinHP + 1) + monst->mMinHP) << 6;
	}
	if(gbMaxPlayers == 1) {
		monster[i]._mmaxhp >>= 1;
		if(monster[i]._mmaxhp < 64) {
			monster[i]._mmaxhp = 64;
		}
	}
	monster[i]._mhitpoints = monster[i]._mmaxhp;
	monster[i]._mAi = monst->MData->mAi;
	monster[i]._mint = monst->MData->mInt;
	monster[i]._mgoal = 1;
	monster[i]._mgoalvar1 = 0;
	monster[i]._mgoalvar2 = 0;
	monster[i]._mgoalvar3 = 0;
	monster[i].field_18 = 0;
	monster[i]._pathcount = 0;
	monster[i]._mDelFlag = 0;
	monster[i]._uniqtype = 0;
	monster[i]._msquelch = 0;
	monster[i]._mRndSeed = GetRndSeed();
	monster[i]._mAISeed = GetRndSeed();
	monster[i].mWhoHit = 0;
	monster[i].mLevel = monst->MData->mLevel;
	monster[i].mExp = monst->MData->mExp;
	monster[i].mHit = monst->MData->mHit;
	monster[i].mMinDamage = monst->MData->mMinDamage;
	monster[i].mMaxDamage = monst->MData->mMaxDamage;
	monster[i].mHit2 = monst->MData->mHit2;
	monster[i].mMinDamage2 = monst->MData->mMinDamage2;
	monster[i].mMaxDamage2 = monst->MData->mMaxDamage2;
	monster[i].mArmorClass = monst->MData->mArmorClass;
	monster[i].mMagicRes = monst->MData->mMagicRes;
	monster[i].leader = 0;
	monster[i].leaderflag = 0;
	monster[i]._mFlags = monst->MData->mFlags;
	monster[i].mtalkmsg = 0;

	if(monster[i]._mAi == AI_GARG) {
		monster[i]._mAnimData = monst->Anims[5].Frames[rd];
		monster[i]._mAnimFrame = 1;
		monster[i]._mFlags |= 4;
		monster[i]._mmode = MM_SATTACK;
	}

	if(gnDifficulty == DIFF_NIGHTMARE) {
		monster[i]._mmaxhp = 3 * monster[i]._mmaxhp + 64;
		monster[i]._mhitpoints = monster[i]._mmaxhp;
		monster[i].mLevel += 15;
		monster[i].mExp = 2 * (monster[i].mExp + 1000);
		monster[i].mHit += 85;
		monster[i].mMinDamage = 2 * (monster[i].mMinDamage + 2);
		monster[i].mMaxDamage = 2 * (monster[i].mMaxDamage + 2);
		monster[i].mHit2 += 85;
		monster[i].mMinDamage2 = 2 * (monster[i].mMinDamage2 + 2);
		monster[i].mMaxDamage2 = 2 * (monster[i].mMaxDamage2 + 2);
		monster[i].mArmorClass += 50;
	}
	if(gnDifficulty == DIFF_HELL) {
		monster[i]._mmaxhp = 4 * monster[i]._mmaxhp + 192;
		monster[i]._mhitpoints = monster[i]._mmaxhp;
		monster[i].mLevel += 30;
		monster[i].mExp = 4 * (monster[i].mExp + 1000);
		monster[i].mHit += 120;
		monster[i].mMinDamage = 4 * monster[i].mMinDamage + 6;
		monster[i].mMaxDamage = 4 * monster[i].mMaxDamage + 6;
		monster[i].mHit2 += 120;
		monster[i].mMinDamage2 = 4 * monster[i].mMinDamage2 + 6;
		monster[i].mMaxDamage2 = 4 * monster[i].mMaxDamage2 + 6;
		monster[i].mArmorClass += 80;
		monster[i].mMagicRes = monst->MData->mMagicRes2;
	}
}

void ClrAllMonsters()
{
	int i;
	/* MonsterStruct *Monst; */

	for(i = 0; i < MAXMONSTERS; i++) {
		ClearMVars(i);
		monster[i].mName = "Invalid Monster";
		monster[i]._mgoal = 0;
		monster[i]._mmode = 0;
		monster[i]._mVar1 = 0;
		monster[i]._mVar2 = 0;
		monster[i]._mx = 0;
		monster[i]._my = 0;
		monster[i]._mfutx = 0;
		monster[i]._mfuty = 0;
		monster[i]._moldx = 0;
		monster[i]._moldy = 0;
		monster[i]._mdir = random(89, 8);
		monster[i]._mxvel = 0;
		monster[i]._myvel = 0;
		monster[i]._mAnimData = 0;
		monster[i]._mAnimDelay = 0;
		monster[i]._mAnimCnt = 0;
		monster[i]._mAnimLen = 0;
		monster[i]._mAnimFrame = 0;
		monster[i]._mFlags = 0;
		monster[i]._mDelFlag = 0;
		monster[i]._menemy = random(89, gbActivePlayers);
		monster[i]._menemyx = plr[monster[i]._menemy]._px;
		monster[i]._menemyy = plr[monster[i]._menemy]._py;
	}
}

BOOL MonstPlace(int xp, int yp)
{
	if(xp < 0 || xp >= MAXDUNX || yp < 0 || yp >= MAXDUNY) {
		return FALSE;
	}
	if(dMonster[xp][yp] != 0) {
		return FALSE;
	}
	if(dPlayer[xp][yp] != 0) {
		return FALSE;
	}
	if(dFlags[xp][yp] & 2) {
		return FALSE;
	}
	if(dFlags[xp][yp] & 8) {
		return FALSE;
	}
	if(SolidLoc(xp, yp)) {
		return FALSE;
	}

	return TRUE;
}

void PlaceMonster(int i, int mtype, int x, int y)
{
	int rd;

	dMonster[x][y] = i + 1;
	rd = random(90, 8);
	InitMonster(i, rd, mtype, x, y);
}

void PlaceUniqueMonst(int uniqindex, int miniontype, int packsize)
{
	UniqMonstStruct *Uniq;
	MonsterStruct *Monst;
	int xp, yp, x, y, count, count2, uniqtype, i;
	BOOL done, zharflag;
	char filestr[64];

	/// ASSERT: assert((DWORD)nummonsters < MAXMONSTERS);
	Uniq = &UniqMonst[uniqindex];
	Monst = &monster[nummonsters];
	count = 0;

	if((uniquetrans << 8) + 4864 >= LIGHTSIZE) {
		return;
	}

	/// ASSERT: assert((DWORD)nummtypes <= MAX_LVLMTYPES);
	for(uniqtype = 0; uniqtype < nummtypes; uniqtype++) {
		if(Monsters[uniqtype].mtype == UniqMonst[uniqindex].mtype) {
			break;
		}
	}
	/// ASSERT: assert(uniqtype < nummtypes);

	while(1) {
		x = random(91, 80) + 16;
		y = random(91, 80) + 16;
		count2 = 0;
		for(xp = x - 3; xp < x + 3; xp++) {
			for(yp = y - 3; yp < y + 3; yp++) {
				if(yp >= 0 && yp < MAXDUNY && xp >= 0 && xp < MAXDUNX && MonstPlace(xp, yp)) {
					count2++;
				}
			}
		}
		if(count2 < 9) {
			count++;
			if(count < 1000) {
				continue;
			}
		}
		if(MonstPlace(x, y)) {
			break;
		}
	}
	if(uniqindex == 3) {
		x = 2 * setpc_x + 24;
		y = 2 * setpc_y + 28;
	}
	if(uniqindex == 8) {
		x = 2 * setpc_x + 22;
		y = 2 * setpc_y + 23;
	}
	if(uniqindex == 2) {
		zharflag = TRUE;
		for(i = 0; i < themeCount; i++) {
			if(i == zharlib && zharflag == TRUE) {
				zharflag = FALSE;
				x = 2 * themeLoc[i].x + 20;
				y = 2 * themeLoc[i].y + 20;
			}
		}
	}
	if(gbMaxPlayers == 1) {
		if(uniqindex == 4) {
			x = 32;
			y = 46;
		}
		if(uniqindex == 5) {
			x = 40;
			y = 45;
		}
		if(uniqindex == 6) {
			x = 38;
			y = 49;
		}
		if(uniqindex == 1) {
			x = 35;
			y = 47;
		}
	} else {
		if(uniqindex == 4) {
			x = 2 * setpc_x + 19;
			y = 2 * setpc_y + 22;
		}
		if(uniqindex == 5) {
			x = 2 * setpc_x + 21;
			y = 2 * setpc_y + 19;
		}
		if(uniqindex == 6) {
			x = 2 * setpc_x + 21;
			y = 2 * setpc_y + 25;
		}
	}
	if(uniqindex == 9) {
		done = FALSE;
		for(y = 0; y < MAXDUNY && !done; y++) {
			for(x = 0; x < MAXDUNX && !done; x++) {
				done = dPiece[x][y] == 367;
			}
		}
	}

	PlaceMonster(nummonsters, uniqtype, x, y);

	Monst->_uniqtype = uniqindex + 1;
	if(Uniq->mlevel) {
		Monst->mLevel = 2 * Uniq->mlevel;
	} else {
		Monst->mLevel += 5;
	}
	Monst->mExp *= 2;
	Monst->mName = Uniq->mName;
	Monst->_mmaxhp = Uniq->mmaxhp << 6;
	if(gbMaxPlayers == 1) {
		Monst->_mmaxhp >>= 1;
		if(Monst->_mmaxhp < 64) {
			Monst->_mmaxhp = 64;
		}
	}
	Monst->_mhitpoints = Monst->_mmaxhp;
	Monst->_mAi = Uniq->mAi;
	Monst->_mint = Uniq->mint;
	Monst->mMinDamage = Uniq->mMinDamage;
	Monst->mMaxDamage = Uniq->mMaxDamage;
	Monst->mMinDamage2 = Uniq->mMinDamage;
	Monst->mMaxDamage2 = Uniq->mMaxDamage;
	Monst->mMagicRes = Uniq->mMagicRes;
	Monst->mtalkmsg = Uniq->mtalkmsg;
	Monst->mlid = AddLight(Monst->_mx, Monst->_my, 3);

	if(gbMaxPlayers == 1) {
		if(Monst->mtalkmsg != 0) {
			Monst->_mgoal = 6;
		}
	} else {
		if(Monst->_mAi == AI_LAZHELP) {
			Monst->mtalkmsg = 0;
		}
		if(Monst->_mAi == AI_LAZURUS && quests[Q_BETRAYER]._qvar1 > 3) {
			Monst->_mgoal = 1;
		} else if(Monst->mtalkmsg != 0) {
			Monst->_mgoal = 6;
		}
	}

	if(gnDifficulty == DIFF_NIGHTMARE) {
		Monst->_mmaxhp = 3 * Monst->_mmaxhp + 64;
		Monst->_mhitpoints = Monst->_mmaxhp;
		Monst->mLevel += 15;
		Monst->mExp = 2 * (Monst->mExp + 1000);
		Monst->mMinDamage = 2 * (Monst->mMinDamage + 2);
		Monst->mMaxDamage = 2 * (Monst->mMaxDamage + 2);
		Monst->mMinDamage2 = 2 * (Monst->mMinDamage2 + 2);
		Monst->mMaxDamage2 = 2 * (Monst->mMaxDamage2 + 2);
	}
	if(gnDifficulty == DIFF_HELL) {
		Monst->_mmaxhp = 4 * Monst->_mmaxhp + 192;
		Monst->_mhitpoints = Monst->_mmaxhp;
		Monst->mLevel += 30;
		Monst->mExp = 4 * (Monst->mExp + 1000);
		Monst->mMinDamage = 4 * Monst->mMinDamage + 6;
		Monst->mMaxDamage = 4 * Monst->mMaxDamage + 6;
		Monst->mMinDamage2 = 4 * Monst->mMinDamage2 + 6;
		Monst->mMaxDamage2 = 4 * Monst->mMaxDamage2 + 6;
	}

	sprintf(filestr, "Monsters\\Monsters\\%s.TRN", Uniq->mTrnName);
	/// ASSERT: assert((uniquetrans << 8) + 4864 < LIGHTSIZE);
	LoadFileWithMem(filestr, &pLightTbl[(uniquetrans << 8) + 4864]);
	Monst->_uniqtrans = uniquetrans;
	uniquetrans++;

	if(Uniq->mUnqAttr & 4) {
		Monst->mHit = Uniq->mUnqVar1;
		Monst->mHit2 = Uniq->mUnqVar1;
	}
	if(Uniq->mUnqAttr & 8) {
		Monst->mArmorClass = Uniq->mUnqVar1;
	}

	nummonsters++;

	if(Uniq->mUnqAttr & 1) {
		PlaceGroup(miniontype, packsize, Uniq->mUnqAttr, nummonsters - 1);
	}

	if(Monst->_mAi != AI_GARG) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[Monst->_mdir];
		Monst->_mAnimFrame = random(88, Monst->_mAnimLen - 1) + 1;
		Monst->_mFlags &= ~4;
		Monst->_mmode = MM_STAND;
	}
}

void PlaceQuestMonsters()
{
	int skeltype;
	BYTE *setp;

	if(!setlevel) {
		if(QuestStatus(Q_BUTCHER)) {
			PlaceUniqueMonst(9, 0, 0);
		}
		if(currlevel == quests[Q_SKELKING]._qlevel && gbMaxPlayers != 1) {
			/// ASSERT: assert((DWORD)nummtypes <= MAX_LVLMTYPES);
			for(skeltype = 0; skeltype < nummtypes; skeltype++) {
				if(IsSkel(Monsters[skeltype].mtype)) {
					break;
				}
			}
			/// ASSERT: assert(skeltype < nummtypes);
			PlaceUniqueMonst(1, skeltype, 30);
		}
		if(QuestStatus(Q_LTBANNER)) {
			setp = DiabLoad("Levels\\L1Data\\Banner1.DUN", NULL, 'MONS');
			SetMapMonsters(setp, 2 * setpc_x, 2 * setpc_y);
			MemFreeDbg(setp);
		}
		if(QuestStatus(Q_BLOOD)) {
			setp = DiabLoad("Levels\\L2Data\\Blood2.DUN", NULL, 'MONS');
			SetMapMonsters(setp, 2 * setpc_x, 2 * setpc_y);
			MemFreeDbg(setp);
		}
		if(QuestStatus(Q_BLIND)) {
			setp = DiabLoad("Levels\\L2Data\\Blind2.DUN", NULL, 'MONS');
			SetMapMonsters(setp, 2 * setpc_x, 2 * setpc_y);
			MemFreeDbg(setp);
		}
		if(QuestStatus(Q_ANVIL)) {
			setp = DiabLoad("Levels\\L3Data\\Anvil.DUN", NULL, 'MONS');
			SetMapMonsters(setp, 2 * setpc_x + 2, 2 * setpc_y + 2);
			MemFreeDbg(setp);
		}
		if(QuestStatus(Q_WARLORD)) {
			setp = DiabLoad("Levels\\L4Data\\Warlord.DUN", NULL, 'MONS');
			SetMapMonsters(setp, 2 * setpc_x, 2 * setpc_y);
			MemFreeDbg(setp);
			AddMonsterType(UniqMonst[8].mtype, 1);
		}
		if(QuestStatus(Q_VEIL)) {
			AddMonsterType(UniqMonst[7].mtype, 1);
		}
		if(QuestStatus(Q_ZHAR) && zharlib == -1) {
			quests[Q_ZHAR]._qactive = 0;
		}
		if(currlevel == quests[Q_BETRAYER]._qlevel && gbMaxPlayers != 1) {
			AddMonsterType(UniqMonst[4].mtype, 4);
			AddMonsterType(UniqMonst[5].mtype, 4);
			PlaceUniqueMonst(4, 0, 0);
			PlaceUniqueMonst(5, 0, 0);
			PlaceUniqueMonst(6, 0, 0);
			setp = DiabLoad("Levels\\L4Data\\Vile1.DUN", NULL, 'MONS');
			SetMapMonsters(setp, 2 * setpc_x, 2 * setpc_y);
			MemFreeDbg(setp);
		}
	} else {
		if(setlvlnum == SL_SKELKING) {
			PlaceUniqueMonst(1, 0, 0);
		}
	}
}

void PlaceGroup(int mtype, int num, int leaderf, int leader)
{
	int xp, yp, x1, y1, j, placed, try1, try2, rd;

	placed = 0;
	try1 = 0;
	/// ASSERT: assert((DWORD)leader < MAXMONSTERS);
	do {
		while(placed != 0) {
			nummonsters--;
			placed--;
			/// ASSERT: assert((DWORD)nummonsters < MAXMONSTERS);
			/// ASSERT: assert((DWORD)monster[nummonsters]._mx < MAXDUNX);
			/// ASSERT: assert((DWORD)monster[nummonsters]._my < MAXDUNY);
			dMonster[monster[nummonsters]._mx][monster[nummonsters]._my] = 0;
		}
		if(leaderf & 1) {
			rd = random(92, 8);
			x1 = xp = monster[leader]._mx + offset_x[rd];
			y1 = yp = monster[leader]._my + offset_y[rd];
		} else {
			do {
				x1 = xp = random(93, 80) + 16;
				y1 = yp = random(93, 80) + 16;
			} while(!MonstPlace(xp, yp));
		}
		if(num + nummonsters > totalmonsters) {
			num = totalmonsters - nummonsters;
		}
		j = 0;
		try2 = 0;
		while(j < num && try2 < 100) {
			if(MonstPlace(xp, yp)
			&& dTransVal[xp][yp] == dTransVal[x1][y1]
			&& (!(leaderf & 2) || abs(xp - x1) < 4 && abs(yp - y1) < 4)) {
				PlaceMonster(nummonsters, mtype, xp, yp);
				if(leaderf & 1) {
					monster[nummonsters]._mmaxhp *= 2;
					monster[nummonsters]._mhitpoints = monster[nummonsters]._mmaxhp;
					monster[nummonsters]._mint = monster[leader]._mint;
					if(leaderf & 2) {
						monster[nummonsters].leader = leader;
						monster[nummonsters].leaderflag = 1;
						monster[nummonsters]._mAi = monster[leader]._mAi;
					}
					if(monster[nummonsters]._mAi != AI_GARG) {
						monster[nummonsters]._mAnimData = monster[nummonsters].MType->Anims[0].Frames[monster[nummonsters]._mdir];
						monster[nummonsters]._mAnimFrame = random(88, monster[nummonsters]._mAnimLen - 1) + 1;
						monster[nummonsters]._mFlags &= ~4;
						monster[nummonsters]._mmode = MM_STAND;
					}
				}
				nummonsters++;
				placed++;
				j++;
			} else {
				try2++;
			}
			xp += offset_x[random(94, 8)];
			yp += offset_x[random(94, 8)]; /// BUGFIX: should use 'offset_y'
		}
		if(placed >= num) {
			break;
		}
		try1++;
	} while(try1 < 10);

	if(leaderf & 2) {
		monster[leader].packsize = placed;
	}
}

void LoadDiabMonsts()
{
	BYTE *lpSetPiece;

	lpSetPiece = DiabLoad("Levels\\L4Data\\diab1.DUN", NULL, 'STPC');
	SetMapMonsters(lpSetPiece, 2 * diabquad1x, 2 * diabquad1y);
	MemFreeDbg(lpSetPiece);
	lpSetPiece = DiabLoad("Levels\\L4Data\\diab2a.DUN", NULL, 'STPC');
	SetMapMonsters(lpSetPiece, 2 * diabquad2x, 2 * diabquad2y);
	MemFreeDbg(lpSetPiece);
	lpSetPiece = DiabLoad("Levels\\L4Data\\diab3a.DUN", NULL, 'STPC');
	SetMapMonsters(lpSetPiece, 2 * diabquad3x, 2 * diabquad3y);
	MemFreeDbg(lpSetPiece);
	lpSetPiece = DiabLoad("Levels\\L4Data\\diab4a.DUN", NULL, 'STPC');
	SetMapMonsters(lpSetPiece, 2 * diabquad4x, 2 * diabquad4y);
	MemFreeDbg(lpSetPiece);
}

void InitMonsters()
{
	int i, j, mtype, na, nt, s, t, numscattypes, numplacemonsters;
	int scattertypes[111];
	long fv;

	numscattypes = 0;

	if(gbMaxPlayers != 1) {
		CheckDungeonClear();
	}
	if(!setlevel) {
		AddMonster(1, 0, 0, 0, 0);
		AddMonster(1, 0, 0, 0, 0);
		AddMonster(1, 0, 0, 0, 0);
		AddMonster(1, 0, 0, 0, 0);
	}
	if(!setlevel && currlevel == 16) {
		LoadDiabMonsts();
	}

	nt = numtrigs;
	if(currlevel == 15) {
		nt = 1;
	}

	for(i = 0; i < nt; i++) {
		for(s = -2; s < 2; s++) {
			for(t = -2; t < 2; t++) {
				DoVision(s + trigs[i]._tx, t + trigs[i]._ty, 15, 0, 0);
			}
		}
	}

	PlaceQuestMonsters();

	if(!setlevel) {
		PlaceUniques();
		fv = 0;
		for(i = 16; i < 96; i++) {
			for(j = 16; j < 96; j++) {
				if(!SolidLoc(i, j)) {
					fv++;
				}
			}
		}
		na = fv / 30;
		if(gbMaxPlayers != 1) {
			na += na >> 1;
		}
		if(na + nummonsters > 190) {
			na = 190 - nummonsters;
		}
		totalmonsters = na + nummonsters;
		/// ASSERT: assert((DWORD)nummtypes <= MAX_LVLMTYPES);
		for(i = 0; i < nummtypes; i++) {
			if(Monsters[i].mPlaceFlags & 1) {
				scattertypes[numscattypes++] = i;
			}
		}
		while(nummonsters < totalmonsters) {
			mtype = scattertypes[random(95, numscattypes)];
			if(currlevel != 1 && random(95, 2) != 0) {
				if(currlevel == 2) {
					numplacemonsters = random(95, 2) + 2;
				} else {
					numplacemonsters = random(95, 3) + 3;
				}
			} else {
				numplacemonsters = 1;
			}
			PlaceGroup(mtype, numplacemonsters, 0, 0);
		}
	}

	for(i = 0; i < nt; i++) {
		for(s = -2; s < 2; s++) {
			for(t = -2; t < 2; t++) {
				DoUnVision(s + trigs[i]._tx, t + trigs[i]._ty, 15);
			}
		}
	}
}

void PlaceUniques()
{
	int u, mt;
	BOOL done;

	for(u = 0; UniqMonst[u].mtype != -1; u++) {
#ifdef _DEBUG
		if(UniqMonst[u].mlevel == currlevel || UniqMonst[u].mlevel != 0 && debug_mode_key_d) {
#else
		if(UniqMonst[u].mlevel == currlevel) {
#endif
			done = FALSE;
			/// ASSERT: assert((DWORD)nummtypes <= MAX_LVLMTYPES);
			for(mt = 0; mt < nummtypes && !done; mt++) {
				done = Monsters[mt].mtype == UniqMonst[u].mtype;
			}
			mt--;
			if(u == 0 && quests[Q_GARBUD]._qactive == 0) {
				done = FALSE;
			}
			if(u == 2 && quests[Q_ZHAR]._qactive == 0) {
				done = FALSE;
			}
			if(u == 3 && quests[Q_LTBANNER]._qactive == 0) {
				done = FALSE;
			}
			if(u == 7 && quests[Q_VEIL]._qactive == 0) {
				done = FALSE;
			}
			if(u == 8 && quests[Q_WARLORD]._qactive == 0) {
				done = FALSE;
			}
			if(done) {
				PlaceUniqueMonst(u, mt, 8);
			}
		}
	}
}

void SetMapMonsters(BYTE *pMap, int startx, int starty)
{
	int i, j, mt, mx, my, mtype;
	WORD rw, rh;
	WORD *lm;

	AddMonsterType(MT_GOLEM, 2);
	AddMonster(1, 0, 0, 0, 0);
	AddMonster(1, 0, 0, 0, 0);
	AddMonster(1, 0, 0, 0, 0);
	AddMonster(1, 0, 0, 0, 0);

	if(setlevel && setlvlnum == SL_VILEBETRAYER) {
		AddMonsterType(UniqMonst[4].mtype, 4);
		AddMonsterType(UniqMonst[5].mtype, 4);
		AddMonsterType(UniqMonst[6].mtype, 4);
		PlaceUniqueMonst(4, 0, 0);
		PlaceUniqueMonst(5, 0, 0);
		PlaceUniqueMonst(6, 0, 0);
	}

	lm = (WORD *)pMap;
	rw = *lm;
	lm++;
	rh = *lm;
	lm++;
	lm += rw * rh;
	rw <<= 1;
	rh <<= 1;
	lm += rw * rh;

	for(j = 0; j < rh; j++) {
		for(i = 0; i < rw; i++) {
			if(*lm != 0) {
				mt = *lm;
				mt = MonstConvTbl[mt - 1];
				mtype = AddMonsterType(mt, 2);
				mx = i + startx + 16;
				my = j + starty + 16;
				PlaceMonster(nummonsters++, mtype, mx, my);
			}
			lm++;
		}
	}
}

void DeleteMonster(int i)
{
	int temp;

	/// ASSERT: assert((DWORD)(nummonsters-1) < MAXMONSTERS);
	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	nummonsters--;
	temp = monstactive[nummonsters];
	monstactive[nummonsters] = monstactive[i];
	monstactive[i] = temp;
}

int AddMonster(int x, int y, int dir, int mtype, BOOL InMap)
{
	int i;

	if(nummonsters >= MAXMONSTERS) {
		return -1;
	}

	i = monstactive[nummonsters++];

	if(InMap) {
		dMonster[x][y] = i + 1;
	}

	InitMonster(i, dir, mtype, x, y);

	return i;
}

void NewMonsterAnim(int i, AnimStruct &anim, int md)
{
	MonsterStruct *Monst;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert((DWORD)md < 8);
	Monst = &monster[i];
	Monst->_mAnimData = anim.Frames[md];
	Monst->_mAnimLen = anim.Rate;
	Monst->_mAnimFrame = 1;
	Monst->_mAnimCnt = 0;
	Monst->_mAnimDelay = anim.Delay;
	Monst->_mdir = md;
	Monst->_mFlags &= ~6;
}

BOOL M_Ranged(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	return monster[i]._mAi == AI_SKELBOW
		|| monster[i]._mAi == AI_GOATBOW
		|| monster[i]._mAi == AI_SUCC
		|| monster[i]._mAi == AI_LAZHELP;
}

BOOL M_Talker(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	return monster[i]._mAi == AI_LAZURUS
		|| monster[i]._mAi == AI_WARLORD
		|| monster[i]._mAi == AI_GARBUD
		|| monster[i]._mAi == AI_ZHAR
		|| monster[i]._mAi == AI_SNOTSPIL
		|| monster[i]._mAi == AI_LACHDAN
		|| monster[i]._mAi == AI_LAZHELP;
}

void M_Enemy(int i)
{
	int j, mi, pnum, closest, dist, bestdist;
	BYTE enemyx, enemyy;
	BOOL sameroom, bestsameroom;
	MonsterStruct *Monst;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	Monst = &monster[i];

	closest = -1;
	bestdist = -1;
	bestsameroom = FALSE;

	if(!(Monst->_mFlags & 0x20)) {
		for(pnum = 0; pnum < MAX_PLRS; pnum++) {
			if(!plr[pnum].plractive || currlevel != plr[pnum].plrlevel || plr[pnum]._pLvlChanging) {
				continue;
			}
			if(plr[pnum]._pHitPoints == 0 && gbMaxPlayers != 1) {
				continue;
			}
			sameroom = dTransVal[Monst->_mx][Monst->_my] == dTransVal[plr[pnum].WorldX][plr[pnum].WorldY];
			if(abs(Monst->_mx - plr[pnum].WorldX) > abs(Monst->_my - plr[pnum].WorldY)) {
				dist = abs(Monst->_mx - plr[pnum].WorldX);
			} else {
				dist = abs(Monst->_my - plr[pnum].WorldY);
			}
			if(sameroom && !bestsameroom || (sameroom || !bestsameroom) && dist < bestdist || closest == -1) {
				Monst->_mFlags &= ~0x10;
				closest = pnum;
				enemyx = plr[pnum]._px;
				enemyy = plr[pnum]._py;
				bestdist = dist;
				bestsameroom = sameroom;
			}
		}
	}

	/// ASSERT: assert((DWORD)nummonsters <= MAXMONSTERS);
	for(j = 0; j < nummonsters; j++) {
		mi = monstactive[j];
		if(mi == i) {
			continue;
		}
		if(monster[mi]._mx == 1 && monster[mi]._my == 0) {
			continue;
		}
		if(M_Talker(mi) && monster[mi].mtalkmsg != 0) {
			continue;
		}
		if(!(Monst->_mFlags & 0x20) && (abs(monster[mi]._mx - Monst->_mx) >= 2 || abs(monster[mi]._my - Monst->_my) >= 2) && !M_Ranged(i)) {
			continue;
		}
		if(!(Monst->_mFlags & 0x20) && !(monster[mi]._mFlags & 0x20)) {
			continue;
		}
		sameroom = dTransVal[Monst->_mx][Monst->_my] == dTransVal[monster[mi]._mx][monster[mi]._my];
		if(abs(Monst->_mx - monster[mi]._mx) > abs(Monst->_my - monster[mi]._my)) {
			dist = abs(Monst->_mx - monster[mi]._mx);
		} else {
			dist = abs(Monst->_my - monster[mi]._my);
		}
		if(sameroom && !bestsameroom || (sameroom || !bestsameroom) && dist < bestdist || closest == -1) {
			Monst->_mFlags |= 0x10;
			closest = mi;
			enemyx = monster[mi]._mfutx;
			enemyy = monster[mi]._mfuty;
			bestdist = dist;
			bestsameroom = sameroom;
		}
	}

	if(closest != -1) {
		Monst->_mFlags &= ~0x400;
		Monst->_menemy = closest;
		Monst->_menemyx = enemyx;
		Monst->_menemyy = enemyy;
	} else {
		Monst->_mFlags |= 0x400;
	}
}

int M_GetDir(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	return GetDirection(monster[i]._mx, monster[i]._my, monster[i]._menemyx, monster[i]._menemyy);
}

void M_CheckEFlag(int i)
{
	int j, x, y, mt;
	MICROS *pMap;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	x = monster[i]._mx - 1;
	y = monster[i]._my + 1;
	mt = 0;

	pMap = &dpiece_defs_map_2[x][y];
	if(pMap < &dpiece_defs_map_2[0][0]) {
		monster[i]._meflag = 0;
		return;
	}

	for(j = 2; j < 10; j++) {
		mt |= pMap->mt[j];
	}

	if(dSpecial[x][y] | mt) {
		monster[i]._meflag = 1;
	} else {
		monster[i]._meflag = 0;
	}
}

void M_StartStand(int i, int md)
{
	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	ClearMVars(i);

	if(monster[i].MType->mtype == MT_GOLEM) {
		NewMonsterAnim(i, monster[i].MType->Anims[1], md);
	} else {
		NewMonsterAnim(i, monster[i].MType->Anims[0], md);
	}

	monster[i]._mVar1 = monster[i]._mmode;
	monster[i]._mVar2 = 0;
	monster[i]._mmode = MM_STAND;
	monster[i]._mxoff = 0;
	monster[i]._myoff = 0;
	monster[i]._mfutx = monster[i]._mx;
	monster[i]._mfuty = monster[i]._my;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	monster[i]._mdir = md;
	M_CheckEFlag(i);
	M_Enemy(i);
}

void M_StartDelay(int i, int len)
{
	if(len <= 0) {
		return;
	}

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	if(monster[i]._mAi != AI_LAZURUS) {
		monster[i]._mVar2 = len;
		monster[i]._mmode = MM_DELAY;
	}
}

void M_StartSpStand(int i, int md)
{
	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	NewMonsterAnim(i, monster[i].MType->Anims[5], md);
	monster[i]._mmode = MM_SPSTAND;
	monster[i]._mxoff = 0;
	monster[i]._myoff = 0;
	monster[i]._mfutx = monster[i]._mx;
	monster[i]._mfuty = monster[i]._my;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	monster[i]._mdir = md;
	M_CheckEFlag(i);
}

void M_StartWalk(int i, int xvel, int yvel, int xadd, int yadd, int EndDir)
{
	long fx, fy;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	fx = monster[i]._mx + xadd;
	fy = monster[i]._my + yadd;
	/// ASSERT: assert((DWORD)fx < MAXDUNX);
	/// ASSERT: assert((DWORD)fy < MAXDUNY);
	dMonster[fx][fy] = -(i + 1);
	monster[i]._mmode = MM_WALK;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	monster[i]._mfutx = fx;
	monster[i]._mfuty = fy;
	monster[i]._mxvel = xvel;
	monster[i]._myvel = yvel;
	monster[i]._mVar1 = xadd;
	monster[i]._mVar2 = yadd;
	monster[i]._mVar3 = EndDir;
	monster[i]._mdir = EndDir;
	NewMonsterAnim(i, monster[i].MType->Anims[1], EndDir);
	monster[i]._mVar6 = 0;
	monster[i]._mVar7 = 0;
	monster[i]._mVar8 = 0;
	M_CheckEFlag(i);
}

void M_StartWalk2(int i, int xvel, int yvel, int xoff, int yoff, int xadd, int yadd, int EndDir)
{
	long fx, fy;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	fx = monster[i]._mx + xadd;
	fy = monster[i]._my + yadd;
	/// ASSERT: assert((DWORD)fx < MAXDUNX);
	/// ASSERT: assert((DWORD)fy < MAXDUNY);
	/// ASSERT: assert((DWORD)monster[i]._mx < MAXDUNX);
	/// ASSERT: assert((DWORD)monster[i]._my < MAXDUNY);
	dMonster[monster[i]._mx][monster[i]._my] = -(i + 1);
	monster[i]._mVar1 = monster[i]._mx;
	monster[i]._mVar2 = monster[i]._my;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	monster[i]._mx = fx;
	monster[i]._my = fy;
	monster[i]._mfutx = fx;
	monster[i]._mfuty = fy;
	dMonster[fx][fy] = i + 1;
	if(monster[i]._uniqtype != 0) {
		ChangeLightXY(monster[i].mlid, monster[i]._mx, monster[i]._my);
	}
	monster[i]._mxoff = xoff;
	monster[i]._myoff = yoff;
	monster[i]._mmode = MM_WALK2;
	monster[i]._mxvel = xvel;
	monster[i]._myvel = yvel;
	monster[i]._mVar3 = EndDir;
	monster[i]._mdir = EndDir;
	NewMonsterAnim(i, monster[i].MType->Anims[1], EndDir);
	monster[i]._mVar6 = 16 * xoff;
	monster[i]._mVar7 = 16 * yoff;
	monster[i]._mVar8 = 0;
	M_CheckEFlag(i);
}

void M_StartWalk3(int i, int xvel, int yvel, int xoff, int yoff, int xadd, int yadd, int mapx, int mapy, int EndDir)
{
	long fx, fy, x, y;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	fx = monster[i]._mx + xadd;
	fy = monster[i]._my + yadd;
	x = monster[i]._mx + mapx;
	y = monster[i]._my + mapy;

	if(monster[i]._uniqtype != 0) {
		ChangeLightXY(monster[i].mlid, x, y);
	}

	/// ASSERT: assert((DWORD)fx < MAXDUNX);
	/// ASSERT: assert((DWORD)fy < MAXDUNY);
	/// ASSERT: assert((DWORD)monster[i]._mx < MAXDUNX);
	/// ASSERT: assert((DWORD)monster[i]._my < MAXDUNY);
	dMonster[monster[i]._mx][monster[i]._my] = -(i + 1);
	dMonster[fx][fy] = -(i + 1);
	monster[i]._mVar4 = x;
	monster[i]._mVar5 = y;
	dFlags[x][y] |= 0x10;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	monster[i]._mfutx = fx;
	monster[i]._mfuty = fy;
	monster[i]._mxoff = xoff;
	monster[i]._myoff = yoff;
	monster[i]._mmode = MM_WALK3;
	monster[i]._mxvel = xvel;
	monster[i]._myvel = yvel;
	monster[i]._mVar1 = fx;
	monster[i]._mVar2 = fy;
	monster[i]._mVar3 = EndDir;
	monster[i]._mdir = EndDir;
	NewMonsterAnim(i, monster[i].MType->Anims[1], EndDir);
	monster[i]._mVar6 = 16 * xoff;
	monster[i]._mVar7 = 16 * yoff;
	monster[i]._mVar8 = 0;
	M_CheckEFlag(i);
}

void M_StartAttack(int i)
{
	int md;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	md = M_GetDir(i);
	NewMonsterAnim(i, monster[i].MType->Anims[2], md);
	monster[i]._mmode = MM_ATTACK;
	monster[i]._mxoff = 0;
	monster[i]._myoff = 0;
	monster[i]._mfutx = monster[i]._mx;
	monster[i]._mfuty = monster[i]._my;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	monster[i]._mdir = md;
	M_CheckEFlag(i);
}

void M_StartRAttack(int i, int missile_type, int dam)
{
	int md;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	md = M_GetDir(i);
	NewMonsterAnim(i, monster[i].MType->Anims[2], md);
	monster[i]._mmode = MM_RATTACK;
	monster[i]._mVar1 = missile_type;
	monster[i]._mVar2 = dam;
	monster[i]._mxoff = 0;
	monster[i]._myoff = 0;
	monster[i]._mfutx = monster[i]._mx;
	monster[i]._mfuty = monster[i]._my;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	monster[i]._mdir = md;
	M_CheckEFlag(i);
}

void M_StartRSpAttack(int i, int missile_type, int dam)
{
	int md;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	md = M_GetDir(i);
	NewMonsterAnim(i, monster[i].MType->Anims[5], md);
	monster[i]._mmode = MM_RSPATTACK;
	monster[i]._mVar1 = missile_type;
	monster[i]._mVar2 = 0;
	monster[i]._mVar3 = dam;
	monster[i]._mxoff = 0;
	monster[i]._myoff = 0;
	monster[i]._mfutx = monster[i]._mx;
	monster[i]._mfuty = monster[i]._my;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	monster[i]._mdir = md;
	M_CheckEFlag(i);
}

void M_StartSpAttack(int i)
{
	int md;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	md = M_GetDir(i);
	NewMonsterAnim(i, monster[i].MType->Anims[5], md);
	monster[i]._mmode = MM_SATTACK;
	monster[i]._mxoff = 0;
	monster[i]._myoff = 0;
	monster[i]._mfutx = monster[i]._mx;
	monster[i]._mfuty = monster[i]._my;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	monster[i]._mdir = md;
	M_CheckEFlag(i);
}

void M_StartEat(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	NewMonsterAnim(i, monster[i].MType->Anims[5], monster[i]._mdir);
	monster[i]._mmode = MM_SATTACK;
	monster[i]._mxoff = 0;
	monster[i]._myoff = 0;
	monster[i]._mfutx = monster[i]._mx;
	monster[i]._mfuty = monster[i]._my;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	M_CheckEFlag(i);
}

void M_ClearSquares(int i)
{
	int mx, my, mt, mt2, x, y;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	mx = monster[i]._moldx;
	my = monster[i]._moldy;
	/// ASSERT: assert((DWORD)(mx+1) < MAXDUNX);
	/// ASSERT: assert((DWORD)(my+1) < MAXDUNY);
	mt = -(i + 1);
	mt2 = i + 1;

	for(y = my - 1; y <= my + 1; y++) {
		if(y >= 0 && y < MAXDUNY) {
			for(x = mx - 1; x <= mx + 1; x++) {
				if(x >= 0 && x < MAXDUNX) {
					if(dMonster[x][y] == mt || dMonster[x][y] == mt2) {
						dMonster[x][y] = 0;
					}
				}
			}
		}
	}

	if(mx + 1 < MAXDUNX) {
		dFlags[mx + 1][my] &= ~0x10;
	}
	if(my + 1 < MAXDUNY) {
		dFlags[mx][my + 1] &= ~0x10;
	}
}

void M_GetKnockback(int i)
{
	int d;

	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	d = (monster[i]._mdir - 4) & 7;
	if(!DirOK(i, d)) {
		return;
	}

	M_ClearSquares(i);
	monster[i]._moldx += offset_x[d];
	monster[i]._moldy += offset_y[d];
	NewMonsterAnim(i, monster[i].MType->Anims[3], monster[i]._mdir);
	monster[i]._mmode = MM_GOTHIT;
	monster[i]._mxoff = 0;
	monster[i]._myoff = 0;
	monster[i]._mx = monster[i]._moldx;
	monster[i]._my = monster[i]._moldy;
	monster[i]._mfutx = monster[i]._mx;
	monster[i]._mfuty = monster[i]._my;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	M_CheckEFlag(i);
	M_ClearSquares(i);
	/// ASSERT: assert((DWORD)monster[i]._mx < MAXDUNX);
	/// ASSERT: assert((DWORD)monster[i]._my < MAXDUNY);
	dMonster[monster[i]._mx][monster[i]._my] = i + 1;
}

void M_StartHit(int i, int pnum, int dam)
{
	/// ASSERT: assert((DWORD)i < MAXMONSTERS);
	/// ASSERT: assert(monster[i].MType != NULL);
	if(pnum >= 0) {
		monster[i].mWhoHit |= 1 << pnum;
	}
	if(pnum == myplr) {
		delta_monster_hp(i, monster[i]._mhitpoints, currlevel);
		NetSendCmdParam2(FALSE, CMD_MONSTDAMAGE, i, dam);
	}

	PlayEffect(i, 1);

	if(monster[i].MType->mtype >= MT_SNEAK && monster[i].MType->mtype <= MT_ILLWEAV || dam >> 6 >= monster[i].mLevel + 3) {
		if(pnum >= 0) {
			monster[i]._menemy = pnum;
			monster[i]._menemyx = plr[pnum]._px;
			monster[i]._menemyy = plr[pnum]._py;
			monster[i]._mFlags &= ~0x10;
			monster[i]._mdir = M_GetDir(i);
		}
		if(monster[i].MType->mtype == MT_BLINK) {
			M_Teleport(i);
		} else if(monster[i].MType->mtype >= MT_NSCAV && monster[i].MType->mtype <= MT_YSCAV) {
			monster[i]._mgoal = 1;
		}
		if(monster[i]._mmode != MM_STONE) {
			NewMonsterAnim(i, monster[i].MType->Anims[3], monster[i]._mdir);
			monster[i]._mmode = MM_GOTHIT;
			monster[i]._mxoff = 0;
			monster[i]._myoff = 0;
			monster[i]._mx = monster[i]._moldx;
			monster[i]._my = monster[i]._moldy;
			monster[i]._mfutx = monster[i]._mx;
			monster[i]._mfuty = monster[i]._my;
			monster[i]._moldx = monster[i]._mx;
			monster[i]._moldy = monster[i]._my;
			M_CheckEFlag(i);
			M_ClearSquares(i);
			dMonster[monster[i]._mx][monster[i]._my] = i + 1;
		}
	}
}

void M_DiabloDeath(int i, BOOL sendmsg)
{
	int steps, j, k;
	MonsterStruct *Monst;

	Monst = &monster[i];

	PlaySFX(USFX_DIABLOD);
	quests[Q_DIABLO]._qactive = 3;

	if(sendmsg) {
		NetSendCmdQuest(TRUE, Q_DIABLO);
	}

	sgbSaveSoundOn = gbSoundOn;
	gbProcessPlayers = 0;

	/// ASSERT: assert((DWORD)nummonsters <= MAXMONSTERS);
	for(j = 0; j < nummonsters; j++) {
		k = monstactive[j];
		if(k == i || monster[i]._msquelch == 0) {
			continue;
		}
		NewMonsterAnim(k, monster[k].MType->Anims[4], monster[k]._mdir);
		monster[k]._mmode = MM_DEATH;
		monster[k]._mxoff = 0;
		monster[k]._myoff = 0;
		monster[k]._mVar1 = 0;
		monster[k]._mx = monster[k]._moldx;
		monster[k]._my = monster[k]._moldy;
		monster[k]._mfutx = monster[k]._mx;
		monster[k]._mfuty = monster[k]._my;
		monster[k]._moldx = monster[k]._mx;
		monster[k]._moldy = monster[k]._my;
		M_CheckEFlag(k);
		M_ClearSquares(k);
		dMonster[monster[k]._mx][monster[k]._my] = k + 1;
	}

	AddLight(Monst->_mx, Monst->_my, 8);
	DoVision(Monst->_mx, Monst->_my, 8, 0, 1);

	if(abs(ViewX - Monst->_mx) > abs(ViewY - Monst->_my)) {
		steps = abs(ViewX - Monst->_mx);
	} else {
		steps = abs(ViewY - Monst->_my);
	}
	if(steps > 20) {
		steps = 20;
	}

	Monst->_mVar3 = ViewX << 16;
	Monst->_mVar4 = ViewY << 16;
	Monst->_mVar5 = (double)(Monst->_mVar3 - (Monst->_mx << 16)) / (double)steps;
	Monst->_mVar6 = (double)(Monst->_mVar4 - (Monst->_my << 16)) / (double)steps;
}

void M2MStartHit(int mid, int i, int dam)
{
	if((DWORD)mid >= MAXMONSTERS) {
		app_fatal("Invalid monster %d getting hit by monster", mid);
	}
	if(monster[mid].MType == NULL) {
		app_fatal("Monster %d \"%s\" getting hit by monster: MType NULL", mid, monster[mid].mName);
	}

	if(i >= 0) {
		monster[i].mWhoHit |= 1 << i;
	}

	delta_monster_hp(mid, monster[mid]._mhitpoints, currlevel);
	NetSendCmdParam2(FALSE, CMD_MONSTDAMAGE, mid, dam);
	PlayEffect(mid, 1);

	if(monster[mid].MType->mtype >= MT_SNEAK && monster[mid].MType->mtype <= MT_ILLWEAV || dam >> 6 >= monster[mid].mLevel + 3) {
		if(i >= 0) {
			monster[mid]._mdir = (monster[i]._mdir - 4) & 7;
		}
		if(monster[mid].MType->mtype == MT_BLINK) {
			M_Teleport(mid);
		} else if(monster[mid].MType->mtype >= MT_NSCAV && monster[mid].MType->mtype <= MT_YSCAV) {
			monster[mid]._mgoal = 1;
		}
		if(monster[mid]._mmode != MM_STONE) {
			if(monster[mid].MType->mtype != MT_GOLEM) {
				NewMonsterAnim(mid, monster[mid].MType->Anims[3], monster[mid]._mdir);
				monster[mid]._mmode = MM_GOTHIT;
			}
			monster[mid]._mxoff = 0;
			monster[mid]._myoff = 0;
			monster[mid]._mx = monster[mid]._moldx;
			monster[mid]._my = monster[mid]._moldy;
			monster[mid]._mfutx = monster[mid]._mx;
			monster[mid]._mfuty = monster[mid]._my;
			monster[mid]._moldx = monster[mid]._mx;
			monster[mid]._moldy = monster[mid]._my;
			M_CheckEFlag(mid);
			M_ClearSquares(mid);
			dMonster[monster[mid]._mx][monster[mid]._my] = mid + 1;
		}
	}
}

void MonstStartKill(int i, int pnum, BOOL sendmsg)
{
	int md;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MonstStartKill: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("MonstStartKill: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	Monst = &monster[i];

	if(pnum >= 0) {
		Monst->mWhoHit |= 1 << pnum;
	}
	if(pnum < MAX_PLRS && i > 4) {
		AddPlrMonstExper(Monst->mLevel, Monst->mExp, Monst->mWhoHit);
	}

	monstkills[Monst->MType->mtype]++;
	Monst->_mhitpoints = 0;
	SetRndSeed(Monst->_mRndSeed);

	if(QuestStatus(Q_GARBUD) && Monst->mName == UniqMonst[0].mName) {
		CreateTypeItem(Monst->_mx + 1, Monst->_my + 1, TRUE, ITYPE_MACE, IMISC_NONE, TRUE, FALSE);
	} else if(i > 3) {
		SpawnItem(i, Monst->_mx, Monst->_my, sendmsg);
	}
	if(Monst->MType->mtype == MT_DIABLO) {
		M_DiabloDeath(i, TRUE);
	} else {
		PlayEffect(i, 2);
	}
	if(pnum >= 0) {
		md = M_GetDir(i);
	} else {
		md = Monst->_mdir;
	}

	Monst->_mdir = md;
	NewMonsterAnim(i, Monst->MType->Anims[4], md);
	Monst->_mmode = MM_DEATH;
	Monst->_mxoff = 0;
	Monst->_myoff = 0;
	Monst->_mVar1 = 0;
	Monst->_mx = Monst->_moldx;
	Monst->_my = Monst->_moldy;
	Monst->_mfutx = Monst->_mx;
	Monst->_mfuty = Monst->_my;
	Monst->_moldx = Monst->_mx;
	Monst->_moldy = Monst->_my;
	M_CheckEFlag(i);
	M_ClearSquares(i);
	dMonster[Monst->_mx][Monst->_my] = i + 1;
	CheckQuestKill(i, sendmsg);
	M_FallenFear(Monst->_mx, Monst->_my);

	if(Monst->MType->mtype >= MT_NACID && Monst->MType->mtype <= MT_XACID) {
		AddMissile(Monst->_mx, Monst->_my, 0, 0, 0, MIS_ACIDPUD, 1, i, Monst->_mint + 1, 0);
	}
}

void M2MStartKill(int i, int mid)
{
	int md;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M2MStartKill: Invalid monster (attacker) %d", i);
	}
	if((DWORD)i >= MAXMONSTERS) { /// BUGFIX: should use `mid`
		app_fatal("M2MStartKill: Invalid monster (killed) %d", mid);
	}
	if(monster[i].MType == NULL) { /// BUGFIX: should use `mid`
		app_fatal("M2MStartKill: Monster %d \"%s\" MType NULL", mid, monster[mid].mName);
	}

	delta_kill_monster(mid, monster[mid]._mx, monster[mid]._my, currlevel);
	NetSendCmdLocParam1(FALSE, CMD_MONSTDEATH, monster[mid]._mx, monster[mid]._my, mid);
	monster[mid].mWhoHit |= 1 << i;

	if(i < 4) {
		AddPlrMonstExper(monster[mid].mLevel, monster[mid].mExp, monster[mid].mWhoHit);
	}

	monstkills[monster[mid].MType->mtype]++;
	monster[mid]._mhitpoints = 0;
	SetRndSeed(monster[mid]._mRndSeed);

	if(mid >= 4) {
		SpawnItem(mid, monster[mid]._mx, monster[mid]._my, TRUE);
	}
	if(monster[mid].MType->mtype == MT_DIABLO) {
		M_DiabloDeath(mid, TRUE);
	} else {
		PlayEffect(i, 2);
	}

	PlayEffect(mid, 2);

	md = (monster[i]._mdir - 4) & 7;
	if(monster[mid].MType->mtype == MT_GOLEM) {
		md = 0;
	}

	monster[mid]._mdir = md;
	NewMonsterAnim(mid, monster[mid].MType->Anims[4], md);
	monster[mid]._mmode = MM_DEATH;
	monster[mid]._mxoff = 0;
	monster[mid]._myoff = 0;
	monster[mid]._mx = monster[mid]._moldx;
	monster[mid]._my = monster[mid]._moldy;
	monster[mid]._mfutx = monster[mid]._mx;
	monster[mid]._mfuty = monster[mid]._my;
	monster[mid]._moldx = monster[mid]._mx;
	monster[mid]._moldy = monster[mid]._my;
	M_CheckEFlag(mid);
	M_ClearSquares(mid);
	dMonster[monster[mid]._mx][monster[mid]._my] = mid + 1;
	CheckQuestKill(mid, TRUE);
	M_FallenFear(monster[mid]._mx, monster[mid]._my);

	if(monster[mid].MType->mtype >= MT_NACID && monster[mid].MType->mtype <= MT_XACID) {
		AddMissile(monster[mid]._mx, monster[mid]._my, 0, 0, 0, MIS_ACIDPUD, 1, mid, monster[mid]._mint + 1, 0);
	}
}

void M_StartKill(int i, int pnum)
{
	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_StartKill: Invalid monster %d", i);
	}

	if(myplr == pnum) {
		delta_kill_monster(i, monster[i]._mx, monster[i]._my, currlevel);
		if(i != pnum) {
			NetSendCmdLocParam1(FALSE, CMD_MONSTDEATH, monster[i]._mx, monster[i]._my, i);
		} else {
			NetSendCmdLocParam1(FALSE, CMD_KILLGOLEM, monster[i]._mx, monster[i]._my, currlevel);
		}
	}

	MonstStartKill(i, pnum, TRUE);
}

void M_SyncStartKill(int i, int x, int y, int pnum)
{
	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_SyncStartKill: Invalid monster %d", i);
	}

	if(monster[i]._mhitpoints == 0 || monster[i]._mmode == MM_DEATH) {
		return;
	}

	/// ASSERT: assert(pnum != myplr);

	if(dMonster[x][y] == 0) {
		M_ClearSquares(i);
		monster[i]._mx = x;
		monster[i]._my = y;
		monster[i]._moldx = x;
		monster[i]._moldy = y;
	}

	if(monster[i]._mmode == MM_STONE) {
		MonstStartKill(i, pnum, FALSE);
		monster[i]._mmode = MM_STONE;
	} else {
		MonstStartKill(i, pnum, FALSE);
	}

	/// ASSERT: assert(dMonster[monster[i]._mx][monster[i]._my] == i+1 || dMonster[monster[i]._mx][monster[i]._my] == -(i+1));
}

void M_StartFadein(int i, int md, BOOL backwards)
{
	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_StartFadein: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_StartFadein: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	NewMonsterAnim(i, monster[i].MType->Anims[5], md);
	monster[i]._mmode = MM_FADEIN;
	monster[i]._mxoff = 0;
	monster[i]._myoff = 0;
	monster[i]._mfutx = monster[i]._mx;
	monster[i]._mfuty = monster[i]._my;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	M_CheckEFlag(i);
	monster[i]._mdir = md;
	monster[i]._mFlags &= ~1;

	if(backwards) {
		monster[i]._mFlags |= 2;
		monster[i]._mAnimFrame = monster[i]._mAnimLen;
	}
}

void M_StartFadeout(int i, int md, BOOL backwards)
{
	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_StartFadeout: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_StartFadeout: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	NewMonsterAnim(i, monster[i].MType->Anims[5], md);
	monster[i]._mmode = MM_FADEOUT;
	monster[i]._mxoff = 0;
	monster[i]._myoff = 0;
	monster[i]._mfutx = monster[i]._mx;
	monster[i]._mfuty = monster[i]._my;
	monster[i]._moldx = monster[i]._mx;
	monster[i]._moldy = monster[i]._my;
	M_CheckEFlag(i);
	monster[i]._mdir = md;

	if(backwards) {
		monster[i]._mFlags |= 2;
		monster[i]._mAnimFrame = monster[i]._mAnimLen;
	}
}

void M_StartHeal(int i)
{
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_StartHeal: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_StartHeal: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	Monst = &monster[i];
	Monst->_mAnimData = Monst->MType->Anims[5].Frames[Monst->_mdir];
	Monst->_mAnimFrame = Monst->MType->Anims[5].Rate;
	Monst->_mFlags |= 2;
	Monst->_mmode = MM_HEAL;
	Monst->_mVar1 = Monst->_mmaxhp / (16 * (random(97, 5) + 4));
}

void M_ChangeLightOffset(int monst)
{
	int lx, ly, sign;

	if((DWORD)monst >= MAXMONSTERS) {
		app_fatal("M_ChangeLightOffset: Invalid monster %d", monst);
	}

	lx = 2 * monster[monst]._myoff + monster[monst]._mxoff;
	ly = 2 * monster[monst]._myoff - monster[monst]._mxoff;

	if(lx < 0) {
		sign = -1;
		lx = -lx;
	} else {
		sign = 1;
	}
	lx >>= 3;
	lx *= sign;

	if(ly < 0) {
		sign = -1;
		ly = -ly;
	} else {
		sign = 1;
	}
	ly >>= 3;
	ly *= sign;

	ChangeLightOff(monster[monst].mlid, lx, ly);
}

BOOL M_DoStand(int i)
{
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoStand: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoStand: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	Monst = &monster[i];
	if(Monst->MType->mtype == MT_GOLEM) {
		Monst->_mAnimData = Monst->MType->Anims[1].Frames[Monst->_mdir];
	} else {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[Monst->_mdir];
	}

	if(Monst->_mAnimFrame == Monst->_mAnimLen) {
		M_Enemy(i);
	}

	Monst->_mVar2++;

	return FALSE;
}

BOOL M_DoWalk(int i)
{
	BOOL rv;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoWalk: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoWalk: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	if(monster[i]._mVar8 == monster[i].MType->Anims[1].Rate) {
		dMonster[monster[i]._mx][monster[i]._my] = 0;
		monster[i]._mx += monster[i]._mVar1;
		monster[i]._my += monster[i]._mVar2;
		dMonster[monster[i]._mx][monster[i]._my] = i + 1;
		if(monster[i]._uniqtype != 0) {
			ChangeLightXY(monster[i].mlid, monster[i]._mx, monster[i]._my);
		}
		M_StartStand(i, monster[i]._mdir);
		rv = TRUE;
	} else {
		if(monster[i]._mAnimCnt == 0) {
			monster[i]._mVar8++;
			monster[i]._mVar6 += monster[i]._mxvel;
			monster[i]._mVar7 += monster[i]._myvel;
			monster[i]._mxoff = monster[i]._mVar6 >> 4;
			monster[i]._myoff = monster[i]._mVar7 >> 4;
		}
		rv = FALSE;
	}

	if(monster[i]._uniqtype != 0) {
		M_ChangeLightOffset(i);
	}

	return rv;
}

BOOL M_DoWalk2(int i)
{
	BOOL rv;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoWalk2: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoWalk2: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	if(monster[i]._mVar8 == monster[i].MType->Anims[1].Rate) {
		dMonster[monster[i]._mVar1][monster[i]._mVar2] = 0;
		if(monster[i]._uniqtype != 0) {
			ChangeLightXY(monster[i].mlid, monster[i]._mx, monster[i]._my);
		}
		M_StartStand(i, monster[i]._mdir);
		rv = TRUE;
	} else {
		if(monster[i]._mAnimCnt == 0) {
			monster[i]._mVar8++;
			monster[i]._mVar6 += monster[i]._mxvel;
			monster[i]._mVar7 += monster[i]._myvel;
			monster[i]._mxoff = monster[i]._mVar6 >> 4;
			monster[i]._myoff = monster[i]._mVar7 >> 4;
		}
		rv = FALSE;
	}

	if(monster[i]._uniqtype != 0) {
		M_ChangeLightOffset(i);
	}

	return rv;
}

BOOL M_DoWalk3(int i)
{
	BOOL rv;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoWalk3: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoWalk3: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	if(monster[i]._mVar8 == monster[i].MType->Anims[1].Rate) {
		dMonster[monster[i]._mx][monster[i]._my] = 0;
		monster[i]._mx = monster[i]._mVar1;
		monster[i]._my = monster[i]._mVar2;
		dFlags[monster[i]._mVar4][monster[i]._mVar5] &= ~0x10;
		dMonster[monster[i]._mx][monster[i]._my] = i + 1;
		if(monster[i]._uniqtype != 0) {
			ChangeLightXY(monster[i].mlid, monster[i]._mx, monster[i]._my);
		}
		M_StartStand(i, monster[i]._mdir);
		rv = TRUE;
	} else {
		if(monster[i]._mAnimCnt == 0) {
			monster[i]._mVar8++;
			monster[i]._mVar6 += monster[i]._mxvel;
			monster[i]._mVar7 += monster[i]._myvel;
			monster[i]._mxoff = monster[i]._mVar6 >> 4;
			monster[i]._myoff = monster[i]._mVar7 >> 4;
		}
		rv = FALSE;
	}

	if(monster[i]._uniqtype != 0) {
		M_ChangeLightOffset(i);
	}

	return rv;
}

void M_TryM2MHit(int i, int mid, int hper, int mind, int maxd)
{
	int hit, dam;
	BOOL ret;

	if((DWORD)mid >= MAXMONSTERS) {
		app_fatal("M_TryM2MHit: Invalid monster %d", mid);
	}
	if(monster[mid].MType == NULL) {
		app_fatal("M_TryM2MHit: Monster %d \"%s\" MType NULL", mid, monster[mid].mName);
	}

	if(monster[mid]._mhitpoints >> 6 <= 0) {
		return;
	}
	if(monster[mid].MType->mtype == MT_ILLWEAV && monster[mid]._mgoal == 2) {
		return;
	}

	hit = random(4, 100);
	if(monster[mid]._mmode == MM_STONE) {
		hit = 0;
	}
	if(CheckMonsterHit(mid, ret)) {
		return;
	}
	if(hit < hper) {
		dam = (random(5, maxd - mind + 1) + mind) << 6;
		monster[mid]._mhitpoints -= dam;
		if(monster[mid]._mhitpoints >> 6 <= 0) {
			if(monster[mid]._mmode == MM_STONE) {
				M2MStartKill(i, mid);
				monster[mid]._mmode = MM_STONE;
			} else {
				M2MStartKill(i, mid);
			}
		} else {
			if(monster[mid]._mmode == MM_STONE) {
				M2MStartHit(mid, i, dam);
				monster[mid]._mmode = MM_STONE;
			} else {
				M2MStartHit(mid, i, dam);
			}
		}
	}
}

void M_TryH2HHit(int i, int pnum, int Hit, int MinDam, int MaxDam)
{
	int hit, hper, tac, dx, dy, blk, blkper, blkdir, mdam, newx, newy, j, k, mi;
	long dam;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_TryH2HHit: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_TryH2HHit: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	if(monster[i]._mFlags & 0x10) {
		M_TryM2MHit(i, pnum, Hit, MinDam, MaxDam);
		return;
	}
	/// ASSERT: assert((DWORD)pnum < MAX_PLRS);
	if(plr[pnum]._pHitPoints >> 6 <= 0 || plr[pnum]._pInvincible || plr[pnum]._pSpellFlags & 1) {
		return;
	}
	dx = abs(monster[i]._mx - plr[pnum].WorldX);
	dy = abs(monster[i]._my - plr[pnum].WorldY);
	if(dx >= 2 || dy >= 2) {
		return;
	}

	hit = random(98, 100);
#ifdef _DEBUG
	if(debug_mode_dollar_sign || debug_mode_key_inverted_v) {
		hit = 1000;
	}
#endif
	tac = plr[pnum]._pIAC + plr[pnum]._pIBonusAC + plr[pnum]._pDexterity / 5;
	hper = Hit + 30 + ((monster[i].mLevel - plr[pnum]._pLevel) << 1) - tac;
	if(hper < 15) {
		hper = 15;
	}
	if(currlevel == 14 && hper < 20) {
		hper = 20;
	}
	if(currlevel == 15 && hper < 25) {
		hper = 25;
	}
	if(currlevel == 16 && hper < 30) {
		hper = 30;
	}

	if((plr[pnum]._pmode == PM_STAND || plr[pnum]._pmode == PM_ATTACK) && plr[pnum]._pBlockFlag) {
		blk = random(98, 100);
	} else {
		blk = 100;
	}
	blkper = plr[pnum]._pBaseToBlk + plr[pnum]._pDexterity - ((monster[i].mLevel - plr[pnum]._pLevel) << 1);
	if(blkper < 0) {
		blkper = 0;
	}
	if(blkper > 100) {
		blkper = 100;
	}

	if(hit < hper) {
		if(blk < blkper) {
			blkdir = GetDirection(plr[pnum].WorldX, plr[pnum].WorldY, monster[i]._mx, monster[i]._my);
			StartPlrBlock(pnum, blkdir);
			return;
		}
		if(monster[i].MType->mtype == MT_YZOMBIE && pnum == myplr) {
			mi = -1;
			for(j = 0; j < nummissiles; j++) {
				k = missileactive[j];
				if(missile[k]._mitype == MIS_MANASHIELD && missile[k]._misource == pnum) {
					mi = k;
				}
			}
			if(plr[pnum]._pMaxHP > 64 && plr[pnum]._pMaxHPBase > 64) {
				plr[pnum]._pMaxHP -= 64;
				if(plr[pnum]._pHitPoints > plr[pnum]._pMaxHP) {
					plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
					if(mi >= 0) {
						missile[mi]._miVar1 = plr[pnum]._pMaxHP;
					}
				}
				plr[pnum]._pMaxHPBase -= 64;
				if(plr[pnum]._pHPBase > plr[pnum]._pMaxHPBase) {
					plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;
					if(mi >= 0) {
						missile[mi]._miVar2 = plr[pnum]._pMaxHPBase;
					}
				}
			}
		}
		dam = (MaxDam - MinDam + 1) << 6;
		dam = random(99, dam) + (MinDam << 6);
		dam += plr[pnum]._pIGetHit << 6;
		if(dam < 64) {
			dam = 64;
		}
		if(pnum == myplr) {
			plr[pnum]._pHitPoints -= dam;
			plr[pnum]._pHPBase -= dam;
		}
		if(plr[pnum]._pIFlags & ISPL_THORNS) {
			mdam = (random(99, 3) + 1) << 6;
			monster[i]._mhitpoints -= mdam;
			if(monster[i]._mhitpoints >> 6 <= 0) {
				M_StartKill(i, pnum);
			} else {
				M_StartHit(i, pnum, mdam);
			}
		}
		if(!(monster[i]._mFlags & 0x1000) && monster[i].MType->mtype == MT_SKING && gbMaxPlayers != 1) {
			monster[i]._mhitpoints += dam;
		}
		if(plr[pnum]._pHitPoints > plr[pnum]._pMaxHP) {
			plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
			plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;
		}
		if(plr[pnum]._pHitPoints >> 6 <= 0) {
			SyncPlrKill(pnum, 0);
		} else {
			StartPlrHit(pnum, dam, FALSE);
			if(monster[i]._mFlags & 0x80) {
				if(plr[pnum]._pmode != PM_GOTHIT) {
					StartPlrHit(pnum, 0, TRUE);
				}
				newx = plr[pnum].WorldX + offset_x[monster[i]._mdir];
				newy = plr[pnum].WorldY + offset_y[monster[i]._mdir];
				if(PosOkPlayer(pnum, newx, newy)) {
					plr[pnum].WorldX = newx;
					plr[pnum].WorldY = newy;
					FixPlayerLocation(pnum, plr[pnum]._pdir);
					FixPlrWalkTags(pnum);
					dPlayer[newx][newy] = pnum + 1;
					SetPlayerOld(pnum);
				}
			}
		}
	}
}

BOOL M_DoAttack(int i)
{
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoAttack: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoAttack: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}
	if(monster[i].MType == NULL) { /// BUGFIX: should check 'MData'
		app_fatal("M_DoAttack: Monster %d \"%s\" MData NULL", i, monster[i].mName);
	}

	Monst = &monster[i];
	if(Monst->_mAnimFrame == Monst->MData->mAFNum) {
		M_TryH2HHit(i, Monst->_menemy, Monst->mHit, Monst->mMinDamage, Monst->mMaxDamage);
		if(Monst->_mAi != AI_SNAKE) {
			PlayEffect(i, 0);
		}
	}
	if(Monst->MType->mtype >= MT_NMAGMA && Monst->MType->mtype <= MT_WMAGMA && Monst->_mAnimFrame == 9) {
		M_TryH2HHit(i, Monst->_menemy, Monst->mHit + 10, Monst->mMinDamage - 2, Monst->mMaxDamage - 2);
		PlayEffect(i, 0);
	}
	if(Monst->MType->mtype >= MT_STORM && Monst->MType->mtype <= MT_MAEL && Monst->_mAnimFrame == 13) {
		M_TryH2HHit(i, Monst->_menemy, Monst->mHit - 20, Monst->mMinDamage + 4, Monst->mMaxDamage + 4);
		PlayEffect(i, 0);
	}
	if(Monst->_mAi == AI_SNAKE && Monst->_mAnimFrame == 1) {
		PlayEffect(i, 0);
	}

	if(Monst->_mAnimFrame == Monst->_mAnimLen) {
		M_StartStand(i, Monst->_mdir);
		return TRUE;
	}

	return FALSE;
}

BOOL M_DoRAttack(int i)
{
	int multimissiles, mi;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoRAttack: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoRAttack: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}
	if(monster[i].MType == NULL) { /// BUGFIX: should check 'MData'
		app_fatal("M_DoRAttack: Monster %d \"%s\" MData NULL", i, monster[i].mName);
	}

	if(monster[i]._mAnimFrame == monster[i].MData->mAFNum) {
		if(monster[i]._mVar1 != -1) {
			if(monster[i]._mVar1 == MIS_CBOLT) {
				multimissiles = 3;
			} else {
				multimissiles = 1;
			}
			for(mi = 0; mi < multimissiles; mi++) {
				AddMissile(
					monster[i]._mx,
					monster[i]._my,
					monster[i]._menemyx,
					monster[i]._menemyy,
					monster[i]._mdir,
					monster[i]._mVar1,
					1,
					i,
					monster[i]._mVar2,
					0);
			}
		}
		PlayEffect(i, 0);
	}

	if(monster[i]._mAnimFrame == monster[i]._mAnimLen) {
		M_StartStand(i, monster[i]._mdir);
		return TRUE;
	}

	return FALSE;
}

BOOL M_DoRSpAttack(int i)
{
	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoRSpAttack: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoRSpAttack: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}
	if(monster[i].MType == NULL) { /// BUGFIX: should check 'MData'
		app_fatal("M_DoRSpAttack: Monster %d \"%s\" MData NULL", i, monster[i].mName);
	}

	if(monster[i]._mAnimFrame == monster[i].MData->mAFNum2 && monster[i]._mAnimCnt == 0) {
		AddMissile(
			monster[i]._mx,
			monster[i]._my,
			monster[i]._menemyx,
			monster[i]._menemyy,
			monster[i]._mdir,
			monster[i]._mVar1,
			1,
			i,
			monster[i]._mVar3,
			0);
		PlayEffect(i, 3);
	}
	if(monster[i]._mAi == AI_MEGA && monster[i]._mAnimFrame == 3) {
		if(monster[i]._mVar2++ == 0) {
			monster[i]._mFlags |= 4;
		} else if(monster[i]._mVar2 == 15) {
			monster[i]._mFlags &= ~4;
		}
	}

	if(monster[i]._mAnimFrame == monster[i]._mAnimLen) {
		M_StartStand(i, monster[i]._mdir);
		return TRUE;
	}

	return FALSE;
}

BOOL M_DoSAttack(int i)
{
	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoSAttack: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoSAttack: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}
	if(monster[i].MType == NULL) { /// BUGFIX: should check 'MData'
		app_fatal("M_DoSAttack: Monster %d \"%s\" MData NULL", i, monster[i].mName);
	}

	if(monster[i]._mAnimFrame == monster[i].MData->mAFNum2) {
		M_TryH2HHit(i, monster[i]._menemy, monster[i].mHit2, monster[i].mMinDamage2, monster[i].mMaxDamage2);
	}

	if(monster[i]._mAnimFrame == monster[i]._mAnimLen) {
		M_StartStand(i, monster[i]._mdir);
		return TRUE;
	}

	return FALSE;
}

BOOL M_DoFadein(int i)
{
	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoFadein: Invalid monster %d", i);
	}

	if(monster[i]._mFlags & 2 && monster[i]._mAnimFrame == 1
	|| !(monster[i]._mFlags & 2) && monster[i]._mAnimFrame == monster[i]._mAnimLen) {
		M_StartStand(i, monster[i]._mdir);
		monster[i]._mFlags &= ~2;
		return TRUE;
	}

	return FALSE;
}

BOOL M_DoFadeout(int i)
{
	int mtype;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoFadeout: Invalid monster %d", i);
	}

	if(monster[i]._mFlags & 2 && monster[i]._mAnimFrame == 1
	|| !(monster[i]._mFlags & 2) && monster[i]._mAnimFrame == monster[i]._mAnimLen) {
		/// ASSERT: assert(monster[i].MType != NULL);
		mtype = monster[i].MType->mtype;
		if(mtype >= MT_INCIN && mtype <= MT_HELLBURN) {
			monster[i]._mFlags &= ~2;
		} else {
			monster[i]._mFlags &= ~2;
			monster[i]._mFlags |= 1;
		}
		M_StartStand(i, monster[i]._mdir);
		return TRUE;
	}

	return FALSE;
}

BOOL M_DoHeal(int i)
{
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoHeal: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(monster[i]._mFlags & 8) {
		Monst->_mFlags &= ~4;
		Monst->_mmode = MM_SATTACK;
		return FALSE;
	}

	if(Monst->_mAnimFrame == 1) {
		Monst->_mFlags &= ~2;
		Monst->_mFlags |= 4;
		if(Monst->_mhitpoints + Monst->_mVar1 < Monst->_mmaxhp) {
			Monst->_mhitpoints += Monst->_mVar1;
		} else {
			Monst->_mhitpoints = Monst->_mmaxhp;
			Monst->_mFlags &= ~4;
			Monst->_mmode = MM_SATTACK;
		}
	}

	return FALSE;
}

BOOL M_DoTalk(int i)
{
	int tren;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoTalk: Invalid monster %d", i);
	}

	Monst = &monster[i];
	M_StartStand(i, monster[i]._mdir);
	Monst->_mgoal = 7;

	if(effect_is_playing(alltext[monster[i].mtalkmsg].sfxnr)) {
		return FALSE;
	}

	InitQTextMsg(monster[i].mtalkmsg);

	if(monster[i].mName == UniqMonst[0].mName) {
		if(monster[i].mtalkmsg == TEXT_GARBUD1) {
			quests[Q_GARBUD]._qactive = 2;
		}
		quests[Q_GARBUD]._qlog = 1;
		if(monster[i].mtalkmsg == TEXT_GARBUD2 && !(monster[i]._mFlags & 0x40)) {
			SpawnItem(i, monster[i]._mx + 1, monster[i]._my + 1, TRUE);
			monster[i]._mFlags |= 0x40;
		}
	}
	if(monster[i].mName == UniqMonst[2].mName) {
		if(monster[i].mtalkmsg == TEXT_ZHAR1 && !(monster[i]._mFlags & 0x40)) {
			quests[Q_ZHAR]._qactive = 2;
			quests[Q_ZHAR]._qlog = 1;
			CreateTypeItem(monster[i]._mx + 1, monster[i]._my + 1, FALSE, ITYPE_MISC, IMISC_BOOK, TRUE, FALSE);
			monster[i]._mFlags |= 0x40;
		}
	}
	if(monster[i].mName == UniqMonst[3].mName) {
		if(monster[i].mtalkmsg == TEXT_BANNER10 && !(monster[i]._mFlags & 0x40)) {
			/// ASSERT: assert(setpc_x != 0);
			ObjChangeMap(setpc_x, setpc_y, setpc_x + (setpc_w >> 1) + 2, setpc_y + (setpc_h >> 1) - 2);
			tren = TransVal;
			TransVal = 9;
			DRLG_MRectTrans(setpc_x, setpc_y, setpc_x + (setpc_w >> 1) + 4, setpc_y + (setpc_h >> 1));
			TransVal = tren;
			quests[Q_LTBANNER]._qvar1 = 2;
			if(quests[Q_LTBANNER]._qactive == 1) {
				quests[Q_LTBANNER]._qactive = 2;
			}
			monster[i]._mFlags |= 0x40;
		}
		if(quests[Q_LTBANNER]._qvar1 < 2) {
			sprintf(tempstr, "SS Talk = %i, Flags = %i", monster[i].mtalkmsg, monster[i]._mFlags);
			app_fatal(tempstr);
		}
	}
	if(monster[i].mName == UniqMonst[7].mName) {
		if(monster[i].mtalkmsg == TEXT_VEIL9) {
			quests[Q_VEIL]._qactive = 2;
			quests[Q_VEIL]._qlog = 1;
		}
		if(monster[i].mtalkmsg == TEXT_VEIL11 && !(monster[i]._mFlags & 0x40)) {
			SpawnUnique(UITEM_STEELVEIL, monster[i]._mx + 1, monster[i]._my + 1);
			monster[i]._mFlags |= 0x40;
		}
	}
	if(monster[i].mName == UniqMonst[8].mName) {
		/// ASSERT: assert(gbMaxPlayers == 1);
		quests[Q_WARLORD]._qvar1 = 2;
	}
	if(monster[i].mName == UniqMonst[4].mName && gbMaxPlayers != 1) {
		monster[i]._msquelch = 255;
		monster[i].mtalkmsg = 0;
		quests[Q_BETRAYER]._qvar1 = 6;
		monster[i]._mgoal = 1;
	}

	return FALSE;
}

void M_Teleport(int i)
{
	int mulx, muly, x, y, a, b, px, py;
	BOOL done;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_Teleport: Invalid monster %d", i);
	}

	Monst = &monster[i];
	done = FALSE;

	if(Monst->_mmode == MM_STONE) {
		return;
	}

	px = Monst->_menemyx;
	py = Monst->_menemyy;
	a = 2 * random(100, 2) - 1;
	b = 2 * random(100, 2) - 1;

	for(mulx = -1; mulx <= 1 && !done; mulx++) {
		for(muly = -1; muly < 1 && !done; muly++) {
			if(mulx != 0 || muly != 0) {
				x = px + a * mulx;
				y = py + b * muly;
				if(y >= 0 && y < MAXDUNY && x >= 0 && x < MAXDUNX && x != Monst->_mx && y != Monst->_my) {
					if(PosOkMonst(i, x, y)) {
						done = TRUE;
					}
				}
			}
		}
	}

	if(done) {
		M_ClearSquares(i);
		/// ASSERT: assert((DWORD)Monst->_mx < MAXDUNX);
		/// ASSERT: assert((DWORD)Monst->_my < MAXDUNY);
		dMonster[Monst->_mx][Monst->_my] = 0;
		dMonster[x][y] = i + 1;
		Monst->_moldx = x;
		Monst->_moldy = y;
		Monst->_mdir = M_GetDir(i);
		M_CheckEFlag(i);
	}
}

BOOL M_DoGotHit(int i)
{
	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoGotHit: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoGotHit: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	if(monster[i]._mAnimFrame == monster[i]._mAnimLen) {
		M_StartStand(i, monster[i]._mdir);
		return TRUE;
	}

	return FALSE;
}

void M_UpdateLeader(int i)
{
	int x, tmp;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_UpdateLeader: Invalid monster %d", i);
	}

	if(monster[i]._uniqtype != 0 && UniqMonst[monster[i]._uniqtype - 1].mUnqAttr & 2) {
		/// ASSERT: assert((DWORD)nummonsters <= MAXMONSTERS);
	}

	for(x = 0; x < nummonsters; x++) {
		tmp = monstactive[x];
		if(monster[tmp].leaderflag == 1 && monster[tmp].leader == i) {
			monster[tmp].leaderflag = 0;
		}
	}

	if(monster[i].leaderflag == 1) {
		monster[monster[i].leader].packsize--;
	}
}

void DoEnding()
{
	BOOL bMusicOn;
	long lVolume;

	if(gbMaxPlayers > 1) {
		SNetLeaveGame(0x40000004);
	}

	music_stop();

	if(gbMaxPlayers > 1) {
		Sleep(1000);
	}

	if(plr[myplr]._pClass == PC_WARRIOR) {
		play_movie("gendata\\DiabVic2.smk", FALSE);
	} else if(plr[myplr]._pClass == PC_SORCERER) {
		play_movie("gendata\\DiabVic1.smk", FALSE);
	} else {
		play_movie("gendata\\DiabVic3.smk", FALSE);
	}

	play_movie("gendata\\Diabend.smk", FALSE);
	bMusicOn = gbMusicOn;
	gbMusicOn = 1;
	lVolume = sound_get_or_set_music_volume(1);
	sound_get_or_set_music_volume(0);
	music_start(TMUSIC_L2);
	loop_movie = TRUE;
	play_movie("gendata\\loopdend.smk", TRUE);
	loop_movie = FALSE;
	music_stop();
	sound_get_or_set_music_volume(lVolume);
	gbMusicOn = bMusicOn;
}

void PrepDoEnding()
{
	int i, d;
	DWORD k;

	gbSoundOn = sgbSaveSoundOn;
	gbRunGame = 0;
	deathflag = 0;
	cineflag = 1;

	/// ASSERT: assert((DWORD)myplr < MAX_PLRS);
	k = plr[myplr].pDiabloKillLevel;
	d = gnDifficulty + 1;
	if(k > d) {
		d = k;
	}
	plr[myplr].pDiabloKillLevel = d;

	for(i = 0; i < MAX_PLRS; i++) {
		plr[i]._pmode = PM_QUIT;
		plr[i]._pInvincible = 1;
		if(gbMaxPlayers > 1) {
			if(plr[i]._pHitPoints >> 6 == 0) {
				plr[i]._pHitPoints = 64;
			}
			if(plr[i]._pMana >> 6 == 0) {
				plr[i]._pMana = 64;
			}
		}
	}
}

BOOL M_DoDeath(int i)
{
	int x, y;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoDeath: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoDeath: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	monster[i]._mVar1++;

	if(monster[i].MType->mtype == MT_DIABLO) {
		if(monster[i]._mx - ViewX < 0) {
			x = -1;
		} else if(monster[i]._mx - ViewX > 0) {
			x = 1;
		} else {
			x = 0;
		}
		ViewX += x;
		if(monster[i]._my - ViewY < 0) {
			y = -1;
		} else if(monster[i]._my - ViewY > 0) {
			y = 1;
		} else {
			y = 0;
		}
		ViewY += y;
		if(monster[i]._mVar1 == 140) {
			PrepDoEnding();
		}
		return FALSE;
	}

	if(monster[i]._mAnimFrame == monster[i]._mAnimLen) {
		if(monster[i]._uniqtype == 0) {
			AddDead(monster[i]._mx, monster[i]._my, monster[i].MType->mdeadval, monster[i]._mdir);
		} else {
			AddDead(monster[i]._mx, monster[i]._my, monster[i]._udeadval, monster[i]._mdir);
		}
		/// ASSERT: assert(!(dFlags[monster[i]._mx+1][monster[i]._my] & BFLAG_MONSTLR) && !(dFlags[monster[i]._mx][monster[i]._my+1] & BFLAG_MONSTLR));
		dMonster[monster[i]._mx][monster[i]._my] = 0;
		monster[i]._mDelFlag = 1;
		M_UpdateLeader(i);
	}

	return FALSE;
}

BOOL M_DoSpStand(int i)
{
	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoSpStand: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoSpStand: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	if(monster[i]._mAnimFrame == monster[i].MData->mAFNum2) {
		PlayEffect(i, 3);
	}

	if(monster[i]._mAnimFrame == monster[i]._mAnimLen) {
		M_StartStand(i, monster[i]._mdir);
		return TRUE;
	}

	return FALSE;
}

BOOL M_DoDelay(int i)
{
	int md, tmp;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoDelay: Invalid monster %d", i);
	}
	if(monster[i].MType == NULL) {
		app_fatal("M_DoDelay: Monster %d \"%s\" MType NULL", i, monster[i].mName);
	}

	md = M_GetDir(i);
	monster[i]._mAnimData = monster[i].MType->Anims[0].Frames[md];

	if(monster[i]._mAi == AI_LAZURUS && (monster[i]._mVar2 > 8 || monster[i]._mVar2 < 0)) {
		monster[i]._mVar2 = 8;
	}

	if(monster[i]._mVar2-- == 0) {
		tmp = monster[i]._mAnimFrame;
		M_StartStand(i, monster[i]._mdir);
		monster[i]._mAnimFrame = tmp;
		return TRUE;
	}

	return FALSE;
}

BOOL M_DoStone(int i)
{
	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_DoStone: Invalid monster %d", i);
	}

	if(monster[i]._mhitpoints == 0) {
		dMonster[monster[i]._mx][monster[i]._my] = 0;
		monster[i]._mDelFlag = 1;
	}

	return FALSE;
}

void M_WalkDir(int i, int md)
{
	int mwi;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_WalkDir: Invalid monster %d", i);
	}

	mwi = monster[i].MType->Anims[1].Rate - 1;

	switch(md) {
	case DIR_N:
		M_StartWalk(i, 0, -MWVel[mwi][1], -1, -1, 4);
		break;
	case DIR_NE:
		M_StartWalk(i, MWVel[mwi][1], -MWVel[mwi][0], 0, -1, 5);
		break;
	case DIR_E:
		M_StartWalk3(i, MWVel[mwi][2], 0, -32, -16, 1, -1, 1, 0, 6);
		break;
	case DIR_SE:
		M_StartWalk2(i, MWVel[mwi][1], MWVel[mwi][0], -32, -16, 1, 0, 7);
		break;
	case DIR_S:
		M_StartWalk2(i, 0, MWVel[mwi][1], 0, -32, 1, 1, 0);
		break;
	case DIR_SW:
		M_StartWalk2(i, -MWVel[mwi][1], MWVel[mwi][0], 32, -16, 0, 1, 1);
		break;
	case DIR_W:
		M_StartWalk3(i, -MWVel[mwi][2], 0, 32, -16, -1, 1, 0, 1, 2);
		break;
	case DIR_NW:
		M_StartWalk(i, -MWVel[mwi][1], -MWVel[mwi][0], -1, 0, 3);
		break;
	}
}

void GroupUnity(int i)
{
	int leader, tmp, m;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("GroupUnity: Invalid monster %d", i);
	}

	if(monster[i].leaderflag != 0) {
		leader = monster[i].leader;
		tmp = LineClearF(CheckNoSolid, monster[i]._mx, monster[i]._my, monster[leader]._mfutx, monster[leader]._mfuty);
		if(!tmp && monster[i].leaderflag == 1) {
			monster[leader].packsize--;
			monster[i].leaderflag = 2;
		} else if(tmp && monster[i].leaderflag == 2) {
			if(abs(monster[i]._mx - monster[leader]._mfutx) < 4 && abs(monster[i]._my - monster[leader]._mfuty) < 4) {
				monster[leader].packsize++;
				monster[i].leaderflag = 1;
			}
		}
	}

	if(monster[i].leaderflag == 1) {
		if(monster[i]._msquelch > monster[leader]._msquelch) {
			monster[leader]._lastx = monster[i]._mx;
			monster[leader]._lasty = monster[i]._my;
			monster[leader]._msquelch = monster[i]._msquelch - 1;
		}
		if(monster[leader]._mAi == AI_GARG && monster[leader]._mFlags & 4) {
			monster[leader]._mmode = MM_SATTACK;
			monster[leader]._mFlags &= ~4;
		}
	} else if(monster[i]._uniqtype != 0 && UniqMonst[monster[i]._uniqtype - 1].mUnqAttr & 2) {
		/// ASSERT: assert((DWORD)nummonsters <= MAXMONSTERS);
		for(m = 0; m < nummonsters; m++) {
			tmp = monstactive[m];
			if(monster[tmp].leaderflag == 1 && monster[tmp].leader == i) {
				if(monster[i]._msquelch > monster[tmp]._msquelch) {
					monster[tmp]._lastx = monster[i]._mx;
					monster[tmp]._lasty = monster[i]._my;
					monster[tmp]._msquelch = monster[i]._msquelch - 1;
				}
				if(monster[tmp]._mAi == AI_GARG && monster[tmp]._mFlags & 4) {
					monster[tmp]._mmode = MM_SATTACK;
					monster[tmp]._mFlags &= ~4;
				}
			}
		}
	}
}

BOOL M_CallWalk(int i, int md)
{
	int mdtemp;
	BOOL ok;

	mdtemp = md;
	ok = DirOK(i, md);

	if(random(101, 2) != 0) {
		ok = ok || (md = left[mdtemp], DirOK(i, md)) || (md = right[mdtemp], DirOK(i, md));
	} else {
		ok = ok || (md = right[mdtemp], DirOK(i, md)) || (md = left[mdtemp], DirOK(i, md));
	}
	if(random(102, 2) != 0) {
		ok = ok || (md = right[right[mdtemp]], DirOK(i, md)) || (md = left[left[mdtemp]], DirOK(i, md));
	} else {
		ok = ok || (md = left[left[mdtemp]], DirOK(i, md)) || (md = right[right[mdtemp]], DirOK(i, md));
	}

	if(ok) {
		M_WalkDir(i, md);
	}

	return ok;
}

BOOL M_PathWalk(int i)
{
	static const char plr2monst[9] = { 0, 5, 3, 7, 1, 4, 6, 0, 2 };
	BOOL (*Check)(int, int, int);
	char path[25];

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("M_PathWalk: Invalid monster %d", i);
	}

	if(monster[i]._mFlags & 0x200) {
		Check = PosOkMonst3;
	} else {
		Check = PosOkMonst;
	}

	if(FindPath(Check, i, monster[i]._mx, monster[i]._my, monster[i]._menemyx, monster[i]._menemyy, path) != 0) {
		M_CallWalk(i, plr2monst[path[0]]);
		return TRUE;
	}

	return FALSE;
}

BOOL M_CallWalk2(int i, int md)
{
	int mdtemp;
	BOOL ok;

	mdtemp = md;
	ok = DirOK(i, md);

	if(random(101, 2) != 0) {
		ok = ok || (md = left[mdtemp], DirOK(i, md)) || (md = right[mdtemp], DirOK(i, md));
	} else {
		ok = ok || (md = right[mdtemp], DirOK(i, md)) || (md = left[mdtemp], DirOK(i, md));
	}

	if(ok) {
		M_WalkDir(i, md);
	}

	return ok;
}

BOOL M_DumbWalk(int i, int md)
{
	BOOL ok;

	ok = DirOK(i, md);
	if(ok) {
		M_WalkDir(i, md);
	}

	return ok;
}

BOOL M_RoundWalk(int i, int md, int &dir)
{
	int mdtemp;
	BOOL ok;

	if(dir != 0) {
		md = left[left[md]];
	} else {
		md = right[right[md]];
	}

	mdtemp = md;
	ok = DirOK(i, md);

	if(!ok) {
		if(dir != 0) {
			ok = (md = right[mdtemp], DirOK(i, md)) || (md = right[right[mdtemp]], DirOK(i, md));
		} else {
			ok = (md = left[mdtemp], DirOK(i, md)) || (md = left[left[mdtemp]], DirOK(i, md));
		}
	}

	if(ok) {
		M_WalkDir(i, md);
	} else {
		dir = dir == 0;
		ok = M_CallWalk(i, opposite[mdtemp]);
	}

	return ok;
}

void MAI_Zombie(int i)
{
	int mx, my, md, v;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Zombie: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND) {
		return;
	}

	mx = Monst->_mx;
	my = Monst->_my;
	if(!(dFlags[mx][my] & 2)) {
		return;
	}

	mx -= Monst->_menemyx;
	my -= Monst->_menemyy;
	md = Monst->_mdir;
	v = random(103, 100);
	if(abs(mx) < 2 && abs(my) < 2) {
		if(v < 2 * Monst->_mint + 10) {
			M_StartAttack(i);
		}
	} else {
		if(v < 2 * Monst->_mint + 10) {
			if(abs(mx) < 2 * Monst->_mint + 4 && abs(my) < 2 * Monst->_mint + 4) {
				md = M_GetDir(i);
				M_CallWalk(i, md);
			} else {
				if(random(104, 100) < 2 * Monst->_mint + 20) {
					md = random(104, 8);
				}
				M_DumbWalk(i, md);
			}
		}
	}

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[Monst->_mdir];
	}
}

void MAI_SkelSd(int i)
{
	int mx, my, md;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_SkelSd: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	mx = Monst->_mx - Monst->_menemyx;
	my = Monst->_my - Monst->_menemyy;
	md = GetDirection(Monst->_mx, Monst->_my, Monst->_lastx, Monst->_lasty);
	Monst->_mdir = md;
	if(abs(mx) < 2 && abs(my) < 2) {
		if(Monst->_mVar1 == 13 || random(105, 100) < 2 * Monst->_mint + 20) {
			M_StartAttack(i);
		} else {
			M_StartDelay(i, random(105, 10) + 10 - 2 * Monst->_mint);
		}
	} else {
		if(Monst->_mVar1 != 13 && random(106, 100) < 35 - 4 * Monst->_mint) {
			M_StartDelay(i, random(106, 10) + 15 - 2 * Monst->_mint);
		} else {
			M_CallWalk(i, md);
		}
	}

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

BOOL MAI_Path(int i)
{
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Path: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->MType->mtype != MT_GOLEM) {
		if(Monst->_msquelch == 0 || Monst->_mmode != MM_STAND) {
			return FALSE;
		}
		if(Monst->_mgoal != 1 && Monst->_mgoal != 4 && Monst->_mgoal != 5) {
			return FALSE;
		}
		if(Monst->_mx == 1 && Monst->_my == 0) {
			return FALSE;
		}
	}
	if(!LineClearF1(PosOkMonst2, i, Monst->_mx, Monst->_my, Monst->_menemyx, Monst->_menemyy)
	|| Monst->_pathcount >= 5 && Monst->_pathcount < 8) {
		if(Monst->_mFlags & 0x200) {
			MonstCheckDoors(i);
		}
		Monst->_pathcount++;
		if(Monst->_pathcount < 5) {
			return FALSE;
		}
		if(M_PathWalk(i)) {
			return TRUE;
		}
	}
	if(Monst->MType->mtype != MT_GOLEM) {
		Monst->_pathcount = 0;
	}

	return FALSE;
}

void MAI_Snake(int i)
{
	MonsterStruct *Monst;
	int fx, fy, mx, my, md, pnum, tmp;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Snake: Invalid monster %d", i);
	}

	char pattern[6] = { 1, 1, 0, -1, -1, 0 };
	Monst = &monster[i];
	pnum = Monst->_menemy;
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	fx = Monst->_menemyx;
	fy = Monst->_menemyy;
	mx = Monst->_mx - fx;
	my = Monst->_my - fy;
	md = GetDirection(Monst->_mx, Monst->_my, Monst->_lastx, Monst->_lasty);
	Monst->_mdir = md;

	if(abs(mx) < 2 && abs(my) < 2) {
		if(Monst->_mVar1 == 13 || Monst->_mVar1 == 14 || random(105, 100) < Monst->_mint + 20) {
			M_StartAttack(i);
		} else {
			M_StartDelay(i, random(105, 10) + 10 - Monst->_mint);
		}
	} else if(abs(mx) < 3 && abs(my) < 3 && LineClearF1(PosOkMonst, i, Monst->_mx, Monst->_my, fx, fy) && Monst->_mVar1 != 14) {
		if(AddMissile(Monst->_mx, Monst->_my, fx, fy, md, MIS_RHINO, pnum, i, 0, 0) != -1) {
			PlayEffect(i, 0);
			dMonster[Monst->_mx][Monst->_my] = -(i + 1);
			Monst->_mmode = MM_CHARGE;
		}
	} else if(Monst->_mVar1 != 13 && random(106, 100) < 35 - 2 * Monst->_mint) {
		M_StartDelay(i, random(106, 10) + 15 - Monst->_mint);
	} else {
		if(md + pattern[Monst->_mgoalvar1] < 0) {
			md = md + pattern[Monst->_mgoalvar1] + 8;
		} else if(md + pattern[Monst->_mgoalvar1] >= 8) {
			md = md + pattern[Monst->_mgoalvar1] - 8;
		} else {
			md = md + pattern[Monst->_mgoalvar1];
		}
		Monst->_mgoalvar1++;
		if(Monst->_mgoalvar1 > 5) {
			Monst->_mgoalvar1 = 0;
		}
		tmp = md;
		if(md - Monst->_mgoalvar2 < 0) {
			md = md - Monst->_mgoalvar2 + 8;
		} else if(md - Monst->_mgoalvar2 >= 8) {
			md = md - Monst->_mgoalvar2 - 8;
		} else {
			md = md - Monst->_mgoalvar2;
		}
		if(md > 0) {
			if(md < 4) {
				if(Monst->_mgoalvar2 + 1 < 0) {
					md = Monst->_mgoalvar2 + 1 + 8;
				} else if(Monst->_mgoalvar2 + 1 >= 8) {
					md = Monst->_mgoalvar2 + 1 - 8;
				} else {
					md = Monst->_mgoalvar2 + 1;
				}
				Monst->_mgoalvar2 = md;
			} else if(md == 4) {
				Monst->_mgoalvar2 = tmp;
			} else {
				if(Monst->_mgoalvar2 - 1 < 0) {
					md = Monst->_mgoalvar2 - 1 + 8;
				} else if(Monst->_mgoalvar2 - 1 >= 8) {
					md = Monst->_mgoalvar2 - 1 - 8;
				} else {
					md = Monst->_mgoalvar2 - 1;
				}
				Monst->_mgoalvar2 = md;
			}
		}
		/// ASSERT: assert(Monst->_mgoalvar2 >= 0 && Monst->_mgoalvar2 < 8);
		if(!M_DumbWalk(i, Monst->_mgoalvar2)) {
			M_CallWalk2(i, Monst->_mdir);
		}
	}

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[Monst->_mdir];
	}
}

void MAI_Bat(int i)
{
	MonsterStruct *Monst;
	int mx, my, md, v, pnum, fx, fy;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Bat: Invalid monster %d", i);
	}

	Monst = &monster[i];
	pnum = Monst->_menemy;
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	mx = Monst->_mx - Monst->_menemyx;
	my = Monst->_my - Monst->_menemyy;
	md = GetDirection(Monst->_mx, Monst->_my, Monst->_lastx, Monst->_lasty);
	Monst->_mdir = md;
	v = random(107, 100);

	if(Monst->_mgoal == 2) {
		if(Monst->_mgoalvar1 == 0) {
			M_CallWalk(i, opposite[md]);
			Monst->_mgoalvar1++;
		} else {
			if(random(108, 2) != 0) {
				M_CallWalk(i, left[md]);
			} else {
				M_CallWalk(i, right[md]);
			}
			Monst->_mgoal = 1;
		}
		return;
	}

	fx = Monst->_menemyx;
	fy = Monst->_menemyy;

	if(Monst->MType->mtype == MT_GLOOM
	&& (abs(mx) >= 5 || abs(my) >= 5)
	&& v < 4 * Monst->_mint + 33
	&& LineClearF1(PosOkMonst, i, Monst->_mx, Monst->_my, fx, fy)) {
		if(AddMissile(Monst->_mx, Monst->_my, fx, fy, md, MIS_RHINO, pnum, i, 0, 0) != -1) {
			dMonster[Monst->_mx][Monst->_my] = -(i + 1);
			Monst->_mmode = MM_CHARGE;
		}
	} else if(abs(mx) >= 2 || abs(my) >= 2) {
		if(Monst->_mVar2 > 20 && v < Monst->_mint + 13
		|| (Monst->_mVar1 == 1 || Monst->_mVar1 == 2 || Monst->_mVar1 == 3) && Monst->_mVar2 == 0 && v < Monst->_mint + 63) {
			M_CallWalk(i, md);
		}
	} else if(v < 4 * Monst->_mint + 8) {
		M_StartAttack(i);
		Monst->_mgoal = 2;
		Monst->_mgoalvar1 = 0;
		if(Monst->MType->mtype == MT_FAMILIAR) {
			AddMissile(Monst->_menemyx, Monst->_menemyy, Monst->_menemyx + 1, 0, -1, MIS_LIGHTNING, 1, i, random(109, 10) + 1, 0);
		}
	}

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_SkelBow(int i)
{
	int mx, my, md, fx, fy, v;
	BOOL walking;
	MonsterStruct *Monst;

	walking = FALSE;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_SkelBow: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	mx = Monst->_mx - Monst->_menemyx;
	my = Monst->_my - Monst->_menemyy;
	md = M_GetDir(i);
	Monst->_mdir = md;
	v = random(110, 100);

	if(abs(mx) < 4 && abs(my) < 4) {
		if(Monst->_mVar2 > 20 && v < 2 * Monst->_mint + 13
		|| (Monst->_mVar1 == 1 || Monst->_mVar1 == 2 || Monst->_mVar1 == 3) && Monst->_mVar2 == 0 && v < 2 * Monst->_mint + 63) {
			walking = M_DumbWalk(i, opposite[md]);
		}
	}

	fx = Monst->_menemyx;
	fy = Monst->_menemyy;

	if(!walking && random(110, 100) < 2 * Monst->_mint + 3 && LineClear(Monst->_mx, Monst->_my, fx, fy)) {
		M_StartRAttack(i, MIS_ARROW, 4);
	}

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_Fat(int i)
{
	int mx, my, md, v;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Fat: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	mx = Monst->_mx - Monst->_menemyx;
	my = Monst->_my - Monst->_menemyy;
	md = M_GetDir(i);
	Monst->_mdir = md;
	v = random(111, 100);

	if(abs(mx) >= 2 || abs(my) >= 2) {
		if(Monst->_mVar2 > 20 && v < 4 * Monst->_mint + 20
		|| (Monst->_mVar1 == 1 || Monst->_mVar1 == 2 || Monst->_mVar1 == 3) && Monst->_mVar2 == 0 && v < 4 * Monst->_mint + 70) {
			M_CallWalk(i, md);
		}
	} else {
		if(v < 4 * Monst->_mint + 15) {
			M_StartAttack(i);
		} else if(v < 4 * Monst->_mint + 20) {
			M_StartSpAttack(i);
		}
	}

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_Sneak(int i)
{
	int mx, my, md, v, dist;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Sneak: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND) {
		return;
	}

	mx = Monst->_mx;
	my = Monst->_my;
	if(dLight[mx][my] == lightmax) {
		return;
	}

	mx -= Monst->_menemyx;
	my -= Monst->_menemyy;
	md = M_GetDir(i);
	dist = 5 - Monst->_mint;

	if(Monst->_mVar1 == 5) {
		Monst->_mgoal = 2;
		Monst->_mgoalvar1 = 0;
	} else if(abs(mx) >= dist + 3 || abs(my) >= dist + 3 || Monst->_mgoalvar1 > 8) {
		Monst->_mgoal = 1;
		Monst->_mgoalvar1 = 0;
	}

	if(Monst->_mgoal == 2) {
		if(Monst->_mFlags & 0x10) {
			md = GetDirection(Monst->_mx, Monst->_my, plr[Monst->_menemy]._pownerx, plr[Monst->_menemy]._pownery);
		}
		md = opposite[md];
		if(Monst->MType->mtype == MT_UNSEEN) {
			if(random(112, 2) != 0) {
				md = left[md];
			} else {
				md = right[md];
			}
		}
	}

	Monst->_mdir = md;
	v = random(112, 100);

	if(abs(mx) < dist && abs(my) < dist && Monst->_mFlags & 1) {
		M_StartFadein(i, md, FALSE);
	} else if((abs(mx) >= dist + 1 || abs(my) >= dist + 1) && !(Monst->_mFlags & 1)) {
		M_StartFadeout(i, md, TRUE);
	} else if(Monst->_mgoal == 2
	|| (abs(mx) >= 2 || abs(my) >= 2)
	&& (Monst->_mVar2 > 20 && v < 4 * Monst->_mint + 14
	|| (Monst->_mVar1 == 1 || Monst->_mVar1 == 2 || Monst->_mVar1 == 3)
	&& Monst->_mVar2 == 0
	&& v < 4 * Monst->_mint + 64)) {
		Monst->_mgoalvar1++;
		M_CallWalk(i, md);
	}

	if(Monst->_mmode == MM_STAND) {
		if(abs(mx) < 2 && abs(my) < 2 && v < 4 * Monst->_mint + 10) {
			M_StartAttack(i);
		} else {
			Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
		}
	}
}

void MAI_Fireman(int i)
{
	int mx, my, md, v, pnum, fx, fy;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Fireman: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	pnum = Monst->_menemy;
	fx = Monst->_menemyx;
	fy = Monst->_menemyy;
	mx = Monst->_mx - fx;
	my = Monst->_my - fy;
	md = M_GetDir(i);

	if(Monst->_mgoal == 1) {
		if(LineClear(Monst->_mx, Monst->_my, fx, fy)
		&& AddMissile(Monst->_mx, Monst->_my, fx, fy, md, MIS_NULL_32, pnum, i, 0, 0) != -1) {
			Monst->_mmode = MM_CHARGE;
			Monst->_mgoal = 5;
			Monst->_mgoalvar1 = 0;
		}
	} else if(Monst->_mgoal == 5) {
		if(Monst->_mgoalvar1 == 3) {
			Monst->_mgoal = 1;
			M_StartFadeout(i, md, TRUE);
		} else if(LineClear(Monst->_mx, Monst->_my, fx, fy)) {
			M_StartRAttack(i, MIS_NULL_33, 4);
			Monst->_mgoalvar1++;
		} else {
			M_StartDelay(i, random(112, 10) + 5);
			Monst->_mgoalvar1++;
		}
	} else if(Monst->_mgoal == 2) {
		M_StartFadein(i, md, FALSE);
		Monst->_mgoal = 5;
	}

	Monst->_mdir = md;
	v = random(112, 100); /* unused */

	if(Monst->_mmode == MM_STAND) {
		if(abs(mx) < 2 && abs(my) < 2 && Monst->_mgoal == 1) {
			M_TryH2HHit(i, monster[i]._menemy, monster[i].mHit, monster[i].mMinDamage, monster[i].mMaxDamage);
			Monst->_mgoal = 2;
			if(!M_CallWalk(i, opposite[md])) {
				M_StartFadein(i, md, FALSE);
				Monst->_mgoal = 5;
			}
		} else if(!M_CallWalk(i, md) && (Monst->_mgoal == 1 || Monst->_mgoal == 2)) {
			M_StartFadein(i, md, FALSE);
			Monst->_mgoal = 5;
		}
	}
}

void MAI_Fallen(int i)
{
	MonsterStruct *Monst;
	int x, y, xpos, ypos, m, rad, mx, my, aitype;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Fallen: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mgoal == 5) {
		if(Monst->_mgoalvar1 != 0) {
			Monst->_mgoalvar1--;
		} else {
			Monst->_mgoal = 1;
		}
	}

	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	if(Monst->_mgoal == 2 && Monst->_mgoalvar1-- == 0) {
		Monst->_mgoal = 1;
		M_StartStand(i, opposite[Monst->_mdir]);
	}

	if(Monst->_mAnimFrame == Monst->_mAnimLen) {
		if(random(113, 4) == 0) {
			if(!(monster[i]._mFlags & 8)) {
				M_StartSpStand(i, Monst->_mdir);
				if(Monst->_mmaxhp - (2 * Monst->_mint + 2) >= Monst->_mhitpoints) {
					Monst->_mhitpoints += 2 * Monst->_mint + 2;
				} else {
					Monst->_mhitpoints = Monst->_mmaxhp;
				}
			}
			rad = 2 * Monst->_mint + 4;
			for(y = -rad; y <= rad; y++) {
				for(x = -rad; x <= rad; x++) {
					xpos = x + Monst->_mx;
					ypos = y + Monst->_my;
					if(y >= 0 && y < MAXDUNY && x >= 0 && x < MAXDUNX) {
						m = dMonster[xpos][ypos];
						if(m > 0) {
							m--;
							aitype = monster[m]._mAi;
							if(aitype == AI_FALLEN) {
								monster[m]._mgoal = 5;
								monster[m]._mgoalvar1 = 5 * (6 * Monst->_mint + 21);
							}
						}
					}
				}
			}
		}
	} else {
		if(Monst->_mgoal == 2) {
			M_CallWalk(i, Monst->_mdir);
		} else if(Monst->_mgoal == 5) {
			mx = Monst->_mx - Monst->_menemyx;
			my = Monst->_my - Monst->_menemyy;
			if(abs(mx) < 2 && abs(my) < 2) {
				M_StartAttack(i);
			} else {
				M_CallWalk(i, M_GetDir(i));
			}
		} else {
			MAI_SkelSd(i);
		}
	}
}

void MAI_Cleaver(int i)
{
	MonsterStruct *Monst;
	int mx, my, md;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Cleaver: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	mx = Monst->_mx - Monst->_menemyx;
	my = Monst->_my - Monst->_menemyy;
	md = GetDirection(Monst->_mx, Monst->_my, Monst->_lastx, Monst->_lasty);
	Monst->_mdir = md;

	if(abs(mx) < 2 && abs(my) < 2) {
		M_StartAttack(i);
	} else {
		M_CallWalk(i, md);
	}

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_Round(int i, BOOL special)
{
	int mx, my, md, v, fx, fy, dist;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Round: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	fx = Monst->_menemyx;
	fy = Monst->_menemyy;
	mx = Monst->_mx - fx;
	my = Monst->_my - fy;
	md = GetDirection(Monst->_mx, Monst->_my, Monst->_lastx, Monst->_lasty);

	if(Monst->_msquelch < 255) {
		MonstCheckDoors(i);
	}

	v = random(114, 100);

	if(abs(mx) < 2 && abs(my) < 2 || Monst->_msquelch != 255 || dTransVal[Monst->_mx][Monst->_my] != dTransVal[fx][fy]) {
		Monst->_mgoal = 1;
	} else if(Monst->_mgoal == 4 || (abs(mx) >= 4 || abs(my) >= 4) && random(115, 4) == 0) {
		if(Monst->_mgoal != 4) {
			Monst->_mgoalvar1 = 0;
			Monst->_mgoalvar2 = random(116, 2);
		}
		Monst->_mgoal = 4;
		if(abs(mx) > abs(my)) {
			dist = abs(mx);
		} else {
			dist = abs(my);
		}
		if(Monst->_mgoalvar1++ >= 2 * dist && DirOK(i, md) || dTransVal[Monst->_mx][Monst->_my] != dTransVal[fx][fy]) {
			Monst->_mgoal = 1;
		} else if(!M_RoundWalk(i, md, Monst->_mgoalvar2)) {
			M_StartDelay(i, random(125, 10) + 10);
		}
	}

	if(Monst->_mgoal == 1) {
		if(abs(mx) < 2 && abs(my) < 2) {
			if(v < 2 * Monst->_mint + 23) {
				Monst->_mdir = md;
				if(special && Monst->_mhitpoints < Monst->_mmaxhp >> 1 && random(117, 2)) {
					M_StartSpAttack(i);
				} else {
					M_StartAttack(i);
				}
			}
		} else if(Monst->_mVar2 > 20 && v < 2 * Monst->_mint + 28
		|| (Monst->_mVar1 == 1 || Monst->_mVar1 == 2 || Monst->_mVar1 == 3)
		&& Monst->_mVar2 == 0
		&& v < 2 * Monst->_mint + 78) {
			M_CallWalk(i, md);
		}
	}
	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_GoatMc(int i)
{
	MAI_Round(i, TRUE);
}

void MAI_Ranged(int i, int missile_type, BOOL special)
{
	int fx, fy, mx, my, md;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Ranged: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND) {
		return;
	}

	if(Monst->_msquelch == 255 || monster[i]._mFlags & 0x10) {
		fx = Monst->_menemyx;
		fy = Monst->_menemyy;
		mx = Monst->_mx - fx;
		my = Monst->_my - fy;
		md = M_GetDir(i);
		if(Monst->_msquelch < 255) {
			MonstCheckDoors(i);
		}
		Monst->_mdir = md;
		if(Monst->_mVar1 == 10) {
			M_StartDelay(i, random(118, 20));
		} else if(abs(mx) < 4 && abs(my) < 4) {
			if(random(119, 100) < 10 * (Monst->_mint + 7)) {
				M_CallWalk(i, opposite[md]);
			}
		}
		if(Monst->_mmode == MM_STAND) {
			if(LineClear(Monst->_mx, Monst->_my, fx, fy)) {
				if(special) {
					M_StartRSpAttack(i, missile_type, 4);
				} else {
					M_StartRAttack(i, missile_type, 4);
				}
			} else {
				Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
			}
		}
	} else if(Monst->_msquelch != 0) {
		mx = Monst->_lastx;
		my = Monst->_lasty;
		md = GetDirection(Monst->_mx, Monst->_my, mx, my);
		M_CallWalk(i, md);
	}
}

void MAI_GoatBow(int i)
{
	MAI_Ranged(i, MIS_ARROW, FALSE);
}

void MAI_Succ(int i)
{
	MAI_Ranged(i, MIS_FLARE, FALSE);
}

void MAI_AcidUniq(int i)
{
	MAI_Ranged(i, MIS_ACID, TRUE);
}

void MAI_Scav(int i)
{
	MonsterStruct *Monst;
	int x, y;
	BOOL done;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Scav: Invalid monster %d", i);
	}

	Monst = &monster[i];
	done = FALSE;
	if(Monst->_mmode != MM_STAND) {
		return;
	}

	if(Monst->_mhitpoints < Monst->_mmaxhp >> 1 && Monst->_mgoal != 3) {
		if(monster[i].leaderflag != 0) {
			monster[monster[i].leader].packsize--;
			monster[i].leaderflag = 0;
		}
		Monst->_mgoal = 3;
		Monst->_mgoalvar3 = 10;
	}
	if(Monst->_mgoal == 3 && Monst->_mgoalvar3 != 0) {
		Monst->_mgoalvar3--;
		if(dDead[Monst->_mx][Monst->_my] != 0) {
			M_StartEat(i);
			if(!(monster[i]._mFlags & 8)) {
				Monst->_mhitpoints += 64;
			}
			if(Monst->_mhitpoints >= (Monst->_mmaxhp >> 1) + (Monst->_mmaxhp >> 2)) {
				Monst->_mgoal = 1;
				Monst->_mgoalvar1 = 0;
				Monst->_mgoalvar2 = 0;
			}
		} else {
			if(Monst->_mgoalvar1 == 0) {
				if(random(120, 2) != 0) {
					for(y = -4; y <= 4 && !done; y++) {
						for(x = -4; x <= 4 && !done; x++) {
							if(y >= 0 && y < MAXDUNY && x >= 0 && x < MAXDUNX) {
								done = dDead[x + Monst->_mx][y + Monst->_my] != 0
								&& LineClearF(CheckNoSolid, Monst->_mx, Monst->_my, x + Monst->_mx, y + Monst->_my);
							}
						}
					}
					x--;
					y--;
				} else {
					for(y = 4; y >= -4 && !done; y--) {
						for(x = 4; x >= -4 && !done; x--) {
							if(y >= 0 && y < MAXDUNY && x >= 0 && x < MAXDUNX) {
								done = dDead[x + Monst->_mx][y + Monst->_my] != 0
								&& LineClearF(CheckNoSolid, Monst->_mx, Monst->_my, x + Monst->_mx, y + Monst->_my);
							}
						}
					}
					x++;
					y++;
				}
				if(done) {
					Monst->_mgoalvar1 = Monst->_mx + x + 1;
					Monst->_mgoalvar2 = Monst->_my + y + 1;
				}
			}
			if(Monst->_mgoalvar1 != 0) {
				x = Monst->_mgoalvar1 - 1;
				y = Monst->_mgoalvar2 - 1;
				Monst->_mdir = GetDirection(Monst->_mx, Monst->_my, x, y);
				M_CallWalk(i, Monst->_mdir);
			}
		}
	}
	if(Monst->_mmode == MM_STAND) {
		MAI_SkelSd(i);
	}
}

void MAI_Garg(int i)
{
	MonsterStruct *Monst;
	int mx, my, md;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Garg: Invalid monster %d", i);
	}

	Monst = &monster[i];
	mx = Monst->_mx - Monst->_lastx;
	my = Monst->_my - Monst->_lasty;
	md = M_GetDir(i);

	if(Monst->_msquelch != 0 && Monst->_mFlags & 4) {
		M_Enemy(i);
		mx = Monst->_mx - Monst->_menemyx;
		my = Monst->_my - Monst->_menemyy;
		if(abs(mx) < Monst->_mint + 2 && abs(my) < Monst->_mint + 2) {
			Monst->_mFlags &= ~4;
		}
		return;
	}

	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	if(Monst->_mhitpoints < Monst->_mmaxhp >> 1 && !(Monst->_mFlags & 8)) {
		Monst->_mgoal = 2;
	}
	if(Monst->_mgoal == 2) {
		if(abs(mx) < Monst->_mint + 2 && abs(my) < Monst->_mint + 2) {
			if(!M_CallWalk(i, opposite[md])) {
				Monst->_mgoal = 1;
			}
		} else {
			Monst->_mgoal = 1;
			M_StartHeal(i);
		}
	}

	MAI_Round(i, FALSE);
}

void MAI_RoundRanged(int i, int missile_type, BOOL checkdoors, int dam, int lessmissiles)
{
	int fx, fy, mx, my, md, v, dist;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_RoundRanged: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	fx = Monst->_menemyx;
	fy = Monst->_menemyy;
	mx = Monst->_mx - fx;
	my = Monst->_my - fy;
	md = GetDirection(Monst->_mx, Monst->_my, Monst->_lastx, Monst->_lasty);

	if(checkdoors && Monst->_msquelch < 255) {
		MonstCheckDoors(i);
	}

	v = random(121, 10000);

	if(abs(mx) < 2 && abs(my) < 2 || Monst->_msquelch != 255 || dTransVal[Monst->_mx][Monst->_my] != dTransVal[fx][fy]) {
		Monst->_mgoal = 1;
	} else if(Monst->_mgoal == 4 || (abs(mx) >= 3 || abs(my) >= 3) && random(122, 4 << lessmissiles) == 0) {
		if(Monst->_mgoal != 4) {
			Monst->_mgoalvar1 = 0;
			Monst->_mgoalvar2 = random(123, 2);
		}
		Monst->_mgoal = 4;
		if(abs(mx) > abs(my)) {
			dist = abs(mx);
		} else {
			dist = abs(my);
		}
		if(Monst->_mgoalvar1++ >= 2 * dist && DirOK(i, md)) {
			Monst->_mgoal = 1;
		} else if(v < 500 * (Monst->_mint + 1) >> lessmissiles && LineClear(Monst->_mx, Monst->_my, fx, fy)) {
			M_StartRSpAttack(i, missile_type, dam);
		} else {
			M_RoundWalk(i, md, Monst->_mgoalvar2);
		}
	}

	if(Monst->_mgoal == 1) {
		if(((abs(mx) >= 3 || abs(my) >= 3) && v < 500 * (Monst->_mint + 2) >> lessmissiles
		|| v < 500 * (Monst->_mint + 1) >> lessmissiles)
		&& LineClear(Monst->_mx, Monst->_my, fx, fy)) {
			M_StartRSpAttack(i, missile_type, dam);
		} else if(abs(mx) < 2 && abs(my) < 2) {
			if(v < 1000 * (Monst->_mint + 6)) {
				Monst->_mdir = md;
				M_StartAttack(i);
			}
		} else {
			v = random(124, 100);
			if(v < 1000 * (Monst->_mint + 5)
			|| (Monst->_mVar1 == 1 || Monst->_mVar1 == 2 || Monst->_mVar1 == 3) && Monst->_mVar2 == 0 && v < 1000 * (Monst->_mint + 8)) {
				M_CallWalk(i, md);
			}
		}
	}
	if(Monst->_mmode == MM_STAND) {
		M_StartDelay(i, random(125, 10) + 5);
	}
}

void MAI_Magma(int i)
{
	MAI_RoundRanged(i, MIS_MAGMABALL, TRUE, 4, 0);
}

void MAI_Storm(int i)
{
	MAI_RoundRanged(i, MIS_LIGHTCTRL2, TRUE, 4, 0);
}

void MAI_Acid(int i)
{
	MAI_RoundRanged(i, MIS_ACID, FALSE, 4, 1);
}

void MAI_Diablo(int i)
{
	MAI_RoundRanged(i, MIS_DIABAPOCA, FALSE, 40, 0);
}

void MAI_RR2(int i, int mistype, int dam)
{
	int fx, fy, mx, my, md, v, dist;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_RR2: Invalid monster %d", i);
	}

	Monst = &monster[i];
	mx = Monst->_mx - Monst->_menemyx;
	my = Monst->_my - Monst->_menemyy;
	if(abs(mx) >= 5 || abs(my) >= 5) {
		MAI_SkelSd(i);
		return;
	}
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	fx = Monst->_menemyx;
	fy = Monst->_menemyy;
	mx = Monst->_mx - fx;
	my = Monst->_my - fy;
	md = GetDirection(Monst->_mx, Monst->_my, Monst->_lastx, Monst->_lasty);

	if(Monst->_msquelch < 255) {
		MonstCheckDoors(i);
	}

	v = random(121, 100);

	if(abs(mx) < 2 && abs(my) < 2 || Monst->_msquelch != 255 || dTransVal[Monst->_mx][Monst->_my] != dTransVal[fx][fy]) {
		Monst->_mgoal = 1;
	} else if(Monst->_mgoal == 4 || abs(mx) >= 3 || abs(my) >= 3) {
		if(Monst->_mgoal != 4) {
			Monst->_mgoalvar1 = 0;
			Monst->_mgoalvar2 = random(123, 2);
		}
		Monst->_mgoal = 4;
		Monst->_mgoalvar3 = 4;
		if(abs(mx) > abs(my)) {
			dist = abs(mx);
		} else {
			dist = abs(my);
		}
		if(Monst->_mgoalvar1++ >= 2 * dist && DirOK(i, md)) {
			Monst->_mgoal = 1;
		} else if(v < 5 * (Monst->_mint + 16)) {
			M_RoundWalk(i, md, Monst->_mgoalvar2);
		}
	}

	if(Monst->_mgoal == 1) {
		if(((abs(mx) >= 3 || abs(my) >= 3) && v < 5 * (Monst->_mint + 2) || v < 5 * (Monst->_mint + 1) || Monst->_mgoalvar3 == 4)
		&& LineClear(Monst->_mx, Monst->_my, fx, fy)) {
			M_StartRSpAttack(i, mistype, dam);
		} else if(abs(mx) < 2 && abs(my) < 2) {
			v = random(124, 100);
			if(v < 10 * (Monst->_mint + 4)) {
				Monst->_mdir = md;
				if(random(124, 2) != 0) {
					M_StartAttack(i);
				} else {
					M_StartRSpAttack(i, mistype, dam);
				}
			}
		} else {
			v = random(124, 100);
			if(v < 2 * (5 * Monst->_mint + 25)
			|| (Monst->_mVar1 == 1 || Monst->_mVar1 == 2 || Monst->_mVar1 == 3) && Monst->_mVar2 == 0 && v < 2 * (5 * Monst->_mint + 40)) {
				M_CallWalk(i, md);
			}
		}
		Monst->_mgoalvar3 = 1;
	}
	if(Monst->_mmode == MM_STAND) {
		M_StartDelay(i, random(125, 10) + 5);
	}
}

void MAI_Mega(int i)
{
	MAI_RR2(i, MIS_FLAMEC, 0);
}

void MAI_Golum(int i)
{
	int j, k, mid, mx, my, md;
	BOOL ok, have_enemy;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Golum: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mx == 1 && Monst->_my == 0) {
		return;
	}
	if(Monst->_mmode == MM_DEATH || Monst->_mmode == MM_SPSTAND) {
		return;
	}
	if(Monst->_mmode >= MM_WALK && Monst->_mmode <= MM_WALK3) {
		return;
	}

	if(!(monster[i]._mFlags & 0x10)) {
		M_Enemy(i);
	}

	have_enemy = !(Monst->_mFlags & 0x400);

	if(Monst->_mmode == MM_ATTACK) {
		return;
	}

	mx = Monst->_mx - monster[Monst->_menemy]._mfutx;
	my = Monst->_my - monster[Monst->_menemy]._mfuty;
	md = GetDirection(Monst->_mx, Monst->_my, monster[Monst->_menemy]._mx, monster[Monst->_menemy]._my);
	Monst->_mdir = md;

	if(abs(mx) < 2 && abs(my) < 2) {
		if(have_enemy) {
			Monst->_menemyx = monster[Monst->_menemy]._mx;
			Monst->_menemyy = monster[Monst->_menemy]._my;
			if(monster[Monst->_menemy]._msquelch == 0) {
				monster[Monst->_menemy]._msquelch = 255;
				monster[Monst->_menemy]._lastx = Monst->_mx;
				monster[Monst->_menemy]._lasty = Monst->_my;
				for(k = 0; k < 5; k++) {
					for(j = 0; j < 5; j++) {
						mid = dMonster[j + monster[i]._mx - 2][k + monster[i]._my - 2];
						if(mid > 0) {
							monster[mid]._msquelch = 255;
						}
					}
				}
			}
			M_StartAttack(i);
			return;
		}
	} else if(have_enemy && MAI_Path(i)) {
		return;
	}

	Monst->_pathcount++;
	if(Monst->_pathcount > 8) {
		Monst->_pathcount = 5;
	}

	ok = M_CallWalk(i, plr[i]._pdir);
	if(!ok) {
		md = (md - 1) & 7;
		for(j = 0; j < 8 && !ok; j++) {
			md = (md + 1) & 7;
			ok = DirOK(i, md);
		}
		if(ok) {
			M_WalkDir(i, md);
		}
	}
}

void MAI_SkelKing(int i)
{
	int fx, fy, mx, my, md, v, dist, nx, ny, skel;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_SkelKing: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	fx = Monst->_menemyx;
	fy = Monst->_menemyy;
	mx = Monst->_mx - fx;
	my = Monst->_my - fy;
	md = GetDirection(Monst->_mx, Monst->_my, Monst->_lastx, Monst->_lasty);

	if(Monst->_msquelch < 255) {
		MonstCheckDoors(i);
	}

	v = random(126, 100);

	if(abs(mx) < 2 && abs(my) < 2 || Monst->_msquelch != 255 || dTransVal[Monst->_mx][Monst->_my] != dTransVal[fx][fy]) {
		Monst->_mgoal = 1;
	} else if(Monst->_mgoal == 4 || (abs(mx) >= 3 || abs(my) >= 3) && random(127, 4) == 0) {
		if(Monst->_mgoal != 4) {
			Monst->_mgoalvar1 = 0;
			Monst->_mgoalvar2 = random(128, 2);
		}
		Monst->_mgoal = 4;
		if(abs(mx) > abs(my)) {
			dist = abs(mx);
		} else {
			dist = abs(my);
		}
		if(Monst->_mgoalvar1++ >= 2 * dist && DirOK(i, md) || dTransVal[Monst->_mx][Monst->_my] != dTransVal[fx][fy]) {
			Monst->_mgoal = 1;
		} else if(!M_RoundWalk(i, md, Monst->_mgoalvar2)) {
			M_StartDelay(i, random(125, 10) + 10);
		}
	}

	if(Monst->_mgoal == 1) {
		if(gbMaxPlayers == 1
		&& ((abs(mx) >= 3 || abs(my) >= 3) && v < 4 * Monst->_mint + 35 || v < 6)
		&& LineClear(Monst->_mx, Monst->_my, fx, fy)) {
			nx = Monst->_mx + offset_x[md];
			ny = Monst->_my + offset_y[md];
			if(PosOkMonst(i, nx, ny) && nummonsters < MAXMONSTERS) {
				skel = M_SpawnSkel(nx, ny, md); /// BUGFIX: was 'skel' to be used?
				M_StartSpStand(i, md);
			}
		} else if(abs(mx) < 2 && abs(my) < 2) {
			if(v < Monst->_mint + 20) {
				Monst->_mdir = md;
				M_StartAttack(i);
			}
		} else {
			v = random(129, 100);
			if(v < Monst->_mint + 25
			|| (Monst->_mVar1 == 1 || Monst->_mVar1 == 2 || Monst->_mVar1 == 3) && Monst->_mVar2 == 0 && v < Monst->_mint + 75) {
				M_CallWalk(i, md);
			} else {
				M_StartDelay(i, random(130, 10) + 10);
			}
		}
	}
	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_Rhino(int i)
{
	int fx, fy, mx, my, md, v, dist;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Rhino: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	fx = Monst->_menemyx;
	fy = Monst->_menemyy;
	mx = Monst->_mx - fx;
	my = Monst->_my - fy;
	md = GetDirection(Monst->_mx, Monst->_my, Monst->_lastx, Monst->_lasty);

	if(Monst->_msquelch < 255) {
		MonstCheckDoors(i);
	}

	v = random(131, 100);

	if(abs(mx) < 2 && abs(my) < 2) {
		Monst->_mgoal = 1;
	} else if(Monst->_mgoal == 4 || (abs(mx) >= 5 || abs(my) >= 5) && random(132, 4) != 0) {
		if(Monst->_mgoal != 4) {
			Monst->_mgoalvar1 = 0;
			Monst->_mgoalvar2 = random(133, 2);
		}
		Monst->_mgoal = 4;
		if(abs(mx) > abs(my)) {
			dist = abs(mx);
		} else {
			dist = abs(my);
		}
		if(Monst->_mgoalvar1++ >= 2 * dist || dTransVal[Monst->_mx][Monst->_my] != dTransVal[fx][fy]) {
			Monst->_mgoal = 1;
		} else if(!M_RoundWalk(i, md, Monst->_mgoalvar2)) {
			M_StartDelay(i, random(125, 10) + 10);
		}
	}

	if(Monst->_mgoal == 1) {
		if((abs(mx) >= 5 || abs(my) >= 5)
		&& v < 2 * Monst->_mint + 43
		&& LineClearF1(PosOkMonst, i, Monst->_mx, Monst->_my, fx, fy)) {
			if(AddMissile(Monst->_mx, Monst->_my, fx, fy, md, MIS_RHINO, Monst->_menemy, i, 0, 0) != -1) {
				if(Monst->MData->snd_special) {
					PlayEffect(i, 3);
				}
				dMonster[Monst->_mx][Monst->_my] = -(i + 1);
				Monst->_mmode = MM_CHARGE;
			}
		} else if(abs(mx) < 2 && abs(my) < 2) {
			if(v < 2 * Monst->_mint + 28) {
				Monst->_mdir = md;
				M_StartAttack(i);
			}
		} else {
			v = random(134, 100);
			if(v < 2 * Monst->_mint + 33
			|| (Monst->_mVar1 == 1 || Monst->_mVar1 == 2 || Monst->_mVar1 == 3) && Monst->_mVar2 == 0 && v < 2 * Monst->_mint + 83) {
				M_CallWalk(i, md);
			} else {
				M_StartDelay(i, random(135, 10) + 10);
			}
		}
	}
	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[Monst->_mdir]; /// BUGFIX: use 'md'?
	}
}

void MAI_Counselor(int i)
{
	static const BYTE counsmiss[4] = { MIS_FIREBOLT, MIS_CBOLT, MIS_LIGHTCTRL, MIS_FIREBALL };
	int fx, fy, mx, my, md, v, dist;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Counselor: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND || Monst->_msquelch == 0) {
		return;
	}

	fx = Monst->_menemyx;
	fy = Monst->_menemyy;
	mx = Monst->_mx - fx;
	my = Monst->_my - fy;
	md = GetDirection(Monst->_mx, Monst->_my, Monst->_lastx, Monst->_lasty);

	if(Monst->_msquelch < 255) {
		MonstCheckDoors(i);
	}

	v = random(121, 100);

	if(Monst->_mgoal == 2) {
		if(Monst->_mgoalvar1++ > 3) {
			Monst->_mgoal = 1;
			M_StartFadein(i, md, TRUE);
		} else {
			M_CallWalk(i, opposite[md]);
		}
	} else if(Monst->_mgoal == 4) {
		if(abs(mx) > abs(my)) {
			dist = abs(mx);
		} else {
			dist = abs(my);
		}
		if(abs(mx) < 2 && abs(my) < 2 || Monst->_msquelch != 255 || dTransVal[Monst->_mx][Monst->_my] != dTransVal[fx][fy]) {
			Monst->_mgoal = 1;
			M_StartFadein(i, md, TRUE);
		} else if(Monst->_mgoalvar1++ >= 2 * dist && DirOK(i, md)) {
			Monst->_mgoal = 1;
			M_StartFadein(i, md, TRUE);
		} else {
			M_RoundWalk(i, md, Monst->_mgoalvar2);
		}
	} else if(Monst->_mgoal == 1) {
		if(abs(mx) < 2 && abs(my) < 2) {
			Monst->_mdir = md;
			if(Monst->_mhitpoints < Monst->_mmaxhp >> 1) {
				Monst->_mgoal = 2;
				Monst->_mgoalvar1 = 0;
				M_StartFadeout(i, md, FALSE);
			} else if(Monst->_mVar1 == 13 || random(105, 100) < 2 * Monst->_mint + 20) {
				M_StartRAttack(i, -1, 0);
				AddMissile(monster[i]._mx, monster[i]._my, 0, 0, monster[i]._mdir, MIS_FLASH, 1, i, 4, 0);
				AddMissile(monster[i]._mx, monster[i]._my, 0, 0, monster[i]._mdir, MIS_FLASH2, 1, i, 4, 0);
			} else {
				M_StartDelay(i, random(105, 10) + 2 * (5 - Monst->_mint));
			}
		} else if(v < 5 * (Monst->_mint + 10) && LineClear(Monst->_mx, Monst->_my, fx, fy)) {
			M_StartRAttack(i, counsmiss[Monst->_mint], random(77, Monst->mMaxDamage - Monst->mMinDamage + 1) + Monst->mMinDamage);
		} else {
			v = random(124, 100);
			if(v < 30) {
				Monst->_mgoal = 4;
				Monst->_mgoalvar1 = 0;
				M_StartFadeout(i, md, FALSE);
			} else {
				M_StartDelay(i, random(105, 10) + 2 * (5 - Monst->_mint));
			}
		}
	}

	if(Monst->_mmode == MM_STAND) {
		M_StartDelay(i, random(125, 10) + 5);
	}
}

void MAI_Garbud(int i)
{
	int mx, my, md;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Garbud: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND) {
		return;
	}

	mx = Monst->_mx;
	my = Monst->_my;
	md = M_GetDir(i);

	if(Monst->mtalkmsg < TEXT_GARBUD4 && Monst->mtalkmsg > TEXT_GARBUD1 - 1 && !(dFlags[mx][my] & 2) && Monst->_mgoal == 7) {
		Monst->mtalkmsg++;
		Monst->_mgoal = 6;
	}
	if(dFlags[mx][my] & 2) {
		if(Monst->mtalkmsg == TEXT_GARBUD4 && !effect_is_playing(USFX_GARBUD4) && Monst->_mgoal == 7) {
			Monst->_mgoal = 1;
			Monst->_msquelch = 255;
			Monst->mtalkmsg = 0;
		}
	}
	if(Monst->_mgoal == 1 || Monst->_mgoal == 4) {
		MAI_Round(i, TRUE);
	}

	monster[i]._mdir = md;

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_Zhar(int i)
{
	int mx, my, md, dist;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Zhar: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND) {
		return;
	}

	mx = Monst->_mx;
	my = Monst->_my;
	md = M_GetDir(i);

	if(Monst->mtalkmsg == TEXT_ZHAR1 && !(dFlags[mx][my] & 2) && Monst->_mgoal == 7) {
		Monst->mtalkmsg++;
		Monst->_mgoal = 6;
	}
	if(dFlags[mx][my] & 2) {
		mx = Monst->_mx - Monst->_menemyx;
		my = Monst->_my - Monst->_menemyy;
		if(abs(mx) > abs(my)) { /// BUGFIX: leftover unused code
			dist = abs(mx);
		} else {
			dist = abs(my);
		}
		if(Monst->mtalkmsg == TEXT_ZHAR2 && !effect_is_playing(USFX_ZHAR2) && Monst->_mgoal == 7) {
			Monst->_mgoal = 1;
			Monst->_msquelch = 255;
			Monst->mtalkmsg = 0;
		}
	}
	if(Monst->_mgoal == 1 || Monst->_mgoal == 2 || Monst->_mgoal == 4) {
		MAI_Counselor(i);
	}

	monster[i]._mdir = md;

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_SnotSpil(int i)
{
	int mx, my, md;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_SnotSpil: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND) {
		return;
	}

	mx = Monst->_mx;
	my = Monst->_my;
	md = M_GetDir(i);

	if(Monst->mtalkmsg == TEXT_BANNER10 && !(dFlags[mx][my] & 2) && Monst->_mgoal == 7) {
		Monst->mtalkmsg++;
		Monst->_mgoal = 6;
	}
	if(Monst->mtalkmsg == TEXT_BANNER11 && quests[Q_LTBANNER]._qvar1 == 3) {
		Monst->mtalkmsg = 0;
		Monst->_mgoal = 1;
	}
	if(dFlags[mx][my] & 2) {
		if(Monst->mtalkmsg == TEXT_BANNER12 && !effect_is_playing(USFX_SNOT3) && Monst->_mgoal == 7) {
			ObjChangeMap(setpc_x, setpc_y, setpc_x + setpc_w + 1, setpc_y + setpc_h + 1);
			quests[Q_LTBANNER]._qvar1 = 3;
			RedoPlayerVision();
			Monst->_mgoal = 1;
			Monst->_msquelch = 255;
			Monst->mtalkmsg = 0;
		}
		if(quests[Q_LTBANNER]._qvar1 == 3) {
			if(Monst->_mgoal == 1 || Monst->_mgoal == 5) {
				MAI_Fallen(i);
			}
		}
	}

	monster[i]._mdir = md;

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_Lazurus(int i)
{
	int mx, my, md;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Lazurus: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND) {
		return;
	}

	mx = Monst->_mx;
	my = Monst->_my;
	md = M_GetDir(i);

	if(dFlags[mx][my] & 2) {
		if(gbMaxPlayers == 1) {
			if(Monst->mtalkmsg == TEXT_VILE13 && Monst->_mgoal == 6 && plr[myplr].WorldX == 35 && plr[myplr].WorldY == 46) {
				PlayInGameMovie("gendata\\fprst3.smk");
				Monst->_mmode = MM_TALK;
				quests[Q_BETRAYER]._qvar1 = 5;
			}
			if(Monst->mtalkmsg == TEXT_VILE13 && !effect_is_playing(USFX_LAZ1) && Monst->_mgoal == 7) {
				ObjChangeMapResync(1, 18, 20, 24);
				RedoPlayerVision();
				quests[Q_BETRAYER]._qvar1 = 6;
				Monst->_mgoal = 1;
				Monst->_msquelch = 255;
				Monst->mtalkmsg = 0;
			}
		}
		if(gbMaxPlayers != 1) {
			if(Monst->mtalkmsg == TEXT_VILE13 && Monst->_mgoal == 6 && quests[Q_BETRAYER]._qvar1 <= 3) {
				Monst->_mmode = MM_TALK;
			}
		}
	}
	if(Monst->_mgoal == 1 || Monst->_mgoal == 2 || Monst->_mgoal == 4) {
		Monst->mtalkmsg = 0;
		MAI_Counselor(i);
	}

	monster[i]._mdir = md;

	if(Monst->_mmode == MM_STAND || Monst->_mmode == MM_TALK) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_Lazhelp(int i)
{
	int mx, my;
	volatile int md; /* fix */
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Lazhelp: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND) {
		return;
	}

	mx = Monst->_mx;
	my = Monst->_my;
	md = M_GetDir(i);

	if(dFlags[mx][my] & 2) {
		if(gbMaxPlayers == 1) {
			if(quests[Q_BETRAYER]._qvar1 <= 5) {
				Monst->_mgoal = 6;
			} else {
				Monst->_mgoal = 1;
				Monst->mtalkmsg = 0;
			}
		} else {
			Monst->_mgoal = 1;
		}
	}
	if(Monst->_mgoal == 1) {
		MAI_Succ(i);
	}

	monster[i]._mdir = md;

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_Lachdanan(int i)
{
	int mx, my, md;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Lachdanan: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND) {
		return;
	}

	mx = Monst->_mx;
	my = Monst->_my;
	md = M_GetDir(i);

	if(Monst->mtalkmsg == TEXT_VEIL9 && !(dFlags[mx][my] & 2) && Monst->_mgoal == 7) {
		Monst->mtalkmsg++;
		Monst->_mgoal = 6;
	}
	if(dFlags[mx][my] & 2) {
		if(Monst->mtalkmsg == TEXT_VEIL11 && !effect_is_playing(USFX_LACH3) && Monst->_mgoal == 7) {
			Monst->mtalkmsg = 0;
			quests[Q_VEIL]._qactive = 3;
			M_StartKill(i, -1);
		}
	}

	monster[i]._mdir = md;

	if(Monst->_mmode == MM_STAND) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void MAI_Warlord(int i)
{
	int mx, my, md;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("MAI_Warlord: Invalid monster %d", i);
	}

	Monst = &monster[i];
	if(Monst->_mmode != MM_STAND) {
		return;
	}

	mx = Monst->_mx;
	my = Monst->_my;
	md = M_GetDir(i);

	if(dFlags[mx][my] & 2) {
		if(Monst->mtalkmsg == TEXT_WARLRD9 && Monst->_mgoal == 6) {
			Monst->_mmode = MM_TALK;
		}
		if(Monst->mtalkmsg == TEXT_WARLRD9 && !effect_is_playing(USFX_WARLRD1) && Monst->_mgoal == 7) {
			Monst->_mgoal = 1;
			Monst->_msquelch = 255;
			Monst->mtalkmsg = 0;
		}
	}
	if(Monst->_mgoal == 1) {
		MAI_SkelSd(i);
	}

	monster[i]._mdir = md;

	if(Monst->_mmode == MM_STAND || Monst->_mmode == MM_TALK) {
		Monst->_mAnimData = Monst->MType->Anims[0].Frames[md];
	}
}

void DeleteMonsterList()
{
	int i, mi;

	for(i = 0; i < 4; i++) {
		if(monster[i]._mDelFlag) {
			monster[i]._mx = 1;
			monster[i]._my = 0;
			monster[i]._mfutx = 0;
			monster[i]._mfuty = 0;
			monster[i]._moldx = 0;
			monster[i]._moldy = 0;
			monster[i]._mDelFlag = 0;
		}
	}

	i = 4;
	/// ASSERT: assert((DWORD)nummonsters <= MAXMONSTERS);
	while(i < nummonsters) {
		mi = monstactive[i];
		if(monster[mi]._mDelFlag) {
			DeleteMonster(i);
			i = 0;
		} else {
			i++;
		}
	}
}

void ProcessMonsters()
{
	int i, mi, mx, my;
	BOOL raflag;
	MonsterStruct *Monst;

	DeleteMonsterList();

	/// ASSERT: assert((DWORD)nummonsters <= MAXMONSTERS);
	for(i = 0; i < nummonsters; i++) {
		mi = monstactive[i];
		Monst = &monster[mi];
		raflag = FALSE;
		if(gbMaxPlayers > 1) {
			SetRndSeed(Monst->_mAISeed);
			Monst->_mAISeed = GetRndSeed();
		}
		if(!(monster[mi]._mFlags & 8) && Monst->_mhitpoints < Monst->_mmaxhp && Monst->_mhitpoints >> 6 > 0) {
			if(Monst->mLevel <= 1) {
				Monst->_mhitpoints += Monst->mLevel;
			} else {
				Monst->_mhitpoints += Monst->mLevel >> 1;
			}
		}
		mx = Monst->_mx;
		my = Monst->_my;
		if(dFlags[mx][my] & 2 && Monst->_msquelch == 0 && Monst->MType->mtype == MT_CLEAVER) {
			PlaySFX(USFX_CLEAVER);
		}
		if(Monst->_mFlags & 0x10) {
			if((DWORD)Monst->_menemy >= MAXMONSTERS) {
				app_fatal("Illegal enemy monster %d for monster \"%s\"", Monst->_menemy, Monst->mName);
			}
			Monst->_lastx = monster[Monst->_menemy]._mfutx;
			Monst->_menemyx = Monst->_lastx;
			Monst->_lasty = monster[Monst->_menemy]._mfuty;
			Monst->_menemyy = Monst->_lasty;
		} else {
			if((DWORD)Monst->_menemy >= MAX_PLRS) {
				app_fatal("Illegal enemy player %d for monster \"%s\"", Monst->_menemy, Monst->mName);
			}
			Monst->_menemyx = plr[Monst->_menemy]._px;
			Monst->_menemyy = plr[Monst->_menemy]._py;
			if(dFlags[mx][my] & 2) {
				Monst->_msquelch = 255;
				Monst->_lastx = plr[Monst->_menemy]._px;
				Monst->_lasty = plr[Monst->_menemy]._py;
			} else if(Monst->_msquelch != 0 && Monst->_mAi != MT_DIABLO) { /// BUGFIX: change '_mAi' to 'MType->mtype'
				Monst->_msquelch--;
			}
		}
		do {
			if(!(Monst->_mFlags & 0x100)) {
				AiProc[Monst->_mAi](mi);
			} else if(!MAI_Path(mi)) {
				AiProc[Monst->_mAi](mi);
			}
			switch(Monst->_mmode) {
			case MM_STAND:
				raflag = M_DoStand(mi);
				break;
			case MM_WALK:
				raflag = M_DoWalk(mi);
				break;
			case MM_WALK2:
				raflag = M_DoWalk2(mi);
				break;
			case MM_WALK3:
				raflag = M_DoWalk3(mi);
				break;
			case MM_ATTACK:
				raflag = M_DoAttack(mi);
				break;
			case MM_RATTACK:
				raflag = M_DoRAttack(mi);
				break;
			case MM_GOTHIT:
				raflag = M_DoGotHit(mi);
				break;
			case MM_DEATH:
				raflag = M_DoDeath(mi);
				break;
			case MM_SATTACK:
				raflag = M_DoSAttack(mi);
				break;
			case MM_FADEIN:
				raflag = M_DoFadein(mi);
				break;
			case MM_FADEOUT:
				raflag = M_DoFadeout(mi);
				break;
			case MM_SPSTAND:
				raflag = M_DoSpStand(mi);
				break;
			case MM_RSPATTACK:
				raflag = M_DoRSpAttack(mi);
				break;
			case MM_DELAY:
				raflag = M_DoDelay(mi);
				break;
			case MM_CHARGE:
				raflag = FALSE;
				break;
			case MM_STONE:
				raflag = M_DoStone(mi);
				break;
			case MM_HEAL:
				raflag = M_DoHeal(mi);
				break;
			case MM_TALK:
				raflag = M_DoTalk(mi);
				break;
			}
			if(raflag) {
				GroupUnity(mi);
			}
		} while(raflag);
		if(Monst->_mmode != MM_STONE) {
			Monst->_mAnimCnt++;
			if(!(Monst->_mFlags & 4) && Monst->_mAnimCnt >= Monst->_mAnimDelay) {
				Monst->_mAnimCnt = 0;
				if(Monst->_mFlags & 2) {
					Monst->_mAnimFrame--;
					if(Monst->_mAnimFrame == 0) {
						Monst->_mAnimFrame = Monst->_mAnimLen;
					}
				} else {
					Monst->_mAnimFrame++;
					if(Monst->_mAnimFrame > Monst->_mAnimLen) {
						Monst->_mAnimFrame = 1;
					}
				}
			}
		}
	}

	DeleteMonsterList();
}

void FreeMonsters()
{
	int i, anim, mtype;

	/// ASSERT: assert((DWORD)nummtypes <= MAX_LVLMTYPES);
	for(i = 0; i < nummtypes; i++) {
		mtype = Monsters[i].mtype;
		for(anim = 0; anim < 6; anim++) {
			if(animletter[anim] != 's' || monsterdata[mtype].has_special) {
				MemFreeDbg(Monsters[i].Anims[anim].CMem);
			}
		}
	}

	FreeMissiles2();
}

BOOL DirOK(int i, int mdir)
{
	long fx, fy;
	int tmp, mcount, x, y;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("DirOK: Invalid monster %d", i);
	}

	fx = monster[i]._mx + offset_x[mdir];
	fy = monster[i]._my + offset_y[mdir];
	if(fy < 0 || fy >= MAXDUNY || fx < 0 || fx >= MAXDUNX) {
		return FALSE;
	}
	if(!PosOkMonst(i, fx, fy)) {
		return FALSE;
	}

	if(mdir == 6) {
		if(SolidLoc(fx, fy + 1)) {
			return FALSE;
		}
		if(dFlags[fx][fy + 1] & 0x10) {
			return FALSE;
		}
	} else if(mdir == 2) {
		if(SolidLoc(fx + 1, fy)) {
			return FALSE;
		}
		if(dFlags[fx + 1][fy] & 0x10) {
			return FALSE;
		}
	} else if(mdir == 4) {
		if(SolidLoc(fx + 1, fy)) {
			return FALSE;
		}
		if(SolidLoc(fx, fy + 1)) {
			return FALSE;
		}
	} else if(mdir == 0) {
		if(SolidLoc(fx - 1, fy)) {
			return FALSE;
		}
		if(SolidLoc(fx, fy - 1)) {
			return FALSE;
		}
	}

	if(monster[i].leaderflag == 1) {
		return abs(fx - monster[monster[i].leader]._mfutx) < 4
			&& abs(fy - monster[monster[i].leader]._mfuty) < 4;
	}
	if(monster[i]._uniqtype == 0 || !(UniqMonst[monster[i]._uniqtype - 1].mUnqAttr & 2)) {
		return TRUE;
	}

	mcount = 0;
	for(x = fx - 3; x <= fx + 3; x++) {
		for(y = fy - 3; y <= fy + 3; y++) {
			if(y >= 0 && y < MAXDUNY && x >= 0 && x < MAXDUNX) {
				tmp = dMonster[x][y];
				if(tmp < 0) {
					tmp = -tmp;
				}
				if(tmp != 0) {
					tmp = tmp - 1;
				}
				/// ASSERT: assert(tmp >= 0);
				if(monster[tmp].leaderflag == 1
				&& monster[tmp].leader == i
				&& monster[tmp]._mfutx == x
				&& monster[tmp]._mfuty == y) {
					mcount++;
				}
			}
		}
	}

	return mcount == monster[i].packsize;
}

BOOL PosOkMissile(int x, int y)
{
	return !nMissileTable[dPiece[x][y]] && !(dFlags[x][y] & 0x10);
}

BOOL CheckNoSolid(int x, int y)
{
	return !nSolidTable[dPiece[x][y]];
}

BOOL LineClearF(BOOL (*Clear)(int, int), int x1, int y1, int x2, int y2)
{
	int dx, dy, d, dincH, dincD, xincD, yincD, xorg, yorg, tmp;
	BOOL done;

	done = FALSE;
	xorg = x1;
	yorg = y1;
	dx = x2 - x1;
	dy = y2 - y1;

	if(abs(dx) > abs(dy)) {
		if(dx < 0) {
			tmp = x1;
			x1 = x2;
			x2 = tmp;
			tmp = y1;
			y1 = y2;
			y2 = tmp;
			dx = -dx;
			dy = -dy;
		}
		if(dy > 0) {
			d = 2 * dy - dx;
			dincH = 2 * dy;
			dincD = 2 * (dy - dx);
			yincD = 1;
		} else {
			d = 2 * dy + dx;
			dincH = 2 * dy;
			dincD = 2 * (dx + dy);
			yincD = -1;
		}
		while(!done && (x1 != x2 || y1 != y2)) {
			if((d <= 0) ^ (yincD < 0)) {
				d += dincH;
			} else {
				d += dincD;
				y1 += yincD;
			}
			x1++;
			done = (x1 != xorg || y1 != yorg) && !Clear(x1, y1);
		}
	} else {
		if(dy < 0) {
			tmp = y1;
			y1 = y2;
			y2 = tmp;
			tmp = x1;
			x1 = x2;
			x2 = tmp;
			dy = -dy;
			dx = -dx;
		}
		if(dx > 0) {
			d = 2 * dx - dy;
			dincH = 2 * dx;
			dincD = 2 * (dx - dy);
			xincD = 1;
		} else {
			d = 2 * dx + dy;
			dincH = 2 * dx;
			dincD = 2 * (dx + dy);
			xincD = -1;
		}
		while(!done && (y1 != y2 || x1 != x2)) {
			if((d <= 0) ^ (xincD < 0)) {
				d += dincH;
			} else {
				d += dincD;
				x1 += xincD;
			}
			y1++;
			done = (y1 != yorg || x1 != xorg) && !Clear(x1, y1);
		}
	}

	return x1 == x2 && y1 == y2;
}

BOOL LineClear(int x1, int y1, int x2, int y2)
{
	return LineClearF(PosOkMissile, x1, y1, x2, y2);
}

BOOL LineClearF1(BOOL (*Clear)(int, int, int), int monst, int x1, int y1, int x2, int y2)
{
	int dx, dy, d, dincH, dincD, xincD, yincD, xorg, yorg, tmp;
	BOOL done;

	done = FALSE;
	xorg = x1;
	yorg = y1;
	dx = x2 - x1;
	dy = y2 - y1;

	if(abs(dx) > abs(dy)) {
		if(dx < 0) {
			tmp = x1;
			x1 = x2;
			x2 = tmp;
			tmp = y1;
			y1 = y2;
			y2 = tmp;
			dx = -dx;
			dy = -dy;
		}
		if(dy > 0) {
			d = 2 * dy - dx;
			dincH = 2 * dy;
			dincD = 2 * (dy - dx);
			yincD = 1;
		} else {
			d = 2 * dy + dx;
			dincH = 2 * dy;
			dincD = 2 * (dx + dy);
			yincD = -1;
		}
		while(!done && (x1 != x2 || y1 != y2)) {
			if((d <= 0) ^ (yincD < 0)) {
				d += dincH;
			} else {
				d += dincD;
				y1 += yincD;
			}
			x1++;
			done = (x1 != xorg || y1 != yorg) && !Clear(monst, x1, y1);
		}
	} else {
		if(dy < 0) {
			tmp = y1;
			y1 = y2;
			y2 = tmp;
			tmp = x1;
			x1 = x2;
			x2 = tmp;
			dy = -dy;
			dx = -dx;
		}
		if(dx > 0) {
			d = 2 * dx - dy;
			dincH = 2 * dx;
			dincD = 2 * (dx - dy);
			xincD = 1;
		} else {
			d = 2 * dx + dy;
			dincH = 2 * dx;
			dincD = 2 * (dx + dy);
			xincD = -1;
		}
		while(!done && (y1 != y2 || x1 != x2)) {
			if((d <= 0) ^ (xincD < 0)) {
				d += dincH;
			} else {
				d += dincD;
				x1 += xincD;
			}
			y1++;
			done = (y1 != yorg || x1 != xorg) && !Clear(monst, x1, y1);
		}
	}

	return x1 == x2 && y1 == y2;
}

void SyncMonsterAnim(int m)
{
	int md;

	if((DWORD)m >= MAXMONSTERS) {
		app_fatal("SyncMonsterAnim: Invalid monster %d", m);
	}

	/// ASSERT: assert((DWORD)monster[m]._mMTidx < MAX_LVLMTYPES);
	monster[m].MType = &Monsters[monster[m]._mMTidx];
	monster[m].MData = Monsters[monster[m]._mMTidx].MData;

	if(monster[m]._uniqtype != 0) {
		monster[m].mName = UniqMonst[monster[m]._uniqtype - 1].mName;
	} else {
		monster[m].mName = monster[m].MData->mName;
	}

	md = monster[m]._mdir;

	switch(monster[m]._mmode) {
	case MM_STAND:
		monster[m]._mAnimData = monster[m].MType->Anims[0].Frames[md];
		break;
	case MM_WALK:
		monster[m]._mAnimData = monster[m].MType->Anims[1].Frames[md];
		break;
	case MM_WALK2:
		monster[m]._mAnimData = monster[m].MType->Anims[1].Frames[md];
		break;
	case MM_WALK3:
		monster[m]._mAnimData = monster[m].MType->Anims[1].Frames[md];
		break;
	case MM_ATTACK:
		monster[m]._mAnimData = monster[m].MType->Anims[2].Frames[md];
		break;
	case MM_RATTACK:
		monster[m]._mAnimData = monster[m].MType->Anims[2].Frames[md];
		break;
	case MM_GOTHIT:
		monster[m]._mAnimData = monster[m].MType->Anims[3].Frames[md];
		break;
	case MM_DEATH:
		monster[m]._mAnimData = monster[m].MType->Anims[4].Frames[md];
		break;
	case MM_SATTACK:
		monster[m]._mAnimData = monster[m].MType->Anims[5].Frames[md];
		break;
	case MM_FADEIN:
		monster[m]._mAnimData = monster[m].MType->Anims[5].Frames[md];
		break;
	case MM_FADEOUT:
		monster[m]._mAnimData = monster[m].MType->Anims[5].Frames[md];
		break;
	case MM_SPSTAND:
		monster[m]._mAnimData = monster[m].MType->Anims[5].Frames[md];
		break;
	case MM_RSPATTACK:
		monster[m]._mAnimData = monster[m].MType->Anims[5].Frames[md];
		break;
	case MM_DELAY:
		monster[m]._mAnimData = monster[m].MType->Anims[0].Frames[md];
		break;
	case MM_HEAL:
		monster[m]._mAnimData = monster[m].MType->Anims[5].Frames[md];
		break;
	case MM_TALK:
		monster[m]._mAnimData = monster[m].MType->Anims[0].Frames[md];
		break;
	case MM_STONE:
		monster[m]._mAnimData = monster[m].MType->Anims[0].Frames[md];
		monster[m]._mAnimFrame = 1;
		monster[m]._mAnimLen = monster[m].MType->Anims[0].Rate;
		break;
	case MM_CHARGE:
		monster[m]._mAnimData = monster[m].MType->Anims[2].Frames[md];
		monster[m]._mAnimFrame = 1;
		monster[m]._mAnimLen = monster[m].MType->Anims[2].Rate;
		break;
	default:
		monster[m]._mAnimData = monster[m].MType->Anims[0].Frames[md];
		monster[m]._mAnimFrame = 1;
		monster[m]._mAnimLen = monster[m].MType->Anims[0].Rate;
		break;
	}
}

void M_FallenFear(int x, int y)
{
	int i, mi, rundist, aitype;

	/// ASSERT: assert((DWORD)nummonsters <= MAXMONSTERS);
	for(i = 0; i < nummonsters; i++) {
		mi = monstactive[i];
		rundist = 0;
		switch(monster[mi].MType->mtype) {
		case MT_RFALLSP:
		case MT_RFALLSD:
			rundist = 7;
			break;
		case MT_DFALLSP:
		case MT_DFALLSD:
			rundist = 5;
			break;
		case MT_YFALLSP:
		case MT_YFALLSD:
			rundist = 3;
			break;
		case MT_BFALLSP:
		case MT_BFALLSD:
			rundist = 2;
			break;
		}
		aitype = monster[mi]._mAi;
		if(aitype != AI_FALLEN || rundist == 0) {
			continue;
		}
		if(abs(x - monster[mi]._mx) < 5 && abs(y - monster[mi]._my) < 5 && monster[mi]._mhitpoints >> 6 > 0) {
			monster[mi]._mgoal = 2;
			monster[mi]._mgoalvar1 = rundist;
			monster[mi]._mdir = GetDirection(x, y, monster[i]._mx, monster[i]._my);
		}
	}
}

void PrintMonstHistory(int mt)
{
	int res, minhp, maxhp;

	sprintf(tempstr, "Total kills : %i", monstkills[mt]);
	AddPanelString(tempstr, 1);

	if(monstkills[mt] >= 30) {
		minhp = monsterdata[mt].mMinHP;
		maxhp = monsterdata[mt].mMaxHP;
		if(gbMaxPlayers == 1) {
			minhp >>= 1;
			maxhp >>= 1;
		}
		if(minhp < 1) {
			minhp = 1;
		}
		if(maxhp < 1) {
			maxhp = 1;
		}
		if(gnDifficulty == DIFF_NIGHTMARE) {
			minhp = 3 * minhp + 1;
			maxhp = 3 * maxhp + 1;
		}
		if(gnDifficulty == DIFF_HELL) {
			minhp = 4 * minhp + 3;
			maxhp = 4 * maxhp + 3;
		}
		sprintf(tempstr, "Hit Points : %i-%i", minhp, maxhp);
		AddPanelString(tempstr, 1);
	}
	if(monstkills[mt] >= 15) {
		if(gnDifficulty != DIFF_HELL) {
			res = monsterdata[mt].mMagicRes;
		} else {
			res = monsterdata[mt].mMagicRes2;
		}
		res &= 0x3F;
		if(res == 0) {
			strcpy(tempstr, "No magic resistance");
			AddPanelString(tempstr, 1);
		} else {
			if(res & 7) {
				strcpy(tempstr, "Resists : ");
				if(res & 1) {
					strcat(tempstr, "Magic ");
				}
				if(res & 2) {
					strcat(tempstr, "Fire ");
				}
				if(res & 4) {
					strcat(tempstr, "Lightning ");
				}
				tempstr[strlen(tempstr) - 1] = '\0';
				AddPanelString(tempstr, 1);
			}
			if(res & 0x38) {
				strcpy(tempstr, "Immune : ");
				if(res & 8) {
					strcat(tempstr, "Magic ");
				}
				if(res & 0x10) {
					strcat(tempstr, "Fire ");
				}
				if(res & 0x20) {
					strcat(tempstr, "Lightning ");
				}
				tempstr[strlen(tempstr) - 1] = '\0';
				AddPanelString(tempstr, 1);
			}
		}
	}

	pinfoflag = 1;
}

void PrintUniqueHistory()
{
	int res;

	res = monster[pcursmonst].mMagicRes;
	res &= 0x3F;

	if(res == 0) {
		strcpy(tempstr, "No resistances");
		AddPanelString(tempstr, 1);
		strcpy(tempstr, "No Immunities");
		AddPanelString(tempstr, 1);
	} else {
		if(res & 7) {
			strcpy(tempstr, "Some Magic Resistances");
		} else {
			strcpy(tempstr, "No resistances");
		}
		AddPanelString(tempstr, 1);
		if(res & 0x38) {
			strcpy(tempstr, "Some Magic Immunities");
		} else {
			strcpy(tempstr, "No Immunities");
		}
		AddPanelString(tempstr, 1);
	}

	pinfoflag = 1;
}

void MissToMonst(int i, int x, int y)
{
	int oldx, oldy, newx, newy, m, pnum;
	MissileStruct *Miss;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMISSILES) {
		app_fatal("MissToMonst: Invalid missile %d", i);
	}

	Miss = &missile[i];
	m = Miss->_misource;
	if((DWORD)m >= MAXMONSTERS) {
		app_fatal("MissToMonst: Invalid monster %d", m);
	}

	Monst = &monster[m];
	/// ASSERT: assert(Monst->_mmode == MM_MISSILE);
	oldx = Miss->_mix;
	oldy = Miss->_miy;
	dMonster[x][y] = m + 1;
	Monst->_mdir = Miss->_mimfnum;
	Monst->_mx = x;
	Monst->_my = y;
	M_StartStand(m, Monst->_mdir);

	if(Monst->MType->mtype >= MT_INCIN && Monst->MType->mtype <= MT_HELLBURN) {
		M_StartFadein(m, Monst->_mdir, FALSE);
	} else if(!(Monst->_mFlags & 0x10)) {
		M_StartHit(m, -1, 0);
	} else {
		M2MStartHit(m, -1, 0);
	}

	if(!(Monst->_mFlags & 0x10)) {
		pnum = dPlayer[oldx][oldy] - 1;
		if(dPlayer[oldx][oldy] > 0) {
			if(Monst->MType->mtype == MT_GLOOM) {
				return;
			}
			if(Monst->MType->mtype >= MT_INCIN && Monst->MType->mtype <= MT_HELLBURN) {
				return;
			}
			M_TryH2HHit(m, dPlayer[oldx][oldy] - 1, 500, Monst->mMinDamage2, Monst->mMaxDamage2);
			if(pnum != dPlayer[oldx][oldy] - 1) {
				return;
			}
			if(Monst->MType->mtype >= MT_NSNAKE && Monst->MType->mtype <= MT_GSNAKE) {
				return;
			}
			if(plr[pnum]._pmode != PM_GOTHIT && plr[pnum]._pmode != PM_DEATH) {
				StartPlrHit(pnum, 0, TRUE);
			}
			newx = oldx + offset_x[Monst->_mdir];
			newy = oldy + offset_y[Monst->_mdir];
			if(PosOkPlayer(pnum, newx, newy)) {
				plr[pnum].WorldX = newx;
				plr[pnum].WorldY = newy;
				FixPlayerLocation(pnum, plr[pnum]._pdir);
				FixPlrWalkTags(pnum);
				dPlayer[newx][newy] = pnum + 1;
				SetPlayerOld(pnum);
			}
		}
	} else {
		if(dMonster[oldx][oldy] > 0) {
			if(Monst->MType->mtype == MT_GLOOM) {
				return;
			}
			if(Monst->MType->mtype >= MT_INCIN && Monst->MType->mtype <= MT_HELLBURN) {
				return;
			}
			M_TryM2MHit(m, dMonster[oldx][oldy] - 1, 500, Monst->mMinDamage2, Monst->mMaxDamage2);
			if(Monst->MType->mtype >= MT_NSNAKE && Monst->MType->mtype <= MT_GSNAKE) {
				return;
			}
			newx = oldx + offset_x[Monst->_mdir];
			newy = oldy + offset_y[Monst->_mdir];
			if(PosOkMonst(dMonster[oldx][oldy] - 1, newx, newy)) {
				dMonster[newx][newy] = dMonster[oldx][oldy];
				m = dMonster[newx][newy];
				dMonster[oldx][oldy] = 0;
				m--;
				monster[m]._mx = newx;
				monster[m]._mfutx = monster[m]._mx;
				monster[m]._my = newy;
				monster[m]._mfuty = monster[m]._my;
			}
		}
	}
}

BOOL PosOkMonst(int i, int x, int y)
{
	int oi, mi;
	BOOL ret, fire;

	ret = TRUE;
	fire = FALSE;

	ret = !SolidLoc(x, y) && dPlayer[x][y] == 0 && dMonster[x][y] == 0;
	if(ret && dObject[x][y] != 0) {
		oi = dObject[x][y] > 0 ? dObject[x][y] - 1 : -(dObject[x][y] + 1);
		if(object[oi]._oSolidFlag) {
			ret = FALSE;
		}
	}
	if(ret && dMissile[x][y] != 0 && i >= 0) {
		mi = dMissile[x][y];
		if(mi > 0) {
			if(missile[mi]._mitype == MIS_FIREWALL) {
				fire = TRUE;
			} else {
				for(mi = 0; mi < nummissiles; mi++) {
					if(missile[missileactive[mi]]._mitype == MIS_FIREWALL) {
						fire = TRUE;
					}
				}
			}
		}
		if(fire && (!(monster[i].mMagicRes & 0x10) || monster[i].MType->mtype == MT_DIABLO)) {
			ret = FALSE;
		}
	}

	return ret;
}

BOOL PosOkMonst2(int i, int x, int y)
{
	int oi, mi;
	BOOL ret, fire;

	ret = TRUE;
	fire = FALSE;

	ret = !SolidLoc(x, y);
	if(ret && dObject[x][y] != 0) {
		oi = dObject[x][y] > 0 ? dObject[x][y] - 1 : -(dObject[x][y] + 1);
		if(object[oi]._oSolidFlag) {
			ret = FALSE;
		}
	}
	if(ret && dMissile[x][y] != 0 && i >= 0) {
		mi = dMissile[x][y];
		if(mi > 0) {
			if(missile[mi]._mitype == MIS_FIREWALL) {
				fire = TRUE;
			} else {
				for(mi = 0; mi < nummissiles; mi++) {
					if(missile[missileactive[mi]]._mitype == MIS_FIREWALL) {
						fire = TRUE;
					}
				}
			}
		}
		if(fire && (!(monster[i].mMagicRes & 0x10) || monster[i].MType->mtype == MT_DIABLO)) {
			ret = FALSE;
		}
	}

	return ret;
}

BOOL PosOkMonst3(int i, int x, int y)
{
	int oi, objtype, mi;
	BOOL ret, fire, isdoor;

	ret = TRUE;
	fire = FALSE;
	isdoor = FALSE;

	if(ret && dObject[x][y] != 0) {
		oi = dObject[x][y] > 0 ? dObject[x][y] - 1 : -(dObject[x][y] + 1);
		objtype = object[oi]._otype;
		isdoor = objtype == OBJ_L1LDOOR || objtype == OBJ_L1RDOOR
			  || objtype == OBJ_L2LDOOR || objtype == OBJ_L2RDOOR
			  || objtype == OBJ_L3LDOOR || objtype == OBJ_L3RDOOR;
		if(object[oi]._oSolidFlag && !isdoor) {
			ret = FALSE;
		}
	}
	if(ret) {
		ret = (!SolidLoc(x, y) || isdoor) && dPlayer[x][y] == 0 && dMonster[x][y] == 0;
	}
	if(ret && dMissile[x][y] != 0 && i >= 0) {
		mi = dMissile[x][y];
		if(mi > 0) {
			if(missile[mi]._mitype == MIS_FIREWALL) {
				fire = TRUE;
			} else {
				for(mi = 0; mi < nummissiles; mi++) {
					if(missile[missileactive[mi]]._mitype == MIS_FIREWALL) {
						fire = TRUE;
					}
				}
			}
		}
		if(fire && (!(monster[i].mMagicRes & 0x10) || monster[i].MType->mtype == MT_DIABLO)) {
			ret = FALSE;
		}
	}

	return ret;
}

BOOL IsSkel(int mt)
{
	return mt >= MT_WSKELAX && mt <= MT_XSKELAX
		|| mt >= MT_WSKELBW && mt <= MT_XSKELBW
		|| mt >= MT_WSKELSD && mt <= MT_XSKELSD;
}

BOOL IsGoat(int mt)
{
	return mt >= MT_NGOATMC && mt <= MT_GGOATMC
		|| mt >= MT_NGOATBW && mt <= MT_GGOATBW;
}

int M_SpawnSkel(int x, int y, int dir)
{
	int i, j, skeltypes, skel;

	skeltypes = 0;

	/// ASSERT: assert((DWORD)nummtypes <= MAX_LVLMTYPES);
	for(i = 0; i < nummtypes; i++) {
		if(IsSkel(Monsters[i].mtype)) {
			skeltypes++;
		}
	}

	if(skeltypes != 0) {
		j = random(136, skeltypes);
		skeltypes = 0;
		for(i = 0; i < nummtypes && skeltypes <= j; i++) {
			if(IsSkel(Monsters[i].mtype)) {
				skeltypes++;
			}
		}
		skel = AddMonster(x, y, dir, i - 1, TRUE);
		if(skel != -1) {
			M_StartSpStand(skel, dir);
		}
		return skel;
	} else {
		return -1;
	}
}

void ActivateSpawn(int i, int x, int y, int dir)
{
	dMonster[x][y] = i + 1;
	monster[i]._mx = x;
	monster[i]._my = y;
	monster[i]._mfutx = x;
	monster[i]._mfuty = y;
	monster[i]._moldx = x;
	monster[i]._moldy = y;
	M_StartSpStand(i, dir);
}

BOOL SpawnSkeleton(int ii, int x, int y)
{
	int monstok[3][3];
	int i, j, xx, yy, rs;
	BOOL savail;

	if(ii == -1) {
		return FALSE;
	}

	if(PosOkMonst(-1, x, y)) {
		ActivateSpawn(ii, x, y, GetDirection(x, y, x, y));
		return TRUE;
	}

	savail = FALSE;
	j = 0;
	for(yy = y - 1; yy <= y + 1; yy++) {
		i = 0;
		for(xx = x - 1; xx <= x + 1; xx++) {
			monstok[i][j] = PosOkMonst(-1, xx, yy);
			savail |= monstok[i][j];
			i++;
		}
		j++;
	}

	if(!savail) {
		return FALSE;
	}

	rs = random(137, 15) + 1;
	i = 0;
	j = 0;
	while(rs > 0) {
		if(monstok[i][j]) {
			rs--;
		}
		if(rs > 0) {
			i++;
			if(i == 3) {
				i = 0;
				j++;
				if(j == 3) {
					j = 0;
				}
			}
		}
	}

	i += x - 1;
	j += y - 1;
	ActivateSpawn(ii, i, j, GetDirection(i, j, x, y));
	return TRUE;
}

int PreSpawnSkeleton()
{
	int i, j, skeltypes, skel;

	skeltypes = 0;

	/// ASSERT: assert((DWORD)nummtypes <= MAX_LVLMTYPES);
	for(i = 0; i < nummtypes; i++) {
		if(IsSkel(Monsters[i].mtype)) {
			skeltypes++;
		}
	}

	if(skeltypes != 0) {
		j = random(136, skeltypes);
		skeltypes = 0;
		for(i = 0; i < nummtypes && skeltypes <= j; i++) {
			if(IsSkel(Monsters[i].mtype)) {
				skeltypes++;
			}
		}
		skel = AddMonster(0, 0, 0, i - 1, FALSE);
		if(skel != -1) {
			M_StartStand(skel, 0);
		}
		return skel;
	} else {
		return -1;
	}
}

void TalktoMonster(int i)
{
	int pnum, itm;
	MonsterStruct *Monst;

	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("TalktoMonster: Invalid monster %d", i);
	}

	Monst = &monster[i];
	pnum = Monst->_menemy;
	Monst->_mmode = MM_TALK;

	if(Monst->_mAi == AI_SNOTSPIL || Monst->_mAi == AI_LACHDAN) {
		if(QuestStatus(Q_LTBANNER) && quests[Q_LTBANNER]._qvar1 == 2 && PlrHasItem(pnum, IDI_BANNER, itm)) {
			RemoveInvItem(pnum, itm);
			quests[Q_LTBANNER]._qactive = 3;
			Monst->mtalkmsg = TEXT_BANNER12;
			Monst->_mgoal = 6;
		}
		if(QuestStatus(Q_VEIL) && Monst->mtalkmsg >= TEXT_VEIL9 && PlrHasItem(pnum, IDI_GLDNELIX, itm)) {
			RemoveInvItem(pnum, itm);
			Monst->mtalkmsg = TEXT_VEIL11;
			Monst->_mgoal = 6;
		}
	}
}

void SpawnGolum(int i, int x, int y, int mi)
{
	if((DWORD)i >= MAXMONSTERS) {
		app_fatal("SpawnGolum: Invalid monster %d", i);
	}

	dMonster[x][y] = i + 1;
	monster[i]._mx = x;
	monster[i]._my = y;
	monster[i]._mfutx = x;
	monster[i]._mfuty = y;
	monster[i]._moldx = x;
	monster[i]._moldy = y;
	monster[i]._pathcount = 0;
	monster[i]._mmaxhp = 2 * (320 * missile[mi]._mispllvl + plr[i]._pMaxMana / 3);
	monster[i]._mhitpoints = monster[i]._mmaxhp;
	monster[i].mArmorClass = 25;
	monster[i].mHit = 5 * (missile[mi]._mispllvl + 8) + 2 * plr[i]._pLevel;
	monster[i].mMinDamage = 2 * (missile[mi]._mispllvl + 4);
	monster[i].mMaxDamage = 2 * (missile[mi]._mispllvl + 8);
	monster[i]._mFlags |= 0x20;

	M_StartSpStand(i, 0);
	M_Enemy(i);

	if(i == myplr) {
		NetSendCmdGolem(monster[i]._mx, monster[i]._my, monster[i]._mdir, monster[i]._menemy, monster[i]._mhitpoints, currlevel);
	}
}

BOOL CanTalkToMonst(int m)
{
	if((DWORD)m >= MAXMONSTERS) {
		app_fatal("CanTalkToMonst: Invalid monster %d", m);
	}

	if(monster[m]._mgoal == 6) {
		return TRUE;
	}
	if(monster[m]._mgoal == 7) {
		return TRUE;
	}

	return FALSE;
}

BOOL CheckMonsterHit(int m, BOOL &ret)
{
	if((DWORD)m >= MAXMONSTERS) {
		app_fatal("CheckMonsterHit: Invalid monster %d", m);
	}

	if(monster[m]._mAi == AI_GARG && monster[m]._mFlags & 4) {
		monster[m]._mFlags &= ~4;
		monster[m]._mmode = MM_SATTACK;
		ret = TRUE;
		return TRUE;
	}
	if(monster[m].MType->mtype >= MT_COUNSLR && monster[m].MType->mtype <= MT_ADVOCATE && monster[m]._mgoal != 1) {
		ret = FALSE;
		return TRUE;
	}

	return FALSE;
}

int encode_enemy(int m)
{
	if(!(monster[m]._mFlags & 0x10)) {
		return monster[m]._menemy;
	} else {
		return monster[m]._menemy + 4;
	}
}

void decode_enemy(int m, int enemy)
{
	if(enemy < 4) {
		monster[m]._mFlags &= ~0x10;
		monster[m]._menemy = enemy;
		monster[m]._menemyx = plr[enemy]._px;
		monster[m]._menemyy = plr[enemy]._py;
	} else {
		enemy -= 4;
		monster[m]._mFlags |= 0x10;
		monster[m]._menemy = enemy;
		monster[m]._menemyx = monster[enemy]._mfutx;
		monster[m]._menemyy = monster[enemy]._mfuty;
	}
}
