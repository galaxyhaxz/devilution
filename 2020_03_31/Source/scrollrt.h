//HEADER_GOES_HERE
#ifndef __SCROLLRT_H__
#define __SCROLLRT_H__

extern int light_table_index; // weak
extern int PitchTbl[1024];
extern DWORD sgdwCursWdtOld; // idb
extern DWORD sgdwCursX; // idb
extern DWORD sgdwCursY; // idb
extern unsigned char *gpBufEnd; // weak
extern DWORD sgdwCursHgt;
extern int level_cel_block; // weak
extern DWORD sgdwCursXOld; // idb
extern DWORD sgdwCursYOld; // idb
extern char arch_draw_type; // weak
extern DDSURFACEDESC DDS_desc;
extern int cel_transparency_active; // weak
extern int level_piece_id; // weak
extern DWORD sgdwCursWdt;
extern void (*DrawPlrProc)(int, int, int, int, int, BYTE *, int, int, int, int);
extern BYTE sgSaveBack[8192];
extern int draw_monster_num; // weak
extern DWORD sgdwCursHgtOld; // idb

void ClearCursor();
void DrawMissile(int x, int y, int sx, int sy, int a5, int a6, BOOL pre);
void DrawClippedMissile(int x, int y, int sx, int sy, int a5, int a6, BOOL pre);
void DrawDeadPlayer(int x, int y, int sx, int sy, int a5, int a6, BOOL clipped);
void DrawView(int StartX, int StartY);
void ClearScreenBuffer();
#ifdef _DEBUG
void ScrollView();
void EnableFrameCount();
#endif
void scrollrt_draw_game_screen(BOOL draw_cursor);
void DrawAndBlit();

/* data */

/* used in 1.00 debug */
extern char *szMonModeAssert[18];
extern char *szPlrModeAssert[12];

#endif /* __SCROLLRT_H__ */
