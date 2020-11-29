#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

char sz_error_buf[256];
int terminating; // weak
int cleanup_thread_id; // weak

void TriggerBreak()
{
#ifdef _DEBUG
	LPTOP_LEVEL_EXCEPTION_FILTER pFilter;

	pFilter = SetUnhandledExceptionFilter(BreakFilter);
#ifdef USE_ASM
	__asm {
		int		3
	}
#else
	__debugbreak();
#endif
	SetUnhandledExceptionFilter(pFilter);
#endif
}

#ifdef _DEBUG
LONG __stdcall BreakFilter(PEXCEPTION_POINTERS pExc)
{
	if(pExc->ExceptionRecord == NULL) {
		return 0;
	}
	if(pExc->ExceptionRecord->ExceptionCode != EXCEPTION_BREAKPOINT) {
		return 0;
	}

	if(((BYTE *)pExc->ContextRecord->Eip)[0] == 0xCC) { // int 3
		pExc->ContextRecord->Eip++;
	}

	return -1;
}
#endif

char *GetErrorStr(int error_code)
{
	int v1; // edi
	unsigned int v2; // eax
	signed int v4; // eax
	char *i; // ecx

	v1 = error_code;
	v2 = ((unsigned int)error_code >> 16) & 0x1FFF;
	if ( v2 == 0x0878 )
	{
		TraceErrorDS(error_code, sz_error_buf, 256);
	}
	else if ( v2 == 0x0876 )
	{
		TraceErrorDD(error_code, sz_error_buf, 256);
	}
	else
	{
		if ( !SErrGetErrorStr(error_code, sz_error_buf, 256) && !FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, v1, 0x400u, sz_error_buf, 0x100u, NULL) )
			wsprintf(sz_error_buf, "unknown error 0x%08x", v1);
	}
	v4 = strlen(sz_error_buf);
	for ( i = &sz_error_buf[v4-1]; v4 > 0; *i = 0 )
	{
		--v4;
		--i;
		if ( *i != '\r' && *i != '\n' )
			break;
	}
	return sz_error_buf;
}

void TraceErrorDD(HRESULT hError, char *pszBuffer, DWORD dwMaxChars)
{
	const char *szError;

	switch(hError) {
	case DDERR_CANTPAGEUNLOCK:
		szError = "DDERR_CANTPAGEUNLOCK";
		break;
	case DDERR_NOTPAGELOCKED:
		szError = "DDERR_NOTPAGELOCKED";
		break;
	case DD_OK:
		szError = "DD_OK";
		break;
	case DDERR_CANTPAGELOCK:
		szError = "DDERR_CANTPAGELOCK";
		break;
	case DDERR_BLTFASTCANTCLIP:
		szError = "DDERR_BLTFASTCANTCLIP";
		break;
	case DDERR_NOBLTHW:
		szError = "DDERR_NOBLTHW";
		break;
	case DDERR_NODDROPSHW:
		szError = "DDERR_NODDROPSHW";
		break;
	case DDERR_OVERLAYNOTVISIBLE:
		szError = "DDERR_OVERLAYNOTVISIBLE";
		break;
	case DDERR_NOOVERLAYDEST:
		szError = "DDERR_NOOVERLAYDEST";
		break;
	case DDERR_INVALIDPOSITION:
		szError = "DDERR_INVALIDPOSITION";
		break;
	case DDERR_NOTAOVERLAYSURFACE:
		szError = "DDERR_NOTAOVERLAYSURFACE";
		break;
	case DDERR_EXCLUSIVEMODEALREADYSET:
		szError = "DDERR_EXCLUSIVEMODEALREADYSET";
		break;
	case DDERR_NOTFLIPPABLE:
		szError = "DDERR_NOTFLIPPABLE";
		break;
	case DDERR_CANTDUPLICATE:
		szError = "DDERR_CANTDUPLICATE";
		break;
	case DDERR_NOTLOCKED:
		szError = "DDERR_NOTLOCKED";
		break;
	case DDERR_CANTCREATEDC:
		szError = "DDERR_CANTCREATEDC";
		break;
	case DDERR_NODC:
		szError = "DDERR_NODC";
		break;
	case DDERR_WRONGMODE:
		szError = "DDERR_WRONGMODE";
		break;
	case DDERR_IMPLICITLYCREATED:
		szError = "DDERR_IMPLICITLYCREATED";
		break;
	case DDERR_NOTPALETTIZED:
		szError = "DDERR_NOTPALETTIZED";
		break;
	case DDERR_NOMIPMAPHW:
		szError = "DDERR_NOMIPMAPHW";
		break;
	case DDERR_INVALIDSURFACETYPE:
		szError = "DDERR_INVALIDSURFACETYPE";
		break;
	case DDERR_DCALREADYCREATED:
		szError = "DDERR_DCALREADYCREATED";
		break;
	case DDERR_NOPALETTEHW:
		szError = "DDERR_NOPALETTEHW";
		break;
	case DDERR_DIRECTDRAWALREADYCREATED:
		szError = "DDERR_DIRECTDRAWALREADYCREATED";
		break;
	case DDERR_NODIRECTDRAWHW:
		szError = "DDERR_NODIRECTDRAWHW";
		break;
	case DDERR_PRIMARYSURFACEALREADYEXISTS:
		szError = "DDERR_PRIMARYSURFACEALREADYEXISTS";
		break;
	case DDERR_NOEMULATION:
		szError = "DDERR_NOEMULATION";
		break;
	case DDERR_REGIONTOOSMALL:
		szError = "DDERR_REGIONTOOSMALL";
		break;
	case DDERR_CLIPPERISUSINGHWND:
		szError = "DDERR_CLIPPERISUSINGHWND";
		break;
	case DDERR_NOCLIPPERATTACHED:
		szError = "DDERR_NOCLIPPERATTACHED";
		break;
	case DDERR_NOHWND:
		szError = "DDERR_NOHWND";
		break;
	case DDERR_HWNDSUBCLASSED:
		szError = "DDERR_HWNDSUBCLASSED";
		break;
	case DDERR_HWNDALREADYSET:
		szError = "DDERR_HWNDALREADYSET";
		break;
	case DDERR_NOPALETTEATTACHED:
		szError = "DDERR_NOPALETTEATTACHED";
		break;
	case DDERR_INVALIDDIRECTDRAWGUID:
		szError = "DDERR_INVALIDDIRECTDRAWGUID";
		break;
	case DDERR_UNSUPPORTEDFORMAT:
		szError = "DDERR_UNSUPPORTEDFORMAT";
		break;
	case DDERR_UNSUPPORTEDMASK:
		szError = "DDERR_UNSUPPORTEDMASK";
		break;
	case DDERR_VERTICALBLANKINPROGRESS:
		szError = "DDERR_VERTICALBLANKINPROGRESS";
		break;
	case DDERR_WASSTILLDRAWING:
		szError = "DDERR_WASSTILLDRAWING";
		break;
	case DDERR_XALIGN:
		szError = "DDERR_XALIGN";
		break;
	case DDERR_TOOBIGWIDTH:
		szError = "DDERR_TOOBIGWIDTH";
		break;
	case DDERR_CANTLOCKSURFACE:
		szError = "DDERR_CANTLOCKSURFACE";
		break;
	case DDERR_SURFACEISOBSCURED:
		szError = "DDERR_SURFACEISOBSCURED";
		break;
	case DDERR_SURFACELOST:
		szError = "DDERR_SURFACELOST";
		break;
	case DDERR_SURFACENOTATTACHED:
		szError = "DDERR_SURFACENOTATTACHED";
		break;
	case DDERR_TOOBIGHEIGHT:
		szError = "DDERR_TOOBIGHEIGHT";
		break;
	case DDERR_TOOBIGSIZE:
		szError = "DDERR_TOOBIGSIZE";
		break;
	case DDERR_SURFACEBUSY:
		szError = "DDERR_SURFACEBUSY";
		break;
	case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
		szError = "DDERR_OVERLAYCOLORKEYONLYONEACTIVE";
		break;
	case DDERR_PALETTEBUSY:
		szError = "DDERR_PALETTEBUSY";
		break;
	case DDERR_COLORKEYNOTSET:
		szError = "DDERR_COLORKEYNOTSET";
		break;
	case DDERR_SURFACEALREADYATTACHED:
		szError = "DDERR_SURFACEALREADYATTACHED";
		break;
	case DDERR_SURFACEALREADYDEPENDENT:
		szError = "DDERR_SURFACEALREADYDEPENDENT";
		break;
	case DDERR_OVERLAYCANTCLIP:
		szError = "DDERR_OVERLAYCANTCLIP";
		break;
	case DDERR_NOVSYNCHW:
		szError = "DDERR_NOVSYNCHW";
		break;
	case DDERR_NOZBUFFERHW:
		szError = "DDERR_NOZBUFFERHW";
		break;
	case DDERR_NOZOVERLAYHW:
		szError = "DDERR_NOZOVERLAYHW";
		break;
	case DDERR_OUTOFCAPS:
		szError = "DDERR_OUTOFCAPS";
		break;
	case DDERR_OUTOFVIDEOMEMORY:
		szError = "DDERR_OUTOFVIDEOMEMORY";
		break;
	case DDERR_NOTEXTUREHW:
		szError = "DDERR_NOTEXTUREHW";
		break;
	case DDERR_NOROTATIONHW:
		szError = "DDERR_NOROTATIONHW";
		break;
	case DDERR_NOSTRETCHHW:
		szError = "DDERR_NOSTRETCHHW";
		break;
	case DDERR_NOT4BITCOLOR:
		szError = "DDERR_NOT4BITCOLOR";
		break;
	case DDERR_NOT4BITCOLORINDEX:
		szError = "DDERR_NOT4BITCOLORINDEX";
		break;
	case DDERR_NOT8BITCOLOR:
		szError = "DDERR_NOT8BITCOLOR";
		break;
	case DDERR_NORASTEROPHW:
		szError = "DDERR_NORASTEROPHW";
		break;
	case DDERR_NOEXCLUSIVEMODE:
		szError = "DDERR_NOEXCLUSIVEMODE";
		break;
	case DDERR_NOFLIPHW:
		szError = "DDERR_NOFLIPHW";
		break;
	case DDERR_NOGDI:
		szError = "DDERR_NOGDI";
		break;
	case DDERR_NOMIRRORHW:
		szError = "DDERR_NOMIRRORHW";
		break;
	case DDERR_NOTFOUND:
		szError = "DDERR_NOTFOUND";
		break;
	case DDERR_NOOVERLAYHW:
		szError = "DDERR_NOOVERLAYHW";
		break;
	case DDERR_NOCOLORKEYHW:
		szError = "DDERR_NOCOLORKEYHW";
		break;
	case DDERR_NOALPHAHW:
		szError = "DDERR_NOALPHAHW";
		break;
	case DDERR_NOCLIPLIST:
		szError = "DDERR_NOCLIPLIST";
		break;
	case DDERR_NOCOLORCONVHW:
		szError = "DDERR_NOCOLORCONVHW";
		break;
	case DDERR_NOCOOPERATIVELEVELSET:
		szError = "DDERR_NOCOOPERATIVELEVELSET";
		break;
	case DDERR_NOCOLORKEY:
		szError = "DDERR_NOCOLORKEY";
		break;
	case DDERR_NO3D:
		szError = "DDERR_NO3D";
		break;
	case DDERR_INVALIDMODE:
		szError = "DDERR_INVALIDMODE";
		break;
	case DDERR_INVALIDOBJECT:
		szError = "DDERR_INVALIDOBJECT";
		break;
	case DDERR_INVALIDPIXELFORMAT:
		szError = "DDERR_INVALIDPIXELFORMAT";
		break;
	case DDERR_INVALIDRECT:
		szError = "DDERR_INVALIDRECT";
		break;
	case DDERR_LOCKEDSURFACES:
		szError = "DDERR_LOCKEDSURFACES";
		break;
	case DDERR_INVALIDCLIPLIST:
		szError = "DDERR_INVALIDCLIPLIST";
		break;
	case DDERR_CURRENTLYNOTAVAIL:
		szError = "DDERR_CURRENTLYNOTAVAIL";
		break;
	case DDERR_EXCEPTION:
		szError = "DDERR_EXCEPTION";
		break;
	case DDERR_HEIGHTALIGN:
		szError = "DDERR_HEIGHTALIGN";
		break;
	case DDERR_INCOMPATIBLEPRIMARY:
		szError = "DDERR_INCOMPATIBLEPRIMARY";
		break;
	case DDERR_INVALIDCAPS:
		szError = "DDERR_INVALIDCAPS";
		break;
	case DDERR_CANNOTDETACHSURFACE:
		szError = "DDERR_CANNOTDETACHSURFACE";
		break;
	case DDERR_UNSUPPORTED:
		szError = "DDERR_UNSUPPORTED";
		break;
	case DDERR_GENERIC:
		szError = "DDERR_GENERIC";
		break;
	case DDERR_OUTOFMEMORY:
		szError = "DDERR_OUTOFMEMORY";
		break;
	case DDERR_INVALIDPARAMS:
		szError = "DDERR_INVALIDPARAMS";
		break;
	case DDERR_ALREADYINITIALIZED:
		szError = "DDERR_ALREADYINITIALIZED";
		break;
	case DDERR_CANNOTATTACHSURFACE:
		szError = "DDERR_CANNOTATTACHSURFACE";
		break;
	default:
		const char szUnknown[] = "DDERR unknown 0x%x";
		/// ASSERT: assert(dwMaxChars >= sizeof(szUnknown) + 10);
		sprintf(pszBuffer, szUnknown, hError);
		return;
	}

	strncpy(pszBuffer, szError, dwMaxChars);
}

void TraceErrorDS(HRESULT hError, char *pszBuffer, DWORD dwMaxChars)
{
	const char *szError;

	switch(hError) {
	case DSERR_PRIOLEVELNEEDED:
		szError = "DSERR_PRIOLEVELNEEDED";
		break;
	case DSERR_BADFORMAT:
		szError = "DSERR_BADFORMAT";
		break;
	case DSERR_NODRIVER:
		szError = "DSERR_NODRIVER";
		break;
	case DSERR_ALREADYINITIALIZED:
		szError = "DSERR_ALREADYINITIALIZED";
		break;
	case DSERR_BUFFERLOST:
		szError = "DSERR_BUFFERLOST";
		break;
	case DS_OK:
		szError = "DS_OK";
		break;
	case DSERR_INVALIDCALL:
		szError = "DSERR_INVALIDCALL";
		break;
	case E_NOINTERFACE:
		szError = "E_NOINTERFACE";
		break;
	case DSERR_NOAGGREGATION:
		szError = "DSERR_NOAGGREGATION";
		break;
	case DSERR_OUTOFMEMORY:
		szError = "DSERR_OUTOFMEMORY";
		break;
	case DSERR_INVALIDPARAM:
		szError = "DSERR_INVALIDPARAM";
		break;
	case DSERR_ALLOCATED:
		szError = "DSERR_ALLOCATED";
		break;
	case DSERR_CONTROLUNAVAIL:
		szError = "DSERR_CONTROLUNAVAIL";
		break;
	default:
		const char szUnknown[] = "DSERR unknown 0x%x";
		/// ASSERT: assert(dwMaxChars >= sizeof(szUnknown) + 10);
		sprintf(pszBuffer, szUnknown, hError);
		return;
	}

	strncpy(pszBuffer, szError, dwMaxChars);
}

char *TraceLastError()
{
	int v0; // eax

	v0 = GetLastError();
	return GetErrorStr(v0);
}

void __cdecl app_fatal(const char *pszFmt, ...)
{
	va_list arglist; // [esp+8h] [ebp+8h]

	va_start(arglist, pszFmt);
	FreeDlg();
#ifdef _DEBUG
	TriggerBreak();
#endif
	if ( pszFmt )
		MsgBox(pszFmt, arglist);
	init_cleanup(0);
	exit(1);
}

void MsgBox(const char *pszFmt, va_list va)
{
	char Text[256]; // [esp+0h] [ebp-100h]

	wvsprintf(Text, pszFmt, va);
	if ( ghMainWnd )
		SetWindowPos(ghMainWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
	MessageBox(ghMainWnd, Text, "ERROR", MB_TASKMODAL|MB_ICONHAND);
}

void FreeDlg()
{
	if ( terminating && cleanup_thread_id != GetCurrentThreadId() )
		Sleep(20000u);
	terminating = 1;
	cleanup_thread_id = GetCurrentThreadId();
	dx_cleanup();
	if ( (unsigned char)gbMaxPlayers > 1u )
	{
		if ( SNetLeaveGame(3) )
			Sleep(2000u);
	}
	SNetDestroy();
	ShowCursor(1);
}

void __cdecl DrawDlg(char *pszFmt, ...)
{
	char text[256]; // [esp+0h] [ebp-100h]
	va_list arglist; // [esp+10Ch] [ebp+Ch]

	va_start(arglist, pszFmt);
	wvsprintf(text, pszFmt, arglist);
	SDrawMessageBox(text, "Diablo", MB_TASKMODAL|MB_ICONEXCLAMATION);
}

#ifdef _DEBUG
void assert_fail(int nLineNo, const char *pszFile, const char *pszFail)
{
	app_fatal("assertion failed (%d:%s)\n%s", nLineNo, pszFile, pszFail);
}
#endif

void DDErrMsg(HRESULT hError, int nLineNo, const char *pszFile)
{
	if(hError == DD_OK) {
		return;
	}

	app_fatal("Direct draw error (%s:%d)\n%s", pszFile, nLineNo, GetErrorStr(hError));
}

void DSErrMsg(HRESULT hError, int nLineNo, const char *pszFile)
{
	if(hError == DS_OK) {
		return;
	}

	app_fatal("Direct sound error (%s:%d)\n%s", pszFile, nLineNo, GetErrorStr(hError));
}

void center_window(HWND hWnd)
{
	int w, h, hr, vr;
	RECT Rect;
	HDC hdc;

	GetWindowRect(hWnd, &Rect);
	w = Rect.right - Rect.left;
	h = Rect.bottom - Rect.top;

	hdc = GetDC(hWnd);
	hr = GetDeviceCaps(hdc, HORZRES);
	vr = GetDeviceCaps(hdc, VERTRES);
	ReleaseDC(hWnd, hdc);

	if(!SetWindowPos(hWnd, HWND_TOP, (hr - w) / 2, (vr - h) / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE)) {
		app_fatal("center_window: %s", TraceLastError());
	}
}

void ErrDlg(int nResource, DWORD dwError, const char *pszFile, int nLineNo)
{
	char *s;
	char szBuffer[512];

	FreeDlg();

	s = strrchr(pszFile, '\\');
	if(s != NULL) {
		pszFile = s + 1;
	}

	wsprintf(szBuffer, "%s\nat: %s line %d", GetErrorStr(dwError), pszFile, nLineNo);

	if(DialogBoxParam(ghInst, MAKEINTRESOURCE(nResource), ghMainWnd, (DLGPROC)FuncDlg, (LPARAM)szBuffer) == -1) {
		app_fatal("ErrDlg: %d", nResource);
	}

	app_fatal(NULL);
}

BOOL __stdcall FuncDlg(HWND hDlg, UINT uMsg, WPARAM wParam, char *text)
{
	if ( uMsg == WM_INITDIALOG )
	{
		TextDlg(hDlg, text);
	}
	else
	{
		if ( uMsg != WM_COMMAND )
			return 0;
		if ( (_WORD)wParam == 1 )
		{
			EndDialog(hDlg, 1);
		}
		else if ( (_WORD)wParam == 2 )
		{
			EndDialog(hDlg, 0);
		}
	}
	return 1;
}

void TextDlg(HWND hDlg, char *text)
{
	char *v2; // esi
	HWND v3; // edi

	v2 = text;
	v3 = hDlg;
	center_window(hDlg);
	if ( v2 )
		SetDlgItemText(v3, 1000, v2);
}

void ErrOkDlg(int template_id, int error_code, char *log_file_path, int log_line_nr)
{
	char *v4; // esi
	int v5; // edi
	unsigned short v6; // bx
	char *v7; // eax
	char *v8; // eax
	LPARAM dwInitParam[128]; // [esp+Ch] [ebp-200h]

	v4 = log_file_path;
	v5 = error_code;
	v6 = template_id;
	v7 = strrchr(log_file_path, '\\');
	if ( v7 )
		v4 = v7 + 1;
	v8 = GetErrorStr(v5);
	wsprintf((LPSTR)dwInitParam, "%s\nat: %s line %d", v8, v4, log_line_nr);
	DialogBoxParam(ghInst, MAKEINTRESOURCE(v6), ghMainWnd, (DLGPROC)FuncDlg, (LPARAM)dwInitParam);
}

void FileErrDlg(char *error)
{
	char *v1; // esi

	v1 = error;
	FreeDlg();
	if ( !v1 )
		v1 = "";
	if ( DialogBoxParam(ghInst, MAKEINTRESOURCE(IDD_DIALOG3), ghMainWnd, (DLGPROC)FuncDlg, (LPARAM)v1) == -1 )
		app_fatal("FileErrDlg");
	app_fatal(0);
}

void DiskFreeDlg(char *error)
{
	char *v1; // esi

	v1 = error;
	FreeDlg();
	if ( DialogBoxParam(ghInst, MAKEINTRESOURCE(IDD_DIALOG7), ghMainWnd, (DLGPROC)FuncDlg, (LPARAM)v1) == -1 )
		app_fatal("DiskFreeDlg");
	app_fatal(0);
}

_bool InsertCDDlg()
{
	int v0; // edi

	ShowCursor(1);
	v0 = DialogBoxParam(ghInst, MAKEINTRESOURCE(IDD_DIALOG9), ghMainWnd, (DLGPROC)FuncDlg, (LPARAM)"");
	if ( v0 == -1 )
		app_fatal("InsertCDDlg");
	ShowCursor(0);
	return v0 == 1;
}

void DirErrorDlg(char *error)
{
	char *v1; // esi

	v1 = error;
	FreeDlg();
	if ( DialogBoxParam(ghInst, MAKEINTRESOURCE(IDD_DIALOG11), ghMainWnd, (DLGPROC)FuncDlg, (LPARAM)v1) == -1 )
		app_fatal("DirErrorDlg");
	app_fatal(0);
}
