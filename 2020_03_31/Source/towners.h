//HEADER_GOES_HERE
#ifndef __TOWNERS_H__
#define __TOWNERS_H__

extern int storeflag; // weak
extern int sgnCowMsg; // weak
extern int numtowners; // idb
extern DWORD sgdwCowClicks; // weak
extern BOOL bannerflag; // weak // unused 0x6AAC28
extern BOOL boyloadflag; // weak
extern BYTE *pCowCels; // idb
extern TownerStruct towner[16];

int GetActiveTowner(int t);
void SetTownerGPtrs(BYTE *pData, BYTE **pAnim);
void NewTownerAnim(int tnum, unsigned char *pAnim, int numFrames, int Delay);
void InitTownerInfo(int i, long w, BOOL sel, int t, int x, int y, char ao, int tp);
void InitQstSnds(int i);
void InitSmith();
void InitBarOwner();
void InitTownDead();
void InitWitch();
void InitBarmaid();
void InitBoy();
void InitHealer();
void InitTeller();
void InitDrunk();
void InitCows();
void InitTowners();
void FreeTownerGFX();
void TownCtrlMsg(int i);
void TownBlackSmith();
void TownBarOwner();
void TownDead();
void TownHealer();
void TownStory();
void TownDrunk();
void TownBoy();
void TownWitch();
void TownBarMaid();
void TownCow();
void ProcessTowners();
ItemStruct *PlrHasItem(int pnum, int item, int &i);
void TownerTalk(int first, int t);
void TalkToTowner(int p, int t);
void CowSFX(int pnum);

/* data */

extern char AnimOrder[6][148];
extern int TownCowX[3];
extern int TownCowY[3];
extern int TownCowDir[3];
extern int cowoffx[8];
extern int cowoffy[8];
extern int Qtalklist[11][16];
extern int CowPlaying;

#endif /* __TOWNERS_H__ */
