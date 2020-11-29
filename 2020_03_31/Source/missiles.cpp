#include "diablo.h"

int missileactive[MAXMISSILES];
int missileavail[MAXMISSILES];
MissileStruct missile[MAXMISSILES];
int nummissiles; // idb
int ManashieldFlag;
ChainStruct chain[MAXMISSILES];
int MissilePreFlag; // weak
int numchains; // weak

int XDirAdd[8] = { 1, 0, -1, -1, -1, 0, 1, 1 };
int YDirAdd[8] = { 1, 1, 1, 0, -1, -1, -1, 0 };

void GetDamageAmt(int i, int *mind, int *maxd)
{
	int k, sl;

	/// ASSERT: assert((DWORD)myplr < MAX_PLRS);
	/// ASSERT: assert((DWORD)i < 64);
	sl = plr[myplr]._pSplLvl[i] + plr[myplr]._pISplLvlAdd;

	switch(i) {
	case SPL_FIREBOLT:
		*mind = (plr[myplr]._pMagic >> 3) + sl + 1;
		*maxd = (plr[myplr]._pMagic >> 3) + sl + 10;
		break;
	case SPL_HEAL: /// BUGFIX: healing calculation is unused
		*mind = plr[myplr]._pLevel + sl + 1;
		if(plr[myplr]._pClass == PC_WARRIOR) {
			*mind *= 2;
		}
		if(plr[myplr]._pClass == PC_ROGUE) {
			*mind += *mind >> 1;
		}
		*maxd = 10;
		for(k = 0; k < plr[myplr]._pLevel; k++) {
			*maxd += 4;
		}
		for(k = 0; k < sl; k++) {
			*maxd += 6;
		}
		if(plr[myplr]._pClass == PC_WARRIOR) {
			*maxd *= 2;
		}
		if(plr[myplr]._pClass == PC_ROGUE) {
			*maxd += *maxd >> 1;
		}
		*mind = -1;
		*maxd = -1;
		break;
	case SPL_LIGHTNING:
		*mind = 2;
		*maxd = plr[myplr]._pLevel + 2;
		break;
	case SPL_FLASH:
		*mind = plr[myplr]._pLevel;
		for(k = 0; k < sl; k++) {
			*mind += *mind >> 3;
		}
		*mind += *mind >> 1;
		*maxd = *mind * 2;
		break;
	case SPL_IDENTIFY:
	case SPL_TOWN:
	case SPL_STONE:
	case SPL_INFRA:
	case SPL_RNDTELEPORT:
	case SPL_MANASHIELD:
	case SPL_DOOMSERP:
	case SPL_BLODRIT:
	case SPL_INVISIBIL:
	case SPL_BLODBOIL:
	case SPL_TELEPORT:
	case SPL_ETHEREALIZE:
	case SPL_REPAIR:
	case SPL_RECHARGE:
	case SPL_DISARM:
	case SPL_RESURRECT:
	case SPL_TELEKINESIS:
	case SPL_BONESPIRIT:
		*mind = -1;
		*maxd = -1;
		break;
	case SPL_FIREWALL:
		*mind = (4 * plr[myplr]._pLevel + 8) >> 1;
		*maxd = (4 * plr[myplr]._pLevel + 80) >> 1;
		break;
	case SPL_FIREBALL:
		*mind = 2 * plr[myplr]._pLevel + 4;
		for(k = 0; k < sl; k++) {
			*mind += *mind >> 3;
		}
		*maxd = 2 * plr[myplr]._pLevel + 40;
		for(k = 0; k < sl; k++) {
			*maxd += *maxd >> 3;
		}
		break;
	case SPL_GUARDIAN:
		*mind = (plr[myplr]._pLevel >> 1) + 1;
		for(k = 0; k < sl; k++) {
			*mind += *mind >> 3;
		}
		*maxd = (plr[myplr]._pLevel >> 1) + 10;
		for(k = 0; k < sl; k++) {
			*maxd += *maxd >> 3;
		}
		break;
	case SPL_CHAIN:
		*mind = 4;
		*maxd = 2 * plr[myplr]._pLevel + 4;
		break;
	case SPL_WAVE:
		*mind = 6 * (plr[myplr]._pLevel + 1);
		*maxd = 6 * (plr[myplr]._pLevel + 10);
		break;
	case SPL_NOVA:
		*mind = (plr[myplr]._pLevel + 5) >> 1;
		for(k = 0; k < sl; k++) {
			*mind += *mind >> 3;
		}
		*mind *= 5;
		*maxd = (plr[myplr]._pLevel + 30) >> 1;
		for(k = 0; k < sl; k++) {
			*maxd += *maxd >> 3;
		}
		*maxd *= 5;
		break;
	case SPL_FLAME:
		*mind = 3;
		*maxd = plr[myplr]._pLevel + 4;
		*maxd += *maxd >> 1;
		break;
	case SPL_GOLEM:
		*mind = 11;
		*maxd = 17;
		break;
	case SPL_APOCA:
		*mind = 0;
		for(k = 0; k < plr[myplr]._pLevel; k++) {
			(*mind)++;
		}
		*maxd = 0;
		for(k = 0; k < plr[myplr]._pLevel; k++) {
			*maxd += 6;
		}
		break;
	case SPL_ELEMENT:
		*mind = 2 * plr[myplr]._pLevel + 4;
		for(k = 0; k < sl; k++) {
			*mind += *mind >> 3;
		}
		/// BUGFIX: add here '*mind >>= 1;'
		*maxd = 2 * plr[myplr]._pLevel + 40;
		for(k = 0; k < sl; k++) {
			*maxd += *maxd >> 3;
		}
		/// BUGFIX: add here '*maxd >>= 1;'
		break;
	case SPL_CBOLT:
		*mind = 1;
		*maxd = (plr[myplr]._pMagic >> 2) + 1;
		break;
	case SPL_HBOLT:
		*mind = plr[myplr]._pLevel + 9;
		*maxd = plr[myplr]._pLevel + 18;
		break;
	case SPL_HEALOTHER: /// BUGFIX: healing calculation is unused
		*mind = plr[myplr]._pLevel + sl + 1;
		if(plr[myplr]._pClass == PC_WARRIOR) {
			*mind *= 2;
		}
		if(plr[myplr]._pClass == PC_ROGUE) {
			*mind += *mind >> 1;
		}
		*maxd = 10;
		for(k = 0; k < plr[myplr]._pLevel; k++) {
			*maxd += 4;
		}
		for(k = 0; k < sl; k++) {
			*maxd += 6;
		}
		if(plr[myplr]._pClass == PC_WARRIOR) {
			*maxd *= 2;
		}
		if(plr[myplr]._pClass == PC_ROGUE) {
			*maxd += *maxd >> 1;
		}
		*mind = -1;
		*maxd = -1;
		break;
	case SPL_FLARE:
		*mind = (plr[myplr]._pMagic >> 1) + 3 * sl - (plr[myplr]._pMagic >> 3);
		*maxd = *mind;
		break;
	}
}

BOOL CheckBlock(int fx, int fy, int tx, int ty)
{
	int pn, d;
	BOOL coll;

	coll = FALSE;
	while(fx != tx || fy != ty) {
		d = GetDirection(fx, fy, tx, ty);
		fx += XDirAdd[d];
		fy += YDirAdd[d];
		/// ASSERT: assert((DWORD)fx < MAXDUNX);
		/// ASSERT: assert((DWORD)fy < MAXDUNY);
		pn = dPiece[fx][fy];
		/// ASSERT: assert((DWORD)pn <= MAXTILES);
		if(nSolidTable[pn]) {
			coll = TRUE;
		}
	}

	return coll;
}

int FindClosest(int sx, int sy, int rad)
{
	int cr, cidx, cent, cne, mid, tx, ty;
	int CrawlNum[19] = { 0, 3, 12, 45, 94, 159, 240, 337, 450, 579, 724, 885, 1062, 1255, 1464, 1689, 1930, 2187, 2460 };

	if(rad > 19) {
		rad = 19;
	}

	for(cr = 1; cr < rad; cr++) {
		cidx = CrawlNum[cr];
		cent = cidx + 1;
		for(cne = (BYTE)CrawlTable[cidx]; cne > 0; cne--) {
			tx = sx + CrawlTable[cent];
			ty = sy + CrawlTable[cent + 1];
			if(tx > 0 && tx < MAXDUNX && ty > 0 && ty < MAXDUNY) {
				mid = dMonster[tx][ty];
				if(mid > 0 && !CheckBlock(sx, sy, tx, ty)) {
					return mid - 1;
				}
			}
			cent += 2;
		}
	}

	return -1;
}

int GetSpellLevel(int id, int sn)
{
	int rv;

	/// ASSERT: assert((DWORD)id < MAX_PLRS);
	/// ASSERT: assert((DWORD)sn < 64);

	if(id == myplr) {
		rv = plr[id]._pSplLvl[sn] + plr[id]._pISplLvlAdd;
	} else {
		rv = 1;
	}

	if(rv < 0) {
		rv = 0;
	}

	return rv;
}

int GetDirection8(int x1, int y1, int x2, int y2)
{
	int mx, my, md;
	BYTE Dirs[16][16] = {
		{ 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{  2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{  2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{  2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
		{  2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
		{  2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
		{  2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{  2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{  2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{  2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{  2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{  2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{  2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{  2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{  2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{  2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};
	__declspec(align(1)) BYTE lrtoul[3] = { 3, 4, 5 };
	__declspec(align(1)) BYTE urtoll[3] = { 3, 2, 1 };
	__declspec(align(1)) BYTE lltour[3] = { 7, 6, 5 };
	__declspec(align(1)) BYTE ultolr[3] = { 7, 0, 1 };

	mx = abs(x2 - x1);
	if(mx > 15) {
		mx = 15;
	}
	my = abs(y2 - y1);
	if(my > 15) {
		my = 15;
	}
	md = Dirs[my][mx];

	if(x1 > x2) {
		if(y1 > y2) {
			md = lrtoul[md];
		} else {
			md = urtoll[md];
		}
	} else {
		if(y1 > y2) {
			md = lltour[md];
		} else {
			md = ultolr[md];
		}
	}

	return md;
}

int GetDirection16(int x1, int y1, int x2, int y2)
{
	int mx, my, md;
	BYTE Dirs[16][16] = {
		{ 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{  4, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{  4, 3, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
		{  4, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
		{  4, 4, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{  4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{  4, 4, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1 },
		{  4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1 },
		{  4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1 },
		{  4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1 },
		{  4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1 },
		{  4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1 },
		{  4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2 },
		{  4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2 },
		{  4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2 },
		{  4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2 }
	};
	BYTE lrtoul[5] = { 6, 7, 8, 9, 10 };
	BYTE urtoll[5] = { 6, 5, 4, 3, 2 };
	BYTE lltour[5] = { 14, 13, 12, 11, 10 };
	BYTE ultolr[5] = { 14, 15, 0, 1, 2 };

	mx = abs(x2 - x1);
	if(mx > 15) {
		mx = 15;
	}
	my = abs(y2 - y1);
	if(my > 15) {
		my = 15;
	}
	md = Dirs[my][mx];

	if(x1 > x2) {
		if(y1 > y2) {
			md = lrtoul[md];
		} else {
			md = urtoll[md];
		}
	} else {
		if(y1 > y2) {
			md = lltour[md];
		} else {
			md = ultolr[md];
		}
	}

	return md;
}

void DeleteMissile(int mi, int i)
{
	int id;

	if(missile[mi]._mitype == MIS_MANASHIELD) {
		id = missile[mi]._misource;
		if(id == myplr) {
			NetSendCmd(TRUE, CMD_REMSHIELD);
		}
		plr[id].pManaShield = 0;
	}

	/// ASSERT: assert((DWORD)nummissiles <= MAXMISSILES);
	missileavail[MAXMISSILES - nummissiles] = mi;
	nummissiles--;
	if(nummissiles > 0 && i != nummissiles) {
		/// ASSERT: assert((DWORD)i < MAXMISSILES);
		missileactive[i] = missileactive[nummissiles];
	}
}

void GetMissileVel(int i, int sx, int sy, int dx, int dy, int v)
{
	double dxp, dyp, dr;

	if(dx == sx && dy == sy) {
		missile[i]._mixvel = 0;
		missile[i]._miyvel = 0;
	} else {
		dxp = (((dx - sx) << 5) - ((dy - sy) << 5)) << 16;
		dyp = (((dx - sx) << 5) + ((dy - sy) << 5)) << 16;
		dr = sqrt(dyp * dyp + dxp * dxp);
		/// ASSERT: assert((DWORD)i < MAXMISSILES);
		missile[i]._mixvel = (dxp * (v << 16)) / dr;
		missile[i]._miyvel = (dyp * (v << 15)) / dr;
	}
}

void PutMissile(int i)
{
	int mx, my;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	mx = missile[i]._mix;
	my = missile[i]._miy;
	if(mx <= 0 || my <= 0 || mx >= MAXDUNX || my >= MAXDUNY) {
		missile[i]._miDelFlag = 1;
	}

	if(!missile[i]._miDelFlag) {
		dFlags[mx][my] |= 1;
		if(dMissile[mx][my] == 0) {
			dMissile[mx][my] = i + 1;
		} else {
			dMissile[mx][my] = -1;
		}
		if(missile[i]._miPreFlag) {
			MissilePreFlag = 1;
		}
	}
}

void GetMissilePos(int i)
{
	long mx, my, dx, dy, lx, ly;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	mx = missile[i]._mitxoff >> 16;
	my = missile[i]._mityoff >> 16;
	dx = 2 * my + mx;
	dy = 2 * my - mx;

	if(dx < 0) {
		lx = -(-dx >> 3);
		dx = -(-dx >> 6);
	} else {
		lx = dx >> 3;
		dx = dx >> 6;
	}
	if(dy < 0) {
		ly = -(-dy >> 3);
		dy = -(-dy >> 6);
	} else {
		ly = dy >> 3;
		dy = dy >> 6;
	}

	missile[i]._mix = dx + missile[i]._misx;
	missile[i]._miy = dy + missile[i]._misy;
	missile[i]._mixoff = mx - ((dx - dy) << 5);
	missile[i]._miyoff = my - ((dx + dy) << 4);
	ChangeLightOff(missile[i]._mlid, lx - (dx << 3), ly - (dy << 3));
}

void MoveMissilePos(int i)
{
	int dx, dy, mx, my;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	switch(missile[i]._mimfnum) {
	case 0:
		dx = 1;
		dy = 1;
		break;
	case 1:
		dx = 1;
		dy = 1;
		break;
	case 2:
		dx = 0;
		dy = 1;
		break;
	case 3:
		dx = 0;
		dy = 0;
		break;
	case 4:
		dx = 0;
		dy = 0;
		break;
	case 5:
		dx = 0;
		dy = 0;
		break;
	case 6:
		dx = 1;
		dy = 0;
		break;
	case 7:
		dx = 1;
		dy = 1;
		break;
	}

	mx = dx + missile[i]._mix;
	my = dy + missile[i]._miy;
	if(PosOkMonst(missile[i]._misource, mx, my)) {
		missile[i]._mix += dx;
		missile[i]._miy += dy;
		missile[i]._mixoff -= (dx - dy) << 5;
		missile[i]._miyoff -= (dx + dy) << 4;
	}
}

BOOL MonsterTrapHit(int m, int mindam, int maxdam, int dist, int t, BOOLEAN shift)
{
	int hit, hper, mor, mir;
	long dam;
	BOOL resist, ret;

	resist = FALSE;

	/// ASSERT: assert((DWORD)m < MAXMONSTERS);
	if(monster[m].mtalkmsg != 0) {
		return FALSE;
	}
	if(monster[m]._mhitpoints >> 6 <= 0) {
		return FALSE;
	}
	/// ASSERT: assert(monster[m].MType != NULL);
	if(monster[m].MType->mtype == MT_ILLWEAV && monster[m]._mgoal == 2) {
		return FALSE;
	}
	if(monster[m]._mmode == MM_CHARGE) {
		return FALSE;
	}

	mir = missiledata[t].mResist;
	mor = monster[m].mMagicRes;
	if(mor & 8 && mir == 3 || mor & 0x10 && mir == 1 || mor & 0x20 && mir == 2) {
		return FALSE;
	}
	if(mor & 1 && mir == 3 || mor & 2 && mir == 1 || mor & 4 && mir == 2) {
		resist = TRUE;
	}

	hit = random(68, 100);
	hper = 90 - (BYTE)monster[m].mArmorClass - dist;
	if(hper < 5) {
		hper = 5;
	}
	if(hper > 95) {
		hper = 95;
	}

	if(CheckMonsterHit(m, ret)) {
		return ret;
	}
#ifdef _DEBUG
	else if(hit < hper || debug_mode_dollar_sign || debug_mode_key_inverted_v || monster[m]._mmode == MM_STONE) {
#else
	else if(hit < hper || monster[m]._mmode == MM_STONE) {
#endif
		dam = random(68, maxdam - mindam + 1) + mindam;
		if(!shift) {
			dam <<= 6;
		}
		if(resist) {
			monster[m]._mhitpoints -= dam >> 2;
		} else {
			monster[m]._mhitpoints -= dam;
		}
#ifdef _DEBUG
		if(debug_mode_dollar_sign || debug_mode_key_inverted_v) {
			monster[m]._mhitpoints = 0;
		}
#endif
		if(monster[m]._mhitpoints >> 6 <= 0) {
			if(monster[m]._mmode == MM_STONE) {
				M_StartKill(m, -1);
				monster[m]._mmode = MM_STONE;
			} else {
				M_StartKill(m, -1);
			}
		} else {
			if(resist) {
				PlayEffect(m, 1);
			} else if(monster[m]._mmode == MM_STONE) {
				if(m > 3) {
					M_StartHit(m, -1, dam);
				}
				monster[m]._mmode = MM_STONE;
			} else {
				if(m > 3) {
					M_StartHit(m, -1, dam);
				}
			}
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL MonsterMHit(int pnum, int m, int mindam, int maxdam, int dist, int t, BOOLEAN shift)
{
	int hit, hper, mor, mir;
	long dam;
	BOOL resist, ret;

	resist = FALSE;

	/// ASSERT: assert((DWORD)m < MAXMONSTERS);
	if(monster[m].mtalkmsg != 0) {
		return FALSE;
	}
	if(monster[m]._mhitpoints >> 6 <= 0) {
		return FALSE;
	}
	/// ASSERT: assert(monster[m].MData != NULL);
	if(t == MIS_HBOLT && monster[m].MType->mtype != MT_DIABLO && monster[m].MData->mMonstClass != MC_UNDEAD) {
		return FALSE;
	}
	/// ASSERT: assert(monster[m].MType != NULL);
	if(monster[m].MType->mtype == MT_ILLWEAV && monster[m]._mgoal == 2) {
		return FALSE;
	}
	if(monster[m]._mmode == MM_CHARGE) {
		return FALSE;
	}

	mir = missiledata[t].mResist;
	mor = monster[m].mMagicRes;
	if(mor & 8 && mir == 3 || mor & 0x10 && mir == 1 || mor & 0x20 && mir == 2 || mor & 0x80 && mir == 4) {
		return FALSE;
	}
	if(mor & 1 && mir == 3 || mor & 2 && mir == 1 || mor & 4 && mir == 2) {
		resist = TRUE;
	}

	hit = random(69, 100);
	if(missiledata[t].mType == 0) {
		hper = plr[pnum]._pLevel + 50 - (BYTE)monster[m].mArmorClass + plr[pnum]._pIEnAc;
		hper += plr[pnum]._pIBonusToHit + plr[pnum]._pDexterity;
		hper -= dist * dist >> 1;
		if(plr[pnum]._pClass == PC_ROGUE) {
			hper += 20;
		}
		if(plr[pnum]._pClass == PC_WARRIOR) {
			hper += 10;
		}
	} else {
		hper = plr[pnum]._pMagic + 50 - (monster[m].mLevel << 1) - dist;
		if(plr[pnum]._pClass == PC_SORCERER) {
			hper += 20;
		}
	}
	if(hper < 5) {
		hper = 5;
	}
	if(hper > 95) {
		hper = 95;
	}
	if(monster[m]._mmode == MM_STONE) {
		hit = 0;
	}

	if(CheckMonsterHit(m, ret)) {
		return ret;
	}
#ifdef _DEBUG
	else if(hit < hper || debug_mode_key_inverted_v || debug_mode_dollar_sign) {
#else
	else if(hit < hper) {
#endif
		if(t == MIS_BONESPIRIT) {
			dam = monster[m]._mhitpoints / 3 >> 6;
		} else {
			dam = random(70, maxdam - mindam + 1) + mindam;
		}
		if(missiledata[t].mType == 0) {
			dam += plr[pnum]._pIBonusDamMod + dam * plr[pnum]._pIBonusDam / 100;
			if(plr[pnum]._pClass == PC_ROGUE) {
				dam += plr[pnum]._pDamageMod;
			} else {
				dam += plr[pnum]._pDamageMod >> 1;
			}
		}
		if(!shift) {
			dam <<= 6;
		}
		if(resist) {
			dam >>= 2;
		}
		if(pnum == myplr) {
			monster[m]._mhitpoints -= dam;
		}
		if(plr[pnum]._pIFlags & ISPL_FIRE_ARROWS) {
			monster[m]._mFlags |= 8;
		}
		if(monster[m]._mhitpoints >> 6 <= 0) {
			if(monster[m]._mmode == MM_STONE) {
				M_StartKill(m, pnum);
				monster[m]._mmode = MM_STONE;
			} else {
				M_StartKill(m, pnum);
			}
		} else {
			if(resist) {
				PlayEffect(m, 1);
			} else if(monster[m]._mmode == MM_STONE) {
				if(m > 3) {
					M_StartHit(m, pnum, dam);
				}
				monster[m]._mmode = MM_STONE;
			} else {
				if(missiledata[t].mType == 0 && plr[pnum]._pIFlags & ISPL_KNOCKBACK) {
					M_GetKnockback(m);
				}
				if(m > 3) {
					M_StartHit(m, pnum, dam);
				}
			}
		}
		if(monster[m]._msquelch == 0) {
			monster[m]._msquelch = 255;
			monster[m]._lastx = plr[pnum].WorldX;
			monster[m]._lasty = plr[pnum].WorldY;
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL PlayerMHit(int pnum, int m, int dist, int mind, int maxd, int mtype, BOOLEAN shift, int earflag)
{
	int hit, hper, tac, blk, blkper, blkdir, resper;
	long dam;

	/// ASSERT: assert((DWORD)pnum < MAX_PLRS);
	if(plr[pnum]._pHitPoints >> 6 <= 0) {
		return FALSE;
	}
	if(plr[pnum]._pInvincible) {
		return FALSE;
	}
	if(plr[pnum]._pSpellFlags & 1 && missiledata[mtype].mType == 0) {
		return FALSE;
	}

	hit = random(72, 100);
#ifdef _DEBUG
	if(debug_mode_dollar_sign || debug_mode_key_inverted_v) {
		hit = 1000;
	}
#endif

	if(missiledata[mtype].mType == 0) {
		tac = plr[pnum]._pIAC + plr[pnum]._pIBonusAC + plr[pnum]._pDexterity / 5;
		if(m != -1) {
			hper = monster[m].mHit + 30 + ((monster[m].mLevel - plr[pnum]._pLevel) << 1) - tac;
			hper -= dist << 1;
		} else {
			hper = 100 - (tac >> 1);
			hper -= dist << 1;
		}
	} else {
		if(m != -1) {
			hper = 40 + (monster[m].mLevel << 1) - (plr[pnum]._pLevel << 1);
			hper -= dist << 1;
		} else {
			hper = 40;
		}
	}

	if(hper < 10) {
		hper = 10;
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
		blk = random(73, 100);
	} else {
		blk = 100;
	}
	if(shift == TRUE) {
		blk = 100;
	}
	if(mtype == MIS_ACIDPUD) {
		blk = 100;
	}

	if(m != -1) {
		blkper = plr[pnum]._pBaseToBlk + plr[pnum]._pDexterity - ((monster[m].mLevel - plr[pnum]._pLevel) << 1);
	} else {
		blkper = plr[pnum]._pBaseToBlk + plr[pnum]._pDexterity;
	}
	if(blkper < 0) {
		blkper = 0;
	}
	if(blkper > 100) {
		blkper = 100;
	}

	switch(missiledata[mtype].mResist) {
	case 1:
		resper = plr[pnum]._pFireResist;
		break;
	case 2:
		resper = plr[pnum]._pLghtResist;
		break;
	case 3:
	case 4:
		resper = plr[pnum]._pMagResist;
		break;
	default:
		resper = 0;
		break;
	}

	if(hit < hper) {
		if(mtype == MIS_BONESPIRIT) {
			dam = plr[pnum]._pHitPoints / 3;
		} else {
			if(!shift) {
				dam = random(75, (maxd - mind + 1) << 6) + (mind << 6);
				if(m == -1 && plr[pnum]._pIFlags & ISPL_ABSHALFTRAP) {
					dam >>= 1;
				}
				dam += plr[pnum]._pIGetHit << 6;
				if(dam < 64) {
					dam = 64;
				}
			} else {
				dam = random(75, maxd - mind + 1) + mind;
				if(m == -1 && plr[pnum]._pIFlags & ISPL_ABSHALFTRAP) {
					dam >>= 1;
				}
				dam += plr[pnum]._pIGetHit;
				if(dam < 64) {
					dam = 64;
				}
			}
		}
		if(resper > 0) {
			dam -= resper * dam / 100;
			if(pnum == myplr) {
				plr[pnum]._pHitPoints -= dam;
				plr[pnum]._pHPBase -= dam;
			}
			if(plr[pnum]._pHitPoints > plr[pnum]._pMaxHP) {
				plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
				plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;
			}
			if(plr[pnum]._pHitPoints >> 6 <= 0) {
				SyncPlrKill(pnum, earflag);
			} else {
				if(plr[pnum]._pClass == PC_WARRIOR) {
					PlaySfxLoc(PS_WARR69, plr[pnum].WorldX, plr[pnum].WorldY);
				} else if(plr[pnum]._pClass == PC_ROGUE) {
					PlaySfxLoc(PS_ROGUE69, plr[pnum].WorldX, plr[pnum].WorldY);
				} else if(plr[pnum]._pClass == PC_SORCERER) {
					PlaySfxLoc(PS_MAGE69, plr[pnum].WorldX, plr[pnum].WorldY);
				}
				drawhpflag = 1;
			}
			return TRUE;
		} else if(blk < blkper) {
			if(m != -1) {
				blkdir = GetDirection(plr[pnum].WorldX, plr[pnum].WorldY, monster[m]._mx, monster[m]._my);
			} else {
				blkdir = plr[pnum]._pdir;
			}
			StartPlrBlock(pnum, blkdir);
			return TRUE;
		} else {
			if(pnum == myplr) {
				plr[pnum]._pHitPoints -= dam;
				plr[pnum]._pHPBase -= dam;
			}
			if(plr[pnum]._pHitPoints > plr[pnum]._pMaxHP) {
				plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
				plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;
			}
			if(plr[pnum]._pHitPoints >> 6 <= 0) {
				SyncPlrKill(pnum, earflag);
			} else {
				StartPlrHit(pnum, dam, FALSE);
			}
			return TRUE;
		}
	}

	return FALSE;
}

BOOL Plr2PlrMHit(int pnum, int p, int mindam, int maxdam, int dist, int mtype, BOOLEAN shift)
{
	int hit, hper, tac, blk, blkper, blkdir, resper;
	long dam;

	/// ASSERT: assert((DWORD)p < MAX_PLRS);
	if(plr[p]._pInvincible) {
		return FALSE;
	}
	if(mtype == MIS_HBOLT) {
		return FALSE;
	}
	if(plr[p]._pSpellFlags & 1 && missiledata[mtype].mType == 0) {
		return FALSE;
	}

	switch(missiledata[mtype].mResist) {
	case 1:
		resper = plr[p]._pFireResist;
		break;
	case 2:
		resper = plr[p]._pLghtResist;
		break;
	case 3:
	case 4:
		resper = plr[p]._pMagResist;
		break;
	default:
		resper = 0;
		break;
	}

	hit = random(69, 100);

	if(missiledata[mtype].mType == 0) {
		tac = plr[p]._pIAC + plr[p]._pIBonusAC + plr[p]._pDexterity / 5;
		hper = plr[pnum]._pLevel + 50 - tac;
		hper += plr[pnum]._pIBonusToHit + plr[pnum]._pDexterity;
		hper -= dist * dist >> 1;
		if(plr[pnum]._pClass == PC_ROGUE) {
			hper += 20;
		}
		if(plr[pnum]._pClass == PC_WARRIOR) {
			hper += 10;
		}
	} else {
		hper = plr[pnum]._pMagic + 50 - (plr[p]._pLevel << 1) - dist;
		if(plr[pnum]._pClass == PC_SORCERER) {
			hper += 20;
		}
	}

	if(hper < 5) {
		hper = 5;
	}
	if(hper > 95) {
		hper = 95;
	}

	if(hit < hper) {
		if((plr[p]._pmode == PM_STAND || plr[p]._pmode == PM_ATTACK) && plr[p]._pBlockFlag) {
			blk = random(73, 100);
		} else {
			blk = 100;
		}
		if(shift == TRUE) {
			blk = 100;
		}
		blkper = plr[p]._pDexterity + plr[p]._pBaseToBlk - ((plr[pnum]._pLevel - plr[p]._pLevel) << 1);
		if(blkper < 0) {
			blkper = 0;
		}
		if(blkper > 100) {
			blkper = 100;
		}
		if(mtype == MIS_BONESPIRIT) {
			dam = plr[p]._pHitPoints / 3;
		} else {
			dam = random(70, maxdam - mindam + 1) + mindam;
			if(missiledata[mtype].mType == 0) {
				dam += plr[pnum]._pDamageMod + plr[pnum]._pIBonusDamMod + dam * plr[pnum]._pIBonusDam / 100;
			}
			if(!shift) {
				dam <<= 6;
			}
		}
		if(missiledata[mtype].mType != 0) {
			dam >>= 1;
		}
		if(resper > 0) {
			dam -= resper * dam / 100;
			if(pnum == myplr) {
				NetSendCmdDamage(TRUE, p, dam);
			}
			if(plr[pnum]._pClass == PC_WARRIOR) {
				PlaySfxLoc(PS_WARR69, plr[pnum].WorldX, plr[pnum].WorldY);
			} else if(plr[pnum]._pClass == PC_ROGUE) {
				PlaySfxLoc(PS_ROGUE69, plr[pnum].WorldX, plr[pnum].WorldY);
			} else if(plr[pnum]._pClass == PC_SORCERER) {
				PlaySfxLoc(PS_MAGE69, plr[pnum].WorldX, plr[pnum].WorldY);
			}
			return TRUE;
		} else if(blk < blkper) {
			blkdir = GetDirection(plr[p].WorldX, plr[p].WorldY, plr[pnum].WorldX, plr[pnum].WorldY);
			StartPlrBlock(p, blkdir);
			return TRUE;
		} else {
			if(pnum == myplr) {
				NetSendCmdDamage(TRUE, p, dam);
			}
			StartPlrHit(p, dam, FALSE);
			return TRUE;
		}
	}

	return FALSE;
}

void CheckMissileCol(int i, int mindam, int maxdam, BOOLEAN shift, int mx, int my, BOOLEAN nodel)
{
	int pn, oi;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	/// ASSERT: assert((DWORD)mx < MAXDUNX);
	/// ASSERT: assert((DWORD)my < MAXDUNY);

	if(missile[i]._miAnimType == 4 || missile[i]._misource == -1) {
		if(dMonster[mx][my] > 0) {
			if(missile[i]._miAnimType == 4) {
				if(MonsterMHit(missile[i]._misource, dMonster[mx][my] - 1, mindam, maxdam, missile[i]._midist, missile[i]._mitype, shift)) {
					if(!nodel) {
						missile[i]._mirange = 0;
					}
					missile[i]._miHitFlag = 1;
				}
			} else {
				if(MonsterTrapHit(dMonster[mx][my] - 1, mindam, maxdam, missile[i]._midist, missile[i]._mitype, shift)) {
					if(!nodel) {
						missile[i]._mirange = 0;
					}
					missile[i]._miHitFlag = 1;
				}
			}
		}
		if(dPlayer[mx][my] > 0) {
			if(PlayerMHit(dPlayer[mx][my] - 1, -1, missile[i]._midist, mindam, maxdam, missile[i]._mitype, shift, missile[i]._miAnimType == 4)) {
				if(!nodel) {
					missile[i]._mirange = 0;
				}
				missile[i]._miHitFlag = 1;
			}
		}
	} else {
		if(missile[i]._micaster == 0) {
			if(dMonster[mx][my] > 0) {
				if(MonsterMHit(missile[i]._misource, dMonster[mx][my] - 1, mindam, maxdam, missile[i]._midist, missile[i]._mitype, shift)) {
					if(!nodel) {
						missile[i]._mirange = 0;
					}
					missile[i]._miHitFlag = 1;
				}
			} else if(dMonster[mx][my] < 0 && monster[-(dMonster[mx][my] + 1)]._mmode == MM_STONE) {
				if(MonsterMHit(missile[i]._misource, -(dMonster[mx][my] + 1), mindam, maxdam, missile[i]._midist, missile[i]._mitype, shift)) {
					if(!nodel) {
						missile[i]._mirange = 0;
					}
					missile[i]._miHitFlag = 1;
				}
			}
			if(dPlayer[mx][my] > 0 && dPlayer[mx][my] - 1 != missile[i]._misource) {
				if(Plr2PlrMHit(missile[i]._misource, dPlayer[mx][my] - 1, mindam, maxdam, missile[i]._midist, missile[i]._mitype, shift)) {
					if(!nodel) {
						missile[i]._mirange = 0;
					}
					missile[i]._miHitFlag = 1;
				}
			}
		} else {
			if(monster[missile[i]._misource]._mFlags & 0x10 && dMonster[mx][my] > 0 && monster[dMonster[mx][my] - 1]._mFlags & 0x20) {
				if(MonsterTrapHit(dMonster[mx][my] - 1, mindam, maxdam, missile[i]._midist, missile[i]._mitype, shift)) {
					if(!nodel) {
						missile[i]._mirange = 0;
					}
					missile[i]._miHitFlag = 1;
				}
			}
			if(dPlayer[mx][my] > 0) {
				if(PlayerMHit(dPlayer[mx][my] - 1, missile[i]._misource, missile[i]._midist, mindam, maxdam, missile[i]._mitype, shift, 0)) {
					if(!nodel) {
						missile[i]._mirange = 0;
					}
					missile[i]._miHitFlag = 1;
				}
			}
		}
	}

	if(dObject[mx][my] != 0) {
		oi = dObject[mx][my] > 0 ? dObject[mx][my] - 1 : -(dObject[mx][my] + 1);
		if(!object[oi]._oMissFlag) {
			if(object[oi]._oBreak == 1) {
				BreakObject(-1, oi);
			}
			if(!nodel) {
				missile[i]._mirange = 0;
			}
			missile[i]._miHitFlag = 0;
		}
	}

	pn = dPiece[mx][my];
	/// ASSERT: assert((DWORD)pn <= MAXTILES);
	if(nMissileTable[pn]) {
		if(!nodel) {
			missile[i]._mirange = 0;
		}
		missile[i]._miHitFlag = 0;
	}

	if(missile[i]._mirange == 0 && missiledata[missile[i]._mitype].miSFX != -1) {
		PlaySfxLoc(missiledata[missile[i]._mitype].miSFX, missile[i]._mix, missile[i]._miy);
	}
}

void SetMissAnim(int mi, int animtype)
{
	int dir;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	dir = missile[mi]._mimfnum;
	missile[mi]._miAnimType = animtype;
	missile[mi]._miAnimFlags = misfiledata[animtype].mFlags;
	missile[mi]._miAnimData = misfiledata[animtype].mAnimData[dir];
	missile[mi]._miAnimDelay = misfiledata[animtype].mAnimDelay[dir];
	missile[mi]._miAnimLen = misfiledata[animtype].mAnimLen[dir];
	missile[mi]._miAnimWidth = misfiledata[animtype].mAnimWidth[dir];
	missile[mi]._miAnimWidth2 = misfiledata[animtype].mAnimWidth2[dir];
	missile[mi]._miAnimCnt = 0;
	missile[mi]._miAnimFrame = 1;
}

void SetMissDir(int mi, int dir)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mimfnum = dir;
	SetMissAnim(mi, missile[mi]._miAnimType);
}

void LoadMissileGFX(BYTE mi)
{
	int i;
	BYTE *p;
	MisFileData *m;
	char filestr[256];

	m = &misfiledata[mi];

	if(m->mFlags & 4) {
		sprintf(filestr, "Missiles\\%s.CL2", m->mName);
		p = DiabLoad(filestr, NULL, 'MISS');
		for(i = 0; i < m->mAnimFAmt; i++) {
			m->mAnimData[i] = &p[((DWORD *)p)[i]];
		}
	} else if(m->mAnimFAmt == 1) {
		sprintf(filestr, "Missiles\\%s.CL2", m->mName);
		if(m->mAnimData[0] == NULL) {
			m->mAnimData[0] = DiabLoad(filestr, NULL, 'MISS');
		}
	} else {
		for(i = 0; i < m->mAnimFAmt; i++) {
			sprintf(filestr, "Missiles\\%s%i.CL2", m->mName, i + 1);
			if(m->mAnimData[i] == NULL) {
				m->mAnimData[i] = DiabLoad(filestr, NULL, 'MISS');
			}
		}
	}
}

void InitMissileGFX()
{
	int i;

	for(i = 0; misfiledata[i].mAnimFAmt != 0; i++) {
		if(!(misfiledata[i].mFlags & 1)) {
			LoadMissileGFX(i);
		}
	}
}

void FreeMissileGFX(int mi)
{
	int i;
	DWORD *p;

	if(misfiledata[mi].mFlags & 4) {
		if(misfiledata[mi].mAnimData[0] != NULL) {
			p = (DWORD *)misfiledata[mi].mAnimData[0];
			p -= misfiledata[mi].mAnimFAmt;
			MemFreeDbg(p);
			misfiledata[mi].mAnimData[0] = NULL;
		}
	} else {
		for(i = 0; i < misfiledata[mi].mAnimFAmt; i++) {
			if(misfiledata[mi].mAnimData[i] != NULL) {
				MemFreeDbg(misfiledata[mi].mAnimData[i]);
			}
		}
	}
}

void FreeMissiles()
{
	int i;

	for(i = 0; misfiledata[i].mAnimFAmt != 0; i++) {
		if(!(misfiledata[i].mFlags & 1)) {
			FreeMissileGFX(i);
		}
	}
}

void FreeMissiles2()
{
	int i;

	for(i = 0; misfiledata[i].mAnimFAmt != 0; i++) {
		if(misfiledata[i].mFlags & 1) {
			FreeMissileGFX(i);
		}
	}
}

void InitMissiles()
{
	int i, j, mx;

	plr[myplr]._pSpellFlags &= ~1;

	if(plr[myplr]._pInfraFlag == 1) {
		/// ASSERT: assert((DWORD)nummissiles <= MAXMISSILES);
		for(i = 0; i < nummissiles; i++) {
			mx = missileactive[i];
			/// ASSERT: assert((DWORD)mx < MAXMISSILES);
			if(missile[mx]._mitype == MIS_INFRA && missile[mx]._misource == myplr) {
				CalcPlrItemVals(missile[mx]._misource, TRUE);
			}
		}
	}

	nummissiles = 0;
	for(i = 0; i < MAXMISSILES; i++) {
		missileavail[i] = i;
		missileactive[i] = 0;
	}

	numchains = 0;
	for(i = 0; i < MAXMISSILES; i++) {
		chain[i].idx = -1;
		chain[i]._mitype = 0;
		chain[i]._mirange = 0;
	}

	for(j = 0; j < MAXDUNY; j++) {
		for(i = 0; i < MAXDUNX; i++) {
			dFlags[i][j] &= ~1;
		}
	}
}

void AddLArrow(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	if(sx == dx && sy == dy) {
		dx += XDirAdd[midir];
		dy += YDirAdd[midir];
	}

	if(mienemy == 0) {
		if(plr[id]._pClass == PC_ROGUE) {
			GetMissileVel(mi, sx, sy, dx, dy, (plr[id]._pLevel >> 2) + 31);
		} else if(plr[id]._pClass == PC_WARRIOR) {
			GetMissileVel(mi, sx, sy, dx, dy, (plr[id]._pLevel >> 3) + 31);
		} else {
			GetMissileVel(mi, sx, sy, dx, dy, 32);
		}
	} else {
		GetMissileVel(mi, sx, sy, dx, dy, 32);
	}

	SetMissDir(mi, GetDirection16(sx, sy, dx, dy));
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mirange = 256;
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	missile[mi]._mlid = AddLight(sx, sy, 5);
}

void AddArrow(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int av;

	if(sx == dx && sy == dy) {
		dx += XDirAdd[midir];
		dy += YDirAdd[midir];
	}

	if(mienemy == 0) {
		av = 32;
		if(plr[id]._pIFlags & ISPL_RNDARROWVEL) {
			av = random(64, 32) + 16;
		}
		if(plr[id]._pClass == PC_ROGUE) {
			av += (plr[id]._pLevel - 1) >> 2;
		}
		if(plr[id]._pClass == PC_WARRIOR) {
			av += (plr[id]._pLevel - 1) >> 3;
		}
		GetMissileVel(mi, sx, sy, dx, dy, av);
	} else {
		GetMissileVel(mi, sx, sy, dx, dy, 32);
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miAnimFrame = GetDirection16(sx, sy, dx, dy) + 1;
	missile[mi]._mirange = 256;
}

void GetVileMissPos(int mi, int dx, int dy)
{
	int xx, yy, i, j, l;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);

	for(l = 1; l < 50; l++) {
		for(j = -l; j <= l; j++) {
			yy = j + dy;
			for(i = -l; i <= l; i++) {
				xx = i + dx;
				if(PosOkPlayer(myplr, xx, yy)) {
					missile[mi]._mix = xx;
					missile[mi]._miy = yy;
					return;
				}
			}
		}
	}

	missile[mi]._mix = dx;
	missile[mi]._miy = dy;
}

void AddRndTeleport(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int pn, r1, r2, nTries, oi;
	BOOL dirok;

	dirok = FALSE;
	nTries = 0;
	while(1) {
		nTries++;
		if(nTries > 500) {
			break;
		}
		r1 = random(58, 3) + 4;
		r2 = random(58, 3) + 4;
		if(random(58, 2) == 1) {
			r1 = -r1;
		}
		if(random(58, 2) == 1) {
			r2 = -r2;
		}
		pn = dPiece[r1 + sx][r2 + sy];
		if(!nSolidTable[pn] && dObject[r1 + sx][r2 + sy] == 0 && dMonster[r1 + sx][r2 + sy] == 0) {
			dirok = TRUE;
			break;
		}
	}
	if(!dirok) {
		r1 = 0;
		r2 = 0;
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mirange = 2;
	missile[mi]._miVar1 = 0;

	if(setlevel && setlvlnum == SL_VILEBETRAYER) {
		oi = dObject[dx][dy] - 1;
		if(object[oi]._otype == OBJ_MCIRCLE1 || object[oi]._otype == OBJ_MCIRCLE2) {
			missile[mi]._mix = dx;
			missile[mi]._miy = dy;
			if(!PosOkPlayer(myplr, dx, dy)) {
				GetVileMissPos(mi, dx, dy);
			}
		}
	} else {
		missile[mi]._mix = sx + r1;
		missile[mi]._miy = sy + r2;
		if(mienemy == 0) {
			UseMana(id, SPL_RNDTELEPORT);
		}
	}
}

void AddFirebolt(int mi, int sx, int sy, int dx, int dy, int midir, char micaster, int id, int dam)
{
	int sp, i, mx;

	if(sx == dx && sy == dy) {
		dx += XDirAdd[midir];
		dy += YDirAdd[midir];
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	if(micaster == 0) {
		/// ASSERT: assert((DWORD)nummissiles <= MAXMISSILES);
		for(i = 0; i < nummissiles; i++) {
			mx = missileactive[i];
			/// ASSERT: assert((DWORD)mx < MAXMISSILES);
			if(missile[mx]._mitype == MIS_GUARDIAN && missile[mx]._misource == id && missile[mx]._miVar3 == mi) {
				break;
			}
		}
		if(i == nummissiles) {
			UseMana(id, SPL_FIREBOLT);
		}
		if(id != -1) {
			sp = 2 * missile[mi]._mispllvl + 16;
			if(sp >= 63) {
				sp = 63;
			}
		} else {
			sp = 16;
		}
	} else {
		sp = 26;
	}

	GetMissileVel(mi, sx, sy, dx, dy, sp);
	SetMissDir(mi, GetDirection16(sx, sy, dx, dy));
	missile[mi]._mirange = 256;
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	missile[mi]._mlid = AddLight(sx, sy, 8);
}

void AddMagmaball(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	GetMissileVel(mi, sx, sy, dx, dy, 16);
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mitxoff += 3 * missile[mi]._mixvel;
	missile[mi]._mityoff += 3 * missile[mi]._miyvel;
	GetMissilePos(mi);
	missile[mi]._mirange = 256;
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	missile[mi]._mlid = AddLight(sx, sy, 8);
}

void miss_null_33(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	GetMissileVel(mi, sx, sy, dx, dy, 16);
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mirange = 256;
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	PutMissile(mi);
}

void AddTeleport(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i, pn, k, l, j, tx, ty;
	int CrawlNum[6] = { 0, 3, 12, 45, 94, 159 };

	/// ASSERT: assert((DWORD)dx < MAXDUNX);
	/// ASSERT: assert((DWORD)dy < MAXDUNY);
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 1;

	for(j = 0; j < 6; j++) {
		k = CrawlNum[j];
		l = k + 1;
		for(i = (BYTE)CrawlTable[k]; i > 0; i--) {
			tx = dx + CrawlTable[l];
			ty = dy + CrawlTable[l + 1];
			if(tx > 0 && tx < MAXDUNX && ty > 0 && ty < MAXDUNY) {
				pn = dPiece[tx][ty];
				if(!(dMonster[tx][ty] | dObject[tx][ty] | dPlayer[tx][ty] | nSolidTable[pn])) {
					missile[mi]._mix = tx;
					missile[mi]._miy = ty;
					missile[mi]._misx = tx;
					missile[mi]._misy = ty;
					missile[mi]._miDelFlag = 0;
					j = 6;
					break;
				}
			}
			l += 2;
		}
	}

	if(!missile[mi]._miDelFlag) {
		UseMana(id, SPL_TELEPORT);
		missile[mi]._mirange = 2;
	}
}

void AddLightball(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	GetMissileVel(mi, sx, sy, dx, dy, 16);
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._midam = dam;
	missile[mi]._miAnimFrame = random(63, 8) + 1;
	missile[mi]._mirange = 255;

	if(id < 0) {
		missile[mi]._miVar1 = sx;
		missile[mi]._miVar2 = sy;
	} else {
		missile[mi]._miVar1 = plr[id].WorldX;
		missile[mi]._miVar2 = plr[id].WorldY;
	}
}

void AddFirewall(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._midam = 16 * (random(53, 10) + random(53, 10) + plr[id]._pLevel + 2);
	missile[mi]._midam >>= 1;
	GetMissileVel(mi, sx, sy, dx, dy, 16);
	missile[mi]._mirange = 10;
	for(i = missile[mi]._mispllvl; i > 0; i--) {
		missile[mi]._mirange += 10;
	}
	missile[mi]._mirange += missile[mi]._mirange * plr[id]._pISplDur >> 7;
	missile[mi]._mirange <<= 4;
	missile[mi]._miVar1 = missile[mi]._mirange - missile[mi]._miAnimLen;
	missile[mi]._miVar2 = 0;
}

void AddFireball(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i;

	if(sx == dx && sy == dy) {
		dx += XDirAdd[midir];
		dy += YDirAdd[midir];
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	if(mienemy == 0) {
		missile[mi]._midam = 2 * (random(60, 10) + random(60, 10) + plr[id]._pLevel + 2);
		for(i = missile[mi]._mispllvl; i > 0; i--) {
			missile[mi]._midam += missile[mi]._midam >> 3;
		}
		i = 2 * missile[mi]._mispllvl + 16;
		if(i > 50) {
			i = 50;
		}
		UseMana(id, SPL_FIREBALL);
	} else {
		i = 16;
	}

	GetMissileVel(mi, sx, sy, dx, dy, i);
	SetMissDir(mi, GetDirection16(sx, sy, dx, dy));
	missile[mi]._mirange = 256;
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	missile[mi]._miVar3 = 0;
	missile[mi]._miVar4 = sx;
	missile[mi]._miVar5 = sy;
	missile[mi]._mlid = AddLight(sx, sy, 8);
}

void AddLightctrl(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	if(dam == 0 && mienemy == 0) {
		UseMana(id, SPL_LIGHTNING);
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	GetMissileVel(mi, sx, sy, dx, dy, 32);
	missile[mi]._miAnimFrame = random(52, 8) + 1;
	missile[mi]._mirange = 256;
}

void AddLightning(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._misx = dx;
	missile[mi]._misy = dy;

	if(midir >= 0) {
		missile[mi]._mixoff = missile[midir]._mixoff;
		missile[mi]._miyoff = missile[midir]._miyoff;
		missile[mi]._mitxoff = missile[midir]._mitxoff;
		missile[mi]._mityoff = missile[midir]._mityoff;
	}

	missile[mi]._miAnimFrame = random(52, 8) + 1;

	if(midir >= 0 && mienemy != 1 && id != -1) {
		missile[mi]._mirange = (missile[mi]._mispllvl >> 1) + 6;
	} else if(midir >= 0 && id != -1) {
		missile[mi]._mirange = 10;
	} else {
		missile[mi]._mirange = 8;
	}

	missile[mi]._mlid = AddLight(missile[mi]._mix, missile[mi]._miy, 4);
}

void AddMisexp(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	if(mienemy != 0 && id > 0) {
		/// ASSERT: assert((DWORD)id < MAXMONSTERS);
		/// ASSERT: assert(monster[id].MType != NULL);
		if(monster[id].MType->mtype == MT_SUCCUBUS) {
			SetMissAnim(mi, MFILE_FLAREEXP);
		}
		if(monster[id].MType->mtype == MT_SNOWWICH) {
			SetMissAnim(mi, MFILE_SCBSEXPB);
		}
		if(monster[id].MType->mtype == MT_HLSPWN) {
			SetMissAnim(mi, MFILE_SCBSEXPD);
		}
		if(monster[id].MType->mtype == MT_SOLBRNR) {
			SetMissAnim(mi, MFILE_SCBSEXPC);
		}
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mix = missile[dx]._mix;
	missile[mi]._miy = missile[dx]._miy;
	missile[mi]._misx = missile[dx]._misx;
	missile[mi]._misy = missile[dx]._misy;
	missile[mi]._mixoff = missile[dx]._mixoff;
	missile[mi]._miyoff = missile[dx]._miyoff;
	missile[mi]._mitxoff = missile[dx]._mitxoff;
	missile[mi]._mityoff = missile[dx]._mityoff;
	missile[mi]._mixvel = 0;
	missile[mi]._miyvel = 0;
	missile[mi]._mirange = missile[mi]._miAnimLen;
	missile[mi]._miVar1 = 0;
}

void AddWeapexp(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mix = sx;
	missile[mi]._miy = sy;
	missile[mi]._misx = sx;
	missile[mi]._misy = sy;
	missile[mi]._mixvel = 0;
	missile[mi]._miyvel = 0;
	missile[mi]._miVar1 = 0;
	missile[mi]._miVar2 = dx;
	missile[mi]._mimfnum = 0;

	if(dx == 1) {
		SetMissAnim(mi, MFILE_MAGBLOS);
	} else {
		SetMissAnim(mi, MFILE_MINILTNG);
	}

	missile[mi]._mirange = missile[mi]._miAnimLen - 1;
}

BOOL CheckIfTrig(int x, int y)
{
	int i;

	/// ASSERT: assert((DWORD)numtrigs <= MAXTRIGGERS);
	for(i = 0; i < numtrigs; i++) {
		if(x == trigs[i]._tx && y == trigs[i]._ty) {
			return TRUE;
		}
		if(abs(trigs[i]._tx - x) < 2 && abs(trigs[i]._ty - y) < 2) {
			return TRUE;
		}
	}

	return FALSE;
}

void AddTown(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i, pn, k, l, j, tx, ty, mx;
	int CrawlNum[6] = { 0, 3, 12, 45, 94, 159 };

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	if(currlevel != 0) {
		missile[mi]._miDelFlag = 1;
		for(j = 0; j < 6; j++) {
			k = CrawlNum[j];
			l = k + 1;
			for(i = (BYTE)CrawlTable[k]; i > 0; i--) {
				tx = dx + CrawlTable[l];
				ty = dy + CrawlTable[l + 1];
				if(tx > 0 && tx < MAXDUNX && ty > 0 && ty < MAXDUNY) {
					pn = dPiece[tx][ty];
					/// ASSERT: assert((DWORD)pn <= MAXTILES);
					if(!(dObject[tx][ty] | dPlayer[tx][ty] | dMissile[tx][ty] | nSolidTable[pn] | nMissileTable[pn])) {
						if(!CheckIfTrig(tx, ty)) {
							missile[mi]._mix = tx;
							missile[mi]._miy = ty;
							missile[mi]._misx = tx;
							missile[mi]._misy = ty;
							missile[mi]._miDelFlag = 0;
							j = 6;
							break;
						}
					}
				}
				l += 2;
			}
		}
	} else {
		tx = dx;
		ty = dy;
		missile[mi]._mix = tx;
		missile[mi]._miy = ty;
		missile[mi]._misx = tx;
		missile[mi]._misy = ty;
		missile[mi]._miDelFlag = 0;
	}

	missile[mi]._mirange = 100;
	missile[mi]._miVar1 = missile[mi]._mirange - missile[mi]._miAnimLen;
	missile[mi]._miVar2 = 0;

	for(i = 0; i < nummissiles; i++) {
		mx = missileactive[i];
		if(missile[mx]._mitype == MIS_TOWN && mx != mi && missile[mx]._misource == id) {
			missile[mx]._mirange = 0;
		}
	}

	PutMissile(mi);

	if(id == myplr && !missile[mi]._miDelFlag && currlevel != 0) {
		if(!setlevel) {
			NetSendCmdLocParam3(TRUE, CMD_ACTIVATEPORTAL, tx, ty, currlevel, leveltype, 0);
		} else {
			NetSendCmdLocParam3(TRUE, CMD_ACTIVATEPORTAL, tx, ty, setlvlnum, leveltype, 1);
		}
	}
}

void AddFlash(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	if(mienemy == 0) {
		if(id != -1) {
			missile[mi]._midam = 0;
			for(i = 0; i <= plr[id]._pLevel; i++) {
				missile[mi]._midam += random(55, 20) + 1;
			}
			for(i = missile[mi]._mispllvl; i > 0; i--) {
				missile[mi]._midam += missile[mi]._midam >> 3;
			}
			missile[mi]._midam += missile[mi]._midam >> 1;
			UseMana(id, SPL_FLASH);
		} else {
			missile[mi]._midam = currlevel >> 1;
		}
	} else {
		missile[mi]._midam = 2 * monster[id].mLevel;
	}

	missile[mi]._mirange = 19;
}

void AddFlash2(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	if(mienemy == 0) {
		if(id != -1) {
			missile[mi]._midam = 0; /// BUGFIX: damage mechanics should be the same as 'AddFlash' above?
			for(i = 0; i <= plr[id]._pLevel; i++) {
				missile[mi]._midam += random(56, 2) + 1;
			}
			for(i = missile[mi]._mispllvl; i > 0; i--) {
				missile[mi]._midam += missile[mi]._midam >> 3;
			}
			missile[mi]._midam += missile[mi]._midam >> 1;
		} else {
			missile[mi]._midam = currlevel >> 1;
		}
	}

	missile[mi]._miPreFlag = 1;
	missile[mi]._mirange = 19;
}

void AddManashield(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mirange = 48 * plr[id]._pLevel;
	missile[mi]._miVar1 = plr[id]._pHitPoints;
	missile[mi]._miVar2 = plr[id]._pHPBase;
	missile[mi]._miVar8 = -1;

	if(mienemy == 0) {
		UseMana(id, SPL_MANASHIELD);
	}
	if(id == myplr) {
		NetSendCmd(TRUE, CMD_SETSHIELD);
	}

	plr[id].pManaShield = 1;
}

void AddFiremove(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._midam = random(59, 10) + plr[id]._pLevel + 1;
	GetMissileVel(mi, sx, sy, dx, dy, 16);
	missile[mi]._mirange = 255;
	missile[mi]._miVar1 = 0;
	missile[mi]._miVar2 = 0;
	missile[mi]._mix++;
	missile[mi]._miy++;
	missile[mi]._miyoff -= 32;
}

void AddGuardian(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i, pn, k, l, j, tx, ty;
	int CrawlNum[6] = { 0, 3, 12, 45, 94, 159 };

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._midam = random(62, 10) + (plr[id]._pLevel >> 1) + 1;
	for(i = missile[mi]._mispllvl; i > 0; i--) {
		missile[mi]._midam += missile[mi]._midam >> 3;
	}

	missile[mi]._miDelFlag = 1;

	for(j = 0; j < 6; j++) {
		k = CrawlNum[j];
		l = k + 1;
		for(i = (BYTE)CrawlTable[k]; i > 0; i--) {
			tx = dx + CrawlTable[l];
			ty = dy + CrawlTable[l + 1];
			/// ASSERT: assert((DWORD)tx < MAXDUNX);
			/// ASSERT: assert((DWORD)ty < MAXDUNY);
			pn = dPiece[tx][ty];
			/// ASSERT: assert((DWORD)pn <= MAXTILES);
			if(tx > 0 && tx < MAXDUNX && ty > 0 && ty < MAXDUNY) {
				if(LineClear(sx, sy, tx, ty)) {
					if(!(dMonster[tx][ty] | dObject[tx][ty] | dMissile[tx][ty] | nSolidTable[pn] | nMissileTable[pn])) {
						missile[mi]._mix = tx;
						missile[mi]._miy = ty;
						missile[mi]._misx = tx;
						missile[mi]._misy = ty;
						missile[mi]._miDelFlag = 0;
						UseMana(id, SPL_GUARDIAN);
						j = 6;
						break;
					}
				}
			}
			l += 2;
		}
	}

	if(missile[mi]._miDelFlag != 1) {
		missile[mi]._misource = id;
		missile[mi]._mlid = AddLight(missile[mi]._mix, missile[mi]._miy, 1);
		missile[mi]._mirange = missile[mi]._mispllvl + (plr[id]._pLevel >> 1);
		missile[mi]._mirange += missile[mi]._mirange * plr[id]._pISplDur >> 7;
		if(missile[mi]._mirange > 30) {
			missile[mi]._mirange = 30;
		}
		missile[mi]._mirange <<= 4;
		if(missile[mi]._mirange < 30) {
			missile[mi]._mirange = 30;
		}
		missile[mi]._miVar1 = missile[mi]._mirange - missile[mi]._miAnimLen;
		missile[mi]._miVar2 = 0;
		missile[mi]._miVar3 = 1;
	}
}

void AddChain(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miVar1 = dx;
	missile[mi]._miVar2 = dy;
	missile[mi]._mirange = 1;
	UseMana(id, SPL_CHAIN);
}

void miss_null_11(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	SetMissDir(mi, dx);
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._midam = 0;
	missile[mi]._miLightFlag = 1;
	missile[mi]._mirange = 250;
}

void miss_null_12(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	if(dx > 3) {
		dx = 2;
	}

	SetMissDir(mi, dx);
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._midam = 0;
	missile[mi]._miLightFlag = 1;
	missile[mi]._mirange = 250;
}

void miss_null_13(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	if(dx > 3) {
		dx = 2;
	}

	SetMissDir(mi, dx);
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._midam = 0;
	missile[mi]._miLightFlag = 1;
	missile[mi]._mirange = missile[mi]._miAnimLen;
}

void AddRhino(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	AnimStruct *anim;

	/// ASSERT: assert((DWORD)id < MAXMONSTERS);
	/// ASSERT: assert(monster[id].MType != NULL);
	if(monster[id].MType->mtype >= MT_HORNED && monster[id].MType->mtype <= MT_OBLORD) {
		anim = &monster[id].MType->Anims[5];
	} else if(monster[id].MType->mtype >= MT_NSNAKE && monster[id].MType->mtype <= MT_GSNAKE) {
		anim = &monster[id].MType->Anims[2];
	} else {
		anim = &monster[id].MType->Anims[1];
	}

	GetMissileVel(mi, sx, sy, dx, dy, 18);
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mimfnum = midir;
	missile[mi]._miAnimFlags = 0;
	missile[mi]._miAnimData = anim->Frames[midir];
	missile[mi]._miAnimDelay = anim->Delay;
	missile[mi]._miAnimLen = anim->Rate;
	missile[mi]._miAnimWidth = monster[id].MType->flags_1;
	missile[mi]._miAnimWidth2 = monster[id].MType->flags_2;
	missile[mi]._miAnimAdd = 1;
	if(monster[id].MType->mtype >= MT_NSNAKE && monster[id].MType->mtype <= MT_GSNAKE) {
		missile[mi]._miAnimFrame = 7;
	}
	missile[mi]._miVar1 = 0;
	missile[mi]._miVar2 = 0;
	missile[mi]._miLightFlag = 1;
	if(monster[id]._uniqtype != 0) {
		missile[mi]._miUniqTrans = monster[id]._uniqtrans + 1;
		missile[mi]._mlid = monster[id].mlid;
	}
	missile[mi]._mirange = 256;
	PutMissile(mi);
}

void miss_null_32(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	AnimStruct *anim;

	/// ASSERT: assert((DWORD)id < MAXMONSTERS);
	/// ASSERT: assert(monster[id].MType != NULL);
	anim = &monster[id].MType->Anims[1];

	GetMissileVel(mi, sx, sy, dx, dy, 16);
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mimfnum = midir;
	missile[mi]._miAnimFlags = 0;
	missile[mi]._miAnimData = anim->Frames[midir];
	missile[mi]._miAnimDelay = anim->Delay;
	missile[mi]._miAnimLen = anim->Rate;
	missile[mi]._miAnimWidth = monster[id].MType->flags_1;
	missile[mi]._miAnimWidth2 = monster[id].MType->flags_2;
	missile[mi]._miAnimAdd = 1;
	missile[mi]._miVar1 = 0;
	missile[mi]._miVar2 = 0;
	missile[mi]._miLightFlag = 1;
	if(monster[id]._uniqtype != 0) {
		missile[mi]._miUniqTrans = monster[id]._uniqtrans + 1;
	}
	dMonster[monster[id]._mx][monster[id]._my] = 0;
	missile[mi]._mirange = 256;
	PutMissile(mi);
}

void AddFlare(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	long l;

	if(sx == dx && sy == dy) {
		dx += XDirAdd[midir];
		dy += YDirAdd[midir];
	}

	GetMissileVel(mi, sx, sy, dx, dy, 16);
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mirange = 256;
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	missile[mi]._mlid = AddLight(sx, sy, 8);

	if(mienemy == 0) {
		UseMana(id, SPL_FLARE);
		l = 5;
#ifdef _DEBUG
		if(debug_mode_dollar_sign || debug_mode_key_inverted_v) {
			l = 0;
		}
#endif
		plr[id]._pHitPoints -= l << 6;
		plr[id]._pHPBase -= l << 6;
		drawhpflag = 1;
		if(plr[id]._pHitPoints <= 0) {
			SyncPlrKill(id, 0);
		}
	} else if(id > 0) {
		if(monster[id].MType->mtype == MT_SUCCUBUS) {
			SetMissAnim(mi, MFILE_FLARE);
		}
		if(monster[id].MType->mtype == MT_SNOWWICH) {
			SetMissAnim(mi, MFILE_SCUBMISB);
		}
		if(monster[id].MType->mtype == MT_HLSPWN) {
			SetMissAnim(mi, MFILE_SCUBMISD);
		}
		if(monster[id].MType->mtype == MT_SOLBRNR) {
			SetMissAnim(mi, MFILE_SCUBMISC);
		}
	}
}

void AddAcid(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	GetMissileVel(mi, sx, sy, dx, dy, 16);
	SetMissDir(mi, GetDirection16(sx, sy, dx, dy));
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mirange = 5 * (monster[id]._mint + 4);
	missile[mi]._mlid = -1;
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	PutMissile(mi);
}

void miss_null_1D(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._midam = dam;
	missile[mi]._mixvel = 0;
	missile[mi]._miyvel = 0;
	missile[mi]._mirange = 50;
	missile[mi]._miVar1 = missile[mi]._mirange - missile[mi]._miAnimLen;
	missile[mi]._miVar2 = 0;
}

void AddAcidpud(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int monst;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mixvel = 0;
	missile[mi]._miyvel = 0;
	missile[mi]._mixoff = 0;
	missile[mi]._miyoff = 0;
	missile[mi]._miLightFlag = 1;
	monst = missile[mi]._misource;
	missile[mi]._mirange = random(50, 15) + 40 * (monster[monst]._mint + 1);
	missile[mi]._miPreFlag = 1;
}

void AddStone(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i, j, k, l, tx, ty, mid;
	int CrawlNum[6] = { 0, 3, 12, 45, 94, 159 };

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._misource = id;

	for(j = 0; j < 6; j++) {
		k = CrawlNum[j];
		l = k + 1;
		for(i = (BYTE)CrawlTable[k]; i > 0; i--) {
			tx = dx + CrawlTable[l];
			ty = dy + CrawlTable[l + 1];
			if(tx > 0 && tx < MAXDUNX && ty > 0 && ty < MAXDUNY) {
				mid = dMonster[tx][ty];
				mid = mid > 0 ? mid - 1 : -(mid + 1);
				if(mid > 3 && monster[mid]._mAi != AI_DIABLO) {
					if(monster[mid]._mmode != MM_FADEIN && monster[mid]._mmode != MM_FADEOUT && monster[mid]._mmode != MM_CHARGE) {
						i = -99;
						j = 6;
						missile[mi]._miVar1 = monster[mid]._mmode;
						missile[mi]._miVar2 = mid;
						monster[mid]._mmode = MM_STONE;
						break;
					}
				}
			}
			l += 2;
		}
	}

	if(i != -99) {
		missile[mi]._miDelFlag = 1;
	} else {
		missile[mi]._mix = tx;
		missile[mi]._miy = ty;
		missile[mi]._misx = tx;
		missile[mi]._misy = ty;
		missile[mi]._mirange = missile[mi]._mispllvl + 6;
		missile[mi]._mirange += missile[mi]._mirange * plr[id]._pISplDur >> 7;
		if(missile[mi]._mirange > 15) {
			missile[mi]._mirange = 15;
		}
		missile[mi]._mirange <<= 4;
		UseMana(id, SPL_STONE);
	}
}

void AddGolem(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i, mx;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 0;

	for(i = 0; i < nummissiles; i++) {
		mx = missileactive[i];
		if(missile[mx]._mitype == MIS_GOLEM && mx != mi && missile[mx]._misource == id) {
			missile[mi]._miDelFlag = 1;
			return;
		}
	}

	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	missile[mi]._miVar4 = dx;
	missile[mi]._miVar5 = dy;

	if((monster[id]._mx != 1 || monster[id]._my != 0) && id == myplr) {
		M_StartKill(id, id);
	}

	UseMana(id, SPL_GOLEM);
}

void AddEtherealize(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mirange = 16 * plr[id]._pLevel >> 1;
	for(i = missile[mi]._mispllvl; i > 0; i--) {
		missile[mi]._mirange += missile[mi]._mirange >> 3;
	}
	missile[mi]._mirange += missile[mi]._mirange * plr[id]._pISplDur >> 7;
	missile[mi]._miVar1 = plr[id]._pHitPoints;
	missile[mi]._miVar2 = plr[id]._pHPBase;

	if(mienemy == 0) {
		UseMana(id, SPL_ETHEREALIZE);
	}
}

void miss_null_1F(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 1;
}

void miss_null_23(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._midam = dam;
	missile[mi]._mix = sx;
	missile[mi]._miy = sy;
	missile[mi]._misx = sx;
	missile[mi]._misy = sy;
	missile[mi]._misource = id;
	if(dam == 1) {
		SetMissDir(mi, 0);
	} else {
		SetMissDir(mi, 1);
	}
	missile[mi]._miLightFlag = 1;
	missile[mi]._mirange = missile[mi]._miAnimLen;
}

void AddBoom(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mix = dx;
	missile[mi]._miy = dy;
	missile[mi]._misx = dx;
	missile[mi]._misy = dy;
	missile[mi]._mixvel = 0;
	missile[mi]._miyvel = 0;
	missile[mi]._midam = dam;
	missile[mi]._mirange = missile[mi]._miAnimLen;
	missile[mi]._miVar1 = 0;
}

void AddHeal(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i;
	long l;

	l = (random(57, 10) + 1) << 6;
	for(i = 0; i < plr[id]._pLevel; i++) {
		l += (random(57, 4) + 1) << 6;
	}
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	for(i = 0; i < missile[mi]._mispllvl; i++) {
		l += (random(57, 6) + 1) << 6;
	}
	if(plr[id]._pClass == PC_WARRIOR) {
		l *= 2;
	}
	if(plr[id]._pClass == PC_ROGUE) {
		l += l >> 1;
	}

	plr[id]._pHitPoints += l;
	if(plr[id]._pHitPoints > plr[id]._pMaxHP) {
		plr[id]._pHitPoints = plr[id]._pMaxHP;
	}
	plr[id]._pHPBase += l;
	if(plr[id]._pHPBase > plr[id]._pMaxHPBase) {
		plr[id]._pHPBase = plr[id]._pMaxHPBase;
	}

	UseMana(id, SPL_HEAL);
	drawhpflag = 1;
	missile[mi]._miDelFlag = 1;
}

void AddHealOther(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 1;
	UseMana(id, SPL_HEALOTHER);

	if(id == myplr) {
		SetCursor_(CURSOR_HEALOTHER);
	}
}

void AddElement(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i;

	if(sx == dx && sy == dy) {
		dx += XDirAdd[midir];
		dy += YDirAdd[midir];
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._midam = 2 * (random(60, 10) + random(60, 10) + plr[id]._pLevel + 2);
	for(i = missile[mi]._mispllvl; i > 0; i--) {
		missile[mi]._midam += missile[mi]._midam >> 3;
	}
	missile[mi]._midam >>= 1;
	GetMissileVel(mi, sx, sy, dx, dy, 16);
	SetMissDir(mi, GetDirection8(sx, sy, dx, dy));
	missile[mi]._mirange = 256;
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	missile[mi]._miVar3 = 0;
	missile[mi]._miVar4 = dx;
	missile[mi]._miVar5 = dy;
	missile[mi]._mlid = AddLight(sx, sy, 8);
	UseMana(id, SPL_ELEMENT);
}

void AddIdentify(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 1;
	UseMana(id, SPL_IDENTIFY);

	if(id == myplr) {
		if(sbookflag) {
			sbookflag = 0;
		}
		if(!invflag) {
			invflag = 1;
		}
		SetCursor_(CURSOR_IDENTIFY);
	}
}

void AddFirewallC(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i, pn, k, l, j, tx, ty;
	int CrawlNum[6] = { 0, 3, 12, 45, 94, 159 };

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 1;

	for(j = 0; j < 6; j++) {
		k = CrawlNum[j];
		l = k + 1;
		for(i = (BYTE)CrawlTable[k]; i > 0; i--) {
			tx = dx + CrawlTable[l];
			ty = dy + CrawlTable[l + 1];
			if(tx > 0 && tx < MAXDUNX && ty > 0 && ty < MAXDUNY) {
				pn = dPiece[tx][ty];
				if(LineClear(sx, sy, tx, ty)) {
					if((sx != tx || sy != ty) && !(nSolidTable[pn] | dObject[tx][ty])) {
						missile[mi]._miVar1 = tx;
						missile[mi]._miVar2 = ty;
						missile[mi]._miVar5 = tx;
						missile[mi]._miVar6 = ty;
						missile[mi]._miDelFlag = 0;
						j = 6;
						break;
					}
				}
			}
			l += 2;
		}
	}

	if(missile[mi]._miDelFlag != 1) {
		missile[mi]._miVar7 = 0;
		missile[mi]._miVar8 = 0;
		missile[mi]._miVar3 = (midir - 2) & 7;
		missile[mi]._miVar4 = (midir + 2) & 7;
		missile[mi]._mirange = 7;
		UseMana(id, SPL_FIREWALL);
	}
}

void AddInfra(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mirange = 1584;
	for(i = missile[mi]._mispllvl; i > 0; i--) {
		missile[mi]._mirange += missile[mi]._mirange >> 3;
	}
	missile[mi]._mirange += missile[mi]._mirange * plr[id]._pISplDur >> 7;

	if(mienemy == 0) {
		UseMana(id, SPL_INFRA);
	}
}

void AddWave(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	missile[mi]._miVar1 = dx;
	missile[mi]._miVar2 = dy;
	missile[mi]._miVar3 = 0;
	missile[mi]._miVar4 = 0;
	missile[mi]._mirange = 1;
	missile[mi]._miAnimFrame = 4;
	UseMana(id, SPL_WAVE);
}

void AddNova(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int k;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miVar1 = dx;
	missile[mi]._miVar2 = dy;

	if(id != -1) {
		missile[mi]._midam = random(66, 6) + random(66, 6) + random(66, 6) + random(66, 6) + random(66, 6);
		missile[mi]._midam += plr[id]._pLevel + 5;
		missile[mi]._midam >>= 1;
		for(k = missile[mi]._mispllvl; k > 0; k--) {
			missile[mi]._midam += missile[mi]._midam >> 3;
		}
		if(mienemy == 0) {
			UseMana(id, SPL_NOVA);
		}
	} else {
		missile[mi]._midam = random(66, 3) + random(66, 3) + random(66, 3);
		missile[mi]._midam += currlevel >> 1;
	}

	missile[mi]._mirange = 1;
}

void AddBlodboil(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	missile[mi]._miDelFlag = 1;
}

void AddRepair(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 1;
	UseMana(id, SPL_REPAIR);

	if(id == myplr) {
		if(sbookflag) {
			sbookflag = 0;
		}
		if(!invflag) {
			invflag = 1;
		}
		SetCursor_(CURSOR_REPAIR);
	}
}

void AddRecharge(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 1;
	UseMana(id, SPL_RECHARGE);

	if(id == myplr) {
		if(sbookflag) {
			sbookflag = 0;
		}
		if(!invflag) {
			invflag = 1;
		}
		SetCursor_(CURSOR_RECHARGE);
	}
}

void AddDisarm(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 1;
	UseMana(id, SPL_DISARM);

	if(id == myplr) {
		SetCursor_(CURSOR_DISARM);
	}
}

void AddApoca(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miVar1 = 8;
	missile[mi]._miVar2 = sy - missile[mi]._miVar1;
	missile[mi]._miVar3 = sy + missile[mi]._miVar1;
	missile[mi]._miVar4 = sx - missile[mi]._miVar1;
	missile[mi]._miVar5 = sx + missile[mi]._miVar1;
	missile[mi]._miVar6 = missile[mi]._miVar4;

	if(missile[mi]._miVar2 <= 0) {
		missile[mi]._miVar2 = 1;
	}
	if(missile[mi]._miVar3 >= MAXDUNY) {
		missile[mi]._miVar3 = MAXDUNY - 1;
	}
	if(missile[mi]._miVar4 <= 0) {
		missile[mi]._miVar4 = 1;
	}
	if(missile[mi]._miVar5 >= MAXDUNX) {
		missile[mi]._miVar5 = MAXDUNX - 1;
	}

	for(i = 0; i < plr[id]._pLevel; i++) {
		missile[mi]._midam += random(67, 6) + 1;
	}

	missile[mi]._mirange = 255;
	missile[mi]._miDelFlag = 0;
	UseMana(id, SPL_APOCA);
}

void AddFlame(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int i;

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miVar2 = 0;
	for(i = dam; i > 0; i--) {
		missile[mi]._miVar2 += 5;
	}
	missile[mi]._misx = dx;
	missile[mi]._misy = dy;
	missile[mi]._mixoff = missile[midir]._mixoff;
	missile[mi]._miyoff = missile[midir]._miyoff;
	missile[mi]._mitxoff = missile[midir]._mitxoff;
	missile[mi]._mityoff = missile[midir]._mityoff;
	missile[mi]._mirange = missile[mi]._miVar2 + 20;
	missile[mi]._mlid = AddLight(sx, sy, 1);

	if(mienemy == 0) {
		missile[mi]._midam = 8 * (random(79, 2) + random(79, plr[id]._pLevel) + 2);
		missile[mi]._midam += missile[mi]._midam >> 1;
	} else {
		missile[mi]._midam = random(77, monster[id].mMaxDamage - monster[id].mMinDamage + 1) + monster[id].mMinDamage;
	}
}

void AddFlamec(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	if(sx == dx && sy == dy) {
		dx += XDirAdd[midir];
		dy += YDirAdd[midir];
	}

	GetMissileVel(mi, sx, sy, dx, dy, 32);

	if(mienemy == 0) {
		UseMana(id, SPL_FLAME);
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	missile[mi]._miVar3 = 0;
	missile[mi]._mirange = 256;
}

void AddCbolt(int mi, int sx, int sy, int dx, int dy, int midir, char micaster, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	if(micaster == 0) {
		if(id == myplr) {
			missile[mi]._mirnd = random(63, 15) + 1;
			missile[mi]._midam = random(68, plr[id]._pMagic >> 2) + 1;
		} else {
			missile[mi]._mirnd = random(63, 15) + 1;
			missile[mi]._midam = random(68, plr[id]._pMagic >> 2) + 1;
		}
	} else {
		missile[mi]._mirnd = random(63, 15) + 1;
		missile[mi]._midam = 15;
	}

	if(sx == dx && sy == dy) {
		dx += XDirAdd[midir];
		dy += YDirAdd[midir];
	}

	missile[mi]._miAnimFrame = random(63, 8) + 1;
	missile[mi]._mlid = AddLight(sx, sy, 5);
	GetMissileVel(mi, sx, sy, dx, dy, 8);
	missile[mi]._miVar1 = 5;
	missile[mi]._miVar2 = midir;
	missile[mi]._miVar3 = 0;
	missile[mi]._mirange = 256;
}

void AddHbolt(int mi, int sx, int sy, int dx, int dy, int midir, char micaster, int id, int dam)
{
	int sp;

	if(sx == dx && sy == dy) {
		dx += XDirAdd[midir];
		dy += YDirAdd[midir];
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	if(id != -1) {
		sp = 2 * missile[mi]._mispllvl + 16;
		if(sp >= 63) {
			sp = 63;
		}
	} else {
		sp = 16;
	}

	GetMissileVel(mi, sx, sy, dx, dy, sp);
	SetMissDir(mi, GetDirection16(sx, sy, dx, dy));
	missile[mi]._mirange = 256;
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	missile[mi]._mlid = AddLight(sx, sy, 8);
	missile[mi]._midam = random(69, 10) + plr[id]._pLevel + 9;
	UseMana(id, SPL_HBOLT);
}

void AddResurrect(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	UseMana(id, SPL_RESURRECT);

	if(id == myplr) {
		SetCursor_(CURSOR_RESURRECT);
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 1;
}

void AddResurrectBeam(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mix = dx;
	missile[mi]._miy = dy;
	missile[mi]._misx = missile[mi]._mix;
	missile[mi]._misy = missile[mi]._miy;
	missile[mi]._mixvel = 0;
	missile[mi]._miyvel = 0;
	missile[mi]._mirange = misfiledata[MFILE_RESSUR1].mAnimLen[0];
}

void AddTelekinesis(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 1;
	UseMana(id, SPL_TELEKINESIS);

	if(id == myplr) {
		SetCursor_(CURSOR_TELEKINESIS);
	}
}

void AddBoneSpirit(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	long l;

	if(sx == dx && sy == dy) {
		dx += XDirAdd[midir];
		dy += YDirAdd[midir];
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._midam = 0;
	GetMissileVel(mi, sx, sy, dx, dy, 16);
	SetMissDir(mi, GetDirection8(sx, sy, dx, dy));
	missile[mi]._mirange = 256;
	missile[mi]._miVar1 = sx;
	missile[mi]._miVar2 = sy;
	missile[mi]._miVar3 = 0;
	missile[mi]._miVar4 = dx;
	missile[mi]._miVar5 = dy;
	missile[mi]._mlid = AddLight(sx, sy, 8);

	if(mienemy == 0) {
		UseMana(id, SPL_BONESPIRIT);
		l = 6;
#ifdef _DEBUG
		if(debug_mode_dollar_sign || debug_mode_key_inverted_v) {
			l = 0;
		}
#endif
		plr[id]._pHitPoints -= l << 6;
		plr[id]._pHPBase -= l << 6;
		drawhpflag = 1;
		if(plr[id]._pHitPoints <= 0) {
			SyncPlrKill(id, 0);
		}
	}
}

void AddRportal(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._mix = sx;
	missile[mi]._miy = sy;
	missile[mi]._misx = sx;
	missile[mi]._misy = sy;
	missile[mi]._mirange = 100;
	missile[mi]._miVar1 = missile[mi]._mirange - missile[mi]._miAnimLen;
	missile[mi]._miVar2 = 0;
	PutMissile(mi);
}

void AddDiabApoca(int mi, int sx, int sy, int dx, int dy, int midir, char mienemy, int id, int dam)
{
	int pnum;

	for(pnum = 0; pnum < gbMaxPlayers; pnum++) {
		if(plr[pnum].plractive && LineClear(sx, sy, plr[pnum]._px, plr[pnum]._py)) {
			AddMissile(0, 0, plr[pnum]._px, plr[pnum]._py, 0, MIS_BOOM2, mienemy, id, dam, 0);
		}
	}

	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	missile[mi]._miDelFlag = 1;
}

int AddMissile(int sx, int sy, int v1, int v2, int midir, int mitype, char micaster, int id, int v3, int spllvl)
{
	int i, mi;

	if(nummissiles >= MAXMISSILES) {
		return -1;
	}

	if(mitype == MIS_MANASHIELD && plr[id].pManaShield == 1) {
		if(currlevel != plr[id].plrlevel) {
			return -1;
		}
		for(i = 0; i < nummissiles; i++) {
			mi = missileactive[i];
			if(missile[mi]._mitype == MIS_MANASHIELD && missile[mi]._misource == id) {
				return -1;
			}
		}
	}

	mi = missileavail[0];
	missileavail[0] = missileavail[MAXMISSILES - nummissiles - 1];
	missileactive[nummissiles] = mi;
	nummissiles++;

	missile[mi]._mitype = mitype;
	missile[mi]._micaster = micaster;
	missile[mi]._misource = id;
	missile[mi]._miAnimType = missiledata[mitype].mFileNum;
	missile[mi]._miDrawFlag = missiledata[mitype].mDraw;
	missile[mi]._mispllvl = spllvl;
	missile[mi]._mimfnum = midir;

	if(missile[mi]._miAnimType != 255 && misfiledata[missile[mi]._miAnimType].mAnimFAmt >= 8) {
		SetMissDir(mi, midir);
	} else {
		SetMissDir(mi, 0);
	}

	missile[mi]._mix = sx;
	missile[mi]._miy = sy;
	missile[mi]._mixoff = 0;
	missile[mi]._miyoff = 0;
	missile[mi]._misx = sx;
	missile[mi]._misy = sy;
	missile[mi]._mitxoff = 0;
	missile[mi]._mityoff = 0;
	missile[mi]._miDelFlag = 0;
	missile[mi]._miAnimAdd = 1;
	missile[mi]._miLightFlag = 0;
	missile[mi]._miPreFlag = 0;
	missile[mi]._miUniqTrans = 0;
	missile[mi]._midam = v3;
	missile[mi]._miHitFlag = 0;
	missile[mi]._midist = 0;
	missile[mi]._mlid = -1;
	missile[mi]._mirnd = 0;

	if(missiledata[mitype].mlSFX != -1) {
		PlaySfxLoc(missiledata[mitype].mlSFX, sx, sy);
	}

	missiledata[mitype].mAddProc(mi, sx, sy, v1, v2, midir, micaster, id, v3);

	return mi;
}

int Sentfire(int i, int sx, int sy)
{
	int ex, dir;

	ex = 0;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	if(LineClear(missile[i]._mix, missile[i]._miy, sx, sy)) {
		if(dMonster[sx][sy] > 0 && monster[dMonster[sx][sy] - 1]._mhitpoints >> 6 > 0 && dMonster[sx][sy] - 1 > 3) {
			dir = GetDirection(missile[i]._mix, missile[i]._miy, sx, sy);
			missile[i]._miVar3 = missileavail[0];
			AddMissile(
				missile[i]._mix,
				missile[i]._miy,
				sx,
				sy,
				dir,
				MIS_FIREBOLT,
				0,
				missile[i]._misource,
				missile[i]._midam,
				GetSpellLevel(missile[i]._misource, 1));
			ex = -1;
		}
	}
	if(ex == -1) {
		SetMissDir(i, 2);
		missile[i]._miVar2 = 3;
	}

	return ex;
}

void MI_Dummy(int i)
{
	return;
}

void MI_Golem(int i)
{
	int id, pn, j, k, l, m, tx, ty;
	int CrawlNum[6] = { 0, 3, 12, 45, 94, 159 };

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	id = missile[i]._misource;

	if(monster[id]._mx == 1 && monster[id]._my == 0) {
		for(j = 0; j < 6; j++) {
			k = CrawlNum[j];
			l = k + 1;
			for(m = (BYTE)CrawlTable[k]; m > 0; m--) {
				tx = missile[i]._miVar4 + CrawlTable[l];
				ty = missile[i]._miVar5 + CrawlTable[l + 1];
				if(tx > 0 && tx < MAXDUNX && ty > 0 && ty < MAXDUNY) {
					pn = dPiece[tx][ty];
					if(LineClear(missile[i]._miVar1, missile[i]._miVar2, tx, ty)) {
						if(!(dMonster[tx][ty] | nSolidTable[pn] | dObject[tx][ty])) {
							j = 6;
							SpawnGolum(id, tx, ty, i);
							break;
						}
					}
				}
				l += 2;
			}
		}
	}

	missile[i]._miDelFlag = 1;
}

void MI_SetManashield(int i)
{
	ManashieldFlag = 1;
}

void MI_LArrow(int i)
{
	int p, mind, maxd, rst;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	p = missile[i]._misource;

	if(missile[i]._miAnimType != MFILE_MINILTNG && missile[i]._miAnimType != MFILE_MAGBLOS) {
		missile[i]._midist++;
		missile[i]._mitxoff += missile[i]._mixvel;
		missile[i]._mityoff += missile[i]._miyvel;
		GetMissilePos(i);
		if(p != -1) {
			if(missile[i]._micaster == 0) {
				mind = plr[p]._pIMinDam;
				maxd = plr[p]._pIMaxDam;
			} else {
				mind = monster[p].mMinDamage;
				maxd = monster[p].mMaxDamage;
			}
		} else {
			mind = random(68, 10) + currlevel + 1;
			maxd = random(68, 10) + 2 * currlevel + 1;
		}
		if(missile[i]._mix != missile[i]._misx || missile[i]._miy != missile[i]._misy) {
			rst = missiledata[missile[i]._mitype].mResist;
			missiledata[missile[i]._mitype].mResist = 0;
			CheckMissileCol(i, mind, maxd, 0, missile[i]._mix, missile[i]._miy, 0);
			missiledata[missile[i]._mitype].mResist = rst;
		}
		if(missile[i]._mirange == 0) {
			missile[i]._mimfnum = 0;
			missile[i]._mitxoff -= missile[i]._mixvel;
			missile[i]._mityoff -= missile[i]._miyvel;
			GetMissilePos(i);
			if(missile[i]._mitype == MIS_LARROW) {
				SetMissAnim(i, MFILE_MINILTNG);
			} else {
				SetMissAnim(i, MFILE_MAGBLOS);
			}
			missile[i]._mirange = missile[i]._miAnimLen - 1;
		} else if(missile[i]._mix != missile[i]._miVar1 || missile[i]._miy != missile[i]._miVar2) {
			missile[i]._miVar1 = missile[i]._mix;
			missile[i]._miVar2 = missile[i]._miy;
			ChangeLight(missile[i]._mlid, missile[i]._miVar1, missile[i]._miVar2, 5);
		}
	} else {
		ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, missile[i]._miAnimFrame + 5);
		rst = missiledata[missile[i]._mitype].mResist;
		if(missile[i]._mitype == MIS_LARROW) {
			if(p != -1) {
				mind = plr[p]._pILMinDam;
				maxd = plr[p]._pILMaxDam;
			} else {
				mind = random(68, 10) + currlevel + 1;
				maxd = random(68, 10) + 2 * currlevel + 1;
			}
			missiledata[MIS_LARROW].mResist = 2;
			CheckMissileCol(i, mind, maxd, 0, missile[i]._mix, missile[i]._miy, 1);
		}
		if(missile[i]._mitype == MIS_FARROW) {
			if(p != -1) {
				mind = plr[p]._pIFMinDam;
				maxd = plr[p]._pIFMaxDam;
			} else {
				mind = random(68, 10) + currlevel + 1;
				maxd = random(68, 10) + 2 * currlevel + 1;
			}
			missiledata[MIS_FARROW].mResist = 1;
			CheckMissileCol(i, mind, maxd, 0, missile[i]._mix, missile[i]._miy, 1);
		}
		missiledata[missile[i]._mitype].mResist = rst;
	}

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		AddUnLight(missile[i]._mlid);
	}

	PutMissile(i);
}

void MI_Arrow(int i)
{
	int p, mind, maxd;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	missile[i]._midist++;
	missile[i]._mitxoff += missile[i]._mixvel;
	missile[i]._mityoff += missile[i]._miyvel;
	GetMissilePos(i);

	p = missile[i]._misource;
	if(p != -1) {
		if(missile[i]._micaster == 0) {
			mind = plr[p]._pIMinDam;
			maxd = plr[p]._pIMaxDam;
		} else {
			mind = monster[p].mMinDamage;
			maxd = monster[p].mMaxDamage;
		}
	} else {
		mind = currlevel;
		maxd = 2 * currlevel;
	}

	if(missile[i]._mix != missile[i]._misx || missile[i]._miy != missile[i]._misy) {
		CheckMissileCol(i, mind, maxd, 0, missile[i]._mix, missile[i]._miy, 0);
	}
	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
	}

	PutMissile(i);
}

void MI_Firebolt(int i)
{
	int omx, omy, d, p;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;

	if(missile[i]._mitype == MIS_BONESPIRIT && missile[i]._mimfnum == 8) {
		if(missile[i]._mirange == 0) {
			if(missile[i]._mlid >= 0) {
				AddUnLight(missile[i]._mlid);
			}
			missile[i]._miDelFlag = 1;
			PlaySfxLoc(LS_BSIMPCT, missile[i]._mix, missile[i]._miy);
		}
		PutMissile(i);
		return;
	}

	omx = missile[i]._mitxoff;
	omy = missile[i]._mityoff;
	missile[i]._mitxoff += missile[i]._mixvel;
	missile[i]._mityoff += missile[i]._miyvel;
	GetMissilePos(i);

	p = missile[i]._misource;
	if(p != -1) {
		if(missile[i]._micaster == 0) {
			switch(missile[i]._mitype) {
			case MIS_FIREBOLT:
				d = (plr[p]._pMagic >> 3) + random(75, 10) + missile[i]._mispllvl + 1;
				break;
			case MIS_FLARE:
				d = (plr[p]._pMagic >> 1) + 3 * missile[i]._mispllvl - (plr[p]._pMagic >> 3);
				break;
			case MIS_BONESPIRIT:
				d = 0;
				break;
			}
		} else {
			d = random(77, monster[p].mMaxDamage - monster[p].mMinDamage + 1) + monster[p].mMinDamage;
		}
	} else {
		d = random(78, 2 * currlevel) + currlevel;
	}

	if(missile[i]._mix != missile[i]._misx || missile[i]._miy != missile[i]._misy) {
		CheckMissileCol(i, d, d, 0, missile[i]._mix, missile[i]._miy, 0);
	}

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		missile[i]._mitxoff = omx;
		missile[i]._mityoff = omy;
		GetMissilePos(i);
		switch(missile[i]._mitype) {
		case MIS_FLARE:
			AddMissile(missile[i]._mix, missile[i]._miy, i, 0, missile[i]._mimfnum, MIS_MISEXP2, missile[i]._micaster, missile[i]._misource, 0, 0);
			break;
		case MIS_FIREBOLT:
		case MIS_MAGMABALL:
			AddMissile(missile[i]._mix, missile[i]._miy, i, 0, missile[i]._mimfnum, MIS_MISEXP, missile[i]._micaster, missile[i]._misource, 0, 0);
			break;
		case MIS_ACID:
			AddMissile(missile[i]._mix, missile[i]._miy, i, 0, missile[i]._mimfnum, MIS_MISEXP3, missile[i]._micaster, missile[i]._misource, 0, 0);
			break;
		case MIS_BONESPIRIT:
			SetMissDir(i, 8);
			missile[i]._mirange = 7;
			missile[i]._miDelFlag = 0;
			PutMissile(i);
			return;
		}
		if(missile[i]._mlid >= 0) {
			AddUnLight(missile[i]._mlid);
		}
	} else if(missile[i]._mix != missile[i]._miVar1 || missile[i]._miy != missile[i]._miVar2) {
		missile[i]._miVar1 = missile[i]._mix;
		missile[i]._miVar2 = missile[i]._miy;
		if(missile[i]._mlid >= 0) {
			ChangeLight(missile[i]._mlid, missile[i]._miVar1, missile[i]._miVar2, 8);
		}
	}

	PutMissile(i);
}

void MI_Lightball(int i)
{
	int j, tx, ty, oi;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	tx = missile[i]._miVar1;
	ty = missile[i]._miVar2;
	missile[i]._mirange--;
	missile[i]._mitxoff += missile[i]._mixvel;
	missile[i]._mityoff += missile[i]._miyvel;
	GetMissilePos(i);
	j = missile[i]._mirange;
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, 0, missile[i]._mix, missile[i]._miy, 0);

	if(missile[i]._miHitFlag == 1) {
		missile[i]._mirange = j;
	}
	if(dObject[tx][ty] != 0 && tx == missile[i]._mix && ty == missile[i]._miy) {
		oi = dObject[tx][ty] > 0 ? dObject[tx][ty] - 1 : -(dObject[tx][ty] + 1);
		if(object[oi]._otype == OBJ_SHRINEL || object[oi]._otype == OBJ_SHRINER) {
			missile[i]._mirange = j;
		}
	}
	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
	}

	PutMissile(i);
}

void mi_null_33(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	missile[i]._mitxoff += missile[i]._mixvel;
	missile[i]._mityoff += missile[i]._miyvel;
	GetMissilePos(i);
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, 0, missile[i]._mix, missile[i]._miy, 0);

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
	}

	PutMissile(i);
}

void MI_Acidpud(int i)
{
	int range;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	range = missile[i]._mirange;
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, 1, missile[i]._mix, missile[i]._miy, 0);
	missile[i]._mirange = range;

	if(missile[i]._mirange == 0) {
		if(missile[i]._mimfnum != 0) {
			missile[i]._miDelFlag = 1;
		} else {
			SetMissDir(i, 1);
			missile[i]._mirange = missile[i]._miAnimLen;
		}
	}

	PutMissile(i);
}

void MI_Firewall(int i)
{
	int ExpLight[14] = { 2, 3, 4, 5, 5, 6, 7, 8, 9, 10, 11, 12, 12 }; /// BUGFIX: array size should be '12'

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;

	if(missile[i]._mirange == missile[i]._miVar1) {
		SetMissDir(i, 1);
		missile[i]._miAnimFrame = random(83, 11) + 1;
	}
	if(missile[i]._mirange == missile[i]._miAnimLen - 1) {
		SetMissDir(i, 0);
		missile[i]._miAnimFrame = 13;
		missile[i]._miAnimAdd = -1;
	}

	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, 1, missile[i]._mix, missile[i]._miy, 1);

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		AddUnLight(missile[i]._mlid);
	}
	if(missile[i]._mimfnum != 0 && missile[i]._mirange != 0 && missile[i]._miAnimAdd != -1 && missile[i]._miVar2 < 12) {
		if(missile[i]._miVar2 == 0) {
			missile[i]._mlid = AddLight(missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar2]);
		}
		ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar2]);
		missile[i]._miVar2++;
	}

	PutMissile(i);
}

void MI_Fireball(int i)
{
	int dam, px, py, id, mx, my;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	id = missile[i]._misource;
	dam = missile[i]._midam;
	missile[i]._mirange--;

	if(missile[i]._micaster == 0) {
		px = plr[id].WorldX;
		py = plr[id].WorldY;
	} else {
		px = monster[id]._mx;
		py = monster[id]._my;
	}

	if(missile[i]._miAnimType == MFILE_BIGEXP) {
		if(missile[i]._mirange == 0) {
			missile[i]._miDelFlag = 1;
			AddUnLight(missile[i]._mlid);
		}
	} else {
		missile[i]._mitxoff += missile[i]._mixvel;
		missile[i]._mityoff += missile[i]._miyvel;
		GetMissilePos(i);
		if(missile[i]._mix != missile[i]._misx || missile[i]._miy != missile[i]._misy) {
			CheckMissileCol(i, dam, dam, FALSE, missile[i]._mix, missile[i]._miy, FALSE);
		}
		if(missile[i]._mirange == 0) {
			mx = missile[i]._mix;
			my = missile[i]._miy;
			ChangeLight(missile[i]._mlid, mx, my, missile[i]._miAnimFrame);
			if(!CheckBlock(px, py, mx, my)) {
				CheckMissileCol(i, dam, dam, FALSE, mx, my, TRUE);
			}
			if(!CheckBlock(px, py, mx, my + 1)) {
				CheckMissileCol(i, dam, dam, FALSE, mx, my + 1, TRUE);
			}
			if(!CheckBlock(px, py, mx, my - 1)) {
				CheckMissileCol(i, dam, dam, FALSE, mx, my - 1, TRUE);
			}
			if(!CheckBlock(px, py, mx + 1, my)) {
				CheckMissileCol(i, dam, dam, FALSE, mx + 1, my, TRUE);
			}
			if(!CheckBlock(px, py, mx + 1, my - 1)) {
				CheckMissileCol(i, dam, dam, FALSE, mx + 1, my - 1, TRUE);
			}
			if(!CheckBlock(px, py, mx + 1, my + 1)) {
				CheckMissileCol(i, dam, dam, FALSE, mx + 1, my + 1, TRUE);
			}
			if(!CheckBlock(px, py, mx - 1, my)) {
				CheckMissileCol(i, dam, dam, FALSE, mx - 1, my, TRUE);
			}
			if(!CheckBlock(px, py, mx - 1, my + 1)) {
				CheckMissileCol(i, dam, dam, FALSE, mx - 1, my + 1, TRUE);
			}
			if(!CheckBlock(px, py, mx - 1, my - 1)) {
				CheckMissileCol(i, dam, dam, FALSE, mx - 1, my - 1, TRUE);
			}
			if(!TransList[dTransVal[mx][my]]
			|| missile[i]._mixvel < 0
			&& (TransList[dTransVal[mx][my + 1]] && nSolidTable[dPiece[mx][my + 1]]
			|| TransList[dTransVal[mx][my - 1]] && nSolidTable[dPiece[mx][my - 1]])) {
				missile[i]._mix++;
				missile[i]._miy++;
				missile[i]._miyoff -= 32;
			}
			if(missile[i]._miyvel > 0
			&& (TransList[dTransVal[mx + 1][my]] && nSolidTable[dPiece[mx + 1][my]]
			|| TransList[dTransVal[mx - 1][my]] && nSolidTable[dPiece[mx - 1][my]])) {
				missile[i]._miyoff -= 32;
			}
			if(missile[i]._mixvel > 0
			&& (TransList[dTransVal[mx][my + 1]] && nSolidTable[dPiece[mx][my + 1]]
			|| TransList[dTransVal[mx][my - 1]] && nSolidTable[dPiece[mx][my - 1]])) {
				missile[i]._mixoff -= 32;
			}
			missile[i]._mimfnum = 0;
			SetMissAnim(i, MFILE_BIGEXP);
			missile[i]._mirange = missile[i]._miAnimLen - 1;
		} else if(missile[i]._mix != missile[i]._miVar1 || missile[i]._miy != missile[i]._miVar2) {
			missile[i]._miVar1 = missile[i]._mix;
			missile[i]._miVar2 = missile[i]._miy;
			ChangeLight(missile[i]._mlid, missile[i]._miVar1, missile[i]._miVar2, 8);
		}
	}

	PutMissile(i);
}

void MI_Lightctrl(int i)
{
	int pn, dam, p, mx, my;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;

	p = missile[i]._misource;
	if(p != -1) {
		if(missile[i]._micaster == 0) {
			dam = (random(79, 2) + random(79, plr[p]._pLevel) + 2) << 6;
		} else {
			dam = 2 * (random(80, monster[p].mMaxDamage - monster[p].mMinDamage + 1) + monster[p].mMinDamage);
		}
	} else {
		dam = random(81, currlevel) + 2 * currlevel;
	}

	missile[i]._mitxoff += missile[i]._mixvel;
	missile[i]._mityoff += missile[i]._miyvel;
	GetMissilePos(i);

	mx = missile[i]._mix;
	my = missile[i]._miy;
	/// ASSERT: assert((DWORD)mx < MAXDUNX);
	/// ASSERT: assert((DWORD)my < MAXDUNY);
	pn = dPiece[missile[i]._mix][missile[i]._miy];
	/// ASSERT: assert((DWORD)pn <= MAXTILES);

	if(missile[i]._misource == -1) {
		if((missile[i]._mix != missile[i]._misx || missile[i]._miy != missile[i]._misy) && nMissileTable[pn]) {
			missile[i]._mirange = 0;
		}
	} else if(nMissileTable[pn]) {
		missile[i]._mirange = 0;
	}

	if(!nMissileTable[pn]) {
		if(missile[i]._mix != missile[i]._miVar1 || missile[i]._miy != missile[i]._miVar2) {
			if(mx > 0 && my > 0 && mx < MAXDUNX && my < MAXDUNY) {
				if(missile[i]._misource != -1) {
					if(missile[i]._micaster == 1
					&& monster[missile[i]._misource].MType->mtype >= MT_STORM
					&& monster[missile[i]._misource].MType->mtype <= MT_MAEL) {
						AddMissile(
							missile[i]._mix,
							missile[i]._miy,
							missile[i]._misx,
							missile[i]._misy,
							i,
							MIS_LIGHTNING2,
							missile[i]._micaster,
							missile[i]._misource,
							dam,
							missile[i]._mispllvl);
					} else {
						AddMissile(
							missile[i]._mix,
							missile[i]._miy,
							missile[i]._misx,
							missile[i]._misy,
							i,
							MIS_LIGHTNING,
							missile[i]._micaster,
							missile[i]._misource,
							dam,
							missile[i]._mispllvl);
					}
				} else {
					AddMissile(
						missile[i]._mix,
						missile[i]._miy,
						missile[i]._misx,
						missile[i]._misy,
						i,
						MIS_LIGHTNING,
						missile[i]._micaster,
						missile[i]._misource,
						dam,
						missile[i]._mispllvl);
				}
				missile[i]._miVar1 = missile[i]._mix;
				missile[i]._miVar2 = missile[i]._miy;
			}
		}
	}

	if(missile[i]._mirange == 0 || mx <= 0 || my <= 0 || mx >= MAXDUNX || my > MAXDUNY) { /// BUGFIX: 'my >='
		missile[i]._miDelFlag = 1;
	}
}

void MI_Lightning(int i)
{
	int j;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	j = missile[i]._mirange;

	if(missile[i]._mix != missile[i]._misx || missile[i]._miy != missile[i]._misy) {
		CheckMissileCol(i, missile[i]._midam, missile[i]._midam, TRUE, missile[i]._mix, missile[i]._miy, FALSE);
	}
	if(missile[i]._miHitFlag == 1) {
		missile[i]._mirange = j;
	}
	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		AddUnLight(missile[i]._mlid);
	}

	PutMissile(i);
}

void MI_Town(int i)
{
	int p;
	int ExpLight[17] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 15 };

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	if(missile[i]._mirange > 1) {
		missile[i]._mirange--;
	}
	if(missile[i]._mirange == missile[i]._miVar1) {
		SetMissDir(i, 1);
	}
	if(currlevel != 0 && missile[i]._mimfnum != 1 && missile[i]._mirange != 0) {
		if(missile[i]._miVar2 == 0) {
			missile[i]._mlid = AddLight(missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar2]);
		}
		ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar2]);
		missile[i]._miVar2++;
	}

	for(p = 0; p < MAX_PLRS; p++) {
		if(plr[p].plractive && currlevel == plr[p].plrlevel && !plr[p]._pLvlChanging && plr[p]._pmode == PM_STAND) {
			if(plr[p].WorldX == missile[i]._mix && plr[p].WorldY == missile[i]._miy) {
				ClrPlrPath(p);
				if(p == myplr) {
					NetSendCmdParam1(TRUE, CMD_WARP, missile[i]._misource);
					plr[p]._pmode = PM_NEWLVL;
				}
			}
		}
	}

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		AddUnLight(missile[i]._mlid);
	}

	PutMissile(i);
}

void MI_Flash(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	if(missile[i]._micaster == 0 && missile[i]._misource != -1) {
		plr[missile[i]._misource]._pInvincible = 1;
	}

	missile[i]._mirange--;
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, TRUE, missile[i]._mix - 1, missile[i]._miy, TRUE);
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, TRUE, missile[i]._mix, missile[i]._miy, TRUE);
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, TRUE, missile[i]._mix + 1, missile[i]._miy, TRUE);
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, TRUE, missile[i]._mix - 1, missile[i]._miy + 1, TRUE);
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, TRUE, missile[i]._mix, missile[i]._miy + 1, TRUE);
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, TRUE, missile[i]._mix + 1, missile[i]._miy + 1, TRUE);

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		if(missile[i]._micaster == 0 && missile[i]._misource != -1) {
			plr[missile[i]._misource]._pInvincible = 0;
		}
	}

	PutMissile(i);
}

void MI_Flash2(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	if(missile[i]._micaster == 0 && missile[i]._misource != -1) {
		plr[missile[i]._misource]._pInvincible = 1;
	}

	missile[i]._mirange--;
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, TRUE, missile[i]._mix - 1, missile[i]._miy - 1, TRUE);
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, TRUE, missile[i]._mix, missile[i]._miy - 1, TRUE);
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, TRUE, missile[i]._mix + 1, missile[i]._miy - 1, TRUE);

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		if(missile[i]._micaster == 0 && missile[i]._misource != -1) {
			plr[missile[i]._misource]._pInvincible = 0;
		}
	}

	PutMissile(i);
}

void MI_Manashield(int i)
{
	int id;
	long diff, pct;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	id = missile[i]._misource;
	missile[i]._mix = plr[id].WorldX;
	missile[i]._miy = plr[id].WorldY;
	missile[i]._mitxoff = plr[id]._pxoff << 16;
	missile[i]._mityoff = plr[id]._pyoff << 16;

	if(plr[id]._pmode == PM_WALK3) {
		missile[i]._misx = plr[id]._px;
		missile[i]._misy = plr[id]._py;
	} else {
		missile[i]._misx = plr[id].WorldX;
		missile[i]._misy = plr[id].WorldY;
	}

	GetMissilePos(i);

	if(plr[id]._pmode == PM_WALK3) {
		if(plr[id]._pdir == 2) {
			missile[i]._mix++;
		} else {
			missile[i]._miy++;
		}
	}

	if(id != myplr) {
		if(currlevel != plr[id].plrlevel) {
			missile[i]._miDelFlag = 1;
		}
		PutMissile(i);
		return;
	}

	if(plr[id]._pMana <= 0 || !plr[id].plractive) {
		missile[i]._mirange = 0;
	}

	if(plr[id]._pHitPoints < missile[i]._miVar1) {
		diff = missile[i]._miVar1 - plr[id]._pHitPoints;
		pct = 0;
		if(missile[i]._mispllvl > 0) {
			pct += 3;
		}
		if(pct > 0) {
			diff -= diff / pct;
		}
		if(diff < 0) {
			diff = 0;
		}
		drawmanaflag = 1;
		drawhpflag = 1;
		if(plr[id]._pMana >= diff) {
			plr[id]._pHitPoints = missile[i]._miVar1;
			plr[id]._pHPBase = missile[i]._miVar2;
			plr[id]._pMana -= diff;
			plr[id]._pManaBase -= diff;
		} else {
			plr[id]._pHitPoints = missile[i]._miVar1 + plr[id]._pMana - diff;
			plr[id]._pHPBase = missile[i]._miVar2 + plr[id]._pMana - diff;
			plr[id]._pMana = 0;
			plr[id]._pManaBase = plr[id]._pMaxManaBase - plr[id]._pMaxMana;
			missile[i]._mirange = 0;
			missile[i]._miDelFlag = 1;
			if(plr[id]._pHitPoints < 0) {
				SetPlayerHitPoints(id, 0);
			}
			if(plr[id]._pHitPoints >> 6 == 0 && id == myplr) {
				SyncPlrKill(id, missile[i]._miVar8);
			}
		}
	}

	if(id == myplr && plr[id]._pHitPoints == 0 && missile[i]._miVar1 == 0 && plr[id]._pmode != PM_DEATH) {
		missile[i]._mirange = 0;
		missile[i]._miDelFlag = 1;
		SyncPlrKill(id, -1);
	}

	missile[i]._miVar1 = plr[id]._pHitPoints;
	missile[i]._miVar2 = plr[id]._pHPBase;

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		NetSendCmd(TRUE, CMD_ENDSHIELD);
	}

	PutMissile(i);
}

void MI_Etherealize(int i)
{
	int id;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	id = missile[i]._misource;
	missile[i]._mix = plr[id].WorldX;
	missile[i]._miy = plr[id].WorldY;
	missile[i]._mitxoff = plr[id]._pxoff << 16;
	missile[i]._mityoff = plr[id]._pyoff << 16;

	if(plr[id]._pmode == PM_WALK3) {
		missile[i]._misx = plr[id]._px;
		missile[i]._misy = plr[id]._py;
	} else {
		missile[i]._misx = plr[id].WorldX;
		missile[i]._misy = plr[id].WorldY;
	}

	GetMissilePos(i);

	if(plr[id]._pmode == PM_WALK3) {
		if(plr[id]._pdir == 2) {
			missile[i]._mix++;
		} else {
			missile[i]._miy++;
		}
	}

	plr[id]._pSpellFlags |= 1;

	if(missile[i]._mirange == 0 || plr[id]._pHitPoints <= 0) {
		missile[i]._miDelFlag = 1;
		plr[id]._pSpellFlags &= ~1;
	}

	PutMissile(i);
}

void MI_Firemove(int i)
{
	int j;
	int ExpLight[14] = { 2, 3, 4, 5, 5, 6, 7, 8, 9, 10, 11, 12, 12 }; /// BUGFIX: array size should be '13'

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mix--;
	missile[i]._miy--;
	missile[i]._miyoff += 32;
	missile[i]._miVar1++;

	if(missile[i]._miVar1 == missile[i]._miAnimLen) {
		SetMissDir(i, 1);
		missile[i]._miAnimFrame = random(82, 11) + 1;
	}

	missile[i]._mitxoff += missile[i]._mixvel;
	missile[i]._mityoff += missile[i]._miyvel;
	GetMissilePos(i);
	j = missile[i]._mirange;
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, FALSE, missile[i]._mix, missile[i]._miy, FALSE);

	if(missile[i]._miHitFlag == 1) {
		missile[i]._mirange = j;
	}
	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		AddUnLight(missile[i]._mlid);
	}

	if(missile[i]._mimfnum == 0 && missile[i]._mirange != 0) {
		if(missile[i]._miVar2 == 0) {
			missile[i]._mlid = AddLight(missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar2]);
		}
		ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar2]);
		missile[i]._miVar2++;
	} else if(missile[i]._mix != missile[i]._miVar3 || missile[i]._miy != missile[i]._miVar4) {
		missile[i]._miVar3 = missile[i]._mix;
		missile[i]._miVar4 = missile[i]._miy;
		ChangeLight(missile[i]._mlid, missile[i]._miVar3, missile[i]._miVar4, 8);
	}

	missile[i]._mix++;
	missile[i]._miy++;
	missile[i]._miyoff -= 32;
	PutMissile(i);
}

void MI_Guardian(int i)
{
	int j, k, sx, sy, sx1, sy1, ex;

	sx1 = 0;
	sy1 = 0;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;

	if(missile[i]._miVar2 > 0) {
		missile[i]._miVar2--;
	}
	if(missile[i]._mirange == missile[i]._miVar1 || missile[i]._mimfnum == MFILE_GUARD && missile[i]._miVar2 == 0) {
		SetMissDir(i, 1);
	}

	if(!(missile[i]._mirange % 16)) {
		ex = 0;
		for(j = 0; j < 23 && ex != -1; j++) {
			for(k = 10; k >= 0 && ex != -1 && (vCrawlTable[j][k] != 0 || vCrawlTable[j][k + 1] != 0); k -= 2) {
				if(sx1 == vCrawlTable[j][k] && sy1 == vCrawlTable[j][k + 1]) {
					continue;
				}
				sx = missile[i]._mix + vCrawlTable[j][k];
				sy = missile[i]._miy + vCrawlTable[j][k + 1];
				ex = Sentfire(i, sx, sy);
				if(ex == -1) {
					break;
				}
				sx = missile[i]._mix - vCrawlTable[j][k];
				sy = missile[i]._miy - vCrawlTable[j][k + 1];
				ex = Sentfire(i, sx, sy);
				if(ex == -1) {
					break;
				}
				sx = missile[i]._mix + vCrawlTable[j][k];
				sy = missile[i]._miy - vCrawlTable[j][k + 1];
				ex = Sentfire(i, sx, sy);
				if(ex == -1) {
					break;
				}
				sx = missile[i]._mix - vCrawlTable[j][k];
				sy = missile[i]._miy + vCrawlTable[j][k + 1];
				ex = Sentfire(i, sx, sy);
				if(ex == -1) {
					break;
				}
				sx1 = vCrawlTable[j][k];
				sy1 = vCrawlTable[j][k + 1];
			}
		}
	}

	if(missile[i]._mirange == 14) {
		SetMissDir(i, 0);
		missile[i]._miAnimFrame = 15;
		missile[i]._miAnimAdd = -1;
	}

	missile[i]._miVar3 += missile[i]._miAnimAdd;

	if(missile[i]._miVar3 > 15) {
		missile[i]._miVar3 = 15;
	} else if(missile[i]._miVar3 > 0) {
		ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, missile[i]._miVar3);
	}

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		AddUnLight(missile[i]._mlid);
	}

	PutMissile(i);
}

void MI_Chain(int i)
{
	int sx, sy, id, l, n, m, k, rad, tx, ty, dir;
	int CrawlNum[19] = { 0, 3, 12, 45, 94, 159, 240, 337, 450, 579, 724, 885, 1062, 1255, 1464, 1689, 1930, 2187, 2460 };

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	id = missile[i]._misource;
	sx = missile[i]._mix;
	sy = missile[i]._miy;
	dir = GetDirection(sx, sy, missile[i]._miVar1, missile[i]._miVar2);
	AddMissile(sx, sy, missile[i]._miVar1, missile[i]._miVar2, dir, MIS_LIGHTCTRL, 0, id, 1, missile[i]._mispllvl);

	rad = missile[i]._mispllvl + 3;
	if(rad > 19) {
		rad = 19;
	}

	for(n = 1; n < rad; n++) {
		k = CrawlNum[n];
		l = k + 1;
		for(m = (BYTE)CrawlTable[k]; m > 0; m--) {
			tx = sx + CrawlTable[l];
			ty = sy + CrawlTable[l + 1];
			if(tx > 0 && tx < MAXDUNX && ty > 0 && ty < MAXDUNY && dMonster[tx][ty] > 0) {
				dir = GetDirection(sx, sy, tx, ty);
				AddMissile(sx, sy, tx, ty, dir, MIS_LIGHTCTRL, 0, id, 1, missile[i]._mispllvl);
			}
			l += 2;
		}
	}

	missile[i]._mirange--;

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
	}
}

void mi_null_11(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
	}
	if(missile[i]._miAnimFrame == missile[i]._miAnimLen) {
		missile[i]._miPreFlag = 1;
	}

	PutMissile(i);
}

void MI_Weapexp(int i)
{
	int id, mind, maxd;
	int ExpLight[10] = { 9, 10, 11, 12, 11, 10, 8, 6, 4, 2 };

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	id = missile[i]._misource;

	if(missile[i]._miVar2 == 1) {
		mind = plr[id]._pIFMinDam;
		maxd = plr[id]._pIFMaxDam;
		missiledata[missile[i]._mitype].mResist = 1;
	} else {
		mind = plr[id]._pILMinDam;
		maxd = plr[id]._pILMaxDam;
		missiledata[missile[i]._mitype].mResist = 2;
	}

	CheckMissileCol(i, mind, maxd, FALSE, missile[i]._mix, missile[i]._miy, FALSE);

	if(missile[i]._miVar1 == 0) {
		missile[i]._mlid = AddLight(missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar1]);
	} else if(missile[i]._mirange != 0) {
		ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar1]);
	}

	missile[i]._miVar1++;

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		AddUnLight(missile[i]._mlid);
	} else {
		PutMissile(i);
	}
}

void MI_Misexp(int i)
{
	int ExpLight[10] = { 9, 10, 11, 12, 11, 10, 8, 6, 4, 2 };

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		AddUnLight(missile[i]._mlid);
	} else {
		if(missile[i]._miVar1 == 0) {
			missile[i]._mlid = AddLight(missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar1]);
		} else {
			ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar1]);
		}
		missile[i]._miVar1++;
		PutMissile(i);
	}
}

void MI_Acidsplat(int i)
{
	int monst, dam;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	if(missile[i]._mirange == missile[i]._miAnimLen) {
		missile[i]._mix++;
		missile[i]._miy++;
		missile[i]._miyoff -= 32;
	}

	missile[i]._mirange--;

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		monst = missile[i]._misource;
		dam = (monster[monst].MData->mLevel >= 2) + 1;
		AddMissile(missile[i]._mix, missile[i]._miy, i, 0, missile[i]._mimfnum, MIS_ACIDPUD, 1, missile[i]._misource, dam, missile[i]._mispllvl);
	} else {
		PutMissile(i);
	}
}

void MI_Teleport(int i)
{
	int id;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	id = missile[i]._misource;
	missile[i]._mirange--;

	if(missile[i]._mirange <= 0) {
		missile[i]._miDelFlag = 1;
	} else {
		dPlayer[plr[id].WorldX][plr[id].WorldY] = 0;
		PlrClrTrans(plr[id].WorldX, plr[id].WorldY);
		plr[id].WorldX = missile[i]._mix;
		plr[id].WorldY = missile[i]._miy;
		plr[id]._px = plr[id].WorldX;
		plr[id]._py = plr[id].WorldY;
		plr[id]._poldx = plr[id].WorldX;
		plr[id]._poldy = plr[id].WorldY;
		PlrDoTrans(plr[id].WorldX, plr[id].WorldY);
		missile[i]._miVar1 = 1;
		dPlayer[plr[id].WorldX][plr[id].WorldY] = id + 1;
		if(leveltype != DTYPE_TOWN) {
			ChangeLightXY(plr[id]._plid, plr[id].WorldX, plr[id].WorldY);
			ChangeVisionXY(plr[id]._pvid, plr[id].WorldX, plr[id].WorldY);
		}
		if(id == myplr) {
			ViewX = plr[id].WorldX - ScrollInfo._sdx;
			ViewY = plr[id].WorldY - ScrollInfo._sdy;
		}
	}
}

void MI_Stone(int i)
{
	int m;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	m = missile[i]._miVar2;

	if(monster[m]._mhitpoints == 0 && missile[i]._miAnimType != MFILE_SHATTER1) {
		missile[i]._mimfnum = 0;
		missile[i]._miDrawFlag = 1;
		SetMissAnim(i, MFILE_SHATTER1);
		missile[i]._mirange = 11;
	}

	if(monster[m]._mmode != MM_STONE) {
		missile[i]._miDelFlag = 1;
		return;
	}

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		if(monster[m]._mhitpoints > 0) {
			/// ASSERT: assert(dMonster[monster[m]._mx][monster[m]._my] == m+1 || dMonster[monster[m]._mx][monster[m]._my] == -(m+1));
			monster[m]._mmode = missile[i]._miVar1;
		} else {
			AddDead(monster[m]._mx, monster[m]._my, stonendx, monster[m]._mdir);
		}
	}
	if(missile[i]._miAnimType == MFILE_SHATTER1) {
		PutMissile(i);
	}
}

void MI_Boom(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;

	if(missile[i]._miVar1 == 0) {
		CheckMissileCol(i, missile[i]._midam, missile[i]._midam, FALSE, missile[i]._mix, missile[i]._miy, TRUE);
	}
	if(missile[i]._miHitFlag == 1) {
		missile[i]._miVar1 = 1;
	}
	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
	}

	PutMissile(i);
}

void MI_Rhino(int i)
{
	int mix, miy, mix2, miy2, omx, omy, monst;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	monst = missile[i]._misource;
	/// ASSERT: assert((DWORD)monst < MAXMONSTERS);

	if(monster[monst]._mmode != MM_CHARGE) {
		missile[i]._miDelFlag = 1;
		return;
	}

	GetMissilePos(i);
	omx = missile[i]._mix;
	omy = missile[i]._miy;
	dMonster[omx][omy] = 0;

	if(monster[monst]._mAi == AI_SNAKE) {
		missile[i]._mitxoff += 2 * missile[i]._mixvel;
		missile[i]._mityoff += 2 * missile[i]._miyvel;
		GetMissilePos(i);
		mix2 = missile[i]._mix;
		miy2 = missile[i]._miy;
		missile[i]._mitxoff -= missile[i]._mixvel;
		missile[i]._mityoff -= missile[i]._miyvel;
	} else {
		missile[i]._mitxoff += missile[i]._mixvel;
		missile[i]._mityoff += missile[i]._miyvel;
	}

	GetMissilePos(i);
	mix = missile[i]._mix;
	miy = missile[i]._miy;

	if(!PosOkMonst(monst, mix, miy) || monster[monst]._mAi == AI_SNAKE && !PosOkMonst(monst, mix2, miy2)) {
		MissToMonst(i, omx, omy);
		missile[i]._miDelFlag = 1;
		return;
	}

	dMonster[mix][miy] = -(monst + 1);
	monster[monst]._mfutx = mix;
	monster[monst]._moldx = mix;
	monster[monst]._mx = mix;
	monster[monst]._mfuty = miy;
	monster[monst]._moldy = miy;
	monster[monst]._my = miy;

	if(monster[monst]._uniqtype != 0) {
		ChangeLightXY(missile[i]._mlid, mix, miy);
	}

	MoveMissilePos(i);
	PutMissile(i);
}

void mi_null_32(int i)
{
	int mix, miy, omx, omy, monst, p, px, py, mid;

	GetMissilePos(i);
	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	omx = missile[i]._mix;
	omy = missile[i]._miy;
	missile[i]._mitxoff += missile[i]._mixvel;
	missile[i]._mityoff += missile[i]._miyvel;

	GetMissilePos(i);
	monst = missile[i]._misource;
	mix = missile[i]._mix;
	miy = missile[i]._miy;
	p = monster[monst]._menemy;

	if(!(monster[monst]._mFlags & 0x10)) {
		px = plr[p].WorldX;
		py = plr[p].WorldY;
	} else {
		px = monster[p]._mx;
		py = monster[p]._my;
	}

	if((mix != omx || miy != omy)
	&& (missile[i]._miVar1 & 1 && (abs(omx - px) >= 4 || abs(omy - py) >= 4) || missile[i]._miVar2 > 1)
	&& PosOkMonst(missile[i]._misource, omx, omy)) {
		MissToMonst(i, omx, omy);
		missile[i]._miDelFlag = 1;
	} else {
		if(!(monster[monst]._mFlags & 0x10)) {
			mid = dPlayer[mix][miy];
		} else {
			mid = dMonster[mix][miy];
		}
	}

	if(!PosOkMissile(mix, miy) || mid > 0 && !(missile[i]._miVar1 & 1)) {
		missile[i]._mixvel = -missile[i]._mixvel;
		missile[i]._miyvel = -missile[i]._miyvel;
		missile[i]._mimfnum = opposite[missile[i]._mimfnum];
		missile[i]._miAnimData = monster[monst].MType->Anims[1].Frames[missile[i]._mimfnum];
		missile[i]._miVar2++;
		if(mid > 0) {
			missile[i]._miVar1 |= 1;
		}
	}

	MoveMissilePos(i);
	PutMissile(i);
}

void MI_FirewallC(int i)
{
	int tx, ty, pn, id;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	id = missile[i]._misource;

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
	} else {
		pn = dPiece[missile[i]._miVar1][missile[i]._miVar2];
		/// ASSERT: assert((DWORD)pn <= MAXTILES);
		tx = missile[i]._miVar1 + XDirAdd[missile[i]._miVar3];
		ty = missile[i]._miVar2 + YDirAdd[missile[i]._miVar3];
		if(!nMissileTable[pn] && missile[i]._miVar8 == 0 && tx > 0 && tx < MAXDUNX && ty > 0 && ty < MAXDUNY) {
			AddMissile(
				missile[i]._miVar1,
				missile[i]._miVar2,
				missile[i]._miVar1,
				missile[i]._miVar2,
				plr[id]._pdir,
				MIS_FIREWALL,
				0,
				id,
				0,
				missile[i]._mispllvl);
			missile[i]._miVar1 = tx;
			missile[i]._miVar2 = ty;
		} else {
			missile[i]._miVar8 = 1;
		}
		pn = dPiece[missile[i]._miVar5][missile[i]._miVar6];
		/// ASSERT: assert((DWORD)pn <= MAXTILES);
		tx = missile[i]._miVar5 + XDirAdd[missile[i]._miVar4];
		ty = missile[i]._miVar6 + YDirAdd[missile[i]._miVar4];
		if(!nMissileTable[pn] && missile[i]._miVar7 == 0 && tx > 0 && tx < MAXDUNX && ty > 0 && ty < MAXDUNY) {
			AddMissile(
				missile[i]._miVar5,
				missile[i]._miVar6,
				missile[i]._miVar5,
				missile[i]._miVar6,
				plr[id]._pdir,
				MIS_FIREWALL,
				0,
				id,
				0,
				missile[i]._mispllvl);
			missile[i]._miVar5 = tx;
			missile[i]._miVar6 = ty;
		} else {
			missile[i]._miVar7 = 1;
		}
	}
}

void MI_Infra(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	plr[missile[i]._misource]._pInfraFlag = 1;

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		CalcPlrItemVals(missile[i]._misource, TRUE);
	}
}

void MI_Apoca(int i)
{
	int j, k, id;
	BOOL exit;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	id = missile[i]._misource;

	exit = FALSE;
	for(k = missile[i]._miVar2; k < missile[i]._miVar3 && !exit; k++) {
		for(j = missile[i]._miVar4; j < missile[i]._miVar5 && !exit; j++) {
			if(dMonster[j][k] > 3 && !nSolidTable[dPiece[j][k]]) {
				AddMissile(j, k, j, k, plr[id]._pdir, MIS_BOOM, 0, id, missile[i]._midam, 0);
				exit = TRUE;
			}
		}
		if(!exit) {
			missile[i]._miVar4 = missile[i]._miVar6;
		}
	}

	if(exit == TRUE) {
		missile[i]._miVar2 = k - 1;
		missile[i]._miVar4 = j;
	} else {
		missile[i]._miDelFlag = 1;
	}
}

void MI_Wave(int i)
{
	int dira, dirb, nxa, nya, nxb, nyb, pn, j, sd, id, sx, sy, v1, v2;
	BOOL f1, f2;

	f1 = FALSE;
	f2 = FALSE;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	id = missile[i]._misource;
	sx = missile[i]._mix;
	sy = missile[i]._miy;
	v1 = missile[i]._miVar1;
	v2 = missile[i]._miVar2;
	sd = GetDirection(sx, sy, v1, v2);
	dira = (sd - 2) & 7;
	dirb = (sd + 2) & 7;
	nxa = sx + XDirAdd[sd];
	nya = sy + YDirAdd[sd];
	pn = dPiece[nxa][nya];
	/// ASSERT: assert((DWORD)pn <= MAXTILES);

	if(!nMissileTable[pn]) {
		AddMissile(nxa, nya, nxa + XDirAdd[sd], nya + YDirAdd[sd], plr[id]._pdir, MIS_FIREMOVE, 0, id, 0, missile[i]._mispllvl);
		nxa += XDirAdd[dira];
		nya += YDirAdd[dira];
		nxb = sx + XDirAdd[sd] + XDirAdd[dirb];
		nyb = sy + YDirAdd[sd] + YDirAdd[dirb];
		for(j = 0; j < (missile[i]._mispllvl >> 1) + 2; j++) {
			pn = dPiece[nxa][nya];
			/// ASSERT: assert((DWORD)pn <= MAXTILES);
			if(!nMissileTable[pn] && !f1 && nxa > 0 && nxa < MAXDUNX && nya > 0 && nya < MAXDUNY) {
				AddMissile(nxa, nya, nxa + XDirAdd[sd], nya + YDirAdd[sd], plr[id]._pdir, MIS_FIREMOVE, 0, id, 0, missile[i]._mispllvl);
				nxa += XDirAdd[dira];
				nya += YDirAdd[dira];
			} else {
				f1 = TRUE;
			}
			pn = dPiece[nxb][nyb];
			/// ASSERT: assert((DWORD)pn <= MAXTILES);
			if(!nMissileTable[pn] && !f2 && nxb > 0 && nxb < MAXDUNX && nyb > 0 && nyb < MAXDUNY) {
				AddMissile(nxb, nyb, nxb + XDirAdd[sd], nyb + YDirAdd[sd], plr[id]._pdir, MIS_FIREMOVE, 0, id, 0, missile[i]._mispllvl);
				nxb += XDirAdd[dirb];
				nyb += YDirAdd[dirb];
			} else {
				f2 = TRUE;
			}
		}
	}

	missile[i]._mirange--;

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
	}
}

void MI_Nova(int i)
{
	int k, id, sx, sy, dir, en, sx1, sy1, dam, v1, v2;

	sx1 = sy1 = 0;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	id = missile[i]._misource;
	dam = missile[i]._midam;
	sx = missile[i]._mix;
	sy = missile[i]._miy;
	v1 = missile[i]._miVar1;
	v2 = missile[i]._miVar2;

	if(id != -1) {
		dir = plr[id]._pdir;
		en = 0;
	} else {
		dir = 0;
		en = 1;
	}

	for(k = 0; k < 23; k++) {
		if(sx1 == vCrawlTable[k][6] && sy1 == vCrawlTable[k][7]) {
			continue;
		}
		v1 = sx + vCrawlTable[k][6];
		v2 = sy + vCrawlTable[k][7];
		AddMissile(sx, sy, v1, v2, dir, MIS_LIGHTBALL, en, id, dam, missile[i]._mispllvl);
		v1 = sx - vCrawlTable[k][6];
		v2 = sy - vCrawlTable[k][7];
		AddMissile(sx, sy, v1, v2, dir, MIS_LIGHTBALL, en, id, dam, missile[i]._mispllvl);
		v1 = sx - vCrawlTable[k][6];
		v2 = sy + vCrawlTable[k][7];
		AddMissile(sx, sy, v1, v2, dir, MIS_LIGHTBALL, en, id, dam, missile[i]._mispllvl);
		v1 = sx + vCrawlTable[k][6];
		v2 = sy - vCrawlTable[k][7];
		AddMissile(sx, sy, v1, v2, dir, MIS_LIGHTBALL, en, id, dam, missile[i]._mispllvl);
		sx1 = vCrawlTable[k][6];
		sy1 = vCrawlTable[k][7];
	}

	missile[i]._mirange--;

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
	}
}

void MI_Blodboil(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._miDelFlag = 1;
}

void MI_Flame(int i)
{
	int k;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	missile[i]._miVar2--;
	k = missile[i]._mirange;
	CheckMissileCol(i, missile[i]._midam, missile[i]._midam, TRUE, missile[i]._mix, missile[i]._miy, FALSE);

	if(missile[i]._mirange == 0 && missile[i]._miHitFlag == 1) {
		missile[i]._mirange = k;
	}
	if(missile[i]._miVar2 == 0) {
		missile[i]._miAnimFrame = 20;
	}
	if(missile[i]._miVar2 <= 0) {
		k = missile[i]._miAnimFrame;
		if(k > 11) {
			k = 24 - k;
		}
		ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, k);
	}
	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		AddUnLight(missile[i]._mlid);
	}
	if(missile[i]._miVar2 <= 0) {
		PutMissile(i);
	}
}

void MI_Flamec(int i)
{
	int id, pn;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	id = missile[i]._misource;
	missile[i]._mitxoff += missile[i]._mixvel;
	missile[i]._mityoff += missile[i]._miyvel;
	GetMissilePos(i);

	if(missile[i]._mix != missile[i]._miVar1 || missile[i]._miy != missile[i]._miVar2) {
		/// ASSERT: assert((DWORD)missile[i]._mix < MAXDUNX);
		/// ASSERT: assert((DWORD)missile[i]._miy < MAXDUNY);
		pn = dPiece[missile[i]._mix][missile[i]._miy];
		/// ASSERT: assert((DWORD)pn <= MAXTILES);
		if(!nMissileTable[pn]) {
			AddMissile(
				missile[i]._mix,
				missile[i]._miy,
				missile[i]._misx,
				missile[i]._misy,
				i,
				MIS_FLAME,
				missile[i]._micaster,
				id,
				missile[i]._miVar3,
				missile[i]._mispllvl);
		} else {
			missile[i]._mirange = 0;
		}
		missile[i]._miVar1 = missile[i]._mix;
		missile[i]._miVar2 = missile[i]._miy;
		missile[i]._miVar3++;
	}
	if(missile[i]._mirange == 0 || missile[i]._miVar3 == 3) {
		missile[i]._miDelFlag = 1;
	}
}

void MI_Cbolt(int i)
{
	int sx, sy, dx, dy, md;
	int bpath[16] = { -1, 0, 1, -1, 0, 1, -1, -1, 0, 0, 1, 1, 0, 1, -1, 0 };

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;

	if(missile[i]._miAnimType != MFILE_LGHNING) {
		if(missile[i]._miVar3 == 0) {
			md = (missile[i]._miVar2 + bpath[missile[i]._mirnd]) & 7;
			missile[i]._mirnd = (missile[i]._mirnd + 1) & 0xF;
			sx = missile[i]._mix;
			sy = missile[i]._miy;
			dx = sx + XDirAdd[md];
			dy = sy + YDirAdd[md];
			GetMissileVel(i, sx, sy, dx, dy, 8);
			missile[i]._miVar3 = 16;
		} else {
			missile[i]._miVar3--;
		}
		missile[i]._mitxoff += missile[i]._mixvel;
		missile[i]._mityoff += missile[i]._miyvel;
		GetMissilePos(i);
		CheckMissileCol(i, missile[i]._midam, missile[i]._midam, FALSE, missile[i]._mix, missile[i]._miy, FALSE);
		if(missile[i]._miHitFlag == 1) {
			missile[i]._miVar1 = 8;
			missile[i]._mimfnum = 0;
			missile[i]._mixoff = 0;
			missile[i]._miyoff = 0;
			SetMissAnim(i, MFILE_LGHNING);
			missile[i]._mirange = missile[i]._miAnimLen;
			GetMissilePos(i);
		}
		ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, missile[i]._miVar1);
	}
	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		AddUnLight(missile[i]._mlid);
	}

	PutMissile(i);
}

void MI_Hbolt(int i)
{
	int dam;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;

	if(missile[i]._miAnimType != MFILE_HOLYEXPL) {
		missile[i]._mitxoff += missile[i]._mixvel;
		missile[i]._mityoff += missile[i]._miyvel;
		GetMissilePos(i);
		dam = missile[i]._midam;
		if(missile[i]._mix != missile[i]._misx || missile[i]._miy != missile[i]._misy) {
			CheckMissileCol(i, dam, dam, FALSE, missile[i]._mix, missile[i]._miy, FALSE);
		}
		if(missile[i]._mirange == 0) {
			missile[i]._mitxoff -= missile[i]._mixvel;
			missile[i]._mityoff -= missile[i]._miyvel;
			GetMissilePos(i);
			missile[i]._mimfnum = 0;
			SetMissAnim(i, MFILE_HOLYEXPL);
			missile[i]._mirange = missile[i]._miAnimLen - 1;
		} else if(missile[i]._mix != missile[i]._miVar1 || missile[i]._miy != missile[i]._miVar2) {
			missile[i]._miVar1 = missile[i]._mix;
			missile[i]._miVar2 = missile[i]._miy;
			ChangeLight(missile[i]._mlid, missile[i]._miVar1, missile[i]._miVar2, 8);
		}
	} else {
		ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, missile[i]._miAnimFrame + 7);
		if(missile[i]._mirange == 0) {
			missile[i]._miDelFlag = 1;
			AddUnLight(missile[i]._mlid);
		}
	}

	PutMissile(i);
}

void MI_Element(int i)
{
	int mid, sd, dam, cx, cy, px, py, id;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	id = missile[i]._misource;
	dam = missile[i]._midam;

	if(missile[i]._miAnimType == MFILE_BIGEXP) {
		cx = missile[i]._mix;
		cy = missile[i]._miy;
		px = plr[id].WorldX;
		py = plr[id].WorldY;
		ChangeLight(missile[i]._mlid, cx, cy, missile[i]._miAnimFrame);
		if(!CheckBlock(px, py, cx, cy)) {
			CheckMissileCol(i, dam, dam, TRUE, cx, cy, TRUE);
		}
		if(!CheckBlock(px, py, cx, cy + 1)) {
			CheckMissileCol(i, dam, dam, TRUE, cx, cy + 1, TRUE);
		}
		if(!CheckBlock(px, py, cx, cy - 1)) {
			CheckMissileCol(i, dam, dam, TRUE, cx, cy - 1, TRUE);
		}
		if(!CheckBlock(px, py, cx + 1, cy)) {
			CheckMissileCol(i, dam, dam, TRUE, cx + 1, cy, TRUE);
		}
		if(!CheckBlock(px, py, cx + 1, cy - 1)) {
			CheckMissileCol(i, dam, dam, TRUE, cx + 1, cy - 1, TRUE);
		}
		if(!CheckBlock(px, py, cx + 1, cy + 1)) {
			CheckMissileCol(i, dam, dam, TRUE, cx + 1, cy + 1, TRUE);
		}
		if(!CheckBlock(px, py, cx - 1, cy)) {
			CheckMissileCol(i, dam, dam, TRUE, cx - 1, cy, TRUE);
		}
		if(!CheckBlock(px, py, cx - 1, cy + 1)) {
			CheckMissileCol(i, dam, dam, TRUE, cx - 1, cy + 1, TRUE);
		}
		if(!CheckBlock(px, py, cx - 1, cy - 1)) {
			CheckMissileCol(i, dam, dam, TRUE, cx - 1, cy - 1, TRUE);
		}
		if(missile[i]._mirange == 0) {
			missile[i]._miDelFlag = 1;
			AddUnLight(missile[i]._mlid);
		}
	} else {
		missile[i]._mitxoff += missile[i]._mixvel;
		missile[i]._mityoff += missile[i]._miyvel;
		GetMissilePos(i);
		cx = missile[i]._mix;
		cy = missile[i]._miy;
		CheckMissileCol(i, dam, dam, FALSE, cx, cy, FALSE);
		if(missile[i]._miVar3 == 0 && cx == missile[i]._miVar4 && cy == missile[i]._miVar5) {
			missile[i]._miVar3 = 1;
		}
		if(missile[i]._miVar3 == 1) {
			missile[i]._miVar3 = 2;
			missile[i]._mirange = 255;
			mid = FindClosest(cx, cy, 19);
			if(mid > 0) {
				sd = GetDirection8(cx, cy, monster[mid]._mx, monster[mid]._my);
				SetMissDir(i, sd);
				GetMissileVel(i, cx, cy, monster[mid]._mx, monster[mid]._my, 16);
			} else {
				sd = plr[id]._pdir;
				SetMissDir(i, sd);
				GetMissileVel(i, cx, cy, cx + XDirAdd[sd], cy + YDirAdd[sd], 16);
			}
		}
		if(cx != missile[i]._miVar1 || cy != missile[i]._miVar2) {
			missile[i]._miVar1 = cx;
			missile[i]._miVar2 = cy;
			ChangeLight(missile[i]._mlid, missile[i]._miVar1, missile[i]._miVar2, 8);
		}
		if(missile[i]._mirange == 0) {
			missile[i]._mimfnum = 0;
			SetMissAnim(i, MFILE_BIGEXP);
			missile[i]._mirange = missile[i]._miAnimLen - 1;
		}
	}

	PutMissile(i);
}

void MI_Bonespirit(int i)
{
	int mid, sd, dam, cx, cy, id;

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;
	dam = missile[i]._midam;
	id = missile[i]._misource;

	if(missile[i]._mimfnum == 8) {
		ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, missile[i]._miAnimFrame);
		if(missile[i]._mirange == 0) {
			missile[i]._miDelFlag = 1;
			AddUnLight(missile[i]._mlid);
		}
		PutMissile(i);
		return;
	}

	missile[i]._mitxoff += missile[i]._mixvel;
	missile[i]._mityoff += missile[i]._miyvel;
	GetMissilePos(i);
	cx = missile[i]._mix;
	cy = missile[i]._miy;
	CheckMissileCol(i, dam, dam, FALSE, cx, cy, FALSE);

	if(missile[i]._miVar3 == 0 && cx == missile[i]._miVar4 && cy == missile[i]._miVar5) {
		missile[i]._miVar3 = 1;
	}
	if(missile[i]._miVar3 == 1) {
		missile[i]._miVar3 = 2;
		missile[i]._mirange = 255;
		mid = FindClosest(cx, cy, 19);
		if(mid > 0) {
			missile[i]._midam = monster[mid]._mhitpoints >> 7;
			sd = GetDirection8(cx, cy, monster[mid]._mx, monster[mid]._my);
			SetMissDir(i, sd);
			GetMissileVel(i, cx, cy, monster[mid]._mx, monster[mid]._my, 16);
		} else {
			sd = plr[id]._pdir;
			SetMissDir(i, sd);
			GetMissileVel(i, cx, cy, cx + XDirAdd[sd], cy + YDirAdd[sd], 16);
		}
	}
	if(cx != missile[i]._miVar1 || cy != missile[i]._miVar2) {
		missile[i]._miVar1 = cx;
		missile[i]._miVar2 = cy;
		ChangeLight(missile[i]._mlid, missile[i]._miVar1, missile[i]._miVar2, 8);
	}
	if(missile[i]._mirange == 0) {
		SetMissDir(i, 8);
		missile[i]._mirange = 7;
	}

	PutMissile(i);
}

void MI_ResurrectBeam(int i)
{
	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	missile[i]._mirange--;

	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
	}

	PutMissile(i);
}

void MI_Rportal(int i)
{
	int ExpLight[17] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 15 };

	/// ASSERT: assert((DWORD)i < MAXMISSILES);
	if(missile[i]._mirange > 1) {
		missile[i]._mirange--;
	}
	if(missile[i]._mirange == missile[i]._miVar1) {
		SetMissDir(i, 1);
	}
	if(currlevel != 0 && missile[i]._mimfnum != 1 && missile[i]._mirange != 0) {
		if(missile[i]._miVar2 == 0) {
			missile[i]._mlid = AddLight(missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar2]);
		}
		ChangeLight(missile[i]._mlid, missile[i]._mix, missile[i]._miy, ExpLight[missile[i]._miVar2]);
		missile[i]._miVar2++;
	}
	if(missile[i]._mirange == 0) {
		missile[i]._miDelFlag = 1;
		AddUnLight(missile[i]._mlid);
	}

	PutMissile(i);
}

void ProcessMissiles()
{
	int i, mi;

	for(i = 0; i < nummissiles; i++) {
		mi = missileactive[i];
		dFlags[missile[mi]._mix][missile[mi]._miy] &= ~1;
		dMissile[missile[mi]._mix][missile[mi]._miy] = 0;
	}

	i = 0;
	while(i < nummissiles) {
		mi = missileactive[i];
		if(missile[mi]._miDelFlag) {
			DeleteMissile(mi, i);
			i = 0;
		} else {
			i++;
		}
	}

	MissilePreFlag = 0;
	ManashieldFlag = 0;

	for(i = 0; i < nummissiles; i++) {
		mi = missileactive[i];
		missiledata[missile[mi]._mitype].mProc(mi);
		if(!(missile[mi]._miAnimFlags & 2)) {
			missile[mi]._miAnimCnt++;
			if(missile[mi]._miAnimCnt >= missile[mi]._miAnimDelay) {
				missile[mi]._miAnimCnt = 0;
				missile[mi]._miAnimFrame += missile[mi]._miAnimAdd;
				if(missile[mi]._miAnimFrame > missile[mi]._miAnimLen) {
					missile[mi]._miAnimFrame = 1;
				}
				if(missile[mi]._miAnimFrame < 1) {
					missile[mi]._miAnimFrame = missile[mi]._miAnimLen;
				}
			}
		}
	}

	if(ManashieldFlag) {
		for(i = 0; i < nummissiles; i++) {
			mi = missileactive[i];
			if(missile[mi]._mitype == MIS_MANASHIELD) {
				MI_Manashield(mi);
			}
		}
	}

	i = 0;
	while(i < nummissiles) {
		mi = missileactive[i];
		if(missile[mi]._miDelFlag) {
			DeleteMissile(mi, i);
			i = 0;
		} else {
			i++;
		}
	}
}

void missiles_process_charge()
{
	int i, mi;
	MissileStruct *miss;
	CMonster *monst;
	AnimStruct *anim;

	for(i = 0; i < nummissiles; i++) {
		/// ASSERT: assert((DWORD)i < MAXMISSILES);
		mi = missileactive[i];
		/// ASSERT: assert((DWORD)mi < MAXMISSILES);
		miss = &missile[mi];
		miss->_miAnimData = misfiledata[miss->_miAnimType].mAnimData[miss->_mimfnum];
		if(miss->_mitype == MIS_RHINO) {
			monst = monster[miss->_misource].MType;
			if(monst->mtype >= MT_HORNED && monst->mtype <= MT_OBLORD) {
				anim = &monst->Anims[5];
			} else if(monst->mtype >= MT_NSNAKE && monst->mtype <= MT_GSNAKE) {
				anim = &monst->Anims[2];
			} else {
				anim = &monst->Anims[1];
			}
			missile[mi]._miAnimData = anim->Frames[miss->_mimfnum];
		}
	}
}

void ClearMissileSpot(int mi)
{
	/// ASSERT: assert((DWORD)mi < MAXMISSILES);
	dFlags[missile[mi]._mix][missile[mi]._miy] &= ~1;
	dMissile[missile[mi]._mix][missile[mi]._miy] = 0;
}
