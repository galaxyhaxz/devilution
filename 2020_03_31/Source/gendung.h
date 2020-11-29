//HEADER_GOES_HERE
#ifndef __GENDUNG_H__
#define __GENDUNG_H__

extern unsigned char dungeon[40][40];
extern BYTE pdungeon[40][40];
extern char dflags[40][40];
extern int setpc_x; // idb
extern int setpc_y; // idb
extern int setpc_w; // weak
extern int setpc_h; // weak
extern char *pSetPiece_2;
extern int setloadflag_2; // weak
extern BYTE *pSpecialCels;
extern BYTE *pMegaTiles;
extern BYTE *pLevelPieces;
extern BYTE *pDungeonCels;
extern BYTE *pSpeedCels;
extern int SpeedFrameTbl[128][16];
extern char block_lvid[2049];
extern int level_frame_count[MAXTILES];
extern int tile_defs[MAXTILES];
extern WORD level_frame_types[MAXTILES];
extern int level_frame_sizes[MAXTILES];
extern int nlevel_frames; // weak
extern BOOLEAN nBlockTable[2049];
extern BOOLEAN nSolidTable[2049];
extern BOOLEAN nTransTable[2049];
extern BOOLEAN nMissileTable[2049];
extern BOOLEAN nTrapTable[2049];
extern int dminx; // weak
extern int dminy; // weak
extern int dmaxx; // weak
extern int dmaxy; // weak
extern int gnDifficulty; // idb
extern unsigned char leveltype; // weak
extern unsigned char currlevel; // idb
extern char setlevel; // weak
extern BYTE setlvlnum; // weak
extern char setlvltype; // weak
extern int ViewX; // idb
extern int ViewY; // idb
extern int ViewDX; // weak
extern int ViewDY; // weak
extern int ViewBX; // weak
extern int ViewBY; // weak
extern ScrollStruct ScrollInfo;
extern int LvlViewX; // weak
extern int LvlViewY; // weak
extern int MicroTileLen;
extern char TransVal; // weak
extern char TransList[256];
extern int dPiece[MAXDUNX][MAXDUNY];
extern MICROS dpiece_defs_map_2[MAXDUNX][MAXDUNY];
extern MICROS dpiece_defs_map_1[MAXDUNX * MAXDUNY];
extern char dTransVal[MAXDUNX][MAXDUNY];
extern char dLight[MAXDUNX][MAXDUNY];
extern char dPreLight[MAXDUNX][MAXDUNY];
extern char dFlags[MAXDUNX][MAXDUNY];
extern char dPlayer[MAXDUNX][MAXDUNY];
extern int dMonster[MAXDUNX][MAXDUNY];
extern char dDead[MAXDUNX][MAXDUNY];
extern char dObject[MAXDUNX][MAXDUNY];
extern char dItem[MAXDUNX][MAXDUNY];
extern char dMissile[MAXDUNX][MAXDUNY];
extern char dSpecial[MAXDUNX][MAXDUNY];
extern int themeCount;
extern THEME_LOC themeLoc[MAXTHEMES];

void FillSolidBlockTbls();
void MakeSpeedCels();
int IsometricCoord(int x, int y);
void SetSpeedCels();
void SetDungeonMicros();
void DRLG_InitTrans();
void DRLG_MRectTrans(int x1, int y1, int x2, int y2);
void DRLG_RectTrans(int x1, int y1, int x2, int y2);
void DRLG_CopyTrans(int sx, int sy, int dx, int dy);
void DRLG_ListTrans(int num, unsigned char *List);
void DRLG_AreaTrans(int num, unsigned char *List);
void DRLG_InitSetPC();
void DRLG_SetPC();
void Make_SetPC(int x, int y, int w, int h);
BOOL DRLG_WillThemeRoomFit(int floor, int x, int y, int minSize, int maxSize, int *width, int *height);
void DRLG_CreateThemeRoom(int themeIndex);
void DRLG_PlaceThemeRooms(int minSize, int maxSize, int floor, int freq, int rndSize);
void DRLG_HoldThemeRooms();
BOOL SkipThemeRoom(int x, int y);
void InitLevels();

#endif /* __GENDUNG_H__ */
