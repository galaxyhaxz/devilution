//HEADER_GOES_HERE
#ifndef __AUTOMAP_H__
#define __AUTOMAP_H__

extern WORD automaptype[512];
extern int AutoMapX; // weak
extern int AutoMapY; // weak
extern BOOL automapflag; // idb
extern char AmShiftTab[31];
extern BOOLEAN automapview[40][40];
extern int AutoMapScale; // idb
extern int AutoMapXOfs; // weak
extern int AutoMapYOfs; // weak
extern int AmLine64; // weak
extern int AmLine32; // weak
extern int AmLine16; // weak
extern int AmLine8; // weak
extern int AmLine4; // weak

void InitAutomapOnce();
void InitAutomap();
void StartAutomap();
void AutomapUp();
void AutomapDown();
void AutomapLeft();
void AutomapRight();
void AutomapZoomIn();
void AutomapZoomOut();
void DrawAutomap();
void DrawAutomapTile(int sx, int sy, WORD tile);
void DrawAutomapPlr();
WORD GetAutomapType(int x, int y, BOOL view);
void DrawAutomapText();
void SetAutomapView(int x, int y);
void AutomapZoomReset();

#endif /* __AUTOMAP_H__ */
