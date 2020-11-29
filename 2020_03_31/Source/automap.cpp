#include "diablo.h"

WORD automaptype[512];
int AutoMapX; // weak
int AutoMapY; // weak
BOOL automapflag; // idb
char AmShiftTab[31];
BOOLEAN automapview[40][40];
int AutoMapScale; // idb
int AutoMapXOfs; // weak
int AutoMapYOfs; // weak
int AmLine64; // weak
int AmLine32; // weak
int AmLine16; // weak
int AmLine8; // weak
int AmLine4; // weak

void InitAutomapOnce()
{
	automapflag = 0;
	AutoMapScale = 50;
	AmLine64 = (AutoMapScale << 6) / 100;
	AmLine32 = AmLine64 >> 1;
	AmLine16 = AmLine32 >> 1;
	AmLine8 = AmLine16 >> 1;
	AmLine4 = AmLine8 >> 1;
}

void InitAutomap()
{
	int i, j, l, s;
	BYTE b1, b2;
	DWORD dwTiles, d;
	BYTE *pAFile, *pTmp;

	l = 50;
	for(i = 0; i < 31; i++) {
		s = (l << 6) / 100;
		AmShiftTab[i] = 2 * (320 / s) + 1;
		if(320 % s) {
			AmShiftTab[i]++;
		}
		if(320 % s >= (l << 5) / 100) {
			AmShiftTab[i]++;
		}
		l += 5;
	}

	memset(automaptype, 0, sizeof(automaptype));

	switch(leveltype) {
	case DTYPE_CATHEDRAL:
		pAFile = DiabLoad("Levels\\L1Data\\L1.AMP", &dwTiles, 'AMAP');
		dwTiles >>= 1;
		break;
	case DTYPE_CATACOMBS:
		pAFile = DiabLoad("Levels\\L2Data\\L2.AMP", &dwTiles, 'AMAP');
		dwTiles >>= 1;
		break;
	case DTYPE_CAVES:
		pAFile = DiabLoad("Levels\\L3Data\\L3.AMP", &dwTiles, 'AMAP');
		dwTiles >>= 1;
		break;
	case DTYPE_HELL:
		pAFile = DiabLoad("Levels\\L4Data\\L4.AMP", &dwTiles, 'AMAP');
		dwTiles >>= 1;
		break;
	default:
		return;
	}

	pTmp = pAFile;
	for(d = 1; d <= dwTiles; d++) {
		b1 = *pTmp++;
		b2 = *pTmp++;
		automaptype[d] = b1 + (b2 << 8);
	}

	MemFreeDbg(pAFile);
	memset(automapview, 0, sizeof(automapview));

	for(j = 0; j < MAXDUNY; j++) {
		for(i = 0; i < MAXDUNX; i++) {
			dFlags[i][j] &= ~0x80;
		}
	}
}

void StartAutomap()
{
	AutoMapXOfs = 0;
	AutoMapYOfs = 0;
	automapflag = 1;
}

void AutomapUp()
{
	AutoMapXOfs--;
	AutoMapYOfs--;
}

void AutomapDown()
{
	AutoMapXOfs++;
	AutoMapYOfs++;
}

void AutomapLeft()
{
	AutoMapXOfs--;
	AutoMapYOfs++;
}

void AutomapRight()
{
	AutoMapXOfs++;
	AutoMapYOfs--;
}

void AutomapZoomIn()
{
	if(AutoMapScale < 200) {
		AutoMapScale += 5;
		AmLine64 = (AutoMapScale << 6) / 100;
		AmLine32 = AmLine64 >> 1;
		AmLine16 = AmLine32 >> 1;
		AmLine8 = AmLine16 >> 1;
		AmLine4 = AmLine8 >> 1;
	}
}

void AutomapZoomOut()
{
	if(AutoMapScale > 50) {
		AutoMapScale -= 5;
		AmLine64 = (AutoMapScale << 6) / 100;
		AmLine32 = AmLine64 >> 1;
		AmLine16 = AmLine32 >> 1;
		AmLine8 = AmLine16 >> 1;
		AmLine4 = AmLine8 >> 1;
	}
}

void DrawAutomap()
{
	int MapX, MapY, Sx, Sy, Len, i, j, Px, Py;
	WORD Tile;

	if(leveltype == DTYPE_TOWN) {
		DrawAutomapText();
		return;
	}

	/// ASSERT: assert(gpBuffer);
	gpBufEnd = &gpBuffer[(352 + SCREEN_Y) * BUFFER_WIDTH];

	AutoMapX = (ViewX - 16) >> 1;
	while(AutoMapX + AutoMapXOfs < 0) {
		AutoMapXOfs++;
	}
	while(AutoMapX + AutoMapXOfs >= DMAXX) {
		AutoMapXOfs--;
	}
	AutoMapX += AutoMapXOfs;

	AutoMapY = (ViewY - 16) >> 1;
	while(AutoMapY + AutoMapYOfs < 0) {
		AutoMapYOfs++;
	}
	while(AutoMapY + AutoMapYOfs >= DMAXY) {
		AutoMapYOfs--;
	}
	AutoMapY += AutoMapYOfs;

	Len = AmShiftTab[(AutoMapScale - 50) / 5];
	if(ScrollInfo._sxoff + ScrollInfo._syoff) {
		Len++;
	}

	MapX = AutoMapX - Len;
	MapY = AutoMapY - 1;

	if(Len & 1) {
		Sx = 640 / 2 + SCREEN_X - AmLine64 * ((Len - 1) >> 1);
		Sy = 352 / 2 + SCREEN_Y - AmLine32 * ((Len + 1) >> 1);
	} else {
		Sx = 640 / 2 + SCREEN_X - AmLine64 * (Len >> 1) + AmLine32;
		Sy = 352 / 2 + SCREEN_Y - AmLine32 * (Len >> 1) - AmLine16;
	}

	if(ViewX & 1) {
		Sx -= AmLine16;
		Sy -= AmLine8;
	}
	if(ViewY & 1) {
		Sx += AmLine16;
		Sy -= AmLine8;
	}

	Sx += ScrollInfo._sxoff * AutoMapScale / 100 >> 1;
	Sy += ScrollInfo._syoff * AutoMapScale / 100 >> 1;

	if(invflag || sbookflag) {
		Sx -= 160;
	}
	if(chrflag || questlog) {
		Sx += 160;
	}

	for(i = 0; i <= Len + 1; i++) {
		Px = Sx;
		Py = Sy;
		for(j = 0; j < Len; j++) {
			Tile = GetAutomapType(j + MapX, MapY - j, TRUE);
			if(Tile != 0) {
				DrawAutomapTile(Px, Py, Tile);
			}
			Px += AmLine64;
		}
		MapY++;
		Px = Sx - AmLine32;
		Py = Sy + AmLine16;
		for(j = 0; j <= Len; j++) {
			Tile = GetAutomapType(j + MapX, MapY - j, TRUE);
			if(Tile != 0) {
				DrawAutomapTile(Px, Py, Tile);
			}
			Px += AmLine64;
		}
		MapX++;
		Sy += AmLine32;
	}

	DrawAutomapPlr();
	DrawAutomapText();
}

void DrawAutomapTile(int sx, int sy, WORD tile)
{
	int x1, y1, x2, y2;
	BYTE f;
	BOOL AMLWallFlag, AMRWallFlag, AMLLWallFlag, AMLRWallFlag;

	f = tile >> 8;

	if(f & AFLAG_DIRT) {
		ENG_set_pixel(sx, sy, 200);
		ENG_set_pixel(sx - AmLine8, sy - AmLine4, 200);
		ENG_set_pixel(sx - AmLine8, sy + AmLine4, 200);
		ENG_set_pixel(sx + AmLine8, sy - AmLine4, 200);
		ENG_set_pixel(sx + AmLine8, sy + AmLine4, 200);
		ENG_set_pixel(sx - AmLine16, sy, 200);
		ENG_set_pixel(sx + AmLine16, sy, 200);
		ENG_set_pixel(sx, sy - AmLine8, 200);
		ENG_set_pixel(sx, sy + AmLine8, 200);
		ENG_set_pixel(sx + AmLine8 - AmLine32, sy + AmLine4, 200);
		ENG_set_pixel(sx + AmLine32 - AmLine8, sy + AmLine4, 200);
		ENG_set_pixel(sx - AmLine16, sy + AmLine8, 200);
		ENG_set_pixel(sx + AmLine16, sy + AmLine8, 200);
		ENG_set_pixel(sx - AmLine8, sy + AmLine16 - AmLine4, 200);
		ENG_set_pixel(sx + AmLine8, sy + AmLine16 - AmLine4, 200);
		ENG_set_pixel(sx, sy + AmLine16, 200);
	}
	if(f & AFLAG_STAIRS) {
		DrawLine(sx - AmLine8, sy - AmLine8 - AmLine4, sx + AmLine8 + AmLine16, sy + AmLine4, 144);
		DrawLine(sx - AmLine16, sy - AmLine8, sx + AmLine16, sy + AmLine8, 144);
		DrawLine(sx - AmLine16 - AmLine8, sy - AmLine4, sx + AmLine8, sy + AmLine8 + AmLine4, 144);
		DrawLine(sx - AmLine32, sy, sx, sy + AmLine16, 144);
	}

	AMLWallFlag = FALSE;
	AMRWallFlag = FALSE;
	AMLLWallFlag = FALSE;
	AMLRWallFlag = FALSE;

	switch(tile & 0xF) {
	case 1:
		x1 = sx - AmLine16;
		y1 = sy - AmLine16;
		x2 = x1 + AmLine32;
		y2 = sy - AmLine8;
		DrawLine(sx, y1, x1, y2, 200);
		DrawLine(sx, y1, x2, y2, 200);
		DrawLine(sx, sy, x1, y2, 200);
		DrawLine(sx, sy, x2, y2, 200);
		break;
	case 2:
	case 5:
		AMLWallFlag = TRUE;
		break;
	case 3:
	case 6:
		AMRWallFlag = TRUE;
		break;
	case 4:
		AMLWallFlag = TRUE;
		AMRWallFlag = TRUE;
		break;
	case 8:
		AMLWallFlag = TRUE;
		AMLLWallFlag = TRUE;
		break;
	case 9:
		AMRWallFlag = TRUE;
		AMLRWallFlag = TRUE;
		break;
	case 0xA:
		AMLLWallFlag = TRUE;
		break;
	case 0xB:
		AMLRWallFlag = TRUE;
		break;
	case 0xC:
		AMLLWallFlag = TRUE;
		AMLRWallFlag = TRUE;
		break;
	}

	if(AMLWallFlag) {
		if(f & AFLAG_VERTDOOR) {
			x1 = sx - AmLine32;
			x2 = sx - AmLine16;
			y1 = sy - AmLine16;
			y2 = sy - AmLine8;
			DrawLine(sx, y1, sx - AmLine8, y1 + AmLine4, 200);
			DrawLine(x1, sy, x1 + AmLine8, sy - AmLine4, 200);
			DrawLine(x2, y1, x1, y2, 144);
			DrawLine(x2, y1, sx, y2, 144);
			DrawLine(x2, sy, x1, y2, 144);
			DrawLine(x2, sy, sx, y2, 144);
		}
		if(f & AFLAG_VERTGRATE) {
			DrawLine(sx - AmLine16, sy - AmLine8, sx - AmLine32, sy, 200);
			f |= AFLAG_VERTARCH;
		}
		if(f & AFLAG_VERTARCH) {
			x1 = sx - AmLine16;
			y1 = sy - AmLine16;
			x2 = x1 + AmLine32;
			y2 = sy - AmLine8;
			DrawLine(sx, y1, x1, y2, 200);
			DrawLine(sx, y1, x2, y2, 200);
			DrawLine(sx, sy, x1, y2, 200);
			DrawLine(sx, sy, x2, y2, 200);
		}
		if(!(f & (AFLAG_VERTGRATE | AFLAG_VERTARCH | AFLAG_VERTDOOR))) {
			DrawLine(sx, sy - AmLine16, sx - AmLine32, sy, 200);
		}
	}
	if(AMRWallFlag) {
		if(f & AFLAG_HORZDOOR) {
			x1 = sx + AmLine16;
			x2 = sx + AmLine32;
			y1 = sy - AmLine16;
			y2 = sy - AmLine8;
			DrawLine(sx, y1, sx + AmLine8, y1 + AmLine4, 200);
			DrawLine(x2, sy, x2 - AmLine8, sy - AmLine4, 200);
			DrawLine(x1, y1, sx, y2, 144);
			DrawLine(x1, y1, x2, y2, 144);
			DrawLine(x1, sy, sx, y2, 144);
			DrawLine(x1, sy, x2, y2, 144);
		}
		if(f & AFLAG_HORZGRATE) {
			DrawLine(sx + AmLine16, sy - AmLine8, sx + AmLine32, sy, 200);
			f |= AFLAG_HORZARCH;
		}
		if(f & AFLAG_HORZARCH) {
			x1 = sx - AmLine16;
			y1 = sy - AmLine16;
			x2 = x1 + AmLine32;
			y2 = sy - AmLine8;
			DrawLine(sx, y1, x1, y2, 200);
			DrawLine(sx, y1, x2, y2, 200);
			DrawLine(sx, sy, x1, y2, 200);
			DrawLine(sx, sy, x2, y2, 200);
		}
		if(!(f & (AFLAG_HORZGRATE | AFLAG_HORZARCH | AFLAG_HORZDOOR))) {
			DrawLine(sx, sy - AmLine16, sx + AmLine32, sy, 200);
		}
	}
	if(AMLLWallFlag) {
		if(f & AFLAG_VERTDOOR) {
			x1 = sx - AmLine32;
			x2 = sx - AmLine16;
			y1 = sy + AmLine16;
			y2 = sy + AmLine8;
			DrawLine(sx, y1, sx - AmLine8, y1 - AmLine4, 200);
			DrawLine(x1, sy, x1 + AmLine8, sy + AmLine4, 200);
			DrawLine(x2, y1, x1, y2, 144);
			DrawLine(x2, y1, sx, y2, 144);
			DrawLine(x2, sy, x1, y2, 144);
			DrawLine(x2, sy, sx, y2, 144);
		} else {
			DrawLine(sx, sy + AmLine16, sx - AmLine32, sy, 200);
		}
	}
	if(AMLRWallFlag) {
		if(f & AFLAG_HORZDOOR) {
			x1 = sx + AmLine16;
			x2 = sx + AmLine32;
			y1 = sy + AmLine16;
			y2 = sy + AmLine8;
			DrawLine(sx, y1, sx + AmLine8, y1 - AmLine4, 200);
			DrawLine(x2, sy, x2 - AmLine8, sy + AmLine4, 200);
			DrawLine(x1, y1, sx, y2, 144);
			DrawLine(x1, y1, x2, y2, 144);
			DrawLine(x1, sy, sx, y2, 144);
			DrawLine(x1, sy, x2, y2, 144);
		} else {
			DrawLine(sx, sy + AmLine16, sx + AmLine32, sy, 200);
		}
	}
}

void DrawAutomapPlr()
{
	int Px, Py, MapX, MapY, x, y;

	if(plr[myplr]._pmode == PM_WALK3) {
		Px = plr[myplr]._px;
		Py = plr[myplr]._py;
		if(plr[myplr]._pdir == DIR_W) {
			Px++;
		} else {
			Py++;
		}
	} else {
		Px = plr[myplr].WorldX;
		Py = plr[myplr].WorldY;
	}

	MapX = Px - ViewX - 2 * AutoMapXOfs;
	MapY = Py - ViewY - 2 * AutoMapYOfs;
	x = 640 / 2 + SCREEN_X + MapX * AmLine16 - MapY * AmLine16;
	y = 352 / 2 + SCREEN_Y + MapX * AmLine8 + MapY * AmLine8;
	x += plr[myplr]._pxoff * AutoMapScale / 100 >> 1;
	y += plr[myplr]._pyoff * AutoMapScale / 100 >> 1;
	x += ScrollInfo._sxoff * AutoMapScale / 100 >> 1;
	y += ScrollInfo._syoff * AutoMapScale / 100 >> 1;

	if(invflag || sbookflag) {
		x -= 160;
	}
	if(chrflag || questlog) {
		x += 160;
	}

	y -= AmLine8;

	switch(plr[myplr]._pdir) {
	case DIR_N:
		DrawLine(x, y, x, y - AmLine16, 153);
		DrawLine(x, y - AmLine16, x - AmLine4, y - AmLine8, 153);
		DrawLine(x, y - AmLine16, x + AmLine4, y - AmLine8, 153);
		break;
	case DIR_NE:
		DrawLine(x, y, x + AmLine16, y - AmLine8, 153);
		DrawLine(x + AmLine16, y - AmLine8, x + AmLine8, y - AmLine8, 153);
		DrawLine(x + AmLine16, y - AmLine8, x + AmLine8 + AmLine4, y, 153);
		break;
	case DIR_E:
		DrawLine(x, y, x + AmLine16, y, 153);
		DrawLine(x + AmLine16, y, x + AmLine8, y - AmLine4, 153);
		DrawLine(x + AmLine16, y, x + AmLine8, y + AmLine4, 153);
		break;
	case DIR_SE:
		DrawLine(x, y, x + AmLine16, y + AmLine8, 153);
		DrawLine(x + AmLine16, y + AmLine8, x + AmLine8 + AmLine4, y, 153);
		DrawLine(x + AmLine16, y + AmLine8, x + AmLine8, y + AmLine8, 153);
		break;
	case DIR_S:
		DrawLine(x, y, x, y + AmLine16, 153);
		DrawLine(x, y + AmLine16, x + AmLine4, y + AmLine8, 153);
		DrawLine(x, y + AmLine16, x - AmLine4, y + AmLine8, 153);
		break;
	case DIR_SW:
		DrawLine(x, y, x - AmLine16, y + AmLine8, 153);
		DrawLine(x - AmLine16, y + AmLine8, x - AmLine4 - AmLine8, y, 153);
		DrawLine(x - AmLine16, y + AmLine8, x - AmLine8, y + AmLine8, 153);
		break;
	case DIR_W:
		DrawLine(x, y, x - AmLine16, y, 153);
		DrawLine(x - AmLine16, y, x - AmLine8, y - AmLine4, 153);
		DrawLine(x - AmLine16, y, x - AmLine8, y + AmLine4, 153);
		break;
	case DIR_NW:
		DrawLine(x, y, x - AmLine16, y - AmLine8, 153);
		DrawLine(x - AmLine16, y - AmLine8, x - AmLine8, y - AmLine8, 153);
		DrawLine(x - AmLine16, y - AmLine8, x - AmLine4 - AmLine8, y, 153);
		break;
	}
}

WORD GetAutomapType(int x, int y, BOOL view)
{
	WORD rv, f;

	if(view && x == -1 && y >= 0 && y < DMAXY && automapview[0][y]) {
		if(GetAutomapType(0, y, FALSE) & 0x4000) {
			return 0;
		} else {
			return 0x4000;
		}
	}
	if(view && y == -1 && x >= 0 && x < DMAXX && automapview[x][0]) {
		if(GetAutomapType(x, 0, FALSE) & 0x4000) {
			return 0;
		} else {
			return 0x4000;
		}
	}

	if(x < 0 || x >= DMAXX) {
		return 0;
	}
	if(y < 0 || y >= DMAXY) {
		return 0;
	}
	if(!automapview[x][y] && view) {
		return 0;
	}

	rv = automaptype[dungeon[x][y]];
	if(rv == 7) {
		f = GetAutomapType(x - 1, y, FALSE) >> 8;
		if(f & AFLAG_HORZARCH) {
			f = GetAutomapType(x, y - 1, FALSE) >> 8;
			if(f & AFLAG_VERTARCH) {
				rv = 1;
			}
		}
	}
	return rv;
}

void DrawAutomapText()
{
	int y;
	char gamestr[256];

	y = 20;
	if(gbMaxPlayers > 1) {
		strcat(strcpy(gamestr, "game: "), szPlayerName);
		PrintGameStr(8, y, gamestr, COL_GOLD);
		y += 15;
		if(szPlayerDescript[0] != '\0') {
			strcat(strcpy(gamestr, "password: "), szPlayerDescript);
			PrintGameStr(8, y, gamestr, COL_GOLD);
			y += 15;
		}
	}

	if(setlevel) {
		PrintGameStr(8, y, quest_level_names[setlvlnum], COL_GOLD);
	} else if(currlevel != 0) {
		sprintf(gamestr, "Level: %i", currlevel);
		PrintGameStr(8, y, gamestr, COL_GOLD);
	}
}

void SetAutomapView(int x, int y)
{
	int xx, yy;
	WORD s, d;

	xx = (x - 16) >> 1;
	yy = (y - 16) >> 1;

	if(xx < 0 || xx >= DMAXX) {
		return;
	}
	if(yy < 0 || yy >= DMAXY) {
		return;
	}

	automapview[xx][yy] = TRUE;
	d = GetAutomapType(xx, yy, FALSE);
	s = d & 0x4000;
	d &= 0xF;

	switch(d) {
	case 2:
		if(s) {
			if(GetAutomapType(xx, yy + 1, FALSE) == 0x4007) {
				automapview[xx][yy + 1] = TRUE;
			}
		} else {
			if(GetAutomapType(xx - 1, yy, FALSE) & 0x4000) {
				automapview[xx - 1][yy] = TRUE;
			}
		}
		break;
	case 3:
		if(s) {
			if(GetAutomapType(xx + 1, yy, FALSE) == 0x4007) {
				automapview[xx + 1][yy] = TRUE;
			}
		} else {
			if(GetAutomapType(xx, yy - 1, FALSE) & 0x4000) {
				automapview[xx][yy - 1] = TRUE;
			}
		}
		break;
	case 4:
		if(s) {
			if(GetAutomapType(xx, yy + 1, FALSE) == 0x4007) {
				automapview[xx][yy + 1] = TRUE;
			}
			if(GetAutomapType(xx + 1, yy, FALSE) == 0x4007) {
				automapview[xx + 1][yy] = TRUE;
			}
		} else {
			if(GetAutomapType(xx - 1, yy, FALSE) & 0x4000) {
				automapview[xx - 1][yy] = TRUE;
			}
			if(GetAutomapType(xx, yy - 1, FALSE) & 0x4000) {
				automapview[xx][yy - 1] = TRUE;
			}
			if(GetAutomapType(xx - 1, yy - 1, FALSE) & 0x4000) {
				automapview[xx - 1][yy - 1] = TRUE;
			}
		}
		break;
	case 5:
		if(s) {
			if(GetAutomapType(xx, yy - 1, FALSE) & 0x4000) {
				automapview[xx][yy - 1] = TRUE;
			}
			if(GetAutomapType(xx, yy + 1, FALSE) == 0x4007) {
				automapview[xx][yy + 1] = TRUE;
			}
		} else {
			if(GetAutomapType(xx - 1, yy, FALSE) & 0x4000) {
				automapview[xx - 1][yy] = TRUE;
			}
		}
		break;
	case 6:
		if(s) {
			if(GetAutomapType(xx - 1, yy, FALSE) & 0x4000) {
				automapview[xx - 1][yy] = TRUE;
			}
			if(GetAutomapType(xx + 1, yy, FALSE) == 0x4007) {
				automapview[xx + 1][yy] = TRUE;
			}
		} else {
			if(GetAutomapType(xx, yy - 1, FALSE) & 0x4000) {
				automapview[xx][yy - 1] = TRUE;
			}
		}
		break;
	}
}

void AutomapZoomReset()
{
	AmLine64 = (AutoMapScale << 6) / 100;
	AmLine32 = AmLine64 >> 1;
	AmLine16 = AmLine32 >> 1;
	AmLine8 = AmLine16 >> 1;
	AmLine4 = AmLine8 >> 1;
	AutoMapXOfs = 0;
	AutoMapYOfs = 0;
}
