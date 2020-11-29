#include "diablo.h"

int GetManaAmount(int id, int sn)
{
	int i, sl, ma, adj;

	adj = 0;
	sl = plr[id]._pSplLvl[sn] + plr[id]._pISplLvlAdd - 1;
	if(sl < 0) {
		sl = 0;
	}

	for(i = sl; i > 0; i--) {
		adj += spelldata[sn].sManaAdj;
	}

	if(sn == SPL_FIREBOLT) {
		adj >>= 1;
	}
	if(sn == SPL_RESURRECT && sl > 0) {
		adj = 0;
		for(i = sl; i > 0; i--) {
			adj += spelldata[SPL_RESURRECT].sManaCost >> 3;
		}
	}

	if(spelldata[sn].sManaCost == 255) {
		ma = ((BYTE)plr[id]._pMaxManaBase - adj) << 6;
	} else {
		ma = (spelldata[sn].sManaCost - adj) << 6;
	}

	if(sn == SPL_HEAL) {
		ma = (spelldata[SPL_HEAL].sManaCost + 2 * plr[id]._pLevel - adj) << 6;
	}
	if(sn == SPL_HEALOTHER) {
		ma = (spelldata[SPL_HEAL].sManaCost + 2 * plr[id]._pLevel - adj) << 6;
	}
	if(sn == SPL_RESURRECT) { /// BUGFIX: optimized away, 'adj' has no purpose now
		adj = 0;
		for(i = sl; i > 0; i--) {
			adj += spelldata[SPL_RESURRECT].sManaCost;
		}
	}

	if(plr[id]._pClass == PC_ROGUE) {
		ma -= ma >> 2;
	}
	if(spelldata[sn].sMinMana > ma >> 6) {
		ma = spelldata[sn].sMinMana << 6;
	}

	ma = ma * (100 - plr[id]._pISplCost) / 100;
	return ma;
}

void UseMana(int id, int sn)
{
	int ma;

	if(id != myplr) {
		return;
	}

	switch(plr[id]._pSplType) {
	case RSPLTYPE_SKILL:
	case RSPLTYPE_INVALID:
		break;
	case RSPLTYPE_SCROLL:
		RemoveScroll(id);
		break;
	case RSPLTYPE_CHARGES:
		UseStaffCharge(id);
		break;
	case RSPLTYPE_SPELL:
#ifdef _DEBUG
		if(debug_mode_key_inverted_v) {
			break;
		}
#endif
		ma = GetManaAmount(id, sn);
		plr[id]._pMana -= ma;
		plr[id]._pManaBase -= ma;
		drawmanaflag = 1;
		break;
	}
}

BOOL CheckSpell(int id, int sn, char st, BOOL manaonly)
{
	int ma;

#ifdef _DEBUG
	if(debug_mode_key_inverted_v) {
		return TRUE;
	}
#endif
	if(!manaonly && pcurs != CURSOR_HAND) {
		return FALSE;
	}
	if(st == RSPLTYPE_SKILL) {
		return TRUE;
	}
	if(GetSpellLevel(id, sn) <= 0) {
		return FALSE;
	}
	ma = GetManaAmount(id, sn);
	if(plr[id]._pMana < ma) {
		return FALSE;
	}

	return TRUE;
}

void CastSpell(int id, int spl, int sx, int sy, int dx, int dy, int caster, int spllvl)
{
	int i, dir;

	switch(caster) {
	case 0:
		dir = plr[id]._pdir;
		caster = 0;
		if(spl == SPL_FIREWALL) {
			dir = plr[id]._pVar3;
		}
		break;
	case 1:
		dir = monster[id]._mdir;
		break;
	}

	for(i = 0; spelldata[spl].sMissiles[i] != 0 && i < 3; i++) {
		AddMissile(sx, sy, dx, dy, dir, spelldata[spl].sMissiles[i], caster, id, 0, spllvl);
	}

	if(spelldata[spl].sMissiles[0] == MIS_TOWN) {
		UseMana(id, SPL_TOWN);
	}
	if(spelldata[spl].sMissiles[0] == MIS_CBOLT) {
		UseMana(id, SPL_CBOLT);
		for(i = (spllvl >> 1) + 3; i > 0; i--) {
			AddMissile(sx, sy, dx, dy, dir, MIS_CBOLT, caster, id, 0, spllvl);
		}
	}
}

static void PlacePlayer(int pnum)
{
	int i, j, l, xx, yy;
	BOOL done;

	if(plr[pnum].plrlevel != currlevel) {
		return;
	}

	for(i = 0; (DWORD)i < 8; i++) {
		xx = plr[pnum].WorldX + plrxoff2[i];
		yy = plr[pnum].WorldY + plryoff2[i];
		if(PosOkPlayer(pnum, xx, yy)) {
			break;
		}
	}

	if(!PosOkPlayer(pnum, xx, yy)) {
		done = FALSE;
		for(l = 1; l < 50 && !done; l++) {
			for(j = -l; j <= l && !done; j++) {
				yy = j + plr[pnum].WorldY;
				for(i = -l; i <= l && !done; i++) {
					xx = i + plr[pnum].WorldX;
					if(PosOkPlayer(pnum, xx, yy)) {
						done = TRUE;
					}
				}
			}
		}
	}

	plr[pnum].WorldX = xx;
	plr[pnum].WorldY = yy;
	dPlayer[xx][yy] = pnum + 1;

	if(pnum == myplr) {
		ViewX = xx;
		ViewY = yy;
	}
}

void DoResurrect(int pnum, int rid)
{
	int hp;

	if((BYTE)rid != 255) {
		AddMissile(plr[rid].WorldX, plr[rid].WorldY, plr[rid].WorldX, plr[rid].WorldY, 0, MIS_RESURRECTBEAM, 0, pnum, 0, 0);
	}
	if(pnum == myplr) {
		SetCursor_(CURSOR_HAND);
	}
	if((BYTE)rid != 255) {
		if(plr[rid]._pHitPoints != 0) {
			return;
		}
		if(rid == myplr) {
			deathflag = 0;
			gamemenu_off();
			drawhpflag = 1;
			drawmanaflag = 1;
		}
		ClrPlrPath(rid);
		plr[rid].destAction = -1;
		plr[rid]._pInvincible = 0;
		PlacePlayer(rid);
		hp = 640;
		if(plr[rid]._pMaxHPBase < hp) {
			hp = plr[rid]._pMaxHPBase;
		}
		SetPlayerHitPoints(rid, hp);
		plr[rid]._pHPBase = plr[rid]._pHitPoints - (plr[rid]._pMaxHP - plr[rid]._pMaxHPBase);
		plr[rid]._pMana = 0;
		plr[rid]._pManaBase = plr[rid]._pMana - (plr[rid]._pMaxMana - plr[rid]._pMaxManaBase);
		CalcPlrInv(rid, TRUE);
		if(plr[rid].plrlevel == currlevel) {
			StartStand(rid, plr[rid]._pdir);
		} else {
			plr[rid]._pmode = PM_STAND;
		}
	}
}

void DoHealOther(int pnum, int rid)
{
	int i;
	long l;

	if(pnum == myplr) {
		SetCursor_(CURSOR_HAND);
	}
	if((BYTE)rid != 255) {
		if(plr[rid]._pHitPoints >> 6 <= 0) {
			return;
		}
		l = (random(57, 10) + 1) << 6;
		for(i = 0; i < plr[pnum]._pLevel; i++) {
			l += (random(57, 4) + 1) << 6;
		}
		for(i = 0; i < GetSpellLevel(pnum, SPL_HEALOTHER); i++) {
			l += (random(57, 6) + 1) << 6;
		}
		if(plr[pnum]._pClass == PC_WARRIOR) {
			l <<= 1;
		}
		if(plr[pnum]._pClass == PC_ROGUE) {
			l += l >> 1;
		}
		plr[rid]._pHitPoints += l;
		if(plr[rid]._pHitPoints > plr[rid]._pMaxHP) {
			plr[rid]._pHitPoints = plr[rid]._pMaxHP;
		}
		plr[rid]._pHPBase += l;
		if(plr[rid]._pHPBase > plr[rid]._pMaxHPBase) {
			plr[rid]._pHPBase = plr[rid]._pMaxHPBase;
		}
		drawhpflag = 1;
	}
}
