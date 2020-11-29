#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

void *sgpBackBuf;
IDirectDraw *lpDDInterface;
IDirectDrawPalette *lpDDPalette; // idb
int sgdwLockCount;
BYTE *gpBuffer;
IDirectDrawSurface *lpDDSBackBuf;
IDirectDrawSurface *lpDDSPrimary;
#ifdef _DEBUG
int locktbl[256];
#endif
static CCritSect sgMemCrit;
char gbBackBuf; // weak
char gbEmulate; // weak
HINSTANCE sghInstDD; // idb

static void dx_create_back_buffer()
{
	HRESULT hDDVal;
	DDSURFACEDESC ddsd;
	DDSCAPS caps;

	/// ASSERT: assert(! gpBuffer);
	/// ASSERT: assert(! sgdwLockCount);
	/// ASSERT: assert(! sgpBackBuf);
	/// ASSERT: assert(lpDDSPrimary);

	hDDVal = lpDDSPrimary->GetCaps(&caps);
	if(hDDVal != DD_OK) {
		DDErrMsg(hDDVal, 59, "C:\\Src\\Diablo\\Source\\dx.cpp");
	}
	/// ASSERT: assert(caps.dwCaps & DDSCAPS_PRIMARYSURFACE);

	if(!gbBackBuf) {
		ddsd.dwSize = sizeof(ddsd);
		hDDVal = lpDDSPrimary->Lock(NULL, &ddsd, DDLOCK_WRITEONLY | DDLOCK_WAIT, NULL);
		if(hDDVal == DD_OK) {
			hDDVal = lpDDSPrimary->Unlock(NULL);
#ifdef _DEBUG
			if(hDDVal != DD_OK) {
				DDErrMsg(hDDVal, 69, "C:\\Diablo\\Direct\\dx.cpp");
			}
#endif
			sgpBackBuf = DiabloAllocPtr(BUFFER_WIDTH * BUFFER_HEIGHT);
			return;
		}
		if(hDDVal != DDERR_CANTLOCKSURFACE) {
			ErrDlg(IDD_DIALOG1, hDDVal, "C:\\Src\\Diablo\\Source\\dx.cpp", 81);
		}
	}

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_PIXELFORMAT | DDSD_PITCH | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = BUFFER_WIDTH;
	ddsd.dwHeight = BUFFER_HEIGHT;
	ddsd.lPitch = BUFFER_WIDTH;
	ddsd.ddpfPixelFormat.dwSize = sizeof(ddsd.ddpfPixelFormat);

	hDDVal = lpDDSPrimary->GetPixelFormat(&ddsd.ddpfPixelFormat);
	if(hDDVal != DD_OK) {
		ErrDlg(IDD_DIALOG1, hDDVal, "C:\\Src\\Diablo\\Source\\dx.cpp", 94);
	}
	hDDVal = lpDDInterface->CreateSurface(&ddsd, &lpDDSBackBuf, NULL);
	if(hDDVal != DD_OK) {
		ErrDlg(IDD_DIALOG1, hDDVal, "C:\\Src\\Diablo\\Source\\dx.cpp", 96);
	}
}

static void dx_create_primary_surface()
{
	HRESULT hDDVal;
	DDSURFACEDESC ddsd;

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	hDDVal = lpDDInterface->CreateSurface(&ddsd, &lpDDSPrimary, NULL);
	if(hDDVal != DD_OK) {
		ErrDlg(IDD_DIALOG1, hDDVal, "C:\\Src\\Diablo\\Source\\dx.cpp", 109);
	}
}

static HRESULT dx_DirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter)
{
	DDCREATEPROC ddcp;

	if(sghInstDD == NULL) {
		sghInstDD = LoadLibrary("ddraw.dll");
		if(sghInstDD == NULL) {
			ErrDlg(IDD_DIALOG4, GetLastError(), "C:\\Src\\Diablo\\Source\\dx.cpp", 122);
		}
	}
	ddcp = (DDCREATEPROC)GetProcAddress(sghInstDD, "DirectDrawCreate");
	if(ddcp == NULL) {
		ErrDlg(IDD_DIALOG4, GetLastError(), "C:\\Src\\Diablo\\Source\\dx.cpp", 127);
	}

	return ddcp(lpGUID, lplpDD, pUnkOuter);
}

void dx_init(HWND hWnd)
{
	HRESULT hDDVal;
	int winw, winh;
	BOOL bSuccess;
	GUID *lpGUID;

	/// ASSERT: assert(! gpBuffer);
	/// ASSERT: assert(! sgdwLockCount);
	/// ASSERT: assert(! sgpBackBuf);

	SetFocus(hWnd);
	ShowWindow(hWnd, SW_SHOWNORMAL);

	lpGUID = NULL;
	if(gbEmulate) {
		lpGUID = (GUID *)DDCREATE_EMULATIONONLY;
	}
	hDDVal = dx_DirectDrawCreate(lpGUID, &lpDDInterface, NULL);
	if(hDDVal != DD_OK) {
		ErrDlg(IDD_DIALOG1, hDDVal, "C:\\Src\\Diablo\\Source\\dx.cpp", 149);
	}

#ifdef COLORFIX
#ifdef __DDRAWI_INCLUDED__
	((LPDDRAWI_DIRECTDRAW_INT)lpDDInterface)->lpLcl->dwAppHackFlags |= 0x800;
#else
	((DWORD **)lpDDInterface)[1][18] |= 0x800;
#endif
#endif

#ifndef _DEBUG
	fullscreen = TRUE;
#endif
	if(!fullscreen) {
		hDDVal = lpDDInterface->SetCooperativeLevel(hWnd, DDSCL_NORMAL | DDSCL_ALLOWREBOOT);
		if(hDDVal == DDERR_EXCLUSIVEMODEALREADYSET) {
			TriggerBreak();
		} else if(hDDVal != DD_OK) {
			ErrDlg(IDD_DIALOG1, hDDVal, "C:\\Diablo\\Direct\\dx.cpp", 155);
		}
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	} else {
		hDDVal = lpDDInterface->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT | DDSCL_FULLSCREEN);
		if(hDDVal == DDERR_EXCLUSIVEMODEALREADYSET) {
			TriggerBreak();
		} else if(hDDVal != DD_OK) {
			ErrDlg(IDD_DIALOG1, hDDVal, "C:\\Src\\Diablo\\Source\\dx.cpp", 170);
		}
		hDDVal = lpDDInterface->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
		if(hDDVal != DD_OK) {
			winw = GetSystemMetrics(SM_CXSCREEN);
			winh = GetSystemMetrics(SM_CYSCREEN);
			hDDVal = lpDDInterface->SetDisplayMode(winw, winh, SCREEN_BPP);
		}
		if(hDDVal != DD_OK) {
			ErrDlg(IDD_DIALOG1, hDDVal, "C:\\Src\\Diablo\\Source\\dx.cpp", 183);
		}
	}

	dx_create_primary_surface();
	palette_init();
	GdiSetBatchLimit(1);
	dx_create_back_buffer();
	bSuccess = SDrawManualInitialize(hWnd, lpDDInterface, lpDDSPrimary, NULL, NULL, lpDDSBackBuf, lpDDPalette, NULL);
	/// ASSERT: assert(bSuccess);
}

static void lock_buf_priv()
{
	HRESULT hDDVal;
	DDSURFACEDESC ddsd;

	sgMemCrit.Enter();

	if(sgpBackBuf != NULL) {
		gpBuffer = (BYTE *)sgpBackBuf;
	} else if(lpDDSBackBuf == NULL) {
		Sleep(20000);
		app_fatal("lock_buf_priv");
	} else if(sgdwLockCount == 0) {
		ddsd.dwSize = sizeof(ddsd);
		hDDVal = lpDDSBackBuf->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
		if(hDDVal != DD_OK) {
			DDErrMsg(hDDVal, 235, "C:\\Src\\Diablo\\Source\\dx.cpp");
		}
		gpBuffer = (BYTE *)ddsd.lpSurface;
		/// ASSERT: assert(gpBuffer);
		gpBufEnd += (DWORD)gpBuffer;
	}

	sgdwLockCount++;
}

void lock_buf(BYTE idx)
{
#ifdef _DEBUG
	locktbl[idx]++;
#endif
	lock_buf_priv();
}

static void unlock_buf_priv()
{
	HRESULT hDDVal;

	if(sgdwLockCount == 0) {
		app_fatal("draw main unlock error");
	}
	if(gpBuffer == NULL) {
		app_fatal("draw consistency error");
	}
	if(sgpBackBuf == NULL) {
		/// ASSERT: assert(lpDDSBackBuf);
	}

	sgdwLockCount--;

	if(sgdwLockCount == 0) {
		gpBufEnd -= (DWORD)gpBuffer;
		gpBuffer = NULL;
		if(sgpBackBuf == NULL) {
			hDDVal = lpDDSBackBuf->Unlock(NULL);
			if(hDDVal != DD_OK) {
				DDErrMsg(hDDVal, 273, "C:\\Src\\Diablo\\Source\\dx.cpp");
			}
		}
	}

	sgMemCrit.Leave();
}

void unlock_buf(BYTE idx)
{
#ifdef _DEBUG
	if(locktbl[idx] == 0) {
		app_fatal("Draw lock underflow: 0x%x", idx);
	}
	locktbl[idx]--;
#endif
	unlock_buf_priv();
}

void dx_cleanup()
{
	if(ghMainWnd != NULL) {
		ShowWindow(ghMainWnd, SW_HIDE);
	}

	SDrawDestroy();
	sgMemCrit.Enter();

	if(sgpBackBuf != NULL) {
		/// ASSERT: assert(! lpDDSBackBuf);
		MemFreeDbg(sgpBackBuf);
	} else if(lpDDSBackBuf != NULL) {
		lpDDSBackBuf->Release();
		lpDDSBackBuf = NULL;
	}

	sgdwLockCount = 0;
	gpBuffer = NULL;

	sgMemCrit.Leave();

	if(lpDDSPrimary != NULL) {
		lpDDSPrimary->Release();
		lpDDSPrimary = NULL;
	}
	if(lpDDPalette != NULL) {
		lpDDPalette->Release();
		lpDDPalette = NULL;
	}
	if(lpDDInterface != NULL) {
		lpDDInterface->Release();
		lpDDInterface = NULL;
	}
}

void dx_reinit()
{
	DWORD lock;

	sgMemCrit.Enter();

	/// ASSERT: assert(ghMainWnd);
	ClearCursor();

	lock = sgdwLockCount;
	while(sgdwLockCount != 0) {
		unlock_buf_priv();
	}

	dx_cleanup();
	force_redraw = 255;
	dx_init(ghMainWnd);

	while(lock != 0) {
		lock_buf_priv();
		lock--;
	}

	sgMemCrit.Leave();
}
