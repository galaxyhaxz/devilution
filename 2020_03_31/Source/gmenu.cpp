#include "diablo.h"

void *optbar_cel;
BOOLEAN mouse_in_slider; // weak
void *PentSpin_cel;
TMenuItem *sgpCurrItem;
void *BigTGold_cel;
int menu_draw_ticks; // weak
char menu_spinner_frame; // weak
void (*sgpUpdateFunc)(TMenuItem *);
TMenuItem *sgpMenuItems; // idb
void *option_cel;
void *sgpLogo;
int num_menu_items; // weak

const unsigned char lfontframe[127] =
{
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,  37,  49,  38,   0,  39,  40,  47,
   42,  43,  41,  45,  52,  44,  53,  55,  36,  27,
   28,  29,  30,  31,  32,  33,  34,  35,  51,  50,
	0,  46,   0,  54,   0,   1,   2,   3,   4,   5,
	6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
   16,  17,  18,  19,  20,  21,  22,  23,  24,  25,
   26,  42,   0,  43,   0,   0,   0,   1,   2,   3,
	4,   5,   6,   7,   8,   9,  10,  11,  12,  13,
   14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
   24,  25,  26,  20,   0,  21,   0
};
const unsigned char lfontkern[56] =
{
   18,  33,  21,  26,  28,  19,  19,  26,  25,  11,
   12,  25,  19,  34,  28,  32,  20,  32,  28,  20,
   28,  36,  35,  46,  33,  33,  24,  11,  23,  22,
   22,  21,  22,  21,  21,  21,  32,  10,  20,  36,
   31,  17,  13,  12,  13,  18,  16,  11,  20,  21,
   11,  10,  12,  11,  21,  23
};

void gmenu_draw_pause()
{
	if(currlevel != 0) {
		RedBack();
	}
	if(sgpMenuItems == NULL) {
		light_table_index = 0;
		gmenu_print_text(316, 336, "Pause");
	}
}

void gmenu_print_text(int x, int y, char *pszStr)
{
	BYTE c;

	/// ASSERT: assert(pszStr);
	while(*pszStr != '\0') {
		c = gbFontTransTbl[(BYTE)*pszStr];
		pszStr++;
		c = lfontframe[c];
		if(c != 0) {
			CelDecodeLightOnly(x, y, (BYTE *)BigTGold_cel, c, 46);
		}
		x += lfontkern[c] + 2;
	}
}

void FreeGMenu()
{
	MemFreeDbg(sgpLogo);
	MemFreeDbg(BigTGold_cel);
	MemFreeDbg(PentSpin_cel);
	MemFreeDbg(option_cel);
	MemFreeDbg(optbar_cel);
}

void gmenu_init_menu()
{
	menu_spinner_frame = 1;
	sgpMenuItems = NULL;
	sgpCurrItem = NULL;
	sgpUpdateFunc = NULL;
	num_menu_items = 0;
	mouse_in_slider = FALSE;

	/// ASSERT: assert(! sgpLogo);
	sgpLogo = DiabLoad("Data\\Diabsmal.CEL", NULL, 'MENU');
	BigTGold_cel = DiabLoad("Data\\BigTGold.CEL", NULL, 'MENU');
	PentSpin_cel = DiabLoad("Data\\PentSpin.CEL", NULL, 'MENU');
	option_cel = DiabLoad("Data\\option.CEL", NULL, 'MENU');
	optbar_cel = DiabLoad("Data\\optbar.CEL", NULL, 'MENU');
}

BOOL gmenu_is_active()
{
	return sgpMenuItems != NULL;
}

void gmenu_set_items(TMenuItem *pMenuItems, void (*fnUpdate)(TMenuItem *))
{
	TMenuItem *p;

	PauseMode = 0;
	sgpMenuItems = pMenuItems;
	mouse_in_slider = FALSE;

	sgpUpdateFunc = fnUpdate;
	if(sgpUpdateFunc != NULL) {
		sgpUpdateFunc(sgpMenuItems);
	}

	num_menu_items = 0;
	if(sgpMenuItems != NULL) {
		for(p = sgpMenuItems; p->fnMenu != NULL; p++) {
			num_menu_items++;
		}
	}

	sgpCurrItem = &sgpMenuItems[num_menu_items - 1];
	gmenu_up_down(TRUE);
}

void gmenu_up_down(BOOL down)
{
	int i;

	if(sgpCurrItem == NULL) {
		return;
	}

	mouse_in_slider = FALSE;
	i = num_menu_items;
	if(num_menu_items != 0) {
		while(i--) {
			if(down) {
				sgpCurrItem++;
				if(sgpCurrItem->fnMenu == NULL) {
					sgpCurrItem = sgpMenuItems;
				}
			} else {
				if(sgpCurrItem == &sgpMenuItems[0]) {
					sgpCurrItem = &sgpMenuItems[num_menu_items];
				}
				sgpCurrItem--;
			}
			if(sgpCurrItem->dwFlags & 0x80000000) {
				if(i != 0) {
					PlaySFX(IS_TITLEMOV);
				}
				break;
			}
		}
	}
}

void gmenu_draw()
{
	int y, nTicks;
	TMenuItem *p;

	if(sgpMenuItems == NULL) {
		return;
	}

	if(sgpUpdateFunc != NULL) {
		sgpUpdateFunc(sgpMenuItems);
	}

	CelDecodeOnly(236, 262, (BYTE *)sgpLogo, 1, 296);

	y = 320;
	p = sgpMenuItems;
	if(sgpMenuItems->fnMenu != NULL) {
		while(p->fnMenu != NULL) {
			gmenu_draw_menu_item(p, y);
			y += 45;
			p++;
		}
	}

	nTicks = GetTickCount();
	if(nTicks - menu_draw_ticks > 25) {
		menu_spinner_frame++;
		if(menu_spinner_frame == 9) {
			menu_spinner_frame = 1;
		}
		menu_draw_ticks = nTicks;
	}
}

void gmenu_draw_menu_item(TMenuItem *pItem, int y)
{
	int x, sx, yy;
	DWORD ticks, maxt, w;

	yy = y - 2; /* workaround, should be fixed */
	w = gmenu_get_lfont(pItem);

	if(pItem->dwFlags & 0x40000000) {
		sx = (w >> 1) + 80;
		CelDecodeOnly(sx, y - 10, (BYTE *)optbar_cel, 1, 287);
		ticks = pItem->dwFlags & 0xFFF;
		maxt = pItem->dwFlags & 0xFFF000;
		maxt >>= 12;
		if(maxt < 2) {
			maxt = 2;
		}
		ticks <<= 8;
		ticks /= maxt;
		gmenu_clear_buffer(sx + 2, yy - 10, ticks + 13, 28);
		sx += ticks;
		CelDecodeOnly(sx + 2, y - 12, (BYTE *)option_cel, 1, 27);
	}

	x = 320 - (w >> 1) + 64;

	if(pItem->dwFlags & 0x80000000) {
		light_table_index = 0;
	} else {
		light_table_index = 15;
	}

	gmenu_print_text(x, y, pItem->pszStr);

	if(pItem == sgpCurrItem) {
		CelDecodeOnly(x - 54, y + 1, (BYTE *)PentSpin_cel, menu_spinner_frame, 48);
		CelDecodeOnly(x + w + 4, y + 1, (BYTE *)PentSpin_cel, menu_spinner_frame, 48);
	}
}

void gmenu_clear_buffer(int x, int y, int width, int height)
{
	BYTE *p;

	/// ASSERT: assert(gpBuffer);
	p = &gpBuffer[x + PitchTbl[y]];

	while(height--) {
		memset(p, 205, width);
		p -= BUFFER_WIDTH;
	}
}

int gmenu_get_lfont(TMenuItem *pItem)
{
	BYTE c;
	int len;
	char *s;

	if(pItem->dwFlags & 0x40000000) {
		return 490;
	}

	len = 0;
	s = pItem->pszStr;
	while(*s != '\0') {
		c = gbFontTransTbl[(BYTE)*s];
		s++;
		len += lfontkern[lfontframe[c]] + 2;
	}

	len -= 2;
	return len;
}

BOOL gmenu_presskeys(int vkey)
{
	if(sgpMenuItems == NULL) {
		return FALSE;
	}

	/// ASSERT: assert(sgpCurrItem);

	switch(vkey) {
	case VK_SPACE:
		return FALSE;
	case VK_LEFT:
		gmenu_left_right(FALSE);
		break;
	case VK_RIGHT:
		gmenu_left_right(TRUE);
		break;
	case VK_UP:
		gmenu_up_down(FALSE);
		break;
	case VK_DOWN:
		gmenu_up_down(TRUE);
		break;
	case VK_RETURN:
		/// ASSERT: assert(sgpCurrItem);
		if(sgpCurrItem->dwFlags & 0x80000000) {
			PlaySFX(IS_TITLEMOV);
			sgpCurrItem->fnMenu(TRUE);
		}
		break;
	case VK_ESCAPE:
		PlaySFX(IS_TITLEMOV);
		gmenu_set_items(NULL, NULL);
		break;
	}

	return TRUE;
}

void gmenu_left_right(BOOL right)
{
	int ticks, maxt;

	if(!(sgpCurrItem->dwFlags & 0x40000000)) {
		return;
	}

	ticks = sgpCurrItem->dwFlags & 0xFFF;
	maxt = sgpCurrItem->dwFlags & 0xFFF000;
	maxt >>= 12;

	if(right) {
		if(ticks == maxt) {
			return;
		}
		ticks++;
	} else {
		if(ticks == 0) {
			return;
		}
		ticks--;
	}

	sgpCurrItem->dwFlags &= 0xFFFFF000;
	sgpCurrItem->dwFlags |= ticks;
	sgpCurrItem->fnMenu(FALSE);
}

BOOL gmenu_on_mouse_move()
{
	int maxt;
	long lOffset;

	if(!mouse_in_slider) {
		return FALSE;
	}

	/// ASSERT: assert(sgpCurrItem);

	gmenu_get_mouse_slider(&lOffset);
	maxt = sgpCurrItem->dwFlags & 0xFFF000;
	maxt >>= 12;
	lOffset *= maxt;
	lOffset /= 256;
	sgpCurrItem->dwFlags &= 0xFFFFF000;
	sgpCurrItem->dwFlags |= lOffset;
	sgpCurrItem->fnMenu(FALSE);
	return TRUE;
}

BOOLEAN gmenu_get_mouse_slider(long *plOffset)
{
	/// ASSERT: assert(plOffset);
	*plOffset = 282;

	if(MouseX < *plOffset) {
		*plOffset = 0;
		return FALSE;
	}
	if(MouseX > *plOffset + 256) {
		*plOffset = 256;
		return FALSE;
	}

	*plOffset = MouseX - *plOffset;
	return TRUE;
}

BOOL gmenu_left_mouse(BOOL down)
{
	int my;
	DWORD w;
	long lOffset;
	TMenuItem *pItem;

	if(!down) {
		if(!mouse_in_slider) {
			return FALSE;
		}
		mouse_in_slider = FALSE;
		return TRUE;
	}

	if(sgpMenuItems == NULL) {
		return FALSE;
	}
	if(MouseY >= 352) {
		return FALSE;
	}

	my = MouseY - 117;
	if(my < 0) {
		return TRUE;
	}
	my /= 45;
	if((DWORD)my >= num_menu_items) {
		return TRUE;
	}

	pItem = &sgpMenuItems[my];
	if(!(pItem->dwFlags & 0x80000000)) {
		return TRUE;
	}

	w = gmenu_get_lfont(pItem);
	if(MouseX < 320 - (w >> 1)) {
		return TRUE;
	}
	if(MouseX > 320 + (w >> 1)) {
		return TRUE;
	}

	sgpCurrItem = pItem;
	PlaySFX(IS_TITLEMOV);
	if(pItem->dwFlags & 0x40000000) {
		mouse_in_slider = gmenu_get_mouse_slider(&lOffset);
		gmenu_on_mouse_move();
	} else {
		sgpCurrItem->fnMenu(TRUE);
	}

	return TRUE;
}

void gmenu_enable(TMenuItem *pMenuItem, BOOL enable)
{
	/// ASSERT: assert(pMenuItem);

	if(enable) {
		pMenuItem->dwFlags |= 0x80000000;
	} else {
		pMenuItem->dwFlags &= ~0x80000000;
	}
}

void gmenu_set_slider_ticks(TMenuItem *pItem, int min, int max, int ticks)
{
	int maxt;

	/// ASSERT: assert(pItem);
	maxt = pItem->dwFlags & 0xFFF000;
	maxt >>= 12;
	if(maxt < 2) {
		maxt = 2;
	}
	ticks -= min;
	ticks *= maxt;
	ticks += (max - min - 1) / 2;
	ticks /= max - min;
	pItem->dwFlags &= 0xFFFFF000;
	pItem->dwFlags |= ticks;
}

int gmenu_get_slider_ticks(TMenuItem *pItem, int min, int max)
{
	int ticks, maxt;

	/// ASSERT: assert(pItem);
	ticks = pItem->dwFlags & 0xFFF;
	maxt = pItem->dwFlags & 0xFFF000;
	maxt >>= 12;
	if(maxt < 2) {
		maxt = 2;
	}
	ticks *= max - min;
	ticks += (maxt - 1) / 2;
	ticks /= maxt;
	ticks += min;
	return ticks;
}

void gmenu_set_max_ticks(TMenuItem *pItem, DWORD dwTicks)
{
	/// ASSERT: assert(pItem);
	/// ASSERT: assert(dwTicks >= MIN_SLIDER_TICKS && dwTicks <= MAX_SLIDER_TICKS);
	pItem->dwFlags &= 0xFF000FFF;
	pItem->dwFlags |= (dwTicks << 12) & 0xFFF000;
}
