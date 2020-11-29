//HEADER_GOES_HERE
#ifndef __DRLG_L1_H__
#define __DRLG_L1_H__

extern char L5dungeon[80][80];
extern unsigned char L5dflags[40][40];
extern int setloadflag; // weak
extern int HR1;
extern int HR2;
extern int HR3;
extern int VR1;
extern int VR2;
extern int VR3;
extern void *pSetPiece; // idb

void DRLG_Init_Globals();
void LoadL1Dungeon(char *sFileName, int vx, int vy);
void LoadPreL1Dungeon(char *sFileName, int vx, int vy);
void CreateL5Dungeon(int rseed, int entry);

/* rdata */
extern const ShadowStruct SPATS[37];
extern const unsigned char BSTYPES[206];
extern const unsigned char L5BTYPES[206];
extern const unsigned char STAIRSUP[];
extern const unsigned char L5STAIRSUP[];
extern const unsigned char STAIRSDOWN[];
extern const unsigned char LAMPS[];
extern const unsigned char PWATERIN[];

/* data */
extern unsigned char L5ConvTbl[16];

#endif /* __DRLG_L1_H__ */
