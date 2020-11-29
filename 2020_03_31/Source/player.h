//HEADER_GOES_HERE
#ifndef __PLAYER_H__
#define __PLAYER_H__

extern int plr_lframe_size; // idb
extern int plr_wframe_size; // idb
extern BYTE plr_gfx_flag; // weak
extern int plr_aframe_size; // idb
extern int myplr;
extern PlayerStruct plr[MAX_PLRS];
extern int plr_fframe_size; // idb
extern int plr_qframe_size; // idb
extern int deathflag; // idb
extern int plr_hframe_size; // idb
extern int plr_bframe_size; // idb
extern char plr_gfx_bflag; // weak
extern int plr_sframe_size; // idb
extern int deathdelay; // weak
extern int plr_dframe_size; // idb

void SetPlayerGPtrs(BYTE *pData, BYTE **pAnim);
void LoadPlrGFX(int pnum, DWORD dwMask);
void InitPlayerGFX(int pnum);
void InitPlrGFXMem(int pnum);
DWORD GetPlrGFXSize(char *szCel);
void FreePlayerGFX(int pnum);
void NewPlrAnim(int pnum, unsigned char *Peq, int numFrames, int Delay, int width);
void ClearPlrPVars(int pnum);
void SetPlrAnims(int pnum);
void ClearPlrRVars(PlayerStruct *p);
void CreatePlayer(int pnum, char c);
int CalcStatDiff(int pnum);
void NextPlrLevel(int pnum);
void AddPlrExperience(int pnum, int lvl, long exp);
void AddPlrMonstExper(int lvl, long exp, char pmask);
void InitPlayer(int pnum, BOOL FirstTime);
void InitMultiView();
void CheckEFlag(int pnum, BOOL flag);
BOOL SolidLoc(int x, int y);
BOOL PlrDirOK(int pnum, int dir);
void PlrClrTrans(int x, int y);
void PlrDoTrans(int x, int y);
void SetPlayerOld(int pnum);
void FixPlayerLocation(int pnum, int bDir);
void StartStand(int pnum, int dir);
void StartWalkStand(int pnum);
void PM_ChangeLightOff(int pnum);
void PM_ChangeOffset(int pnum);
void StartWalk(int pnum, int xvel, int yvel, int xadd, int yadd, int EndDir, int sdir);
void StartWalk2(int pnum, int xvel, int yvel, int xoff, int yoff, int xadd, int yadd, int EndDir, int sdir);
void StartWalk3(int pnum, int xvel, int yvel, int xoff, int yoff, int xadd, int yadd, int mapx, int mapy, int EndDir, int sdir);
void StartAttack(int pnum, int d);
void StartRangeAttack(int pnum, int d, int cx, int cy);
void StartPlrBlock(int pnum, int dir);
void StartSpell(int pnum, int d, int cx, int cy);
void FixPlrWalkTags(int pnum);
void RemovePlrFromMap(int pnum);
void StartPlrHit(int pnum, int dam, unsigned char forcehit);
void RespawnDeadItem(ItemStruct *itm, int x, int y);
void StartPlayerKill(int pnum, int earflag);
void PlrDeadItem(int pnum, ItemStruct *itm, int xx, int yy);
void DropHalfPlayersGold(int pnum);
void SyncPlrKill(int pnum, int earflag);
void j_StartPlayerKill(int pnum, int earflag);
void RemovePlrMissiles(int pnum);
void InitLevelChange(int pnum);
void StartNewLvl(int pnum, int fom, int lvl);
void RestartTownLvl(int pnum);
void StartWarpLvl(int pnum, int pidx);
int PM_DoStand(int pnum);
int PM_DoWalk(int pnum);
int PM_DoWalk2(int pnum);
int PM_DoWalk3(int pnum);
BOOL WeaponDur(int pnum, int durrnd);
BOOL PlrHitMonst(int pnum, int m);
BOOL PlrHitPlr(int pnum, char p);
BOOL PlrHitObj(int pnum, int mx, int my);
int PM_DoAttack(int pnum);
int PM_DoRangeAttack(int pnum);
void ShieldDur(int pnum);
int PM_DoBlock(int pnum);
int PM_DoSpell(int pnum);
int PM_DoGotHit(int pnum);
void ArmorDur(int pnum);
int PM_DoDeath(int pnum);
int PM_DoNewLvl(int pnum);
void CheckNewPath(int pnum);
BOOL PlrDeathModeOK(int p);
void ValidatePlayer();
void ProcessPlayers();
void CheckCheatStats(int pnum);
void ClrPlrPath(int pnum);
BOOL PosOkPlayer(int pnum, int px, int py);
void MakePlrPath(int pnum, int xx, int yy, BOOL endspace);
void CheckPlrSpell();
void SyncPlrAnim(int pnum);
void SyncInitPlrPos(int pnum);
void SyncInitPlr(int pnum);
void CheckStats(int pnum);
void ModifyPlrStr(int pnum, int l);
void ModifyPlrMag(int pnum, int l);
void ModifyPlrDex(int pnum, int l);
void ModifyPlrVit(int pnum, int l);
void SetPlayerHitPoints(int pnum, int newhp);
void SetPlrStr(int pnum, int v);
void SetPlrMag(int pnum, int v);
void SetPlrDex(int pnum, int v);
void SetPlrVit(int pnum, int v);
void InitDungMsgs(int pnum);
void PlayDungMsgs();

/* rdata */

extern const char ArmourChar[4];
extern const char WepChar[10];
extern const char CharChar[4];

/* data */

extern int plrxoff[9];
extern int plryoff[9];
extern int plrxoff2[9];
extern int plryoff2[9];
extern char PlrGFXAnimLens[3][11];
extern int PWVel[3][3];
extern int WalkAnimTbl[3];
extern int StrengthTbl[3];
extern int MagicTbl[3];
extern int DexterityTbl[3];
extern int VitalityTbl[3];
extern int ToBlkTbl[3];
extern char *ClassStrTblOld[3];
extern int MaxStats[3][4];
extern int ExpLvlsTbl[51];
extern char *ClassStrTbl[3];

#endif /* __PLAYER_H__ */
