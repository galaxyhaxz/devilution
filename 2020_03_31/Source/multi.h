//HEADER_GOES_HERE
#ifndef __MULTI_H__
#define __MULTI_H__

extern char gbSomebodyWonGameKludge; // weak
#ifdef _DEBUG
extern DWORD gdwHistTicks;
#endif
extern TBuffer sgHiPriBuf;
extern char szPlayerDescript[128];
extern WORD sgwPackPlrOffsetTbl[MAX_PLRS];
extern PkPlayerStruct netplr[MAX_PLRS];
extern BYTE sgbPlayerTurnBitTbl[MAX_PLRS];
extern char sgbPlayerLeftGameTbl[MAX_PLRS];
extern int sgbSentThisCycle; // idb
extern int dword_678628; // weak
extern char gbActivePlayers; // weak
extern char gbGameDestroyed; // weak
extern char sgbSendDeltaTbl[MAX_PLRS];
extern _gamedata sgGameInitInfo;
extern char gbSelectProvider; // weak
extern int sglTimeoutStart; // weak
extern int sgdwPlayerLeftReasonTbl[MAX_PLRS];
extern TBuffer sgLoPriBuf;
extern unsigned int sgdwGameLoops; // idb
extern unsigned char gbMaxPlayers; // weak
extern char sgbTimeout; // weak
extern char szPlayerName[128];
extern BYTE gbDeltaSender; // weak
extern int sgbNetInited; // weak
extern DWORD player_state[MAX_PLRS];

#ifdef _DEBUG
void __cdecl dumphist(const char *pszFmt, ...);
#endif
void multi_msg_add(const BYTE *pbMsg, BYTE bLen);
void NetSendLoPri(const BYTE *pbMsg, BYTE bLen);
void multi_copy_packet(TBuffer *pBuf, const BYTE *pbMsg, BYTE bLen);
void multi_send_packet(const BYTE *pbMsg, BYTE bLen);
void NetRecvPlrData(TPkt *pkt);
void NetSendHiPri(const BYTE *pbMsg, BYTE bLen);
BYTE *multi_recv_packet(TBuffer *pBuf, BYTE *p, DWORD *dwSize);
void multi_send_msg_packet(int msk, const BYTE *pbMsg, BYTE bLen);
void multi_msg_countdown();
void multi_parse_turn(int pnum, DWORD turn);
void multi_handle_turn_upper_bit(int pnum);
void multi_player_left(int pnum, DWORD reason);
void multi_clear_left_tbl();
void multi_player_left_msg(int pnum, BOOL left);
void multi_net_ping();
BOOL multi_handle_delta();
BOOL multi_check_pkt_valid(TBuffer *pBuf);
void multi_mon_seeds();
void multi_begin_timeout();
void multi_check_drop_player();
void multi_process_network_packets();
void multi_handle_all_packets(int pnum, BYTE *pData, DWORD dwSize);
void multi_process_tmsgs();
void multi_send_zero_packet(int pnum, BYTE bCmd, BYTE *pbSrc, DWORD dwLen);
void NetClose();
void multi_event_handler(BOOL reg);
void __stdcall multi_handle_events(_SNETEVENT *pEvt);
BOOL NetInit(BOOL bSinglePlayer, BOOL *pfExitProgram);
void buffer_init(TBuffer *pBuf);
void multi_send_pinfo(int pnum, BYTE cmd);
int InitLevelType(int l);
void SetupLocalCoords();
BOOL multi_init_single(_SNETPROGRAMDATA *progdata, _SNETPLAYERDATA *plrdata, _SNETUIDATA *uidata);
BOOL multi_init_multi(_SNETPROGRAMDATA *progdata, _SNETPLAYERDATA *plrdata, _SNETUIDATA *uidata, BOOL *pfExitProgram);
BOOL multi_upgrade(BOOL *pfExitProgram);
void recv_plrinfo(int pnum, TCmdPlrInfoHdr *p, BOOL recv);

/* rdata */

extern const int event_types[3];

#endif /* __MULTI_H__ */
