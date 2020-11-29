#include "diablo.h"

int trapid; // weak
int trapdir; // weak
unsigned char *pObjCels[40];
char ObjFileList[40];
int objectactive[MAXOBJECTS];
int nobjects; // idb
int leverid; // idb
int objectavail[MAXOBJECTS];
ObjectStruct object[MAXOBJECTS];
int InitObjFlag; // weak
int numobjfiles; // weak

int bxadd[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
int byadd[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
char *shrinestrs[26] =
{
  "Mysterious",
  "Hidden",
  "Gloomy",
  "Weird",
  "Magical",
  "Stone",
  "Religious",
  "Enchanted",
  "Thaumaturgic",
  "Fascinating",
  "Cryptic",
  "Magical",
  "Eldritch",
  "Eerie",
  "Divine",
  "Holy",
  "Sacred",
  "Spiritual",
  "Spooky",
  "Abandoned",
  "Creepy",
  "Quiet",
  "Secluded",
  "Ornate",
  "Glimmering",
  "Tainted"
};
char shrinemin[26] =
{
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1
};
char shrinemax[26] =
{
   16,  16,  16,  16,  16,  16,  16,   8,  16,  16,
   16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
   16,  16,  16,  16,  16,  16
};
// 0 - sp+mp, 1 - sp only, 2 - mp only
char shrineavail[26] =
{
	0,   0,   1,   1,   0,   0,   0,   0,   1,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   2,   0,
	0,   0,   0,   0,   0,   2
};
char *StoryBookName[9] =
{
  "The Great Conflict",
  "The Wages of Sin are War",
  "The Tale of the Horadrim",
  "The Dark Exile",
  "The Sin War",
  "The Binding of the Three",
  "The Realms Beyond",
  "Tale of the Three",
  "The Black King"
};
int StoryText[3][3] =
{
  { TEXT_BOOK11, TEXT_BOOK12, TEXT_BOOK13 },
  { TEXT_BOOK21, TEXT_BOOK22, TEXT_BOOK23 },
  { TEXT_BOOK31, TEXT_BOOK32, TEXT_BOOK33 }
};

void InitObjectGFX()
{
	int i, t;
	BOOLEAN fileload[56];
	char filestr[32];

	memset(fileload, 0, sizeof(fileload));

	for(i = 0; AllObjects[i].oload != -1; i++) {
		if(AllObjects[i].oload == 1 && currlevel >= AllObjects[i].ominlvl && currlevel <= AllObjects[i].omaxlvl) {
			fileload[AllObjects[i].ofindex] = TRUE;
		}
		if(AllObjects[i].otheme != -1) {
			for(t = 0; t < numthemes; t++) {
				if(themes[t].ttype == AllObjects[i].otheme) {
					fileload[AllObjects[i].ofindex] = TRUE;
				}
			}
		}
		if(AllObjects[i].oquest != -1 && QuestStatus(AllObjects[i].oquest)) {
			fileload[AllObjects[i].ofindex] = TRUE;
		}
	}

	/// ASSERT: assert(numobjfiles == 0);
	for(i = 0; i < (int)sizeof(fileload); i++) {
		if(fileload[i]) {
			ObjFileList[numobjfiles] = i;
			sprintf(filestr, "Objects\\%s.CEL", ObjMasterLoadList[i]);
			/// ASSERT: assert(! pObjCels[numobjfiles]);
			pObjCels[numobjfiles] = DiabLoad(filestr, NULL, 'OGFX');
			numobjfiles++;
		}
	}
}

void FreeObjectGFX()
{
	int i;

	for(i = 0; i < numobjfiles; i++) {
		MemFreeDbg(pObjCels[i]);
	}

	numobjfiles = 0;
}

BOOL RndLocOk(int xp, int yp)
{
	if(dMonster[xp][yp] != 0) {
		return FALSE;
	}
	if(dPlayer[xp][yp] != 0) {
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
	if(leveltype == DTYPE_CATHEDRAL && dPiece[xp][yp] > 126 && dPiece[xp][yp] < 144) {
		return FALSE;
	}

	return TRUE;
}

void InitRndLocObj(int min, int max, int objtype)
{
	int i, xp, yp, numobjs;

	numobjs = random(139, max - min) + min;

	for(i = 0; i < numobjs; i++) {
		while(1) {
			xp = random(139, 80) + 16;
			yp = random(139, 80) + 16;
			if(RndLocOk(xp - 1, yp - 1)
			&& RndLocOk(xp, yp - 1)
			&& RndLocOk(xp + 1, yp - 1)
			&& RndLocOk(xp - 1, yp)
			&& RndLocOk(xp, yp)
			&& RndLocOk(xp + 1, yp)
			&& RndLocOk(xp - 1, yp + 1)
			&& RndLocOk(xp, yp + 1)
			&& RndLocOk(xp + 1, yp + 1)) {
				AddObject(objtype, xp, yp);
				break;
			}
		}
	}
}

void InitRndLocBigObj(int min, int max, int objtype)
{
	int i, xp, yp, numobjs;

	numobjs = random(140, max - min) + min;

	for(i = 0; i < numobjs; i++) {
		while(1) {
			xp = random(140, 80) + 16;
			yp = random(140, 80) + 16;
			if(RndLocOk(xp - 1, yp - 2)
			&& RndLocOk(xp, yp - 2)
			&& RndLocOk(xp + 1, yp - 2)
			&& RndLocOk(xp - 1, yp - 1)
			&& RndLocOk(xp, yp - 1)
			&& RndLocOk(xp + 1, yp - 1)
			&& RndLocOk(xp - 1, yp)
			&& RndLocOk(xp, yp)
			&& RndLocOk(xp + 1, yp)
			&& RndLocOk(xp - 1, yp + 1)
			&& RndLocOk(xp, yp + 1)
			&& RndLocOk(xp + 1, yp + 1)) {
				AddObject(objtype, xp, yp);
				break;
			}
		}
	}
}

void InitRndLocObj5x5(int min, int max, int objtype)
{
	int i, xp, yp, xx, yy, cnt, numobjs;
	BOOL done;

	numobjs = random(139, max - min) + min;

	for(i = 0; i < numobjs; i++) {
		cnt = 0;
		done = FALSE;
		while(!done) {
			done = TRUE;
			xp = random(139, 80) + 16;
			yp = random(139, 80) + 16;
			for(yy = -2; yy <= 2; yy++) {
				for(xx = -2; xx <= 2; xx++) {
					if(!RndLocOk(xx + xp, yy + yp)) {
						done = FALSE;
					}
				}
			}
			if(!done) {
				cnt++;
				if(cnt > 20000) {
					return;
				}
			}
		}
		AddObject(objtype, xp, yp);
	}
}

void ClrAllObjects()
{
	int i;

	for(i = 0; i < MAXOBJECTS; i++) {
		object[i]._ox = 0;
		object[i]._oy = 0;
		object[i]._oAnimData = NULL;
		object[i]._oAnimDelay = 0;
		object[i]._oAnimCnt = 0;
		object[i]._oAnimLen = 0;
		object[i]._oAnimFrame = 0;
		object[i]._oDelFlag = 0;
		object[i]._oVar1 = 0;
		object[i]._oVar2 = 0;
		object[i]._oVar3 = 0;
		object[i]._oVar4 = 0;
	}

	nobjects = 0;

	for(i = 0; i < MAXOBJECTS; i++) {
		objectavail[i] = i;
		objectactive[i] = 0;
	}

	trapid = 1;
	trapdir = 0;
	leverid = 1;
}

void AddTortures()
{
	int xp, yp;

	for(yp = 0; yp < MAXDUNY; yp++) {
		for(xp = 0; xp < MAXDUNX; xp++) {
			if(dPiece[xp][yp] == 367) {
				AddObject(OBJ_TORTURE1, xp, yp + 1);
				AddObject(OBJ_TORTURE3, xp + 2, yp - 1);
				AddObject(OBJ_TORTURE2, xp, yp + 3);
				AddObject(OBJ_TORTURE4, xp + 4, yp - 1);
				AddObject(OBJ_TORTURE5, xp, yp + 5);
				AddObject(OBJ_TNUDEM1, xp + 1, yp + 3);
				AddObject(OBJ_TNUDEM2, xp + 4, yp + 5);
				AddObject(OBJ_TNUDEM3, xp + 2, yp);
				AddObject(OBJ_TNUDEM4, xp + 3, yp + 2);
				AddObject(OBJ_TNUDEW1, xp + 2, yp + 4);
				AddObject(OBJ_TNUDEW2, xp + 2, yp + 1);
				AddObject(OBJ_TNUDEW3, xp + 4, yp + 2);
			}
		}
	}
}

void AddCandles()
{
	int xp, yp;

	xp = quests[Q_PWATER]._qtx;
	yp = quests[Q_PWATER]._qty;
	AddObject(OBJ_STORYCANDLE, xp - 2, yp + 1);
	AddObject(OBJ_STORYCANDLE, xp + 3, yp + 1);
	AddObject(OBJ_STORYCANDLE, xp - 1, yp + 2);
	AddObject(OBJ_STORYCANDLE, xp + 2, yp + 2);
}

void AddBookLever(int lx1, int ly1, int lx2, int ly2, int x1, int y1, int x2, int y2, int msg)
{
	int i, xp, yp, xx, yy, cnt;
	BOOL done;

	cnt = 0;
	done = FALSE;
	while(!done) {
		done = TRUE;
		xp = random(139, 80) + 16;
		yp = random(139, 80) + 16;
		for(yy = -2; yy <= 2; yy++) {
			for(xx = -2; xx <= 2; xx++) {
				if(!RndLocOk(xx + xp, yy + yp)) {
					done = FALSE;
				}
			}
		}
		if(!done) {
			cnt++;
			if(cnt > 20000) {
				return;
			}
		}
	}

	if(QuestStatus(Q_BLIND)) {
		AddObject(OBJ_BLINDBOOK, xp, yp);
	}
	if(QuestStatus(Q_WARLORD)) {
		AddObject(OBJ_STEELTOME, xp, yp);
	}
	if(QuestStatus(Q_BLOOD)) {
		xp = 2 * setpc_x + 25;
		yp = 2 * setpc_y + 40;
		AddObject(OBJ_BLOODBOOK, xp, yp);
	}

	/// ASSERT: assert((DWORD)xp < MAXDUNX);
	/// ASSERT: assert((DWORD)yp < MAXDUNY);
	i = dObject[xp][yp] - 1;
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	SetObjMapRange(i, x1, y1, x2, y2, leverid);
	SetBookMsg(i, msg);
	leverid++;
	object[i]._oVar6 = object[i]._oAnimFrame + 1;
}

void InitRndBarrels()
{
	int i, xp, yp, o, c, t, d, numobjs;
	BOOL found;

	numobjs = random(143, 5) + 3;

	for(i = 0; i < numobjs; i++) {
		do {
			xp = random(143, 80) + 16;
			yp = random(143, 80) + 16;
		} while(!RndLocOk(xp, yp));
		if(random(143, 4) != 0) {
			o = OBJ_BARREL;
		} else {
			o = OBJ_BARRELEX;
		}
		AddObject(o, xp, yp);
		c = 1;
		found = TRUE;
		while(random(143, c >> 1) == 0 && found) {
			t = 0;
			found = FALSE;
			while(!found && t < 3) {
				d = random(143, 8);
				xp += bxadd[d];
				yp += byadd[d];
				found = RndLocOk(xp, yp);
				t++;
			}
			if(found) {
				if(random(143, 5) != 0) {
					o = OBJ_BARREL;
				} else {
					o = OBJ_BARRELEX;
				}
				AddObject(o, xp, yp);
				c++;
			}
		}
	}
}

void AddL1Objs(int x1, int y1, int x2, int y2)
{
	int i, j, pn;

	for(j = y1; j < y2; j++) {
		for(i = x1; i < x2; i++) {
			pn = dPiece[i][j];
			if(pn == 270) {
				AddObject(OBJ_L1LIGHT, i, j);
			}
			if(pn == 44 || pn == 51 || pn == 214) {
				AddObject(OBJ_L1LDOOR, i, j);
			}
			if(pn == 46 || pn == 56) {
				AddObject(OBJ_L1RDOOR, i, j);
			}
		}
	}
}

void AddL2Objs(int x1, int y1, int x2, int y2)
{
	int i, j, pn;

	for(j = y1; j < y2; j++) {
		for(i = x1; i < x2; i++) {
			pn = dPiece[i][j];
			if(pn == 13 || pn == 541) {
				AddObject(OBJ_L2LDOOR, i, j);
			}
			if(pn == 17 || pn == 542) {
				AddObject(OBJ_L2RDOOR, i, j);
			}
		}
	}
}

void AddL3Objs(int x1, int y1, int x2, int y2)
{
	int i, j, pn;

	for(j = y1; j < y2; j++) {
		for(i = x1; i < x2; i++) {
			pn = dPiece[i][j];
			if(pn == 531) {
				AddObject(OBJ_L3LDOOR, i, j);
			}
			if(pn == 534) {
				AddObject(OBJ_L3RDOOR, i, j);
			}
		}
	}
}

BOOL WallTrapLocOk(int xp, int yp)
{
	if(dFlags[xp][yp] & 8) {
		return FALSE;
	}

	return TRUE;
}

void AddL2Torches()
{
	int i, j, pn;

	for(j = 0; j < MAXDUNY; j++) {
		for(i = 0; i < MAXDUNX; i++) {
			if(WallTrapLocOk(i, j)) {
				pn = dPiece[i][j];
				if(pn == 1 && random(145, 3) == 0) {
					AddObject(OBJ_TORCHL2, i, j);
				}
				if(pn == 5 && random(145, 3) == 0) {
					AddObject(OBJ_TORCHR2, i, j);
				}
				if(pn == 37 && random(145, 10) == 0 && dObject[i - 1][j] == 0) {
					AddObject(OBJ_TORCHL, i - 1, j);
				}
				if(pn == 41 && random(145, 10) == 0 && dObject[i][j - 1] == 0) {
					AddObject(OBJ_TORCHR, i, j - 1);
				}
			}
		}
	}
}

BOOL TorchLocOK(int xp, int yp)
{
	if(dFlags[xp][yp] & 8) {
		return FALSE;
	}
	if(!nTrapTable[dPiece[xp][yp]]) {
		return FALSE;
	}

	return TRUE;
}

void AddObjTraps()
{
	int i, j, x, y, rndv;
	char oi, oti;

	if(currlevel == 1) {
		rndv = 10;
	}
	if(currlevel >= 2) {
		rndv = 15;
	}
	if(currlevel >= 5) {
		rndv = 20;
	}
	if(currlevel >= 7) {
		rndv = 25;
	}

	for(j = 0; j < MAXDUNY; j++) {
		for(i = 0; i < MAXDUNX; i++) {
			if(dObject[i][j] > 0 && random(144, 100) < rndv) {
				oi = dObject[i][j] - 1;
				if(AllObjects[object[oi]._otype].oTrapFlag) {
					x = i;
					y = j;
					if(random(144, 2) == 0) {
						x--;
						while(!nSolidTable[dPiece[x][y]]) {
							x--;
						}
						if(TorchLocOK(x, y) && i - x > 1) {
							AddObject(OBJ_TRAPL, x, y);
							oti = dObject[x][y] - 1;
							object[oti]._oVar1 = i;
							object[oti]._oVar2 = j;
							object[oi]._oTrapFlag = 1;
						}
					} else {
						y--;
						while(!nSolidTable[dPiece[x][y]]) {
							y--;
						}
						if(TorchLocOK(x, y) && j - y > 1) {
							AddObject(OBJ_TRAPR, x, y);
							oti = dObject[x][y] - 1;
							object[oti]._oVar1 = i;
							object[oti]._oVar2 = j;
							object[oi]._oTrapFlag = 1;
						}
					}
				}
			}
		}
	}
}

void AddChestTraps()
{
	int i, j;
	char oi;

	for(j = 0; j < MAXDUNY; j++) {
		for(i = 0; i < MAXDUNX; i++) {
			if(dObject[i][j] > 0) {
				oi = dObject[i][j] - 1;
				if(object[oi]._otype >= OBJ_CHEST1 && object[oi]._otype <= OBJ_CHEST3 && !object[oi]._oTrapFlag && random(0, 100) < 10) {
					object[oi]._otype += OBJ_TCHEST1 - OBJ_CHEST1;
					object[oi]._oTrapFlag = 1;
					if(leveltype == DTYPE_CATACOMBS) {
						object[oi]._oVar4 = random(0, 2);
					} else {
						object[oi]._oVar4 = random(0, 3);
					}
				}
			}
		}
	}
}

void LoadMapObjects(BYTE *pMap, int startx, int starty, int x1, int y1, int w, int h, int leveridx)
{
	int i, j, rw, rh, ot, ox, oy, oi;
	long mapoff;
	BYTE *lm;

	InitObjFlag = 1;

	lm = pMap;
	rw = *lm;
	lm += 2;
	rh = *lm;
	mapoff = 2 * rw * rh + 2;
	rw <<= 1;
	rh <<= 1;
	mapoff += 4 * rw * rh;
	lm += mapoff;

	for(j = 0; j < rh; j++) {
		for(i = 0; i < rw; i++) {
			if(*lm != 0) {
				ot = *lm;
				ox = i + startx + 16;
				oy = j + starty + 16;
				AddObject(ObjTypeConv[ot], ox, oy);
				oi = ObjIndex(ox, oy);
				SetObjMapRange(oi, x1, y1, x1 + w, y1 + h, leveridx);
			}
			lm += 2;
		}
	}

	InitObjFlag = 0;
}

void LoadMapObjs(BYTE *pMap, int startx, int starty)
{
	int i, j, rw, rh, ot, ox, oy;
	long mapoff;
	BYTE *lm;

	InitObjFlag = 1;

	lm = pMap;
	rw = *lm;
	lm += 2;
	rh = *lm;
	mapoff = 2 * rw * rh + 2;
	rw <<= 1;
	rh <<= 1;
	mapoff += 4 * rw * rh;
	lm += mapoff;

	for(j = 0; j < rh; j++) {
		for(i = 0; i < rw; i++) {
			if(*lm != 0) {
				ot = *lm;
				ox = i + startx + 16;
				oy = j + starty + 16;
				AddObject(ObjTypeConv[ot], ox, oy);
			}
			lm += 2;
		}
	}

	InitObjFlag = 0;
}

void AddDiabObjs()
{
	BYTE *lpSetPiece;

	lpSetPiece = DiabLoad("Levels\\L4Data\\diab1.DUN", NULL, 'STPC');
	LoadMapObjects(lpSetPiece, 2 * diabquad1x, 2 * diabquad1y, diabquad2x, diabquad2y, 11, 12, 1);
	MemFreeDbg(lpSetPiece);
	lpSetPiece = DiabLoad("Levels\\L4Data\\diab2a.DUN", NULL, 'STPC');
	LoadMapObjects(lpSetPiece, 2 * diabquad2x, 2 * diabquad2y, diabquad3x, diabquad3y, 11, 11, 2);
	MemFreeDbg(lpSetPiece);
	lpSetPiece = DiabLoad("Levels\\L4Data\\diab3a.DUN", NULL, 'STPC');
	LoadMapObjects(lpSetPiece, 2 * diabquad3x, 2 * diabquad3y, diabquad4x, diabquad4y, 9, 9, 3);
	MemFreeDbg(lpSetPiece);
}

void AddStoryBooks()
{
	int xp, yp, xx, yy, cnt;
	BOOL done;

	cnt = 0;
	done = FALSE;
	while(!done) {
		done = TRUE;
		xp = random(139, 80) + 16;
		yp = random(139, 80) + 16;
		for(yy = -2; yy <= 2; yy++) {
			for(xx = -3; xx <= 3; xx++) {
				if(!RndLocOk(xx + xp, yy + yp)) {
					done = FALSE;
				}
			}
		}
		if(!done) {
			cnt++;
			if(cnt > 20000) {
				return;
			}
		}
	}

	AddObject(OBJ_STORYBOOK, xp, yp);
	AddObject(OBJ_STORYCANDLE, xp - 2, yp + 1);
	AddObject(OBJ_STORYCANDLE, xp - 2, yp);
	AddObject(OBJ_STORYCANDLE, xp - 1, yp - 1);
	AddObject(OBJ_STORYCANDLE, xp + 1, yp - 1);
	AddObject(OBJ_STORYCANDLE, xp + 2, yp);
	AddObject(OBJ_STORYCANDLE, xp + 2, yp + 1);
}

void AddHookedBodies(int freq)
{
	int i, j, ii, jj;

	for(j = 0; j < DMAXY; j++) {
		jj = 2 * j + 16;
		for(i = 0; i < DMAXX; i++) {
			ii = 2 * i + 16;
			if(dungeon[i][j] == 1 || dungeon[i][j] == 2) {
				if(random(0, freq) == 0 && SkipThemeRoom(i, j)) {
					if(dungeon[i][j] == 1 && dungeon[i + 1][j] == 6) {
						switch(random(0, 3)) {
						case 0:
							AddObject(OBJ_TORTURE1, ii + 1, jj);
							break;
						case 1:
							AddObject(OBJ_TORTURE2, ii + 1, jj);
							break;
						case 2:
							AddObject(OBJ_TORTURE5, ii + 1, jj);
							break;
						}
					} else if(dungeon[i][j] == 2 && dungeon[i][j + 1] == 6) {
						switch(random(0, 2)) {
						case 0:
							AddObject(OBJ_TORTURE3, ii, jj);
							break;
						case 1:
							AddObject(OBJ_TORTURE4, ii, jj);
							break;
						}
					}
				}
			}
		}
	}
}

void AddL4Goodies()
{
	AddHookedBodies(6);
	InitRndLocObj(2, 6, OBJ_TNUDEM1);
	InitRndLocObj(2, 6, OBJ_TNUDEM2);
	InitRndLocObj(2, 6, OBJ_TNUDEM3);
	InitRndLocObj(2, 6, OBJ_TNUDEM4);
	InitRndLocObj(2, 6, OBJ_TNUDEW1);
	InitRndLocObj(2, 6, OBJ_TNUDEW2);
	InitRndLocObj(2, 6, OBJ_TNUDEW3);
	InitRndLocObj(2, 6, OBJ_DECAP);
	InitRndLocObj(1, 3, OBJ_CAULDRON);
}

void AddLazStand()
{
	int xp, yp, xx, yy, cnt;
	BOOL done;

	cnt = 0;
	done = FALSE;
	while(!done) {
		done = TRUE;
		xp = random(139, 80) + 16;
		yp = random(139, 80) + 16;
		for(yy = -3; yy <= 3; yy++) {
			for(xx = -2; xx <= 3; xx++) {
				if(!RndLocOk(xx + xp, yy + yp)) {
					done = FALSE;
				}
			}
		}
		if(!done) {
			cnt++;
			if(cnt > 10000) {
				InitRndLocObj(1, 1, OBJ_LAZSTAND);
				return;
			}
		}
	}

	AddObject(OBJ_LAZSTAND, xp, yp);
	AddObject(OBJ_TNUDEM2, xp, yp + 2);
	AddObject(OBJ_STORYCANDLE, xp + 1, yp + 2);
	AddObject(OBJ_TNUDEM3, xp + 2, yp + 2);
	AddObject(OBJ_TNUDEW1, xp, yp - 2);
	AddObject(OBJ_STORYCANDLE, xp + 1, yp - 2);
	AddObject(OBJ_TNUDEW2, xp + 2, yp - 2);
	AddObject(OBJ_STORYCANDLE, xp - 1, yp - 1);
	AddObject(OBJ_TNUDEW3, xp - 1, yp);
	AddObject(OBJ_STORYCANDLE, xp - 1, yp + 1);
}

void InitObjects()
{
	int textdef;
	BYTE *setp;

	ClrAllObjects();

	if(currlevel == 16) {
		AddDiabObjs();
		return;
	}

	InitObjFlag = 1;
	GetRndSeed();

	if(currlevel == 9 && gbMaxPlayers == 1) {
		AddSlainHero();
	}
	if(currlevel == quests[Q_MUSHROOM]._qlevel && quests[Q_MUSHROOM]._qactive == 1) {
		AddMushPatch();
	}
	if(currlevel == 4) {
		AddStoryBooks();
	}
	if(currlevel == 8) {
		AddStoryBooks();
	}
	if(currlevel == 12) {
		AddStoryBooks();
	}

	if(leveltype == DTYPE_CATHEDRAL) {
		if(QuestStatus(Q_BUTCHER)) {
			AddTortures();
		}
		if(QuestStatus(Q_PWATER)) {
			AddCandles();
		}
		if(QuestStatus(Q_LTBANNER)) {
			AddObject(OBJ_SIGNCHEST, 2 * setpc_x + 26, 2 * setpc_y + 19);
		}
		InitRndLocBigObj(10, 15, OBJ_SARC);
		AddL1Objs(0, 0, 112, 112);
		InitRndBarrels();
	}
	if(leveltype == DTYPE_CATACOMBS) {
		if(QuestStatus(Q_ROCK)) {
			InitRndLocObj5x5(1, 1, OBJ_STAND);
		}
		if(QuestStatus(Q_SCHAMB)) {
			InitRndLocObj5x5(1, 1, OBJ_BOOK2R);
		}
		AddL2Objs(0, 0, 112, 112);
		AddL2Torches();
		if(QuestStatus(Q_BLIND)) {
			if(plr[myplr]._pClass == PC_WARRIOR) {
				textdef = TEXT_BLINDING;
			} else if(plr[myplr]._pClass == PC_ROGUE) {
				textdef = TEXT_RBLINDING;
			} else if(plr[myplr]._pClass == PC_SORCERER) {
				textdef = TEXT_MBLINDING;
			}
			quests[Q_BLIND]._qmsg = textdef;
			AddBookLever(0, 0, 112, 112, setpc_x, setpc_y, setpc_x + setpc_w + 1, setpc_y + setpc_h + 1, textdef);
			setp = DiabLoad("Levels\\L2Data\\Blind2.DUN", NULL, 'STPC');
			LoadMapObjs(setp, 2 * setpc_x, 2 * setpc_y);
			MemFreeDbg(setp);
		}
		if(QuestStatus(Q_BLOOD)) {
			if(plr[myplr]._pClass == PC_WARRIOR) {
				textdef = TEXT_BLOODY;
			} else if(plr[myplr]._pClass == PC_ROGUE) {
				textdef = TEXT_RBLOODY;
			} else if(plr[myplr]._pClass == PC_SORCERER) {
				textdef = TEXT_MBLOODY;
			}
			quests[Q_BLOOD]._qmsg = textdef;
			AddBookLever(0, 0, 112, 112, setpc_x, setpc_y + 3, setpc_x + 2, setpc_y + 7, textdef);
			AddObject(OBJ_PEDISTAL, 2 * setpc_x + 25, 2 * setpc_y + 32);
		}
		InitRndBarrels();
	}
	if(leveltype == DTYPE_CAVES) {
		AddL3Objs(0, 0, 112, 112);
		InitRndBarrels();
	}
	if(leveltype == DTYPE_HELL) {
		if(QuestStatus(Q_WARLORD)) {
			if(plr[myplr]._pClass == PC_WARRIOR) {
				textdef = TEXT_BLOODWAR;
			} else if(plr[myplr]._pClass == PC_ROGUE) {
				textdef = TEXT_RBLOODWAR;
			} else if(plr[myplr]._pClass == PC_SORCERER) {
				textdef = TEXT_MBLOODWAR;
			}
			quests[Q_WARLORD]._qmsg = textdef;
			AddBookLever(0, 0, 112, 112, setpc_x, setpc_y, setpc_x + setpc_w, setpc_y + setpc_h, textdef);
			setp = DiabLoad("Levels\\L4Data\\Warlord.DUN", NULL, 'STPC');
			LoadMapObjs(setp, 2 * setpc_x, 2 * setpc_y);
			MemFreeDbg(setp);
		}
		if(QuestStatus(Q_BETRAYER) && gbMaxPlayers == 1) {
			AddLazStand();
		}
		InitRndBarrels();
		AddL4Goodies();
	}

	InitRndLocObj(5, 10, 5);
	InitRndLocObj(3, 6, 6);
	InitRndLocObj(1, 5, 7);

	if(leveltype != DTYPE_HELL) {
		AddObjTraps();
	}
	if(leveltype > DTYPE_CATHEDRAL) {
		AddChestTraps();
	}

	InitObjFlag = 0;
}

void SetMapObjects(BYTE *pMap, int startx, int starty)
{
	int i, j, rw, rh, ot, ox, oy;
	long mapoff;
	BYTE *lm, *h;
	BOOL fileload[56];
	char filestr[32];

	ClrAllObjects();
	InitObjFlag = 1;

	for(i = 0; i < sizeof(fileload) / sizeof(fileload[0]); i++) {
		fileload[i] = FALSE;
	}

	for(i = 0; AllObjects[i].oload != -1; i++) {
		if(AllObjects[i].oload == 1 && leveltype == AllObjects[i].olvltype) {
			fileload[AllObjects[i].ofindex] = TRUE;
		}
	}

	lm = pMap;
	rw = *lm;
	lm += 2;
	rh = *lm;
	mapoff = 2 * rw * rh + 2;
	rw <<= 1;
	rh <<= 1;
	mapoff += 4 * rw * rh;
	lm += mapoff;
	h = lm;

	for(j = 0; j < rh; j++) {
		for(i = 0; i < rw; i++) {
			if(*lm != 0) {
				ot = *lm;
				ot = ObjTypeConv[ot];
				fileload[AllObjects[ot].ofindex] = TRUE;
			}
			lm += 2;
		}
	}

	/// ASSERT: assert(numobjfiles == 0);
	for(i = 0; i < (int)(sizeof(fileload) / sizeof(fileload[0])); i++) {
		if(fileload[i]) {
			ObjFileList[numobjfiles] = i;
			sprintf(filestr, "Objects\\%s.CEL", ObjMasterLoadList[i]);
			/// ASSERT: assert(! pObjCels[numobjfiles]);
			pObjCels[numobjfiles] = DiabLoad(filestr, NULL, 'OGFX');
			numobjfiles++;
		}
	}

	lm = h;
	for(j = 0; j < rh; j++) {
		for(i = 0; i < rw; ++i) {
			if(*lm != 0) {
				ot = *lm;
				ox = i + startx + 16;
				oy = j + starty + 16;
				AddObject(ObjTypeConv[ot], ox, oy);
			}
			lm += 2;
		}
	}

	InitObjFlag = 0;
}

void DeleteObject_(int oi, int i)
{
	int ox, oy;

	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
	ox = object[oi]._ox;
	oy = object[oi]._oy;
	dObject[ox][oy] = 0;
	objectavail[MAXOBJECTS - nobjects] = oi;
	nobjects--;

	if(nobjects > 0 && i != nobjects) {
		objectactive[i] = objectactive[nobjects];
	}
}

void SetupObject(int i, int x, int y, int ot)
{
	int ai, j;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._otype = ot;
	object[i]._ox = x;
	object[i]._oy = y;

	ai = AllObjects[ot].ofindex;
	j = 0;
	while(ObjFileList[j] != ai) {
		j++;
	}

	object[i]._oAnimData = pObjCels[j];
	object[i]._oAnimFlag = AllObjects[ot].oAnimFlag;

	if(object[i]._oAnimFlag != 0) {
		object[i]._oAnimDelay = AllObjects[ot].oAnimDelay;
		object[i]._oAnimCnt = random(146, AllObjects[ot].oAnimDelay);
		object[i]._oAnimLen = AllObjects[ot].oAnimLen;
		object[i]._oAnimFrame = random(146, AllObjects[ot].oAnimLen - 1) + 1;
	} else {
		object[i]._oAnimDelay = 1000;
		object[i]._oAnimCnt = 0;
		object[i]._oAnimLen = AllObjects[ot].oAnimLen;
		object[i]._oAnimFrame = AllObjects[ot].oAnimDelay;
	}

	object[i]._oAnimWidth = AllObjects[ot].oAnimWidth;
	object[i]._oSolidFlag = AllObjects[ot].oSolidFlag;
	object[i]._oMissFlag = AllObjects[ot].oMissFlag;
	object[i]._oLight = AllObjects[ot].oLightFlag;
	object[i]._oDelFlag = 0;
	object[i]._oBreak = AllObjects[ot].oBreak;
	object[i]._oSelFlag = AllObjects[ot].oSelFlag;
	object[i]._oPreFlag = 0;
	object[i]._oTrapFlag = 0;
	object[i]._oDoorFlag = 0;
}

void SetObjMapRange(int i, int x1, int y1, int x2, int y2, int v)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oVar1 = x1;
	object[i]._oVar2 = y1;
	object[i]._oVar3 = x2;
	object[i]._oVar4 = y2;
	object[i]._oVar8 = v;
}

void SetBookMsg(int i, int msg)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oVar7 = msg;
}

void AddL1Door(int i, int x, int y, int ot)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oDoorFlag = 1;

	if(ot == OBJ_L1LDOOR) {
		object[i]._oVar1 = dPiece[x][y];
		object[i]._oVar2 = dPiece[x][y - 1];
	} else {
		object[i]._oVar1 = dPiece[x][y];
		object[i]._oVar2 = dPiece[x - 1][y];
	}

	object[i]._oVar4 = 0;
}

void AddSCambBook(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oVar1 = setpc_x;
	object[i]._oVar2 = setpc_y;
	object[i]._oVar3 = setpc_x + setpc_w + 1;
	object[i]._oVar4 = setpc_y + setpc_h + 1;
	object[i]._oVar6 = object[i]._oAnimFrame + 1;
}

void AddChest(int i, int t)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);

	if(random(147, 2) == 0) {
		object[i]._oAnimFrame += 3;
	}

	object[i]._oRndSeed = GetRndSeed();

	switch(t) {
	case OBJ_CHEST1:
	case OBJ_TCHEST1:
		if(setlevel) {
			object[i]._oVar1 = 1;
		} else {
			object[i]._oVar1 = random(147, 2);
		}
		break;
	case OBJ_CHEST2:
	case OBJ_TCHEST2:
		if(setlevel) {
			object[i]._oVar1 = 2;
		} else {
			object[i]._oVar1 = random(147, 3);
		}
		break;
	case OBJ_CHEST3:
	case OBJ_TCHEST3:
		if(setlevel) {
			object[i]._oVar1 = 3;
		} else {
			object[i]._oVar1 = random(147, 4);
		}
		break;
	}

	object[i]._oVar2 = random(147, 8);
}

void AddL2Door(int i, int x, int y, int ot)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oDoorFlag = 1;

	if(ot == OBJ_L2LDOOR) {
		ObjSetMicro(x, y, 538);
	} else {
		ObjSetMicro(x, y, 540);
	}

	object[i]._oVar4 = 0;
}

void AddL3Door(int i, int x, int y, int ot)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oDoorFlag = 1;

	if(ot == OBJ_L3LDOOR) {
		ObjSetMicro(x, y, 531);
	} else {
		ObjSetMicro(x, y, 534);
	}

	object[i]._oVar4 = 0;
}

void AddSarc(int i)
{
	int x, y;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	x = object[i]._ox;
	y = object[i]._oy - 1;
	dObject[x][y] = -(i + 1);
	object[i]._oVar1 = random(153, 10);
	object[i]._oRndSeed = GetRndSeed();

	if(object[i]._oVar1 >= 8) {
		object[i]._oVar2 = PreSpawnSkeleton();
	}
}

void AddFlameTrap(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oVar1 = trapid;
	object[i]._oVar2 = 0;
	object[i]._oVar3 = trapdir;
	object[i]._oVar4 = 0;
}

void AddFlameLvr(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oVar1 = trapid;
	object[i]._oVar2 = OBJ_FLAMEHOLE; /* check, might be missile */
}

void AddTrap(int i, int ot)
{
	int mt, c;

	c = currlevel / 3 + 1;
	mt = random(148, c);

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(mt == 0) {
		object[i]._oVar3 = MIS_ARROW;
	}
	if(mt == 1) {
		object[i]._oVar3 = MIS_FIREBOLT;
	}
	if(mt == 2) {
		object[i]._oVar3 = MIS_LIGHTCTRL;
	}

	object[i]._oVar4 = 0;
}

void AddObjLight(int i, int r)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(InitObjFlag) {
		DoLighting(object[i]._ox, object[i]._oy, r, -1);
		object[i]._oVar1 = -1;
	} else {
		object[i]._oVar1 = 0;
	}
}

void AddBarrel(int i, int ot)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oVar1 = 0;
	object[i]._oRndSeed = GetRndSeed();
	object[i]._oVar2 = random(149, 10);
	object[i]._oVar3 = random(149, 3);

	if(object[i]._oVar2 >= 8) {
		object[i]._oVar4 = PreSpawnSkeleton();
	}
}

void AddShrine(int i)
{
	int st;
	BOOL slist[26];

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oPreFlag = 1;

	for(st = 0; st < 26; st++) {
		if(currlevel >= shrinemin[st] && currlevel <= shrinemax[st]) {
			slist[st] = TRUE;
		} else {
			slist[st] = FALSE;
		}
		if(gbMaxPlayers != 1 && shrineavail[st] == 1) {
			slist[st] = FALSE;
		}
		if(gbMaxPlayers == 1 && shrineavail[st] == 2) {
			slist[st] = FALSE;
		}
	}

	do {
		st = random(150, 26);
	} while(!slist[st]);

	object[i]._oVar1 = st;

	if(random(150, 2) != 0) {
		object[i]._oAnimFrame = 12;
		object[i]._oAnimLen = 22;
	}
}

void AddBookcase(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oRndSeed = GetRndSeed();
	object[i]._oPreFlag = 1;
}

void AddBookstand(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oRndSeed = GetRndSeed();
}

void AddBloodFtn(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oRndSeed = GetRndSeed();
}

void AddPurifyingFountain(int i)
{
	int x, y;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	x = object[i]._ox;
	y = object[i]._oy;
	dObject[x][y - 1] = -(i + 1);
	dObject[x - 1][y] = -(i + 1);
	dObject[x - 1][y - 1] = -(i + 1);
	object[i]._oRndSeed = GetRndSeed();
}

void AddArmorStand(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(!armorFlag) {
		object[i]._oAnimFlag = 2;
		object[i]._oSelFlag = 0;
	}

	object[i]._oRndSeed = GetRndSeed();
}

void AddGoatShrine(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oRndSeed = GetRndSeed();
}

void AddCauldron(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oRndSeed = GetRndSeed();
}

void AddMurkyFountain(int i)
{
	int x, y;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	x = object[i]._ox;
	y = object[i]._oy;
	dObject[x][y - 1] = -(i + 1);
	dObject[x - 1][y] = -(i + 1);
	dObject[x - 1][y - 1] = -(i + 1);
	object[i]._oRndSeed = GetRndSeed();
}

void AddTearFountain(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oRndSeed = GetRndSeed();
}

void AddDecap(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oRndSeed = GetRndSeed();
	object[i]._oAnimFrame = random(151, 8) + 1;
	object[i]._oPreFlag = 1;
}

void AddVilebook(int i)
{
	if(setlevel && setlvlnum == SL_VILEBETRAYER) {
		/// ASSERT: assert((DWORD)i < MAXOBJECTS);
		object[i]._oAnimFrame = 4;
	}
}

void AddMagicCircle(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oRndSeed = GetRndSeed();
	object[i]._oPreFlag = 1;
	object[i]._oVar6 = 0;
	object[i]._oVar5 = 1;
}

void AddBrnCross(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oRndSeed = GetRndSeed();
}

void AddPedistal(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oVar1 = setpc_x;
	object[i]._oVar2 = setpc_y;
	object[i]._oVar3 = setpc_x + setpc_w;
	object[i]._oVar4 = setpc_y + setpc_h;
}

void AddStoryBook(int i)
{
	SetRndSeed(glSeedTbl[16]);
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oVar1 = random(0, 3);

	if(currlevel == 4) {
		object[i]._oVar2 = StoryText[object[i]._oVar1][0];
	}
	if(currlevel == 8) {
		object[i]._oVar2 = StoryText[object[i]._oVar1][1];
	}
	if(currlevel == 12) {
		object[i]._oVar2 = StoryText[object[i]._oVar1][2];
	}

	object[i]._oVar3 = (currlevel >> 2) + 3 * object[i]._oVar1 - 1;
	object[i]._oAnimFrame = 5 - 2 * object[i]._oVar1;
	object[i]._oVar4 = object[i]._oAnimFrame + 1;
}

void AddWeaponRack(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(!weaponFlag) {
		object[i]._oAnimFlag = 2;
		object[i]._oSelFlag = 0;
	}

	object[i]._oRndSeed = GetRndSeed();
}

void AddTorturedBody(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oRndSeed = GetRndSeed();
	object[i]._oAnimFrame = random(0, 4) + 1;
	object[i]._oPreFlag = 1;
}

void GetRndObjLoc(int randarea, int &xx, int &yy)
{
	int i, j, tries;
	BOOL failed;

	if(randarea == 0) {
		return;
	}

	tries = 0;

	while(1) {
		tries++;
		if(tries > 1000 && randarea > 1) {
			randarea--;
		}
		xx = random(0, MAXDUNX);
		yy = random(0, MAXDUNY);
		failed = FALSE;
		for(i = 0; i < randarea && !failed; i++) {
			for(j = 0; j < randarea && !failed; j++) {
				failed = !RndLocOk(i + xx, j + yy);
			}
		}
		if(!failed) {
			break;
		}
	}
}

void AddMushPatch()
{
	int i, x, y;

	if(nobjects >= MAXOBJECTS) {
		return;
	}

	i = objectavail[0];
	GetRndObjLoc(5, x, y);
	dObject[x + 1][y + 1] = -(i + 1);
	dObject[x + 2][y + 1] = -(i + 1);
	dObject[x + 1][y + 2] = -(i + 1);
	AddObject(OBJ_MUSHPATCH, x + 2, y + 2);
}

void AddSlainHero()
{
	int x, y;

	GetRndObjLoc(5, x, y);
	AddObject(OBJ_SLAINHERO, x + 2, y + 2);
}

void AddObject(int ot, int ox, int oy)
{
	int oi;

	if(nobjects >= MAXOBJECTS) {
		return;
	}

	oi = objectavail[0];
	objectavail[0] = objectavail[MAXOBJECTS - nobjects - 1];
	objectactive[nobjects] = oi;
	dObject[ox][oy] = oi + 1;
	SetupObject(oi, ox, oy, ot);

	switch(ot) {
	case OBJ_L1LIGHT:
		AddObjLight(oi, 5);
		break;
	case OBJ_SKFIRE:
	case OBJ_CANDLE1:
	case OBJ_CANDLE2:
	case OBJ_BOOKCANDLE:
		AddObjLight(oi, 5);
		break;
	case OBJ_STORYCANDLE:
		AddObjLight(oi, 3);
		break;
	case OBJ_TORCHL:
	case OBJ_TORCHR:
	case OBJ_TORCHL2:
	case OBJ_TORCHR2:
		AddObjLight(oi, 8);
		break;
	case OBJ_L1LDOOR:
	case OBJ_L1RDOOR:
		AddL1Door(oi, ox, oy, ot);
		break;
	case OBJ_L2LDOOR:
	case OBJ_L2RDOOR:
		AddL2Door(oi, ox, oy, ot);
		break;
	case OBJ_L3LDOOR:
	case OBJ_L3RDOOR:
		AddL3Door(oi, ox, oy, ot);
		break;
	case OBJ_BOOK2R:
		AddSCambBook(oi);
		break;
	case OBJ_CHEST1:
	case OBJ_CHEST2:
	case OBJ_CHEST3:
	case OBJ_TCHEST1:
	case OBJ_TCHEST2:
	case OBJ_TCHEST3:
		AddChest(oi, ot);
		break;
	case OBJ_SARC:
		AddSarc(oi);
		break;
	case OBJ_FLAMEHOLE:
		AddFlameTrap(oi);
		break;
	case OBJ_FLAMELVR:
		AddFlameLvr(oi);
		break;
	case OBJ_WATER:
		/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
		object[oi]._oAnimFrame = 1;
		break;
	case OBJ_TRAPL:
	case OBJ_TRAPR:
		AddTrap(oi, ot);
		break;
	case OBJ_BARREL:
	case OBJ_BARRELEX:
		AddBarrel(oi, ot);
		break;
	case OBJ_SHRINEL:
	case OBJ_SHRINER:
		AddShrine(oi);
		break;
	case OBJ_BOOKCASEL:
	case OBJ_BOOKCASER:
		AddBookcase(oi);
		break;
	case OBJ_SKELBOOK:
	case OBJ_BOOKSTAND:
		AddBookstand(oi);
		break;
	case OBJ_BLOODFTN:
		AddBloodFtn(oi);
		break;
	case OBJ_DECAP:
		AddDecap(oi);
		break;
	case OBJ_PURIFYINGFTN:
		AddPurifyingFountain(oi);
		break;
	case OBJ_ARMORSTAND:
	case OBJ_WARARMOR:
		AddArmorStand(oi);
		break;
	case OBJ_GOATSHRINE:
		AddGoatShrine(oi);
		break;
	case OBJ_CAULDRON:
		AddCauldron(oi);
		break;
	case OBJ_MURKYFTN:
		AddMurkyFountain(oi);
		break;
	case OBJ_TEARFTN:
		AddTearFountain(oi);
		break;
	case OBJ_BOOK2L:
		AddVilebook(oi);
		break;
	case OBJ_MCIRCLE1:
	case OBJ_MCIRCLE2:
		AddMagicCircle(oi);
		break;
	case OBJ_STORYBOOK:
		AddStoryBook(oi);
		break;
	case OBJ_BCROSS:
	case OBJ_TBCROSS:
		AddBrnCross(oi);
		AddObjLight(oi, 5);
		break;
	case OBJ_PEDISTAL:
		AddPedistal(oi);
		break;
	case OBJ_WARWEAP:
	case OBJ_WEAPONRACK:
		AddWeaponRack(oi);
		break;
	case OBJ_TNUDEM2:
		AddTorturedBody(oi);
		break;
	}

	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
	object[oi]._oAnimWidth2 = (object[oi]._oAnimWidth - 64) >> 1;
	nobjects++;
}

void Obj_Light(int i, int lr)
{
	int ox, oy, dx, dy, p, tr;
	BOOL turnon;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oVar1 == -1) {
		return;
	}

	ox = object[i]._ox;
	oy = object[i]._oy;
	tr = lr + 10;
	turnon = FALSE;

	if(!lightflag) {
		for(p = 0; p < MAX_PLRS && !turnon; p++) {
			if(plr[p].plractive && currlevel == plr[p].plrlevel) {
				dx = abs(plr[p].WorldX - ox);
				dy = abs(plr[p].WorldY - oy);
				if(dx < tr && dy < tr) {
					turnon = TRUE;
				}
			}
		}
	}

	if(turnon) {
		if(object[i]._oVar1 == 0) {
			object[i]._olid = AddLight(ox, oy, lr);
		}
		object[i]._oVar1 = 1;
	} else {
		if(object[i]._oVar1 == 1) {
			AddUnLight(object[i]._olid);
		}
		object[i]._oVar1 = 0;
	}
}

void Obj_Circle(int i)
{
	int px, py, ox, oy;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	ox = object[i]._ox;
	oy = object[i]._oy;
	px = plr[myplr].WorldX;
	py = plr[myplr].WorldY;

	if(px == ox && py == oy) {
		if(object[i]._otype == OBJ_MCIRCLE1) {
			object[i]._oAnimFrame = 2;
		}
		if(object[i]._otype == OBJ_MCIRCLE2) {
			object[i]._oAnimFrame = 4;
		}
		if(ox == 45 && oy == 47) {
			object[i]._oVar6 = 2;
		} else if(ox == 26 && oy == 46) {
			object[i]._oVar6 = 1;
		} else {
			object[i]._oVar6 = 0;
		}
		if(ox == 35 && oy == 36 && object[i]._oVar5 == 3) {
			object[i]._oVar6 = 4;
			ObjChangeMapResync(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
			if(quests[Q_BETRAYER]._qactive == 2) {
				quests[Q_BETRAYER]._qvar1 = 4;
			}
			AddMissile(plr[myplr].WorldX, plr[myplr].WorldY, 35, 46, plr[myplr]._pdir, MIS_RNDTELEPORT, 0, myplr, 0, 0);
			track_repeat_walk(0);
			sgbMouseDown = 0;
			ReleaseCapture();
			ClrPlrPath(myplr);
			StartStand(myplr, 0);
		}
	} else {
		if(object[i]._otype == OBJ_MCIRCLE1) {
			object[i]._oAnimFrame = 1;
		}
		if(object[i]._otype == OBJ_MCIRCLE2) {
			object[i]._oAnimFrame = 3;
		}
		object[i]._oVar6 = 0;
	}
}

void Obj_StopAnim(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oAnimFrame == object[i]._oAnimLen) {
		object[i]._oAnimCnt = 0;
		object[i]._oAnimDelay = 1000;
	}
}

void Obj_Door(int i)
{
	int dx, dy;
	BOOL dok;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oVar4 == 0) {
		object[i]._oSelFlag = 3;
		object[i]._oMissFlag = 0;
	} else {
		dx = object[i]._ox;
		dy = object[i]._oy;
		dok = dMonster[dx][dy] == 0;
		dok &= dItem[dx][dy] == 0;
		dok &= dDead[dx][dy] == 0;
		dok &= dPlayer[dx][dy] == 0;
		object[i]._oSelFlag = 2;
		if(dok) {
			object[i]._oVar4 = 1;
		} else {
			object[i]._oVar4 = 2;
		}
		object[i]._oMissFlag = 1;
	}
}

void Obj_Sarc(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oAnimFrame == object[i]._oAnimLen) {
		object[i]._oAnimFlag = 0;
	}
}

void ActivateTrapLine(int ttype, int tid)
{
	int i, oi;

	for(i = 0; i < nobjects; i++) {
		oi = objectactive[i];
		if(object[oi]._otype == ttype && object[oi]._oVar1 == tid) {
			object[oi]._oVar4 = 1;
			object[oi]._oAnimFlag = 1;
			object[oi]._oAnimDelay = 1;
			object[oi]._olid = AddLight(object[oi]._ox, object[oi]._oy, 1);
		}
	}
}

void Obj_FlameTrap(int i)
{
	int xp, yp, j;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oVar2 != 0) {
		if(object[i]._oVar4 != 0) {
			object[i]._oAnimFrame--;
			if(object[i]._oAnimFrame == 1) {
				object[i]._oVar4 = 0;
				AddUnLight(object[i]._olid);
			} else if(object[i]._oAnimFrame <= 4) {
				ChangeLightRadius(object[i]._olid, object[i]._oAnimFrame);
			}
		}
	} else if(object[i]._oVar4 == 0) {
		if(object[i]._oVar3 == 2) {
			xp = object[i]._ox - 2;
			yp = object[i]._oy;
			for(j = 0; j < 5; j++) {
				if(dPlayer[xp][yp] != 0 || dMonster[xp][yp] != 0) {
					object[i]._oVar4 = 1;
				}
				xp++;
			}
		} else {
			xp = object[i]._ox;
			yp = object[i]._oy - 2;
			for(j = 0; j < 5; j++) {
				if(dPlayer[xp][yp] != 0 || dMonster[xp][yp] != 0) {
					object[i]._oVar4 = 1;
				}
				yp++;
			}
		}
		if(object[i]._oVar4 != 0) {
			ActivateTrapLine(object[i]._otype, object[i]._oVar1);
		}
	} else {
		if(object[i]._oAnimFrame == object[i]._oAnimLen) {
			object[i]._oAnimFrame = 11;
		}
		if(object[i]._oAnimFrame <= 5) {
			ChangeLightRadius(object[i]._olid, object[i]._oAnimFrame);
		}
	}
}

void Obj_Trap(int i)
{
	int oti, sx, sy, dx, dy, x, y, ax, ay, dir;
	BOOL otrig;

	otrig = FALSE;
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oVar4 != 0) {
		return;
	}

	oti = dObject[object[i]._oVar1][object[i]._oVar2] - 1;
	switch(object[oti]._otype) {
	case OBJ_LEVER:
	case OBJ_CHEST1:
	case OBJ_CHEST2:
	case OBJ_CHEST3:
	case OBJ_SWITCHSKL:
	case OBJ_SARC:
		if(object[oti]._oSelFlag == 0) {
			otrig = TRUE;
		}
		break;
	case OBJ_L1LDOOR:
	case OBJ_L1RDOOR:
	case OBJ_L2LDOOR:
	case OBJ_L2RDOOR:
	case OBJ_L3LDOOR:
	case OBJ_L3RDOOR:
		if(object[oti]._oVar4 != 0) {
			otrig = TRUE;
		}
		break;
	}

	if(otrig) {
		object[i]._oVar4 = 1;
		sx = object[i]._ox;
		sy = object[i]._oy;
		dx = object[oti]._ox;
		dy = object[oti]._oy;
		ax = dx;
		ay = dy;
		for(y = ay - 1; y <= ay + 1; y++) {
			for(x = ax - 1; x <= ax + 1; x++) {
				if(dPlayer[x][y] != 0) {
					dx = x;
					dy = y;
				}
			}
		}
		if(!deltaload) {
			dir = GetDirection(sx, sy, dx, dy);
			AddMissile(sx, sy, dx, dy, dir, object[i]._oVar3, 1, -1, 0, 0);
			PlaySfxLoc(IS_TRAP, object[oti]._ox, object[oti]._oy);
		}
		object[oti]._oTrapFlag = 0;
	}
}

void Obj_BCrossDamage(int i)
{
	int resist;
	int damage[4] = { 6, 8, 10, 12 };

	if(plr[myplr]._pmode == PM_DEATH) {
		return;
	}

	resist = plr[myplr]._pFireResist;
	if(resist > 0) {
		damage[leveltype - 1] -= resist * damage[leveltype - 1] / 100;
	}

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(plr[myplr].WorldX != object[i]._ox || plr[myplr].WorldY != object[i]._oy - 1) {
		return;
	}

	plr[myplr]._pHitPoints -= damage[leveltype - 1];
	plr[myplr]._pHPBase -= damage[leveltype - 1];

	if(plr[myplr]._pHitPoints >> 6 <= 0) {
		SyncPlrKill(myplr, 0);
	} else {
		if(plr[myplr]._pClass == PC_WARRIOR) {
			PlaySfxLoc(PS_WARR68, plr[myplr].WorldX, plr[myplr].WorldY);
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			PlaySfxLoc(PS_ROGUE68, plr[myplr].WorldX, plr[myplr].WorldY);
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			PlaySfxLoc(PS_MAGE68, plr[myplr].WorldX, plr[myplr].WorldY);
		}
	}

	drawhpflag = 1;
}

void ProcessObjects()
{
	int i, oi;

	for(i = 0; i < nobjects; i++) {
		oi = objectactive[i];
		/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
		switch(object[oi]._otype) {
		case OBJ_L1LIGHT:
			Obj_Light(oi, 10);
			break;
		case OBJ_SKFIRE:
		case OBJ_CANDLE2:
		case OBJ_BOOKCANDLE:
			Obj_Light(oi, 5);
			break;
		case OBJ_STORYCANDLE:
			Obj_Light(oi, 3);
			break;
		case OBJ_CRUX1:
		case OBJ_CRUX2:
		case OBJ_CRUX3:
		case OBJ_BARREL:
		case OBJ_BARRELEX:
		case OBJ_SHRINEL:
		case OBJ_SHRINER:
			Obj_StopAnim(oi);
			break;
		case OBJ_L1LDOOR:
		case OBJ_L1RDOOR:
		case OBJ_L2LDOOR:
		case OBJ_L2RDOOR:
		case OBJ_L3LDOOR:
		case OBJ_L3RDOOR:
			Obj_Door(oi);
			break;
		case OBJ_TORCHL:
		case OBJ_TORCHR:
		case OBJ_TORCHL2:
		case OBJ_TORCHR2:
			Obj_Light(oi, 8);
			break;
		case OBJ_SARC:
			Obj_Sarc(oi);
			break;
		case OBJ_FLAMEHOLE:
			Obj_FlameTrap(oi);
			break;
		case OBJ_TRAPL:
		case OBJ_TRAPR:
			Obj_Trap(oi);
			break;
		case OBJ_MCIRCLE1:
		case OBJ_MCIRCLE2:
			Obj_Circle(oi);
			break;
		case OBJ_BCROSS:
		case OBJ_TBCROSS:
			Obj_Light(oi, 10);
			Obj_BCrossDamage(oi);
			break;
		}
		if(object[oi]._oAnimFlag != 0) {
			object[oi]._oAnimCnt++;
			if(object[oi]._oAnimCnt >= object[oi]._oAnimDelay) {
				object[oi]._oAnimCnt = 0;
				object[oi]._oAnimFrame++;
				if(object[oi]._oAnimFrame > object[oi]._oAnimLen) {
					object[oi]._oAnimFrame = 1;
				}
			}
		}
	}

	i = 0;
	while(i < nobjects) {
		oi = objectactive[i];
		if(object[oi]._oDelFlag) {
			DeleteObject_(oi, i);
			i = 0;
		} else {
			i++;
		}
	}
}

void ObjSetMicro(int dx, int dy, int pn)
{
	int i;
	WORD *pPiece;
	MICROS *pMap;

	dPiece[dx][dy] = pn;
	pn--;
	pMap = &dpiece_defs_map_1[IsometricCoord(dx, dy)];

	if(leveltype != DTYPE_HELL) {
		pPiece = (WORD *)&pLevelPieces[20 * pn];
		for(i = 0; i < 10; i++) {
			pMap->mt[i] = pPiece[(i & 1) + 10 - 2 - (i & 0xE)];
		}
	} else {
		pPiece = (WORD *)&pLevelPieces[32 * pn];
		for(i = 0; i < 16; i++) {
			pMap->mt[i] = pPiece[(i & 1) + 16 - 2 - (i & 0xE)];
		}
	}
}

void objects_set_door_piece(int x, int y)
{
	long v1, v2, lv, lc, rc;

	lv = dPiece[x][y] - 1;

#ifdef USE_ASM
	__asm {
		mov		esi, pLevelPieces
		xor		eax, eax
		mov		ax, word ptr lv
		mov		ebx, 20
		mul		ebx
		add		esi, eax
		add		esi, 16
		xor		eax, eax
		lodsw
		mov		word ptr v1, ax
		lodsw
		mov		word ptr v2, ax
	}
#else
	v1 = ((WORD *)&pLevelPieces[20 * lv + 16])[0];
	v2 = ((WORD *)&pLevelPieces[20 * lv + 16])[1];
#endif

	lc = IsometricCoord(x, y);
	dpiece_defs_map_1[lc].mt[0] = v1;
	rc = IsometricCoord(x, y);
	dpiece_defs_map_1[rc].mt[1] = v2;
}

void ObjSetMini(int x, int y, int v)
{
	int xx, yy;
	long v1, v2, v3, v4;

#ifdef USE_ASM
	__asm {
		mov		esi, pMegaTiles
		xor		eax, eax
		mov		ax, word ptr v
		dec		eax
		shl		eax, 3
		add		esi, eax
		xor		eax, eax
		lodsw
		inc		eax
		mov		v1, eax
		lodsw
		inc		eax
		mov		v2, eax
		lodsw
		inc		eax
		mov		v3, eax
		lodsw
		inc		eax
		mov		v4, eax
	}
#else
	v--;
	v1 = ((WORD *)&pMegaTiles[v << 3])[0] + 1;
	v2 = ((WORD *)&pMegaTiles[v << 3])[1] + 1;
	v3 = ((WORD *)&pMegaTiles[v << 3])[2] + 1;
	v4 = ((WORD *)&pMegaTiles[v << 3])[3] + 1;
#endif

	xx = 2 * x + 16;
	yy = 2 * y + 16;
	ObjSetMicro(xx, yy, v1);
	ObjSetMicro(xx + 1, yy, v2);
	ObjSetMicro(xx, yy + 1, v3);
	ObjSetMicro(xx + 1, yy + 1, v4);
}

void ObjL1Special(int x1, int y1, int x2, int y2)
{
	int i, j;

	for(j = y1; j <= y2; j++) {
		for(i = x1; i <= x2; i++) {
			dSpecial[i][j] = 0;
			if(dPiece[i][j] == 12) {
				dSpecial[i][j] = 1;
			}
			if(dPiece[i][j] == 11) {
				dSpecial[i][j] = 2;
			}
			if(dPiece[i][j] == 71) {
				dSpecial[i][j] = 1;
			}
			if(dPiece[i][j] == 259) {
				dSpecial[i][j] = 5;
			}
			if(dPiece[i][j] == 249) {
				dSpecial[i][j] = 2;
			}
			if(dPiece[i][j] == 325) {
				dSpecial[i][j] = 2;
			}
			if(dPiece[i][j] == 321) {
				dSpecial[i][j] = 1;
			}
			if(dPiece[i][j] == 255) {
				dSpecial[i][j] = 4;
			}
			if(dPiece[i][j] == 211) {
				dSpecial[i][j] = 1;
			}
			if(dPiece[i][j] == 344) {
				dSpecial[i][j] = 2;
			}
			if(dPiece[i][j] == 341) {
				dSpecial[i][j] = 1;
			}
			if(dPiece[i][j] == 331) {
				dSpecial[i][j] = 2;
			}
			if(dPiece[i][j] == 418) {
				dSpecial[i][j] = 1;
			}
			if(dPiece[i][j] == 421) {
				dSpecial[i][j] = 2;
			}
		}
	}
}

void ObjL2Special(int x1, int y1, int x2, int y2)
{
	int i, j;

	for(j = y1; j <= y2; j++) {
		for(i = x1; i <= x2; i++) {
			dSpecial[i][j] = 0;
			if(dPiece[i][j] == 541) {
				dSpecial[i][j] = 5;
			}
			if(dPiece[i][j] == 178) {
				dSpecial[i][j] = 5;
			}
			if(dPiece[i][j] == 551) {
				dSpecial[i][j] = 5;
			}
			if(dPiece[i][j] == 542) {
				dSpecial[i][j] = 6;
			}
			if(dPiece[i][j] == 553) {
				dSpecial[i][j] = 6;
			}
			if(dPiece[i][j] == 13) {
				dSpecial[i][j] = 5;
			}
			if(dPiece[i][j] == 17) {
				dSpecial[i][j] = 6;
			}
		}
	}
	for(j = y1; j <= y2; j++) {
		for(i = x1; i <= x2; i++) {
			if(dPiece[i][j] == 132) {
				dSpecial[i][j + 1] = 2;
				dSpecial[i][j + 2] = 1;
			}
			if(dPiece[i][j] == 135 || dPiece[i][j] == 139) {
				dSpecial[i + 1][j] = 3;
				dSpecial[i + 2][j] = 4;
			}
		}
	}
}

void DoorSet(int oi, int dx, int dy)
{
	int pn;

	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);

	pn = dPiece[dx][dy];
	if(pn == 43) {
		ObjSetMicro(dx, dy, 392);
	}
	if(pn == 45) {
		ObjSetMicro(dx, dy, 394);
	}
	if(pn == 50 && object[oi]._otype == OBJ_L1LDOOR) {
		ObjSetMicro(dx, dy, 411);
	}
	if(pn == 50 && object[oi]._otype == OBJ_L1RDOOR) {
		ObjSetMicro(dx, dy, 412);
	}
	if(pn == 54) {
		ObjSetMicro(dx, dy, 397);
	}
	if(pn == 55) {
		ObjSetMicro(dx, dy, 398);
	}
	if(pn == 61) {
		ObjSetMicro(dx, dy, 399);
	}
	if(pn == 67) {
		ObjSetMicro(dx, dy, 400);
	}
	if(pn == 68) {
		ObjSetMicro(dx, dy, 401);
	}
	if(pn == 69) {
		ObjSetMicro(dx, dy, 403);
	}
	if(pn == 70) {
		ObjSetMicro(dx, dy, 404);
	}
	if(pn == 72) {
		ObjSetMicro(dx, dy, 406);
	}
	if(pn == 212) {
		ObjSetMicro(dx, dy, 407);
	}
	if(pn == 354) {
		ObjSetMicro(dx, dy, 409);
	}
	if(pn == 355) {
		ObjSetMicro(dx, dy, 410);
	}
	if(pn == 411) {
		ObjSetMicro(dx, dy, 396);
	}
	if(pn == 412) {
		ObjSetMicro(dx, dy, 396);
	}
}

void RedoPlayerVision()
{
	int p;

	for(p = 0; p < MAX_PLRS; p++) {
		if(plr[p].plractive && currlevel == plr[p].plrlevel) {
			ChangeVisionXY(plr[p]._pvid, plr[p].WorldX, plr[p].WorldY);
		}
	}
}

void OperateL1RDoor(int pnum, int oi, BOOL sendflag)
{
	int dx, dy;
	BOOL dok;

	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
	if(object[oi]._oVar4 == 2) {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		return;
	}

	dx = object[oi]._ox;
	dy = object[oi]._oy;
	if(object[oi]._oVar4 == 0) {
		if(pnum == myplr && sendflag) {
			NetSendCmdParam1(TRUE, CMD_OPENDOOR, oi);
		}
		if(!deltaload) {
			PlaySfxLoc(IS_DOOROPEN, object[oi]._ox, object[oi]._oy);
		}
		ObjSetMicro(dx, dy, 395);
		dSpecial[dx][dy] = 8;
		objects_set_door_piece(dx, dy - 1);
		dx--;
		object[oi]._oAnimFrame += 2;
		object[oi]._oPreFlag = 1;
		DoorSet(oi, dx, dy);
		object[oi]._oVar4 = 1;
		object[oi]._oSelFlag = 2;
		RedoPlayerVision();
	} else {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		dok = dMonster[dx][dy] == 0;
		dok &= dItem[dx][dy] == 0;
		dok &= dDead[dx][dy] == 0;
		if(dok) {
			if(pnum == myplr && sendflag) {
				NetSendCmdParam1(TRUE, CMD_CLOSEDOOR, oi);
			}
			object[oi]._oVar4 = 0;
			object[oi]._oSelFlag = 3;
			ObjSetMicro(dx, dy, object[oi]._oVar1);
			if(object[oi]._oVar2 != 50) {
				ObjSetMicro(dx - 1, dy, object[oi]._oVar2);
			} else if(dPiece[dx - 1][dy] == 396) {
				ObjSetMicro(dx - 1, dy, 411);
			} else {
				ObjSetMicro(dx - 1, dy, object[oi]._oVar2);
			}
			object[oi]._oAnimFrame -= 2;
			object[oi]._oPreFlag = 0;
			RedoPlayerVision();
		} else {
			object[oi]._oVar4 = 2;
		}
	}
}

void OperateL1LDoor(int pnum, int oi, BOOL sendflag)
{
	int dx, dy;
	BOOL dok;

	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
	if(object[oi]._oVar4 == 2) {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		return;
	}

	dx = object[oi]._ox;
	dy = object[oi]._oy;
	if(object[oi]._oVar4 == 0) {
		if(pnum == myplr && sendflag) {
			NetSendCmdParam1(TRUE, CMD_OPENDOOR, oi);
		}
		if(!deltaload) {
			PlaySfxLoc(IS_DOOROPEN, object[oi]._ox, object[oi]._oy);
		}
		if(object[oi]._oVar1 == 214) {
			ObjSetMicro(dx, dy, 408);
		} else {
			ObjSetMicro(dx, dy, 393);
		}
		dSpecial[dx][dy] = 7;
		objects_set_door_piece(dx - 1, dy);
		dy--;
		object[oi]._oAnimFrame += 2;
		object[oi]._oPreFlag = 1;
		DoorSet(oi, dx, dy);
		object[oi]._oVar4 = 1;
		object[oi]._oSelFlag = 2;
		RedoPlayerVision();
	} else {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		dok = dMonster[dx][dy] == 0;
		dok &= dItem[dx][dy] == 0;
		dok &= dDead[dx][dy] == 0;
		if(dok) {
			if(pnum == myplr && sendflag) {
				NetSendCmdParam1(TRUE, CMD_CLOSEDOOR, oi);
			}
			object[oi]._oVar4 = 0;
			object[oi]._oSelFlag = 3;
			ObjSetMicro(dx, dy, object[oi]._oVar1);
			if(object[oi]._oVar2 != 50) {
				ObjSetMicro(dx, dy - 1, object[oi]._oVar2);
			} else if(dPiece[dx][dy - 1] == 396) {
				ObjSetMicro(dx, dy - 1, 412);
			} else {
				ObjSetMicro(dx, dy - 1, object[oi]._oVar2);
			}
			object[oi]._oAnimFrame -= 2;
			object[oi]._oPreFlag = 0;
			RedoPlayerVision();
		} else {
			object[oi]._oVar4 = 2;
		}
	}
}

void OperateL2RDoor(int pnum, int oi, BOOL sendflag)
{
	int dx, dy;
	BOOL dok;

	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
	if(object[oi]._oVar4 == 2) {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		return;
	}

	dx = object[oi]._ox;
	dy = object[oi]._oy;
	if(object[oi]._oVar4 == 0) {
		if(pnum == myplr && sendflag) {
			NetSendCmdParam1(TRUE, CMD_OPENDOOR, oi);
		}
		if(!deltaload) {
			PlaySfxLoc(IS_DOOROPEN, object[oi]._ox, object[oi]._oy);
		}
		ObjSetMicro(dx, dy, 17);
		object[oi]._oAnimFrame += 2;
		object[oi]._oPreFlag = 1;
		object[oi]._oVar4 = 1;
		object[oi]._oSelFlag = 2;
		RedoPlayerVision();
	} else {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		dok = dMonster[dx][dy] == 0;
		dok &= dItem[dx][dy] == 0;
		dok &= dDead[dx][dy] == 0;
		if(dok) {
			if(pnum == myplr && sendflag) {
				NetSendCmdParam1(TRUE, CMD_CLOSEDOOR, oi);
			}
			object[oi]._oVar4 = 0;
			object[oi]._oSelFlag = 3;
			ObjSetMicro(dx, dy, 540);
			object[oi]._oAnimFrame -= 2;
			object[oi]._oPreFlag = 0;
			RedoPlayerVision();
		} else {
			object[oi]._oVar4 = 2;
		}
	}
}

void OperateL2LDoor(int pnum, int oi, BOOL sendflag)
{
	int dx, dy;
	BOOL dok;

	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
	if(object[oi]._oVar4 == 2) {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		return;
	}

	dx = object[oi]._ox;
	dy = object[oi]._oy;
	if(object[oi]._oVar4 == 0) {
		if(pnum == myplr && sendflag) {
			NetSendCmdParam1(TRUE, CMD_OPENDOOR, oi);
		}
		if(!deltaload) {
			PlaySfxLoc(IS_DOOROPEN, object[oi]._ox, object[oi]._oy);
		}
		ObjSetMicro(dx, dy, 13);
		object[oi]._oAnimFrame += 2;
		object[oi]._oPreFlag = 1;
		object[oi]._oVar4 = 1;
		object[oi]._oSelFlag = 2;
		RedoPlayerVision();
	} else {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		dok = dMonster[dx][dy] == 0;
		dok &= dItem[dx][dy] == 0;
		dok &= dDead[dx][dy] == 0;
		if(dok) {
			if(pnum == myplr && sendflag) {
				NetSendCmdParam1(TRUE, CMD_CLOSEDOOR, oi);
			}
			object[oi]._oVar4 = 0;
			object[oi]._oSelFlag = 3;
			ObjSetMicro(dx, dy, 538);
			object[oi]._oAnimFrame -= 2;
			object[oi]._oPreFlag = 0;
			RedoPlayerVision();
		} else {
			object[oi]._oVar4 = 2;
		}
	}
}

void OperateL3RDoor(int pnum, int oi, BOOL sendflag)
{
	int dx, dy;
	BOOL dok;

	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
	if(object[oi]._oVar4 == 2) {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		return;
	}

	dx = object[oi]._ox;
	dy = object[oi]._oy;
	if(object[oi]._oVar4 == 0) {
		if(pnum == myplr && sendflag) {
			NetSendCmdParam1(TRUE, CMD_OPENDOOR, oi);
		}
		if(!deltaload) {
			PlaySfxLoc(IS_DOOROPEN, object[oi]._ox, object[oi]._oy);
		}
		ObjSetMicro(dx, dy, 541);
		object[oi]._oAnimFrame += 2;
		object[oi]._oPreFlag = 1;
		object[oi]._oVar4 = 1;
		object[oi]._oSelFlag = 2;
		RedoPlayerVision();
	} else {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		dok = dMonster[dx][dy] == 0;
		dok &= dItem[dx][dy] == 0;
		dok &= dDead[dx][dy] == 0;
		if(dok) {
			if(pnum == myplr && sendflag) {
				NetSendCmdParam1(TRUE, CMD_CLOSEDOOR, oi);
			}
			object[oi]._oVar4 = 0;
			object[oi]._oSelFlag = 3;
			ObjSetMicro(dx, dy, 534);
			object[oi]._oAnimFrame -= 2;
			object[oi]._oPreFlag = 0;
			RedoPlayerVision();
		} else {
			object[oi]._oVar4 = 2;
		}
	}
}

void OperateL3LDoor(int pnum, int oi, BOOL sendflag)
{
	int dx, dy;
	BOOL dok;

	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
	if(object[oi]._oVar4 == 2) {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		return;
	}

	dx = object[oi]._ox;
	dy = object[oi]._oy;
	if(object[oi]._oVar4 == 0) {
		if(pnum == myplr && sendflag) {
			NetSendCmdParam1(TRUE, CMD_OPENDOOR, oi);
		}
		if(!deltaload) {
			PlaySfxLoc(IS_DOOROPEN, object[oi]._ox, object[oi]._oy);
		}
		ObjSetMicro(dx, dy, 538);
		object[oi]._oAnimFrame += 2;
		object[oi]._oPreFlag = 1;
		object[oi]._oVar4 = 1;
		object[oi]._oSelFlag = 2;
		RedoPlayerVision();
	} else {
		if(!deltaload) {
			PlaySfxLoc(IS_DOORCLOS, object[oi]._ox, object[oi]._oy);
		}
		dok = dMonster[dx][dy] == 0;
		dok &= dItem[dx][dy] == 0;
		dok &= dDead[dx][dy] == 0;
		if(dok) {
			if(pnum == myplr && sendflag) {
				NetSendCmdParam1(TRUE, CMD_CLOSEDOOR, oi);
			}
			object[oi]._oVar4 = 0;
			object[oi]._oSelFlag = 3;
			ObjSetMicro(dx, dy, 531);
			object[oi]._oAnimFrame -= 2;
			object[oi]._oPreFlag = 0;
			RedoPlayerVision();
		} else {
			object[oi]._oVar4 = 2;
		}
	}
}

void MonstCheckDoors(int m)
{
	int i, oi, dpx, dpy, mx, my;

	mx = monster[m]._mx;
	my = monster[m]._my;

	if(dObject[mx - 1][my - 1] == 0
	&& dObject[mx][my - 1] == 0
	&& dObject[mx + 1][my - 1] == 0
	&& dObject[mx - 1][my] == 0
	&& dObject[mx + 1][my] == 0
	&& dObject[mx - 1][my + 1] == 0
	&& dObject[mx][my + 1] == 0
	&& dObject[mx + 1][my + 1] == 0) {
		return;
	}

	for(i = 0; i < nobjects; i++) {
		oi = objectactive[i];
		/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
		if((object[oi]._otype == OBJ_L1LDOOR || object[oi]._otype == OBJ_L1RDOOR) && object[oi]._oVar4 == 0) {
			dpx = abs(object[oi]._ox - mx);
			dpy = abs(object[oi]._oy - my);
			if(dpx == 1 && dpy <= 1 && object[oi]._otype == OBJ_L1LDOOR) {
				OperateL1LDoor(myplr, oi, TRUE);
			}
			if(dpx <= 1 && dpy == 1 && object[oi]._otype == OBJ_L1RDOOR) {
				OperateL1RDoor(myplr, oi, TRUE);
			}
		}
		if((object[oi]._otype == OBJ_L2LDOOR || object[oi]._otype == OBJ_L2RDOOR) && object[oi]._oVar4 == 0) {
			dpx = abs(object[oi]._ox - mx);
			dpy = abs(object[oi]._oy - my);
			if(dpx == 1 && dpy <= 1 && object[oi]._otype == OBJ_L2LDOOR) {
				OperateL2LDoor(myplr, oi, TRUE);
			}
			if(dpx <= 1 && dpy == 1 && object[oi]._otype == OBJ_L2RDOOR) {
				OperateL2RDoor(myplr, oi, TRUE);
			}
		}
		if((object[oi]._otype == OBJ_L3LDOOR || object[oi]._otype == OBJ_L3RDOOR) && object[oi]._oVar4 == 0) {
			dpx = abs(object[oi]._ox - mx);
			dpy = abs(object[oi]._oy - my);
			if(dpx == 1 && dpy <= 1 && object[oi]._otype == OBJ_L3RDOOR) {
				OperateL3RDoor(myplr, oi, TRUE);
			}
			if(dpx <= 1 && dpy == 1 && object[oi]._otype == OBJ_L3LDOOR) {
				OperateL3LDoor(myplr, oi, TRUE);
			}
		}
	}
}

void ObjChangeMap(int x1, int y1, int x2, int y2)
{
	int i, j;

	for(j = y1; j <= y2; j++) {
		for(i = x1; i <= x2; i++) {
			ObjSetMini(i, j, pdungeon[i][j]);
			dungeon[i][j] = pdungeon[i][j];
		}
	}

	if(leveltype == DTYPE_CATHEDRAL) {
		ObjL1Special(2 * x1 + 16, 2 * y1 + 16, 2 * x2 + 17, 2 * y2 + 17);
		AddL1Objs(2 * x1 + 16, 2 * y1 + 16, 2 * x2 + 17, 2 * y2 + 17);
	}
	if(leveltype == DTYPE_CATACOMBS) {
		ObjL2Special(2 * x1 + 16, 2 * y1 + 16, 2 * x2 + 17, 2 * y2 + 17);
		AddL2Objs(2 * x1 + 16, 2 * y1 + 16, 2 * x2 + 17, 2 * y2 + 17);
	}
}

void ObjChangeMapResync(int x1, int y1, int x2, int y2)
{
	int i, j;

	for(j = y1; j <= y2; j++) {
		for(i = x1; i <= x2; i++) {
			ObjSetMini(i, j, pdungeon[i][j]);
			dungeon[i][j] = pdungeon[i][j];
		}
	}

	if(leveltype == DTYPE_CATHEDRAL) {
		ObjL1Special(2 * x1 + 16, 2 * y1 + 16, 2 * x2 + 17, 2 * y2 + 17);
	}
	if(leveltype == DTYPE_CATACOMBS) {
		ObjL2Special(2 * x1 + 16, 2 * y1 + 16, 2 * x2 + 17, 2 * y2 + 17);
	}
}

void OperateL1Door(int pnum, int i, BOOL sendflag)
{
	int dpx, dpy;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	dpx = abs(object[i]._ox - plr[pnum].WorldX);
	dpy = abs(object[i]._oy - plr[pnum].WorldY);
	if(dpx == 1 && dpy <= 1 && object[i]._otype == OBJ_L1LDOOR) {
		OperateL1LDoor(pnum, i, sendflag);
	}
	if(dpx <= 1 && dpy == 1 && object[i]._otype == OBJ_L1RDOOR) {
		OperateL1RDoor(pnum, i, sendflag);
	}
}

void OperateLever(int pnum, int i)
{
	int j, oi, ot;
	BOOL mapflag;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}

	if(!deltaload) {
		PlaySfxLoc(IS_LEVER, object[i]._ox, object[i]._oy);
	}

	object[i]._oSelFlag = 0;
	object[i]._oAnimFrame++;

	mapflag = TRUE;
	if(currlevel == 16) {
		for(j = 0; j < nobjects; j++) {
			oi = objectactive[j];
			ot = object[oi]._otype;
			if(ot == OBJ_SWITCHSKL && object[i]._oVar8 == object[oi]._oVar8 && object[oi]._oSelFlag != 0) {
				mapflag = FALSE;
			}
		}
	}

	if(mapflag) {
		ObjChangeMap(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
	}
	if(pnum == myplr) {
		NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
	}
}

void OperateBook(int pnum, int i)
{
	int v1, v2, j, oi, ot /* itm */;
	BOOL found, dowarp;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}

	if(setlevel && setlvlnum == SL_VILEBETRAYER) {
		found = FALSE;
		dowarp = FALSE;
		for(j = 0; j < nobjects; j++) {
			oi = objectactive[j];
			ot = object[oi]._otype;
			if(ot == OBJ_MCIRCLE2 && object[oi]._oVar6 == 1) {
				v1 = 27;
				v2 = 29;
				object[oi]._oVar6 = 4;
				dowarp = TRUE;
			}
			if(ot == OBJ_MCIRCLE2 && object[oi]._oVar6 == 2) {
				v1 = 43;
				v2 = 29;
				object[oi]._oVar6 = 4;
				dowarp = TRUE;
			}
			if(dowarp) {
				object[dObject[35][36] - 1]._oVar5++;
				AddMissile(plr[pnum].WorldX, plr[pnum].WorldY, v1, v2, plr[pnum]._pdir, MIS_RNDTELEPORT, 0, pnum, 0, 0);
				found = TRUE;
				dowarp = FALSE;
			}
		}
		if(!found) {
			return;
		}
	}

	object[i]._oSelFlag = 0;
	object[i]._oAnimFrame++;

	if(setlevel && setlvlnum == SL_BONECHAMB) {
		plr[myplr]._pMemSpells64 |= (__int64)1 << (SPL_GUARDIAN - 1); /// BUGFIX: `pnum` instead of `myplr` here and below
		if(plr[pnum]._pSplLvl[SPL_GUARDIAN] < 15) {
			plr[myplr]._pSplLvl[SPL_GUARDIAN]++;
		}
		quests[Q_SCHAMB]._qactive = 3;
		if(!deltaload) {
			PlaySfxLoc(IS_QUESTDN, object[i]._ox, object[i]._oy);
		}
		InitDiabloMsg(43);
		AddMissile(plr[myplr].WorldX, plr[myplr].WorldY, object[i]._ox - 2, object[i]._oy - 4, plr[myplr]._pdir, MIS_GUARDIAN, 0, myplr, 0, 0);
	}
	if(setlevel && setlvlnum == SL_VILEBETRAYER) {
		ObjChangeMapResync(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
		for(j = 0; j < nobjects; j++) {
			SyncObjectAnim(objectactive[j]);
		}
	}
}

void OperateBookLever(int pnum, int i)
{
	int x, y, tren;

	/// ASSERT: assert(gbMaxPlayers == 1);
	x = 2 * setpc_x + 16;
	y = 2 * setpc_y + 16;
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}
	if(qtextflag) {
		return;
	}

	if(object[i]._otype == OBJ_BLINDBOOK && quests[Q_BLIND]._qvar1 == 0) {
		quests[Q_BLIND]._qactive = 2;
		quests[Q_BLIND]._qlog = 1;
		quests[Q_BLIND]._qvar1 = 1;
	}
	if(object[i]._otype == OBJ_BLOODBOOK && quests[Q_BLOOD]._qvar1 == 0) {
		quests[Q_BLOOD]._qactive = 2;
		quests[Q_BLOOD]._qlog = 1;
		quests[Q_BLOOD]._qvar1 = 1;
		SpawnQuestItem(IDI_BLDSTONE, 2 * setpc_x + 19, 2 * setpc_y + 26, 0, 1);
		SpawnQuestItem(IDI_BLDSTONE, 2 * setpc_x + 31, 2 * setpc_y + 26, 0, 1);
		SpawnQuestItem(IDI_BLDSTONE, 2 * setpc_x + 25, 2 * setpc_y + 33, 0, 1);
	}
	if(object[i]._otype == OBJ_STEELTOME && quests[Q_WARLORD]._qvar1 == 0) {
		quests[Q_WARLORD]._qactive = 2;
		quests[Q_WARLORD]._qlog = 1;
		quests[Q_WARLORD]._qvar1 = 1;
	}
	if(object[i]._oAnimFrame != object[i]._oVar6) {
		if(object[i]._otype != OBJ_BLOODBOOK) {
			ObjChangeMap(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
		}
		if(object[i]._otype == OBJ_BLINDBOOK) {
			CreateItem(UITEM_OPTAMULET, x + 5, y + 5);
			tren = TransVal;
			TransVal = 9;
			DRLG_MRectTrans(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
			TransVal = tren;
		}
	}

	object[i]._oAnimFrame = object[i]._oVar6;
	InitQTextMsg(object[i]._oVar7);

	if(pnum == myplr) {
		NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
	}
}

void OperateSChambBk(int pnum, int i)
{
	int textdef, j;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}
	if(qtextflag) {
		return;
	}

	if(object[i]._oAnimFrame != object[i]._oVar6) {
		ObjChangeMapResync(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
		for(j = 0; j < nobjects; j++) {
			SyncObjectAnim(objectactive[j]);
		}
	}
	object[i]._oAnimFrame = object[i]._oVar6;

	if(quests[Q_SCHAMB]._qactive == 1) {
		quests[Q_SCHAMB]._qactive = 2;
		quests[Q_SCHAMB]._qlog = 1;
	}

	if(plr[myplr]._pClass == PC_WARRIOR) { /// BUGFIX: `pnum` instead of `myplr` here and below
		textdef = TEXT_BONER;
	} else if(plr[myplr]._pClass == PC_ROGUE) {
		textdef = TEXT_RBONER;
	} else if(plr[myplr]._pClass == PC_SORCERER) {
		textdef = TEXT_MBONER;
	}
	quests[Q_SCHAMB]._qmsg = textdef;
	InitQTextMsg(textdef);
}

void OperateChest(int pnum, int i, BOOL sendmsg)
{
	int j, mdir, mtype;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}

	if(!deltaload) {
		PlaySfxLoc(IS_CHEST, object[i]._ox, object[i]._oy);
	}

	object[i]._oSelFlag = 0;
	object[i]._oAnimFrame += 2;

	if(deltaload) {
		return;
	}

	SetRndSeed(object[i]._oRndSeed);

	if(setlevel) {
		for(j = 0; j < object[i]._oVar1; j++) {
			CreateRndItem(object[i]._ox, object[i]._oy, TRUE, sendmsg, FALSE);
		}
	} else {
		for(j = 0; j < object[i]._oVar1; j++) {
			if(object[i]._oVar2 != 0) {
				CreateRndItem(object[i]._ox, object[i]._oy, FALSE, sendmsg, FALSE);
			} else {
				CreateRndUseful(pnum, object[i]._ox, object[i]._oy, sendmsg);
			}
		}
	}

	if(object[i]._oTrapFlag && object[i]._otype >= OBJ_TCHEST1 && object[i]._otype <= OBJ_TCHEST3) {
		mdir = GetDirection(object[i]._ox, object[i]._oy, plr[pnum].WorldX, plr[pnum].WorldY);
		switch(object[i]._oVar4) {
		case 0:
			mtype = MIS_ARROW;
			break;
		case 1:
			mtype = MIS_FARROW;
			break;
		case 2:
			mtype = MIS_NOVA;
			break;
		}
		AddMissile(object[i]._ox, object[i]._oy, plr[pnum].WorldX, plr[pnum].WorldY, mdir, mtype, 1, -1, 0, 0);
		object[i]._oTrapFlag = 0;
	}
	if(pnum == myplr) {
		NetSendCmdParam2(FALSE, CMD_PLROPOBJ, pnum, i);
	}
}

void OperateMushPatch(int pnum, int i)
{
	int x, y;

	if(quests[Q_MUSHROOM]._qactive != 2 || quests[Q_MUSHROOM]._qvar1 < 2) {
		if(deltaload || pnum != myplr) {
			return;
		}
		if(plr[myplr]._pClass == PC_WARRIOR) {
			PlaySFX(PS_WARR13);
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			PlaySFX(PS_ROGUE13);
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			PlaySFX(PS_MAGE13);
		}
	} else {
		/// ASSERT: assert((DWORD)i < MAXOBJECTS);
		if(object[i]._oSelFlag == 0) {
			return;
		}
		if(!deltaload) {
			PlaySfxLoc(IS_CHEST, object[i]._ox, object[i]._oy);
		}
		object[i]._oSelFlag = 0;
		object[i]._oAnimFrame++;
		if(deltaload) {
			return;
		}
		GetSuperItemLoc(object[i]._ox, object[i]._oy, x, y);
		SpawnQuestItem(IDI_MUSHROOM, x, y, 0, 0);
		quests[Q_MUSHROOM]._qvar1 = 3;
	}
}

void OperateInnSignChest(int pnum, int i)
{
	int x, y;

	if(quests[Q_LTBANNER]._qvar1 != 2) {
		if(deltaload || pnum != myplr) {
			return;
		}
		if(plr[myplr]._pClass == PC_WARRIOR) {
			PlaySFX(PS_WARR24);
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			PlaySFX(PS_ROGUE24);
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			PlaySFX(PS_MAGE24);
		}
	} else {
		if(object[i]._oSelFlag == 0) {
			return;
		}
		if(!deltaload) {
			PlaySfxLoc(IS_CHEST, object[i]._ox, object[i]._oy);
		}
		object[i]._oSelFlag = 0;
		object[i]._oAnimFrame += 2;
		if(deltaload) {
			return;
		}
		GetSuperItemLoc(object[i]._ox, object[i]._oy, x, y);
		SpawnQuestItem(IDI_BANNER, x, y, 0, 0);
	}
}

void OperateSlainHero(int pnum, int i, BOOL sendmsg)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}

	object[i]._oSelFlag = 0;

	if(deltaload) {
		return;
	}

	if(plr[pnum]._pClass == PC_WARRIOR) {
		CreateMagicArmor(object[i]._ox, object[i]._oy, ITYPE_HARMOR, 153, FALSE, TRUE);
		PlaySfxLoc(PS_WARR9, plr[myplr].WorldX, plr[myplr].WorldY);
	} else if(plr[pnum]._pClass == PC_ROGUE) {
		CreateMagicWeapon(object[i]._ox, object[i]._oy, ITYPE_BOW, 119, FALSE, TRUE);
		PlaySfxLoc(PS_ROGUE9, plr[myplr].WorldX, plr[myplr].WorldY);
	} else if(plr[pnum]._pClass == PC_SORCERER) {
		CreateSpellBook(object[i]._ox, object[i]._oy, SPL_LIGHTNING, FALSE, TRUE);
		PlaySfxLoc(PS_MAGE9, plr[myplr].WorldX, plr[myplr].WorldY);
	}

	if(pnum == myplr) {
		NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
	}
}

void OperateTrapLvr(int i)
{
	int j, oi;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oAnimFrame == 1) {
		object[i]._oAnimFrame++;
		for(j = 0; j < nobjects; j++) {
			oi = objectactive[j];
			if(object[oi]._otype == object[i]._oVar2 && object[oi]._oVar1 == object[i]._oVar1) {
				object[oi]._oVar2 = 1;
				object[oi]._oAnimFlag = 0;
			}
		}
	} else {
		object[i]._oAnimFrame--;
		for(j = 0; j < nobjects; j++) {
			oi = objectactive[j];
			if(object[oi]._otype == object[i]._oVar2 && object[oi]._oVar1 == object[i]._oVar1) {
				object[oi]._oVar2 = 0;
				if(object[oi]._oVar4 != 0) {
					object[oi]._oAnimFlag = 1;
				}
			}
		}
	}
}

void OperateSarc(int pnum, int i, BOOL sendmsg)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}

	if(!deltaload) {
		PlaySfxLoc(IS_SARC, object[i]._ox, object[i]._oy);
	}

	object[i]._oSelFlag = 0;

	if(deltaload) {
		object[i]._oAnimFrame = object[i]._oAnimLen;
		return;
	}

	object[i]._oAnimFlag = 1;
	object[i]._oAnimDelay = 3;
	SetRndSeed(object[i]._oRndSeed);

	if(object[i]._oVar1 <= 2) {
		CreateRndItem(object[i]._ox, object[i]._oy, FALSE, sendmsg, FALSE);
	}
	if(object[i]._oVar1 >= 8) {
		SpawnSkeleton(object[i]._oVar2, object[i]._ox, object[i]._oy);
	}
	if(pnum == myplr) {
		NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
	}
}

void OperateL2Door(int pnum, int i, BOOL sendflag)
{
	int dpx, dpy;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	dpx = abs(object[i]._ox - plr[pnum].WorldX);
	dpy = abs(object[i]._oy - plr[pnum].WorldY);
	if(dpx == 1 && dpy <= 1 && object[i]._otype == OBJ_L2LDOOR) {
		OperateL2LDoor(pnum, i, sendflag);
	}
	if(dpx <= 1 && dpy == 1 && object[i]._otype == OBJ_L2RDOOR) {
		OperateL2RDoor(pnum, i, sendflag);
	}
}

void OperateL3Door(int pnum, int i, BOOL sendflag)
{
	int dpx, dpy;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	dpx = abs(object[i]._ox - plr[pnum].WorldX);
	dpy = abs(object[i]._oy - plr[pnum].WorldY);
	if(dpx == 1 && dpy <= 1 && object[i]._otype == OBJ_L3RDOOR) {
		OperateL3RDoor(pnum, i, sendflag);
	}
	if(dpx <= 1 && dpy == 1 && object[i]._otype == OBJ_L3LDOOR) {
		OperateL3LDoor(pnum, i, sendflag);
	}
}

void OperatePedistal(int pnum, int i)
{
	int jstn;
	BYTE *setp;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oVar6 == 3) {
		return;
	}

	if(PlrHasItem(pnum, IDI_BLDSTONE, jstn) != NULL) {
		RemoveInvItem(pnum, jstn);
		object[i]._oAnimFrame++;
		object[i]._oVar6++;
	}
	if(object[i]._oVar6 == 1) {
		if(!deltaload) {
			PlaySfxLoc(LS_PUDDLE, object[i]._ox, object[i]._oy);
		}
		ObjChangeMap(setpc_x, setpc_y + 3, setpc_x + 2, setpc_y + 7);
	}
	if(object[i]._oVar6 == 2) {
		if(!deltaload) {
			PlaySfxLoc(LS_PUDDLE, object[i]._ox, object[i]._oy);
		}
		ObjChangeMap(setpc_x + 6, setpc_y + 3, setpc_x + setpc_w, setpc_y + 7);
	}
	if(object[i]._oVar6 == 3) {
		if(!deltaload) {
			PlaySfxLoc(LS_BLODSTAR, object[i]._ox, object[i]._oy);
		}
		ObjChangeMap(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
		setp = DiabLoad("Levels\\L2Data\\Blood2.DUN", NULL, 'STPC');
		LoadMapObjs(setp, 2 * setpc_x, 2 * setpc_y);
		MemFreeDbg(setp);
		CreateItem(UITEM_ARMOFVAL, 2 * setpc_x + 25, 2 * setpc_y + 19);
		object[i]._oSelFlag = 0;
	}
}

void TryDisarm(int pnum, int i)
{
	int j, oi, oti, trapdisper;
	BOOL checkflag;

	if(pnum == myplr) {
		SetCursor_(CURSOR_HAND);
	}

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(!object[i]._oTrapFlag) {
		return;
	}

	trapdisper = 2 * plr[pnum]._pDexterity - 5 * currlevel;
	if(random(154, 100) > trapdisper) {
		return;
	}

	for(j = 0; j < nobjects; j++) {
		oi = objectactive[j];
		checkflag = FALSE;
		if(object[oi]._otype == OBJ_TRAPL) {
			checkflag = TRUE;
		}
		if(object[oi]._otype == OBJ_TRAPR) {
			checkflag = TRUE;
		}
		if(checkflag) {
			oti = dObject[object[oi]._oVar1][object[oi]._oVar2] - 1;
			if(oti == i) {
				object[oi]._oVar4 = 1;
				object[i]._oTrapFlag = 0;
			}
		}
	}

	if(object[i]._otype >= OBJ_TCHEST1 && object[i]._otype <= OBJ_TCHEST3) {
		object[i]._oTrapFlag = 0;
	}
}

int ItemMiscIdIdx(int imiscid)
{
	int i;

	i = 0;
	while(AllItemsList[i].iRnd == IDROP_NEVER || AllItemsList[i].iMiscId != imiscid) {
		i++;
	}

	return i;
}

void OperateShrine(int pnum, int i, int sType)
{
	int v1, v2, v3, v4;
	int v12; // edx
	int v21; // eax
	int v60; // ebx
	int j; // edi
	int v72; // edi
	int v88; // ebx
	int v107; // ST38_4
	int v108; // ST34_4
	int v133; // eax
	int xx, yy;
	int min, max;
	unsigned __int64 spell, spells;

	if(dropGoldFlag) {
		dropGoldFlag = 0;
		dropGoldValue = 0;
	}

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);

	if(!object[i]._oSelFlag)
		return;

	SetRndSeed(object[i]._oRndSeed);
	object[i]._oSelFlag = 0;

	if(!deltaload) {
		PlaySfxLoc(sType, object[i]._ox, object[i]._oy);
		object[i]._oAnimFlag = 1;
		object[i]._oAnimDelay = 1;
	} else {
		object[i]._oAnimFlag = 0;
		object[i]._oAnimFrame = object[i]._oAnimLen;
	}

	switch(object[i]._oVar1) {
	case 0:
		if(deltaload || pnum != myplr)
			return;
		ModifyPlrStr(pnum, -1);
		ModifyPlrMag(pnum, -1);
		ModifyPlrDex(pnum, -1);
		ModifyPlrVit(pnum, -1);
		switch(random(0, 4)) {
		case 0:
			ModifyPlrStr(pnum, 6);
			break;
		case 1:
			ModifyPlrMag(pnum, 6);
			break;
		case 2:
			ModifyPlrDex(pnum, 6);
			break;
		case 3:
			ModifyPlrVit(pnum, 6);
			break;
		}
		CheckStats(pnum);
		InitDiabloMsg(12);
		break;
	case 1:
		v12 = 0;
		if(deltaload || pnum != myplr)
			return;
		for(j = 0; j < 7; j++) {
			if(plr[pnum].InvBody[j]._itype != -1)
				v12++;
		}
		if(v12 > 0) {
			for(j = 0; j < 7; j++) {
				if(plr[pnum].InvBody[j]._itype != -1
				&& plr[pnum].InvBody[j]._iMaxDur != 255
				&& plr[pnum].InvBody[j]._iMaxDur)
				{
					plr[pnum].InvBody[j]._iDurability += 10;
					plr[pnum].InvBody[j]._iMaxDur += 10;
					if(plr[pnum].InvBody[j]._iDurability > plr[pnum].InvBody[j]._iMaxDur)
						plr[pnum].InvBody[j]._iDurability = plr[pnum].InvBody[j]._iMaxDur;
				}
			}
			while(1) {
				v12 = 0;
				for(j = 0; j < 7; j++) {
					if(plr[pnum].InvBody[j]._itype != -1
					&& plr[pnum].InvBody[j]._iMaxDur != 255
					&& plr[pnum].InvBody[j]._iMaxDur)
						v12++;
				}
				if(v12 == 0)
					break;
				v21 = random(0, 7);
				if(plr[pnum].InvBody[v21]._itype == -1 || plr[pnum].InvBody[v21]._iMaxDur == 255 || !plr[pnum].InvBody[v21]._iMaxDur)
					continue;
				plr[pnum].InvBody[v21]._iDurability -= 20;
				plr[pnum].InvBody[v21]._iMaxDur -= 20;
				if(plr[pnum].InvBody[v21]._iDurability <= 0)
					plr[pnum].InvBody[v21]._iDurability = 1;
				if(plr[pnum].InvBody[v21]._iMaxDur <= 0)
					plr[pnum].InvBody[v21]._iMaxDur = 1;
				break;
			}
		}
		InitDiabloMsg(13);
		break;
	case 2:
		if(deltaload)
			return;
		if(pnum == myplr) {
			if(plr[pnum].InvBody[0]._itype != ITYPE_NONE)
				plr[pnum].InvBody[0]._iAC += 2;
			if(plr[pnum].InvBody[6]._itype != ITYPE_NONE)
				plr[pnum].InvBody[6]._iAC += 2;
			if(plr[pnum].InvBody[4]._itype != ITYPE_NONE) {
				if(plr[pnum].InvBody[4]._itype == ITYPE_SHIELD) {
					plr[pnum].InvBody[4]._iAC += 2;
				} else {
					plr[pnum].InvBody[4]._iMaxDam--;
					if(plr[pnum].InvBody[4]._iMaxDam < plr[pnum].InvBody[4]._iMinDam)
						plr[pnum].InvBody[4]._iMaxDam = plr[pnum].InvBody[4]._iMinDam;
				}
			}
			if(plr[pnum].InvBody[5]._itype != ITYPE_NONE) {
				if(plr[pnum].InvBody[5]._itype == ITYPE_SHIELD) {
					plr[pnum].InvBody[5]._iAC += 2;
				} else {
					plr[pnum].InvBody[5]._iMaxDam--;
					if(plr[pnum].InvBody[5]._iMaxDam < plr[pnum].InvBody[5]._iMinDam)
						plr[pnum].InvBody[5]._iMaxDam = plr[pnum].InvBody[5]._iMinDam;
				}
			}
			for(j = 0; j < plr[pnum]._pNumInv; j++) {
				switch(plr[pnum].InvList[j]._itype) {
				case 1:
				case 2:
				case 3:
				case 4:
				case 10:
					plr[pnum].InvList[j]._iMaxDam--;
					if(plr[pnum].InvList[j]._iMaxDam < plr[pnum].InvList[j]._iMinDam)
						plr[pnum].InvList[j]._iMaxDam = plr[pnum].InvList[j]._iMinDam;
					break;
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
					plr[pnum].InvList[j]._iAC += 2;
					break;
				}
			}
			InitDiabloMsg(14);
		}
		break;
	case 3:
		if(deltaload)
			return;
		if(pnum == myplr) {
			if(plr[pnum].InvBody[4]._itype != ITYPE_NONE && plr[pnum].InvBody[4]._itype != ITYPE_SHIELD)
				plr[pnum].InvBody[4]._iMaxDam++;
			if(plr[pnum].InvBody[5]._itype != ITYPE_NONE && plr[pnum].InvBody[5]._itype != ITYPE_SHIELD)
				plr[pnum].InvBody[5]._iMaxDam++;
			for(j = 0; j < plr[pnum]._pNumInv; j++) {
				switch(plr[pnum].InvList[j]._itype) {
				case 1:
				case 2:
				case 3:
				case 4:
				case 10:
					plr[pnum].InvList[j]._iMaxDam++;
					break;
				}
			}
			InitDiabloMsg(15);
		}
		break;
	case 4:
	case 11:
		if(deltaload)
			return;
		AddMissile(
			plr[pnum].WorldX,
			plr[pnum].WorldY,
			plr[pnum].WorldX,
			plr[pnum].WorldY,
			plr[pnum]._pdir,
			MIS_MANASHIELD,
			-1,
			pnum,
			0,
			2 * (unsigned char)leveltype);
		if(pnum != myplr)
			return;
		InitDiabloMsg(16);
		break;
	case 5:
		if(deltaload)
			return;
		if(pnum == myplr) {
			for(j = 0; j < 7; j++) {
				if(plr[pnum].InvBody[j]._itype == 10)
					plr[pnum].InvBody[j]._iCharges = plr[pnum].InvBody[j]._iMaxCharges;
			}
			for(j = 0; j < plr[pnum]._pNumInv; j++) {
				if(plr[pnum].InvList[j]._itype == 10)
					plr[pnum].InvList[j]._iCharges = plr[pnum].InvList[j]._iMaxCharges;
			}
			for(j = 0; j < 8; j++) {
				if(plr[pnum].SpdList[j]._itype == 10)
					plr[pnum].SpdList[j]._iCharges = plr[pnum].SpdList[j]._iMaxCharges; // belt items don't have charges?
			}
			InitDiabloMsg(17);
		}
		break;
	case 6:
		if(deltaload)
			return;
		if(pnum == myplr) {
			for(j = 0; j < 7; j++)
				plr[pnum].InvBody[j]._iDurability = plr[pnum].InvBody[j]._iMaxDur;
			for(j = 0; j < plr[pnum]._pNumInv; j++)
				plr[pnum].InvList[j]._iDurability = plr[pnum].InvList[j]._iMaxDur;
			for(j = 0; j < 8; j++)
				plr[pnum].SpdList[j]._iDurability = plr[pnum].SpdList[j]._iMaxDur; // belt items don't have durability?
			InitDiabloMsg(18);
		}
		break;
	case 7:
		if(deltaload || pnum != myplr)
			return;
		v12 = 0;
		spell = 1;
		spells = plr[pnum]._pMemSpells64;
		for(j = 1; j <= 37; j++) {
			if(spells & spell)
				v12++;
			spell <<= 1;
		}
		if(v12 > 1) {
			spell = 1;
			for(j = 1; j <= 37; j++) {
				if(plr[pnum]._pMemSpells64 & spell) {
					if(plr[pnum]._pSplLvl[j] < 15)
						plr[pnum]._pSplLvl[j]++;
				}
				spell <<= 1;
			}
			do {
				v60 = random(0, 37);
			}
			while(!(plr[pnum]._pMemSpells64 & ((__int64)1 << v60)));
			if(plr[pnum]._pSplLvl[v60] >= 2)
				plr[pnum]._pSplLvl[v60] -= 2;
			else
				plr[pnum]._pSplLvl[v60] = 0;
		}
		InitDiabloMsg(19);
		break;
	case 8:
		for(j = 0; j < nobjects; j++) {
			v1 = objectactive[j];
			/// ASSERT: assert((DWORD)v1 < MAXOBJECTS);
			if((object[v1]._otype == OBJ_CHEST1
			 || object[v1]._otype == OBJ_CHEST2
			 || object[v1]._otype == OBJ_CHEST3)
			 && !object[v1]._oSelFlag)
			{
				object[v1]._oRndSeed = GetRndSeed();
				object[v1]._oSelFlag = 1;
				object[v1]._oAnimFrame -= 2;
			}
		}
		if(deltaload)
			return;
		if(pnum == myplr)
			InitDiabloMsg(20);
		break;
	case 9:
		if(deltaload || pnum != myplr)
			return;
		plr[pnum]._pMemSpells64 |= (__int64)1 << (SPL_FIREBOLT-1);
		if(plr[pnum]._pSplLvl[SPL_FIREBOLT] < 15)
			plr[pnum]._pSplLvl[SPL_FIREBOLT]++;
		if(plr[pnum]._pSplLvl[SPL_FIREBOLT] < 15)
			plr[pnum]._pSplLvl[SPL_FIREBOLT]++;
		v72 = plr[pnum]._pMaxManaBase / 10;
		min = plr[pnum]._pMana - plr[pnum]._pManaBase;
		max = plr[pnum]._pMaxMana - plr[pnum]._pMaxManaBase;
		plr[pnum]._pManaBase -= v72;
		plr[pnum]._pMana -= v72;
		plr[pnum]._pMaxMana -= v72;
		plr[pnum]._pMaxManaBase -= v72;
		if((signed int)(plr[pnum]._pMana & 0xFFFFFFC0) <= 0) {
			plr[pnum]._pMana = min;
			plr[pnum]._pManaBase = 0;
		}
		if((signed int)(plr[pnum]._pMaxMana & 0xFFFFFFC0) <= 0) {
			plr[pnum]._pMaxMana = max;
			plr[pnum]._pMaxManaBase = 0;
		}
		InitDiabloMsg(21);
		break;
	case 10:
		if(deltaload)
			return;
		AddMissile(
			plr[pnum].WorldX,
			plr[pnum].WorldY,
			plr[pnum].WorldX,
			plr[pnum].WorldY,
			plr[pnum]._pdir,
			MIS_NOVA,
			-1,
			pnum,
			0,
			2 * (unsigned char)leveltype);
		if(pnum != myplr)
			return;
		plr[pnum]._pMana = plr[pnum]._pMaxMana;
		plr[pnum]._pManaBase = plr[pnum]._pMaxManaBase;
		InitDiabloMsg(22);
		break;
	case 12: /// BUGFIX: change `plr[pnum].HoldItem` to use a temporary buffer to prevent deleting item in hand
		if(deltaload)
			return;
		if(pnum == myplr) {
			for(j = 0; j < plr[pnum]._pNumInv; j++) {
				if(!plr[pnum].InvList[j]._itype) {
					if(plr[pnum].InvList[j]._iMiscId == IMISC_HEAL
					|| plr[pnum].InvList[j]._iMiscId == IMISC_MANA)
					{
						SetPlrHandItem(&plr[pnum].HoldItem, ItemMiscIdIdx(IMISC_REJUV));
						GetPlrHandSeed(&plr[pnum].HoldItem);
						plr[pnum].HoldItem._iStatFlag = 1;
						plr[pnum].InvList[j] = plr[pnum].HoldItem;
					}
					if(plr[pnum].InvList[j]._iMiscId == IMISC_FULLHEAL
					|| plr[pnum].InvList[j]._iMiscId == IMISC_FULLMANA)
					{
						SetPlrHandItem(&plr[pnum].HoldItem, ItemMiscIdIdx(IMISC_FULLREJUV));
						GetPlrHandSeed(&plr[pnum].HoldItem);
						plr[pnum].HoldItem._iStatFlag = 1;
						plr[pnum].InvList[j] = plr[pnum].HoldItem;
					}
				}
			}
			for(j = 0; j < 8; j++) {
				if(!plr[pnum].SpdList[j]._itype) {
					if(plr[pnum].SpdList[j]._iMiscId == IMISC_HEAL
					|| plr[pnum].SpdList[j]._iMiscId == IMISC_MANA)
					{
						SetPlrHandItem(&plr[pnum].HoldItem, ItemMiscIdIdx(IMISC_REJUV));
						GetPlrHandSeed(&plr[pnum].HoldItem);
						plr[pnum].HoldItem._iStatFlag = 1;
						plr[pnum].SpdList[j] = plr[pnum].HoldItem;
					}
					if(plr[pnum].SpdList[j]._iMiscId == IMISC_FULLHEAL
					|| plr[pnum].SpdList[j]._iMiscId == IMISC_FULLMANA)
					{
						SetPlrHandItem(&plr[pnum].HoldItem, ItemMiscIdIdx(IMISC_FULLREJUV));
						GetPlrHandSeed(&plr[pnum].HoldItem);
						plr[pnum].HoldItem._iStatFlag = 1;
						plr[pnum].SpdList[j] = plr[pnum].HoldItem;
					}
				}
			}
			InitDiabloMsg(24);
		}
		break;
	case 13:
		if(deltaload || pnum != myplr)
			return;
		ModifyPlrMag(pnum, 2);
		CheckStats(pnum);
		InitDiabloMsg(25);
		break;
	case 14:
		if(deltaload || pnum != myplr)
			return;
		if(2 * currlevel < 7) {
			CreateTypeItem(object[i]._ox, object[i]._oy, 0, ITYPE_MISC, IMISC_FULLMANA, 0, 1);
			CreateTypeItem(object[i]._ox, object[i]._oy, 0, ITYPE_MISC, IMISC_FULLHEAL, 0, 1);
		} else {
			CreateTypeItem(object[i]._ox, object[i]._oy, 0, ITYPE_MISC, IMISC_FULLREJUV, 0, 1);
			CreateTypeItem(object[i]._ox, object[i]._oy, 0, ITYPE_MISC, IMISC_FULLREJUV, 0, 1);
		}
		plr[pnum]._pMana = plr[pnum]._pMaxMana;
		plr[pnum]._pManaBase = plr[pnum]._pMaxManaBase;
		plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
		plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;
		InitDiabloMsg(26);
		break;
	case 15:
		if(deltaload)
			return;
		v88 = 0;
		do {
			xx = random(159, MAXDUNX);
			yy = random(159, MAXDUNY);
			v88++;
		}
		while(v88 <= MAXDUNX*MAXDUNY && (nSolidTable[dPiece[xx][yy]] || dObject[xx][yy] || dMonster[xx][yy]));
		AddMissile(plr[pnum].WorldX, plr[pnum].WorldY, xx, yy, plr[pnum]._pdir, MIS_RNDTELEPORT, -1, pnum, 0, 2 * (unsigned char)leveltype);
		if(pnum != myplr)
			return;
		InitDiabloMsg(27);
		break;
	case 16:
		if(deltaload || pnum != myplr)
			return;
		plr[pnum]._pMemSpells64 |= (__int64)1 << (SPL_CBOLT-1);
		if(plr[pnum]._pSplLvl[SPL_CBOLT] < 15)
			plr[pnum]._pSplLvl[SPL_CBOLT]++;
		if(plr[pnum]._pSplLvl[SPL_CBOLT] < 15)
			plr[pnum]._pSplLvl[SPL_CBOLT]++;
		v72 = plr[pnum]._pMaxManaBase / 10;
		min = plr[pnum]._pMana - plr[pnum]._pManaBase;
		max = plr[pnum]._pMaxMana - plr[pnum]._pMaxManaBase;
		plr[pnum]._pManaBase -= v72;
		plr[pnum]._pMana -= v72;
		plr[pnum]._pMaxMana -= v72;
		plr[pnum]._pMaxManaBase -= v72;
		if((signed int)(plr[pnum]._pMana & 0xFFFFFFC0) <= 0) {
			plr[pnum]._pMana = min;
			plr[pnum]._pManaBase = 0;
		}
		if((signed int)(plr[pnum]._pMaxMana & 0xFFFFFFC0) <= 0) {
			plr[pnum]._pMaxMana = max;
			plr[pnum]._pMaxManaBase = 0;
		}
		InitDiabloMsg(28);
		break;
	case 17:
		if(deltaload || pnum != myplr)
			return;
		for(j = 0; j < 40; j++) {
			if(!plr[pnum].InvGrid[j]) {
				v107 = 5 * (unsigned char)leveltype + random(160, 10 * (unsigned char)leveltype);
				v108 = plr[pnum]._pNumInv; // check
				plr[pnum].InvList[v108] = golditem;
				plr[pnum].InvList[v108]._iSeed = GetRndSeed();
				plr[pnum]._pNumInv++;
				plr[pnum].InvGrid[j] = plr[pnum]._pNumInv;
				plr[pnum].InvList[v108]._ivalue = v107;
				plr[pnum]._pGold += v107;
				SetGoldCurs(pnum, v108);
			}
		}
		InitDiabloMsg(29);
		break;
	case 18:
		if(deltaload)
			return;
		if(pnum == myplr) {
			InitDiabloMsg(30);
		} else {
			InitDiabloMsg(31);
			plr[myplr]._pHitPoints = plr[myplr]._pMaxHP;
			plr[myplr]._pHPBase = plr[myplr]._pMaxHPBase;
			plr[myplr]._pMana = plr[myplr]._pMaxMana;
			plr[myplr]._pManaBase = plr[myplr]._pMaxManaBase;
		}
		break;
	case 19:
		if(deltaload || pnum != myplr)
			return;
		ModifyPlrDex(pnum, 2);
		CheckStats(pnum);
		if(pnum == myplr)
			InitDiabloMsg(32);
		break;
	case 20:
		if(deltaload || pnum != myplr)
			return;
		ModifyPlrStr(pnum, 2);
		CheckStats(pnum);
		if(pnum == myplr)
			InitDiabloMsg(33);
		break;
	case 21:
		if(deltaload || pnum != myplr)
			return;
		ModifyPlrVit(pnum, 2);
		CheckStats(pnum);
		if(pnum == myplr)
			InitDiabloMsg(34);
		break;
	case 22:
		if(deltaload)
			return;
		if(pnum == myplr) {
			for(yy = 0; yy < DMAXY; yy++) {
				for(xx = 0; xx < DMAXX; xx++) {
					automapview[xx][yy] = 1;
				}
			}
			InitDiabloMsg(35);
		}
		break;
	case 23:
		if(deltaload || pnum != myplr)
			return;
		plr[pnum]._pMemSpells64 |= (__int64)1 << (SPL_HBOLT-1);
		if(plr[pnum]._pSplLvl[SPL_HBOLT] < 15)
			plr[pnum]._pSplLvl[SPL_HBOLT]++;
		if(plr[pnum]._pSplLvl[SPL_HBOLT] < 15)
			plr[pnum]._pSplLvl[SPL_HBOLT]++;
		v72 = plr[pnum]._pMaxManaBase / 10;
		min = plr[pnum]._pMana - plr[pnum]._pManaBase;
		max = plr[pnum]._pMaxMana - plr[pnum]._pMaxManaBase;
		plr[pnum]._pManaBase -= v72;
		plr[pnum]._pMana -= v72;
		plr[pnum]._pMaxMana -= v72;
		plr[pnum]._pMaxManaBase -= v72;
		if((signed int)(plr[pnum]._pMana & 0xFFFFFFC0) <= 0) {
			plr[pnum]._pMana = min;
			plr[pnum]._pManaBase = 0;
		}
		if((signed int)(plr[pnum]._pMaxMana & 0xFFFFFFC0) <= 0) {
			plr[pnum]._pMaxMana = max;
			plr[pnum]._pMaxManaBase = 0;
		}
		InitDiabloMsg(36);
		break;
	case 24:
		if(deltaload || pnum != myplr)
			return;
		for(j = 0; j < 7; j++) {
			if(plr[pnum].InvBody[j]._iMagical && !plr[pnum].InvBody[j]._iIdentified)
				plr[pnum].InvBody[j]._iIdentified = 1;
		}
		for(j = 0; j < plr[pnum]._pNumInv; j++) {
			if(plr[pnum].InvList[j]._iMagical && !plr[pnum].InvList[j]._iIdentified)
				plr[pnum].InvList[j]._iIdentified = 1;
		}
		for(j = 0; j < 8; j++) {
			if(plr[pnum].SpdList[j]._iMagical && !plr[pnum].SpdList[j]._iIdentified)
				plr[pnum].SpdList[j]._iIdentified = 1; // belt items can't be magical?
		}
		InitDiabloMsg(37);
		break;
	case 25:
		if(deltaload)
			return;
		if(pnum == myplr) {
			InitDiabloMsg(38);
		} else {
			InitDiabloMsg(39);
			v133 = random(155, 4);
			if(v133 != 0)
				v1 = -1;
			else
				v1 = 1;
			if(v133 != 1)
				v2 = -1;
			else
				v2 = 1;
			if(v133 == 2)
				v3 = 1;
			else
				v3 = -1;
			if(v133 == 3)
				v4 = 1;
			else
				v4 = -1;
			ModifyPlrStr(myplr, v1);
			ModifyPlrMag(myplr, v2);
			ModifyPlrDex(myplr, v3);
			ModifyPlrVit(myplr, v4);
			CheckStats(myplr);
		}
		break;
	}

	CalcPlrInv(pnum, 1);
	force_redraw = 255;

	if(pnum == myplr)
		NetSendCmdParam2(0, CMD_PLROPOBJ, pnum, i);
}

void OperateSkelBook(int pnum, int i, BOOL sendmsg)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}

	if(!deltaload) {
		PlaySfxLoc(IS_ISCROL, object[i]._ox, object[i]._oy);
	}

	object[i]._oSelFlag = 0;
	object[i]._oAnimFrame += 2;

	if(deltaload) {
		return;
	}

	SetRndSeed(object[i]._oRndSeed);

	if(random(161, 5) != 0) {
		CreateTypeItem(object[i]._ox, object[i]._oy, FALSE, ITYPE_MISC, IMISC_SCROLL, sendmsg, FALSE);
	} else {
		CreateTypeItem(object[i]._ox, object[i]._oy, FALSE, ITYPE_MISC, IMISC_BOOK, sendmsg, FALSE);
	}

	if(pnum == myplr) {
		NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
	}
}

void OperateBookCase(int pnum, int i, BOOL sendmsg)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}

	if(!deltaload) {
		PlaySfxLoc(IS_ISCROL, object[i]._ox, object[i]._oy);
	}

	object[i]._oSelFlag = 0;
	object[i]._oAnimFrame -= 2;

	if(deltaload) {
		return;
	}

	SetRndSeed(object[i]._oRndSeed);
	CreateTypeItem(object[i]._ox, object[i]._oy, FALSE, ITYPE_MISC, IMISC_BOOK, sendmsg, FALSE);

	if(QuestStatus(Q_ZHAR) && monster[4].mName == UniqMonst[2].mName && monster[4]._msquelch == 255 && monster[4]._mhitpoints != 0) {
		monster[4].mtalkmsg = TEXT_ZHAR2;
		M_StartStand(0, monster[4]._mdir);
		monster[4]._mgoal = 5;
		monster[4]._mmode = MM_TALK;
	}

	if(pnum == myplr) {
		NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
	}
}

void OperateDecap(int pnum, int i, BOOL sendmsg)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}

	object[i]._oSelFlag = 0;

	if(deltaload) {
		return;
	}

	SetRndSeed(object[i]._oRndSeed);
	CreateRndItem(object[i]._ox, object[i]._oy, FALSE, sendmsg, FALSE);

	if(pnum == myplr) {
		NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
	}
}

void OperateArmorStand(int pnum, int i, BOOL sendmsg)
{
	int uniqueRnd;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}

	object[i]._oSelFlag = 0;
	object[i]._oAnimFrame++;

	if(deltaload) {
		return;
	}

	SetRndSeed(object[i]._oRndSeed);

	uniqueRnd = random(0, 2);
	if(currlevel <= 5) {
		CreateTypeItem(object[i]._ox, object[i]._oy, TRUE, ITYPE_LARMOR, IMISC_NONE, sendmsg, FALSE);
	} else if(currlevel >= 6 && currlevel <= 9) {
		CreateTypeItem(object[i]._ox, object[i]._oy, uniqueRnd, ITYPE_MARMOR, IMISC_NONE, sendmsg, FALSE);
	} else if(currlevel >= 10 && currlevel <= 12) {
		CreateTypeItem(object[i]._ox, object[i]._oy, FALSE, ITYPE_HARMOR, IMISC_NONE, sendmsg, FALSE);
	} else if(currlevel >= 13 && currlevel <= 16) {
		CreateTypeItem(object[i]._ox, object[i]._oy, TRUE, ITYPE_HARMOR, IMISC_NONE, sendmsg, FALSE);
	}

	if(pnum == myplr) {
		NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
	}
}

int FindValidShrine(int i)
{
	int rv;
	BOOL done;

	do {
		done = FALSE;
		do {
			rv = random(0, 26);
			if(currlevel >= shrinemin[rv] && currlevel <= shrinemax[rv] && rv != 8) {
				done = TRUE;
			}
		} while(!done);
		if(gbMaxPlayers != 1 && shrineavail[rv] == 1) {
			done = FALSE;
			continue;
		}
		if(gbMaxPlayers == 1 && shrineavail[rv] == 2) {
			done = FALSE;
			continue;
		}
		done = TRUE;
	} while(!done);

	return rv;
}

void OperateGoatShrine(int pnum, int i, int sType)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	SetRndSeed(object[i]._oRndSeed);
	object[i]._oVar1 = FindValidShrine(i);
	OperateShrine(pnum, i, sType);
	object[i]._oAnimDelay = 2;
	force_redraw = 255;
}

void OperateCauldron(int pnum, int i, int sType)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	SetRndSeed(object[i]._oRndSeed);
	object[i]._oVar1 = FindValidShrine(i);
	OperateShrine(pnum, i, sType);
	object[i]._oAnimFrame = 3;
	object[i]._oAnimFlag = 0;
	force_redraw = 255;
}

BOOL OperateFountains(int pnum, int i)
{
	int ii, rndVal, statVal, saveRnd;
	BOOL rv, status;

	rv = FALSE;
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	SetRndSeed(object[i]._oRndSeed);

	switch(object[i]._otype) {
	case OBJ_PURIFYINGFTN:
		if(deltaload) {
			return FALSE;
		}
		if(pnum != myplr) {
			return FALSE;
		}
		if(plr[pnum]._pMana < plr[pnum]._pMaxMana) {
			PlaySfxLoc(LS_FOUNTAIN, object[i]._ox, object[i]._oy);
			plr[pnum]._pMana += 64;
			plr[pnum]._pManaBase += 64;
			if(plr[pnum]._pMana > plr[pnum]._pMaxMana) {
				plr[pnum]._pMana = plr[pnum]._pMaxMana;
				plr[pnum]._pManaBase = plr[pnum]._pMaxManaBase;
			}
			rv = TRUE;
		} else if(!deltaload) {
			PlaySfxLoc(LS_FOUNTAIN, object[i]._ox, object[i]._oy);
		}
		break;
	case OBJ_BLOODFTN:
		if(deltaload) {
			return FALSE;
		}
		if(pnum != myplr) {
			return FALSE;
		}
		if(plr[pnum]._pHitPoints < plr[pnum]._pMaxHP) {
			PlaySfxLoc(LS_FOUNTAIN, object[i]._ox, object[i]._oy);
			plr[pnum]._pHitPoints += 64;
			plr[pnum]._pHPBase += 64;
			if(plr[pnum]._pHitPoints > plr[pnum]._pMaxHP) {
				plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
				plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;
			}
			rv = TRUE;
		} else if(!deltaload) {
			PlaySfxLoc(LS_FOUNTAIN, object[i]._ox, object[i]._oy);
		}
		break;
	case OBJ_MURKYFTN:
		if(object[i]._oSelFlag == 0) {
			break;
		}
		if(!deltaload) {
			PlaySfxLoc(LS_FOUNTAIN, object[i]._ox, object[i]._oy);
		}
		object[i]._oSelFlag = 0;
		if(deltaload) {
			return FALSE;
		}
		AddMissile(
			plr[pnum].WorldX,
			plr[pnum].WorldY,
			plr[pnum].WorldX,
			plr[pnum].WorldY,
			plr[pnum]._pdir,
			MIS_INFRA,
			-1,
			pnum,
			0,
			2 * leveltype);
		rv = TRUE;
		if(pnum == myplr) {
			NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
		}
		break;
	case OBJ_TEARFTN:
		if(object[i]._oSelFlag == 0) {
			break;
		}
		saveRnd = -1;
		statVal = -1;
		status = FALSE;
		ii = 0;
		if(!deltaload) {
			PlaySfxLoc(LS_FOUNTAIN, object[i]._ox, object[i]._oy);
		}
		object[i]._oSelFlag = 0;
		if(deltaload) {
			return FALSE;
		}
		if(pnum != myplr) {
			return FALSE;
		}
		while(!status) {
			rndVal = random(0, 4);
			if(rndVal != saveRnd) {
				switch(rndVal) {
				case 0:
					ModifyPlrStr(pnum, statVal);
					break;
				case 1:
					ModifyPlrMag(pnum, statVal);
					break;
				case 2:
					ModifyPlrDex(pnum, statVal);
					break;
				case 3:
					ModifyPlrVit(pnum, statVal);
					break;
				}
				saveRnd = rndVal;
				statVal = 1;
				ii++;
			}
			if(ii > 1) {
				status = TRUE;
			}
		}
		CheckStats(pnum);
		rv = TRUE;
		if(pnum == myplr) {
			NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
		}
		break;
	}

	force_redraw = 255;

	return rv;
}

void OperateWeaponRack(int pnum, int i, BOOL sendmsg)
{
	int weaponType;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}

	SetRndSeed(object[i]._oRndSeed);

	switch(random(0, 4) + 1) {
	case 1:
		weaponType = ITYPE_SWORD;
		break;
	case 2:
		weaponType = ITYPE_AXE;
		break;
	case 3:
		weaponType = ITYPE_BOW;
		break;
	case 4:
		weaponType = ITYPE_MACE;
		break;
	}

	object[i]._oSelFlag = 0;
	object[i]._oAnimFrame++;

	if(deltaload) {
		return;
	}

	if(leveltype > DTYPE_CATHEDRAL) {
		CreateTypeItem(object[i]._ox, object[i]._oy, TRUE, weaponType, IMISC_NONE, sendmsg, FALSE);
	} else {
		CreateTypeItem(object[i]._ox, object[i]._oy, FALSE, weaponType, IMISC_NONE, sendmsg, FALSE);
	}

	if(pnum == myplr) {
		NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
	}
}

void OperateStoryBook(int pnum, int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}
	if(deltaload || qtextflag || pnum != myplr) {
		return;
	}

	object[i]._oAnimFrame = object[i]._oVar4;
	PlaySfxLoc(IS_ISCROL, object[i]._ox, object[i]._oy);
	InitQTextMsg(object[i]._oVar2);
	NetSendCmdParam1(FALSE, CMD_OPERATEOBJ, i);
}

void OperateLazStand(int pnum, int i)
{
	int x, y;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}
	if(deltaload || qtextflag || pnum != myplr) {
		return;
	}

	object[i]._oAnimFrame++;
	object[i]._oSelFlag = 0;
	GetSuperItemLoc(object[i]._ox, object[i]._oy, x, y);
	SpawnQuestItem(IDI_LAZSTAFF, x, y, 0, 0);
}

void OperateObject(int pnum, int i, BOOL TeleFlag)
{
	BOOL senditemmsg;

	senditemmsg = pnum == myplr;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	switch(object[i]._otype) {
	case OBJ_L1LDOOR:
	case OBJ_L1RDOOR:
		if(TeleFlag) {
			if(object[i]._otype == OBJ_L1LDOOR) {
				OperateL1LDoor(pnum, i, TRUE);
			}
			if(object[i]._otype == OBJ_L1RDOOR) {
				OperateL1RDoor(pnum, i, TRUE);
			}
		} else if(pnum == myplr) {
			OperateL1Door(pnum, i, TRUE);
		}
		break;
	case OBJ_L2LDOOR:
	case OBJ_L2RDOOR:
		if(TeleFlag) {
			if(object[i]._otype == OBJ_L2LDOOR) {
				OperateL2LDoor(pnum, i, TRUE);
			}
			if(object[i]._otype == OBJ_L2RDOOR) {
				OperateL2RDoor(pnum, i, TRUE);
			}
		} else if(pnum == myplr) {
			OperateL2Door(pnum, i, TRUE);
		}
		break;
	case OBJ_L3LDOOR:
	case OBJ_L3RDOOR:
		if(TeleFlag) {
			if(object[i]._otype == OBJ_L3LDOOR) {
				OperateL3LDoor(pnum, i, TRUE);
			}
			if(object[i]._otype == OBJ_L3RDOOR) {
				OperateL3RDoor(pnum, i, TRUE);
			}
		} else if(pnum == myplr) {
			OperateL3Door(pnum, i, TRUE);
		}
		break;
	case OBJ_LEVER:
	case OBJ_SWITCHSKL:
		OperateLever(pnum, i);
		break;
	case OBJ_BOOK2L:
		OperateBook(pnum, i);
		break;
	case OBJ_BOOK2R:
		OperateSChambBk(pnum, i);
		break;
	case OBJ_CHEST1:
	case OBJ_CHEST2:
	case OBJ_CHEST3:
	case OBJ_TCHEST1:
	case OBJ_TCHEST2:
	case OBJ_TCHEST3:
		OperateChest(pnum, i, senditemmsg);
		break;
	case OBJ_SARC:
		OperateSarc(pnum, i, senditemmsg);
		break;
	case OBJ_FLAMELVR:
		OperateTrapLvr(i);
		break;
	case OBJ_BLINDBOOK:
	case OBJ_BLOODBOOK:
	case OBJ_STEELTOME:
		OperateBookLever(pnum, i);
		break;
	case OBJ_SHRINEL:
	case OBJ_SHRINER:
		OperateShrine(pnum, i, IS_MAGIC);
		break;
	case OBJ_SKELBOOK:
	case OBJ_BOOKSTAND:
		OperateSkelBook(pnum, i, senditemmsg);
		break;
	case OBJ_BOOKCASEL:
	case OBJ_BOOKCASER:
		OperateBookCase(pnum, i, senditemmsg);
		break;
	case OBJ_DECAP:
		OperateDecap(pnum, i, senditemmsg);
		break;
	case OBJ_ARMORSTAND:
	case OBJ_WARARMOR:
		OperateArmorStand(pnum, i, senditemmsg);
		break;
	case OBJ_GOATSHRINE:
		OperateGoatShrine(pnum, i, LS_GSHRINE);
		break;
	case OBJ_CAULDRON:
		OperateCauldron(pnum, i, LS_CALDRON);
		break;
	case OBJ_BLOODFTN:
	case OBJ_PURIFYINGFTN:
	case OBJ_MURKYFTN:
	case OBJ_TEARFTN:
		OperateFountains(pnum, i);
		break;
	case OBJ_STORYBOOK:
		OperateStoryBook(pnum, i);
		break;
	case OBJ_PEDISTAL:
		OperatePedistal(pnum, i);
		break;
	case OBJ_WARWEAP:
	case OBJ_WEAPONRACK:
		OperateWeaponRack(pnum, i, senditemmsg);
		break;
	case OBJ_MUSHPATCH:
		OperateMushPatch(pnum, i);
		break;
	case OBJ_LAZSTAND:
		OperateLazStand(pnum, i);
		break;
	case OBJ_SLAINHERO:
		OperateSlainHero(pnum, i, senditemmsg);
		break;
	case OBJ_SIGNCHEST:
		OperateInnSignChest(pnum, i);
		break;
	}
}

void SyncOpL1Door(int pnum, int cmd, int i)
{
	BOOL opok;

	if(pnum == myplr) {
		return;
	}

	opok = FALSE;
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(cmd == CMD_OPENDOOR && object[i]._oVar4 == 0) {
		opok = TRUE;
	}
	if(cmd == CMD_CLOSEDOOR && object[i]._oVar4 == 1) {
		opok = TRUE;
	}
	if(opok) {
		if(object[i]._otype == OBJ_L1LDOOR) {
			OperateL1LDoor(-1, i, FALSE);
		}
		if(object[i]._otype == OBJ_L1RDOOR) {
			OperateL1RDoor(-1, i, FALSE);
		}
	}
}

void SyncOpL2Door(int pnum, int cmd, int i)
{
	BOOL opok;

	if(pnum == myplr) {
		return;
	}

	opok = FALSE;
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(cmd == CMD_OPENDOOR && object[i]._oVar4 == 0) {
		opok = TRUE;
	}
	if(cmd == CMD_CLOSEDOOR && object[i]._oVar4 == 1) {
		opok = TRUE;
	}
	if(opok) {
		if(object[i]._otype == OBJ_L2LDOOR) {
			OperateL2LDoor(-1, i, FALSE);
		}
		if(object[i]._otype == OBJ_L2RDOOR) {
			OperateL2RDoor(-1, i, FALSE);
		}
	}
}

void SyncOpL3Door(int pnum, int cmd, int i)
{
	BOOL opok;

	if(pnum == myplr) {
		return;
	}

	opok = FALSE;
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(cmd == CMD_OPENDOOR && object[i]._oVar4 == 0) {
		opok = TRUE;
	}
	if(cmd == CMD_CLOSEDOOR && object[i]._oVar4 == 1) {
		opok = TRUE;
	}
	if(opok) {
		if(object[i]._otype == OBJ_L3LDOOR) {
			OperateL3LDoor(-1, i, FALSE);
		}
		if(object[i]._otype == OBJ_L3RDOOR) {
			OperateL3RDoor(-1, i, FALSE);
		}
	}
}

void SyncOpObject(int pnum, int cmd, int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	switch(object[i]._otype) {
	case OBJ_L1LDOOR:
	case OBJ_L1RDOOR:
		SyncOpL1Door(pnum, cmd, i);
		break;
	case OBJ_L2LDOOR:
	case OBJ_L2RDOOR:
		SyncOpL2Door(pnum, cmd, i);
		break;
	case OBJ_L3LDOOR:
	case OBJ_L3RDOOR:
		SyncOpL3Door(pnum, cmd, i);
		break;
	case OBJ_LEVER:
	case OBJ_SWITCHSKL:
		OperateLever(pnum, i);
		break;
	case OBJ_CHEST1:
	case OBJ_CHEST2:
	case OBJ_CHEST3:
	case OBJ_TCHEST1:
	case OBJ_TCHEST2:
	case OBJ_TCHEST3:
		OperateChest(pnum, i, FALSE);
		break;
	case OBJ_SARC:
		OperateSarc(pnum, i, FALSE);
		break;
	case OBJ_BLINDBOOK:
	case OBJ_BLOODBOOK:
	case OBJ_STEELTOME:
		OperateBookLever(pnum, i);
		break;
	case OBJ_SHRINEL:
	case OBJ_SHRINER:
		OperateShrine(pnum, i, IS_MAGIC);
		break;
	case OBJ_SKELBOOK:
	case OBJ_BOOKSTAND:
		OperateSkelBook(pnum, i, FALSE);
		break;
	case OBJ_BOOKCASEL:
	case OBJ_BOOKCASER:
		OperateBookCase(pnum, i, FALSE);
		break;
	case OBJ_DECAP:
		OperateDecap(pnum, i, FALSE);
		break;
	case OBJ_ARMORSTAND:
	case OBJ_WARARMOR:
		OperateArmorStand(pnum, i, FALSE);
		break;
	case OBJ_GOATSHRINE:
		OperateGoatShrine(pnum, i, LS_GSHRINE);
		break;
	case OBJ_CAULDRON:
		OperateCauldron(pnum, i, LS_CALDRON);
		break;
	case OBJ_MURKYFTN:
	case OBJ_TEARFTN:
		OperateFountains(pnum, i);
		break;
	case OBJ_STORYBOOK:
		OperateStoryBook(pnum, i);
		break;
	case OBJ_PEDISTAL:
		OperatePedistal(pnum, i);
		break;
	case OBJ_WARWEAP:
	case OBJ_WEAPONRACK:
		OperateWeaponRack(pnum, i, FALSE);
		break;
	case OBJ_MUSHPATCH:
		OperateMushPatch(pnum, i);
		break;
	case OBJ_SLAINHERO:
		OperateSlainHero(pnum, i, FALSE);
		break;
	case OBJ_SIGNCHEST:
		OperateInnSignChest(pnum, i);
		break;
	}
}

void BreakCrux(int i)
{
	int j, ot, oi;
	BOOL mapflag;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oAnimFlag = 1;
	object[i]._oAnimFrame = 1;
	object[i]._oAnimDelay = 1;
	object[i]._oSolidFlag = 1;
	object[i]._oMissFlag = 1;
	object[i]._oBreak = -1;
	object[i]._oSelFlag = 0;

	mapflag = TRUE;
	for(j = 0; j < nobjects; j++) {
		oi = objectactive[j];
		ot = object[oi]._otype;
		if((ot == OBJ_CRUX1 || ot == OBJ_CRUX2 || ot == OBJ_CRUX3) && object[i]._oVar8 == object[oi]._oVar8 && object[oi]._oBreak != -1) {
			mapflag = FALSE;
		}
	}

	if(mapflag) {
		if(!deltaload) {
			PlaySfxLoc(IS_LEVER, object[i]._ox, object[i]._oy);
		}
		ObjChangeMap(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
	}
}

void BreakBarrel(int pnum, int i, int dam, BOOL forcebreak, BOOL sendmsg)
{
	int x, y, oi;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		return;
	}

	if(forcebreak) {
		object[i]._oVar1 = 0;
	} else {
		object[i]._oVar1 -= dam;
		if(pnum != myplr && object[i]._oVar1 <= 0) {
			object[i]._oVar1 = 1;
		}
	}

	if(object[i]._oVar1 > 0) {
		if(!deltaload) {
			PlaySfxLoc(IS_IBOW, object[i]._ox, object[i]._oy);
		}
		return;
	}

	object[i]._oVar1 = 0;
	object[i]._oAnimFlag = 1;
	object[i]._oAnimFrame = 1;
	object[i]._oAnimDelay = 1;
	object[i]._oSolidFlag = 0;
	object[i]._oMissFlag = 1;
	object[i]._oBreak = -1;
	object[i]._oSelFlag = 0;
	object[i]._oPreFlag = 1;

	if(deltaload) {
		object[i]._oAnimFrame = object[i]._oAnimLen;
		object[i]._oAnimCnt = 0;
		object[i]._oAnimDelay = 1000;
		return;
	}

	if(object[i]._otype == OBJ_BARRELEX) {
		PlaySfxLoc(IS_BARLFIRE, object[i]._ox, object[i]._oy);
		for(y = object[i]._oy - 1; y <= object[i]._oy + 1; y++) {
			for(x = object[i]._ox - 1; x <= object[i]._ox + 1; x++) {
				if(dMonster[x][y] > 0) {
					MonsterTrapHit(dMonster[x][y] - 1, 1, 4, 0, 1, FALSE);
				}
				if(dPlayer[x][y] > 0) {
					PlayerMHit(dPlayer[x][y] - 1, -1, 0, 8, 16, 1, FALSE, 0);
				}
				if(dObject[x][y] > 0) {
					oi = dObject[x][y] - 1;
					if(object[oi]._otype == OBJ_BARRELEX && object[oi]._oBreak != -1) {
						BreakBarrel(pnum, oi, dam, TRUE, sendmsg);
					}
				}
			}
		}
	} else {
		PlaySfxLoc(IS_BARREL, object[i]._ox, object[i]._oy);
		SetRndSeed(object[i]._oRndSeed);
		if(object[i]._oVar2 <= 1) {
			if(object[i]._oVar3 == 0) {
				CreateRndUseful(pnum, object[i]._ox, object[i]._oy, sendmsg);
			} else {
				CreateRndItem(object[i]._ox, object[i]._oy, FALSE, sendmsg, FALSE);
			}
		}
		if(object[i]._oVar2 >= 8) {
			SpawnSkeleton(object[i]._oVar4, object[i]._ox, object[i]._oy);
		}
	}

	if(pnum == myplr) {
		NetSendCmdParam2(FALSE, CMD_BREAKOBJ, pnum, i);
	}
}

void BreakObject(int pnum, int oi)
{
	int objdam, mind, maxd;

	if(pnum != -1) {
		mind = plr[pnum]._pIMinDam;
		maxd = plr[pnum]._pIMaxDam;
		objdam = random(163, maxd - mind + 1) + mind;
		objdam += objdam * plr[pnum]._pIBonusDam / 100;
		objdam += plr[pnum]._pDamageMod + plr[pnum]._pIBonusDamMod;
	} else {
		objdam = 10;
	}

	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
	switch(object[oi]._otype) {
	case OBJ_CRUX1:
	case OBJ_CRUX2:
	case OBJ_CRUX3:
		BreakCrux(oi);
		break;
	case OBJ_BARREL:
	case OBJ_BARRELEX:
		BreakBarrel(pnum, oi, objdam, FALSE, TRUE);
		break;
	}
}

void SyncBreakObj(int pnum, int oi)
{
	/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
	switch(object[oi]._otype) {
	case OBJ_BARREL:
	case OBJ_BARRELEX:
		BreakBarrel(pnum, oi, 0, TRUE, FALSE);
		break;
	}
}

void SyncL1Doors(int i)
{
	int dx, dy;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oVar4 == 0) {
		object[i]._oMissFlag = 0;
		return;
	}

	object[i]._oMissFlag = 1;
	dx = object[i]._ox;
	dy = object[i]._oy;
	object[i]._oSelFlag = 2;

	if(object[i]._otype == OBJ_L1LDOOR) {
		if(object[i]._oVar1 == 214) {
			ObjSetMicro(dx, dy, 408);
		} else {
			ObjSetMicro(dx, dy, 393);
		}
		dSpecial[dx][dy] = 7;
		objects_set_door_piece(dx - 1, dy);
		dy--;
	} else {
		ObjSetMicro(dx, dy, 395);
		dSpecial[dx][dy] = 8;
		objects_set_door_piece(dx, dy - 1);
		dx--;
	}

	DoorSet(i, dx, dy);
}

void SyncCrux(int i)
{
	int j, ot, oi;
	BOOL mapflag;

	mapflag = TRUE;
	for(j = 0; j < nobjects; j++) {
		oi = objectactive[j];
		/// ASSERT: assert((DWORD)oi < MAXOBJECTS);
		ot = object[oi]._otype;
		if((ot == OBJ_CRUX1 || ot == OBJ_CRUX2 || ot == OBJ_CRUX3) && object[i]._oVar8 == object[oi]._oVar8 && object[oi]._oBreak != -1) {
			mapflag = FALSE;
		}
	}

	if(mapflag) {
		ObjChangeMap(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
	}
}

void SyncLever(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oSelFlag == 0) {
		ObjChangeMap(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
	}
}

void SyncQSTLever(int i)
{
	int tren;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oAnimFrame == object[i]._oVar6) {
		ObjChangeMapResync(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
		if(object[i]._otype == OBJ_BLINDBOOK) {
			tren = TransVal;
			TransVal = 9;
			DRLG_MRectTrans(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
			TransVal = tren;
		}
	}
}

void SyncPedistal(int i)
{
	BYTE *setp;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oVar6 == 1) {
		ObjChangeMapResync(setpc_x, setpc_y + 3, setpc_x + 2, setpc_y + 7);
	}
	if(object[i]._oVar6 == 2) {
		ObjChangeMapResync(setpc_x, setpc_y + 3, setpc_x + 2, setpc_y + 7);
		ObjChangeMapResync(setpc_x + 6, setpc_y + 3, setpc_x + setpc_w, setpc_y + 7);
	}
	if(object[i]._oVar6 == 3) {
		ObjChangeMapResync(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
		setp = DiabLoad("Levels\\L2Data\\Blood2.DUN", NULL, 'STPC');
		LoadMapObjs(setp, 2 * setpc_x, 2 * setpc_y);
		MemFreeDbg(setp);
	}
}

void SyncL2Doors(int i)
{
	int dx, dy;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	if(object[i]._oVar4 == 0) {
		object[i]._oMissFlag = 0;
	} else {
		object[i]._oMissFlag = 1;
	}

	dx = object[i]._ox;
	dy = object[i]._oy;
	object[i]._oSelFlag = 2;

	if(object[i]._otype == OBJ_L2LDOOR && object[i]._oVar4 == 0) {
		ObjSetMicro(dx, dy, 538);
		return;
	}
	if(object[i]._otype == OBJ_L2LDOOR && (object[i]._oVar4 == 1 || object[i]._oVar4 == 2)) {
		ObjSetMicro(dx, dy, 13);
		return;
	}
	if(object[i]._otype == OBJ_L2RDOOR && object[i]._oVar4 == 0) {
		ObjSetMicro(dx, dy, 540);
		return;
	}
	if(object[i]._otype == OBJ_L2RDOOR && (object[i]._oVar4 == 1 || object[i]._oVar4 == 2)) {
		ObjSetMicro(dx, dy, 17);
		return;
	}
}

void SyncL3Doors(int i)
{
	int dx, dy;

	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	object[i]._oMissFlag = 1;
	dx = object[i]._ox;
	dy = object[i]._oy;
	object[i]._oSelFlag = 2;

	if(object[i]._otype == OBJ_L3LDOOR && object[i]._oVar4 == 0) {
		ObjSetMicro(dx, dy, 531);
		return;
	}
	if(object[i]._otype == OBJ_L3LDOOR && (object[i]._oVar4 == 1 || object[i]._oVar4 == 2)) {
		ObjSetMicro(dx, dy, 538);
		return;
	}
	if(object[i]._otype == OBJ_L3RDOOR && object[i]._oVar4 == 0) {
		ObjSetMicro(dx, dy, 534);
		return;
	}
	if(object[i]._otype == OBJ_L3RDOOR && (object[i]._oVar4 == 1 || object[i]._oVar4 == 2)) {
		ObjSetMicro(dx, dy, 541);
		return;
	}
}

void SyncObjectAnim(int o)
{
	int ai, ot, j;

	/// ASSERT: assert((DWORD)o < MAXOBJECTS);
	ot = object[o]._otype;
	ai = AllObjects[ot].ofindex;
	j = 0;
	while(ObjFileList[j] != ai) {
		j++;
	}
	object[o]._oAnimData = pObjCels[j];

	switch(object[o]._otype) {
	case OBJ_L1LDOOR:
	case OBJ_L1RDOOR:
		SyncL1Doors(o);
		break;
	case OBJ_L2LDOOR:
	case OBJ_L2RDOOR:
		SyncL2Doors(o);
		break;
	case OBJ_L3LDOOR:
	case OBJ_L3RDOOR:
		SyncL3Doors(o);
		break;
	case OBJ_CRUX1:
	case OBJ_CRUX2:
	case OBJ_CRUX3:
		SyncCrux(o);
		break;
	case OBJ_LEVER:
	case OBJ_BOOK2L:
	case OBJ_SWITCHSKL:
		SyncLever(o);
		break;
	case OBJ_BOOK2R:
	case OBJ_BLINDBOOK:
	case OBJ_STEELTOME:
		SyncQSTLever(o);
		break;
	case OBJ_PEDISTAL:
		SyncPedistal(o);
		break;
	}
}

void GetObjectStr(int i)
{
	/// ASSERT: assert((DWORD)i < MAXOBJECTS);
	switch(object[i]._otype) {
	case OBJ_CRUX1:
	case OBJ_CRUX2:
	case OBJ_CRUX3:
		strcpy(infostr, "Crucified Skeleton");
		break;
	case OBJ_LEVER:
	case OBJ_FLAMELVR:
		strcpy(infostr, "Lever");
		break;
	case OBJ_L1LDOOR:
	case OBJ_L1RDOOR:
	case OBJ_L2LDOOR:
	case OBJ_L2RDOOR:
	case OBJ_L3LDOOR:
	case OBJ_L3RDOOR:
		if(object[i]._oVar4 == 1) {
			strcpy(infostr, "Open Door");
		}
		if(object[i]._oVar4 == 0) {
			strcpy(infostr, "Closed Door");
		}
		if(object[i]._oVar4 == 2) {
			strcpy(infostr, "Blocked Door");
		}
		break;
	case OBJ_BOOK2L:
		if(setlevel) {
			if(setlvlnum == SL_BONECHAMB) {
				strcpy(infostr, "Ancient Tome");
			} else if(setlvlnum == SL_VILEBETRAYER) {
				strcpy(infostr, "Book of Vileness");
			}
		}
		break;
	case OBJ_SWITCHSKL:
		strcpy(infostr, "Skull Lever");
		break;
	case OBJ_BOOK2R:
		strcpy(infostr, "Mythical Book");
		break;
	case OBJ_CHEST1:
	case OBJ_TCHEST1:
		strcpy(infostr, "Small Chest");
		break;
	case OBJ_CHEST2:
	case OBJ_TCHEST2:
		strcpy(infostr, "Chest");
		break;
	case OBJ_CHEST3:
	case OBJ_TCHEST3:
	case OBJ_SIGNCHEST:
		strcpy(infostr, "Large Chest");
		break;
	case OBJ_SARC:
		strcpy(infostr, "Sarcophagus");
		break;
	case OBJ_BOOKSHELF:
		strcpy(infostr, "Bookshelf");
		break;
	case OBJ_BOOKCASEL:
	case OBJ_BOOKCASER:
		strcpy(infostr, "Bookcase");
		break;
	case OBJ_BARREL:
	case OBJ_BARRELEX:
		strcpy(infostr, "Barrel");
		break;
	case OBJ_SHRINEL:
	case OBJ_SHRINER:
		sprintf(tempstr, "%s Shrine", shrinestrs[object[i]._oVar1]);
		strcpy(infostr, tempstr);
		break;
	case OBJ_SKELBOOK:
		strcpy(infostr, "Skeleton Tome");
		break;
	case OBJ_BOOKSTAND:
		strcpy(infostr, "Library Book");
		break;
	case OBJ_BLOODFTN:
		strcpy(infostr, "Blood Fountain");
		break;
	case OBJ_DECAP:
		strcpy(infostr, "Decapitated Body");
		break;
	case OBJ_BLINDBOOK:
		strcpy(infostr, "Book of the Blind");
		break;
	case OBJ_STEELTOME:
		strcpy(infostr, "Steel Tome");
		break;
	case OBJ_BLOODBOOK:
		strcpy(infostr, "Book of Blood");
		break;
	case OBJ_PURIFYINGFTN:
		strcpy(infostr, "Purifying Spring");
		break;
	case OBJ_ARMORSTAND:
	case OBJ_WARARMOR:
		strcpy(infostr, "Armor");
		break;
	case OBJ_WARWEAP:
		strcpy(infostr, "Weapon Rack");
		break;
	case OBJ_GOATSHRINE:
		strcpy(infostr, "Goat Shrine");
		break;
	case OBJ_CAULDRON:
		strcpy(infostr, "Cauldron");
		break;
	case OBJ_MURKYFTN:
		strcpy(infostr, "Murky Pool");
		break;
	case OBJ_TEARFTN:
		strcpy(infostr, "Fountain of Tears");
		break;
	case OBJ_PEDISTAL:
		strcpy(infostr, "Pedestal of Blood");
		break;
	case OBJ_STORYBOOK:
		strcpy(infostr, StoryBookName[object[i]._oVar3]);
		break;
	case OBJ_WEAPONRACK:
		strcpy(infostr, "Weapon Rack");
		break;
	case OBJ_MUSHPATCH:
		strcpy(infostr, "Mushroom Patch");
		break;
	case OBJ_LAZSTAND:
		strcpy(infostr, "Vile Stand");
		break;
	case OBJ_SLAINHERO:
		strcpy(infostr, "Slain Hero");
		break;
	}

	if(plr[myplr]._pClass == PC_ROGUE && object[i]._oTrapFlag) {
		sprintf(tempstr, "Trapped %s", infostr);
		strcpy(infostr, tempstr);
		infoclr = COL_RED;
	}
}
