//HEADER_GOES_HERE
#ifndef __CONTROL_H__
#define __CONTROL_H__

extern BYTE sgbNextTalkSave; // weak
extern BYTE sgbTalkSavePos; // weak
extern void *pDurIcons;
extern void *pChrButtons;
extern int drawhpflag; // idb
extern int dropGoldFlag; // weak
extern int panbtn[8];
extern int chrbtn[4];
extern void *pMultiBtns;
extern void *pPanelButtons;
extern void *pChrPanel;
extern int lvlbtndown; // weak
extern char sgszTalkSave[8][80];
extern int dropGoldValue; // idb
extern int drawmanaflag; // idb
extern int chrbtnactive; // weak
extern char sgszTalkMsg[80];
extern BYTE *pPanelText;
extern int frame_4B8800; // idb
extern BYTE *pLifeBuff;
extern BYTE *pBtmBuff;
extern void *pTalkBtns;
extern int pstrjust[4];
extern int pnumlines; // idb
extern int pinfoflag; // weak
extern int talkbtndown[3];
extern int pSpell; // weak
extern BYTE *pManaBuff;
extern char infoclr; // weak
extern int sgbPlrTalkTbl; // weak // should be char [4]
extern void *pGBoxBuff;
extern void *pSBkBtnCel;
extern char tempstr[256];
extern char byte_4B894C[4];
extern int sbooktab; // weak
extern int pSplType; // weak
extern int frame; // idb
extern int initialDropGoldIndex; // idb
extern int talkflag; // weak
extern void *pSBkIconCels;
extern int sbookflag; // weak
extern int chrflag;
extern int drawbtnflag; // idb
extern void *pSpellBkCel;
extern char infostr[256];
extern int numpanbtns; // weak
extern void *pStatusPanel;
extern char panelstr[4][64];
extern int panelflag; // weak
extern BYTE SplTransTbl[256];
extern int initialDropGoldValue; // idb
extern void *pSpellCels;
extern int panbtndown; // weak
extern void *pTalkPanel; // idb
extern int spselflag; // weak

void DrawSpellCel(int xp, int yp, BYTE *pCelBuff, int nCel, int w);
void SetSpellTrans(char t);
void DrawSpell();
void DrawSpellList();
void SetSpell();
void SetSpeedSpell(int slot);
void ToggleSpell(int slot);
void PrintChar(int nOffset, int nCel, char col);
void AddPanelString(const char *str, BOOL just);
void ClearPanel();
void DrawPanelBox(int x, int y, int w, int h, int sx, int sy);
// void InitPanelStr();
void SetFlaskHeight(BYTE *pCelBuff, int min, int max, int x, int y);
void DrawFlask(BYTE *pCelBuff, int w, int nSrcOff, BYTE *pBuff, int nDstOff, int h);
void DrawLifeFlask();
void UpdateLifeFlask();
void DrawManaFlask();
void control_update_life_mana();
void UpdateManaFlask();
void InitControlPan();
void DrawCtrlPan();
void DrawCtrlBtns();
void DoSpeedBook();
void DoPanBtn();
void control_set_button_down(int btn_id);
void control_check_btn_press();
void DoAutoMap();
void CheckPanelInfo();
void CheckBtnUp();
void FreeControlPan();
BOOL control_WriteStringToBuffer(const char *str);
void DrawInfoBox();
void PrintInfo();
void CPrintString(int No, char *pszStr, BOOL Just, int lines);
void PrintGameStr(int x, int y, const char *pszStr, int col);
void DrawChr();
void ADD_PlrStringXY(int x, int y, int x2, const char *pszStr, char col);
void MY_PlrStringXY(int x, int y, int x2, const char *pszStr, char col, int base);
void CheckLvlBtn();
void ReleaseLvlBtn();
void DrawLevelUpIcon();
void CheckChrBtns();
void ReleaseChrBtns();
void DrawDurIcon();
int DrawDurIcon4Item(const ItemStruct *pItem, int x, int c);
void RedBack();
char GetSBookTrans(int ii, BOOL townok);
void DrawSpellBook();
void PrintSBookStr(int x, int y, BOOL cjustflag, const char *pszStr, char col);
void CheckSBook();
const char *get_pieces_str(int nGold);
void DrawGoldSplit(int amount);
void control_drop_gold(char vkey);
void control_remove_gold(int pnum, int ii);
void control_set_gold_curs(int pnum);
void DrawTalkPan();
char *control_print_talk_msg(char *msg, int x, int y, int *pnOffset, int col);
BOOL control_check_talk_btn();
void control_release_talk_btn();
void control_reset_talk_msg(char *msg);
void control_type_message();
void control_reset_talk();
BOOL control_talk_last_key(int vkey);
BOOL control_presskeys(int vkey);
void control_press_enter();
void control_up_down(int v);

/* rdata */
extern const unsigned char fontframe[128];
extern const unsigned char fontkern[68];
extern const int lineoffset[5][5];
extern const unsigned char gbFontTransTbl[256];

/* data */

extern char SpellITbl[37];
extern int PanBtnPos[8][5];
extern char *PanBtnHotKey[8];
extern char *PanBtnStr[8];
extern int attribute_inc_rects[4][4];
extern int SpellPages[6][7];

#endif /* __CONTROL_H__ */
