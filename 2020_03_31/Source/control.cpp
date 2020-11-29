#include "diablo.h"

BYTE sgbNextTalkSave; // weak
BYTE sgbTalkSavePos; // weak
void *pDurIcons;
void *pChrButtons;
int drawhpflag; // idb
int dropGoldFlag; // weak
int panbtn[8];
int chrbtn[4];
void *pMultiBtns;
void *pPanelButtons;
void *pChrPanel;
int lvlbtndown; // weak
char sgszTalkSave[8][80];
int dropGoldValue; // idb
int drawmanaflag; // idb
int chrbtnactive; // weak
char sgszTalkMsg[80];
BYTE *pPanelText;
int frame_4B8800; // idb
BYTE *pLifeBuff;
BYTE *pBtmBuff;
void *pTalkBtns;
int pstrjust[4];
int pnumlines; // idb
int pinfoflag; // weak
int talkbtndown[3];
int pSpell; // weak
BYTE *pManaBuff;
char infoclr; // weak
int sgbPlrTalkTbl; // weak // should be char [4]
void *pGBoxBuff;
void *pSBkBtnCel;
char tempstr[256];
char byte_4B894C[4];
int sbooktab; // weak
int pSplType; // weak
int frame; // idb
int initialDropGoldIndex; // idb
int talkflag; // weak
void *pSBkIconCels;
int sbookflag; // weak
int chrflag;
int drawbtnflag; // idb
void *pSpellBkCel;
char infostr[256];
int numpanbtns; // weak
void *pStatusPanel;
char panelstr[4][64];
int panelflag; // weak
BYTE SplTransTbl[256];
int initialDropGoldValue; // idb
void *pSpellCels;
int panbtndown; // weak
void *pTalkPanel; // idb
int spselflag; // weak

const unsigned char fontframe[128] =
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  54, 44, 57, 58, 56, 55, 47, 40, 41, 59, 39, 50, 37, 51, 52,
	36, 27, 28, 29, 30, 31, 32, 33, 34, 35, 48, 49, 60, 38, 61, 53,
	62, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 42, 63, 43, 64, 65,
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 40, 66, 41, 67, 0
};
const unsigned char fontkern[68] =
{
	8,  10,   7,   9,   8,   7,   6,   8,   8,   3,
	3,   8,   6,  11,   9,  10,   6,   9,   9,   6,
	9,  11,  10,  13,  10,  11,   7,   5,   7,   7,
	8,   7,   7,   7,   7,   7,  10,   4,   5,   6,
	3,   3,   4,   3,   6,   6,   3,   3,   3,   3,
	3,   2,   7,   6,   3,  10,  10,   6,   6,   7,
	4,   4,   9,   6,   6,  12,   3,   7
};
const int lineoffset[5][5] =
{
	{ SCREENXY(177, 434), BUFFER_WIDTH * 32, BUFFER_WIDTH * 32, BUFFER_WIDTH * 32, 180 + BUFFER_WIDTH * 32 },
	{ SCREENXY(177, 422), SCREENXY(177, 446), BUFFER_WIDTH * 32, BUFFER_WIDTH * 32, BUFFER_WIDTH * 32 },
	{ SCREENXY(177, 416), SCREENXY(177, 434), SCREENXY(177, 452), BUFFER_WIDTH * 32, BUFFER_WIDTH * 32 },
	{ SCREENXY(177, 412), SCREENXY(177, 427), SCREENXY(177, 441), SCREENXY(177, 456), BUFFER_WIDTH * 32 },
	{ SCREENXY(177, 410), SCREENXY(177, 422), SCREENXY(177, 434), SCREENXY(177, 446), SCREENXY(177, 457) }
};
const unsigned char gbFontTransTbl[256] =
{
	'\0', 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	' ',  '!',  '\"', '#',  '$',  '%',  '&',  '\'', '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
	'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
	'@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
	'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '[',  '\\', ']',  '^',  '_',
	'`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
	'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '{',  '|',  '}',  '~',  0x01,
	'C',  'u',  'e',  'a',  'a',  'a',  'a',  'c',  'e',  'e',  'e',  'i',  'i',  'i',  'A',  'A',
	'E',  'a',  'A',  'o',  'o',  'o',  'u',  'u',  'y',  'O',  'U',  'c',  'L',  'Y',  'P',  'f',
	'a',  'i',  'o',  'u',  'n',  'N',  'a',  'o',  '?',  0x01, 0x01, 0x01, 0x01, '!',  '<',  '>',
	'o',  '+',  '2',  '3',  '\'', 'u',  'P',  '.',  ',',  '1',  '0',  '>',  0x01, 0x01, 0x01, '?',
	'A',  'A',  'A',  'A',  'A',  'A',  'A',  'C',  'E',  'E',  'E',  'E',  'I',  'I',  'I',  'I',
	'D',  'N',  'O',  'O',  'O',  'O',  'O',  'X',  '0',  'U',  'U',  'U',  'U',  'Y',  'b',  'B',
	'a',  'a',  'a',  'a',  'a',  'a',  'a',  'c',  'e',  'e',  'e',  'e',  'i',  'i',  'i',  'i',
	'o',  'n',  'o',  'o',  'o',  'o',  'o',  '/',  '0',  'u',  'u',  'u',  'u',  'y',  'b',  'y'
};

/* data */

char SpellITbl[37] =
{
	1,   1,   2,   3,   4,   5,   6,   7,   8,   9,
   28,  13,  12,  18,  16,  14,  18,  19,  11,  20,
   15,  21,  23,  24,  25,  22,  26,  29,  37,  38,
   39,  42,  41,  40,  10,  36,  30
};
int PanBtnPos[8][5] =
{
  { 9,   361, 71, 19, 1 },
  { 9,   387, 71, 19, 0 },
  { 9,   427, 71, 19, 1 },
  { 9,   453, 71, 19, 0 },
  { 560, 361, 71, 19, 1 },
  { 560, 387, 71, 19, 0 },
  { 87,  443, 33, 32, 1 },
  { 527, 443, 33, 32, 1 }
};
char *PanBtnHotKey[8] = { "'c'", "'q'", "Tab", "Esc", "'i'", "'b'", "Enter", NULL };
char *PanBtnStr[8] =
{
  "Character Information",
  "Quests log",
  "Automap",
  "Main Menu",
  "Inventory",
  "Spell book",
  "Send Message",
  "Player Attack"
};
int attribute_inc_rects[4][4] =
{
  { 137, 138, 41, 22 },
  { 137, 166, 41, 22 },
  { 137, 195, 41, 22 },
  { 137, 223, 41, 22 }
};

int SpellPages[6][7] =
{
	{ SPL_NULL,		SPL_FIREBOLT,   SPL_CBOLT,	   SPL_HBOLT,	 SPL_HEAL,	   SPL_HEALOTHER, SPL_FLAME },
	{ SPL_RESURRECT,   SPL_FIREWALL,   SPL_TELEKINESIS, SPL_LIGHTNING, SPL_TOWN,	   SPL_FLASH,	 SPL_STONE },
	{ SPL_RNDTELEPORT, SPL_MANASHIELD, SPL_ELEMENT,	 SPL_FIREBALL,  SPL_WAVE,	   SPL_CHAIN,	 SPL_GUARDIAN },
	{ SPL_NOVA,		SPL_GOLEM,	  SPL_TELEPORT,	SPL_APOCA,	 SPL_BONESPIRIT, SPL_FLARE,	 SPL_ETHEREALIZE },
	{ -1,			  -1,			 -1,			  -1,			-1,			 -1,			-1 },
	{ -1,			  -1,			 -1,			  -1,			-1,			 -1,			-1 }
};

void DrawSpellCel(int xp, int yp, BYTE *pCelBuff, int nCel, int w)
{
	BYTE *dst, *tbl, *end;

	tbl = SplTransTbl;
	/// ASSERT: assert(gpBuffer);
	dst = &gpBuffer[xp + PitchTbl[yp]];

#ifdef USE_ASM
	__asm {
		mov		ebx, pCelBuff
		mov		eax, nCel
		shl		eax, 2
		add		ebx, eax
		mov		eax, [ebx+4]
		sub		eax, [ebx]
		mov		end, eax
		mov		esi, pCelBuff
		add		esi, [ebx]
		mov		edi, dst
		mov		eax, end
		add		eax, esi
		mov		end, eax
		mov		ebx, tbl
	label1:
		mov		edx, w
	label2:
		xor		eax, eax
		lodsb
		or		al, al
		js		label6
		sub		edx, eax
		mov		ecx, eax
		shr		ecx, 1
		jnb		label3
		lodsb
		xlat
		stosb
		jecxz	label5
	label3:
		shr		ecx, 1
		jnb		label4
		lodsw
		xlat
		ror		ax, 8
		xlat
		ror		ax, 8
		stosw
		jecxz	label5
	label4:
		lodsd
		xlat
		ror		eax, 8
		xlat
		ror		eax, 8
		xlat
		ror		eax, 8
		xlat
		ror		eax, 8
		stosd
		loop	label4
	label5:
		or		edx, edx
		jz		label7
		jmp		label2
	label6:
		neg		al
		add		edi, eax
		sub		edx, eax
		jnz		label2
	label7:
		sub		edi, 768
		sub		edi, w
		cmp		esi, end
		jnz		label1
	}
#else
	int i;
	BYTE width;
	BYTE *src;
	DWORD *pFrameTable;

	pFrameTable = (DWORD *)pCelBuff;
	src = &pCelBuff[pFrameTable[nCel]];
	end = &src[pFrameTable[nCel + 1] - pFrameTable[nCel]];

	for(; src != end; dst -= 768 + w) {
		for(i = w; i;) {
			width = *src++;
			if(!(width & 0x80)) {
				i -= width;
				if(width & 1) {
					dst[0] = tbl[src[0]];
					src++;
					dst++;
				}
				width >>= 1;
				if(width & 1) {
					dst[0] = tbl[src[0]];
					dst[1] = tbl[src[1]];
					src += 2;
					dst += 2;
				}
				width >>= 1;
				for(; width; width--) {
					dst[0] = tbl[src[0]];
					dst[1] = tbl[src[1]];
					dst[2] = tbl[src[2]];
					dst[3] = tbl[src[3]];
					src += 4;
					dst += 4;
				}
			} else {
				width = -(char)width;
				dst += width;
				i -= width;
			}
		}
	}
#endif
}

void SetSpellTrans(char t)
{
	int i;

	if(t == RSPLTYPE_SKILL) {
		for(i = 0; i < 128; i++) {
			SplTransTbl[i] = i;
		}
	}

	for(i = 128; i < 256; i++) {
		SplTransTbl[i] = i;
	}

	SplTransTbl[255] = 0;

	switch(t) {
	case RSPLTYPE_SPELL:
		SplTransTbl[PAL8_YELLOW] = PAL16_BLUE + 1;
		SplTransTbl[PAL8_YELLOW + 1] = PAL16_BLUE + 3;
		SplTransTbl[PAL8_YELLOW + 2] = PAL16_BLUE + 5;
		for(i = PAL16_BLUE; i < PAL16_BLUE + 16; i++) {
			SplTransTbl[PAL16_BEIGE - PAL16_BLUE + i] = i;
			SplTransTbl[PAL16_YELLOW - PAL16_BLUE + i] = i;
			SplTransTbl[PAL16_ORANGE - PAL16_BLUE + i] = i;
		}
		break;
	case RSPLTYPE_SCROLL:
		SplTransTbl[PAL8_YELLOW] = PAL16_BEIGE + 1;
		SplTransTbl[PAL8_YELLOW + 1] = PAL16_BEIGE + 3;
		SplTransTbl[PAL8_YELLOW + 2] = PAL16_BEIGE + 5;
		for(i = PAL16_BEIGE; i < PAL16_BEIGE + 16; i++) {
			SplTransTbl[PAL16_YELLOW - PAL16_BEIGE + i] = i;
			SplTransTbl[PAL16_ORANGE - PAL16_BEIGE + i] = i;
		}
		break;
	case RSPLTYPE_CHARGES:
		SplTransTbl[PAL8_YELLOW] = PAL16_ORANGE + 1;
		SplTransTbl[PAL8_YELLOW + 1] = PAL16_ORANGE + 3;
		SplTransTbl[PAL8_YELLOW + 2] = PAL16_ORANGE + 5;
		for(i = PAL16_ORANGE; i < PAL16_ORANGE + 16; i++) {
			SplTransTbl[PAL16_BEIGE - PAL16_ORANGE + i] = i;
			SplTransTbl[PAL16_YELLOW - PAL16_ORANGE + i] = i;
		}
		break;
	case RSPLTYPE_INVALID:
		SplTransTbl[PAL8_YELLOW] = PAL16_GRAY + 1;
		SplTransTbl[PAL8_YELLOW + 1] = PAL16_GRAY + 3;
		SplTransTbl[PAL8_YELLOW + 2] = PAL16_GRAY + 5;
		for(i = PAL16_GRAY; i < PAL16_GRAY + 15; i++) {
			SplTransTbl[PAL16_BEIGE - PAL16_GRAY + i] = i;
			SplTransTbl[PAL16_YELLOW - PAL16_GRAY + i] = i;
			SplTransTbl[PAL16_ORANGE - PAL16_GRAY + i] = i;
		}
		SplTransTbl[PAL16_BEIGE + 15] = 0;
		SplTransTbl[PAL16_YELLOW + 15] = 0;
		SplTransTbl[PAL16_ORANGE + 15] = 0;
		break;
	}
}

void DrawSpell()
{
	char st, sn;
	int sl;

	st = plr[myplr]._pRSplType;
	sn = plr[myplr]._pRSpell;
	sl = plr[myplr]._pSplLvl[sn] + plr[myplr]._pISplLvlAdd;

	if(st == RSPLTYPE_SPELL && sn != -1) {
		if(!CheckSpell(myplr, sn, st, TRUE)) {
			st = RSPLTYPE_INVALID;
		}
		if(sl <= 0) {
			st = RSPLTYPE_INVALID;
		}
	}
	if(currlevel == 0 && st != RSPLTYPE_INVALID && !spelldata[sn].sTownSpell) {
		st = RSPLTYPE_INVALID;
	}
	if(plr[myplr]._pRSpell < 0) {
		st = RSPLTYPE_INVALID;
	}

	SetSpellTrans(st);

	if(sn != -1) {
		DrawSpellCel(629, 631, (BYTE *)pSpellCels, SpellITbl[sn], 56);
	} else {
		DrawSpellCel(629, 631, (BYTE *)pSpellCels, 27, 56);
	}
}

void DrawSpellList()
{
	int x, y, i, j, t, s, c, v, trans, lx, ly, nCel;
	__int64 mask, spl;

	x = 636;
	y = 495;
	pSpell = -1;
	infostr[0] = '\0';
	ClearPanel();

	for(t = 0; t < 4; t++) {
		switch(t) {
		case RSPLTYPE_SKILL:
			SetSpellTrans(RSPLTYPE_SKILL);
			spl = plr[myplr]._pAblSpells64;
			nCel = 46;
			break;
		case RSPLTYPE_SPELL:
			spl = plr[myplr]._pMemSpells64;
			nCel = 47;
			break;
		case RSPLTYPE_SCROLL:
			SetSpellTrans(RSPLTYPE_SCROLL);
			spl = plr[myplr]._pScrlSpells64;
			nCel = 44;
			break;
		case RSPLTYPE_CHARGES:
			SetSpellTrans(RSPLTYPE_CHARGES);
			spl = plr[myplr]._pISpells64;
			nCel = 45;
			break;
		}
		mask = 1;
		for(s = 1; s < 37; s++) {
			if(spl & mask) {
				if(t == RSPLTYPE_SPELL) {
					v = plr[myplr]._pSplLvl[s] + plr[myplr]._pISplLvlAdd;
					if(v < 0) {
						v = 0;
					}
					if(v > 0) {
						trans = RSPLTYPE_SPELL;
					} else {
						trans = RSPLTYPE_INVALID;
					}
					SetSpellTrans(trans);
				}
				if(currlevel == 0 && !spelldata[s].sTownSpell) {
					SetSpellTrans(RSPLTYPE_INVALID);
				}
				DrawSpellCel(x, y, (BYTE *)pSpellCels, SpellITbl[s], 56);
				lx = x - 64;
				ly = y - 216;
				if(MouseX >= lx && MouseX < lx + 56 && MouseY >= ly && MouseY < ly + 56) {
					pSpell = s;
					pSplType = t;
					DrawSpellCel(x, y, (BYTE *)pSpellCels, nCel, 56);
					switch(t) {
					case RSPLTYPE_SKILL:
						sprintf(infostr, "%s Skill", spelldata[pSpell].sSkillText);
						break;
					case RSPLTYPE_SPELL:
						sprintf(infostr, "%s Spell", spelldata[pSpell].sNameText);
						if(pSpell == SPL_HBOLT) {
							sprintf(tempstr, "Damages undead only");
							AddPanelString(tempstr, TRUE);
						}
						if(v == 0) {
							sprintf(tempstr, "Spell Level 0 - Unusable");
						} else {
							sprintf(tempstr, "Spell Level %i", v);
						}
						AddPanelString(tempstr, TRUE);
						break;
					case RSPLTYPE_SCROLL:
						sprintf(infostr, "Scroll of %s", spelldata[pSpell].sNameText);
						c = 0;
						for(i = 0; i < plr[myplr]._pNumInv; i++) {
							if(plr[myplr].InvList[i]._itype != -1
							&& (plr[myplr].InvList[i]._iMiscId == IMISC_SCROLL
							|| plr[myplr].InvList[i]._iMiscId == IMISC_SCROLLT)
							&& plr[myplr].InvList[i]._iSpell == pSpell) {
								c++;
							}
						}
						for(i = 0; i < 8; i++) {
							if(plr[myplr].SpdList[i]._itype != -1
							&& (plr[myplr].SpdList[i]._iMiscId == IMISC_SCROLL
							|| plr[myplr].SpdList[i]._iMiscId == IMISC_SCROLLT)
							&& plr[myplr].SpdList[i]._iSpell == pSpell) {
								c++;
							}
						}
						if(c == 1) {
							strcpy(tempstr, "1 Scroll");
						} else {
							sprintf(tempstr, "%i Scrolls", c);
						}
						AddPanelString(tempstr, TRUE);
						break;
					case RSPLTYPE_CHARGES:
						sprintf(infostr, "Staff of %s", spelldata[pSpell].sNameText);
						if(plr[myplr].InvBody[4]._iCharges == 1) {
							strcpy(tempstr, "1 Charge");
						} else {
							sprintf(tempstr, "%i Charges", plr[myplr].InvBody[4]._iCharges);
						}
						AddPanelString(tempstr, TRUE);
						break;
					}
					for(j = 0; j < 4; j++) {
						if(plr[myplr]._pSplHotKey[j] == pSpell && plr[myplr]._pSplTHotKey[j] == pSplType) {
							DrawSpellCel(x, y, (BYTE *)pSpellCels, j + 48, 56);
							sprintf(tempstr, "Spell Hot Key #F%i", j + 5);
							AddPanelString(tempstr, TRUE);
						}
					}
				}
				x -= 56;
				if(x == 20) {
					x = 636;
					y -= 56;
				}
			}
			mask <<= 1;
		}
		if(spl != 0 && x != 636) {
			x -= 56;
		}
		if(x == 20) {
			x = 636;
			y -= 56;
		}
	}
}

void SetSpell()
{
	spselflag = 0;

	if(pSpell != -1) {
		ClearPanel();
		force_redraw = 255;
		plr[myplr]._pRSpell = pSpell;
		plr[myplr]._pRSplType = pSplType;
	}
}

void SetSpeedSpell(int slot)
{
	int i;

	if(pSpell != -1) {
		for(i = 0; i < 4; i++) {
			if(plr[myplr]._pSplHotKey[i] == pSpell && plr[myplr]._pSplTHotKey[i] == pSplType) {
				plr[myplr]._pSplHotKey[i] = -1;
			}
		}
		plr[myplr]._pSplHotKey[slot] = pSpell;
		plr[myplr]._pSplTHotKey[slot] = pSplType;
	}
}

void ToggleSpell(int slot)
{
	__int64 spl;

	if(plr[myplr]._pSplHotKey[slot] == -1) {
		return;
	}

	switch(plr[myplr]._pSplTHotKey[slot]) {
	case RSPLTYPE_SKILL:
		spl = plr[myplr]._pAblSpells64;
		break;
	case RSPLTYPE_SPELL:
		spl = plr[myplr]._pMemSpells64;
		break;
	case RSPLTYPE_SCROLL:
		spl = plr[myplr]._pScrlSpells64;
		break;
	case RSPLTYPE_CHARGES:
		spl = plr[myplr]._pISpells64;
		break;
	}

	if(spl & (__int64)1 << (plr[myplr]._pSplHotKey[slot] - 1)) {
		force_redraw = 255;
		plr[myplr]._pRSpell = plr[myplr]._pSplHotKey[slot];
		plr[myplr]._pRSplType = plr[myplr]._pSplTHotKey[slot];
	}
}

void PrintChar(int nOffset, int nCel, char col)
{
	/// ASSERT: assert(gpBuffer);

#ifdef USE_ASM
	__asm {
		mov		ebx, pPanelText
		mov		eax, nCel
		shl		eax, 2
		add		ebx, eax
		mov		edx, [ebx+4]
		sub		edx, [ebx]
		mov		esi, pPanelText
		add		esi, [ebx]
		mov		edi, gpBuffer
		add		edi, nOffset
		mov		ebx, edx
		add		ebx, esi
		xor		edx, edx
		mov		dl, col
		cmp		edx, COL_WHITE
		jz		c0_label1
		cmp		edx, COL_BLUE
		jz		c1_label1
		cmp		edx, COL_RED
		jz		c2_label1
		jmp		d_label1

	// Case 0
	c0_label1:
		mov		edx, 13
	c0_label2:
		xor		eax, eax
		lodsb
		or		al, al
		js		c0_label6
		sub		edx, eax
		mov		ecx, eax
		shr		ecx, 1
		jnb		c0_label3
		movsb
		jecxz	c0_label5
	c0_label3:
		shr		ecx, 1
		jnb		c0_label4
		movsw
		jecxz	c0_label5
	c0_label4:
		rep movsd
	c0_label5:
		or		edx, edx
		jz		c0_label7
		jmp		c0_label2
	c0_label6:
		neg		al
		add		edi, eax
		sub		edx, eax
		jnz		c0_label2
	c0_label7:
		sub		edi, 768 + 13
		cmp		ebx, esi
		jnz		c0_label1
		jmp		labret

	// Case 1
	c1_label1:
		mov		edx, 13
	c1_label2:
		xor		eax, eax
		lodsb
		or		al, al
		js		c1_label6
		sub		edx, eax
		mov		ecx, eax
	c1_label3:
		lodsb
		cmp		al, PAL16_GRAY + 13
		ja		c1_label4
		cmp		al, PAL16_GRAY
		jb		c1_label5
		sub		al, PAL16_GRAY - (PAL16_BLUE + 2)
		jmp		c1_label5
	c1_label4:
		mov		al, PAL16_BLUE + 15
	c1_label5:
		stosb
		loop	c1_label3
		or		edx, edx
		jz		c1_label7
		jmp		c1_label2
	c1_label6:
		neg		al
		add		edi, eax
		sub		edx, eax
		jnz		c1_label2
	c1_label7:
		sub		edi, 768 + 13
		cmp		ebx, esi
		jnz		c1_label1
		jmp		labret

	// Case 2
	c2_label1:
		mov		edx, 13
	c2_label2:
		xor		eax, eax
		lodsb
		or		al, al
		js		c2_label5
		sub		edx, eax
		mov		ecx, eax
	c2_label3:
		lodsb
		cmp		al, PAL16_GRAY
		jb		c2_label4
		sub		al, PAL16_GRAY - PAL16_RED
	c2_label4:
		stosb
		loop	c2_label3
		or		edx, edx
		jz		c2_label6
		jmp		c2_label2
	c2_label5:
		neg		al
		add		edi, eax
		sub		edx, eax
		jnz		c2_label2
	c2_label6:
		sub		edi, 768 + 13
		cmp		ebx, esi
		jnz		c2_label1
		jmp		labret

	// Default
	d_label1:
		mov		edx, 13
	d_label2:
		xor		eax, eax
		lodsb
		or		al, al
		js		d_label6
		sub		edx, eax
		mov		ecx, eax
	d_label3:
		lodsb
		cmp		al, PAL16_GRAY
		jb		d_label5
		cmp		al, PAL16_GRAY + 14
		jnb		d_label4
		sub		al, PAL16_GRAY - (PAL16_YELLOW + 2)
		jmp		d_label5
	d_label4:
		mov		al, PAL16_YELLOW + 15
	d_label5:
		stosb
		loop	d_label3
		or		edx, edx
		jz		d_label7
		jmp		d_label2
	d_label6:
		neg		al
		add		edi, eax
		sub		edx, eax
		jnz		d_label2
	d_label7:
		sub		edi, 768 + 13
		cmp		ebx, esi
		jnz		d_label1

	labret:
	}
#else
	int i;
	BYTE width, pix;
	BYTE *src, *dst, *end;
	DWORD *pFrameTable;

	pFrameTable = (DWORD *)pPanelText;
	src = &pPanelText[pFrameTable[nCel]];
	end = &src[pFrameTable[nCel + 1] - pFrameTable[nCel]];
	dst = &gpBuffer[nOffset];

	switch(col) {
	case COL_WHITE:
		for(; src != end; dst -= 768 + 13) {
			for(i = 13; i;) {
				width = *src++;
				if(!(width & 0x80)) {
					i -= width;
					if(width & 1) {
						dst[0] = src[0];
						src++;
						dst++;
					}
					width >>= 1;
					if(width & 1) {
						dst[0] = src[0];
						dst[1] = src[1];
						src += 2;
						dst += 2;
					}
					width >>= 1;
					while(width) {
						dst[0] = src[0];
						dst[1] = src[1];
						dst[2] = src[2];
						dst[3] = src[3];
						src += 4;
						dst += 4;
						width--;
					}
				} else {
					width = -(char)width;
					dst += width;
					i -= width;
				}
			}
		}
		break;
	case COL_BLUE:
		for(; src != end; dst -= 768 + 13) {
			for(i = 13; i;) {
				width = *src++;
				if(!(width & 0x80)) {
					i -= width;
					while(width) {
						pix = *src++;
						if(pix > PAL16_GRAY + 13) {
							pix = PAL16_BLUE + 15;
						} else if(pix >= PAL16_GRAY) {
							pix -= PAL16_GRAY - (PAL16_BLUE + 2);
						}
						*dst++ = pix;
						width--;
					}
				} else {
					width = -(char)width;
					dst += width;
					i -= width;
				}
			}
		}
		break;
	case COL_RED:
		for(; src != end; dst -= 768 + 13) {
			for(i = 13; i;) {
				width = *src++;
				if(!(width & 0x80)) {
					i -= width;
					while(width) {
						pix = *src++;
						if(pix >= PAL16_GRAY) {
							pix -= PAL16_GRAY - PAL16_RED;
						}
						*dst++ = pix;
						width--;
					}
				} else {
					width = -(char)width;
					dst += width;
					i -= width;
				}
			}
		}
		break;
	default:
		for(; src != end; dst -= 768 + 13) {
			for(i = 13; i;) {
				width = *src++;
				if(!(width & 0x80)) {
					i -= width;
					while(width) {
						pix = *src++;
						if(pix >= PAL16_GRAY) {
							if(pix >= PAL16_GRAY + 14) {
								pix = PAL16_YELLOW + 15;
							} else {
								pix -= PAL16_GRAY - (PAL16_YELLOW + 2);
							}
						}
						*dst++ = pix;
						width--;
					}
				} else {
					width = -(char)width;
					dst += width;
					i -= width;
				}
			}
		}
		break;
	}
#endif
}

void AddPanelString(const char *str, BOOL just)
{
	strcpy(panelstr[pnumlines], str);
	pstrjust[pnumlines] = just;

	if(pnumlines < 4) {
		pnumlines++;
	}
}

void ClearPanel()
{
	pnumlines = 0;
	pinfoflag = 0;
}

void DrawPanelBox(int x, int y, int w, int h, int sx, int sy)
{
	int nSrcOff, nDstOff;

	/// ASSERT: assert(gpBuffer);

	nSrcOff = x + 640 * y;
	nDstOff = sx + 768 * sy;

#ifdef USE_ASM
	__asm {
		mov		esi, pBtmBuff
		add		esi, nSrcOff
		mov		edi, gpBuffer
		add		edi, nDstOff
		xor		ebx, ebx
		mov		bx, word ptr w
		xor		edx, edx
		mov		dx, word ptr h
	label1:
		mov		ecx, ebx
		shr		ecx, 1
		jnb		label2
		movsb
		jecxz	label4
	label2:
		shr		ecx, 1
		jnb		label3
		movsw
		jecxz	label4
	label3:
		rep movsd
	label4:
		add		esi, 640
		sub		esi, ebx
		add		edi, 768
		sub		edi, ebx
		dec		edx
		jnz		label1
	}
#else
	int wdt, hgt;
	BYTE *src, *dst;

	src = &pBtmBuff[nSrcOff];
	dst = &gpBuffer[nDstOff];

	for(hgt = h; hgt; hgt--, src += 640 - w, dst += 768 - w) {
		wdt = w;
		if(wdt & 1) {
			dst[0] = src[0];
			src++;
			dst++;
		}
		wdt >>= 1;
		if(wdt & 1) {
			dst[0] = src[0];
			dst[1] = src[1];
			src += 2;
			dst += 2;
		}
		wdt >>= 1;
		while(wdt) {
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = src[3];
			src += 4;
			dst += 4;
			wdt--;
		}
	}
#endif
}

void SetFlaskHeight(BYTE *pCelBuff, int min, int max, int x, int y)
{
	int nSrcOff, nDstOff, w;

	/// ASSERT: assert(gpBuffer);

	nSrcOff = 88 * min;
	nDstOff = x + 768 * y;
	w = max - min;

#ifdef USE_ASM
	__asm {
		mov		esi, pCelBuff
		add		esi, nSrcOff
		mov		edi, gpBuffer
		add		edi, nDstOff
		mov		edx, w
	label1:
		mov		ecx, 88 / 4
		rep movsd
		add		edi, 768 - 88
		dec		edx
		jnz		label1
	}
#else
	BYTE *src, *dst;

	src = &pCelBuff[nSrcOff];
	dst = &gpBuffer[nDstOff];

	for(; w; w--, src += 88, dst += 768) {
		memcpy(dst, src, 88);
	}
#endif
}

void DrawFlask(BYTE *pCelBuff, int w, int nSrcOff, BYTE *pBuff, int nDstOff, int h)
{
#ifdef USE_ASM
	__asm {
		mov		esi, pCelBuff
		add		esi, nSrcOff
		mov		edi, pBuff
		add		edi, nDstOff
		mov		edx, h
	label1:
		mov		ecx, 59
	label2:
		lodsb
		or		al, al
		jz		label3
		mov		[edi], al
	label3:
		inc		edi
		loop	label2
		add		esi, w
		sub		esi, 59
		add		edi, 768 - 59
		dec		edx
		jnz		label1
	}
#else
	int wdt, hgt;
	BYTE *src, *dst;

	src = &pCelBuff[nSrcOff];
	dst = &pBuff[nDstOff];

	for(hgt = h; hgt; hgt--, src += w - 59, dst += 768 - 59) {
		for(wdt = 59; wdt; wdt--) {
			if(*src != 0) {
				*dst = *src;
			}
			src++;
			dst++;
		}
	}
#endif
}

void DrawLifeFlask()
{
	int p;

	p = (double)plr[myplr]._pHitPoints / (double)plr[myplr]._pMaxHP * 80.0;
	plr[myplr]._pHPPer = p;
	if(p > 80) {
		p = 80;
	}
	p = 80 - p;
	if(p > 11) {
		p = 11;
	}
	p += 2;

	/// ASSERT: assert(gpBuffer);
	DrawFlask(pLifeBuff, 88, 13 + 88 * 3, gpBuffer, SCREENXY(109, 339), p);

	if(p != 13) {
		DrawFlask(pBtmBuff, 640, 109 + (p + 3) * 640, gpBuffer, SCREENXY(109, 339 + p), 13 - p);
	}
}

void UpdateLifeFlask()
{
	int p;

	p = (double)plr[myplr]._pHitPoints / (double)plr[myplr]._pMaxHP * 80.0;
	plr[myplr]._pHPPer = p;
	if(p > 69) {
		p = 69;
	} else if(p < 0) {
		p = 0;
	}

	if(p != 69) {
		SetFlaskHeight(pLifeBuff, 16, 16 + 69 - p, 96 + SCREEN_X, 352 + SCREEN_Y);
	}
	if(p != 0) {
		DrawPanelBox(96, 16 + 69 - p, 88, p, 96 + SCREEN_X, 352 + 69 - p + SCREEN_Y);
	}
}

void DrawManaFlask()
{
	int p;

	p = plr[myplr]._pManaPer;
	if(p > 80) {
		p = 80;
	}
	p = 80 - p;
	if(p > 11) {
		p = 11;
	}
	p += 2;

	/// ASSERT: assert(gpBuffer);
	DrawFlask(pManaBuff, 88, 13 + 88 * 3, gpBuffer, SCREENXY(475, 339), p);

	if(p != 13) {
		DrawFlask(pBtmBuff, 640, 475 + (p + 3) * 640, gpBuffer, SCREENXY(475, 339 + p), 13 - p);
	}
}

void control_update_life_mana()
{
	int cm, mm, p;

	mm = plr[myplr]._pMaxMana;
	cm = plr[myplr]._pMana;
	if(mm < 0) {
		mm = 0;
	}
	if(cm < 0) {
		cm = 0;
	}

	if(mm == 0) {
		p = 0;
	} else {
		p = (double)cm / (double)mm * 80.0;
	}

	plr[myplr]._pManaPer = p;
	p = (double)plr[myplr]._pHitPoints / (double)plr[myplr]._pMaxHP * 80.0;
	plr[myplr]._pHPPer = p;
}

void UpdateManaFlask()
{
	int cm, mm, p;

	mm = plr[myplr]._pMaxMana;
	cm = plr[myplr]._pMana;
	if(mm < 0) {
		mm = 0;
	}
	if(cm < 0) {
		cm = 0;
	}

	if(mm == 0) {
		p = 0;
	} else {
		p = (double)cm / (double)mm * 80.0;
	}
	plr[myplr]._pManaPer = p;
	if(p > 69) {
		p = 69;
	}

	if(p != 69) {
		SetFlaskHeight(pManaBuff, 16, 16 + 69 - p, 464 + SCREEN_X, 352 + SCREEN_Y);
	}
	if(p != 0) {
		DrawPanelBox(464, 16 + 69 - p, 88, p, 464 + SCREEN_X, 352 + 69 - p + SCREEN_Y);
	}

	DrawSpell();
}

void InitControlPan()
{
	int i;

	/// ASSERT: assert(! pBtmBuff);
	if(gbMaxPlayers == 1) {
		pBtmBuff = DiabloAllocPtr(0x16800);
		memset(pBtmBuff, 0, 0x16800);
	} else {
		pBtmBuff = DiabloAllocPtr(0x2D000);
		memset(pBtmBuff, 0, 0x2D000);
	}

	pManaBuff = DiabloAllocPtr(88 * 88);
	memset(pManaBuff, 0, 88 * 88);
	pLifeBuff = DiabloAllocPtr(88 * 88);
	memset(pLifeBuff, 0, 88 * 88);
	pPanelText = DiabLoad("CtrlPan\\SmalText.CEL", NULL, 'CTRL');
	pChrPanel = DiabLoad("Data\\Char.CEL", NULL, 'CTRL');
	pSpellCels = DiabLoad("CtrlPan\\SpelIcon.CEL", NULL, 'CTRL');

	SetSpellTrans(RSPLTYPE_SKILL);

	pStatusPanel = DiabLoad("CtrlPan\\Panel8.CEL", NULL, 'CTRL');
	CelDecodeRect((BYTE *)pBtmBuff, 0, 143, 640, (BYTE *)pStatusPanel, 1, 640);
	MemFreeDbg(pStatusPanel);
	pStatusPanel = DiabLoad("CtrlPan\\P8Bulbs.CEL", NULL, 'CTRL');
	CelDecodeRect((BYTE *)pLifeBuff, 0, 87, 88, (BYTE *)pStatusPanel, 1, 88);
	CelDecodeRect((BYTE *)pManaBuff, 0, 87, 88, (BYTE *)pStatusPanel, 2, 88);
	MemFreeDbg(pStatusPanel);

	talkflag = 0;
	if(gbMaxPlayers != 1) {
		pTalkPanel = DiabLoad("CtrlPan\\TalkPanl.CEL", NULL, 'CTRL');
		CelDecodeRect((BYTE *)pBtmBuff, 0, 287, 640, (BYTE *)pTalkPanel, 1, 640);
		MemFreeDbg(pTalkPanel);
		pMultiBtns = DiabLoad("CtrlPan\\P8But2.CEL", NULL, 'CTRL');
		pTalkBtns = DiabLoad("CtrlPan\\TalkButt.CEL", NULL, 'CTRL');
		sgbPlrTalkTbl = 0;
		sgszTalkMsg[0] = '\0';
		for(i = 0; i < 4; i++) {
			byte_4B894C[i] = 1;
		}
		for(i = 0; i < 3; i++) {
			talkbtndown[i] = 0;
		}
	}

	panelflag = 0;
	lvlbtndown = 0;
	pPanelButtons = DiabLoad("CtrlPan\\Panel8bu.CEL", NULL, 'CTRL');
	for(i = 0; i < 8; i++) {
		panbtn[i] = 0;
	}
	panbtndown = 0;
	if(gbMaxPlayers == 1) {
		numpanbtns = 6;
	} else {
		numpanbtns = 8;
	}

	pChrButtons = DiabLoad("Data\\CharBut.CEL", NULL, 'CTRL');
	for(i = 0; i < 4; i++) {
		chrbtn[i] = 0;
	}
	chrbtnactive = 0;

	pDurIcons = DiabLoad("Items\\DurIcons.CEL", NULL, 'CTRL');
	strcpy(infostr, "");
	ClearPanel();
	drawhpflag = 1;
	drawmanaflag = 1;
	chrflag = 0;
	spselflag = 0;
	pSpellBkCel = DiabLoad("Data\\SpellBk.CEL", NULL, 'CTRL');
	pSBkBtnCel = DiabLoad("Data\\SpellBkB.CEL", NULL, 'CTRL');
	pSBkIconCels = DiabLoad("Data\\SpellI2.CEL", NULL, 'CTRL');
	sbooktab = 0;
	sbookflag = 0;

	if(plr[myplr]._pClass == PC_WARRIOR) {
		SpellPages[0][0] = SPL_REPAIR;
	} else if(plr[myplr]._pClass == PC_ROGUE) {
		SpellPages[0][0] = SPL_DISARM;
	} else if(plr[myplr]._pClass == PC_SORCERER) {
		SpellPages[0][0] = SPL_RECHARGE;
	}

	pQLogCel = DiabLoad("Data\\Quest.CEL", NULL, 'CTRL');
	pGBoxBuff = DiabLoad("CtrlPan\\Golddrop.cel", NULL, 'CTRL');
	dropGoldFlag = 0;
	dropGoldValue = 0;
	initialDropGoldValue = 0;
	initialDropGoldIndex = 0;
	frame_4B8800 = 1;
}

void DrawCtrlPan()
{
	DrawPanelBox(0, sgbPlrTalkTbl + 16, 640, 128, 0 + SCREEN_X, 352 + SCREEN_Y);
	DrawInfoBox();
}

void DrawCtrlBtns()
{
	int i;

	for(i = 0; i < 6; i++) {
		if(!panbtn[i]) {
			DrawPanelBox(PanBtnPos[i][0], PanBtnPos[i][1] - 336, 71, 20, PanBtnPos[i][0] + SCREEN_X, PanBtnPos[i][1] + SCREEN_Y);
		} else {
			CelDecodeOnly(PanBtnPos[i][0] + SCREEN_X, PanBtnPos[i][1] + 18 + SCREEN_Y, (BYTE *)pPanelButtons, i + 1, 71);
		}
	}

	if(numpanbtns == 8) {
		CelDecodeOnly(87 + SCREEN_X, 474 + SCREEN_Y, (BYTE *)pMultiBtns, panbtn[6] + 1, 33);
		if(FriendlyMode) {
			CelDecodeOnly(527 + SCREEN_X, 474 + SCREEN_Y, (BYTE *)pMultiBtns, panbtn[7] + 3, 33);
		} else {
			CelDecodeOnly(527 + SCREEN_X, 474 + SCREEN_Y, (BYTE *)pMultiBtns, panbtn[7] + 5, 33);
		}
	}
}

void DoSpeedBook()
{
	int x, y, cx, cy, t, s;
	__int64 spl, mask;

	spselflag = 1;
	x = 636;
	y = 495;
	cx = x - 36;
	cy = y - 188;

	if(plr[myplr]._pRSpell != -1) {
		for(t = 0; t < 4; t++) {
			switch(t) {
			case RSPLTYPE_SKILL:
				spl = plr[myplr]._pAblSpells64;
				break;
			case RSPLTYPE_SPELL:
				spl = plr[myplr]._pMemSpells64;
				break;
			case RSPLTYPE_SCROLL:
				spl = plr[myplr]._pScrlSpells64;
				break;
			case RSPLTYPE_CHARGES:
				spl = plr[myplr]._pISpells64;
				break;
			}
			mask = 1;
			for(s = 1; s < 37; s++) {
				if(spl & mask) {
					if(s == plr[myplr]._pRSpell && t == plr[myplr]._pRSplType) {
						cx = x - 36;
						cy = y - 188;
					}
					x -= 56;
					if(x == 20) {
						x = 636;
						y -= 56;
					}
				}
				mask <<= 1;
			}
			if(spl != 0 && x != 636) {
				x -= 56;
			}
			if(x == 20) {
				x = 636;
				y -= 56;
			}
		}
	}

	SetCursorPos(cx, cy);
}

void DoPanBtn()
{
	int i, x2, y2;

	for(i = 0; i < numpanbtns; i++) {
		x2 = PanBtnPos[i][0] + PanBtnPos[i][2];
		y2 = PanBtnPos[i][1] + PanBtnPos[i][3];
		if(MouseX >= PanBtnPos[i][0] && MouseX <= x2 && MouseY >= PanBtnPos[i][1] && MouseY <= y2) {
			panbtn[i] = 1;
			drawbtnflag = 1;
			panbtndown = 1;
		}
	}

	if(!spselflag && MouseX >= 565 && MouseX < 565 + 56 && MouseY >= 416 && MouseY < 416 + 56) {
		DoSpeedBook();
		gamemenu_off();
	}
}

void control_set_button_down(int btn_id)
{
	panbtn[btn_id] = 1;
	drawbtnflag = 1;
	panbtndown = 1;
}

void control_check_btn_press()
{
	int x2, y2;

	x2 = PanBtnPos[3][0] + PanBtnPos[3][2];
	y2 = PanBtnPos[3][1] + PanBtnPos[3][3];
	if(MouseX >= PanBtnPos[3][0] && MouseX <= x2 && MouseY >= PanBtnPos[3][1] && MouseY <= y2) {
		control_set_button_down(3);
	}
	x2 = PanBtnPos[6][0] + PanBtnPos[6][2];
	y2 = PanBtnPos[6][1] + PanBtnPos[6][3];
	if(MouseX >= PanBtnPos[6][0] && MouseX <= x2 && MouseY >= PanBtnPos[6][1] && MouseY <= y2) {
		control_set_button_down(6);
	}
}

void DoAutoMap()
{
	if(currlevel == 0 && gbMaxPlayers == 1) {
		InitDiabloMsg(1);
		return;
	}

	if(!automapflag) {
		StartAutomap();
	} else {
		automapflag = 0;
	}
}

void CheckPanelInfo()
{
	int i, j, x2, y2, c, s, v;

	panelflag = 0;
	ClearPanel();

	for(i = 0; i < numpanbtns; i++) {
		x2 = PanBtnPos[i][0] + PanBtnPos[i][2];
		y2 = PanBtnPos[i][1] + PanBtnPos[i][3];
		if(MouseX >= PanBtnPos[i][0] && MouseX <= x2 && MouseY >= PanBtnPos[i][1] && MouseY <= y2) {
			if(i != 7) {
				strcpy(infostr, PanBtnStr[i]);
			} else if(FriendlyMode) {
				strcpy(infostr, "Player friendly");
			} else {
				strcpy(infostr, "Player attack");
			}
			if(PanBtnHotKey[i] != 0) {
				sprintf(tempstr, "Hotkey : %s", PanBtnHotKey[i]);
				AddPanelString(tempstr, TRUE);
			}
			infoclr = COL_WHITE;
			panelflag = 1;
			pinfoflag = 1;
		}
	}

	if(!spselflag && MouseX >= 565 && MouseX < 565 + 56 && MouseY >= 416 && MouseY < 416 + 56) {
		strcpy(infostr, "Select current spell button");
		infoclr = COL_WHITE;
		panelflag = 1;
		pinfoflag = 1;
		strcpy(tempstr, "Hotkey : 's'");
		AddPanelString(tempstr, TRUE);
		s = plr[myplr]._pRSpell;
		if(s != -1) {
			switch(plr[myplr]._pRSplType) {
			case RSPLTYPE_SKILL:
				sprintf(tempstr, "%s Skill", spelldata[s].sSkillText);
				AddPanelString(tempstr, TRUE);
				break;
			case RSPLTYPE_SPELL:
				sprintf(tempstr, "%s Spell", spelldata[s].sNameText);
				AddPanelString(tempstr, TRUE);
				v = plr[myplr]._pSplLvl[s] + plr[myplr]._pISplLvlAdd;
				if(v < 0) {
					v = 0;
				}
				if(v == 0) {
					sprintf(tempstr, "Spell Level 0 - Unusable");
				} else {
					sprintf(tempstr, "Spell Level %i", v);
				}
				AddPanelString(tempstr, TRUE);
				break;
			case RSPLTYPE_SCROLL:
				sprintf(tempstr, "Scroll of %s", spelldata[s].sNameText);
				AddPanelString(tempstr, TRUE);
				c = 0;
				for(j = 0; j < plr[myplr]._pNumInv; j++) {
					if(plr[myplr].InvList[j]._itype != -1
					&& (plr[myplr].InvList[j]._iMiscId == IMISC_SCROLL
					|| plr[myplr].InvList[j]._iMiscId == IMISC_SCROLLT)
					&& plr[myplr].InvList[j]._iSpell == s) {
						c++;
					}
				}
				for(j = 0; j < 8; j++) {
					if(plr[myplr].SpdList[j]._itype != -1
					&& (plr[myplr].SpdList[j]._iMiscId == IMISC_SCROLL
					|| plr[myplr].SpdList[j]._iMiscId == IMISC_SCROLLT)
					&& plr[myplr].SpdList[j]._iSpell == s) {
						c++;
					}
				}
				if(c == 1) {
					strcpy(tempstr, "1 Scroll");
				} else {
					sprintf(tempstr, "%i Scrolls", c);
				}
				AddPanelString(tempstr, TRUE);
				break;
			case RSPLTYPE_CHARGES:
				sprintf(tempstr, "Staff of %s", spelldata[s].sNameText);
				AddPanelString(tempstr, TRUE);
				if(plr[myplr].InvBody[4]._iCharges == 1) {
					strcpy(tempstr, "1 Charge");
				} else {
					sprintf(tempstr, "%i Charges", plr[myplr].InvBody[4]._iCharges);
				}
				AddPanelString(tempstr, TRUE);
				break;
			}
		}
	}
	if(MouseX > 190 && MouseX < 437 && MouseY > 356 && MouseY < 385) {
		pcursinvitem = CheckInvHLight();
	}
}

void CheckBtnUp()
{
	int i;
	BOOLEAN menu;

	menu = TRUE;
	drawbtnflag = 1;
	panbtndown = 0;

	for(i = 0; i < 8; i++) {
		if(!panbtn[i]) {
			continue;
		}
		panbtn[i] = 0;
		if(MouseX < PanBtnPos[i][0]) {
			continue;
		}
		if(MouseX > PanBtnPos[i][0] + PanBtnPos[i][2]) {
			continue;
		}
		if(MouseY < PanBtnPos[i][1]) {
			continue;
		}
		if(MouseY > PanBtnPos[i][1] + PanBtnPos[i][3]) {
			continue;
		}
		switch(i) {
		case PANBTN_CHARINFO:
			questlog = 0;
			chrflag = !chrflag;
			break;
		case PANBTN_QLOG:
			chrflag = 0;
			if(!questlog) {
				StartQuestlog();
			} else {
				questlog = 0;
			}
			break;
		case PANBTN_AUTOMAP:
			DoAutoMap();
			break;
		case PANBTN_MAINMENU:
			qtextflag = 0;
			gamemenu_handle_previous();
			menu = FALSE;
			break;
		case PANBTN_INVENTORY:
			sbookflag = 0;
			invflag = !invflag;
			if(dropGoldFlag) {
				dropGoldFlag = 0;
				dropGoldValue = 0;
			}
			break;
		case PANBTN_SPELLBOOK:
			invflag = 0;
			if(dropGoldFlag) {
				dropGoldFlag = 0;
				dropGoldValue = 0;
			}
			sbookflag = !sbookflag;
			break;
		case PANBTN_SENDMSG:
			if(talkflag) {
				control_reset_talk();
			} else {
				control_type_message();
			}
			break;
		case PANBTN_FRIENDLY:
			FriendlyMode = !FriendlyMode;
			break;
		}
	}

	if(menu) {
		gamemenu_off();
	}
}

void FreeControlPan()
{
	MemFreeDbg(pBtmBuff);
	MemFreeDbg(pManaBuff);
	MemFreeDbg(pLifeBuff);
	MemFreeDbg(pPanelText);
	MemFreeDbg(pChrPanel);
	MemFreeDbg(pSpellCels);
	MemFreeDbg(pPanelButtons);
	MemFreeDbg(pMultiBtns);
	MemFreeDbg(pTalkBtns);
	MemFreeDbg(pChrButtons);
	MemFreeDbg(pDurIcons);
	MemFreeDbg(pQLogCel);
	MemFreeDbg(pSpellBkCel);
	MemFreeDbg(pSBkBtnCel);
	MemFreeDbg(pSBkIconCels);
	MemFreeDbg(pGBoxBuff);
}

BOOL control_WriteStringToBuffer(const char *str)
{
	int w;
	BYTE c;

	w = 0;
	while(*str != '\0') {
		c = gbFontTransTbl[(BYTE)*str];
		str++;
		w += fontkern[fontframe[c]];
		if(w >= 125) {
			return FALSE;
		}
	}

	return TRUE;
}

void DrawInfoBox()
{
	int nGold;

	DrawPanelBox(177, 62, 288, 60, 177 + SCREEN_X, 398 + SCREEN_Y);

	if(!panelflag && !trigflag && pcursinvitem == -1 && !spselflag) {
		infostr[0] = '\0';
		infoclr = COL_WHITE;
		ClearPanel();
	}

	if(spselflag || trigflag) {
		infoclr = COL_WHITE;
	} else if(pcurs >= CURSOR_FIRSTITEM) {
		if(plr[myplr].HoldItem._itype == ITYPE_GOLD) {
			nGold = plr[myplr].HoldItem._ivalue;
			sprintf(infostr, "%i gold %s", nGold, get_pieces_str(nGold));
		} else if(!plr[myplr].HoldItem._iStatFlag) {
			ClearPanel();
			AddPanelString("Requirements not met", TRUE);
			pinfoflag = 1;
		} else {
			if(plr[myplr].HoldItem._iIdentified) {
				strcpy(infostr, plr[myplr].HoldItem._iIName);
			} else {
				strcpy(infostr, plr[myplr].HoldItem._iName);
			}
			if(plr[myplr].HoldItem._iMagical == 1) {
				infoclr = COL_BLUE;
			}
			if(plr[myplr].HoldItem._iMagical == 2) {
				infoclr = COL_GOLD;
			}
		}
	} else {
		if(pcursitem != -1) {
			GetItemStr(pcursitem);
		}
		if(pcursobj != -1) {
			GetObjectStr(pcursobj);
		}
		if(pcursmonst != -1) {
			if(leveltype != DTYPE_TOWN) {
				infoclr = COL_WHITE;
				strcpy(infostr, monster[pcursmonst].mName);
				ClearPanel();
				if(monster[pcursmonst]._uniqtype != 0) {
					infoclr = COL_GOLD;
					PrintUniqueHistory();
				} else {
					PrintMonstHistory(monster[pcursmonst].MType->mtype);
				}
			} else {
				strcpy(infostr, towner[pcursmonst]._tName);
			}
		}
		if(pcursplr != -1) {
			infoclr = COL_GOLD;
			strcpy(infostr, plr[pcursplr]._pName);
			ClearPanel();
			sprintf(tempstr, "Level : %i", plr[pcursplr]._pLevel);
			AddPanelString(tempstr, TRUE);
			sprintf(tempstr, "Hit Points %i of %i", plr[pcursplr]._pHitPoints >> 6, plr[pcursplr]._pMaxHP >> 6);
			AddPanelString(tempstr, TRUE);
		}
	}

	if(infostr[0] != '\0' || pnumlines != 0) {
		PrintInfo();
	}
}

void PrintInfo()
{
	int i, nOffset1, nlines;

	if(talkflag) {
		return;
	}

	nOffset1 = 0;
	nlines = 1;
	if(infostr[0] != '\0') {
		CPrintString(0, infostr, TRUE, pnumlines);
		nOffset1 = 1;
		nlines = 0;
	}

	for(i = 0; i < pnumlines; i++) {
		CPrintString(i + nOffset1, panelstr[i], pstrjust[i], pnumlines - nlines);
	}
}

void CPrintString(int No, char *pszStr, BOOL Just, int lines)
{
	int nOffset, w, cw;
	BYTE c;
	char *s;

	nOffset = lineoffset[lines][No];
	w = 0;

	if(Just == TRUE) {
		cw = 0;
		s = pszStr;
		while(*s != '\0') {
			c = gbFontTransTbl[(BYTE)*s];
			s++;
			c = fontframe[c];
			cw += fontkern[c] + 2;
		}
		if(cw < 288) {
			w = (288 - cw) >> 1;
		}
		nOffset += w;
	}

	while(*pszStr != '\0') {
		c = gbFontTransTbl[(BYTE)*pszStr];
		pszStr++;
		c = fontframe[c];
		w += fontkern[c] + 2;
		if(c != 0 && w < 288) {
			PrintChar(nOffset, c, infoclr);
		}
		nOffset += fontkern[c] + 2;
	}
}

void PrintGameStr(int x, int y, const char *pszStr, int col)
{
	int nOffset;
	BYTE c;

	nOffset = x + SCREEN_X + PitchTbl[y + SCREEN_Y];

	while(*pszStr != '\0') {
		c = gbFontTransTbl[(BYTE)*pszStr];
		pszStr++;
		c = fontframe[c];
		if(c != 0) {
			PrintChar(nOffset, c, col);
		}
		nOffset += fontkern[c] + 1;
	}
}

void DrawChr()
{
	int hper, ac, pc;
	long mind, maxd;
	char c;
	char chrstr[64];

	CelDecodeOnly(64, 511, (BYTE *)pChrPanel, 1, 320);
	ADD_PlrStringXY(20, 32, 151, plr[myplr]._pName, COL_WHITE);

	if(plr[myplr]._pClass == PC_WARRIOR) {
		ADD_PlrStringXY(168, 32, 299, "Warrior", COL_WHITE);
	} else if(plr[myplr]._pClass == PC_ROGUE) {
		ADD_PlrStringXY(168, 32, 299, "Rogue", COL_WHITE);
	} else if(plr[myplr]._pClass == PC_SORCERER) {
		ADD_PlrStringXY(168, 32, 299, "Sorceror", COL_WHITE);
	}

	sprintf(chrstr, "%i", plr[myplr]._pLevel);
	ADD_PlrStringXY(66, 69, 109, chrstr, COL_WHITE);
	sprintf(chrstr, "%li", plr[myplr]._pExperience);
	ADD_PlrStringXY(216, 69, 300, chrstr, COL_WHITE);
	if(plr[myplr]._pLevel == 50) {
		strcpy(chrstr, "None");
		c = COL_GOLD;
	} else {
		sprintf(chrstr, "%li", plr[myplr]._pNextExper);
		c = COL_WHITE;
	}
	ADD_PlrStringXY(216, 97, 300, chrstr, c);

	sprintf(chrstr, "%i", plr[myplr]._pGold);
	ADD_PlrStringXY(216, 146, 300, chrstr, COL_WHITE);

	c = COL_WHITE;
	if(plr[myplr]._pIBonusAC > 0) {
		c = COL_BLUE;
	}
	if(plr[myplr]._pIBonusAC < 0) {
		c = COL_RED;
	}
	ac = plr[myplr]._pIAC + plr[myplr]._pIBonusAC + plr[myplr]._pDexterity / 5;
	sprintf(chrstr, "%i", ac);
	ADD_PlrStringXY(258, 183, 301, chrstr, c);

	c = COL_WHITE;
	if(plr[myplr]._pIBonusToHit > 0) {
		c = COL_BLUE;
	}
	if(plr[myplr]._pIBonusToHit < 0) {
		c = COL_RED;
	}
	hper = 50 + plr[myplr]._pIBonusToHit + (plr[myplr]._pDexterity >> 1);
	sprintf(chrstr, "%i%%", hper);
	ADD_PlrStringXY(258, 211, 301, chrstr, c);

	c = COL_WHITE;
	if(plr[myplr]._pIBonusDam > 0) {
		c = COL_BLUE;
	}
	if(plr[myplr]._pIBonusDam < 0) {
		c = COL_RED;
	}
	mind = plr[myplr]._pIMinDam;
	mind += mind * plr[myplr]._pIBonusDam / 100;
	mind += plr[myplr]._pIBonusDamMod;
	if(plr[myplr].InvBody[4]._itype == ITYPE_BOW) {
		if(plr[myplr]._pClass == PC_ROGUE) {
			mind += plr[myplr]._pDamageMod;
		} else {
			mind += plr[myplr]._pDamageMod >> 1;
		}
	} else {
		mind += plr[myplr]._pDamageMod;
	}
	maxd = plr[myplr]._pIMaxDam;
	maxd += maxd * plr[myplr]._pIBonusDam / 100;
	maxd += plr[myplr]._pIBonusDamMod;
	if(plr[myplr].InvBody[4]._itype == ITYPE_BOW) {
		if(plr[myplr]._pClass == PC_ROGUE) {
			maxd += plr[myplr]._pDamageMod;
		} else {
			maxd += plr[myplr]._pDamageMod >> 1;
		}
	} else {
		maxd += plr[myplr]._pDamageMod;
	}
	sprintf(chrstr, "%i-%i", mind, maxd);
	if(mind >= 100 || maxd >= 100) {
		MY_PlrStringXY(254, 239, 305, chrstr, c, -1);
	} else {
		MY_PlrStringXY(258, 239, 301, chrstr, c, 0);
	}

	if(plr[myplr]._pMagResist == 0) {
		c = COL_WHITE;
	} else {
		c = COL_BLUE;
	}
	if(plr[myplr]._pMagResist < 75) {
		sprintf(chrstr, "%i%%", plr[myplr]._pMagResist);
	} else {
		c = COL_GOLD;
		sprintf(chrstr, "MAX");
	}
	ADD_PlrStringXY(257, 276, 300, chrstr, c);
	if(plr[myplr]._pFireResist == 0) {
		c = COL_WHITE;
	} else {
		c = COL_BLUE;
	}
	if(plr[myplr]._pFireResist < 75) {
		sprintf(chrstr, "%i%%", plr[myplr]._pFireResist);
	} else {
		c = COL_GOLD;
		sprintf(chrstr, "MAX");
	}
	ADD_PlrStringXY(257, 304, 300, chrstr, c);
	if(plr[myplr]._pLghtResist == 0) {
		c = COL_WHITE;
	} else {
		c = COL_BLUE;
	}
	if(plr[myplr]._pLghtResist < 75) {
		sprintf(chrstr, "%i%%", plr[myplr]._pLghtResist);
	} else {
		c = COL_GOLD;
		sprintf(chrstr, "MAX");
	}
	ADD_PlrStringXY(257, 332, 300, chrstr, c);

	c = COL_WHITE;
	sprintf(chrstr, "%i", plr[myplr]._pBaseStr);
	if(MaxStats[plr[myplr]._pClass][0] == plr[myplr]._pBaseStr) {
		c = COL_GOLD;
	}
	ADD_PlrStringXY(95, 155, 126, chrstr, c);
	c = COL_WHITE;
	sprintf(chrstr, "%i", plr[myplr]._pBaseMag);
	if(MaxStats[plr[myplr]._pClass][1] == plr[myplr]._pBaseMag) {
		c = COL_GOLD;
	}
	ADD_PlrStringXY(95, 183, 126, chrstr, c);
	c = COL_WHITE;
	sprintf(chrstr, "%i", plr[myplr]._pBaseDex);
	if(MaxStats[plr[myplr]._pClass][2] == plr[myplr]._pBaseDex) {
		c = COL_GOLD;
	}
	ADD_PlrStringXY(95, 211, 126, chrstr, c);
	c = COL_WHITE;
	sprintf(chrstr, "%i", plr[myplr]._pBaseVit);
	if(MaxStats[plr[myplr]._pClass][3] == plr[myplr]._pBaseVit) {
		c = COL_GOLD;
	}
	ADD_PlrStringXY(95, 239, 126, chrstr, c);

	c = COL_WHITE;
	if(plr[myplr]._pStrength > plr[myplr]._pBaseStr) {
		c = COL_BLUE;
	}
	if(plr[myplr]._pStrength < plr[myplr]._pBaseStr) {
		c = COL_RED;
	}
	sprintf(chrstr, "%i", plr[myplr]._pStrength);
	ADD_PlrStringXY(143, 155, 173, chrstr, c);
	c = COL_WHITE;
	if(plr[myplr]._pMagic > plr[myplr]._pBaseMag) {
		c = COL_BLUE;
	}
	if(plr[myplr]._pMagic < plr[myplr]._pBaseMag) {
		c = COL_RED;
	}
	sprintf(chrstr, "%i", plr[myplr]._pMagic);
	ADD_PlrStringXY(143, 183, 173, chrstr, c);
	c = COL_WHITE;
	if(plr[myplr]._pDexterity > plr[myplr]._pBaseDex) {
		c = COL_BLUE;
	}
	if(plr[myplr]._pDexterity < plr[myplr]._pBaseDex) {
		c = COL_RED;
	}
	sprintf(chrstr, "%i", plr[myplr]._pDexterity);
	ADD_PlrStringXY(143, 211, 173, chrstr, c);
	c = COL_WHITE;
	if(plr[myplr]._pVitality > plr[myplr]._pBaseVit) {
		c = COL_BLUE;
	}
	if(plr[myplr]._pVitality < plr[myplr]._pBaseVit) {
		c = COL_RED;
	}
	sprintf(chrstr, "%i", plr[myplr]._pVitality);
	ADD_PlrStringXY(143, 239, 173, chrstr, c);

	if(plr[myplr]._pStatPts > 0 && CalcStatDiff(myplr) < plr[myplr]._pStatPts) {
		plr[myplr]._pStatPts = CalcStatDiff(myplr);
	}
	if(plr[myplr]._pStatPts > 0) {
		sprintf(chrstr, "%i", plr[myplr]._pStatPts);
		ADD_PlrStringXY(95, 266, 126, chrstr, COL_RED);
		pc = plr[myplr]._pClass;
		if(plr[myplr]._pBaseStr < MaxStats[pc][0]) {
			CelDecodeOnly(201, 319, (BYTE *)pChrButtons, chrbtn[0] + 2, 41);
		}
		if(plr[myplr]._pBaseMag < MaxStats[pc][1]) {
			CelDecodeOnly(201, 347, (BYTE *)pChrButtons, chrbtn[1] + 4, 41);
		}
		if(plr[myplr]._pBaseDex < MaxStats[pc][2]) {
			CelDecodeOnly(201, 376, (BYTE *)pChrButtons, chrbtn[2] + 6, 41);
		}
		if(plr[myplr]._pBaseVit < MaxStats[pc][3]) {
			CelDecodeOnly(201, 404, (BYTE *)pChrButtons, chrbtn[3] + 8, 41);
		}
	}

	if(plr[myplr]._pMaxHP > plr[myplr]._pMaxHPBase) {
		c = COL_BLUE;
	} else {
		c = COL_WHITE;
	}
	sprintf(chrstr, "%i", plr[myplr]._pMaxHP >> 6);
	ADD_PlrStringXY(95, 304, 126, chrstr, c);
	if(plr[myplr]._pHitPoints != plr[myplr]._pMaxHP) {
		c = COL_RED;
	}
	sprintf(chrstr, "%i", plr[myplr]._pHitPoints >> 6);
	ADD_PlrStringXY(143, 304, 174, chrstr, c);

	if(plr[myplr]._pMaxMana > plr[myplr]._pMaxManaBase) {
		c = COL_BLUE;
	} else {
		c = COL_WHITE;
	}
	sprintf(chrstr, "%i", plr[myplr]._pMaxMana >> 6);
	ADD_PlrStringXY(95, 332, 126, chrstr, c);
	if(plr[myplr]._pMana != plr[myplr]._pMaxMana) {
		c = COL_RED;
	}
	sprintf(chrstr, "%i", plr[myplr]._pMana >> 6);
	ADD_PlrStringXY(143, 332, 174, chrstr, c);
}

void ADD_PlrStringXY(int x, int y, int x2, const char *pszStr, char col)
{
	int nOffset, width, w, cw;
	BYTE c;
	const char *s;

	nOffset = x + SCREEN_X + PitchTbl[y + SCREEN_Y];
	width = x2 - x + 1;
	w = 0;
	cw = 0;

	s = pszStr;
	while(*s != '\0') {
		c = gbFontTransTbl[(BYTE)*s];
		s++;
		c = fontframe[c];
		cw += fontkern[c] + 1;
	}
	if(cw < width) {
		w = (width - cw) >> 1;
	}

	nOffset += w;
	while(*pszStr != '\0') {
		c = gbFontTransTbl[(BYTE)*pszStr];
		pszStr++;
		c = fontframe[c];
		w += fontkern[c] + 1;
		if(c != 0 && w < width) {
			PrintChar(nOffset, c, col);
		}
		nOffset += fontkern[c] + 1;
	}
}

void MY_PlrStringXY(int x, int y, int x2, const char *pszStr, char col, int base)
{
	int nOffset, width, w, cw;
	BYTE c;
	const char *s;

	nOffset = x + SCREEN_X + PitchTbl[y + SCREEN_Y];
	width = x2 - x + 1;
	w = 0;
	cw = 0;

	s = pszStr;
	while(*s != '\0') {
		c = gbFontTransTbl[(BYTE)*s];
		s++;
		c = fontframe[c];
		cw += fontkern[c] + base;
	}
	if(cw < width) {
		w = (width - cw) >> 1;
	}

	nOffset += w;
	while(*pszStr != '\0') {
		c = gbFontTransTbl[(BYTE)*pszStr];
		pszStr++;
		c = fontframe[c];
		w += fontkern[c] + base;
		if(c != 0 && w < width) {
			PrintChar(nOffset, c, col);
		}
		nOffset += fontkern[c] + base;
	}
}

void CheckLvlBtn()
{
	if(lvlbtndown) {
		return;
	}

	if(MouseX >= 40 && MouseX <= 81 && MouseY >= 313 && MouseY <= 335) {
		lvlbtndown = 1;
	}
}

void ReleaseLvlBtn()
{
	if(MouseX >= 40 && MouseX <= 81 && MouseY >= 313 && MouseY <= 335) {
		chrflag = 1;
	}

	lvlbtndown = 0;
}

void DrawLevelUpIcon()
{
	int nCel;

	if(stextflag) {
		return;
	}

	if(lvlbtndown) {
		nCel = 3;
	} else {
		nCel = 2;
	}
	ADD_PlrStringXY(0, 303, 120, "Level Up", COL_WHITE);
	CelDecodeOnly(104, 495, (BYTE *)pChrButtons, nCel, 41);
}

void CheckChrBtns()
{
	int i, pc, x2, y2;

	if(chrbtnactive) {
		return;
	}
	if(plr[myplr]._pStatPts == 0) {
		return;
	}

	pc = plr[myplr]._pClass;
	for(i = 0; i < 4; i++) {
		switch(i) {
		case 0:
			if(plr[myplr]._pBaseStr >= MaxStats[pc][0]) {
				continue;
			}
			break;
		case 1:
			if(plr[myplr]._pBaseMag >= MaxStats[pc][1]) {
				continue;
			}
			break;
		case 2:
			if(plr[myplr]._pBaseDex >= MaxStats[pc][2]) {
				continue;
			}
			break;
		case 3:
			if(plr[myplr]._pBaseVit >= MaxStats[pc][3]) {
				continue;
			}
			break;
		default:
			continue;
		}
		x2 = attribute_inc_rects[i][0] + attribute_inc_rects[i][2];
		y2 = attribute_inc_rects[i][1] + attribute_inc_rects[i][3];
		if(MouseX >= attribute_inc_rects[i][0] && MouseX <= x2
		&& MouseY >= attribute_inc_rects[i][1] && MouseY <= y2) {
			chrbtn[i] = 1;
			chrbtnactive = 1;
		}
	}
}

void ReleaseChrBtns()
{
	int i;

	chrbtnactive = 0;

	for(i = 0; i < 4; i++) {
		if(!chrbtn[i]) {
			continue;
		}
		chrbtn[i] = 0;
		if(MouseX < attribute_inc_rects[i][0]) {
			continue;
		}
		if(MouseX > attribute_inc_rects[i][0] + attribute_inc_rects[i][2]) {
			continue;
		}
		if(MouseY < attribute_inc_rects[i][1]) {
			continue;
		}
		if(MouseY > attribute_inc_rects[i][1] + attribute_inc_rects[i][3]) {
			continue;
		}
		switch(i) {
		case ATTRIB_STR:
			NetSendCmdParam1(TRUE, CMD_ADDSTR, 1);
			plr[myplr]._pStatPts--;
			break;
		case ATTRIB_MAG:
			NetSendCmdParam1(TRUE, CMD_ADDMAG, 1);
			plr[myplr]._pStatPts--;
			break;
		case ATTRIB_DEX:
			NetSendCmdParam1(TRUE, CMD_ADDDEX, 1);
			plr[myplr]._pStatPts--;
			break;
		case ATTRIB_VIT:
			NetSendCmdParam1(TRUE, CMD_ADDVIT, 1);
			plr[myplr]._pStatPts--;
			break;
		}
	}
}

void DrawDurIcon()
{
	int x;
	PlayerStruct *p;

	if((chrflag || questlog) && (invflag || sbookflag)) {
		return;
	}

	x = 656;
	if(invflag || sbookflag) {
		x -= 320;
	}

	p = &plr[myplr];
	x = DrawDurIcon4Item(&p->InvBody[0], x, 4);
	x = DrawDurIcon4Item(&p->InvBody[6], x, 3);
	x = DrawDurIcon4Item(&p->InvBody[4], x, 0);
	x = DrawDurIcon4Item(&p->InvBody[5], x, 0);
}

int DrawDurIcon4Item(const ItemStruct *pItem, int x, int c)
{
	if(pItem->_itype == -1) {
		return x;
	}
	if(pItem->_iDurability > 5) {
		return x;
	}

	if(c == 0) {
		if(pItem->_iClass == ICLASS_WEAPON) {
			switch(pItem->_itype) {
			case ITYPE_SWORD:
				c = 2;
				break;
			case ITYPE_AXE:
				c = 6;
				break;
			case ITYPE_BOW:
				c = 7;
				break;
			case ITYPE_MACE:
				c = 5;
				break;
			case ITYPE_STAFF:
				c = 8;
				break;
			}
		} else {
			c = 1;
		}
	}
	if(pItem->_iDurability > 2) {
		c += 8;
	}

	CelDecodeOnly(x, 495, (BYTE *)pDurIcons, c, 32);
	return x - 40;
}

void RedBack()
{
	int idx;

	if(light4flag) {
		idx = 1536;
	} else {
		idx = 4608;
	}

	/// ASSERT: assert(gpBuffer);

#ifdef USE_ASM
	if(leveltype != DTYPE_HELL) {
		__asm {
			mov		edi, gpBuffer
			add		edi, SCREENXY(0, 0)
			mov		ebx, pLightTbl
			add		ebx, idx
			mov		edx, 352
		lx_label1:
			mov		ecx, 640
		lx_label2:
			mov		al, [edi]
			xlat
			stosb
			loop	lx_label2
			add		edi, 768 - 640
			dec		edx
			jnz		lx_label1
		}
	} else {
		__asm {
			mov		edi, gpBuffer
			add		edi, SCREENXY(0, 0)
			mov		ebx, pLightTbl
			add		ebx, idx
			mov		edx, 352
		l4_label1:
			mov		ecx, 640
		l4_label2:
			mov		al, [edi]
			cmp		al, 32
			jb		l4_label3
			xlat
		l4_label3:
			stosb
			loop	l4_label2
			add		edi, 768 - 640
			dec		edx
			jnz		l4_label1
		}
	}
#else
	int w, h;
	BYTE *dst, *tbl;

	if(leveltype != DTYPE_HELL) {
		dst = &gpBuffer[SCREENXY(0, 0)];
		tbl = &pLightTbl[idx];
		for(h = 352; h != 0; h--, dst += 768 - 640) {
			for(w = 640; w != 0; w--) {
				*dst = tbl[*dst];
				dst++;
			}
		}
	} else {
		dst = &gpBuffer[SCREENXY(0, 0)];
		tbl = &pLightTbl[idx];
		for(h = 352; h != 0; h--, dst += 768 - 640) {
			for(w = 640; w != 0; w--) {
				if(*dst >= 32) {
					*dst = tbl[*dst];
				}
				dst++;
			}
		}
	}
#endif
}

char GetSBookTrans(int ii, BOOL townok)
{
	char st, sl;

	st = RSPLTYPE_SPELL;
	if(plr[myplr]._pISpells64 & (__int64)1 << (ii - 1)) {
		st = RSPLTYPE_CHARGES;
	}
	if(plr[myplr]._pAblSpells64 & 1 << (ii - 1)) { /// BUGFIX: missing (__int64)
		st = RSPLTYPE_SKILL;
	}
	if(st == RSPLTYPE_SPELL) {
		if(!CheckSpell(myplr, ii, RSPLTYPE_SPELL, TRUE)) {
			st = RSPLTYPE_INVALID;
		}
		sl = plr[myplr]._pSplLvl[ii] + plr[myplr]._pISplLvlAdd;
		if(sl <= 0) {
			st = RSPLTYPE_INVALID;
		}
	}
	if(townok && currlevel == 0 && st != RSPLTYPE_INVALID && !spelldata[ii].sTownSpell) {
		st = RSPLTYPE_INVALID;
	}

	return st;
}

void DrawSpellBook()
{
	int i, ii, x, y, mind, maxd, c, v;
	char st;
	__int64 tspls;

	CelDecodeOnly(384, 511, (BYTE *)pSpellBkCel, 1, 320);
	x = 76 * sbooktab + 391;
	CelDecodeOnly(x, 508, (BYTE *)pSBkBtnCel, sbooktab + 1, 76);

	y = 215;
	tspls = plr[myplr]._pMemSpells64 | plr[myplr]._pISpells64 | plr[myplr]._pAblSpells64;

	for(i = 1; i < 8; i++) { /* redundant, use 'i = 0; i < 7' */
		ii = SpellPages[sbooktab][i - 1];
		if(ii != -1 && tspls & (__int64)1 << (ii - 1)) {
			st = GetSBookTrans(ii, TRUE);
			SetSpellTrans(st);
			DrawSpellCel(395, y, (BYTE *)pSBkIconCels, SpellITbl[ii], 37);
			if(ii == plr[myplr]._pRSpell && st == plr[myplr]._pRSplType) {
				SetSpellTrans(RSPLTYPE_SKILL);
				DrawSpellCel(395, y, (BYTE *)pSBkIconCels, 43, 37);
			}
			PrintSBookStr(10, y - 23, FALSE, spelldata[ii].sNameText, COL_WHITE);
			st = GetSBookTrans(ii, FALSE);
			switch(st) {
			case RSPLTYPE_SKILL:
				strcpy(tempstr, "Skill");
				break;
			case RSPLTYPE_CHARGES:
				sprintf(tempstr, "Staff (%i charges)", plr[myplr].InvBody[4]._iCharges);
				break;
			default:
				c = GetManaAmount(myplr, ii) >> 6;
				GetDamageAmt(ii, &mind, &maxd);
				if(mind != -1) {
					sprintf(tempstr, "Mana: %i  Dam: %i - %i", c, mind, maxd);
				} else {
					sprintf(tempstr, "Mana: %i   Dam: n/a", c);
				}
				if(ii == SPL_BONESPIRIT) {
					sprintf(tempstr, "Mana: %i  Dam: 1/3 tgt hp", c);
				}
				PrintSBookStr(10, y - 1, FALSE, tempstr, COL_WHITE);
				v = plr[myplr]._pSplLvl[ii] + plr[myplr]._pISplLvlAdd;
				if(v < 0) {
					v = 0;
				}
				if(v == 0) {
					sprintf(tempstr, "Spell Level 0 - Unusable");
				} else {
					sprintf(tempstr, "Spell Level %i", v);
				}
				break;
			}
			PrintSBookStr(10, y - 12, FALSE, tempstr, COL_WHITE);
		}
		y += 43;
	}
}

void PrintSBookStr(int x, int y, BOOL cjustflag, const char *pszStr, char col)
{
	int nOffset, w, cw;
	BYTE c;
	const char *s;

	nOffset = x + PitchTbl[y] + 440;
	w = 0;

	if(cjustflag) {
		cw = 0;
		s = pszStr;
		while(*s != '\0') {
			c = gbFontTransTbl[(BYTE)*s];
			s++;
			c = fontframe[c];
			cw += fontkern[c] + 1;
		}
		if(cw < 222) {
			w = (222 - cw) >> 1;
		}
		nOffset += w;
	}

	while(*pszStr != '\0') {
		c = gbFontTransTbl[(BYTE)*pszStr];
		pszStr++;
		c = fontframe[c];
		w += fontkern[c] + 1;
		if(c != 0 && w <= 222) {
			PrintChar(nOffset, c, col);
		}
		nOffset += fontkern[c] + 1;
	}
}

void CheckSBook()
{
	int ii;
	char st;
	__int64 tspls;

	if(MouseX >= 331 && MouseX < 368 && MouseY >= 18 && MouseY < 314) {
		ii = (MouseY - 18) / 43;
		ii = SpellPages[sbooktab][ii];
		tspls = plr[myplr]._pMemSpells64 | plr[myplr]._pISpells64 | plr[myplr]._pAblSpells64;
		if(ii != -1 && tspls & (__int64)1 << (ii - 1)) {
			st = RSPLTYPE_SPELL;
			if(plr[myplr]._pISpells64 & (__int64)1 << (ii - 1)) {
				st = RSPLTYPE_CHARGES;
			}
			if(plr[myplr]._pAblSpells64 & (__int64)1 << (ii - 1)) {
				st = RSPLTYPE_SKILL;
			}
			plr[myplr]._pRSpell = ii;
			plr[myplr]._pRSplType = st;
			force_redraw = 255;
		}
	}
	if(MouseX >= 327 && MouseX < 633 && MouseY >= 320 && MouseY < 349) { /// BUGFIX: change `< 633` to `< 631`
		sbooktab = (MouseX - 327) / 76;
	}
}

const char *get_pieces_str(int nGold)
{
	if(nGold == 1) {
		return "piece";
	} else {
		return "pieces";
	}
}

void DrawGoldSplit(int amount)
{
	int i, sx;
	BYTE c;

	sx = 0;

	CelDecodeOnly(415, 338, (BYTE *)pGBoxBuff, 1, 261);
	sprintf(tempstr, "You have %u gold", initialDropGoldValue);
	ADD_PlrStringXY(366, 87, 600, tempstr, COL_GOLD);
	sprintf(tempstr, "%s.  How many do", get_pieces_str(initialDropGoldValue));
	ADD_PlrStringXY(366, 103, 600, tempstr, COL_GOLD);
	ADD_PlrStringXY(366, 121, 600, "you want to remove?", COL_GOLD);

	if(amount > 0) {
		sprintf(tempstr, "%u", amount);
		PrintGameStr(388, 140, tempstr, COL_WHITE);
		for(i = 0; i < tempstr[i]; i++) {
			c = gbFontTransTbl[(BYTE)tempstr[i]];
			c = fontframe[c];
			sx += fontkern[c] + 1;
		}
		sx += 452;
	} else {
		sx = 450;
	}

	CelDecodeOnly(sx, 300, (BYTE *)pSPentSpnCels, frame_4B8800, 12);
	frame_4B8800 = (frame_4B8800 & 7) + 1;
}

void control_drop_gold(char vkey)
{
	int l, l2;
	char split[6];

	if(plr[myplr]._pHitPoints >> 6 <= 0) {
		dropGoldFlag = 0;
		dropGoldValue = 0;
		return;
	}

	memset(split, 0, sizeof(split));
	_itoa(dropGoldValue, split, 10);

	if(vkey == VK_RETURN) {
		if(dropGoldValue > 0) {
			control_remove_gold(myplr, initialDropGoldIndex);
		}
		dropGoldFlag = 0;
		return;
	}
	if(vkey == VK_ESCAPE) {
		dropGoldFlag = 0;
		dropGoldValue = 0;
		return;
	}
	if(vkey == VK_BACK) {
		l = strlen(split);
		split[l - 1] = '\0';
		dropGoldValue = atoi(split);
		return;
	}
	if(vkey - '0' >= 0 && vkey - '0' <= 9) {
		if(dropGoldValue == 0 && atoi(split) > initialDropGoldValue) {
			split[0] = vkey;
		} else {
			l2 = strlen(split);
			split[l2] = vkey;
			if(atoi(split) > initialDropGoldValue || strlen(split) > strlen(split)) { /// BUGFIX: 'strlen(split) > 5'?
				return;
			}
		}
		dropGoldValue = atoi(split);
		return;
	}
}

void control_remove_gold(int pnum, int ii)
{
	int i;

	if(ii <= 46) {
		i = ii - 7;
		plr[pnum].InvList[i]._ivalue -= dropGoldValue;
		if(plr[pnum].InvList[i]._ivalue > 0) {
			SetGoldCurs(pnum, i);
		} else {
			RemoveInvItem(pnum, i);
		}
	} else {
		i = ii - 47;
		plr[pnum].SpdList[i]._ivalue -= dropGoldValue;
		if(plr[pnum].SpdList[i]._ivalue > 0) {
			SetSpdbarGoldCurs(pnum, i);
		} else {
			RemoveSpdBarItem(pnum, i);
		}
	}

	SetPlrHandItem(&plr[pnum].HoldItem, IDI_GOLD);
	GetGoldSeed(pnum, &plr[pnum].HoldItem);
	plr[pnum].HoldItem._ivalue = dropGoldValue;
	plr[pnum].HoldItem._iStatFlag = 1;
	control_set_gold_curs(pnum);
	plr[pnum]._pGold = CalculateGold(pnum);
	dropGoldValue = 0;
}

void control_set_gold_curs(int pnum)
{
	if(plr[pnum].HoldItem._ivalue >= 2500) {
		plr[pnum].HoldItem._iCurs = 6;
	} else if(plr[pnum].HoldItem._ivalue <= 1000) {
		plr[pnum].HoldItem._iCurs = 4;
	} else {
		plr[pnum].HoldItem._iCurs = 5;
	}

	SetCursor_(plr[pnum].HoldItem._iCurs + 12);
}

void DrawTalkPan()
{
	int i, x, nOffset, btn, col, nCel;
	char *s;

	if(!talkflag) {
		return;
	}

	/// ASSERT: assert(gpBuffer);

	DrawPanelBox(175, sgbPlrTalkTbl + 20, 294, 5, 175 + SCREEN_X, 356 + SCREEN_Y);
	for(i = 0; i < 10; i++) {
		x = i >> 1;
		DrawPanelBox(x + 175, sgbPlrTalkTbl + i + 25, 293 - i, 1, x + 175 + SCREEN_X, i + 361 + SCREEN_Y);
	}
	DrawPanelBox(185, sgbPlrTalkTbl + 35, 274, 30, 185 + SCREEN_X, 371 + SCREEN_Y);
	DrawPanelBox(180, sgbPlrTalkTbl + 65, 284, 5, 180 + SCREEN_X, 401 + SCREEN_Y);
	for(i = 0; i < 10; i++) {
		DrawPanelBox(180, sgbPlrTalkTbl + i + 70, i + 284, 1, 180 + SCREEN_X, i + 406 + SCREEN_Y);
	}
	DrawPanelBox(170, sgbPlrTalkTbl + 80, 310, 55, 170 + SCREEN_X, 416 + SCREEN_Y);

	s = sgszTalkMsg;
	for(i = 0; i < 39; i += 13) {
		s = control_print_talk_msg(s, 0, i, &nOffset, COL_WHITE);
		if(s == NULL) {
			break;
		}
	}
	if(s != NULL) {
		s[0] = '\0';
	}
	CelDecDatOnly(&gpBuffer[nOffset], (BYTE *)pSPentSpnCels, frame, 12);
	frame = (frame & 7) + 1;

	btn = 0;
	for(i = 0; i < MAX_PLRS; i++) {
		if(i == myplr) {
			continue;
		}
		if(byte_4B894C[i]) {
			col = COL_GOLD;
			if(talkbtndown[btn]) {
				if(btn == 0) {
					nCel = 3;
				} else {
					nCel = 4;
				}
				CelDecodeOnly(236, 18 * btn + 596, (BYTE *)pTalkBtns, nCel, 61);
			}
		} else {
			col = COL_RED;
			if(btn == 0) {
				nCel = 1;
			} else {
				nCel = 2;
			}
			if(talkbtndown[btn]) {
				nCel += 4;
			}
			CelDecodeOnly(236, 18 * btn + 596, (BYTE *)pTalkBtns, nCel, 61);
		}
		if(plr[i].plractive) {
			control_print_talk_msg(plr[i]._pName, 46, 18 * btn + 60, &nOffset, col);
		}
		btn++;
	}
}

char *control_print_talk_msg(char *msg, int x, int y, int *pnOffset, int col)
{
	int w;
	BYTE c;

	x += 264;
	w = x;
	*pnOffset = x + PitchTbl[y + 534];

	while(*msg != '\0') {
		c = gbFontTransTbl[(BYTE)*msg];
		c = fontframe[c];
		w += fontkern[c] + 1;
		if(w > 514) {
			return msg;
		}
		msg++;
		if(c != 0) {
			PrintChar(*pnOffset, c, col);
		}
		*pnOffset += fontkern[c] + 1;
	}

	return NULL;
}

BOOL control_check_talk_btn()
{
	int i;

	if(!talkflag) {
		return FALSE;
	}
	if(MouseX < 172) {
		return FALSE;
	}
	if(MouseY < 421) {
		return FALSE;
	}
	if(MouseX > 233) {
		return FALSE;
	}
	if(MouseY > 475) {
		return FALSE;
	}

	for(i = 0; i < 3; i++) {
		talkbtndown[i] = 0;
	}

	i = (MouseY - 421) / 18;
	talkbtndown[i] = 1;
	return TRUE;
}

void control_release_talk_btn()
{
	int i, y;

	if(!talkflag) {
		return;
	}

	for(i = 0; i < 3; i++) {
		talkbtndown[i] = 0;
	}

	if(MouseX < 172) {
		return;
	}
	if(MouseY < 421) {
		return;
	}
	if(MouseX > 233) {
		return;
	}
	if(MouseY > 475) {
		return;
	}

	y = (MouseY - 421) / 18;
	for(i = 0; i < 4 && y != -1; i++) {
		if(i != myplr) {
			y--;
		}
	}
	if(i <= 4) {
		byte_4B894C[i - 1] = !byte_4B894C[i - 1];
	}
}

void control_reset_talk_msg(char *msg)
{
	int i, msk;

	msk = 0;
	for(i = 0; i < 4; i++) {
		if(byte_4B894C[i]) {
			msk |= 1 << i;
		}
	}

	if(!msgcmd_add_server_cmd_W(sgszTalkMsg)) { /// BUGFIX: supposed to use unused arg 'msg' here?
		NetSendCmdString(msk, sgszTalkMsg);
	}
}

void control_type_message()
{
	int i;

	if(gbMaxPlayers == 1) {
		return;
	}

	talkflag = 1;
	sgbPlrTalkTbl = 144;
	sgszTalkMsg[0] = '\0';
	frame = 1;

	for(i = 0; i < 3; i++) {
		talkbtndown[i] = 0;
	}

	force_redraw = 255;
	sgbTalkSavePos = sgbNextTalkSave;
}

void control_reset_talk()
{
	talkflag = 0;
	sgbPlrTalkTbl = 0;
	force_redraw = 255;
}

BOOL control_talk_last_key(int vkey)
{
	int l;

	if(gbMaxPlayers == 1) {
		return FALSE;
	}
	if(!talkflag) {
		return FALSE;
	}
	if((DWORD)vkey < VK_SPACE) {
		return FALSE;
	}

	l = strlen(sgszTalkMsg);
	if(l < 78) {
		sgszTalkMsg[l] = vkey;
		sgszTalkMsg[l + 1] = '\0';
	}

	return TRUE;
}

BOOL control_presskeys(int vkey)
{
	int l;

	if(gbMaxPlayers == 1) {
		return FALSE;
	}
	if(!talkflag) {
		return FALSE;
	}
	if(vkey == VK_SPACE) {
		return TRUE;
	}

	if(vkey == VK_ESCAPE) {
		control_reset_talk();
	} else if(vkey == VK_RETURN) {
		control_press_enter();
	} else if(vkey == VK_BACK) {
		l = strlen(sgszTalkMsg);
		if(l > 0) {
			sgszTalkMsg[l - 1] = '\0';
		}
	} else if(vkey == VK_DOWN) {
		control_up_down(1);
	} else if(vkey == VK_UP) {
		control_up_down(-1);
	} else {
		return FALSE;
	}

	return TRUE;
}

void control_press_enter()
{
	int i;
	BYTE save;

	if(sgszTalkMsg[0] != '\0') {
		control_reset_talk_msg(sgszTalkMsg);
		for(i = 0; i < 8; i++) {
			if(strcmp(sgszTalkSave[i], sgszTalkMsg) == 0) {
				break;
			}
		}
		if(i >= 8) {
			strcpy(sgszTalkSave[sgbNextTalkSave], sgszTalkMsg);
			sgbNextTalkSave = (sgbNextTalkSave + 1) & 7;
		} else {
			save = (sgbNextTalkSave - 1) & 7;
			if(i != save) {
				strcpy(sgszTalkSave[i], sgszTalkSave[save]);
				strcpy(sgszTalkSave[save], sgszTalkMsg);
			}
		}
		sgbTalkSavePos = sgbNextTalkSave;
		sgszTalkMsg[0] = '\0';
	}

	control_reset_talk();
}

void control_up_down(int v)
{
	int i;

	for(i = 0; i < 8; i++) {
		sgbTalkSavePos = (sgbTalkSavePos + v) & 7;
		if(sgszTalkSave[sgbTalkSavePos][0] != '\0') {
			strcpy(sgszTalkMsg, sgszTalkSave[sgbTalkSavePos]);
			return;
		}
	}
}
