//HEADER_GOES_HERE
#ifndef __MSG_H__
#define __MSG_H__

extern int sgdwOwnerWait; // weak
extern int sgdwRecvOffset; // idb
extern int sgnCurrMegaPlayer; // weak
extern DLevel sgLevels[NUMLEVELS];
extern TMegaPkt *sgpCurrPkt;
extern BYTE sgRecvBuf[4722];
extern unsigned char sgbRecvCmd; // idb
extern LocalLevel sgLocals[NUMLEVELS];
extern DJunk sgJunk;
extern TMegaPkt *sgpMegaPkt;
extern char sgbDeltaChanged; // weak
extern BYTE sgbDeltaChunks; // weak
extern int deltaload; // weak
extern char gbBufferMsgs; // weak
extern int dwRecCount; // weak
extern int msg_err_timer; // weak

void msg_send_drop_pkt(int pnum, DWORD reason);
void msg_send_packet(int pnum, const void *pMsg, DWORD dwLen);
void msg_get_next_packet();
BOOL msg_wait_resync();
void msg_free_packets();
int msg_wait_for_turns();
void run_delta_info();
void msg_pre_packet();
void DeltaExportData(int pnum);
BYTE *DeltaExportItem(BYTE *dst, TCmdPItem *src);
BYTE *DeltaExportObject(BYTE *dst, DObjectStr *src);
BYTE *DeltaExportMonster(BYTE *dst, DMonsterStr *src);
BYTE *DeltaExportJunk(BYTE *dst);
DWORD msg_comp_level(BYTE *p, BYTE *dst);
void delta_init();
void delta_kill_monster(int mi, BYTE x, BYTE y, BYTE bLevel);
void delta_monster_hp(int mi, long hp, BYTE bLevel);
void delta_sync_monster(const TSyncMonster *pSync, BYTE bLevel);
void delta_sync_golem(const TCmdGolem *pG, int pnum, BYTE bLevel);
void delta_leave_sync(BYTE bLevel);
BOOL delta_portal_inited(int i);
BOOL delta_quest_inited(int i);
void DeltaAddItem(int ii);
void DeltaSaveLevel();
void DeltaLoadLevel();
void NetSendCmd(BOOL bHiPri, BYTE bCmd);
void NetSendCmdGolem(BYTE mx, BYTE my, BYTE dir, BYTE menemy, long hp, BYTE cl);
void NetSendCmdLoc(BOOL bHiPri, BYTE bCmd, BYTE x, BYTE y);
void NetSendCmdLocParam1(BOOL bHiPri, BYTE bCmd, BYTE x, BYTE y, WORD wParam1);
void NetSendCmdLocParam2(BOOL bHiPri, BYTE bCmd, BYTE x, BYTE y, WORD wParam1, WORD wParam2);
void NetSendCmdLocParam3(BOOL bHiPri, BYTE bCmd, BYTE x, BYTE y, WORD wParam1, WORD wParam2, WORD wParam3);
void NetSendCmdParam1(BOOL bHiPri, BYTE bCmd, WORD wParam1);
void NetSendCmdParam2(BOOL bHiPri, BYTE bCmd, WORD wParam1, WORD wParam2);
void NetSendCmdParam3(BOOL bHiPri, BYTE bCmd, WORD wParam1, WORD wParam2, WORD wParam3);
void NetSendCmdQuest(BOOL bHiPri, BYTE q);
void NetSendCmdGItem(BOOL bHiPri, BYTE bCmd, BYTE mast, BYTE pnum, BYTE ii);
void NetSendCmdGItem2(BOOL usonly, BYTE bCmd, BYTE mast, BYTE pnum, const TCmdGItem *p);
BOOL NetSendCmdReq2(BYTE bCmd, BYTE mast, BYTE pnum, const TCmdGItem *p);
void NetSendCmdExtra(const TCmdGItem *p);
void NetSendCmdPItem(BOOL bHiPri, BYTE bCmd, BYTE x, BYTE y);
void NetSendCmdChItem(BOOL bHiPri, BYTE bLoc);
void NetSendCmdDelItem(BOOL bHiPri, BYTE bLoc);
void NetSendCmdDItem(BOOL bHiPri, int ii);
void NetSendCmdDamage(BOOL bHiPri, BYTE bPlr, DWORD dwDam);
void NetSendCmdString(int msk, const char *pszStr);
void delta_close_portal(int pnum);
DWORD ParseCmd(int pnum, const TCmd *pCmd);
DWORD On_DLEVEL(int pnum, const TCmd *pCmd);
void DeltaImportData(BYTE bCmd, DWORD dwReceive);
BYTE *DeltaImportItem(BYTE *src, TCmdPItem *dst);
BYTE *DeltaImportObject(BYTE *src, DObjectStr *dst);
BYTE *DeltaImportMonster(BYTE *src, DMonsterStr *dst);
void DeltaImportJunk(BYTE *src);
DWORD On_SYNCDATA(const TCmd *pCmd, int pnum);
DWORD On_WALKXY(const TCmd *pCmd, int pnum);
DWORD On_ADDSTR(const TCmd *pCmd, int pnum);
DWORD On_ADDMAG(const TCmd *pCmd, int pnum);
DWORD On_ADDDEX(const TCmd *pCmd, int pnum);
DWORD On_ADDVIT(const TCmd *pCmd, int pnum);
DWORD On_SBSPELL(const TCmd *pCmd, int pnum);
void __cdecl msg_errorf(const char *pszFmt, ...);
DWORD On_GOTOGETITEM(const TCmd *pCmd, int pnum);
DWORD On_REQUESTGITEM(const TCmd *pCmd, int pnum);
BOOL i_own_level(int nReqLevel);
DWORD On_GETITEM(const TCmd *pCmd, int pnum);
BOOL delta_get_item(const TCmdGItem *pI, BYTE bLevel);
DWORD On_GOTOAGETITEM(const TCmd *pCmd, int pnum);
DWORD On_REQUESTAGITEM(const TCmd *pCmd, int pnum);
DWORD On_AGETITEM(const TCmd *pCmd, int pnum);
DWORD On_ITEMEXTRA(const TCmd *pCmd, int pnum);
DWORD On_PUTITEM(const TCmd *pCmd, int pnum);
void delta_put_item(const TCmdPItem *pI, int x, int y, BYTE bLevel);
void check_update_plr(int pnum);
DWORD On_SYNCPUTITEM(const TCmd *pCmd, int pnum);
DWORD On_RESPAWNITEM(const TCmd *pCmd, int pnum);
DWORD On_ATTACKXY(const TCmd *pCmd, int pnum);
DWORD On_SATTACKXY(const TCmd *pCmd, int pnum);
DWORD On_RATTACKXY(const TCmd *pCmd, int pnum);
DWORD On_SPELLXYD(const TCmd *pCmd, int pnum);
DWORD On_SPELLXY(const TCmd *pCmd, int pnum);
DWORD On_TSPELLXY(const TCmd *pCmd, int pnum);
DWORD On_OPOBJXY(const TCmd *pCmd, int pnum);
DWORD On_DISARMXY(const TCmd *pCmd, int pnum);
DWORD On_OPOBJT(const TCmd *pCmd, int pnum);
DWORD On_ATTACKID(const TCmd *pCmd, int pnum);
DWORD On_ATTACKPID(const TCmd *pCmd, int pnum);
DWORD On_RATTACKID(const TCmd *pCmd, int pnum);
DWORD On_RATTACKPID(const TCmd *pCmd, int pnum);
DWORD On_SPELLID(const TCmd *pCmd, int pnum);
DWORD On_SPELLPID(const TCmd *pCmd, int pnum);
DWORD On_TSPELLID(const TCmd *pCmd, int pnum);
DWORD On_TSPELLPID(const TCmd *pCmd, int pnum);
DWORD On_KNOCKBACK(const TCmd *pCmd, int pnum);
DWORD On_RESURRECT(const TCmd *pCmd, int pnum);
DWORD On_HEALOTHER(const TCmd *pCmd, int pnum);
DWORD On_TALKXY(const TCmd *pCmd, int pnum);
DWORD On_NEWLVL(const TCmd *pCmd, int pnum);
DWORD On_WARP(const TCmd *pCmd, int pnum);
DWORD On_MONSTDEATH(const TCmd *pCmd, int pnum);
DWORD On_KILLGOLEM(const TCmd *pCmd, int pnum);
DWORD On_AWAKEGOLEM(const TCmd *pCmd, int pnum);
DWORD On_MONSTDAMAGE(const TCmd *pCmd, int pnum);
DWORD On_PLRDEAD(const TCmd *pCmd, int pnum);
DWORD On_PLRDAMAGE(const TCmd *pCmd, int pnum);
DWORD On_OPENDOOR(const TCmd *pCmd, int pnum);
void delta_sync_object(int oi, BYTE bCmd, BYTE bLevel);
DWORD On_CLOSEDOOR(const TCmd *pCmd, int pnum);
DWORD On_OPERATEOBJ(const TCmd *pCmd, int pnum);
DWORD On_PLROPOBJ(const TCmd *pCmd, int pnum);
DWORD On_BREAKOBJ(const TCmd *pCmd, int pnum);
DWORD On_CHANGEPLRITEMS(const TCmd *pCmd, int pnum);
DWORD On_DELPLRITEMS(const TCmd *pCmd, int pnum);
DWORD On_PLRLEVEL(const TCmd *pCmd, int pnum);
DWORD On_DROPITEM(const TCmd *pCmd, int pnum);
DWORD On_SEND_PLRINFO(const TCmd *pCmd, int pnum);
DWORD On_ACK_PLRINFO(const TCmd *pCmd, int pnum);
DWORD On_PLAYER_JOINLEVEL(const TCmd *pCmd, int pnum);
DWORD On_ACTIVATEPORTAL(const TCmd *pCmd, int pnum);
void delta_open_portal(int pnum, BYTE x, BYTE y, BYTE bLevel, BYTE bLType, BYTE bSetLvl);
DWORD On_DEACTIVATEPORTAL(const TCmd *pCmd, int pnum);
DWORD On_RETOWN(const TCmd *pCmd, int pnum);
DWORD On_SETSTR(const TCmd *pCmd, int pnum);
DWORD On_SETDEX(const TCmd *pCmd, int pnum);
DWORD On_SETMAG(const TCmd *pCmd, int pnum);
DWORD On_SETVIT(const TCmd *pCmd, int pnum);
DWORD On_STRING(const TCmd *pCmd, int pnum);
DWORD On_STRING2(int pnum, const TCmdString *p);
DWORD On_SYNCQUEST(const TCmd *pCmd, int pnum);
DWORD On_ENDSHIELD(const TCmd *pCmd, int pnum);
DWORD On_CHEAT_EXPERIENCE(const TCmd *pCmd, int pnum);
DWORD On_CHEAT_SPELL_LEVEL(const TCmd *pCmd, int pnum);
DWORD On_DEBUG(const TCmd *pCmd, int pnum);
DWORD On_NOVA(const TCmd *pCmd, int pnum);
DWORD On_SETSHIELD(const TCmd *pCmd, int pnum);
DWORD On_REMSHIELD(const TCmd *pCmd, int pnum);

#endif /* __MSG_H__ */
