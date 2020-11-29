#include "diablo.h"

int stextup; // weak
int storenumh; // weak
int stextlhold; // weak
ItemStruct boyitem;
int stextshold; // idb
ItemStruct premiumitem[6];
void *pSTextBoxCels;
int premiumlevel; // idb
int talker; // weak
STextStruct stext[24];
char stextsize; // weak
int stextsmax; // weak
int sspinframe; // idb
ItemStruct storehold[48];
int gossipstart; // weak
ItemStruct witchitem[20];
BOOL stextscrl; // weak
int numpremium; // idb
ItemStruct healitem[20];
ItemStruct golditem;
char storehidx[48];
void *pSTextSlidCels;
int stextvhold; // weak
int stextsel; // weak
char stextscrldbtn; // weak
int gossipend; // weak
void *pSPentSpnCels;
int stextsval; // idb
int boylevel; // weak
ItemStruct smithitem[20];
int stextdown; // weak
char stextscrlubtn; // weak
char stextflag; // weak

int SStringY[24] =
{
  0,
  12,
  24,
  36,
  48,
  60,
  72,
  84,
  96,
  108,
  120,
  132,
  144,
  156,
  168,
  180,
  192,
  204,
  216,
  228,
  240,
  252,
  264,
  276
};
char *talkname[9] =
{
  "Griswold",
  "Pepin",
  "",
  "Ogden",
  "Cain",
  "Farnham",
  "Adria",
  "Gillian",
  "Wirt"
};

void InitStores()
{
	int i;

	/// ASSERT: assert(! pSTextBoxCels);
	pSTextBoxCels = DiabLoad("Data\\TextBox2.CEL", NULL, 'STOR');
	pSPentSpnCels = DiabLoad("Data\\PentSpn2.CEL", NULL, 'STOR');
	pSTextSlidCels = DiabLoad("Data\\TextSlid.CEL", NULL, 'STOR');

	ClearSText(0, 24);

	stextflag = 0;
	sspinframe = 1;
	stextsize = 0;
	stextscrl = FALSE;
	numpremium = 0;
	premiumlevel = 1;

	for(i = 0; i < 6; i++) {
		premiumitem[i]._itype = -1;
	}

	boyitem._itype = -1;
	boylevel = 0;
}

void SetupTownStores()
{
	int i, l;

	SetRndSeed(glSeedTbl[currlevel] * GetTickCount());

	if(gbMaxPlayers == 1) {
		l = 0;
		for(i = 0; i < NUMLEVELS; i++) {
			if(plr[myplr]._pLvlVisited[i]) {
				l = i;
			}
		}
	} else {
		l = plr[myplr]._pLevel >> 1;
	}

	l += 2;
	if(l < 6) {
		l = 6;
	}
	if(l > 16) {
		l = 16;
	}

	SpawnStoreGold();
	SpawnSmith(l);
	SpawnWitch(l);
	SpawnHealer(l);
	SpawnBoy(plr[myplr]._pLevel);
	SpawnPremium(plr[myplr]._pLevel);
}

void FreeStoreMem()
{
	MemFreeDbg(pSTextBoxCels);
	MemFreeDbg(pSPentSpnCels);
	MemFreeDbg(pSTextSlidCels);
}

void DrawSTextBack()
{
	CelDecodeOnly(408, 487, (BYTE *)pSTextBoxCels, 1, 271);

	/// ASSERT: assert(gpBuffer);

#ifdef USE_ASM
	__asm {
		mov		edi, gpBuffer
		add		edi, SCREENXY(347, 324)
		xor		eax, eax
		mov		edx, 297 / 2
	label1:
		mov		ecx, 265 / 2
	label2:
		stosb
		inc		edi
		loop	label2
		stosb
		sub		edi, 768 + 265
		mov		ecx, 265 / 2
	label3:
		inc		edi
		stosb
		loop	label3
		sub		edi, 768 + (265 - 1)
		dec		edx
		jnz		label1
		mov		ecx, 265 / 2
	label4:
		stosb
		inc		edi
		loop	label4
		stosb
	}
#else
	int wdt, hgt;
	BYTE *dst;

	dst = &gpBuffer[SCREENXY(347, 324)];

	for(hgt = 297 / 2; hgt != 0; hgt--) {
		for(wdt = 265 / 2; wdt != 0; wdt--) {
			dst[0] = 0;
			dst += 2;
		}
		*dst = 0;
		dst -= 768 + (265 - 1);
		for(wdt = 265 / 2; wdt != 0; wdt--) {
			dst[1] = 0;
			dst += 2;
		}
		dst -= 768 + (265 - 1);
	}
	for(wdt = 265 / 2; wdt != 0; wdt--) {
		dst[0] = 0;
		dst += 2;
	}
	*dst = 0;
#endif
}

void PrintSString(int x, int y, BOOL cjustflag, char *str, char col, int val)
{
	int i, l, xx, yy, w, width, No, justw, sx;
	BYTE c;
	char valstr[32];

	yy = SStringY[y] + stext[y]._syoff;

	if(stextsize == 0) {
		xx = 416;
	} else {
		xx = 96;
	}

	No = x + xx + PitchTbl[yy + 204];
	l = strlen(str);

	if(stextsize == 0) {
		width = 257;
	} else {
		width = 577;
	}

	w = 0;
	if(cjustflag) {
		justw = 0;
		for(i = 0; i < l; i++) {
			c = gbFontTransTbl[(BYTE)str[i]];
			c = fontframe[c];
			justw += fontkern[c] + 1;
		}
		if(justw < width) {
			w = (width - justw) >> 1;
		}
		No += w;
	}
	if(stextsel == y) {
		if(cjustflag) {
			sx = xx + w + x - 20;
		} else {
			sx = xx + x - 20;
		}
		CelDecodeOnly(sx, yy + 205, (BYTE *)pSPentSpnCels, sspinframe, 12);
	}

	for(i = 0; i < l; i++) {
		c = gbFontTransTbl[(BYTE)str[i]];
		c = fontframe[c];
		w += fontkern[c] + 1;
		if(c != 0 && w <= width) {
			PrintChar(No, c, col);
		}
		No += fontkern[c] + 1;
	}

	if(!cjustflag && val >= 0) {
		sprintf(valstr, "%i", val);
		No = 656 - x + PitchTbl[yy + 204];
		l = strlen(valstr);
		for(i = l - 1; i >= 0; i--) {
			c = gbFontTransTbl[(BYTE)valstr[i]];
			c = fontframe[c];
			No -= fontkern[c] + 1;
			if(c != 0) {
				PrintChar(No, c, col);
			}
		}
	}
	if(stextsel == y) {
		if(cjustflag) {
			sx = xx + w + x + 4;
		} else {
			sx = 660 - x;
		}
		CelDecodeOnly(sx, yy + 205, (BYTE *)pSPentSpnCels, sspinframe, 12);
	}
}

void DrawSLine(int y)
{
	int yy, nSrcOff, nDstOff, width, line;

	yy = SStringY[y];

	if(stextsize == 1) {
		nSrcOff = SCREENXY(26, 25);
		nDstOff = 26 + 64 + PitchTbl[yy + 198];
		width = 586 / 4;
		line = 768 - 586;
	} else {
		nSrcOff = SCREENXY(346, 25);
		nDstOff = 346 + 64 + PitchTbl[yy + 198];
		width = 266 / 4;
		line = 768 - 266;
	}

	/// ASSERT: assert(gpBuffer);

#ifdef USE_ASM
	__asm {
		mov		esi, gpBuffer
		mov		edi, esi
		add		esi, nSrcOff
		add		edi, nDstOff
		mov		ebx, line
		mov		edx, 3
	copyline:
		mov		ecx, width
		rep movsd
		movsw
		add		esi, ebx
		add		edi, ebx
		dec		edx
		jnz		copyline
	}
#else
	int i;
	BYTE *src, *dst;

	src = &gpBuffer[nSrcOff];
	dst = &gpBuffer[nDstOff];

	for(i = 0; i < 3; i++, src += 768, dst += 768) {
		memcpy(dst, src, 768 - line);
	}
#endif
}

void DrawSSlider(int y1, int y2)
{
	int uy, dy, i, base_y, bar_y;

	uy = SStringY[y1] + 204;
	dy = SStringY[y2] + 204;

	if(stextscrlubtn != -1) {
		CelDecodeOnly(665, uy, (BYTE *)pSTextSlidCels, 12, 12);
	} else {
		CelDecodeOnly(665, uy, (BYTE *)pSTextSlidCels, 10, 12);
	}
	if(stextscrldbtn != -1) {
		CelDecodeOnly(665, dy, (BYTE *)pSTextSlidCels, 11, 12);
	} else {
		CelDecodeOnly(665, dy, (BYTE *)pSTextSlidCels, 9, 12);
	}

	for(i = uy + 12; i < dy; i += 12) {
		CelDecodeOnly(665, i, (BYTE *)pSTextSlidCels, 14, 12);
	}

	if(stextsel != 22) {
		base_y = stextsel;
	} else {
		base_y = stextlhold;
	}
	if(storenumh > 1) {
		bar_y = stextsval + ((base_y - stextup) >> 2);
		bar_y *= 1000;
		bar_y /= storenumh - 1;
		bar_y *= SStringY[y2] - SStringY[y1] - 24;
		bar_y /= 1000;
	} else {
		bar_y = 0;
	}
	bar_y += SStringY[y1 + 1] + 204;
	CelDecodeOnly(665, bar_y, (BYTE *)pSTextSlidCels, 13, 12);
}

void DrawSTextHelp()
{
	stextsize = 1;
	stextsel = -1;
}

void ClearSText(int s, int e)
{
	int i;

	for(i = s; i < e; i++) {
		stext[i]._sx = 0;
		stext[i]._syoff = 0;
		stext[i]._sstr[0] = '\0';
		stext[i]._sjust = 0;
		stext[i]._sclr = 0;
		stext[i]._sline = 0;
		stext[i]._ssel = FALSE;
		stext[i]._sval = -1;
	}
}

void AddSLine(int y)
{
	stext[y]._sx = 0;
	stext[y]._syoff = 0;
	stext[y]._sstr[0] = '\0';
	stext[y]._sline = 1;
}

void AddSTextVal(int y, int val)
{
	stext[y]._sval = val;
}

void OffsetSTextY(int y, int yo)
{
	stext[y]._syoff = yo;
}

void AddSText(int x, int y, BOOL j, char *str, char clr, BOOL sel)
{
	stext[y]._sx = x;
	stext[y]._syoff = 0;
	strcpy(stext[y]._sstr, str);
	stext[y]._sjust = j;
	stext[y]._sclr = clr;
	stext[y]._sline = 0;
	stext[y]._ssel = sel;
}

void StoreAutoPlace()
{
	int i, w, h, idx;
	BOOL done;

	SetICursor(plr[myplr].HoldItem._iCurs + 12);

	done = FALSE;
	w = icursW28;
	h = icursH28;
	if(w == 1 && h == 1) {
		idx = plr[myplr].HoldItem.IDidx;
		if(plr[myplr].HoldItem._iStatFlag && AllItemsList[idx].iUsable) {
			for(i = 0; i < 8 && !done; i++) {
				if(plr[myplr].SpdList[i]._itype == -1) {
					plr[myplr].SpdList[i] = plr[myplr].HoldItem;
					done = TRUE;
				}
			}
		}
		for(i = 30; i <= 39 && !done; i++) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
		for(i = 20; i <= 29 && !done; i++) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
		for(i = 10; i <= 19 && !done; i++) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
		for(i = 0; i <= 9 && !done; i++) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
	}
	if(w == 1 && h == 2) {
		for(i = 29; i >= 20 && !done; i--) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
		for(i = 9; i >= 0 && !done; i--) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
		for(i = 19; i >= 10 && !done; i--) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
	}
	if(w == 1 && h == 3) {
		for(i = 0; i < 20 && !done; i++) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
	}
	if(w == 2 && h == 2) {
		for(i = 0; i < 10 && !done; i++) {
			done = AutoPlace(myplr, AP2x2Tbl[i], w, h, TRUE);
		}
		for(i = 21; i < 29 && !done; i += 2) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
		for(i = 1; i < 9 && !done; i += 2) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
		for(i = 10; i < 19 && !done; i++) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
	}
	if(w == 2 && h == 3) {
		for(i = 0; i < 9 && !done; i++) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
		for(i = 10; i < 19 && !done; i++) {
			done = AutoPlace(myplr, i, w, h, TRUE);
		}
	}
}

void S_StartSmith()
{
	stextsize = 0;
	stextscrl = FALSE;

	AddSText(0, 1, TRUE, "Welcome to the", COL_GOLD, FALSE);
	AddSText(0, 3, TRUE, "Blacksmith's shop", COL_GOLD, FALSE);
	AddSText(0, 7, TRUE, "Would you like to:", COL_GOLD, FALSE);
	AddSText(0, 10, TRUE, "Talk to Griswold", COL_BLUE, TRUE);
	AddSText(0, 12, TRUE, "Buy basic items", COL_WHITE, TRUE);
	AddSText(0, 14, TRUE, "Buy premium items", COL_WHITE, TRUE);
	AddSText(0, 16, TRUE, "Sell items", COL_WHITE, TRUE);
	AddSText(0, 18, TRUE, "Repair items", COL_WHITE, TRUE);
	AddSText(0, 20, TRUE, "Leave the shop", COL_WHITE, TRUE);
	AddSLine(5);

	storenumh = 20;
}

void S_ScrollSBuy(int idx)
{
	int l, ls;
	char iclr;

	ClearSText(5, 21);

	stextup = 5;

	for(l = 5; l < 20; l += 4) {
		if(smithitem[idx]._itype != -1) {
			ls = l;
			iclr = COL_WHITE;
			if(smithitem[idx]._iMagical != 0) {
				iclr = COL_BLUE;
			}
			if(!smithitem[idx]._iStatFlag) {
				iclr = COL_RED;
			}
			if(smithitem[idx]._iMagical != 0) {
				AddSText(20, l, FALSE, smithitem[idx]._iIName, iclr, TRUE);
			} else {
				AddSText(20, l, FALSE, smithitem[idx]._iName, iclr, TRUE);
			}
			AddSTextVal(l, smithitem[idx]._iIvalue);
			PrintStoreItem(&smithitem[idx], l + 1, iclr);
			stextdown = ls;
			idx++;
		}
	}

	if(!stext[stextsel]._ssel && stextsel != 22) {
		stextsel = stextdown;
	}
}

void PrintStoreItem(const ItemStruct *x, int l, char iclr)
{
	char sstr[128];

	sstr[0] = '\0';
	if(x->_iIdentified) {
		if(x->_iMagical != 2 && x->_iPrePower != -1) {
			PrintItemPower(x->_iPrePower, x);
			strcat(sstr, tempstr);
		}
		if(x->_iSufPower != -1) {
			PrintItemPower(x->_iSufPower, x);
			if(sstr[0] != '\0') {
				strcat(sstr, ",  ");
			}
			strcat(sstr, tempstr);
		}
	}
	if(x->_iMiscId == IMISC_STAFF && x->_iMaxCharges != 0) {
		sprintf(tempstr, "Charges: %i/%i", x->_iCharges, x->_iMaxCharges);
		if(sstr[0] != '\0') {
			strcat(sstr, ",  ");
		}
		strcat(sstr, tempstr);
	}
	if(sstr[0] != '\0') {
		AddSText(40, l, FALSE, sstr, iclr, FALSE);
		l++;
	}

	sstr[0] = '\0';
	if(x->_iClass == ICLASS_WEAPON) {
		sprintf(sstr, "Damage: %i-%i  ", x->_iMinDam, x->_iMaxDam);
	}
	if(x->_iClass == ICLASS_ARMOR) {
		sprintf(sstr, "Armor: %i  ", x->_iAC);
	}
	if(x->_iMaxDur == 255 || x->_iMaxDur == 0) {
		strcat(sstr, "Indestructible,  ");
	} else {
		sprintf(tempstr, "Dur: %i/%i,  ", x->_iDurability, x->_iMaxDur);
		strcat(sstr, tempstr);
	}
	if(x->_itype == ITYPE_MISC) {
		sstr[0] = '\0';
	}
	if(!(x->_iMinStr + x->_iMinMag + x->_iMinDex)) {
		strcat(sstr, "No required attributes");
	} else {
		strcpy(tempstr, "Required:");
		if(x->_iMinStr != 0) {
			sprintf(tempstr, "%s %i Str", tempstr, x->_iMinStr);
		}
		if(x->_iMinMag != 0) {
			sprintf(tempstr, "%s %i Mag", tempstr, x->_iMinMag);
		}
		if(x->_iMinDex != 0) {
			sprintf(tempstr, "%s %i Dex", tempstr, x->_iMinDex);
		}
		strcat(sstr, tempstr);
	}
	AddSText(40, l, FALSE, sstr, iclr, FALSE);
	l++;

	if(x->_iMagical == 2 && x->_iIdentified) {
		AddSText(40, l, FALSE, "Unique Item", iclr, FALSE);
	}
}

void S_StartSBuy()
{
	int i;

	stextsize = 1;
	stextscrl = TRUE;
	stextsval = 0;

	sprintf(tempstr, "I have these items for sale :           Your gold : %i", plr[myplr]._pGold);
	AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
	AddSLine(3);
	AddSLine(21);
	S_ScrollSBuy(stextsval);
	AddSText(0, 22, TRUE, "Back", COL_WHITE, FALSE);
	OffsetSTextY(22, 6);

	storenumh = 0;
	for(i = 0; smithitem[i]._itype != -1; i++) {
		storenumh++;
	}

	stextsmax = storenumh - 4;
	if(stextsmax < 0) {
		stextsmax = 0;
	}
}

void S_ScrollSPBuy(int idx)
{
	int l, ls, boughtitems;
	char iclr;

	ClearSText(5, 21);

	stextup = 5;

	boughtitems = idx;
	idx = 0;
	while(boughtitems != 0) {
		if(premiumitem[idx]._itype != -1) {
			boughtitems--;
		}
		idx++;
	}

	for(l = 5; l < 20 && idx < 6; l += 4) {
		if(premiumitem[idx]._itype != -1) {
			ls = l;
			iclr = COL_WHITE;
			if(premiumitem[idx]._iMagical != 0) {
				iclr = COL_BLUE;
			}
			if(!premiumitem[idx]._iStatFlag) {
				iclr = COL_RED;
			}
			AddSText(20, l, FALSE, premiumitem[idx]._iIName, iclr, TRUE);
			AddSTextVal(l, premiumitem[idx]._iIvalue);
			PrintStoreItem(&premiumitem[idx], l + 1, iclr);
			stextdown = ls;
		} else if(idx < 6) {
			l -= 4;
		}
		idx++;
	}

	if(!stext[stextsel]._ssel && stextsel != 22) {
		stextsel = stextdown;
	}
}

BOOL S_StartSPBuy()
{
	int i;

	storenumh = 0;
	for(i = 0; i < 6; i++) {
		if(premiumitem[i]._itype != -1) {
			storenumh++;
		}
	}

	if(storenumh == 0) {
		StartStore(STORE_SMITH);
		stextsel = 14;
		return FALSE;
	}

	stextsize = 1;
	stextscrl = TRUE;
	stextsval = 0;

	sprintf(tempstr, "I have these premium items for sale :   Your gold : %i", plr[myplr]._pGold);
	AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
	AddSLine(3);
	AddSLine(21);
	AddSText(0, 22, TRUE, "Back", COL_WHITE, FALSE);
	OffsetSTextY(22, 6);

	stextsmax = storenumh - 4;
	if(stextsmax < 0) {
		stextsmax = 0;
	}

	S_ScrollSPBuy(stextsval);
	return TRUE;
}

BOOL SmithSellOk(int i)
{
	if(plr[myplr].InvList[i]._itype == -1) {
		return FALSE;
	}
	if(plr[myplr].InvList[i]._itype == ITYPE_MISC) {
		return FALSE;
	}
	if(plr[myplr].InvList[i]._itype == ITYPE_GOLD) {
		return FALSE;
	}
	if(plr[myplr].InvList[i]._itype == ITYPE_MEAT) {
		return FALSE;
	}
	if(plr[myplr].InvList[i]._itype == ITYPE_STAFF) {
		return FALSE;
	}
	if(plr[myplr].InvList[i].IDidx == IDI_LAZSTAFF) {
		return FALSE;
	}

	return TRUE;
}

void S_ScrollSSell(int idx)
{
	int l, ls, v;
	char iclr;

	ClearSText(5, 21);

	stextup = 5;

	for(l = 5; l < 20 && idx < storenumh; l += 4) {
		if(storehold[idx]._itype != -1) {
			ls = l;
			iclr = COL_WHITE;
			if(storehold[idx]._iMagical != 0) {
				iclr = COL_BLUE;
			}
			if(!storehold[idx]._iStatFlag) {
				iclr = COL_RED;
			}
			if(storehold[idx]._iMagical != 0 && storehold[idx]._iIdentified) {
				AddSText(20, l, FALSE, storehold[idx]._iIName, iclr, TRUE);
				v = storehold[idx]._iIvalue;
			} else {
				AddSText(20, l, FALSE, storehold[idx]._iName, iclr, TRUE);
				v = storehold[idx]._ivalue;
			}
			AddSTextVal(l, v);
			PrintStoreItem(&storehold[idx], l + 1, iclr);
			stextdown = ls;
		}
		idx++;
	}

	stextsmax = storenumh - 4;
	if(stextsmax < 0) {
		stextsmax = 0;
	}
}

void S_StartSSell()
{
	int i;
	BOOL sellok;

	stextsize = 1;
	sellok = FALSE;
	storenumh = 0;

	for(i = 0; i < 48; i++) {
		storehold[i]._itype = -1;
	}

	for(i = 0; i < plr[myplr]._pNumInv; i++) {
		if(SmithSellOk(i)) {
			sellok = TRUE;
			storehold[storenumh] = plr[myplr].InvList[i];
			if(storehold[storenumh]._iMagical != 0 && storehold[storenumh]._iIdentified) {
				storehold[storenumh]._ivalue = storehold[storenumh]._iIvalue;
			}
			storehold[storenumh]._ivalue >>= 2;
			if(storehold[storenumh]._ivalue == 0) {
				storehold[storenumh]._ivalue = 1;
			}
			storehold[storenumh]._iIvalue = storehold[storenumh]._ivalue;
			storehidx[storenumh] = i;
			storenumh++;
		}
	}

	if(!sellok) {
		stextscrl = FALSE;
		sprintf(tempstr, "You have nothing I want.            Your gold : %i", plr[myplr]._pGold);
		AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
		AddSLine(3);
		AddSLine(21);
		AddSText(0, 22, TRUE, "Back", COL_WHITE, TRUE);
		OffsetSTextY(22, 6);
	} else {
		stextscrl = TRUE;
		stextsval = 0;
		stextsmax = plr[myplr]._pNumInv;
		sprintf(tempstr, "Which item is for sale?            Your gold : %i", plr[myplr]._pGold);
		AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
		AddSLine(3);
		AddSLine(21);
		S_ScrollSSell(stextsval);
		AddSText(0, 22, TRUE, "Back", COL_WHITE, TRUE);
		OffsetSTextY(22, 6);
	}
}

BOOL SmithRepairOk(int i)
{
	if(plr[myplr].InvList[i]._itype == -1) {
		return FALSE;
	}
	if(plr[myplr].InvList[i]._itype == ITYPE_MISC) {
		return FALSE;
	}
	if(plr[myplr].InvList[i]._itype == ITYPE_GOLD) {
		return FALSE;
	}
	if(plr[myplr].InvList[i]._itype == ITYPE_MEAT) {
		return FALSE;
	}
	if(plr[myplr].InvList[i]._iDurability == plr[myplr].InvList[i]._iMaxDur) {
		return FALSE;
	}

	return TRUE;
}

void S_StartSRepair()
{
	int i;
	BOOL repairok;

	stextsize = 1;
	repairok = FALSE;
	storenumh = 0;

	for(i = 0; i < 48; i++) {
		storehold[i]._itype = -1;
	}

	if(plr[myplr].InvBody[0]._itype != -1 && plr[myplr].InvBody[0]._iDurability != plr[myplr].InvBody[0]._iMaxDur) {
		repairok = TRUE;
		AddStoreHoldRepair(&plr[myplr].InvBody[0], -1);
	}
	if(plr[myplr].InvBody[6]._itype != -1 && plr[myplr].InvBody[6]._iDurability != plr[myplr].InvBody[6]._iMaxDur) {
		repairok = TRUE;
		AddStoreHoldRepair(&plr[myplr].InvBody[6], -2);
	}
	if(plr[myplr].InvBody[4]._itype != -1 && plr[myplr].InvBody[4]._iDurability != plr[myplr].InvBody[4]._iMaxDur) {
		repairok = TRUE;
		AddStoreHoldRepair(&plr[myplr].InvBody[4], -3);
	}
	if(plr[myplr].InvBody[5]._itype != -1 && plr[myplr].InvBody[5]._iDurability != plr[myplr].InvBody[5]._iMaxDur) {
		repairok = TRUE;
		AddStoreHoldRepair(&plr[myplr].InvBody[5], -4);
	}

	for(i = 0; i < plr[myplr]._pNumInv; i++) {
		if(SmithRepairOk(i)) {
			repairok = TRUE;
			AddStoreHoldRepair(&plr[myplr].InvList[i], i);
		}
	}

	if(!repairok) {
		stextscrl = FALSE;
		sprintf(tempstr, "You have nothing to repair.            Your gold : %i", plr[myplr]._pGold);
		AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
		AddSLine(3);
		AddSLine(21);
		AddSText(0, 22, TRUE, "Back", COL_WHITE, TRUE);
		OffsetSTextY(22, 6);
	} else {
		stextscrl = TRUE;
		stextsval = 0;
		stextsmax = plr[myplr]._pNumInv;
		sprintf(tempstr, "Repair which item?            Your gold : %i", plr[myplr]._pGold);
		AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
		AddSLine(3);
		AddSLine(21);
		S_ScrollSSell(stextsval);
		AddSText(0, 22, TRUE, "Back", COL_WHITE, TRUE);
		OffsetSTextY(22, 6);
	}
}

void AddStoreHoldRepair(ItemStruct *itm, int i)
{
	int v;

	/// ASSERT: assert(itm->_iMaxDur > 0);

	storehold[storenumh] = *itm;
	itm = &storehold[storenumh];

	if(itm->_iMagical != 0 && itm->_iIdentified) {
		itm->_ivalue = 30 * itm->_iIvalue / 100;
	}

	v = 100 * (itm->_iMaxDur - itm->_iDurability) / itm->_iMaxDur;
	v = v * itm->_ivalue / 100;

	if(v == 0) {
		if(itm->_iMagical != 0 && itm->_iIdentified) {
			return;
		}
		v = 1;
	}
	if(v > 1) {
		v >>= 1;
	}

	itm->_iIvalue = v;
	itm->_ivalue = v;
	storehidx[storenumh] = i;
	storenumh++;
}

void S_StartWitch()
{
	stextsize = 0;
	stextscrl = FALSE;

	AddSText(0, 2, TRUE, "Witch's shack", COL_GOLD, FALSE);
	AddSText(0, 9, TRUE, "Would you like to:", COL_GOLD, FALSE);
	AddSText(0, 12, TRUE, "Talk to Adria", COL_BLUE, TRUE);
	AddSText(0, 14, TRUE, "Buy items", COL_WHITE, TRUE);
	AddSText(0, 16, TRUE, "Sell items", COL_WHITE, TRUE);
	AddSText(0, 18, TRUE, "Recharge staves", COL_WHITE, TRUE);
	AddSText(0, 20, TRUE, "Leave the shack", COL_WHITE, TRUE);
	AddSLine(5);

	storenumh = 20;
}

void S_ScrollWBuy(int idx)
{
	int l, ls;
	char iclr;

	ClearSText(5, 21);

	stextup = 5;

	for(l = 5; l < 20; l += 4) {
		if(witchitem[idx]._itype != -1) {
			ls = l;
			iclr = COL_WHITE;
			if(witchitem[idx]._iMagical != 0) {
				iclr = COL_BLUE;
			}
			if(!witchitem[idx]._iStatFlag) {
				iclr = COL_RED;
			}
			if(witchitem[idx]._iMagical != 0) {
				AddSText(20, l, FALSE, witchitem[idx]._iIName, iclr, TRUE);
			} else {
				AddSText(20, l, FALSE, witchitem[idx]._iName, iclr, TRUE);
			}
			AddSTextVal(l, witchitem[idx]._iIvalue);
			PrintStoreItem(&witchitem[idx], l + 1, iclr);
			stextdown = ls;
			idx++;
		}
	}

	if(!stext[stextsel]._ssel && stextsel != 22) {
		stextsel = stextdown;
	}
}

void S_StartWBuy()
{
	int i;

	stextsize = 1;
	stextscrl = TRUE;
	stextsval = 0;
	stextsmax = 20;

	sprintf(tempstr, "I have these items for sale :           Your gold : %i", plr[myplr]._pGold);
	AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
	AddSLine(3);
	AddSLine(21);
	S_ScrollWBuy(stextsval);
	AddSText(0, 22, TRUE, "Back", COL_WHITE, FALSE);
	OffsetSTextY(22, 6);

	storenumh = 0;
	for(i = 0; witchitem[i]._itype != -1; i++) {
		storenumh++;
	}

	stextsmax = storenumh - 4;
	if(stextsmax < 0) {
		stextsmax = 0;
	}
}

BOOL WitchSellOk(int i)
{
	BOOL rv;
	ItemStruct *pI;

	rv = FALSE;

	if(i >= 0) {
		pI = &plr[myplr].InvList[i];
	} else {
		pI = &plr[myplr].SpdList[-(i + 1)];
	}

	if(pI->_itype == ITYPE_MISC) {
		rv = TRUE;
	}
	if(pI->_itype == ITYPE_STAFF) {
		rv = TRUE;
	}
	if(pI->IDidx >= IDI_FIRSTQUEST && pI->IDidx <= IDI_LASTQUEST) {
		rv = FALSE;
	}
	if(pI->IDidx == IDI_LAZSTAFF) {
		rv = FALSE;
	}

	return rv;
}

void S_StartWSell()
{
	int i;
	BOOL sellok;

	stextsize = 1;
	sellok = FALSE;
	storenumh = 0;

	for(i = 0; i < 48; i++) {
		storehold[i]._itype = -1;
	}

	for(i = 0; i < plr[myplr]._pNumInv; i++) {
		if(WitchSellOk(i)) {
			sellok = TRUE;
			storehold[storenumh] = plr[myplr].InvList[i];
			if(storehold[storenumh]._iMagical != 0 && storehold[storenumh]._iIdentified) {
				storehold[storenumh]._ivalue = storehold[storenumh]._iIvalue;
			}
			storehold[storenumh]._ivalue >>= 2;
			if(storehold[storenumh]._ivalue == 0) {
				storehold[storenumh]._ivalue = 1;
			}
			storehold[storenumh]._iIvalue = storehold[storenumh]._ivalue;
			storehidx[storenumh] = i;
			storenumh++;
		}
	}
	for(i = 0; i < 8; i++) {
		if(plr[myplr].SpdList[i]._itype != -1 && WitchSellOk(-(i + 1))) {
			sellok = TRUE;
			storehold[storenumh] = plr[myplr].SpdList[i];
			if(storehold[storenumh]._iMagical != 0 && storehold[storenumh]._iIdentified) {
				storehold[storenumh]._ivalue = storehold[storenumh]._iIvalue;
			}
			storehold[storenumh]._ivalue >>= 2;
			if(storehold[storenumh]._ivalue == 0) {
				storehold[storenumh]._ivalue = 1;
			}
			storehold[storenumh]._iIvalue = storehold[storenumh]._ivalue;
			storehidx[storenumh] = -(i + 1);
			storenumh++;
		}
	}

	if(!sellok) {
		stextscrl = FALSE;
		sprintf(tempstr, "You have nothing I want.            Your gold : %i", plr[myplr]._pGold);
		AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
		AddSLine(3);
		AddSLine(21);
		AddSText(0, 22, TRUE, "Back", COL_WHITE, TRUE);
		OffsetSTextY(22, 6);
	} else {
		stextscrl = TRUE;
		stextsval = 0;
		stextsmax = plr[myplr]._pNumInv;
		sprintf(tempstr, "Which item is for sale?            Your gold : %i", plr[myplr]._pGold);
		AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
		AddSLine(3);
		AddSLine(21);
		S_ScrollSSell(stextsval);
		AddSText(0, 22, TRUE, "Back", COL_WHITE, TRUE);
		OffsetSTextY(22, 6);
	}
}

BOOL WitchRechargeOk(int i)
{
	BOOL rv;

	rv = FALSE;

	if(plr[myplr].InvList[i]._itype == ITYPE_STAFF && plr[myplr].InvList[i]._iCharges != plr[myplr].InvList[i]._iMaxCharges) {
		rv = TRUE;
	}

	return rv;
}

void AddStoreHoldRecharge(ItemStruct itm, int i)
{
	int v;

	storehold[storenumh] = itm;
	storehold[storenumh]._ivalue += spelldata[itm._iSpell].sStaffCost;
	v = 100 * (storehold[storenumh]._iMaxCharges - storehold[storenumh]._iCharges) / storehold[storenumh]._iMaxCharges;
	storehold[storenumh]._ivalue = v * storehold[storenumh]._ivalue / 100 >> 1;
	storehold[storenumh]._iIvalue = storehold[storenumh]._ivalue;
	storehidx[storenumh] = i;
	storenumh++;
}

void S_StartWRecharge()
{
	int i;
	BOOL rechargeok;

	stextsize = 1;
	rechargeok = FALSE;
	storenumh = 0;

	for(i = 0; i < 48; i++) {
		storehold[i]._itype = -1;
	}

	if(plr[myplr].InvBody[4]._itype == ITYPE_STAFF && plr[myplr].InvBody[4]._iCharges != plr[myplr].InvBody[4]._iMaxCharges) {
		rechargeok = TRUE;
		AddStoreHoldRecharge(plr[myplr].InvBody[4], -1);
	}

	for(i = 0; i < plr[myplr]._pNumInv; i++) {
		if(WitchRechargeOk(i)) {
			rechargeok = TRUE;
			AddStoreHoldRecharge(plr[myplr].InvList[i], i);
		}
	}

	if(!rechargeok) {
		stextscrl = FALSE;
		sprintf(tempstr, "You have nothing to recharge.            Your gold : %i", plr[myplr]._pGold);
		AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
		AddSLine(3);
		AddSLine(21);
		AddSText(0, 22, TRUE, "Back", COL_WHITE, TRUE);
		OffsetSTextY(22, 6);
	} else {
		stextscrl = TRUE;
		stextsval = 0;
		stextsmax = plr[myplr]._pNumInv;
		sprintf(tempstr, "Recharge which item?            Your gold : %i", plr[myplr]._pGold);
		AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
		AddSLine(3);
		AddSLine(21);
		S_ScrollSSell(stextsval);
		AddSText(0, 22, TRUE, "Back", COL_WHITE, TRUE);
		OffsetSTextY(22, 6);
	}
}

void S_StartNoMoney()
{
	StartStore(stextshold);

	stextsize = 1;
	stextscrl = FALSE;

	ClearSText(5, 23);
	AddSText(0, 14, TRUE, "You do not have enough gold", COL_WHITE, TRUE);
}

void S_StartNoRoom()
{
	StartStore(stextshold);

	stextscrl = FALSE;

	ClearSText(5, 23);
	AddSText(0, 14, TRUE, "You do not have enough room in inventory", COL_WHITE, TRUE);
}

void S_StartConfirm()
{
	char iclr;
	BOOL idprint;

	StartStore(stextshold);

	stextscrl = FALSE;

	ClearSText(5, 23);

	iclr = COL_WHITE;
	if(plr[myplr].HoldItem._iMagical != 0) {
		iclr = COL_BLUE;
	}
	if(!plr[myplr].HoldItem._iStatFlag) {
		iclr = COL_RED;
	}

	idprint = plr[myplr].HoldItem._iMagical != 0;
	if(stextshold == STORE_SIDENTIFY) {
		idprint = FALSE;
	}
	if(plr[myplr].HoldItem._iMagical != 0 && !plr[myplr].HoldItem._iIdentified) {
		if(stextshold == STORE_SSELL) {
			idprint = FALSE;
		}
		if(stextshold == STORE_WSELL) {
			idprint = FALSE;
		}
		if(stextshold == STORE_SREPAIR) {
			idprint = FALSE;
		}
		if(stextshold == STORE_WRECHARGE) {
			idprint = FALSE;
		}
	}

	if(idprint) {
		AddSText(20, 8, FALSE, plr[myplr].HoldItem._iIName, iclr, FALSE);
	} else {
		AddSText(20, 8, FALSE, plr[myplr].HoldItem._iName, iclr, FALSE);
	}

	AddSTextVal(8, plr[myplr].HoldItem._iIvalue);
	PrintStoreItem(&plr[myplr].HoldItem, 9, iclr);

	switch(stextshold) {
	case STORE_SBUY:
	case STORE_WBUY:
	case STORE_HBUY:
	case STORE_SPBUY:
		strcpy(tempstr, "Are you sure you want to buy this item?");
		break;
	case STORE_SSELL:
	case STORE_WSELL:
		strcpy(tempstr, "Are you sure you want to sell this item?");
		break;
	case STORE_SREPAIR:
		strcpy(tempstr, "Are you sure you want to repair this item?");
		break;
	case STORE_WRECHARGE:
		strcpy(tempstr, "Are you sure you want to recharge this item?");
		break;
	case STORE_BBOY:
		strcpy(tempstr, "Do we have a deal?");
		break;
	case STORE_SIDENTIFY:
		strcpy(tempstr, "Are you sure you want to identify this item?");
		break;
	}

	AddSText(0, 15, TRUE, tempstr, COL_WHITE, FALSE);
	AddSText(0, 18, TRUE, "Yes", COL_WHITE, TRUE);
	AddSText(0, 20, TRUE, "No", COL_WHITE, TRUE);
}

void S_StartBoy()
{
	stextsize = 0;
	stextscrl = FALSE;

	AddSText(0, 2, TRUE, "Wirt the Peg-legged boy", COL_GOLD, FALSE);
	AddSLine(5);

	if(boyitem._itype != -1) {
		AddSText(0, 8, TRUE, "Talk to Wirt", COL_BLUE, TRUE);
		AddSText(0, 12, TRUE, "I have something for sale,", COL_GOLD, FALSE);
		AddSText(0, 14, TRUE, "but it will cost 50 gold", COL_GOLD, FALSE);
		AddSText(0, 16, TRUE, "just to take a look. ", COL_GOLD, FALSE);
		AddSText(0, 18, TRUE, "What have you got?", COL_WHITE, TRUE);
		AddSText(0, 20, TRUE, "Say goodbye", COL_WHITE, TRUE);
	} else {
		AddSText(0, 12, TRUE, "Talk to Wirt", COL_BLUE, TRUE);
		AddSText(0, 18, TRUE, "Say goodbye", COL_WHITE, TRUE);
	}
}

void S_StartBBoy()
{
	int iclr;

	stextsize = 1;
	stextscrl = FALSE;

	sprintf(tempstr, "I have this item for sale :           Your gold : %i", plr[myplr]._pGold);
	AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
	AddSLine(3);
	AddSLine(21);

	iclr = COL_WHITE;
	if(boyitem._iMagical != 0) {
		iclr = COL_BLUE;
	}
	if(!boyitem._iStatFlag) {
		iclr = COL_RED;
	}

	if(boyitem._iMagical != 0) {
		AddSText(20, 10, FALSE, boyitem._iIName, iclr, TRUE);
	} else {
		AddSText(20, 10, FALSE, boyitem._iName, iclr, TRUE);
	}

	AddSTextVal(10, boyitem._iIvalue + (boyitem._iIvalue >> 1));
	PrintStoreItem(&boyitem, 11, iclr);
	AddSText(0, 22, TRUE, "Leave", COL_WHITE, TRUE);
	OffsetSTextY(22, 6);
}

void S_StartHealer()
{
	stextsize = 0;
	stextscrl = FALSE;

	AddSText(0, 1, TRUE, "Welcome to the", COL_GOLD, FALSE);
	AddSText(0, 3, TRUE, "Healer's home", COL_GOLD, FALSE);
	AddSText(0, 9, TRUE, "Would you like to:", COL_GOLD, FALSE);
	AddSText(0, 12, TRUE, "Talk to Pepin", COL_BLUE, TRUE);
	AddSText(0, 14, TRUE, "Receive healing", COL_WHITE, TRUE);
	AddSText(0, 16, TRUE, "Buy items", COL_WHITE, TRUE);
	AddSText(0, 18, TRUE, "Leave Healer's home", COL_WHITE, TRUE);
	AddSLine(5);

	storenumh = 20;
}

void S_ScrollHBuy(int idx)
{
	int l, ls;
	char iclr;

	ClearSText(5, 21);

	stextup = 5;

	for(l = 5; l < 20; l += 4) {
		if(healitem[idx]._itype != -1) {
			ls = l;
			iclr = COL_WHITE;
			if(!healitem[idx]._iStatFlag) {
				iclr = COL_RED;
			}
			AddSText(20, l, FALSE, healitem[idx]._iName, iclr, TRUE);
			AddSTextVal(l, healitem[idx]._iIvalue);
			PrintStoreItem(&healitem[idx], l + 1, iclr);
			stextdown = ls;
			idx++;
		}
	}

	if(!stext[stextsel]._ssel && stextsel != 22) {
		stextsel = stextdown;
	}
}

void S_StartHBuy()
{
	int i;

	stextsize = 1;
	stextscrl = TRUE;
	stextsval = 0;

	sprintf(tempstr, "I have these items for sale :           Your gold : %i", plr[myplr]._pGold);
	AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
	AddSLine(3);
	AddSLine(21);
	S_ScrollHBuy(stextsval);
	AddSText(0, 22, TRUE, "Back", COL_WHITE, FALSE);
	OffsetSTextY(22, 6);

	storenumh = 0;
	for(i = 0; healitem[i]._itype != -1; i++) {
		storenumh++;
	}

	stextsmax = storenumh - 4;
	if(stextsmax < 0) {
		stextsmax = 0;
	}
}

void S_StartStory()
{
	stextsize = 0;
	stextscrl = FALSE;

	AddSText(0, 2, TRUE, "The Town Elder", COL_GOLD, FALSE);
	AddSText(0, 9, TRUE, "Would you like to:", COL_GOLD, FALSE);
	AddSText(0, 12, TRUE, "Talk to Cain", COL_BLUE, TRUE);
	AddSText(0, 14, TRUE, "Identify an item", COL_WHITE, TRUE);
	AddSText(0, 18, TRUE, "Say goodbye", COL_WHITE, TRUE);
	AddSLine(5);
}

BOOL IdItemOk(ItemStruct *i)
{
	if(i->_itype == -1) {
		return FALSE;
	}
	if(i->_iMagical == 0) {
		return FALSE;
	}
	if(i->_iIdentified) {
		return FALSE;
	}

	return TRUE;
}

void AddStoreHoldId(ItemStruct itm, int i)
{
	storehold[storenumh] = itm;
	storehold[storenumh]._ivalue = 100;
	storehold[storenumh]._iIvalue = 100;
	storehidx[storenumh] = i;
	storenumh++;
}

void S_StartSIdentify()
{
	int i;
	BOOL idok;

	stextsize = 1;
	idok = FALSE;
	storenumh = 0;

	for(i = 0; i < 48; i++) {
		storehold[i]._itype = -1;
	}

	if(IdItemOk(&plr[myplr].InvBody[0])) {
		idok = TRUE;
		AddStoreHoldId(plr[myplr].InvBody[0], -1);
	}
	if(IdItemOk(&plr[myplr].InvBody[6])) {
		idok = TRUE;
		AddStoreHoldId(plr[myplr].InvBody[6], -2);
	}
	if(IdItemOk(&plr[myplr].InvBody[4])) {
		idok = TRUE;
		AddStoreHoldId(plr[myplr].InvBody[4], -3);
	}
	if(IdItemOk(&plr[myplr].InvBody[5])) {
		idok = TRUE;
		AddStoreHoldId(plr[myplr].InvBody[5], -4);
	}
	if(IdItemOk(&plr[myplr].InvBody[1])) {
		idok = TRUE;
		AddStoreHoldId(plr[myplr].InvBody[1], -5);
	}
	if(IdItemOk(&plr[myplr].InvBody[2])) {
		idok = TRUE;
		AddStoreHoldId(plr[myplr].InvBody[2], -6);
	}
	if(IdItemOk(&plr[myplr].InvBody[3])) {
		idok = TRUE;
		AddStoreHoldId(plr[myplr].InvBody[3], -7);
	}

	for(i = 0; i < plr[myplr]._pNumInv; i++) {
		if(IdItemOk(&plr[myplr].InvList[i])) {
			idok = TRUE;
			AddStoreHoldId(plr[myplr].InvList[i], i);
		}
	}

	if(!idok) {
		stextscrl = FALSE;
		sprintf(tempstr, "You have nothing to identify.            Your gold : %i", plr[myplr]._pGold);
		AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
		AddSLine(3);
		AddSLine(21);
		AddSText(0, 22, TRUE, "Back", COL_WHITE, TRUE);
		OffsetSTextY(22, 6);
	} else {
		stextscrl = TRUE;
		stextsval = 0;
		stextsmax = plr[myplr]._pNumInv;
		sprintf(tempstr, "Identify which item?            Your gold : %i", plr[myplr]._pGold);
		AddSText(0, 1, TRUE, tempstr, COL_GOLD, FALSE);
		AddSLine(3);
		AddSLine(21);
		S_ScrollSSell(stextsval);
		AddSText(0, 22, TRUE, "Back", COL_WHITE, TRUE);
		OffsetSTextY(22, 6);
	}
}

void S_StartIdShow()
{
	char iclr;

	StartStore(stextshold);

	stextscrl = FALSE;

	ClearSText(5, 23);

	iclr = COL_WHITE;
	if(plr[myplr].HoldItem._iMagical != 0) {
		iclr = COL_BLUE;
	}
	if(!plr[myplr].HoldItem._iStatFlag) {
		iclr = COL_RED;
	}

	AddSText(0, 7, TRUE, "This item is:", COL_WHITE, FALSE);
	AddSText(20, 11, FALSE, plr[myplr].HoldItem._iIName, iclr, FALSE);
	PrintStoreItem(&plr[myplr].HoldItem, 12, iclr);
	AddSText(0, 18, TRUE, "Done", COL_WHITE, TRUE);
}

void S_StartTalk()
{
	int i, tq, sn, la, gl;

	stextsize = 0;
	stextscrl = FALSE;

	sprintf(tempstr, "Talk to %s", talkname[talker]);
	AddSText(0, 2, TRUE, tempstr, COL_GOLD, FALSE);
	AddSLine(5);

	tq = 0;
	for(i = 0; i < MAXQUESTS; i++) {
		if(quests[i]._qactive == 2 && Qtalklist[talker][i] != -1 && quests[i]._qlog) {
			tq++;
		}
	}

	if(tq > 6) {
		sn = 14 - (tq >> 1);
		la = 1;
	} else {
		sn = 15 - tq;
		la = 2;
	}

	gl = sn - 2;

	for(i = 0; i < MAXQUESTS; i++) {
		if(quests[i]._qactive == 2 && Qtalklist[talker][i] != -1 && quests[i]._qlog) {
			AddSText(0, sn, TRUE, questlist[i]._qlstr, COL_WHITE, TRUE);
			sn += la;
		}
	}

	AddSText(0, gl, TRUE, "Gossip", COL_BLUE, TRUE);
	AddSText(0, 22, TRUE, "Back", COL_WHITE, TRUE);
}

void S_StartTavern()
{
	stextsize = 0;
	stextscrl = FALSE;

	AddSText(0, 1, TRUE, "Welcome to the", COL_GOLD, FALSE);
	AddSText(0, 3, TRUE, "Rising Sun", COL_GOLD, FALSE);
	AddSText(0, 9, TRUE, "Would you like to:", COL_GOLD, FALSE);
	AddSText(0, 12, TRUE, "Talk to Ogden", COL_BLUE, TRUE);
	AddSText(0, 18, TRUE, "Leave the tavern", COL_WHITE, TRUE);
	AddSLine(5);

	storenumh = 20;
}

void S_StartBarMaid()
{
	stextsize = 0;
	stextscrl = FALSE;

	AddSText(0, 2, TRUE, "Gillian", COL_GOLD, FALSE);
	AddSText(0, 9, TRUE, "Would you like to:", COL_GOLD, FALSE);
	AddSText(0, 12, TRUE, "Talk to Gillian", COL_BLUE, TRUE);
	AddSText(0, 18, TRUE, "Say goodbye", COL_WHITE, TRUE);
	AddSLine(5);

	storenumh = 20;
}

void S_StartDrunk()
{
	stextsize = 0;
	stextscrl = FALSE;

	AddSText(0, 2, TRUE, "Farnham the Drunk", COL_GOLD, FALSE);
	AddSText(0, 9, TRUE, "Would you like to:", COL_GOLD, FALSE);
	AddSText(0, 12, TRUE, "Talk to Farnham", COL_BLUE, TRUE);
	AddSText(0, 18, TRUE, "Say Goodbye", COL_WHITE, TRUE);
	AddSLine(5);

	storenumh = 20;
}

void StartStore(char s)
{
	int i;

	sbookflag = 0;
	invflag = 0;
	chrflag = 0;
	questlog = 0;
	dropGoldFlag = 0;

	ClearSText(0, 24);
	ReleaseStoreBtn();

	switch(s) {
	case STORE_SMITH:
		S_StartSmith();
		break;
	case STORE_SBUY:
		if(storenumh > 0) {
			S_StartSBuy();
		}
		break;
	case STORE_SSELL:
		S_StartSSell();
		break;
	case STORE_SREPAIR:
		S_StartSRepair();
		break;
	case STORE_WITCH:
		S_StartWitch();
		break;
	case STORE_WBUY:
		if(storenumh > 0) {
			S_StartWBuy();
		}
		break;
	case STORE_WSELL:
		S_StartWSell();
		break;
	case STORE_WRECHARGE:
		S_StartWRecharge();
		break;
	case STORE_NOMONEY:
		S_StartNoMoney();
		break;
	case STORE_NOROOM:
		S_StartNoRoom();
		break;
	case STORE_CONFIRM:
		S_StartConfirm();
		break;
	case STORE_BOY:
		S_StartBoy();
		break;
	case STORE_BBOY:
		S_StartBBoy();
		break;
	case STORE_HEALER:
		S_StartHealer();
		break;
	case STORE_STORY:
		S_StartStory();
		break;
	case STORE_HBUY:
		if(storenumh > 0) {
			S_StartHBuy();
		}
		break;
	case STORE_SIDENTIFY:
		S_StartSIdentify();
		break;
	case STORE_SPBUY:
		if(!S_StartSPBuy()) {
			return;
		}
		break;
	case STORE_GOSSIP:
		S_StartTalk();
		break;
	case STORE_IDSHOW:
		S_StartIdShow();
		break;
	case STORE_TAVERN:
		S_StartTavern();
		break;
	case STORE_DRUNK:
		S_StartDrunk();
		break;
	case STORE_BARMAID:
		S_StartBarMaid();
		break;
	}

	for(i = 0; i < 24; i++) {
		if(stext[i]._ssel) {
			break;
		}
	}

	if(i == 24) {
		stextsel = -1;
	} else {
		stextsel = i;
	}

	stextflag = s;

	if(s == STORE_SBUY && storenumh == 0) {
		StartStore(STORE_SMITH);
	}
}

void DrawSText()
{
	int i;

	if(stextsize == 0) {
		DrawSTextBack();
	} else {
		DrawQTextBack();
	}

	if(stextscrl) {
		switch(stextflag) {
		case STORE_SBUY:
			S_ScrollSBuy(stextsval);
			break;
		case STORE_SPBUY:
			S_ScrollSPBuy(stextsval);
			break;
		case STORE_SSELL:
		case STORE_SREPAIR:
		case STORE_WSELL:
		case STORE_WRECHARGE:
		case STORE_SIDENTIFY:
			S_ScrollSSell(stextsval);
			break;
		case STORE_WBUY:
			S_ScrollWBuy(stextsval);
			break;
		case STORE_HBUY:
			S_ScrollHBuy(stextsval);
			break;
		}
	}

	for(i = 0; i < 24; i++) {
		if(stext[i]._sline) {
			DrawSLine(i);
		}
		if(stext[i]._sstr[0] != '\0') {
			PrintSString(stext[i]._sx, i, stext[i]._sjust, stext[i]._sstr, stext[i]._sclr, stext[i]._sval);
		}
	}

	if(stextscrl) {
		DrawSSlider(4, 20);
	}

	sspinframe = (sspinframe & 7) + 1;
}

void STextESC()
{
	if(qtextflag) {
		qtextflag = 0;
		if(leveltype == DTYPE_TOWN) {
			stream_stop();
		}
		return;
	}

	switch(stextflag) {
	case STORE_SMITH:
	case STORE_WITCH:
	case STORE_BOY:
	case STORE_BBOY:
	case STORE_HEALER:
	case STORE_STORY:
	case STORE_TAVERN:
	case STORE_DRUNK:
	case STORE_BARMAID:
		stextflag = 0;
		break;
	case STORE_GOSSIP:
		StartStore(stextshold);
		stextsel = stextlhold;
		break;
	case STORE_SBUY:
		StartStore(STORE_SMITH);
		stextsel = 12;
		break;
	case STORE_SPBUY:
		StartStore(STORE_SMITH);
		stextsel = 14;
		break;
	case STORE_SSELL:
		StartStore(STORE_SMITH);
		stextsel = 16;
		break;
	case STORE_SREPAIR:
		StartStore(STORE_SMITH);
		stextsel = 18;
		break;
	case STORE_WBUY:
		StartStore(STORE_WITCH);
		stextsel = 14;
		break;
	case STORE_WSELL:
		StartStore(STORE_WITCH);
		stextsel = 16;
		break;
	case STORE_WRECHARGE:
		StartStore(STORE_WITCH);
		stextsel = 18;
		break;
	case STORE_HBUY:
		StartStore(STORE_HEALER);
		stextsel = 16;
		break;
	case STORE_SIDENTIFY:
		StartStore(STORE_STORY);
		stextsel = 14;
		break;
	case STORE_IDSHOW:
		StartStore(STORE_SIDENTIFY);
		break;
	case STORE_NOMONEY:
	case STORE_NOROOM:
	case STORE_CONFIRM:
		StartStore(stextshold);
		stextsel = stextlhold;
		stextsval = stextvhold;
		break;
	}
}

void STextUp()
{
	PlaySFX(IS_TITLEMOV);

	if(stextsel != -1) {
		if(stextscrl) {
			if(stextsel == stextup) {
				if(stextsval != 0) {
					stextsval--;
				}
			} else {
				stextsel--;
				while(!stext[stextsel]._ssel) {
					if(stextsel == 0) {
						stextsel = 23;
					} else {
						stextsel--;
					}
				}
			}
		} else {
			if(stextsel == 0) {
				stextsel = 23;
			} else {
				stextsel--;
			}
			while(!stext[stextsel]._ssel) {
				if(stextsel == 0) {
					stextsel = 23;
				} else {
					stextsel--;
				}
			}
		}
	}
}

void STextDown()
{
	PlaySFX(IS_TITLEMOV);

	if(stextsel != -1) {
		if(stextscrl) {
			if(stextsel == stextdown) {
				if(stextsval < stextsmax) {
					stextsval++;
				}
			} else {
				stextsel++;
				while(!stext[stextsel]._ssel) {
					if(stextsel == 23) {
						stextsel = 0;
					} else {
						stextsel++;
					}
				}
			}
		} else {
			if(stextsel == 23) {
				stextsel = 0;
			} else {
				stextsel++;
			}
			while(!stext[stextsel]._ssel) {
				if(stextsel == 23) {
					stextsel = 0;
				} else {
					stextsel++;
				}
			}
		}
	}
}

void STextPrior()
{
	PlaySFX(IS_TITLEMOV);

	if(stextsel != -1 && stextscrl) {
		if(stextsel == stextup) {
			if(stextsval != 0) {
				stextsval -= 4;
			}
			if(stextsval < 0) {
				stextsval = 0;
			}
		} else {
			stextsel = stextup;
		}
	}
}

void STextNext()
{
	PlaySFX(IS_TITLEMOV);

	if(stextsel != -1 && stextscrl) {
		if(stextsel == stextdown) {
			if(stextsval < stextsmax) {
				stextsval += 4;
			}
			if(stextsval > stextsmax) {
				stextsval = stextsmax;
			}
		} else {
			stextsel = stextdown;
		}
	}
}

void S_SmithEnter()
{
	switch(stextsel) {
	case 10:
		talker = TOWN_SMITH;
		stextshold = STORE_SMITH;
		stextlhold = stextsel;
		gossipstart = TEXT_GRISWOLD2;
		gossipend = TEXT_GRISWOLD13;
		StartStore(STORE_GOSSIP);
		break;
	case 12:
		StartStore(STORE_SBUY);
		break;
	case 14:
		StartStore(STORE_SPBUY);
		break;
	case 16:
		StartStore(STORE_SSELL);
		break;
	case 18:
		StartStore(STORE_SREPAIR);
		break;
	case 20:
		stextflag = 0;
		break;
	}
}

void SetGoldCurs(int pnum, int i)
{
	if(plr[pnum].InvList[i]._ivalue >= 2500) {
		plr[pnum].InvList[i]._iCurs = 6;
	} else if(plr[pnum].InvList[i]._ivalue <= 1000) {
		plr[pnum].InvList[i]._iCurs = 4;
	} else {
		plr[pnum].InvList[i]._iCurs = 5;
	}
}

void SetSpdbarGoldCurs(int pnum, int i)
{
	if(plr[pnum].SpdList[i]._ivalue >= 2500) {
		plr[pnum].SpdList[i]._iCurs = 6;
	} else if(plr[pnum].SpdList[i]._ivalue <= 1000) {
		plr[pnum].SpdList[i]._iCurs = 4;
	} else {
		plr[pnum].SpdList[i]._iCurs = 5;
	}
}

void TakePlrsMoney(long cost)
{
	int i;

	plr[myplr]._pGold = CalculateGold(myplr) - cost;

	for(i = 0; i < 8 && cost > 0; i++) {
		if(plr[myplr].SpdList[i]._itype == ITYPE_GOLD && plr[myplr].SpdList[i]._ivalue != 5000) {
			if(cost < plr[myplr].SpdList[i]._ivalue) {
				plr[myplr].SpdList[i]._ivalue -= cost;
				SetSpdbarGoldCurs(myplr, i);
				cost = 0;
			} else {
				cost -= plr[myplr].SpdList[i]._ivalue;
				RemoveSpdBarItem(myplr, i);
				i = -1;
			}
		}
	}
	if(cost > 0) {
		for(i = 0; i < 8 && cost > 0; i++) {
			if(plr[myplr].SpdList[i]._itype == ITYPE_GOLD) {
				if(cost < plr[myplr].SpdList[i]._ivalue) {
					plr[myplr].SpdList[i]._ivalue -= cost;
					SetSpdbarGoldCurs(myplr, i);
					cost = 0;
				} else {
					cost -= plr[myplr].SpdList[i]._ivalue;
					RemoveSpdBarItem(myplr, i);
					i = -1;
				}
			}
		}
	}

	force_redraw = 255;

	if(cost > 0) {
		for(i = 0; i < plr[myplr]._pNumInv && cost > 0; i++) {
			if(plr[myplr].InvList[i]._itype == ITYPE_GOLD && plr[myplr].InvList[i]._ivalue != 5000) {
				if(cost < plr[myplr].InvList[i]._ivalue) {
					plr[myplr].InvList[i]._ivalue -= cost;
					SetGoldCurs(myplr, i);
					cost = 0;
				} else {
					cost -= plr[myplr].InvList[i]._ivalue;
					RemoveInvItem(myplr, i);
					i = -1;
				}
			}
		}
		if(cost > 0) {
			for(i = 0; i < plr[myplr]._pNumInv && cost > 0; i++) {
				if(plr[myplr].InvList[i]._itype == ITYPE_GOLD) {
					if(cost < plr[myplr].InvList[i]._ivalue) {
						plr[myplr].InvList[i]._ivalue -= cost;
						SetGoldCurs(myplr, i);
						cost = 0;
					} else {
						cost -= plr[myplr].InvList[i]._ivalue;
						RemoveInvItem(myplr, i);
						i = -1;
					}
				}
			}
		}
	}
}

void SmithBuyItem()
{
	int idx;

	TakePlrsMoney(plr[myplr].HoldItem._iIvalue);

	if(plr[myplr].HoldItem._iMagical == 0) {
		plr[myplr].HoldItem._iIdentified = FALSE;
	}

	StoreAutoPlace();

	idx = stextvhold + ((stextlhold - stextup) >> 2);
	if(idx == 19) {
		smithitem[idx]._itype = -1;
	} else {
		while(smithitem[idx + 1]._itype != -1) {
			smithitem[idx] = smithitem[idx + 1];
			idx++;
		}
		smithitem[idx]._itype = -1;
	}

	CalcPlrInv(myplr, TRUE);
}

void S_SBuyEnter()
{
	int idx, i;
	BOOL done;

	if(stextsel == 22) {
		StartStore(STORE_SMITH);
		stextsel = 12;
	} else {
		stextshold = STORE_SBUY;
		stextlhold = stextsel;
		stextvhold = stextsval;
		idx = stextsval + ((stextsel - stextup) >> 2);
		if(plr[myplr]._pGold < smithitem[idx]._iIvalue) {
			StartStore(STORE_NOMONEY);
		} else {
			plr[myplr].HoldItem = smithitem[idx];
			SetCursor_(plr[myplr].HoldItem._iCurs + 12);
			done = FALSE;
			for(i = 0; i < 40 && !done; i++) {
				done = AutoPlace(myplr, i, cursW / 28, cursH / 28, FALSE);
			}
			if(done) {
				StartStore(STORE_CONFIRM);
			} else {
				StartStore(STORE_NOROOM);
			}
			SetCursor_(CURSOR_HAND);
		}
	}
}

void SmithBuyPItem()
{
	int idx, i, xx;

	TakePlrsMoney(plr[myplr].HoldItem._iIvalue);

	if(plr[myplr].HoldItem._iMagical == 0) {
		plr[myplr].HoldItem._iIdentified = FALSE;
	}

	StoreAutoPlace();

	idx = stextvhold + ((stextlhold - stextup) >> 2);
	xx = 0;
	for(i = 0; idx >= 0; i++) {
		if(premiumitem[i]._itype != -1) {
			idx--;
			xx = i;
		}
	}

	premiumitem[xx]._itype = -1;
	numpremium--;

	SpawnPremium(plr[myplr]._pLevel);
}

void S_SPBuyEnter()
{
	int idx, i, xx;
	BOOL done;

	if(stextsel == 22) {
		StartStore(STORE_SMITH);
		stextsel = 14;
	} else {
		stextshold = STORE_SPBUY;
		stextlhold = stextsel;
		stextvhold = stextsval;
		idx = stextsval + ((stextsel - stextup) >> 2);
		xx = 0;
		for(i = 0; idx >= 0; i++) {
			if(premiumitem[i]._itype != -1) {
				idx--;
				xx = i;
			}
		}
		if(plr[myplr]._pGold < premiumitem[xx]._iIvalue) {
			StartStore(STORE_NOMONEY);
		} else {
			plr[myplr].HoldItem = premiumitem[xx];
			SetCursor_(plr[myplr].HoldItem._iCurs + 12);
			done = FALSE;
			for(i = 0; i < 40 && !done; i++) {
				done = AutoPlace(myplr, i, cursW / 28, cursH / 28, FALSE);
			}
			if(done) {
				StartStore(STORE_CONFIRM);
			} else {
				StartStore(STORE_NOROOM);
			}
			SetCursor_(CURSOR_HAND);
		}
	}
}

BOOL StoreGoldFit(int idx)
{
	int sz, numsqrs, i;
	long cost;

	cost = storehold[idx]._iIvalue;

	sz = cost / 5000;
	if(cost % 5000) {
		sz++;
	}

	SetCursor_(storehold[idx]._iCurs + 12);
	numsqrs = (cursW / 28) * (cursH / 28);
	SetCursor_(CURSOR_HAND);

	if(numsqrs >= sz) {
		return TRUE;
	}

	for(i = 0; i < 40; i++) {
		if(plr[myplr].InvGrid[i] == 0) {
			numsqrs++;
		}
	}

	for(i = 0; i < plr[myplr]._pNumInv; i++) {
		if(plr[myplr].InvList[i]._itype == ITYPE_GOLD && plr[myplr].InvList[i]._ivalue != 5000) {
			if(cost + plr[myplr].InvList[i]._ivalue <= 5000) {
				cost = 0;
			} else {
				cost -= 5000 - plr[myplr].InvList[i]._ivalue;
			}
		}
	}

	sz = cost / 5000;
	if(cost % 5000) {
		sz++;
	}

	if(numsqrs >= sz) {
		return TRUE;
	}

	return FALSE;
}

void PlaceStoreGold(long v)
{
	int i, ii, xx, yy;
	BOOL done;

	done = FALSE;
	for(ii = 0; ii < 40 && !done; ii++) {
		yy = 10 * (ii / 10);
		xx = ii % 10;
		if(plr[myplr].InvGrid[xx + yy] == 0) {
			i = plr[myplr]._pNumInv;
			GetGoldSeed(myplr, &golditem);
			plr[myplr].InvList[i] = golditem;
			plr[myplr]._pNumInv++;
			plr[myplr].InvGrid[xx + yy] = plr[myplr]._pNumInv;
			plr[myplr].InvList[i]._ivalue = v;
			SetGoldCurs(myplr, i);
			done = TRUE;
		}
	}
}

void StoreSellItem()
{
	int idx, i;
	long cost;

	idx = stextvhold + ((stextlhold - stextup) >> 2);

	if(storehidx[idx] >= 0) {
		RemoveInvItem(myplr, storehidx[idx]);
	} else {
		RemoveSpdBarItem(myplr, -(storehidx[idx] + 1));
	}

	cost = storehold[idx]._iIvalue;

	storenumh--;
	if(idx != storenumh) {
		while(idx < storenumh) {
			storehold[idx] = storehold[idx + 1];
			storehidx[idx] = storehidx[idx + 1];
			idx++;
		}
	}

	plr[myplr]._pGold += cost;

	for(i = 0; i < plr[myplr]._pNumInv && cost > 0; i++) {
		if(plr[myplr].InvList[i]._itype == ITYPE_GOLD && plr[myplr].InvList[i]._ivalue != 5000) {
			if(cost + plr[myplr].InvList[i]._ivalue <= 5000) {
				plr[myplr].InvList[i]._ivalue += cost;
				SetGoldCurs(myplr, i);
				cost = 0;
			} else {
				cost -= 5000 - plr[myplr].InvList[i]._ivalue;
				plr[myplr].InvList[i]._ivalue = 5000;
				SetGoldCurs(myplr, i);
			}
		}
	}

	if(cost > 0) {
		while(cost > 5000) {
			PlaceStoreGold(5000);
			cost -= 5000;
		}
		PlaceStoreGold(cost);
	}
}

void S_SSellEnter()
{
	int idx;

	if(stextsel == 22) {
		StartStore(STORE_SMITH);
		stextsel = 16;
	} else {
		stextshold = STORE_SSELL;
		stextlhold = stextsel;
		stextvhold = stextsval;
		idx = stextsval + ((stextsel - stextup) >> 2);
		plr[myplr].HoldItem = storehold[idx];
		if(StoreGoldFit(idx)) {
			StartStore(STORE_CONFIRM);
		} else {
			StartStore(STORE_NOROOM);
		}
	}
}

void SmithRepairItem()
{
	int i, idx;

	TakePlrsMoney(plr[myplr].HoldItem._iIvalue);

	idx = stextvhold + ((stextlhold - stextup) >> 2);
	storehold[idx]._iDurability = storehold[idx]._iMaxDur;

	i = storehidx[idx];
	if(i < 0) {
		if(i == -1) {
			plr[myplr].InvBody[0]._iDurability = plr[myplr].InvBody[0]._iMaxDur;
		}
		if(i == -2) {
			plr[myplr].InvBody[6]._iDurability = plr[myplr].InvBody[6]._iMaxDur;
		}
		if(i == -3) {
			plr[myplr].InvBody[4]._iDurability = plr[myplr].InvBody[4]._iMaxDur;
		}
		if(i == -4) {
			plr[myplr].InvBody[5]._iDurability = plr[myplr].InvBody[5]._iMaxDur;
		}
	} else {
		plr[myplr].InvList[i]._iDurability = plr[myplr].InvList[i]._iMaxDur;
	}
}

void S_SRepairEnter()
{
	int idx;

	if(stextsel == 22) {
		StartStore(STORE_SMITH);
		stextsel = 18;
	} else {
		stextshold = STORE_SREPAIR;
		stextlhold = stextsel;
		stextvhold = stextsval;
		idx = stextsval + ((stextsel - stextup) >> 2);
		plr[myplr].HoldItem = storehold[idx];
		if(plr[myplr]._pGold < storehold[idx]._iIvalue) {
			StartStore(STORE_NOMONEY);
		} else {
			StartStore(STORE_CONFIRM);
		}
	}
}

void S_WitchEnter()
{
	switch(stextsel) {
	case 12:
		talker = TOWN_WITCH;
		stextshold = STORE_WITCH;
		stextlhold = stextsel;
		gossipstart = TEXT_ADRIA2;
		gossipend = TEXT_ADRIA13;
		StartStore(STORE_GOSSIP);
		break;
	case 14:
		StartStore(STORE_WBUY);
		break;
	case 16:
		StartStore(STORE_WSELL);
		break;
	case 18:
		StartStore(STORE_WRECHARGE);
		break;
	case 20:
		stextflag = 0;
		break;
	}
}

void WitchBuyItem()
{
	int idx;

	idx = stextvhold + ((stextlhold - stextup) >> 2);
	if(idx < 3) {
		plr[myplr].HoldItem._iSeed = GetRndSeed();
	}

	TakePlrsMoney(plr[myplr].HoldItem._iIvalue);
	StoreAutoPlace();

	if(idx < 3) {
		CalcPlrInv(myplr, TRUE);
		return;
	}

	if(idx == 19) {
		witchitem[idx]._itype = -1;
	} else {
		while(witchitem[idx + 1]._itype != -1) {
			witchitem[idx] = witchitem[idx + 1];
			idx++;
		}
		witchitem[idx]._itype = -1;
	}

	CalcPlrInv(myplr, TRUE);
}

void S_WBuyEnter()
{
	int idx, i;
	BOOL done;

	if(stextsel == 22) {
		StartStore(STORE_WITCH);
		stextsel = 14;
	} else {
		stextshold = STORE_WBUY;
		stextlhold = stextsel;
		stextvhold = stextsval;
		idx = stextsval + ((stextsel - stextup) >> 2);
		if(plr[myplr]._pGold < witchitem[idx]._iIvalue) {
			StartStore(STORE_NOMONEY);
		} else {
			plr[myplr].HoldItem = witchitem[idx];
			SetCursor_(plr[myplr].HoldItem._iCurs + 12);
			done = FALSE;
			for(i = 0; i < 40 && !done; i++) {
				done = SpecialAutoPlace(myplr, i, cursW / 28, cursH / 28, FALSE);
			}
			if(done) {
				StartStore(STORE_CONFIRM);
			} else {
				StartStore(STORE_NOROOM);
			}
			SetCursor_(CURSOR_HAND);
		}
	}
}

void S_WSellEnter()
{
	int idx;

	if(stextsel == 22) {
		StartStore(STORE_WITCH);
		stextsel = 16;
	} else {
		stextshold = STORE_WSELL;
		stextlhold = stextsel;
		stextvhold = stextsval;
		idx = stextsval + ((stextsel - stextup) >> 2);
		plr[myplr].HoldItem = storehold[idx];
		if(StoreGoldFit(idx)) {
			StartStore(STORE_CONFIRM);
		} else {
			StartStore(STORE_NOROOM);
		}
	}
}

void WitchRechargeItem()
{
	int i, idx;

	TakePlrsMoney(plr[myplr].HoldItem._iIvalue);

	idx = stextvhold + ((stextlhold - stextup) >> 2);
	storehold[idx]._iCharges = storehold[idx]._iMaxCharges;

	i = storehidx[idx];
	if(i < 0) {
		plr[myplr].InvBody[4]._iCharges = plr[myplr].InvBody[4]._iMaxCharges;
	} else {
		plr[myplr].InvList[i]._iCharges = plr[myplr].InvList[i]._iMaxCharges;
	}

	CalcPlrInv(myplr, TRUE);
}

void S_WRechargeEnter()
{
	int idx;

	if(stextsel == 22) {
		StartStore(STORE_WITCH);
		stextsel = 18;
	} else {
		stextshold = STORE_WRECHARGE;
		stextlhold = stextsel;
		stextvhold = stextsval;
		idx = stextsval + ((stextsel - stextup) >> 2);
		plr[myplr].HoldItem = storehold[idx];
		if(plr[myplr]._pGold < storehold[idx]._iIvalue) {
			StartStore(STORE_NOMONEY);
		} else {
			StartStore(STORE_CONFIRM);
		}
	}
}

void S_BoyEnter()
{
	if(boyitem._itype != -1 && stextsel == 18) {
		if(plr[myplr]._pGold < 50) {
			stextshold = STORE_BOY;
			stextlhold = stextsel;
			stextvhold = stextsval;
			StartStore(STORE_NOMONEY);
		} else {
			TakePlrsMoney(50);
			StartStore(STORE_BBOY);
		}
	} else if(stextsel == 8 && boyitem._itype != -1 || stextsel == 12 && boyitem._itype == -1) {
		talker = TOWN_PEGBOY;
		stextshold = STORE_BOY;
		stextlhold = stextsel;
		gossipstart = TEXT_WIRT2;
		gossipend = TEXT_WIRT12;
		StartStore(STORE_GOSSIP);
	} else {
		stextflag = 0;
	}
}

void BoyBuyItem()
{
	TakePlrsMoney(plr[myplr].HoldItem._iIvalue);
	StoreAutoPlace();

	boyitem._itype = -1;
	stextshold = STORE_BOY;

	CalcPlrInv(myplr, TRUE);
}

void HealerBuyItem()
{
	int idx;

	idx = stextvhold + ((stextlhold - stextup) >> 2);
	if(gbMaxPlayers == 1) {
		if(idx < 2) {
			plr[myplr].HoldItem._iSeed = GetRndSeed();
		}
	} else {
		if(idx < 3) {
			plr[myplr].HoldItem._iSeed = GetRndSeed();
		}
	}

	TakePlrsMoney(plr[myplr].HoldItem._iIvalue);

	if(plr[myplr].HoldItem._iMagical == 0) {
		plr[myplr].HoldItem._iIdentified = FALSE;
	}

	StoreAutoPlace();

	if(gbMaxPlayers == 1) {
		if(idx < 2) {
			return;
		}
	} else {
		if(idx < 3) {
			return;
		}
	}

	idx = stextvhold + ((stextlhold - stextup) >> 2);
	if(idx == 19) {
		healitem[idx]._itype = -1;
	} else {
		while(healitem[idx + 1]._itype != -1) {
			healitem[idx] = healitem[idx + 1];
			idx++;
		}
		healitem[idx]._itype = -1;
	}

	CalcPlrInv(myplr, TRUE);
}

void S_BBuyEnter()
{
	int i;
	BOOL done;

	if(stextsel == 10) {
		stextshold = STORE_BBOY;
		stextlhold = stextsel;
		stextvhold = stextsval;
		if(plr[myplr]._pGold < boyitem._iIvalue + (boyitem._iIvalue >> 1)) {
			StartStore(STORE_NOMONEY);
		} else {
			plr[myplr].HoldItem = boyitem;
			plr[myplr].HoldItem._iIvalue += plr[myplr].HoldItem._iIvalue >> 1;
			SetCursor_(plr[myplr].HoldItem._iCurs + 12);
			done = FALSE;
			for(i = 0; i < 40 && !done; i++) {
				done = AutoPlace(myplr, i, cursW / 28, cursH / 28, FALSE);
			}
			if(done) {
				StartStore(STORE_CONFIRM);
			} else {
				StartStore(STORE_NOROOM);
			}
			SetCursor_(CURSOR_HAND);
		}
	} else {
		stextflag = 0;
	}
}

void StoryIdItem()
{
	int i, idx;

	idx = stextvhold + ((stextlhold - stextup) >> 2);

	i = storehidx[idx];
	if(i < 0) {
		if(i == -1) {
			plr[myplr].InvBody[0]._iIdentified = TRUE;
		}
		if(i == -2) {
			plr[myplr].InvBody[6]._iIdentified = TRUE;
		}
		if(i == -3) {
			plr[myplr].InvBody[4]._iIdentified = TRUE;
		}
		if(i == -4) {
			plr[myplr].InvBody[5]._iIdentified = TRUE;
		}
		if(i == -5) {
			plr[myplr].InvBody[1]._iIdentified = TRUE;
		}
		if(i == -6) {
			plr[myplr].InvBody[2]._iIdentified = TRUE;
		}
		if(i == -7) {
			plr[myplr].InvBody[3]._iIdentified = TRUE;
		}
	} else {
		plr[myplr].InvList[i]._iIdentified = TRUE;
	}

	plr[myplr].HoldItem._iIdentified = TRUE;

	TakePlrsMoney(plr[myplr].HoldItem._iIvalue);
	CalcPlrInv(myplr, TRUE);
}

void S_ConfirmEnter()
{
	if(stextsel == 18) {
		switch(stextshold) {
		case STORE_SBUY:
			SmithBuyItem();
			break;
		case STORE_SPBUY:
			SmithBuyPItem();
			break;
		case STORE_SSELL:
		case STORE_WSELL:
			StoreSellItem();
			break;
		case STORE_SREPAIR:
			SmithRepairItem();
			break;
		case STORE_WBUY:
			WitchBuyItem();
			break;
		case STORE_WRECHARGE:
			WitchRechargeItem();
			break;
		case STORE_BBOY:
			BoyBuyItem();
			break;
		case STORE_HBUY:
			HealerBuyItem();
			break;
		case STORE_SIDENTIFY:
			StoryIdItem();
			StartStore(STORE_IDSHOW);
			return;
		}
		StartStore(stextshold);
	} else {
		StartStore(stextshold);
		stextsel = stextlhold;
		stextsval = stextvhold;
	}
}

void S_HealerEnter()
{
	switch(stextsel) {
	case 12:
		talker = TOWN_HEALER;
		stextshold = STORE_HEALER;
		stextlhold = stextsel;
		gossipstart = TEXT_PEPIN2;
		gossipend = TEXT_PEPIN11;
		StartStore(STORE_GOSSIP);
		break;
	case 14:
		if(plr[myplr]._pHitPoints != plr[myplr]._pMaxHP) {
			PlaySFX(IS_CAST8);
		}
		plr[myplr]._pHitPoints = plr[myplr]._pMaxHP;
		plr[myplr]._pHPBase = plr[myplr]._pMaxHPBase;
		drawhpflag = 1;
		break;
	case 16:
		StartStore(STORE_HBUY);
		break;
	case 18:
		stextflag = 0;
		break;
	}
}

void S_HBuyEnter()
{
	int idx, i;
	BOOL done;

	if(stextsel == 22) {
		StartStore(STORE_HEALER);
		stextsel = 16;
	} else {
		stextshold = STORE_HBUY;
		stextlhold = stextsel;
		stextvhold = stextsval;
		idx = stextsval + ((stextsel - stextup) >> 2);
		if(plr[myplr]._pGold < healitem[idx]._iIvalue) {
			StartStore(STORE_NOMONEY);
		} else {
			plr[myplr].HoldItem = healitem[idx];
			SetCursor_(plr[myplr].HoldItem._iCurs + 12);
			done = FALSE;
			for(i = 0; i < 40 && !done; i++) {
				done = SpecialAutoPlace(myplr, i, cursW / 28, cursH / 28, FALSE);
			}
			if(done) {
				StartStore(STORE_CONFIRM);
			} else {
				StartStore(STORE_NOROOM);
			}
			SetCursor_(CURSOR_HAND);
		}
	}
}

void S_StoryEnter()
{
	switch(stextsel) {
	case 12:
		talker = TOWN_STORY;
		stextshold = STORE_STORY;
		stextlhold = stextsel;
		gossipstart = TEXT_STORY2;
		gossipend = TEXT_STORY11;
		StartStore(STORE_GOSSIP);
		break;
	case 14:
		StartStore(STORE_SIDENTIFY);
		break;
	case 18:
		stextflag = 0;
		break;
	}
}

void S_SIDEnter()
{
	int idx;

	if(stextsel == 22) {
		StartStore(STORE_STORY);
		stextsel = 14;
	} else {
		stextshold = STORE_SIDENTIFY;
		stextlhold = stextsel;
		stextvhold = stextsval;
		idx = stextsval + ((stextsel - stextup) >> 2);
		plr[myplr].HoldItem = storehold[idx];
		if(plr[myplr]._pGold < storehold[idx]._iIvalue) {
			StartStore(STORE_NOMONEY);
		} else {
			StartStore(STORE_CONFIRM);
		}
	}
}

void S_TalkEnter()
{
	int i, tq, sn, la, gl, m;

	if(stextsel == 22) {
		StartStore(stextshold);
		stextsel = stextlhold;
	} else {
		tq = 0;
		for(i = 0; i < MAXQUESTS; i++) {
			if(quests[i]._qactive == 2 && Qtalklist[talker][i] != -1 && quests[i]._qlog) {
				tq++;
			}
		}
		if(tq > 6) {
			sn = 14 - (tq >> 1);
			la = 1;
		} else {
			sn = 15 - tq;
			la = 2;
		}
		gl = sn - 2;
		if(stextsel == gl) {
			SetRndSeed(towner[talker]._tSeed);
			m = random(0, gossipend - gossipstart + 1) + gossipstart;
			InitQTextMsg(m);
		} else {
			for(i = 0; i < MAXQUESTS; i++) {
				if(quests[i]._qactive == 2 && Qtalklist[talker][i] != -1 && quests[i]._qlog) {
					if(sn == stextsel) {
						InitQTextMsg(Qtalklist[talker][i]);
					}
					sn += la;
				}
			}
		}
	}
}

void S_TavernEnter()
{
	switch(stextsel) {
	case 12:
		talker = TOWN_TAVERN;
		stextshold = STORE_TAVERN;
		stextlhold = 12;
		gossipstart = TEXT_OGDEN2;
		gossipend = TEXT_OGDEN10;
		StartStore(STORE_GOSSIP);
		break;
	case 18:
		stextflag = 0;
		break;
	}
}

void S_BarmaidEnter()
{
	switch(stextsel) {
	case 12:
		talker = TOWN_BMAID;
		stextshold = STORE_BARMAID;
		stextlhold = 12;
		gossipstart = TEXT_GILLIAN2;
		gossipend = TEXT_GILLIAN10;
		StartStore(STORE_GOSSIP);
		break;
	case 18:
		stextflag = 0;
		break;
	}
}

void S_DrunkEnter()
{
	switch(stextsel) {
	case 12:
		talker = TOWN_DRUNK;
		stextshold = STORE_DRUNK;
		stextlhold = 12;
		gossipstart = TEXT_FARNHAM2;
		gossipend = TEXT_FARNHAM13;
		StartStore(STORE_GOSSIP);
		break;
	case 18:
		stextflag = 0;
		break;
	}
}

void STextEnter()
{
	if(qtextflag) {
		qtextflag = 0;
		if(leveltype == DTYPE_TOWN) {
			stream_stop();
		}
		return;
	}

	PlaySFX(IS_TITLSLCT);

	switch(stextflag) {
	case STORE_SMITH:
		S_SmithEnter();
		break;
	case STORE_SBUY:
		S_SBuyEnter();
		break;
	case STORE_SPBUY:
		S_SPBuyEnter();
		break;
	case STORE_SSELL:
		S_SSellEnter();
		break;
	case STORE_SREPAIR:
		S_SRepairEnter();
		break;
	case STORE_WITCH:
		S_WitchEnter();
		break;
	case STORE_WBUY:
		S_WBuyEnter();
		break;
	case STORE_WSELL:
		S_WSellEnter();
		break;
	case STORE_WRECHARGE:
		S_WRechargeEnter();
		break;
	case STORE_NOMONEY:
	case STORE_NOROOM:
		StartStore(stextshold);
		stextsel = stextlhold;
		stextsval = stextvhold;
		break;
	case STORE_CONFIRM:
		S_ConfirmEnter();
		break;
	case STORE_BOY:
		S_BoyEnter();
		break;
	case STORE_BBOY:
		S_BBuyEnter();
		break;
	case STORE_HEALER:
		S_HealerEnter();
		break;
	case STORE_STORY:
		S_StoryEnter();
		break;
	case STORE_HBUY:
		S_HBuyEnter();
		break;
	case STORE_SIDENTIFY:
		S_SIDEnter();
		break;
	case STORE_GOSSIP:
		S_TalkEnter();
		break;
	case STORE_IDSHOW:
		StartStore(STORE_SIDENTIFY);
		break;
	case STORE_TAVERN:
		S_TavernEnter();
		break;
	case STORE_BARMAID:
		S_BarmaidEnter();
		break;
	case STORE_DRUNK:
		S_DrunkEnter();
		break;
	}
}

void CheckStoreBtn()
{
	int y;

	if(qtextflag) {
		qtextflag = 0;
		if(leveltype == DTYPE_TOWN) {
			stream_stop();
		}
		return;
	}

	if(stextsel == -1) {
		return;
	}
	if(MouseY < 32 || MouseY > 320) {
		return;
	}
	if(stextsize == 0) {
		if(MouseX < 344 || MouseX > 616) {
			return;
		}
	} else {
		if(MouseX < 24 || MouseX > 616) {
			return;
		}
	}

	y = (MouseY - 32) / 12;
	if(stextscrl && MouseX > 600) {
		if(y == 4) {
			if(stextscrlubtn <= 0) {
				STextUp();
				stextscrlubtn = 10;
			} else {
				stextscrlubtn--;
			}
		}
		if(y == 20) {
			if(stextscrldbtn <= 0) {
				STextDown();
				stextscrldbtn = 10;
			} else {
				stextscrldbtn--;
			}
		}
	} else if(y >= 5) {
		if(y >= 23) {
			y = 22;
		}
		if(stextscrl && y < 21 && !stext[y]._ssel) {
			if(stext[y - 2]._ssel) {
				y -= 2;
			} else if(stext[y - 1]._ssel) {
				y--;
			}
		}
		if(stext[y]._ssel || stextscrl && y == 22) {
			stextsel = y;
			STextEnter();
		}
	}
}

void ReleaseStoreBtn()
{
	stextscrlubtn = -1;
	stextscrldbtn = -1;
}
