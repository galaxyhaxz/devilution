#include "diablo.h"

int invflag;
BYTE *pInvCels;
int drawsbarflag; // idb
int sgdwLastTime; // check name

const InvXY InvRect[73] =
{
  { 452, 31 },  // helmet
  { 480, 31 },  // helmet
  { 452, 59 },  // helmet
  { 480, 59 },  // helmet
  { 365, 205 }, // left ring
  { 567, 205 }, // right ring
  { 524, 59 },  // amulet
  { 337, 104 }, // left hand
  { 366, 104 }, // left hand
  { 337, 132 }, // left hand
  { 366, 132 }, // left hand
  { 337, 160 }, // left hand
  { 366, 160 }, // left hand
  { 567, 104 }, // right hand
  { 596, 104 }, // right hand
  { 567, 132 }, // right hand
  { 596, 132 }, // right hand
  { 567, 160 }, // right hand
  { 596, 160 }, // right hand
  { 452, 104 }, // chest
  { 480, 104 }, // chest
  { 452, 132 }, // chest
  { 480, 132 }, // chest
  { 452, 160 }, // chest
  { 480, 160 }, // chest
  { 337, 250 }, // inv row 1
  { 366, 250 }, // inv row 1
  { 394, 250 }, // inv row 1
  { 423, 250 }, // inv row 1
  { 451, 250 }, // inv row 1
  { 480, 250 }, // inv row 1
  { 509, 250 }, // inv row 1
  { 538, 250 }, // inv row 1
  { 567, 250 }, // inv row 1
  { 596, 250 }, // inv row 1
  { 337, 279 }, // inv row 2
  { 366, 279 }, // inv row 2
  { 394, 279 }, // inv row 2
  { 423, 279 }, // inv row 2
  { 451, 279 }, // inv row 2
  { 480, 279 }, // inv row 2
  { 509, 279 }, // inv row 2
  { 538, 279 }, // inv row 2
  { 567, 279 }, // inv row 2
  { 596, 279 }, // inv row 2
  { 337, 308 }, // inv row 3
  { 366, 308 }, // inv row 3
  { 394, 308 }, // inv row 3
  { 423, 308 }, // inv row 3
  { 451, 308 }, // inv row 3
  { 480, 308 }, // inv row 3
  { 509, 308 }, // inv row 3
  { 538, 308 }, // inv row 3
  { 567, 308 }, // inv row 3
  { 596, 308 }, // inv row 3
  { 337, 336 }, // inv row 4
  { 366, 336 }, // inv row 4
  { 394, 336 }, // inv row 4
  { 423, 336 }, // inv row 4
  { 451, 336 }, // inv row 4
  { 480, 336 }, // inv row 4
  { 509, 336 }, // inv row 4
  { 538, 336 }, // inv row 4
  { 567, 336 }, // inv row 4
  { 596, 336 }, // inv row 4
  { 205, 385 }, // belt
  { 234, 385 }, // belt
  { 263, 385 }, // belt
  { 292, 385 }, // belt
  { 321, 385 }, // belt
  { 350, 385 }, // belt
  { 379, 385 }, // belt
  { 408, 385 }  // belt
};

/* data */

int AP2x2Tbl[10] = { 8, 28, 6, 26, 4, 24, 2, 22, 0, 20 }; // weak

void FreeInvGFX()
{
	MemFreeDbg(pInvCels);
}

void InitInv()
{
	/// ASSERT: assert(! pInvCels);

	if(plr[myplr]._pClass == PC_WARRIOR) {
		pInvCels = DiabLoad("Data\\Inv\\Inv.CEL", NULL, 'INVC');
	} else if(plr[myplr]._pClass == PC_ROGUE) {
		pInvCels = DiabLoad("Data\\Inv\\Inv_rog.CEL", NULL, 'INVC');
	} else if(plr[myplr]._pClass == PC_SORCERER) {
		pInvCels = DiabLoad("Data\\Inv\\Inv_Sor.CEL", NULL, 'INVC');
	}

	invflag = 0;
	drawsbarflag = 0;
}

void InvDrawSlotBack(int X, int Y, int W, int H)
{
	BYTE *dst;

	/// ASSERT: assert(gpBuffer);

	dst = &gpBuffer[X + PitchTbl[Y]];

#ifdef USE_ASM
	__asm {
		mov		edi, dst
		xor		edx, edx
		xor		ebx, ebx
		mov		dx, word ptr H
		mov		bx, word ptr W
	label1:
		mov		ecx, ebx
	label2:
		mov		al, [edi]
		cmp		al, PAL16_BLUE
		jb		label5
		cmp		al, PAL16_BLUE + 15
		ja		label3
		sub		al, PAL16_BLUE - PAL16_BEIGE
		jmp		label4
	label3:
		cmp		al, PAL16_GRAY
		jb		label5
		sub		al, PAL16_GRAY - PAL16_BEIGE
	label4:
		mov		[edi], al
	label5:
		inc		edi
		loop	label2
		sub		edi, 768
		sub		edi, ebx
		dec		edx
		jnz		label1
	}
#else
	int wdt, hgt;
	BYTE pix;

	for(hgt = H; hgt; hgt--, dst -= 768 + W) {
		for(wdt = W; wdt; wdt--) {
			pix = *dst;
			if(pix >= PAL16_BLUE) {
				if(pix <= PAL16_BLUE + 15)
					pix -= PAL16_BLUE - PAL16_BEIGE;
				else if(pix >= PAL16_GRAY)
					pix -= PAL16_GRAY - PAL16_BEIGE;
			}
			*dst++ = pix;
		}
	}
#endif
}

void DrawInv()
{
	int i, ii, sx, sy, cn, w, col;
	BYTE *pBuff;
	BOOL invtest[40];

	/// ASSERT: assert(gpBuffer);

	CelDecodeOnly(384, 511, pInvCels, 1, 320);

	if(plr[myplr].InvBody[0]._itype != -1) {
		InvDrawSlotBack(517, 219, 56, 56);
		cn = plr[myplr].InvBody[0]._iCurs + 12;
		w = InvItemWidth[cn];
		if(pcursinvitem == 0) {
			col = 197;
			if(plr[myplr].InvBody[0]._iMagical != 0) {
				col = 181;
			}
			if(!plr[myplr].InvBody[0]._iStatFlag) {
				col = 229;
			}
			CelDecodeClr(col, 517, 219, pCursCels, cn, w, 0, 8);
		}
		if(plr[myplr].InvBody[0]._iStatFlag) {
			/// ASSERT: assert(pCursCels);
			CelDrawHdrOnly(517, 219, pCursCels, cn, w, 0, 8);
		} else {
			CelDrawHdrLightRed(517, 219, pCursCels, cn, w, 0, 8, 1);
		}
	}
	if(plr[myplr].InvBody[1]._itype != -1) {
		InvDrawSlotBack(432, 365, 28, 28);
		cn = plr[myplr].InvBody[1]._iCurs + 12;
		w = InvItemWidth[cn];
		if(pcursinvitem == 1) {
			col = 197;
			if(plr[myplr].InvBody[1]._iMagical != 0) {
				col = 181;
			}
			if(!plr[myplr].InvBody[1]._iStatFlag) {
				col = 229;
			}
			CelDecodeClr(col, 432, 365, pCursCels, cn, w, 0, 8);
		}
		if(plr[myplr].InvBody[1]._iStatFlag) {
			/// ASSERT: assert(pCursCels);
			CelDrawHdrOnly(432, 365, pCursCels, cn, w, 0, 8);
		} else {
			CelDrawHdrLightRed(432, 365, pCursCels, cn, w, 0, 8, 1);
		}
	}
	if(plr[myplr].InvBody[2]._itype != -1) {
		InvDrawSlotBack(633, 365, 28, 28);
		cn = plr[myplr].InvBody[2]._iCurs + 12;
		w = InvItemWidth[cn];
		if(pcursinvitem == 2) {
			col = 197;
			if(plr[myplr].InvBody[2]._iMagical != 0) {
				col = 181;
			}
			if(!plr[myplr].InvBody[2]._iStatFlag) {
				col = 229;
			}
			CelDecodeClr(col, 633, 365, pCursCels, cn, w, 0, 8);
		}
		if(plr[myplr].InvBody[2]._iStatFlag) {
			/// ASSERT: assert(pCursCels);
			CelDrawHdrOnly(633, 365, pCursCels, cn, w, 0, 8);
		} else {
			CelDrawHdrLightRed(633, 365, pCursCels, cn, w, 0, 8, 1);
		}
	}
	if(plr[myplr].InvBody[3]._itype != -1) {
		InvDrawSlotBack(589, 220, 28, 28);
		cn = plr[myplr].InvBody[3]._iCurs + 12;
		w = InvItemWidth[cn];
		if(pcursinvitem == 3) {
			col = 197;
			if(plr[myplr].InvBody[3]._iMagical != 0) {
				col = 181;
			}
			if(!plr[myplr].InvBody[3]._iStatFlag) {
				col = 229;
			}
			CelDecodeClr(col, 589, 220, pCursCels, cn, w, 0, 8);
		}
		if(plr[myplr].InvBody[3]._iStatFlag) {
			/// ASSERT: assert(pCursCels);
			CelDrawHdrOnly(589, 220, pCursCels, cn, w, 0, 8);
		} else {
			CelDrawHdrLightRed(589, 220, pCursCels, cn, w, 0, 8, 1);
		}
	}
	if(plr[myplr].InvBody[4]._itype != -1) {
		InvDrawSlotBack(401, 320, 56, 84);
		cn = plr[myplr].InvBody[4]._iCurs + 12;
		w = InvItemWidth[cn];
		if(w == 28) {
			sx = 415;
		} else {
			sx = 401;
		}
		if(InvItemHeight[cn] == 84) {
			sy = 320;
		} else {
			sy = 306;
		}
		if(pcursinvitem == 4) {
			col = 197;
			if(plr[myplr].InvBody[4]._iMagical != 0) {
				col = 181;
			}
			if(!plr[myplr].InvBody[4]._iStatFlag) {
				col = 229;
			}
			CelDecodeClr(col, sx, sy, pCursCels, cn, w, 0, 8);
		}
		if(plr[myplr].InvBody[4]._iStatFlag) {
			/// ASSERT: assert(pCursCels);
			CelDrawHdrOnly(sx, sy, pCursCels, cn, w, 0, 8);
		} else {
			CelDrawHdrLightRed(sx, sy, pCursCels, cn, w, 0, 8, 1);
		}
		if(plr[myplr].InvBody[4]._iLoc == ILOC_TWOHAND) {
			InvDrawSlotBack(631, 320, 56, 84);
			light_table_index = 0;
			cel_transparency_active = 1;
			if(w == 28) {
				pBuff = &gpBuffer[SCREENXY(581, 160)];
			} else {
				pBuff = &gpBuffer[SCREENXY(567, 160)];
			}
			CelDecodeHdrLightTrans(pBuff, pCursCels, cn, w, 0, 8);
			cel_transparency_active = 0;
		}
	}
	if(plr[myplr].InvBody[5]._itype != -1) {
		InvDrawSlotBack(631, 320, 56, 84);
		cn = plr[myplr].InvBody[5]._iCurs + 12;
		w = InvItemWidth[cn];
		if(w == 28) {
			sx = 645;
		} else {
			sx = 633;
		}
		if(InvItemHeight[cn] == 84) {
			sy = 320;
		} else {
			sy = 306;
		}
		if(pcursinvitem == 5) {
			col = 197;
			if(plr[myplr].InvBody[5]._iMagical != 0) {
				col = 181;
			}
			if(!plr[myplr].InvBody[5]._iStatFlag) {
				col = 229;
			}
			CelDecodeClr(col, sx, sy, pCursCels, cn, w, 0, 8);
		}
		if(plr[myplr].InvBody[5]._iStatFlag) {
			/// ASSERT: assert(pCursCels);
			CelDrawHdrOnly(sx, sy, pCursCels, cn, w, 0, 8);
		} else {
			CelDrawHdrLightRed(sx, sy, pCursCels, cn, w, 0, 8, 1);
		}
	}
	if(plr[myplr].InvBody[6]._itype != -1) {
		InvDrawSlotBack(517, 320, 56, 84);
		cn = plr[myplr].InvBody[6]._iCurs + 12;
		w = InvItemWidth[cn];
		if(pcursinvitem == 6) {
			col = 197;
			if(plr[myplr].InvBody[6]._iMagical != 0) {
				col = 181;
			}
			if(!plr[myplr].InvBody[6]._iStatFlag) {
				col = 229;
			}
			CelDecodeClr(col, 517, 320, pCursCels, cn, w, 0, 8);
		}
		if(plr[myplr].InvBody[6]._iStatFlag) {
			/// ASSERT: assert(pCursCels);
			CelDrawHdrOnly(517, 320, pCursCels, cn, w, 0, 8);
		} else {
			CelDrawHdrLightRed(517, 320, pCursCels, cn, w, 0, 8, 1);
		}
	}

	for(i = 0; i < 40; i++) {
		invtest[i] = FALSE;
		if(plr[myplr].InvGrid[i] != 0) {
			InvDrawSlotBack(InvRect[i + 25].X + 64, InvRect[i + 25].Y + 160 - 1, 28, 28);
		}
	}
	for(i = 0; i < 40; i++) {
		if(plr[myplr].InvGrid[i] <= 0) {
			continue;
		}
		/// ASSERT: assert(!invtest[i]);
		invtest[i] = TRUE;
		/// ASSERT: assert(plr[myplr].InvGrid[i] <= plr[myplr]._pNumInv);
		ii = plr[myplr].InvGrid[i] - 1;
		cn = plr[myplr].InvList[ii]._iCurs + 12;
		w = InvItemWidth[cn];
		if(pcursinvitem == ii + 7) {
			col = 197;
			if(plr[myplr].InvList[ii]._iMagical != 0) {
				col = 181;
			}
			if(!plr[myplr].InvList[ii]._iStatFlag) {
				col = 229;
			}
			CelDecodeClr(col, InvRect[i + 25].X + 64, InvRect[i + 25].Y + 160 - 1, pCursCels, cn, w, 0, 8);
		}
		if(plr[myplr].InvList[ii]._iStatFlag) {
			/// ASSERT: assert(pCursCels);
			CelDrawHdrOnly(InvRect[i + 25].X + 64, InvRect[i + 25].Y + 160 - 1, pCursCels, cn, w, 0, 8);
		} else {
			CelDrawHdrLightRed(InvRect[i + 25].X + 64, InvRect[i + 25].Y + 160 - 1, pCursCels, cn, w, 0, 8, 1);
		}
	}
}

void DrawInvBelt()
{
	int i, cn, w, col, idx, No;
	BYTE c;

	if(talkflag) {
		return;
	}

	DrawPanelBox(205, 21, 232, 28, 269, 517);

	for(i = 0; i < 8; i++) {
		if(plr[myplr].SpdList[i]._itype == -1) {
			continue;
		}
		InvDrawSlotBack(InvRect[i + 65].X + 64, InvRect[i + 65].Y + 160 - 1, 28, 28);
		cn = plr[myplr].SpdList[i]._iCurs + 12;
		w = InvItemWidth[cn];
		if(pcursinvitem == i + 47) {
			col = 197;
			if(plr[myplr].SpdList[i]._iMagical != 0) {
				col = 181;
			}
			if(!plr[myplr].SpdList[i]._iStatFlag) {
				col = 229;
			}
			CelDecodeClr(col, InvRect[i + 65].X + 64, InvRect[i + 65].Y + 160 - 1, pCursCels, cn, w, 0, 8);
		}
		if(plr[myplr].SpdList[i]._iStatFlag) {
			/// ASSERT: assert(pCursCels);
			CelDrawHdrOnly(InvRect[i + 65].X + 64, InvRect[i + 65].Y + 160 - 1, pCursCels, cn, w, 0, 8);
		} else {
			CelDrawHdrLightRed(InvRect[i + 65].X + 64, InvRect[i + 65].Y + 160 - 1, pCursCels, cn, w, 0, 8, 1);
		}
		idx = plr[myplr].SpdList[i].IDidx;
		if(AllItemsList[idx].iUsable && plr[myplr].SpdList[i]._iStatFlag && plr[myplr].SpdList[i]._itype != ITYPE_GOLD) {
			c = fontframe[gbFontTransTbl[(BYTE)(i + 49)]];
			No = InvRect[i + 65].X + PitchTbl[InvRect[i + 65].Y + 160 - 1] + 28 + 64 - fontkern[c];
			PrintChar(No, c, COL_WHITE);
		}
	}
}

BOOL AutoPlace(int pnum, int ii, int sx, int sy, BOOL saveflag)
{
	int i, j, xx, yy;
	BOOL done;

	done = TRUE;

	yy = 10 * (ii / 10);
	if(yy < 0) {
		yy = 0;
	}
	for(j = 0; j < sy && done; j++) {
		if(yy >= 40) {
			done = FALSE;
		}
		xx = ii % 10;
		if(xx < 0) {
			xx = 0;
		}
		for(i = 0; i < sx && done; i++) {
			if(xx >= 10) {
				done = FALSE;
			} else {
				done = plr[pnum].InvGrid[xx + yy] == 0;
			}
			xx++;
		}
		yy += 10;
	}
	if(done && saveflag) {
		plr[pnum].InvList[plr[pnum]._pNumInv] = plr[pnum].HoldItem;
		plr[pnum]._pNumInv++;
		yy = 10 * (ii / 10);
		if(yy < 0) {
			yy = 0;
		}
		for(j = 0; j < sy; j++) {
			xx = ii % 10;
			if(xx < 0) {
				xx = 0;
			}
			for(i = 0; i < sx; i++) {
				if(i == 0 && j == sy - 1) {
					plr[pnum].InvGrid[xx + yy] = plr[pnum]._pNumInv;
				} else {
					plr[pnum].InvGrid[xx + yy] = -plr[pnum]._pNumInv;
				}
				xx++;
			}
			yy += 10;
		}
		CalcPlrScrolls(pnum);
	}

	return done;
}

BOOL SpecialAutoPlace(int pnum, int ii, int sx, int sy, BOOL saveflag)
{
	int i, j, xx, yy;
	BOOL done;

	done = TRUE;

	yy = 10 * (ii / 10);
	if(yy < 0) {
		yy = 0;
	}
	for(j = 0; j < sy && done; j++) {
		if(yy >= 40) {
			done = FALSE;
		}
		xx = ii % 10;
		if(xx < 0) {
			xx = 0;
		}
		for(i = 0; i < sx && done; i++) {
			if(xx >= 10) {
				done = FALSE;
			} else {
				done = plr[pnum].InvGrid[xx + yy] == 0;
			}
			xx++;
		}
		yy += 10;
	}
	if(!done) {
		if(sx > 1 || sy > 1) {
			done = FALSE;
		} else {
			for(i = 0; i < 8; i++) {
				if(plr[pnum].SpdList[i]._itype == -1) {
					done = TRUE;
					break;
				}
			}
		}
	}
	if(done && saveflag) {
		plr[pnum].InvList[plr[pnum]._pNumInv] = plr[pnum].HoldItem;
		plr[pnum]._pNumInv++;
		yy = 10 * (ii / 10);
		if(yy < 0) {
			yy = 0;
		}
		for(j = 0; j < sy; j++) {
			xx = ii % 10;
			if(xx < 0) {
				xx = 0;
			}
			for(i = 0; i < sx; i++) {
				if(i == 0 && j == sy - 1) {
					plr[pnum].InvGrid[xx + yy] = plr[pnum]._pNumInv;
				} else {
					plr[pnum].InvGrid[xx + yy] = -plr[pnum]._pNumInv;
				}
				xx++;
			}
			yy += 10;
		}
		CalcPlrScrolls(pnum);
	}

	return done;
}

BOOL GoldAutoPlace(int pnum)
{
	int i, ii, xx, yy;
	long gt;
	BOOL done;

	done = FALSE;

	for(i = 0; i < plr[pnum]._pNumInv && !done; i++) {
		if(plr[pnum].InvList[i]._itype == ITYPE_GOLD) {
			gt = plr[pnum].InvList[i]._ivalue + plr[pnum].HoldItem._ivalue;
			if(gt <= 5000) {
				plr[pnum].InvList[i]._ivalue += plr[pnum].HoldItem._ivalue;
				if(gt >= 2500) {
					plr[pnum].InvList[i]._iCurs = 6;
				} else if(gt <= 1000) {
					plr[pnum].InvList[i]._iCurs = 4;
				} else {
					plr[pnum].InvList[i]._iCurs = 5;
				}
				plr[pnum]._pGold = CalculateGold(pnum);
				done = TRUE;
			}
		}
	}
	if(!done) {
		for(i = 0; i < plr[pnum]._pNumInv && !done; i++) {
			if(plr[pnum].InvList[i]._itype == ITYPE_GOLD && plr[pnum].InvList[i]._ivalue < 5000) {
				gt = plr[pnum].InvList[i]._ivalue + plr[pnum].HoldItem._ivalue;
				if(gt <= 5000) {
					plr[pnum].InvList[i]._ivalue += plr[pnum].HoldItem._ivalue;
					if(gt >= 2500) {
						plr[pnum].InvList[i]._iCurs = 6;
					} else if(gt <= 1000) {
						plr[pnum].InvList[i]._iCurs = 4;
					} else {
						plr[pnum].InvList[i]._iCurs = 5;
					}
					plr[pnum]._pGold = CalculateGold(pnum);
					done = TRUE;
				}
			}
		}
	}
	if(!done) {
		for(ii = 39; ii >= 0 && !done; ii--) {
			yy = 10 * (ii / 10);
			xx = ii % 10;
			if(plr[pnum].InvGrid[xx + yy] == 0) {
				i = plr[pnum]._pNumInv;
				plr[pnum].InvList[i] = plr[pnum].HoldItem;
				plr[pnum]._pNumInv++;
				plr[pnum].InvGrid[xx + yy] = plr[pnum]._pNumInv;
				if(plr[pnum].HoldItem._ivalue >= 2500) {
					plr[pnum].InvList[i]._iCurs = 6;
				} else if(plr[pnum].HoldItem._ivalue <= 1000) {
					plr[pnum].InvList[i]._iCurs = 4;
				} else {
					plr[pnum].InvList[i]._iCurs = 5;
				}
				plr[pnum]._pGold = CalculateGold(pnum);
				done = TRUE;
			}
		}
	}

	return done;
}

BOOL WeaponAutoPlace(int pnum)
{
	if(plr[pnum].HoldItem._iLoc != ILOC_TWOHAND) {
		if(plr[pnum].InvBody[4]._itype != -1 && plr[pnum].InvBody[4]._iClass == ICLASS_WEAPON) {
			return FALSE;
		}
		if(plr[pnum].InvBody[5]._itype != -1 && plr[pnum].InvBody[5]._iClass == ICLASS_WEAPON) {
			return FALSE;
		}
		if(plr[pnum].InvBody[4]._itype == -1) {
			NetSendCmdChItem(TRUE, 4);
			plr[pnum].InvBody[4] = plr[pnum].HoldItem;
			return TRUE;
		}
		if(plr[pnum].InvBody[5]._itype == -1 && plr[pnum].InvBody[4]._iLoc != ILOC_TWOHAND) {
			NetSendCmdChItem(TRUE, 5);
			plr[pnum].InvBody[5] = plr[pnum].HoldItem;
			return TRUE;
		}
	} else {
		if(plr[pnum].InvBody[4]._itype == -1 && plr[pnum].InvBody[5]._itype == -1) {
			NetSendCmdChItem(TRUE, 4);
			plr[pnum].InvBody[4] = plr[pnum].HoldItem;
			return TRUE;
		}
	}

	return FALSE;
}

int SwapItem(ItemStruct *a, ItemStruct *b)
{
	ItemStruct h;

	h = *a;
	*a = *b;
	*b = h;

	return h._iCurs + 12;
}

void CheckInvPaste(int pnum, int mx, int my)
{
	int r, rx, ry, sx, sy, i, j, xx, yy, ii;
	BOOL done, done2h;
	int il, cn, it, iv, ig, idx;
	long gt;
	ItemStruct tempitem;

	SetICursor(plr[pnum].HoldItem._iCurs + 12);

	rx = mx + (icursW >> 1);
	ry = my + (icursH >> 1);
	sx = icursW28;
	sy = icursH28;
	done = FALSE;
	for(r = 0; (DWORD)r < 73 && !done; r++) {
		if(rx >= InvRect[r].X && rx < InvRect[r].X + 28
		&& ry >= InvRect[r].Y - 29 && ry < InvRect[r].Y) {
			done = TRUE;
			r--;
		}
		if(r == 24) {
			if(!(sx & 1)) {
				rx -= 14;
			}
			if(!(sy & 1)) {
				ry -= 14;
			}
		}
		if(r == 64 && !(sy & 1)) {
			ry += 14;
		}
	}
	if(!done) {
		return;
	}

	il = ILOC_UNEQUIPABLE;
	if(r >= 0 && r <= 3) {
		il = ILOC_HELM;
	}
	if(r >= 4 && r <= 5) {
		il = ILOC_RING;
	}
	if(r == 6) {
		il = ILOC_AMULET;
	}
	if(r >= 7 && r <= 18) {
		il = ILOC_ONEHAND;
	}
	if(r >= 19 && r <= 24) {
		il = ILOC_ARMOR;
	}
	if(r >= 65 && r <= 72) {
		il = ILOC_BELT;
	}

	done = FALSE;
	if(plr[pnum].HoldItem._iLoc == il) {
		done = TRUE;
	}
	if(il == ILOC_ONEHAND && plr[pnum].HoldItem._iLoc == ILOC_TWOHAND) {
		il = ILOC_TWOHAND;
		done = TRUE;
	}
	if(plr[pnum].HoldItem._iLoc == ILOC_UNEQUIPABLE && il == ILOC_BELT && sx == 1 && sy == 1) {
		done = TRUE;
		idx = plr[pnum].HoldItem.IDidx;
		if(!AllItemsList[idx].iUsable) {
			done = FALSE;
		}
		if(!plr[pnum].HoldItem._iStatFlag) {
			done = FALSE;
		}
		if(plr[pnum].HoldItem._itype == ITYPE_GOLD) {
			done = FALSE;
		}
	}
	if(il == ILOC_UNEQUIPABLE) {
		ii = r - 25;
		iv = 0;
		done = TRUE;
		if(plr[pnum].HoldItem._itype == ITYPE_GOLD) {
			yy = 10 * (ii / 10);
			xx = ii % 10;
			if(plr[pnum].InvGrid[xx + yy] != 0) {
				ig = plr[pnum].InvGrid[xx + yy];
				if(ig > 0) {
					if(plr[pnum].InvList[ig - 1]._itype != ITYPE_GOLD) {
						iv = ig;
					}
				} else {
					iv = -ig;
				}
			}
		} else {
			yy = 10 * (ii / 10 - ((sy - 1) >> 1));
			if(yy < 0) {
				yy = 0;
			}
			for(j = 0; j < sy && done; j++) {
				if(yy >= 40) {
					done = FALSE;
				}
				xx = ii % 10 - ((sx - 1) >> 1);
				if(xx < 0) {
					xx = 0;
				}
				for(i = 0; i < sx && done; i++) {
					if(xx >= 10) {
						done = FALSE;
					} else if(plr[pnum].InvGrid[xx + yy] != 0) {
						ig = plr[pnum].InvGrid[xx + yy];
						if(ig < 0) {
							ig = -ig;
						}
						if(iv != 0) {
							if(iv != ig) {
								done = FALSE;
							}
						} else {
							iv = ig;
						}
					}
					xx++;
				}
				yy += 10;
			}
		}
	}
	if(done && il != ILOC_UNEQUIPABLE && il != ILOC_BELT && !plr[pnum].HoldItem._iStatFlag) {
		done = FALSE;
		if(plr[pnum]._pClass == PC_WARRIOR) {
			PlaySFX(PS_WARR13);
		} else if(plr[pnum]._pClass == PC_ROGUE) {
			PlaySFX(PS_ROGUE13);
		} else if(plr[pnum]._pClass == PC_SORCERER) {
			PlaySFX(PS_MAGE13);
		}
	}
	if(!done) {
		return;
	}

	it = ItemCAnimTbl[plr[pnum].HoldItem._iCurs];
	if(pnum == myplr) {
		PlaySFX(ItemInvSnds[it]);
	}

	cn = CURSOR_HAND;

	switch(il) {
	case ILOC_HELM:
		NetSendCmdChItem(FALSE, 0);
		if(plr[pnum].InvBody[0]._itype == -1) {
			plr[pnum].InvBody[0] = plr[pnum].HoldItem;
		} else {
			cn = SwapItem(&plr[pnum].InvBody[0], &plr[pnum].HoldItem);
		}
		break;
	case ILOC_RING:
		if(r == 4) {
			NetSendCmdChItem(FALSE, 1);
			if(plr[pnum].InvBody[1]._itype == -1) {
				plr[pnum].InvBody[1] = plr[pnum].HoldItem;
			} else {
				cn = SwapItem(&plr[pnum].InvBody[1], &plr[pnum].HoldItem);
			}
		} else {
			NetSendCmdChItem(FALSE, 2);
			if(plr[pnum].InvBody[2]._itype == -1) {
				plr[pnum].InvBody[2] = plr[pnum].HoldItem;
			} else {
				cn = SwapItem(&plr[pnum].InvBody[2], &plr[pnum].HoldItem);
			}
		}
		break;
	case ILOC_AMULET:
		NetSendCmdChItem(FALSE, 3);
		if(plr[pnum].InvBody[3]._itype == -1) {
			plr[pnum].InvBody[3] = plr[pnum].HoldItem;
		} else {
			cn = SwapItem(&plr[pnum].InvBody[3], &plr[pnum].HoldItem);
		}
		break;
	case ILOC_ONEHAND:
		if(r <= 12) {
			if(plr[pnum].InvBody[4]._itype == -1) {
				if(plr[pnum].InvBody[5]._itype != -1 && plr[pnum].InvBody[5]._iClass == plr[pnum].HoldItem._iClass) {
					NetSendCmdChItem(FALSE, 5);
					cn = SwapItem(&plr[pnum].InvBody[5], &plr[pnum].HoldItem);
				} else {
					NetSendCmdChItem(FALSE, 4);
					plr[pnum].InvBody[4] = plr[pnum].HoldItem;
				}
			} else {
				if(plr[pnum].InvBody[5]._itype != -1 && plr[pnum].InvBody[5]._iClass == plr[pnum].HoldItem._iClass) {
					NetSendCmdChItem(FALSE, 5);
					cn = SwapItem(&plr[pnum].InvBody[5], &plr[pnum].HoldItem);
				} else {
					NetSendCmdChItem(FALSE, 4);
					cn = SwapItem(&plr[pnum].InvBody[4], &plr[pnum].HoldItem);
				}
			}
		} else {
			if(plr[pnum].InvBody[5]._itype == -1) {
				if(plr[pnum].InvBody[4]._itype != -1 && plr[pnum].InvBody[4]._iLoc == ILOC_TWOHAND) {
					NetSendCmdDelItem(FALSE, 4);
					NetSendCmdChItem(FALSE, 5);
					SwapItem(&plr[pnum].InvBody[5], &plr[pnum].InvBody[4]);
					cn = SwapItem(&plr[pnum].InvBody[5], &plr[pnum].HoldItem);
				} else if(plr[pnum].InvBody[4]._itype != -1 && plr[pnum].InvBody[4]._iClass == plr[pnum].HoldItem._iClass) {
					NetSendCmdChItem(FALSE, 4);
					cn = SwapItem(&plr[pnum].InvBody[4], &plr[pnum].HoldItem);
				} else {
					NetSendCmdChItem(FALSE, 5);
					plr[pnum].InvBody[5] = plr[pnum].HoldItem;
				}
			} else {
				if(plr[pnum].InvBody[4]._itype != -1 && plr[pnum].InvBody[4]._iClass == plr[pnum].HoldItem._iClass) {
					NetSendCmdChItem(FALSE, 4);
					cn = SwapItem(&plr[pnum].InvBody[4], &plr[pnum].HoldItem);
				} else {
					NetSendCmdChItem(FALSE, 5);
					cn = SwapItem(&plr[pnum].InvBody[5], &plr[pnum].HoldItem);
				}
			}
		}
		break;
	case ILOC_TWOHAND:
		NetSendCmdDelItem(FALSE, 5);
		if(plr[pnum].InvBody[4]._itype != -1 && plr[pnum].InvBody[5]._itype != -1) {
			tempitem = plr[pnum].HoldItem;
			if(plr[pnum].InvBody[5]._itype == ITYPE_SHIELD) {
				plr[pnum].HoldItem = plr[pnum].InvBody[5];
			} else {
				plr[pnum].HoldItem = plr[pnum].InvBody[4];
			}
			if(pnum == myplr) {
				SetCursor_(plr[pnum].HoldItem._iCurs + 12);
			} else {
				SetICursor(plr[pnum].HoldItem._iCurs + 12);
			}
			done2h = FALSE;
			for(i = 0; i < 40 && !done2h; i++) {
				done2h = AutoPlace(pnum, i, icursW28, icursH28, 1);
			}
			plr[pnum].HoldItem = tempitem;
			if(pnum == myplr) {
				SetCursor_(plr[pnum].HoldItem._iCurs + 12);
			} else {
				SetICursor(plr[pnum].HoldItem._iCurs + 12);
			}
			if(!done2h) {
				return;
			}
			if(plr[pnum].InvBody[5]._itype == ITYPE_SHIELD) {
				plr[pnum].InvBody[5]._itype = -1;
			} else {
				plr[pnum].InvBody[4]._itype = -1;
			}
		}
		if(plr[pnum].InvBody[4]._itype == -1 && plr[pnum].InvBody[5]._itype == -1) {
			NetSendCmdChItem(FALSE, 4);
			plr[pnum].InvBody[4] = plr[pnum].HoldItem;
		} else {
			NetSendCmdChItem(FALSE, 4);
			if(plr[pnum].InvBody[4]._itype == -1) {
				SwapItem(&plr[pnum].InvBody[4], &plr[pnum].InvBody[5]);
			}
			cn = SwapItem(&plr[pnum].InvBody[4], &plr[pnum].HoldItem);
		}
		if(plr[pnum].InvBody[4]._itype == ITYPE_STAFF
		&& plr[pnum].InvBody[4]._iSpell != SPL_NULL
		&& plr[pnum].InvBody[4]._iCharges > 0) {
			plr[pnum]._pRSpell = plr[pnum].InvBody[4]._iSpell;
			plr[pnum]._pRSplType = RSPLTYPE_CHARGES;
			force_redraw = 255;
		}
		break;
	case ILOC_ARMOR:
		NetSendCmdChItem(FALSE, 6);
		if(plr[pnum].InvBody[6]._itype == -1) {
			plr[pnum].InvBody[6] = plr[pnum].HoldItem;
		} else {
			cn = SwapItem(&plr[pnum].InvBody[6], &plr[pnum].HoldItem);
		}
		break;
	case ILOC_UNEQUIPABLE:
		if(plr[pnum].HoldItem._itype == ITYPE_GOLD && iv == 0) {
			ii = r - 25;
			yy = 10 * (ii / 10);
			xx = ii % 10;
			if(plr[pnum].InvGrid[xx + yy] > 0) {
				i = plr[pnum].InvGrid[xx + yy] - 1;
				gt = plr[pnum].InvList[i]._ivalue + plr[pnum].HoldItem._ivalue;
				if(gt <= 5000) {
					plr[pnum].InvList[i]._ivalue += plr[pnum].HoldItem._ivalue;
					plr[pnum]._pGold += plr[pnum].HoldItem._ivalue;
					if(gt >= 2500) {
						plr[pnum].InvList[i]._iCurs = 6;
					} else if(gt <= 1000) {
						plr[pnum].InvList[i]._iCurs = 4;
					} else {
						plr[pnum].InvList[i]._iCurs = 5;
					}
				} else {
					gt = 5000 - plr[pnum].InvList[i]._ivalue;
					plr[pnum]._pGold += gt;
					plr[pnum].HoldItem._ivalue -= gt;
					plr[pnum].InvList[i]._ivalue = 5000;
					plr[pnum].InvList[i]._iCurs = 6;
					if(plr[pnum].HoldItem._ivalue >= 2500) {
						cn = 18; /// BUGFIX: wrong cursor from beta
					} else if(plr[pnum].HoldItem._ivalue <= 1000) {
						cn = 16; /// BUGFIX: wrong cursor from beta
					} else {
						cn = 17; /// BUGFIX: wrong cursor from beta
					}
				}
			} else {
				ii = plr[pnum]._pNumInv;
				plr[pnum].InvList[ii] = plr[pnum].HoldItem;
				plr[pnum]._pNumInv++;
				plr[pnum].InvGrid[xx + yy] = plr[pnum]._pNumInv;
				plr[pnum]._pGold += plr[pnum].HoldItem._ivalue;
				if(plr[pnum].HoldItem._ivalue <= 5000) {
					if(plr[pnum].HoldItem._ivalue >= 2500) {
						plr[pnum].InvList[ii]._iCurs = 6;
					} else if(plr[pnum].HoldItem._ivalue <= 1000) {
						plr[pnum].InvList[ii]._iCurs = 4;
					} else {
						plr[pnum].InvList[ii]._iCurs = 5;
					}
				}
			}
		} else {
			if(iv == 0) {
				ii = plr[pnum]._pNumInv;
				plr[pnum].InvList[ii] = plr[pnum].HoldItem;
				plr[pnum]._pNumInv++;
				iv = plr[pnum]._pNumInv;
			} else {
				ii = iv - 1;
				if(plr[pnum].HoldItem._itype == ITYPE_GOLD) {
					plr[pnum]._pGold += plr[pnum].HoldItem._ivalue;
				}
				cn = SwapItem(&plr[pnum].InvList[ii], &plr[pnum].HoldItem);
				if(plr[pnum].HoldItem._itype == ITYPE_GOLD) {
					plr[pnum]._pGold = CalculateGold(pnum);
				}
				for(i = 0; i < 40; i++) {
					if(plr[pnum].InvGrid[i] == iv) {
						plr[pnum].InvGrid[i] = 0;
					}
					if(plr[pnum].InvGrid[i] == -iv) {
						plr[pnum].InvGrid[i] = 0;
					}
				}
			}
			ii = r - 25;
			yy = 10 * (ii / 10 - ((sy - 1) >> 1));
			if(yy < 0) {
				yy = 0;
			}
			for(j = 0; j < sy; j++) {
				xx = ii % 10 - ((sx - 1) >> 1);
				if(xx < 0) {
					xx = 0;
				}
				for(i = 0; i < sx; i++) {
					if(i == 0 && j == sy - 1) {
						plr[pnum].InvGrid[xx + yy] = iv;
					} else {
						plr[pnum].InvGrid[xx + yy] = -iv;
					}
					xx++;
				}
				yy += 10;
			}
		}
		break;
	case ILOC_BELT:
		ii = r - 65;
		if(plr[pnum].HoldItem._itype == ITYPE_GOLD) {
			if(plr[pnum].SpdList[ii]._itype != -1) {
				if(plr[pnum].SpdList[ii]._itype == ITYPE_GOLD) {
					gt = plr[pnum].SpdList[ii]._ivalue + plr[pnum].HoldItem._ivalue;
					if(gt <= 5000) {
						plr[pnum].SpdList[ii]._ivalue += plr[pnum].HoldItem._ivalue;
						plr[pnum]._pGold += plr[pnum].HoldItem._ivalue;
						if(gt >= 2500) {
							plr[pnum].SpdList[ii]._iCurs = 6;
						} else if(gt <= 1000) {
							plr[pnum].SpdList[ii]._iCurs = 4;
						} else {
							plr[pnum].SpdList[ii]._iCurs = 5;
						}
					} else {
						gt = 5000 - plr[pnum].SpdList[ii]._ivalue;
						plr[pnum]._pGold += gt;
						plr[pnum].HoldItem._ivalue -= gt;
						plr[pnum].SpdList[ii]._ivalue = 5000;
						plr[pnum].SpdList[ii]._iCurs = 6;
						if(plr[pnum].HoldItem._ivalue >= 2500) {
							cn = 18; /// BUGFIX: wrong cursor from beta
						} else if(plr[pnum].HoldItem._ivalue <= 1000) {
							cn = 16; /// BUGFIX: wrong cursor from beta
						} else {
							cn = 17; /// BUGFIX: wrong cursor from beta
						}
					}
				} else {
					plr[pnum]._pGold += plr[pnum].HoldItem._ivalue;
					cn = SwapItem(&plr[pnum].SpdList[ii], &plr[pnum].HoldItem);
				}
			} else {
				plr[pnum].SpdList[ii] = plr[pnum].HoldItem;
				plr[pnum]._pGold += plr[pnum].HoldItem._ivalue;
			}
		} else if(plr[pnum].SpdList[ii]._itype == -1) {
			plr[pnum].SpdList[ii] = plr[pnum].HoldItem;
		} else {
			cn = SwapItem(&plr[pnum].SpdList[ii], &plr[pnum].HoldItem);
			if(plr[pnum].HoldItem._itype == ITYPE_GOLD) {
				plr[pnum]._pGold = CalculateGold(pnum);
			}
		}
		drawsbarflag = 1;
		break;
	}

	CalcPlrInv(pnum, TRUE);

	if(pnum == myplr) {
		if(cn == CURSOR_HAND) {
			SetCursorPos(MouseX + (cursW >> 1), MouseY + (cursH >> 1));
		}
		SetCursor_(cn);
	}
}

void CheckInvSwap(int pnum, BYTE bLoc, int idx, WORD wCI, int seed, BOOL bId)
{
	PlayerStruct *p;

	RecreateItem(127, idx, wCI, seed, 0);

	p = &plr[pnum];
	p->HoldItem = item[127];

	if(bId) {
		p->HoldItem._iIdentified = 1;
	}
	if(bLoc < 7) {
		p->InvBody[bLoc] = p->HoldItem;
		if(bLoc == 4 && p->HoldItem._iLoc == ILOC_TWOHAND) {
			p->InvBody[5]._itype = -1;
		} else if(bLoc == 5 && p->HoldItem._iLoc == ILOC_TWOHAND) {
			p->InvBody[4]._itype = -1;
		}
	}

	CalcPlrInv(pnum, TRUE);
}

void CheckInvCut(int pnum, int mx, int my)
{
	int r, i, ii, iv;
	BOOL done;

	if(plr[pnum]._pmode > PM_WALK3) {
		return;
	}

	if(dropGoldFlag) {
		dropGoldFlag = 0;
		dropGoldValue = 0;
	}

	done = FALSE;
	for(r = 0; (DWORD)r < 73 && !done; r++) {
		if(mx >= InvRect[r].X && mx < InvRect[r].X + 29
		&& my >= InvRect[r].Y - 29 && my < InvRect[r].Y) {
			done = TRUE;
			r--;
		}
	}
	if(!done) {
		return;
	}

	plr[pnum].HoldItem._itype = -1;

	if(r >= 0 && r <= 3 && plr[pnum].InvBody[0]._itype != -1) {
		NetSendCmdDelItem(FALSE, 0);
		plr[pnum].HoldItem = plr[pnum].InvBody[0];
		plr[pnum].InvBody[0]._itype = -1;
	}
	if(r == 4 && plr[pnum].InvBody[1]._itype != -1) {
		NetSendCmdDelItem(FALSE, 1);
		plr[pnum].HoldItem = plr[pnum].InvBody[1];
		plr[pnum].InvBody[1]._itype = -1;
	}
	if(r == 5 && plr[pnum].InvBody[2]._itype != -1) {
		NetSendCmdDelItem(FALSE, 2);
		plr[pnum].HoldItem = plr[pnum].InvBody[2];
		plr[pnum].InvBody[2]._itype = -1;
	}
	if(r == 6 && plr[pnum].InvBody[3]._itype != -1) {
		NetSendCmdDelItem(FALSE, 3);
		plr[pnum].HoldItem = plr[pnum].InvBody[3];
		plr[pnum].InvBody[3]._itype = -1;
	}
	if(r >= 7 && r <= 12 && plr[pnum].InvBody[4]._itype != -1) {
		NetSendCmdDelItem(FALSE, 4);
		plr[pnum].HoldItem = plr[pnum].InvBody[4];
		plr[pnum].InvBody[4]._itype = -1;
	}
	if(r >= 13 && r <= 18 && plr[pnum].InvBody[5]._itype != -1) {
		NetSendCmdDelItem(FALSE, 5);
		plr[pnum].HoldItem = plr[pnum].InvBody[5];
		plr[pnum].InvBody[5]._itype = -1;
	}
	if(r >= 19 && r <= 24 && plr[pnum].InvBody[6]._itype != -1) {
		NetSendCmdDelItem(FALSE, 6);
		plr[pnum].HoldItem = plr[pnum].InvBody[6];
		plr[pnum].InvBody[6]._itype = -1;
	}
	if(r >= 25 && r <= 64) {
		ii = r - 25;
		if(plr[pnum].InvGrid[ii] != 0) {
			if(plr[pnum].InvGrid[ii] > 0) {
				iv = plr[pnum].InvGrid[ii];
			} else {
				iv = -plr[pnum].InvGrid[ii];
			}
			for(i = 0; i < 40; i++) {
				if(plr[pnum].InvGrid[i] == iv || plr[pnum].InvGrid[i] == -iv) {
					plr[pnum].InvGrid[i] = 0;
				}
			}
			iv--;
			plr[pnum].HoldItem = plr[pnum].InvList[iv];
			plr[pnum]._pNumInv--;
			if(plr[pnum]._pNumInv > 0 && plr[pnum]._pNumInv != iv) {
				plr[pnum].InvList[iv] = plr[pnum].InvList[plr[pnum]._pNumInv];
				for(i = 0; i < 40; i++) {
					if(plr[pnum].InvGrid[i] == plr[pnum]._pNumInv + 1) {
						plr[pnum].InvGrid[i] = iv + 1;
					}
					if(plr[pnum].InvGrid[i] == -(plr[pnum]._pNumInv + 1)) {
						plr[pnum].InvGrid[i] = -(iv + 1);
					}
				}
			}
		}
	}
	if(r >= 65) {
		ii = r - 65;
		if(plr[pnum].SpdList[ii]._itype != -1) {
			plr[pnum].HoldItem = plr[pnum].SpdList[ii];
			plr[pnum].SpdList[ii]._itype = -1;
			drawsbarflag = 1;
		}
	}

	if(plr[pnum].HoldItem._itype != -1) {
		if(plr[pnum].HoldItem._itype == ITYPE_GOLD) {
			plr[pnum]._pGold = CalculateGold(pnum);
		}
		CalcPlrInv(pnum, TRUE);
		CheckItemStats(pnum);
		if(pnum == myplr) {
			PlaySFX(IS_IGRAB);
			SetCursor_(plr[pnum].HoldItem._iCurs + 12);
			SetCursorPos(mx - (cursW >> 1), MouseY - (cursH >> 1)); /// BUGFIX: use 'MouseX' instead 'mx'?
		}
	}
}

void inv_update_rem_item(int pnum, BYTE bLoc)
{
	if(bLoc < 7) {
		plr[pnum].InvBody[bLoc]._itype = -1;
	}

	if(plr[pnum]._pmode != PM_DEATH) {
		CalcPlrInv(pnum, TRUE);
	} else {
		CalcPlrInv(pnum, FALSE);
	}
}

void RemoveInvItem(int pnum, int iv)
{
	int i;

	iv++;
	for(i = 0; i < 40; i++) {
		if(plr[pnum].InvGrid[i] == iv || plr[pnum].InvGrid[i] == -iv) {
			plr[pnum].InvGrid[i] = 0;
		}
	}

	iv--;
	plr[pnum]._pNumInv--;
	if(plr[pnum]._pNumInv > 0 && plr[pnum]._pNumInv != iv) {
		plr[pnum].InvList[iv] = plr[pnum].InvList[plr[pnum]._pNumInv];
		for(i = 0; i < 40; i++) {
			if(plr[pnum].InvGrid[i] == plr[pnum]._pNumInv + 1) {
				plr[pnum].InvGrid[i] = iv + 1;
			}
			if(plr[pnum].InvGrid[i] == -(plr[pnum]._pNumInv + 1)) {
				plr[pnum].InvGrid[i] = -(iv + 1);
			}
		}
	}

	CalcPlrScrolls(pnum);

	if(plr[pnum]._pRSplType == RSPLTYPE_SCROLL && plr[pnum]._pRSpell != -1) {
		if(!(plr[pnum]._pScrlSpells64 & 1 << (plr[pnum]._pRSpell - 1))) { /// BUGFIX: missing '__int64' cast
			plr[pnum]._pRSpell = -1;
		}
		force_redraw = 255;
	}
}

void RemoveSpdBarItem(int pnum, int iv)
{
	plr[pnum].SpdList[iv]._itype = -1;
	CalcPlrScrolls(pnum);

	if(plr[pnum]._pRSplType == RSPLTYPE_SCROLL && plr[pnum]._pRSpell != -1) {
		if(!(plr[pnum]._pScrlSpells64 & 1 << (plr[pnum]._pRSpell - 1))) { /// BUGFIX: missing '__int64' cast
			plr[pnum]._pRSpell = -1;
		}
	}

	force_redraw = 255;
}

void CheckInvItem()
{
	if(pcurs >= CURSOR_FIRSTITEM) {
		CheckInvPaste(myplr, MouseX, MouseY);
	} else {
		CheckInvCut(myplr, MouseX, MouseY);
	}
}

void CheckInvScrn()
{
	if(MouseX > 190 && MouseX < 437 && MouseY > 352 && MouseY < 385) {
		CheckInvItem();
	}
}

void CheckItemStats(int pnum)
{
	PlayerStruct *p;

	p = &plr[pnum];
	p->HoldItem._iStatFlag = 0;

	if(p->_pStrength >= p->HoldItem._iMinStr
	&& p->_pMagic >= p->HoldItem._iMinMag
	&& p->_pDexterity >= p->HoldItem._iMinDex) {
		p->HoldItem._iStatFlag = 1;
	}
}

void CheckBookLevel(int pnum)
{
	int slvl;

	if(plr[pnum].HoldItem._iMiscId != IMISC_BOOK) {
		return;
	}

	plr[pnum].HoldItem._iMinMag = spelldata[plr[pnum].HoldItem._iSpell].sMinInt;
	slvl = plr[pnum]._pSplLvl[plr[pnum].HoldItem._iSpell];
	while(slvl != 0) {
		plr[pnum].HoldItem._iMinMag += 20 * plr[pnum].HoldItem._iMinMag / 100;
		slvl--;
		if(plr[pnum].HoldItem._iMinMag + 20 * plr[pnum].HoldItem._iMinMag / 100 > 255) {
			plr[pnum].HoldItem._iMinMag = 255;
			slvl = 0;
		}
	}
}

void CheckQuestItem(int pnum)
{
	if(plr[pnum].HoldItem.IDidx == IDI_OPTAMULET) {
		quests[Q_BLIND]._qactive = 3;
	}
	if(plr[pnum].HoldItem.IDidx == IDI_MUSHROOM) {
		if(quests[Q_MUSHROOM]._qactive == 2 && quests[Q_MUSHROOM]._qvar1 == 3) { // QS_MUSHSPAWNED
			sfxdelay = 10;
			if(plr[pnum]._pClass == PC_WARRIOR) {
				sfxdnum = PS_WARR95;
			} else if(plr[pnum]._pClass == PC_ROGUE) {
				sfxdnum = PS_ROGUE95;
			} else if(plr[pnum]._pClass == PC_SORCERER) {
				sfxdnum = PS_MAGE95;
			}
			quests[Q_MUSHROOM]._qvar1 = 4; // QS_MUSHPICKED
		}
	}
	if(plr[pnum].HoldItem.IDidx == IDI_ANVIL) {
		if(quests[Q_ANVIL]._qactive == 1) {
			quests[Q_ANVIL]._qactive = 2;
			quests[Q_ANVIL]._qvar1 = 1;
		}
		if(quests[Q_ANVIL]._qlog == 1) {
			sfxdelay = 10;
			if(plr[myplr]._pClass == PC_WARRIOR) {
				sfxdnum = PS_WARR89;
			} else if(plr[myplr]._pClass == PC_ROGUE) {
				sfxdnum = PS_ROGUE89;
			} else if(plr[myplr]._pClass == PC_SORCERER) {
				sfxdnum = PS_MAGE89;
			}
		}
	}
	if(plr[pnum].HoldItem.IDidx == IDI_GLDNELIX) {
		sfxdelay = 30;
		if(plr[myplr]._pClass == PC_WARRIOR) {
			sfxdnum = PS_WARR88;
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			sfxdnum = PS_ROGUE88;
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			sfxdnum = PS_MAGE88;
		}
	}
	if(plr[pnum].HoldItem.IDidx == IDI_ROCK) {
		if(quests[Q_ROCK]._qactive == 1) {
			quests[Q_ROCK]._qactive = 2;
			quests[Q_ROCK]._qvar1 = 1;
		}
		if(quests[Q_ROCK]._qlog == 1) {
			sfxdelay = 10;
			if(plr[myplr]._pClass == PC_WARRIOR) {
				sfxdnum = PS_WARR87;
			} else if(plr[myplr]._pClass == PC_ROGUE) {
				sfxdnum = PS_ROGUE87;
			} else if(plr[myplr]._pClass == PC_SORCERER) {
				sfxdnum = PS_MAGE87;
			}
		}
	}
	if(plr[pnum].HoldItem.IDidx == IDI_ARMOFVAL) {
		quests[Q_BLOOD]._qactive = 3;
		sfxdelay = 20;
		if(plr[myplr]._pClass == PC_WARRIOR) {
			sfxdnum = PS_WARR91;
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			sfxdnum = PS_ROGUE91;
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			sfxdnum = PS_MAGE91;
		}
	}
}

void InvGetItem(int pnum, int ii)
{
	int j, jj;

	if(dropGoldFlag) {
		dropGoldFlag = 0;
		dropGoldValue = 0;
	}

	if(dItem[item[ii]._ix][item[ii]._iy] == 0) {
		return;
	}

	if(myplr == pnum && pcurs >= CURSOR_FIRSTITEM) {
		NetSendCmdPItem(TRUE, CMD_SYNCPUTITEM, plr[myplr].WorldX, plr[myplr].WorldY);
	}

	item[ii]._iCreateInfo &= ~0x8000;
	plr[pnum].HoldItem = item[ii];
	CheckQuestItem(pnum);
	CheckBookLevel(pnum);
	CheckItemStats(pnum);
	dItem[item[ii]._ix][item[ii]._iy] = 0;

	j = 0;
	while(j < numitems) {
		jj = itemactive[j];
		if(jj == ii) {
			DeleteItem(jj, j);
			j = 0;
		} else {
			j++;
		}
	}

	pcursitem = -1;
	SetCursor_(plr[pnum].HoldItem._iCurs + 12);
}

void AutoGetItem(int pnum, int ii)
{
	int i, g, w, h, idx, j, jj;
	BOOL done;

	if(dropGoldFlag) {
		dropGoldFlag = 0;
		dropGoldValue = 0;
	}

	if(ii != 127 && dItem[item[ii]._ix][item[ii]._iy] == 0) {
		return;
	}

	item[ii]._iCreateInfo &= ~0x8000;
	plr[pnum].HoldItem = item[ii]; /// BUGFIX: overwrites cursor item, allowing for belt dupe bug
	CheckQuestItem(pnum);
	CheckBookLevel(pnum);
	CheckItemStats(pnum);
	SetICursor(plr[pnum].HoldItem._iCurs + 12);

	if(plr[pnum].HoldItem._itype == ITYPE_GOLD) {
		done = GoldAutoPlace(pnum);
	} else {
		done = FALSE;
		g = plr[pnum]._pgfxnum & 0xF;
		if((g == 0 || g == 1) && plr[pnum]._pmode <= PM_WALK3) {
			if(plr[pnum].HoldItem._iStatFlag && plr[pnum].HoldItem._iClass == ICLASS_WEAPON) {
				done = WeaponAutoPlace(pnum);
				if(done) {
					CalcPlrInv(pnum, TRUE);
				}
			}
		}
		if(!done) {
			w = icursW28;
			h = icursH28;
			if(w == 1 && h == 1) {
				idx = plr[pnum].HoldItem.IDidx;
				if(plr[pnum].HoldItem._iStatFlag && AllItemsList[idx].iUsable) {
					for(i = 0; i < 8 && !done; i++) {
						if(plr[pnum].SpdList[i]._itype == -1) {
							plr[pnum].SpdList[i] = plr[pnum].HoldItem;
							CalcPlrScrolls(pnum);
							drawsbarflag = 1;
							done = TRUE;
						}
					}
				}
				for(i = 30; i <= 39 && !done; i++) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
				for(i = 20; i <= 29 && !done; i++) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
				for(i = 10; i <= 19 && !done; i++) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
				for(i = 0; i <= 9 && !done; i++) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
			}
			if(w == 1 && h == 2) {
				for(i = 29; i >= 20 && !done; i--) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
				for(i = 9; i >= 0 && !done; i--) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
				for(i = 19; i >= 10 && !done; i--) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
			}
			if(w == 1 && h == 3) {
				for(i = 0; i < 20 && !done; i++) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
			}
			if(w == 2 && h == 2) {
				for(i = 0; i < 10 && !done; i++) {
					done = AutoPlace(pnum, AP2x2Tbl[i], w, h, TRUE);
				}
				for(i = 21; i < 29 && !done; i += 2) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
				for(i = 1; i < 9 && !done; i += 2) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
				for(i = 10; i < 19 && !done; i++) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
			}
			if(w == 2 && h == 3) {
				for(i = 0; i < 9 && !done; i++) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
				for(i = 10; i < 19 && !done; i++) {
					done = AutoPlace(pnum, i, w, h, TRUE);
				}
			}
		}
	}

	if(done) {
		dItem[item[ii]._ix][item[ii]._iy] = 0;
		j = 0;
		while(j < numitems) {
			jj = itemactive[j];
			if(jj == ii) {
				DeleteItem(jj, j);
				j = 0;
			} else {
				j++;
			}
		}
	} else {
		if(pnum == myplr) {
			if(plr[pnum]._pClass == PC_WARRIOR) {
				PlaySFX(PS_WARR14 + random(0, 3));
			} else if(plr[pnum]._pClass == PC_ROGUE) {
				PlaySFX(PS_ROGUE14 + random(0, 3));
			} else if(plr[pnum]._pClass == PC_SORCERER) {
				PlaySFX(PS_MAGE14 + random(0, 3));
			}
		}
		plr[pnum].HoldItem = item[ii];
		RespawnItem(ii, TRUE);
		NetSendCmdPItem(TRUE, CMD_RESPAWNITEM, item[ii]._ix, item[ii]._iy);
		plr[pnum].HoldItem._itype = -1;
	}
}

int FindGetItem(int idx, WORD ci, int iseed)
{
	int i, ii;

	for(i = 0; i < numitems; i++) {
		ii = itemactive[i];
		if(item[ii].IDidx == idx && item[ii]._iSeed == iseed && item[ii]._iCreateInfo == ci) {
			return ii;
		}
	}

	return -1;
}

void SyncGetItem(int x, int y, int idx, WORD ci, int iseed)
{
	int ii, j, jj;

	/// BUGFIX: useless calls to 'FindGetItem' not assigning 'ii' were probably assertions
	if(dItem[x][y] != 0) {
		ii = dItem[x][y] - 1;
		if(item[ii].IDidx == idx && item[ii]._iSeed == iseed && item[ii]._iCreateInfo == ci) {
			FindGetItem(idx, ci, iseed);
		} else {
			ii = FindGetItem(idx, ci, iseed);
		}
	} else {
		ii = FindGetItem(idx, ci, iseed);
	}

	if(ii == -1) {
		return;
	}

	dItem[item[ii]._ix][item[ii]._iy] = 0;
	j = 0;
	while(j < numitems) {
		jj = itemactive[j];
		if(jj == ii) {
			DeleteItem(jj, j);
			FindGetItem(idx, ci, iseed);
			/// ASSERT: assert(FindGetItem(idx,ci,iseed) == -1);
			FindGetItem(idx, ci, iseed); /* todo replace with above */
			j = 0;
		} else {
			j++;
		}
	}

	/// ASSERT: assert(FindGetItem(idx, ci, iseed) == -1);
	FindGetItem(idx, ci, iseed); /* todo replace with above */
}

BOOL CanPut(int i, int j)
{
	int oi;

	if(dItem[i][j] != 0) {
		return FALSE;
	}
	if(nSolidTable[dPiece[i][j]]) {
		return FALSE;
	}
	if(dObject[i][j] != 0) {
		oi = dObject[i][j] > 0 ? dObject[i][j] - 1 : -(dObject[i][j] + 1);
		if(object[oi]._oSolidFlag) {
			return FALSE;
		}
	}
	if(dObject[i + 1][j + 1] > 0) {
		oi = dObject[i + 1][j + 1] - 1;
		if(object[oi]._oSelFlag) {
			return FALSE;
		}
	}
	if(dObject[i + 1][j + 1] < 0) {
		oi = -(dObject[i + 1][j + 1] + 1);
		if(object[oi]._oSelFlag) {
			return FALSE;
		}
	}
	if(dObject[i + 1][j] > 0 && dObject[i][j + 1] > 0) {
		oi = dObject[i + 1][j] - 1;
		if(object[oi]._oSelFlag) {
			oi = dObject[i][j + 1] - 1;
			if(object[oi]._oSelFlag) {
				return FALSE;
			}
		}
	}
	if(currlevel == 0) {
		if(dMonster[i][j] != 0 || dMonster[i + 1][j + 1] != 0) {
			return FALSE;
		}
	}

	return TRUE;
}

BOOL TryInvPut()
{
	int d, x, y;

	if(numitems >= MAXITEMS) {
		return FALSE;
	}

	d = GetDirection(plr[myplr].WorldX, plr[myplr].WorldY, cursmx, cursmy);
	x = plr[myplr].WorldX + offset_x[d];
	y = plr[myplr].WorldY + offset_y[d];
	if(CanPut(x, y)) {
		return TRUE;
	}
	d = (d - 1) & 7;
	x = plr[myplr].WorldX + offset_x[d];
	y = plr[myplr].WorldY + offset_y[d];
	if(CanPut(x, y)) {
		return TRUE;
	}
	d = (d + 2) & 7;
	x = plr[myplr].WorldX + offset_x[d];
	y = plr[myplr].WorldY + offset_y[d];
	if(CanPut(x, y)) {
		return TRUE;
	}

	x = plr[myplr].WorldX;
	y = plr[myplr].WorldY;
	return CanPut(x, y);
}

void DrawInvMsg(char *msg)
{
	DWORD dwTicks;

	dwTicks = GetTickCount();
	if(dwTicks - sgdwLastTime >= 5000) {
		sgdwLastTime = dwTicks;
		ErrorPlrMsg(msg);
	}
}

int InvPutItem(int pnum, int x, int y)
{
	int ii, d, dx, dy, i, j, l, xx, yy;
	BOOL done;

	if(numitems >= MAXITEMS) {
		return -1;
	}

	if(FindGetItem(plr[pnum].HoldItem.IDidx, plr[pnum].HoldItem._iCreateInfo, plr[pnum].HoldItem._iSeed) != -1) {
		DrawInvMsg("A duplicate item has been detected.	 Destroying duplicate...");
		SyncGetItem(x, y, plr[pnum].HoldItem.IDidx, plr[pnum].HoldItem._iCreateInfo, plr[pnum].HoldItem._iSeed);
	}

	d = GetDirection(plr[pnum].WorldX, plr[pnum].WorldY, x, y);
	dx = x - plr[pnum].WorldX;
	dy = y - plr[pnum].WorldY;
	if(abs(dx) > 1 || abs(dy) > 1) {
		x = plr[pnum].WorldX + offset_x[d];
		y = plr[pnum].WorldY + offset_y[d];
	}
	if(!CanPut(x, y)) {
		d = (d - 1) & 7;
		x = plr[pnum].WorldX + offset_x[d];
		y = plr[pnum].WorldY + offset_y[d];
		if(!CanPut(x, y)) {
			d = (d + 2) & 7;
			x = plr[pnum].WorldX + offset_x[d];
			y = plr[pnum].WorldY + offset_y[d];
			if(!CanPut(x, y)) {
				done = FALSE;
				for(l = 1; l < 50 && !done; l++) {
					for(j = -l; j <= l && !done; j++) {
						yy = j + plr[pnum].WorldY;
						for(i = -l; i <= l && !done; i++) {
							xx = i + plr[pnum].WorldX;
							if(CanPut(xx, yy)) {
								done = TRUE;
								x = xx;
								y = yy;
							}
						}
					}
				}
				if(!done) {
					return -1;
				}
			}
		}
	}

	/// ASSERT: assert(CanPut(x,y));
	CanPut(x, y); /* todo: replace with assert */

	ii = itemavail[0];
	dItem[x][y] = ii + 1;
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;
	item[ii] = plr[pnum].HoldItem;
	item[ii]._ix = x;
	item[ii]._iy = y;
	RespawnItem(ii, TRUE);
	numitems++;
	SetCursor_(CURSOR_HAND);

	return ii;
}

int SyncPutItem(int pnum, int x, int y, int idx, WORD icreateinfo, int iseed, BOOL Id, int dur, int mdur, int ch, int mch, int ivalue, DWORD ibuff)
{
	int ii, d, dx, dy, i, j, l, xx, yy;
	BOOL done;

#if 0
	if(dbg_flag_sync_item) {
		sprintf(tempstr, "Player %i dropping item.", pnum); /* todo: implement */
		NetSendCmdString(1 << myplr, tempstr);
	}
#endif

	if(numitems >= MAXITEMS) {
		return -1;
	}

	if(FindGetItem(idx, icreateinfo, iseed) != -1) {
		DrawInvMsg("A duplicate item has been detected from another player.");
		SyncGetItem(x, y, idx, icreateinfo, iseed);
	}

	d = GetDirection(plr[pnum].WorldX, plr[pnum].WorldY, x, y);
	dx = x - plr[pnum].WorldX;
	dy = y - plr[pnum].WorldY;
	if(abs(dx) > 1 || abs(dy) > 1) {
		x = plr[pnum].WorldX + offset_x[d];
		y = plr[pnum].WorldY + offset_y[d];
	}
	if(!CanPut(x, y)) {
		d = (d - 1) & 7;
		x = plr[pnum].WorldX + offset_x[d];
		y = plr[pnum].WorldY + offset_y[d];
		if(!CanPut(x, y)) {
			d = (d + 2) & 7;
			x = plr[pnum].WorldX + offset_x[d];
			y = plr[pnum].WorldY + offset_y[d];
			if(!CanPut(x, y)) {
				done = FALSE;
				for(l = 1; l < 50 && !done; l++) {
					for(j = -l; j <= l && !done; j++) {
						yy = j + plr[pnum].WorldY;
						for(i = -l; i <= l && !done; i++) {
							xx = i + plr[pnum].WorldX;
							if(CanPut(xx, yy)) {
								done = TRUE;
								x = xx;
								y = yy;
							}
						}
					}
				}
				if(!done) {
					return -1;
				}
			}
		}
	}

	/// ASSERT: assert(CanPut(x,y));
	CanPut(x, y); /* todo: replace with assert */

	ii = itemavail[0];
	dItem[x][y] = ii + 1;
	itemavail[0] = itemavail[MAXITEMS - numitems - 1];
	itemactive[numitems] = ii;

	if(idx == IDI_EAR) {
		RecreateEar(ii, icreateinfo, iseed, Id, dur, mdur, ch, mch, ivalue, ibuff);
	} else {
		RecreateItem(ii, idx, icreateinfo, iseed, ivalue);
		if(Id) {
			item[ii]._iIdentified = 1;
		}
		item[ii]._iDurability = dur;
		item[ii]._iMaxDur = mdur;
		item[ii]._iCharges = ch;
		item[ii]._iMaxCharges = mch;
	}

	item[ii]._ix = x;
	item[ii]._iy = y;
	RespawnItem(ii, TRUE);
	numitems++;

	return ii;
}

char CheckInvHLight()
{
	int r, nGold;
	char rv;
	ItemStruct *pi;
	PlayerStruct *p;

	for(r = 0; (DWORD)r < 73; r++) {
		if(MouseX >= InvRect[r].X && MouseX < InvRect[r].X + 29
		&& MouseY >= InvRect[r].Y - 29 && MouseY < InvRect[r].Y) {
			break;
		}
	}

	if((DWORD)r >= 73) {
		return -1;
	}

	rv = -1;
	infoclr = COL_WHITE;
	pi = NULL;
	p = &plr[myplr];
	ClearPanel();

	if(r >= 0 && r <= 3) {
		rv = 0;
		pi = &p->InvBody[0];
	} else if(r == 4) {
		rv = 1;
		pi = &p->InvBody[1];
	} else if(r == 5) {
		rv = 2;
		pi = &p->InvBody[2];
	} else if(r == 6) {
		rv = 3;
		pi = &p->InvBody[3];
	} else if(r >= 7 && r <= 12) {
		rv = 4;
		pi = &p->InvBody[4];
	} else if(r >= 13 && r <= 18) {
		pi = &p->InvBody[4];
		if(pi->_itype == -1 || pi->_iLoc != 2) {
			rv = 5;
			pi = &p->InvBody[5];
		} else {
			rv = 4;
		}
	} else if(r >= 19 && r <= 24) {
		rv = 6;
		pi = &p->InvBody[6];
	} else if(r >= 25 && r <= 64) {
		r = abs(p->InvGrid[r - 25]);
		if(r == 0) {
			return -1;
		}
		r--;
		rv = r + 7;
		pi = &p->InvList[r];
	} else if(r >= 65) {
		r -= 65;
		drawsbarflag = 1;
		pi = &p->SpdList[r];
		if(pi->_itype == -1) {
			return -1;
		}
		rv = r + 47;
	}

	/// ASSERT: assert(pi);
	if(pi->_itype == -1) {
		return -1;
	}

	if(pi->_itype == ITYPE_GOLD) {
		nGold = pi->_ivalue;
		sprintf(infostr, "%i gold %s", nGold, get_pieces_str(nGold));
	} else {
		if(pi->_iMagical == 1) {
			infoclr = COL_BLUE;
		} else if(pi->_iMagical == 2) {
			infoclr = COL_GOLD;
		}
		strcpy(infostr, pi->_iName);
		if(pi->_iIdentified) {
			strcpy(infostr, pi->_iIName);
			PrintItemDetails(pi);
		} else {
			PrintItemDur(pi);
		}
	}

	return rv;
}

void RemoveScroll(int pnum)
{
	int i;

	for(i = 0; i < plr[pnum]._pNumInv; i++) {
		if(plr[pnum].InvList[i]._itype == -1) {
			continue;
		}
		if(plr[pnum].InvList[i]._iMiscId == IMISC_SCROLL || plr[pnum].InvList[i]._iMiscId == IMISC_SCROLLT) {
			if(plr[pnum].InvList[i]._iSpell == plr[pnum]._pSpell) {
				RemoveInvItem(pnum, i);
				CalcPlrScrolls(pnum);
				return;
			}
		}
	}
	for(i = 0; i < 8; i++) {
		if(plr[pnum].SpdList[i]._itype == -1) {
			continue;
		}
		if(plr[pnum].SpdList[i]._iMiscId == IMISC_SCROLL || plr[pnum].SpdList[i]._iMiscId == IMISC_SCROLLT) {
			if(plr[pnum].SpdList[i]._iSpell == plr[pnum]._pSpell) {
				RemoveSpdBarItem(pnum, i);
				CalcPlrScrolls(pnum);
				return;
			}
		}
	}
}

BOOL UseScroll()
{
	int i;

	if(pcurs != CURSOR_HAND) {
		return FALSE;
	}
	if(leveltype == DTYPE_TOWN && !spelldata[plr[myplr]._pRSpell].sTownSpell) {
		return FALSE;
	}

	for(i = 0; i < plr[myplr]._pNumInv; i++) {
		if(plr[myplr].InvList[i]._itype == -1) {
			continue;
		}
		if(plr[myplr].InvList[i]._iMiscId == IMISC_SCROLL || plr[myplr].InvList[i]._iMiscId == IMISC_SCROLLT) {
			if(plr[myplr].InvList[i]._iSpell == plr[myplr]._pRSpell) {
				return TRUE;
			}
		}
	}
	for(i = 0; i < 8; i++) {
		if(plr[myplr].SpdList[i]._itype == -1) {
			continue;
		}
		if(plr[myplr].SpdList[i]._iMiscId == IMISC_SCROLL || plr[myplr].SpdList[i]._iMiscId == IMISC_SCROLLT) {
			if(plr[myplr].SpdList[i]._iSpell == plr[myplr]._pRSpell) {
				return TRUE;
			}
		}
	}

	return FALSE;
}

void UseStaffCharge(int pnum)
{
	if(plr[pnum].InvBody[4]._itype != -1
	&& plr[pnum].InvBody[4]._iMiscId == IMISC_STAFF
	&& plr[pnum].InvBody[4]._iSpell == plr[pnum]._pRSpell
	&& plr[pnum].InvBody[4]._iCharges > 0) {
		plr[pnum].InvBody[4]._iCharges--;
		CalcPlrStaff(pnum);
	}
}

BOOL UseStaff()
{
	if(pcurs != CURSOR_HAND) {
		return FALSE;
	}

	if(plr[myplr].InvBody[4]._itype != -1
	&& plr[myplr].InvBody[4]._iMiscId == IMISC_STAFF
	&& plr[myplr].InvBody[4]._iSpell == plr[myplr]._pRSpell
	&& plr[myplr].InvBody[4]._iCharges > 0) {
		return TRUE;
	}

	return FALSE;
}

void StartGoldDrop()
{
	initialDropGoldIndex = pcursinvitem;

	if(pcursinvitem <= 46) {
		initialDropGoldValue = plr[myplr].InvList[pcursinvitem - 7]._ivalue;
	} else {
		initialDropGoldValue = plr[myplr].SpdList[pcursinvitem - 47]._ivalue;
	}

	dropGoldFlag = 1;
	dropGoldValue = 0;

	if(talkflag) {
		control_reset_talk();
	}
}

BOOL UseInvItem(int pnum, int cii)
{
	int c, idata, it;
	ItemStruct *Item;
	BOOL speedlist;

	if(plr[pnum]._pInvincible && plr[pnum]._pHitPoints == 0 && pnum == myplr) {
		return TRUE;
	}
	if(pcurs != CURSOR_HAND) {
		return TRUE;
	}
	if(stextflag) {
		return TRUE;
	}
	if(cii <= 5) {
		return FALSE;
	}

	if(cii <= 46) {
		c = cii - 7;
		Item = &plr[pnum].InvList[c];
		speedlist = FALSE;
	} else {
		if(talkflag) {
			return TRUE;
		}
		c = cii - 47;
		Item = &plr[pnum].SpdList[c];
		speedlist = TRUE;
	}

	switch(Item->IDidx) {
	case IDI_MUSHROOM:
		sfxdelay = 10;
		if(plr[pnum]._pClass == PC_WARRIOR) {
			sfxdnum = PS_WARR95;
		} else if(plr[pnum]._pClass == PC_ROGUE) {
			sfxdnum = PS_ROGUE95;
		} else if(plr[pnum]._pClass == PC_SORCERER) {
			sfxdnum = PS_MAGE95;
		}
		return TRUE;
	case IDI_FUNGALTM:
		PlaySFX(IS_IBOOK);
		sfxdelay = 10;
		if(plr[pnum]._pClass == PC_WARRIOR) {
			sfxdnum = PS_WARR29;
		} else if(plr[pnum]._pClass == PC_ROGUE) {
			sfxdnum = PS_ROGUE29;
		} else if(plr[pnum]._pClass == PC_SORCERER) {
			sfxdnum = PS_MAGE29;
		}
		return TRUE;
	}

	idata = Item->IDidx;
	if(!AllItemsList[idata].iUsable) {
		return FALSE;
	}

	if(!Item->_iStatFlag) {
		if(plr[pnum]._pClass == PC_WARRIOR) {
			PlaySFX(PS_WARR13);
		} else if(plr[pnum]._pClass == PC_ROGUE) {
			PlaySFX(PS_ROGUE13);
		} else if(plr[pnum]._pClass == PC_SORCERER) {
			PlaySFX(PS_MAGE13);
		}
		return TRUE;
	}
	if(Item->_iMiscId == IMISC_NONE && Item->_itype == ITYPE_GOLD) {
		StartGoldDrop();
		return TRUE;
	}

	if(dropGoldFlag) {
		dropGoldFlag = 0;
		dropGoldValue = 0;
	}

	if(Item->_iMiscId == IMISC_SCROLL && currlevel == 0 && !spelldata[Item->_iSpell].sTownSpell) {
		return TRUE;
	}
	if(Item->_iMiscId == IMISC_SCROLLT && currlevel == 0 && !spelldata[Item->_iSpell].sTownSpell) {
		return TRUE;
	}

	it = ItemCAnimTbl[Item->_iCurs];
	if(Item->_iMiscId == IMISC_BOOK) {
		PlaySFX(IS_RBOOK);
	} else if(pnum == myplr) {
		PlaySFX(ItemInvSnds[it]);
	}

	UseItem(pnum, Item->_iMiscId, Item->_iSpell);

	if(speedlist) {
		RemoveSpdBarItem(pnum, c);
	} else if(plr[pnum].InvList[c]._iMiscId != IMISC_MAPOFDOOM) {
		RemoveInvItem(pnum, c);
	}

	return TRUE;
}

void DoTelekinesis()
{
	if(pcursobj != -1) {
		NetSendCmdParam1(TRUE, CMD_OPOBJT, pcursobj);
	}
	if(pcursitem != -1) {
		NetSendCmdGItem(TRUE, CMD_REQUESTAGITEM, myplr, myplr, pcursitem);
	}
	if(pcursmonst != -1 && !M_Talker(pcursmonst) && monster[pcursmonst].mtalkmsg == 0) {
		NetSendCmdParam1(TRUE, CMD_KNOCKBACK, pcursmonst);
	}

	SetCursor_(CURSOR_HAND);
}

long CalculateGold(int pnum)
{
	int i;
	long gold;

	gold = 0;

	for(i = 0; i < 8; i++) {
		if(plr[pnum].SpdList[i]._itype == ITYPE_GOLD) {
			gold += plr[pnum].SpdList[i]._ivalue;
			force_redraw = 255;
		}
	}
	for(i = 0; i < plr[pnum]._pNumInv; i++) {
		if(plr[pnum].InvList[i]._itype == ITYPE_GOLD) {
			gold += plr[pnum].InvList[i]._ivalue;
		}
	}

	return gold;
}

BOOL DropItemBeforeTrig()
{
	if(TryInvPut()) {
		NetSendCmdPItem(TRUE, CMD_PUTITEM, cursmx, cursmy);
		SetCursor_(CURSOR_HAND);
		return TRUE;
	}

	return FALSE;
}
