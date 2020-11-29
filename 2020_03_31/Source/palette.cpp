#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

PALETTEENTRY logical_palette[256];
PALETTEENTRY system_palette[256];
PALETTEENTRY orig_palette[256];
int gdwPalEntries;

const char szKeyGamma[] = "Gamma Correction";
const char szKeyCycle[] = "Color Cycling";

/* data */

int gamma_correction = 100; // idb
BOOL color_cycling_enabled = TRUE; // idb
BOOLEAN sgbFadedIn = TRUE;

static void palette_update()
{
	int first, count;

	if(lpDDPalette == NULL) {
		return;
	}

	first = 0;
	count = 256;
	if(!fullscreen) {
		first = gdwPalEntries;
		count = 256 - 2 * gdwPalEntries;
	}
	SDrawUpdatePalette(first, count, &system_palette[first], 0);
}

static void ApplyGamma(PALETTEENTRY *dst, PALETTEENTRY *src, int n)
{
	int i;
	double g;

	g = gamma_correction / 100.0;

	for(i = 0; i < n; i++) {
		dst->peRed = pow(src->peRed / 256.0, g) * 256.0;
		dst->peGreen = pow(src->peGreen / 256.0, g) * 256.0;
		dst->peBlue = pow(src->peBlue / 256.0, g) * 256.0;
		dst++;
		src++;
	}
}

void SaveGamma()
{
	SRegSaveValue("Diablo", szKeyGamma, 0, gamma_correction);
	SRegSaveValue("Diablo", szKeyCycle, 0, color_cycling_enabled);
}

static void LoadGamma()
{
	DWORD val;

	val = gamma_correction;
	if(!SRegLoadValue("Diablo", szKeyGamma, 0, &val)) {
		val = 100;
	}
	gamma_correction = val;

	if(gamma_correction < 30) {
		gamma_correction = 30;
	} else if(gamma_correction > 100) {
		gamma_correction = 100;
	}
	gamma_correction -= gamma_correction % 5;

	if(!SRegLoadValue("Diablo", szKeyCycle, 0, &val)) {
		val = TRUE;
	}
	color_cycling_enabled = val;
}

static void LoadSysPal()
{
	int i;
	HDC hDC;

	for(i = 0; i < 256; i++) {
		system_palette[i].peFlags = PC_NOCOLLAPSE | PC_RESERVED;
	}

	if(fullscreen) {
		return;
	}

	hDC = GetDC(NULL);
	gdwPalEntries = GetDeviceCaps(hDC, NUMRESERVED) / 2;
	GetSystemPaletteEntries(hDC, 0, gdwPalEntries, system_palette);
	for(i = 0; i < gdwPalEntries; i++) {
		system_palette[i].peFlags = 0;
	}
	i = 256 - gdwPalEntries;
	GetSystemPaletteEntries(hDC, i, gdwPalEntries, &system_palette[i]);
	while(i < 256) {
		system_palette[i++].peFlags = 0;
	}
	ReleaseDC(NULL, hDC);
}

void palette_init()
{
	HRESULT hDDVal;

	LoadGamma();
	memcpy(system_palette, orig_palette, sizeof(orig_palette));
	LoadSysPal();

	hDDVal = lpDDInterface->CreatePalette(DDPCAPS_ALLOW256 | DDPCAPS_8BIT, system_palette, &lpDDPalette, NULL);
	if(hDDVal != DD_OK) {
		ErrDlg(IDD_DIALOG8, hDDVal, "C:\\Src\\Diablo\\Source\\PALETTE.CPP", 143);
	}

	hDDVal = lpDDSPrimary->SetPalette(lpDDPalette);
#ifndef RGBMODE
	if(hDDVal != DD_OK) {
		ErrDlg(IDD_DIALOG8, hDDVal, "C:\\Src\\Diablo\\Source\\PALETTE.CPP", 146);
	}
#endif
}

void LoadPalette(const char *pszFileName)
{
	int i;
	void *pBuf;
	BYTE PalData[256][3];

	/// ASSERT: assert(pszFileName);

	WOpenFile((char *)pszFileName, &pBuf, 0);
	WReadFile(pBuf, (char *)PalData, sizeof(PalData));
	WCloseFile(pBuf);

	for(i = 0; i < 256; i++) {
		orig_palette[i].peRed = PalData[i][0];
		orig_palette[i].peGreen = PalData[i][1];
		orig_palette[i].peBlue = PalData[i][2];
		orig_palette[i].peFlags = 0;
	}
}

void LoadRndLvlPal(int l)
{
	char szFileName[MAX_PATH];

	if(l == DTYPE_TOWN) {
		LoadPalette("Levels\\TownData\\Town.pal");
	} else {
		sprintf(szFileName, "Levels\\L%iData\\L%i_%i.PAL", l, l, random(0, 4) + 1);
		LoadPalette(szFileName);
	}
}

void ResetPal()
{
	if(lpDDSPrimary != NULL) {
		if(lpDDSPrimary->IsLost() == DDERR_SURFACELOST) {
			if(lpDDSPrimary->Restore() != DD_OK) {
				return;
			}
		}
	}

	SDrawRealizePalette();
}

void IncreaseGamma()
{
	if(gamma_correction >= 100) {
		return;
	}

	gamma_correction += 5;
	if(gamma_correction > 100) {
		gamma_correction = 100;
	}

	ApplyGamma(system_palette, logical_palette, 256);
	palette_update();
}

void DecreaseGamma()
{
	if(gamma_correction <= 30) {
		return;
	}

	gamma_correction -= 5;
	if(gamma_correction < 30) {
		gamma_correction = 30;
	}

	ApplyGamma(system_palette, logical_palette, 256);
	palette_update();
}

int UpdateGamma(int gamma)
{
	if(gamma != 0) {
		gamma_correction = 100 - gamma + 30;
		ApplyGamma(system_palette, logical_palette, 256);
		palette_update();
	}

	return 100 - gamma_correction + 30;
}

static void SetFadeLevel(int fadeval)
{
	int i, nval;

	if(lpDDInterface == NULL) {
		return;
	}

	for(i = 0; i < 255; i++) {
		nval = fadeval * logical_palette[i].peRed;
		system_palette[i].peRed = (DWORD)nval >> 8;
		nval = fadeval * logical_palette[i].peGreen;
		system_palette[i].peGreen = (DWORD)nval >> 8;
		nval = fadeval * logical_palette[i].peBlue;
		system_palette[i].peBlue = (DWORD)nval >> 8;
	}

	Sleep(3);
	lpDDInterface->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, NULL);
	palette_update();
}

void BlackPalette()
{
	SetFadeLevel(0);
}

void PaletteFadeIn(int fr)
{
	int i;

	ApplyGamma(logical_palette, orig_palette, 256);

	for(i = 0; i < 256; i += fr) {
		SetFadeLevel(i);
	}

	SetFadeLevel(256);
	memcpy(logical_palette, orig_palette, sizeof(orig_palette));
	sgbFadedIn = TRUE;
}

void PaletteFadeOut(int fr)
{
	int i;

	if(!sgbFadedIn) {
		return;
	}

	for(i = 256; i > 0; i -= fr) {
		SetFadeLevel(i);
	}

	SetFadeLevel(0);
	sgbFadedIn = FALSE;
}

void palette_update_caves()
{
	int i;
	PALETTEENTRY col;

	col = system_palette[1];
	for(i = 1; i < 31; i++) {
		system_palette[i].peRed = system_palette[i + 1].peRed;
		system_palette[i].peGreen = system_palette[i + 1].peGreen;
		system_palette[i].peBlue = system_palette[i + 1].peBlue;
	}
	system_palette[i].peRed = col.peRed;
	system_palette[i].peGreen = col.peGreen;
	system_palette[i].peBlue = col.peBlue;

	palette_update();
}

void Lava2Water(int n)
{
	int i;

	for(i = 32 - n; i >= 0; i--) {
		logical_palette[i] = orig_palette[i];
	}

	ApplyGamma(system_palette, logical_palette, 32);
	palette_update();
}

BOOL palette_get_colour_cycling()
{
	return color_cycling_enabled;
}

BOOL palette_set_color_cycling(BOOL bEnable)
{
	color_cycling_enabled = bEnable;

	return color_cycling_enabled;
}
