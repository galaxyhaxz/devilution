//////////////////////////////////////////////////
// direct x
//////////////////////////////////////////////////
typedef HRESULT (WINAPI *DDCREATEPROC)(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);
typedef HRESULT (WINAPI *DSCREATEPROC)(GUID FAR *lpGUID, LPDIRECTSOUND FAR *lplpDS, IUnknown FAR *pUnkOuter);

//////////////////////////////////////////////////
// items
//////////////////////////////////////////////////

typedef struct PLStruct
{
	const char *PLName;
	int PLPower;
	int PLParam1;
	int PLParam2;
	char PLMinLvl;
	int PLIType;
	BYTE PLGOE;
	int PLDouble;
	int PLOk;
	int PLMinVal;
	int PLMaxVal;
	int PLMultVal;
} PLStruct;

typedef struct UItemStruct
{
	char *UIName;
	char UIItemId;
	char UIMinLvl;
	char UINumPL;
	int UIValue;
	char UIPower1;
	int UIParam1;
	int UIParam2;
	char UIPower2;
	int UIParam3;
	int UIParam4;
	char UIPower3;
	int UIParam5;
	int UIParam6;
	char UIPower4;
	int UIParam7;
	int UIParam8;
	char UIPower5;
	int UIParam9;
	int UIParam10;
	char UIPower6;
	int UIParam11;
	int UIParam12;
} UItemStruct;

typedef struct ItemDataStruct
{
	int iRnd;
	char iClass;
	char iLoc;
	int iCurs;
	char itype;
	char iItemId;
	char *iName;
	char *iSName;
	char iMinMLvl;
	int iDurability;
	int iMinDam;
	int iMaxDam;
	int iMinAC;
	int iMaxAC;
	char iMinStr;
	char iMinMag;
	char iMinDex;
	int iFlags;
	int iMiscId;
	int iSpell;
	int iUsable;
	int iValue;
	int iMaxValue;
} ItemDataStruct;

typedef struct ItemGetRecordStruct
{
	int nSeed;
	unsigned short wCI;
	int nIndex;
	unsigned int dwTimestamp;
} ItemGetRecordStruct;

typedef struct ItemStruct
{
	int _iSeed;
	unsigned short _iCreateInfo;
	int _itype;
	int _ix;
	int _iy;
	int _iAnimFlag;
	unsigned char *_iAnimData; // PSX name -> ItemFrame
	int _iAnimLen;
	int _iAnimFrame;
	int _iAnimWidth;
	int _iAnimWidth2; // width 2?
	int _iDelFlag; // set when item is flagged for deletion, deprecated in 1.02
	char _iSelFlag;
	int _iPostDraw;
	int _iIdentified;
	char _iMagical;
	char _iName[64];
	char _iIName[64];
	char _iLoc;
	char _iClass;
	int _iCurs;
	int _ivalue;
	int _iIvalue;
	int _iMinDam;
	int _iMaxDam;
	int _iAC;
	int _iFlags;
	int _iMiscId;
	int _iSpell;
	int _iCharges;
	int _iMaxCharges;
	int _iDurability;
	int _iMaxDur;
	int _iPLDam;
	int _iPLToHit;
	int _iPLAC;
	int _iPLStr;
	int _iPLMag;
	int _iPLDex;
	int _iPLVit;
	int _iPLFR;
	int _iPLLR;
	int _iPLMR;
	int _iPLMana;
	int _iPLHP;
	int _iPLDamMod;
	int _iPLGetHit;
	int _iPLLight;
	char _iSplLvlAdd;
	char _iRequest;
	int _iUid;
	int _iFMinDam;
	int _iFMaxDam;
	int _iLMinDam;
	int _iLMaxDam;
	int _iPLEnAc;
	char _iPrePower;
	char _iSufPower;
	int _iVAdd1;
	int _iVMult1;
	int _iVAdd2;
	int _iVMult2;
	char _iMinStr;
	unsigned char _iMinMag;
	char _iMinDex;
	int _iStatFlag;
	int IDidx;
	int offs016C; // _oldlight or _iInvalid
} ItemStruct;

//////////////////////////////////////////////////
// player
//////////////////////////////////////////////////

typedef struct PlayerStruct
{
	int _pmode;
	char walkpath[25];
	unsigned char plractive;
	int destAction;
	int destParam1;
	int destParam2;
	int destParam3;
	int destParam4;
	int plrlevel;
	int WorldX;
	int WorldY;
	int _px;
	int _py;
	int _ptargx;
	int _ptargy;
	int _pownerx;
	int _pownery;
	int _poldx;
	int _poldy;
	int _pxoff;
	int _pyoff;
	int _pxvel;
	int _pyvel;
	int _pdir;
	int _nextdir;
	int _pgfxnum;
	unsigned char *_pAnimData;
	int _pAnimDelay;
	int _pAnimCnt;
	int _pAnimLen;
	int _pAnimFrame;
	int _pAnimWidth;
	int _pAnimWidth2;
	int _peflag;
	int _plid;
	int _pvid;
	int _pSpell;
	char _pSplType;
	char _pSplFrom;
	int _pTSpell;
	char _pTSplType;
	int _pRSpell;
	char _pRSplType;
	int _pSBkSpell;
	char _pSBkSplType;
	char _pSplLvl[64];
	union {
		__int64 _pMemSpells64;
		int _pMemSpells[2];
	};
	union {
		__int64 _pAblSpells64;
		int _pAblSpells[2];
	};
	union {
		__int64 _pScrlSpells64;
		int _pScrlSpells[2];
	};
	char _pSpellFlags;
	int _pSplHotKey[4];
	char _pSplTHotKey[4];
	int _pwtype;
	unsigned char _pBlockFlag;
	unsigned char _pInvincible;
	char _pLightRad;
	unsigned char _pLvlChanging;
	char _pName[32];
	char _pClass;
	int _pStrength;
	int _pBaseStr;
	int _pMagic;
	int _pBaseMag;
	int _pDexterity;
	int _pBaseDex;
	int _pVitality;
	int _pBaseVit;
	int _pStatPts;
	int _pDamageMod;
	int _pBaseToBlk;
	int _pHPBase;
	int _pMaxHPBase;
	int _pHitPoints;
	int _pMaxHP;
	int _pHPPer;
	int _pManaBase;
	int _pMaxManaBase;
	int _pMana;
	int _pMaxMana;
	int _pManaPer;
	char _pLevel;
	char _pMaxLvl;
	int _pExperience;
	int _pMaxExp;
	int _pNextExper;
	char _pArmorClass;
	char _pMagResist;
	char _pFireResist;
	char _pLghtResist;
	int _pGold;
	int _pInfraFlag;
	int _pVar1;
	int _pVar2;
	int _pVar3;
	int _pVar4;
	int _pVar5;
	int _pVar6;
	int _pVar7;
	int _pVar8;
	unsigned char _pLvlVisited[NUMLEVELS];
	unsigned char _pSLvlVisited[NUMLEVELS]; // only 10 used
	int _pGFXLoad;
	unsigned char *_pNAnim[8];
	int _pNFrames;
	int _pNWidth;
	unsigned char *_pWAnim[8];
	int _pWFrames;
	int _pWWidth;
	unsigned char *_pAAnim[8];
	int _pAFrames;
	int _pAWidth;
	int _pAFNum;
	unsigned char *_pLAnim[8];
	unsigned char *_pFAnim[8];
	unsigned char *_pTAnim[8];
	int _pSFrames;
	int _pSWidth;
	int _pSFNum;
	unsigned char *_pHAnim[8];
	int _pHFrames;
	int _pHWidth;
	unsigned char *_pDAnim[8];
	int _pDFrames;
	int _pDWidth;
	unsigned char *_pBAnim[8];
	int _pBFrames;
	int _pBWidth;
	ItemStruct InvBody[7];
	ItemStruct InvList[40];
	int _pNumInv;
	char InvGrid[40];
	ItemStruct SpdList[8];
	ItemStruct HoldItem;
	int _pIMinDam;
	int _pIMaxDam;
	int _pIAC;
	int _pIBonusDam;
	int _pIBonusToHit;
	int _pIBonusAC;
	int _pIBonusDamMod;
	union {
		__int64 _pISpells64;
		int _pISpells[2];
	};
	int _pIFlags;
	int _pIGetHit;
	char _pISplLvlAdd;
	char _pISplCost; /* unused (beta): reduce mana cost of spells */
	int _pISplDur; /* unused (beta): increase duration of spells */
	int _pIEnAc;
	int _pIFMinDam;
	int _pIFMaxDam;
	int _pILMinDam;
	int _pILMaxDam;
	int _pOilType; /* unused (beta): current oil applied to item */
	unsigned char pTownWarps;
	unsigned char pDungMsgs;
	unsigned char pLvlLoad;
	unsigned char pBattleNet;
	unsigned char pManaShield;
	char bReserved[3];
	short wReserved[8];
	DWORD pDiabloKillLevel;
	int dwReserved[7];
	unsigned char *_pNData;
	unsigned char *_pWData;
	unsigned char *_pAData;
	unsigned char *_pLData;
	unsigned char *_pFData;
	unsigned char *_pTData;
	unsigned char *_pHData;
	unsigned char *_pDData;
	unsigned char *_pBData;
	void *pReserved;
} PlayerStruct;

//////////////////////////////////////////////////
// textdat
//////////////////////////////////////////////////

typedef struct TextDataStruct
{
	char *txtstr;
	int scrlltxt;
	int txtspd;
	int sfxnr;
} TextDataStruct;

//////////////////////////////////////////////////
// missiles
//////////////////////////////////////////////////

// TPDEF PTR FCN VOID MIADDPRC
// TPDEF PTR FCN VOID MIPROC

typedef struct MissileData
{
	unsigned char mName;
	void (* mAddProc)(int, int, int, int, int, int, char, int, int);
	void (* mProc)(int);
	int mDraw;
	unsigned char mType;
	unsigned char mResist;
	unsigned char mFileNum;
	int mlSFX;
	int miSFX;
} MissileData;

typedef struct MisFileData
{
	unsigned char mAnimName;
	unsigned char mAnimFAmt;
	char *mName;
	int mFlags;
	unsigned char *mAnimData[16];
	unsigned char mAnimDelay[16];
	unsigned char mAnimLen[16];
	int mAnimWidth[16];
	int mAnimWidth2[16];
} MisFileData;

typedef struct ChainStruct
{
	int idx;
	int _mitype;
	int _mirange;
} ChainStruct;

typedef struct MissileStruct
{
	int _mitype;
	int _mix;
	int _miy;
	int _mixoff;
	int _miyoff;
	int _mixvel;
	int _miyvel;
	int _misx;
	int _misy;
	int _mitxoff;
	int _mityoff;
	int _mimfnum;
	int _mispllvl;
	int _miDelFlag;
	BYTE _miAnimType;
	int _miAnimFlags;
	unsigned char *_miAnimData;
	int _miAnimDelay;
	int _miAnimLen;
	int _miAnimWidth;
	int _miAnimWidth2;
	int _miAnimCnt;
	int _miAnimAdd;
	int _miAnimFrame;
	int _miDrawFlag;
	int _miLightFlag;
	int _miPreFlag;
	int _miUniqTrans;
	int _mirange;
	int _misource;
	int _micaster;
	int _midam;
	int _miHitFlag;
	int _midist;
	int _mlid;
	int _mirnd;
	int _miVar1;
	int _miVar2;
	int _miVar3;
	int _miVar4;
	int _miVar5;
	int _miVar6;
	int _miVar7;
	int _miVar8;
} MissileStruct;

//////////////////////////////////////////////////
// effects/sound
//////////////////////////////////////////////////

typedef struct CKINFO {
	DWORD dwSize;
	DWORD dwOffset;
} CKINFO;

typedef struct TSnd
{
	WAVEFORMATEX fmt;
	CKINFO chunk;
	char *sound_path;
	IDirectSoundBuffer *DSB;
	int start_tc;
} TSnd;

#pragma pack(push, 1)
typedef struct TSFX
{
	unsigned char bFlags;
	char *pszName;
	TSnd *pSnd;
} TSFX;
#pragma pack(pop)

//////////////////////////////////////////////////
// monster
//////////////////////////////////////////////////

typedef struct AnimStruct // note: wrong names
{
	BYTE *CMem; // [unsigned] char * ??
	unsigned char *Frames[8]; // probably Data[8]
	int Rate;
	int Delay;
} AnimStruct;

typedef struct MonsterData
{
	int flags; // width?
	int mType;
	char *GraphicType;
	int has_special;
	char *sndfile;
	int snd_special;
	int has_trans;
	char *TransFile;
	int Frames[6];
	int Rate[6];
	char *mName;
	char mMinDLvl;
	char mMaxDLvl;
	char mLevel;
	int mMinHP;
	int mMaxHP;
	char mAi;
	int mFlags;
	unsigned char mInt;
	unsigned char mHit;
	unsigned char mAFNum;
	unsigned char mMinDamage;
	unsigned char mMaxDamage;
	unsigned char mHit2;
	unsigned char mAFNum2;
	unsigned char mMinDamage2;
	unsigned char mMaxDamage2;
	char mArmorClass;
	char mMonstClass;
	unsigned short mMagicRes;
	unsigned short mMagicRes2;
	unsigned short mTreasure;
	char mSelFlag;
	unsigned short mExp;
} MonsterData;

typedef struct CMonster
{
	unsigned char mtype;
	unsigned char mPlaceFlags;
	AnimStruct Anims[6];
	TSnd *Snds[4][2];
	int flags_1; // width
	int flags_2; // width 2
	unsigned char mMinHP;
	unsigned char mMaxHP;
	int has_special;
	unsigned char mAFNum;
	char mdeadval;
	MonsterData *MData;
	BYTE *trans_file;
} CMonster;

typedef struct MonsterStruct // note: missing field _mAFNum
{
	int _mMTidx;
	int _mmode;
	char _mgoal;
	int _mgoalvar1;
	int _mgoalvar2;
	int _mgoalvar3;
	int field_18; // _mgoalvar4?
	char _pathcount;
	int _mx;
	int _my;
	int _mfutx;
	int _mfuty;
	int _moldx;
	int _moldy;
	int _mxoff;
	int _myoff;
	int _mxvel;
	int _myvel;
	int _mdir;
	int _menemy;
	unsigned char _menemyx;
	unsigned char _menemyy;
	short falign_52;
	unsigned char *_mAnimData;
	int _mAnimDelay;
	int _mAnimCnt;
	int _mAnimLen;
	int _mAnimFrame;
	int _meflag;
	int _mDelFlag;
	int _mVar1;
	int _mVar2;
	int _mVar3;
	int _mVar4;
	int _mVar5;
	int _mVar6;
	int _mVar7;
	int _mVar8;
	int _mmaxhp;
	int _mhitpoints;
	unsigned char _mAi;
	unsigned char _mint;
	short falign_9A;
	int _mFlags;
	BYTE _msquelch;
	int falign_A4;
	int _lastx;
	int _lasty;
	int _mRndSeed;
	int _mAISeed;
	int falign_B8;
	unsigned char _uniqtype;
	unsigned char _uniqtrans;
	char _udeadval;
	char mWhoHit;
	char mLevel;
	unsigned short mExp;
	unsigned char mHit;
	unsigned char mMinDamage;
	unsigned char mMaxDamage;
	unsigned char mHit2;
	unsigned char mMinDamage2;
	unsigned char mMaxDamage2;
	char mArmorClass;
	char falign_CB;
	WORD mMagicRes;
	int mtalkmsg;
	unsigned char leader;
	unsigned char leaderflag;
	unsigned char packsize;
	unsigned char mlid;
	char *mName;
	CMonster *MType;
	MonsterData *MData;
} MonsterStruct;

typedef struct UniqMonstStruct
{
	char mtype;
	char *mName;
	char *mTrnName;
	unsigned char mlevel;
	unsigned short mmaxhp;
	unsigned char mAi;
	unsigned char mint;
	unsigned char mMinDamage;
	unsigned char mMaxDamage;
	unsigned short mMagicRes;
	unsigned short mUnqAttr;
	unsigned char mUnqVar1;
	unsigned char mUnqVar2;
	int mtalkmsg;
} UniqMonstStruct;

//////////////////////////////////////////////////
// objects
//////////////////////////////////////////////////

typedef struct ObjDataStruct
{
	char oload;
	char ofindex;
	char ominlvl;
	char omaxlvl;
	char olvltype;
	char otheme;
	char oquest;
	int oAnimFlag;
	int oAnimDelay;
	int oAnimLen;
	int oAnimWidth;
	int oSolidFlag;
	int oMissFlag;
	int oLightFlag;
	char oBreak;
	char oSelFlag;
	int oTrapFlag;
} ObjDataStruct;

typedef struct ObjectStruct
{
	int _otype;
	int _ox;
	int _oy;
	int _oLight;
	int _oAnimFlag;
	unsigned char *_oAnimData;
	int _oAnimDelay;
	int _oAnimCnt;
	int _oAnimLen;
	int _oAnimFrame;
	int _oAnimWidth;
	int _oAnimWidth2;
	int _oDelFlag;
	char _oBreak; // check
	int _oSolidFlag;
	int _oMissFlag;
	char _oSelFlag; // check
	int _oPreFlag;
	int _oTrapFlag;
	int _oDoorFlag;
	int _olid;
	int _oRndSeed;
	int _oVar1;
	int _oVar2;
	int _oVar3;
	int _oVar4;
	int _oVar5;
	int _oVar6;
	int _oVar7;
	int _oVar8;
} ObjectStruct;

//////////////////////////////////////////////////
// portal
//////////////////////////////////////////////////

typedef struct PortalStruct
{
	int open;
	int x;
	int y;
	int level;
	int ltype;
	int setlvl;
} PortalStruct;

//////////////////////////////////////////////////
// msg
//////////////////////////////////////////////////

#pragma pack(push, 1)
typedef struct TCmd
{
	unsigned char bCmd;
} TCmd;

typedef struct TCmdLoc
{
	unsigned char bCmd;
	unsigned char x;
	unsigned char y;
} TCmdLoc;

typedef struct TCmdLocParam1
{
	unsigned char bCmd;
	unsigned char x;
	unsigned char y;
	unsigned short wParam1;
} TCmdLocParam1;

typedef struct TCmdLocParam2
{
	unsigned char bCmd;
	unsigned char x;
	unsigned char y;
	unsigned short wParam1;
	unsigned short wParam2;
} TCmdLocParam2;

typedef struct TCmdLocParam3
{
	unsigned char bCmd;
	unsigned char x;
	unsigned char y;
	unsigned short wParam1;
	unsigned short wParam2;
	unsigned short wParam3;
} TCmdLocParam3;

typedef struct TCmdParam1
{
	unsigned char bCmd;
	unsigned short wParam1;
} TCmdParam1;

typedef struct TCmdParam2
{
	unsigned char bCmd;
	unsigned short wParam1;
	unsigned short wParam2;
} TCmdParam2;

typedef struct TCmdParam3
{
	unsigned char bCmd;
	unsigned short wParam1;
	unsigned short wParam2;
	unsigned short wParam3;
} TCmdParam3;

typedef struct TCmdGolem
{
	unsigned char bCmd;
	unsigned char _mx;
	unsigned char _my;
	unsigned char _mdir;
	unsigned char _menemy;
	int _mhitpoints;
	unsigned char _currlevel;
} TCmdGolem;

typedef struct TCmdQuest
{
	unsigned char bCmd;
	unsigned char q;
	unsigned char qstate;
	unsigned char qlog;
	unsigned char qvar1;
} TCmdQuest;

typedef struct TCmdGItem
{
	unsigned char bCmd;
	unsigned char bMaster;
	unsigned char bPnum;
	unsigned char bCursitem;
	unsigned char bLevel;
	unsigned char x;
	unsigned char y;
	unsigned short wIndx;
	unsigned short wCI;
	int dwSeed;
	unsigned char bId;
	unsigned char bDur;
	unsigned char bMDur;
	unsigned char bCh;
	unsigned char bMCh;
	unsigned short wValue;
	int dwBuff;
	int dwTime;
} TCmdGItem;

typedef struct TCmdPItem
{
	char bCmd; /* unsigned */
	unsigned char x;
	unsigned char y;
	unsigned short wIndx;
	unsigned short wCI;
	int dwSeed;
	unsigned char bId;
	unsigned char bDur;
	unsigned char bMDur;
	unsigned char bCh;
	unsigned char bMCh;
	unsigned short wValue;
	int dwBuff;
} TCmdPItem;

typedef struct TCmdChItem
{
	unsigned char bCmd;
	unsigned char bLoc;
	unsigned short wIndx;
	unsigned short wCI;
	int dwSeed;
	unsigned char bId;
} TCmdChItem;

typedef struct TCmdDelItem
{
	unsigned char bCmd;
	unsigned char bLoc;
} TCmdDelItem;

typedef struct TCmdDamage
{
	unsigned char bCmd;
	unsigned char bPlr;
	int dwDam;
} TCmdDamage;

typedef struct TCmdPlrInfoHdr
{
	unsigned char bCmd;
	unsigned short wOffset;
	unsigned short wBytes;
} TCmdPlrInfoHdr;

typedef struct TCmdString
{
	unsigned char bCmd;
	char str[80];
} TCmdString;

typedef struct TFakeCmdPlr
{
	unsigned char bCmd;
	unsigned char bPlr;
} TFakeCmdPlr;

typedef struct TFakeDropPlr
{
	unsigned char bCmd;
	unsigned char bPlr;
	int dwReason;
} TFakeDropPlr;

typedef struct TSyncHeader
{
	unsigned char bCmd;
	unsigned char bLevel;
	unsigned short wLen;
	unsigned char bObjId;
	unsigned char bObjCmd;
	unsigned char bItemI;
	unsigned char bItemX;
	unsigned char bItemY;
	unsigned short wItemIndx;
	unsigned short wItemCI;
	int dwItemSeed;
	unsigned char bItemId;
	unsigned char bItemDur;
	unsigned char bItemMDur;
	unsigned char bItemCh;
	unsigned char bItemMCh;
	unsigned short wItemVal;
	unsigned int dwItemBuff;
	unsigned char bPInvLoc;
	unsigned short wPInvIndx;
	unsigned short wPInvCI;
	int dwPInvSeed;
	unsigned char bPInvId;
} TSyncHeader;

typedef struct TSyncMonster
{
	unsigned char _mndx;
	unsigned char _mx;
	unsigned char _my;
	unsigned char _menemy;
	unsigned char _mdelta;
} TSyncMonster;

typedef struct TPktHdr
{
	unsigned char px;
	unsigned char py;
	unsigned char targx;
	unsigned char targy;
	int php;
	int pmhp;
	unsigned char bstr;
	unsigned char bmag;
	unsigned char bdex;
	unsigned short wCheck;
	unsigned short wLen;
} TPktHdr;

typedef struct TPkt
{
	TPktHdr hdr;
	unsigned char body[493];
} TPkt;

typedef struct DMonsterStr
{
	unsigned char _mx; /* these might be unsigned */
	unsigned char _my;
	unsigned char _mdir;
	unsigned char _menemy;
	unsigned char _mactive;
	int _mhitpoints;
} DMonsterStr;

typedef struct DObjectStr
{
	unsigned char bCmd;
} DObjectStr;

typedef struct DLevel
{
	TCmdPItem item[MAXITEMS];
	DObjectStr object[MAXOBJECTS];
	DMonsterStr monster[MAXMONSTERS];
} DLevel;

typedef struct LocalLevel
{
	unsigned char automapsv[40][40];
} LocalLevel;

typedef struct DPortal
{
	unsigned char x;
	unsigned char y;
	unsigned char level;
	unsigned char ltype;
	unsigned char setlvl;
} DPortal;

typedef struct MultiQuests
{
	unsigned char qstate;
	unsigned char qlog;
	unsigned char qvar1;
} MultiQuests;

typedef struct DJunk
{
	DPortal portal[MAXPORTAL];
	MultiQuests quests[MAXMULTIQUESTS];
} DJunk;
#pragma pack(pop)

typedef struct TMegaPkt
{
	struct TMegaPkt *pNext;
	int dwSpaceLeft;
	unsigned char data[32000];
} TMegaPkt;

typedef struct TBuffer
{
	unsigned int dwNextWriteOffset;
	unsigned char bData[4096];
} TBuffer;

//////////////////////////////////////////////////
// quests
//////////////////////////////////////////////////

typedef struct QuestStruct
{
	unsigned char _qlevel;
	unsigned char _qtype;
	unsigned char _qactive;
	unsigned char _qlvltype;
	int _qtx;
	int _qty;
	unsigned char _qslvl;
	unsigned char _qidx;
	unsigned char _qmsg;
	unsigned char _qvar1;
	unsigned char _qvar2;
	int _qlog; /* char */
} QuestStruct;

typedef struct QuestData
{
	unsigned char _qdlvl;
	char _qdmultlvl;
	unsigned char _qlvlt;
	unsigned char _qdtype;
	unsigned char _qdrnd;
	unsigned char _qslvl;
	int _qflags; /* unsigned char */
	int _qdmsg;
	char *_qlstr;
} QuestData;

//////////////////////////////////////////////////
// gamemenu/gmenu
//////////////////////////////////////////////////

// TPDEF PTR FCN VOID TMenuFcn

typedef struct TMenuItem
{
	unsigned int dwFlags;
	char *pszStr;
	void (* fnMenu)(BOOL); /* fix, should have one arg */
} TMenuItem;

// TPDEF PTR FCN VOID TMenuUpdateFcn

//////////////////////////////////////////////////
// spells
//////////////////////////////////////////////////

typedef struct SpellData
{
	unsigned char sName;
	unsigned char sManaCost;
	unsigned char sType;
	char *sNameText;
	char *sSkillText;
	int sBookLvl;
	int sStaffLvl;
	int sTargeted;
	BOOL sTownSpell;
	int sMinInt;
	unsigned char sSFX;
	unsigned char sMissiles[3];
	unsigned char sManaAdj;
	unsigned char sMinMana;
	int sStaffMin;
	int sStaffMax;
	int sBookCost;
	int sStaffCost;
} SpellData;

//////////////////////////////////////////////////
// towners
//////////////////////////////////////////////////

typedef struct TNQ
{
	unsigned char _qsttype;
	unsigned char _qstmsg;
	unsigned char _qstmsgact;
} TNQ;

typedef struct TownerStruct
{
	int _tmode;
	int _ttype;
	int _tx;
	int _ty;
	int _txoff;
	int _tyoff;
	int _txvel;
	int _tyvel;
	int _tdir;
	unsigned char *_tAnimData;
	int _tAnimDelay;
	int _tAnimCnt;
	int _tAnimLen;
	int _tAnimFrame;
	int _tAnimFrameCnt;
	char _tAnimOrder;
	int _tAnimWidth;
	int _tAnimWidth2;
	int _tTenPer;
	int _teflag;
	int _tbtcnt;
	int _tSelFlag;
	int _tMsgSaid;
	TNQ qsts[16];
	int _tSeed;
	int _tVar1;
	int _tVar2;
	int _tVar3;
	int _tVar4;
	char _tName[32];
	unsigned char *_tNAnim[8];
	int _tNFrames;
	unsigned char *_tNData;
} TownerStruct;

typedef struct QuestTalkData
{
	int _qinfra;
	int _qblkm;
	int _qgarb;
	int _qzhar;
	int _qveil;
	int _qmod;
	int _qbutch;
	int _qbol;
	int _qblind;
	int _qblood;
	int _qanvil;
	int _qwarlrd;
	int _qking;
	int _qpw;
	int _qbone;
	int _qvb;
} QuestTalkData;

//////////////////////////////////////////////////
// gendung
//////////////////////////////////////////////////

typedef struct ScrollStruct
{
	int _sxoff;
	int _syoff;
	int _sdx;
	int _sdy;
	int _sdir;
} ScrollStruct;

typedef struct THEME_LOC
{
	int x;
	int y;
	int ttval;
	int width;
	int height;
} THEME_LOC;

typedef struct MICROS
{
	WORD mt[16];
} MICROS;

//////////////////////////////////////////////////
// drlg
//////////////////////////////////////////////////

typedef struct ShadowStruct
{
	unsigned char strig;
	unsigned char s1;
	unsigned char s2;
	unsigned char s3;
	unsigned char nv1;
	unsigned char nv2;
	unsigned char nv3;
} ShadowStruct;

typedef struct HALLNODE
{
	int nHallx1;
	int nHally1;
	int nHallx2;
	int nHally2;
	int nHalldir;
	struct HALLNODE *pNext;
} HALLNODE;

typedef struct ROOMNODE
{
	int nRoomx1;
	int nRoomy1;
	int nRoomx2;
	int nRoomy2;
	int nRoomDest;
} ROOMNODE;

//////////////////////////////////////////////////
// themes
//////////////////////////////////////////////////

typedef struct ThemeStruct
{
	char ttype; /* aligned 4 */
	int ttval;
} ThemeStruct;

//////////////////////////////////////////////////
// inv
//////////////////////////////////////////////////

typedef struct InvXY
{
	int X;
	int Y;
} InvXY;

//////////////////////////////////////////////////
// lighting
//////////////////////////////////////////////////

typedef struct LightListStruct
{
	int _lx;
	int _ly;
	int _lradius;
	int _lid;
	int _ldel;
	int _lunflag;
	int field_18;
	int _lunx;
	int _luny;
	int _lunr;
	int _xoff;
	int _yoff;
	int _lflags;
} LightListStruct;

//////////////////////////////////////////////////
// dead
//////////////////////////////////////////////////

typedef struct DeadStruct
{
	unsigned char *_deadData[8];
	int _deadFrame;
	int _deadWidth;
	int _deadWidth2;
	char _deadtrans;
} DeadStruct;

//////////////////////////////////////////////////
// storm
//////////////////////////////////////////////////

// TPDEF PTR FCN VOID SEVTHANDLER

// TPDEF PTR FCN UCHAR SMSGIDLEPROC
// TPDEF PTR FCN VOID SMSGHANDLER

typedef struct _SNETCAPS
{
	DWORD size;
	DWORD flags;
	DWORD maxmessagesize;
	DWORD maxqueuesize;
	DWORD maxplayers;
	DWORD bytessec;
	DWORD latencyms;
	DWORD defaultturnssec;
	DWORD defaultturnsintransit;
} _SNETCAPS;

typedef struct _SNETEVENT
{
	DWORD eventid;
	DWORD playerid;
	void *data;
	DWORD databytes;
} _SNETEVENT;

// TPDEF PTR FCN UCHAR SNETABORTPROC
// TPDEF PTR FCN UCHAR SNETCATEGORYPROC
// TPDEF PTR FCN UCHAR SNETCHECKAUTHPROC
// TPDEF PTR FCN UCHAR SNETCREATEPROC
// TPDEF PTR FCN UCHAR SNETDRAWDESCPROC
// TPDEF PTR FCN UCHAR SNETENUMDEVICESPROC
// TPDEF PTR FCN UCHAR SNETENUMGAMESPROC
// TPDEF PTR FCN UCHAR SNETENUMPROVIDERSPROC
// TPDEF PTR FCN VOID SNETEVENTPROC
// TPDEF PTR FCN UCHAR SNETGETARTPROC
// TPDEF PTR FCN UCHAR SNETGETDATAPROC
// TPDEF PTR FCN INT SNETMESSAGEBOXPROC
// TPDEF PTR FCN UCHAR SNETPLAYSOUNDPROC
// TPDEF PTR FCN UCHAR SNETSELECTEDPROC
// TPDEF PTR FCN UCHAR SNETSTATUSPROC

typedef struct _SNETPLAYERDATA
{
	int size;
	char *playername;
	char *playerdescription;
	int reserved;
} _SNETPLAYERDATA;

typedef struct _SNETPROGRAMDATA
{
	int size;
	char *programname;
	char *programdescription;
	int programid;
	int versionid;
	int reserved1;
	int maxplayers;
	void *initdata;
	int initdatabytes;
	void *reserved2;
	int optcategorybits;
	char *cdkey;
	char *registereduser;
	int spawned;
	int lcid;
} _SNETPROGRAMDATA;

typedef struct _SNETUIDATA
{
	int size;
	int uiflags;
	HWND parentwindow;
	void (* artcallback)();
	void (* authcallback)();
	void (* createcallback)();
	void (* drawdesccallback)();
	void (* selectedcallback)();
	void (* messageboxcallback)();
	void (* soundcallback)();
	void (* statuscallback)();
	void (* getdatacallback)();
	void (* categorycallback)();
	void (* categorylistcallback)();
	void (* newaccountcallback)();
	void (* profilecallback)();
	int profilefields;
	void (* profilebitmapcallback)();
	void (* selectnamecallback)();
	void (* changenamecallback)();
} _SNETUIDATA;

typedef struct _SNETVERSIONDATA
{
	int size;
	char *versionstring;
	char *executablefile;
	char *originalarchivefile;
	char *patcharchivefile;
} _SNETVERSIONDATA;

// TPDEF PTR FCN UCHAR SNETSPIBIND
// TPDEF PTR FCN UCHAR SNETSPIQUERY

//////////////////////////////////////////////////
// diabloui
//////////////////////////////////////////////////

// TPDEF PTR FCN VOID PLAYSND

typedef struct _gamedata
{
	DWORD dwSeed;
	unsigned char bDiff;
} _gamedata;

typedef struct _uidefaultstats
{
	unsigned short strength;
	unsigned short magic;
	unsigned short dexterity;
	unsigned short vitality;
} _uidefaultstats;

typedef struct _uiheroinfo
{
	struct _uiheroinfo *next;
	char name[16];
	unsigned short level;
	unsigned char heroclass;
	unsigned char herorank;
	unsigned short strength;
	unsigned short magic;
	unsigned short dexterity;
	unsigned short vitality;
	int gold;
	int hassaved;
	int spawned;
} _uiheroinfo;

// TPDEF PTR FCN UCHAR ENUMHEROPROC
// TPDEF PTR FCN UCHAR ENUMHEROS
// TPDEF PTR FCN UCHAR CREATEHERO
// TPDEF PTR FCN UCHAR DELETEHERO
// TPDEF PTR FCN UCHAR GETDEFHERO

// TPDEF PTR FCN INT PROGRESSFCN

//////////////////////////////////////////////////
// pack
//////////////////////////////////////////////////

#pragma pack(push, 1)
typedef struct PkItemStruct
{
	int iSeed;
	short iCreateInfo;
	short idx;
	char bId;
	char bDur;
	char bMDur;
	char bCh;
	char bMCh;
	short wValue;
	int dwBuff;
} PkItemStruct;

typedef struct PkPlayerStruct
{
	FILETIME archiveTime;
	char destAction;
	char destParam1;
	char destParam2;
	char plrlevel;
	char px;
	char py;
	char targx;
	char targy;
	char pName[32];
	char pClass;
	char pBaseStr;
	char pBaseMag;
	char pBaseDex;
	char pBaseVit;
	char pLevel;
	char pStatPts;
	int pExperience;
	int pGold;
	int pHPBase;
	int pMaxHPBase;
	int pManaBase;
	int pMaxManaBase;
	char pSplLvl[37];
	int pMemSpells; /* __int64 */
	int pMemSpells2;
	PkItemStruct InvBody[7];
	PkItemStruct InvList[40];
	char InvGrid[40];
	char _pNumInv;
	PkItemStruct SpdList[8];
	char pTownWarps;
	char pDungMsgs;
	char pLvlLoad;
	char pBattleNet;
	char pManaShield;
	char bReserved[3];
	short wReserved[8];
	DWORD pDiabloKillLevel;
	int dwReserved[7];
} PkPlayerStruct;
#pragma pack(pop)

//////////////////////////////////////////////////
// path
//////////////////////////////////////////////////

typedef struct PATHNODE
{
	char f;
	char h;
	char g;
	int x;
	int y;
	struct PATHNODE *Parent;
	struct PATHNODE *Child[8];
	struct PATHNODE *NextNode;
} PATHNODE;

// TPDEF PTR FCN UCHAR CHECKFUNC1

// TPDEF PTR FCN UCHAR CHECKFUNC

//////////////////////////////////////////////////
// sha
//////////////////////////////////////////////////

typedef struct SHA1Context
{
	int state[5];
	int count[2];
	char buffer[64];
} SHA1Context;

//////////////////////////////////////////////////
// tmsg
//////////////////////////////////////////////////

#pragma pack(push, 1)
typedef struct TMsg TMsg;

typedef struct TMsgHdr
{
	TMsg *pNext;
	unsigned int dwTime;
	unsigned char bLen;
} TMsgHdr;

typedef struct TMsg
{
	TMsgHdr hdr;
	unsigned char body[3];
} TMsg;
#pragma pack(pop)

//////////////////////////////////////////////////
// mpqapi
//////////////////////////////////////////////////

typedef struct _FILEHEADER
{
	int signature;
	int headersize;
	int filesize;
	short version;
	short sectorsizeid;
	int hashoffset;
	int blockoffset;
	int hashcount;
	int blockcount;
	char pad[72];
} _FILEHEADER;

typedef struct _HASHENTRY
{
	int hashcheck[2];
	int lcid;
	int block;
} _HASHENTRY;

typedef struct _BLOCKENTRY
{
	int offset;
	int sizealloc;
	int sizefile;
	int flags;
} _BLOCKENTRY;

// TPDEF PTR FCN UCHAR TGetNameFcn

// TPDEF PTR FCN VOID TCrypt

//////////////////////////////////////////////////
// trigs
//////////////////////////////////////////////////

typedef struct TriggerStruct
{
	int _tx;
	int _ty;
	int _tmsg;
	int _tlvl;
} TriggerStruct;

//////////////////////////////////////////////////
// stores
//////////////////////////////////////////////////

typedef struct STextStruct
{
	int _sx;
	int _syoff;
	char _sstr[128];
	int _sjust;
	char _sclr;
	int _sline;
	BOOL _ssel;
	int _sval;
} STextStruct;

//////////////////////////////////////////////////
// wave
//////////////////////////////////////////////////

typedef struct MEMFILE
{
	int end;
	int offset;
	int buf_len;
	int dist;
	int bytes_to_read;
	char *buf;
	int file;
} MEMFILE;

//////////////////////////////////////////////////
// plrmsg
//////////////////////////////////////////////////

typedef struct _plrmsg
{
	int time;
	char player;
	char str[144];
} _plrmsg;

//////////////////////////////////////////////////
// capture
//////////////////////////////////////////////////

typedef struct _PcxHeader
{
	BYTE Manufacturer;
	BYTE Version;
	BYTE Encoding;
	BYTE BitsPerPixel;
	WORD Xmin;
	WORD Ymin;
	WORD Xmax;
	WORD Ymax;
	WORD HDpi;
	WORD VDpi;
	BYTE Colormap[48];
	BYTE Reserved;
	BYTE NPlanes;
	WORD BytesPerLine;
	WORD PaletteInfo;
	WORD HscreenSize;
	WORD VscreenSize;
	BYTE Filler[54];
} PCXHEADER;

//////////////////////////////////////////////////
// encrypt
//////////////////////////////////////////////////

typedef struct TDataInfo
{
	unsigned char *pbInBuff;
	unsigned char *pbInBuffEnd;
	unsigned char *pbOutBuff;
	unsigned char *pbOutBuffEnd;
	unsigned char *pbSize;
} TDataInfo;

//////////////////////////////////////////////////
// codec
//////////////////////////////////////////////////

typedef struct CODECCHUNK {
	DWORD checksum;
	BYTE failed;
	BYTE last_chunk_size;
	WORD unused;
} CODECCHUNK;

//////////////////////////////////////////////////
// dthread
//////////////////////////////////////////////////

typedef struct TDeltaInfo {
	TDeltaInfo *pNext;
	int pnum;
	BYTE cmd;
	DWORD size;
	BYTE data[4];
} TDeltaInfo;
