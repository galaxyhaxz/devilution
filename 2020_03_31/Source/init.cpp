#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"
#include "../DiabloUI/diabloui.h"

_SNETVERSIONDATA fileinfo;
int gbActive; // weak
char diablo_exe_path[260];
void *unused_mpq;
char patch_rt_mpq_path[260];
WNDPROC CurrentProc;
void *diabdat_mpq;
char diabdat_mpq_path[260];
void *patch_rt_mpq;
BOOL killed_mom_parent; // weak
_bool screensaver_enabled_prev;

/* data */

char gszVersionNumber[260] = "internal version unknown";
char gszProductName[260] = "Diablo v1.09";

void init_cleanup(BOOL show_cursor)
{
	int v1; // edi

	v1 = show_cursor;
	pfile_flush_W();
	init_disable_screensaver(0);
	init_run_office_from_start_menu();
	if ( diabdat_mpq )
	{
		SFileCloseArchive(diabdat_mpq);
		diabdat_mpq = 0;
	}
	if ( patch_rt_mpq )
	{
		SFileCloseArchive(patch_rt_mpq);
		patch_rt_mpq = 0;
	}
	if ( unused_mpq )
	{
		SFileCloseArchive(unused_mpq);
		unused_mpq = 0;
	}
	UiDestroy();
	effects_cleanup_sfx();
	sound_cleanup();
	NetClose();
	dx_cleanup();
	engine_debug_trap(v1);
	StormDestroy();
	if ( v1 )
		ShowCursor(1);
}

void init_run_office_from_start_menu()
{
	LPITEMIDLIST idl;

	if(!killed_mom_parent) {
		return;
	}

	killed_mom_parent = FALSE;
	char szPath[256] = ""; /// BUGFIX: size should be at least 'MAX_PATH'
	idl = NULL;

	if(SHGetSpecialFolderLocation(GetDesktopWindow(), CSIDL_STARTMENU, &idl) == NOERROR) {
		SHGetPathFromIDList(idl, szPath);
		init_run_office(szPath);
	}
}

void init_run_office(char *dir)
{
	HANDLE hSearch;
	WIN32_FIND_DATA find;
	char szFirst[MAX_PATH];

	strcpy(szFirst, dir);
	if(szFirst[0] != '\0' && szFirst[strlen(szFirst) - 1] == '\\') {
		strcat(szFirst, "*");
	} else {
		strcat(szFirst, "\\*");
	}
	hSearch = FindFirstFile(szFirst, &find);
	if(hSearch == INVALID_HANDLE_VALUE) {
		return;
	}

	while(1) {
		if(find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if(strcmp(find.cFileName, ".") != 0 && strcmp(find.cFileName, "..") != 0) {
				char szNext[MAX_PATH] = "";
				if(dir[0] != '\0' && dir[strlen(dir) - 1] == '\\') {
					sprintf(szNext, "%s%s\\", dir, find.cFileName);
				} else {
					sprintf(szNext, "%s\\%s\\", dir, find.cFileName);
				}
				init_run_office(szNext);
			}
		} else if(_strcmpi(find.cFileName, "Microsoft Office Shortcut Bar.lnk") == 0) {
			ShellExecute(GetDesktopWindow(), "open", find.cFileName, "", dir, SW_SHOWNORMAL);
		}
		if(!FindNextFile(hSearch, &find)) {
			break;
		}
	}

	FindClose(hSearch);
}

void init_disable_screensaver(_bool disable)
{
	_bool v1; // al
	char Data[16]; // [esp+4h] [ebp-20h]
	DWORD Type; // [esp+14h] [ebp-10h]
	DWORD cbData; // [esp+18h] [ebp-Ch]
	HKEY phkResult; // [esp+1Ch] [ebp-8h]
	_bool v6; // [esp+20h] [ebp-4h]

	v6 = disable;
	if ( !RegOpenKeyEx(HKEY_CURRENT_USER, "Control Panel\\Desktop", 0, KEY_READ|KEY_WRITE, &phkResult) )
	{
		if ( v6 )
		{
			cbData = 16;
			if ( !RegQueryValueEx(phkResult, "ScreenSaveActive", 0, &Type, (LPBYTE)Data, &cbData) )
				screensaver_enabled_prev = Data[0] != '0';
			v1 = 0;
		}
		else
		{
			v1 = screensaver_enabled_prev;
		}
		Data[1] = 0;
		Data[0] = (v1 != 0) + '0';
		RegSetValueEx(phkResult, "ScreenSaveActive", 0, REG_SZ, (const BYTE *)Data, 2u);
		RegCloseKey(phkResult);
	}
}

void init_create_window(int nCmdShow)
{
	int nHeight; // eax
	HWND hWnd; // esi
	WNDCLASSEXA wcex; // [esp+8h] [ebp-34h]
	int nWidth; // [esp+38h] [ebp-4h]

	init_kill_mom_parent();
	pfile_init_save_directory();
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = ghInst;
	wcex.hIcon = LoadIcon(ghInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = "DIABLO";
	wcex.lpszClassName = "DIABLO";
	wcex.hIconSm = (HICON)LoadImage(ghInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	if ( !RegisterClassEx(&wcex) )
		app_fatal("Unable to register window class");
	if ( GetSystemMetrics(SM_CXSCREEN) >= 640 )
		nWidth = GetSystemMetrics(SM_CXSCREEN);
	else
		nWidth = 640;
	if ( GetSystemMetrics(SM_CYSCREEN) >= 480 )
		nHeight = GetSystemMetrics(SM_CYSCREEN);
	else
		nHeight = 480;
	hWnd = CreateWindowEx(0, "DIABLO", "DIABLO", WS_POPUP, 0, 0, nWidth, nHeight, NULL, NULL, ghInst, NULL);
	if ( !hWnd )
		app_fatal("Unable to create main window");
	ShowWindow(hWnd, SW_SHOWNORMAL); // nCmdShow used only in beta: ShowWindow(hWnd, nCmdShow)
	UpdateWindow(hWnd);
	init_await_mom_parent_exit();
	dx_init(hWnd);
	BlackPalette();
	snd_init(hWnd);
	init_archives();
	init_disable_screensaver(1);
}

void init_kill_mom_parent()
{
	HWND v0; // eax

	v0 = init_find_mom_parent();
	if ( v0 )
	{
		PostMessage(v0, WM_CLOSE, 0, 0);
		killed_mom_parent = TRUE;
	}
}

HWND init_find_mom_parent()
{
	HWND i; // eax
	HWND v1; // esi
	char ClassName[256]; // [esp+4h] [ebp-100h]

	for ( i = GetForegroundWindow(); ; i = GetWindow(v1, GW_HWNDNEXT) )
	{
		v1 = i;
		if ( !i )
			break;
		GetClassName(i, ClassName, 255);
		if ( !_strcmpi(ClassName, "MOM Parent") )
			break;
	}
	return v1;
}

void init_await_mom_parent_exit()
{
	DWORD v0; // edi

	v0 = GetTickCount();
	do
	{
		if ( !init_find_mom_parent() )
			break;
		Sleep(250);
	}
	while ( GetTickCount() - v0 <= 4000 );
}

void init_archives()
{
	void *a1; // [esp+8h] [ebp-8h]
#ifdef COPYPROT
	int v1; // [esp+Ch] [ebp-4h]
#endif

	fileinfo.size = 20;
	fileinfo.versionstring = gszVersionNumber;
	fileinfo.executablefile = diablo_exe_path;
	fileinfo.originalarchivefile = diabdat_mpq_path;
	fileinfo.patcharchivefile = patch_rt_mpq_path;
	init_get_file_info();
#ifdef COPYPROT
	while ( 1 )
	{
#endif
		diabdat_mpq = init_test_access(diabdat_mpq_path, "\\diabdat.mpq", "DiabloCD", 1000, 1);
#ifdef COPYPROT
		if ( diabdat_mpq )
			break;
		UiCopyProtError(&v1);
		if ( v1 == COPYPROT_CANCEL )
			FileErrDlg("diabdat.mpq");
	}
#endif
	if ( !WOpenFile("ui_art\\title.pcx", &a1, 1) )
		FileErrDlg("Main program archive: diabdat.mpq");
	WCloseFile(a1);
	patch_rt_mpq = init_test_access(patch_rt_mpq_path, "\\patch_rt.mpq", "DiabloInstall", 2000, 0);
}

void *init_test_access(char *mpq_path, char *mpq_name, char *reg_loc, int flags, _bool on_cd)
{
	char *v5; // esi
	char *v7; // eax
	char Filename[260]; // [esp+Ch] [ebp-314h]
	char Buffer[260]; // [esp+110h] [ebp-210h]
	char v15[260]; // [esp+214h] [ebp-10Ch]
	char *mpq_namea; // [esp+318h] [ebp-8h]
	void *archive; // [esp+31Ch] [ebp-4h]

	mpq_namea = mpq_name;
	v5 = mpq_path;
	if ( !GetCurrentDirectory(0x104u, Buffer) )
		app_fatal("Can't get program path");
	init_strip_trailing_slash(Buffer);
	if ( !SFileSetBasePath(Buffer) )
		app_fatal("SFileSetBasePath");
	if ( !GetModuleFileName(ghInst, Filename, 0x104u) )
		app_fatal("Can't get program name");
	v7 = strrchr(Filename, '\\');
	if ( v7 )
		*v7 = 0;
	init_strip_trailing_slash(Filename);
	strcpy(v5, Buffer);
	strcat(v5, mpq_namea);
#ifdef COPYPROT
	if ( SFileOpenArchive(v5, flags, on_cd, &archive) )
#else
	if ( SFileOpenArchive(v5, flags, 0, &archive) )
#endif
		return archive;
	if ( strcmp(Filename, Buffer) )
	{
		strcpy(v5, Filename);
		strcat(v5, mpq_namea);
#ifdef COPYPROT
		if ( SFileOpenArchive(v5, flags, on_cd, &archive) )
#else
		if ( SFileOpenArchive(v5, flags, 0, &archive) )
#endif
			return archive;
	}
	v15[0] = 0;
	if ( reg_loc )
	{
		if ( SRegLoadString("Archives", (const char *)reg_loc, 0, v15, 260) )
		{
			init_strip_trailing_slash(v15);
			strcpy(v5, v15);
			strcat(v5, mpq_namea);
#ifdef COPYPROT
			if ( SFileOpenArchive(v5, flags, on_cd, &archive) )
#else
			if ( SFileOpenArchive(v5, flags, 0, &archive) )
#endif
				return archive;
		}
	}
	if ( on_cd && init_read_test_file(v15, mpq_namea, flags, &archive) )
	{
		strcpy(v5, v15);
		return archive;
	}
	return 0;
}

char *init_strip_trailing_slash(char *path)
{
	char *result; // eax

	result = strrchr(path, '\\');
	if ( result )
	{
		if ( !result[1] )
			*result = 0;
	}
	return result;
}

BOOL init_read_test_file(char *pszPath, char *pszArchive, int flags, HANDLE *phArchive)
{
	DWORD dwSize;
	char *pszDrive, *pszRoot;
	char szDrive[MAX_PATH];

	dwSize = GetLogicalDriveStrings(sizeof(szDrive), szDrive);
	if(dwSize == 0 || dwSize > sizeof(szDrive)) {
		return FALSE;
	}

	while(*pszArchive == '\\') {
		pszArchive++;
	}

	pszDrive = szDrive;
	while(*pszDrive != '\0') {
		pszRoot = pszDrive;
		while(*pszDrive++ != '\0');
		if(GetDriveType(pszRoot) == DRIVE_CDROM) {
			strcpy(pszPath, pszRoot);
			strcat(pszPath, pszArchive);
			if(SFileOpenArchive(pszPath, flags, 1, phArchive)) {
				return TRUE;
			}
		}
	}

	return FALSE;
}

void init_get_file_info()
{
	int v0; // eax
	DWORD v1; // edi
	void *v2; // ebx
	unsigned int uBytes; // [esp+8h] [ebp-Ch]
	DWORD dwHandle; // [esp+Ch] [ebp-8h]
	VS_FIXEDFILEINFO *lpBuffer; // [esp+10h] [ebp-4h]

	if ( GetModuleFileName(ghInst, diablo_exe_path, 0x104u) )
	{
		v0 = GetFileVersionInfoSize(diablo_exe_path, &dwHandle);
		v1 = v0;
		if ( v0 )
		{
			v2 = DiabloAllocPtr(v0);
			if ( GetFileVersionInfo(diablo_exe_path, 0, v1, v2) )
			{
				if ( VerQueryValue(v2, "\\", (LPVOID *)&lpBuffer, &uBytes) )
					sprintf(
						gszVersionNumber,
						"version %d.%d.%d.%d",
						lpBuffer->dwProductVersionMS >> 16,
						lpBuffer->dwProductVersionMS & 0xFFFF,
						lpBuffer->dwProductVersionLS >> 16,
						lpBuffer->dwProductVersionLS & 0xFFFF);
			}
			mem_free_dbg(v2);
		}
	}
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) {
	case WM_CREATE:
		ghMainWnd = hWnd;
		break;
	case WM_DESTROY:
		init_cleanup(TRUE);
		ghMainWnd = NULL;
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		force_redraw = 255;
		break;
	case WM_CLOSE:
	case WM_ERASEBKGND:
		return 0;
	case WM_ACTIVATEAPP:
		init_activate_window(hWnd, wParam);
		break;
#ifdef _DEBUG
	case WM_SYSKEYUP:
		if(wParam == VK_RETURN) {
			fullscreen = !fullscreen;
			dx_reinit();
			return 0;
		}
		break;
#endif
	case WM_QUERYNEWPALETTE:
		SDrawRealizePalette();
		return 1;
	case WM_PALETTECHANGED:
		if((HWND)wParam != hWnd) {
			SDrawRealizePalette();
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void init_activate_window(HWND hWnd, BOOL bActive)
{
	LONG dwNewLong; // eax

	gbActive = bActive;
	UiAppActivate(bActive);
	dwNewLong = GetWindowLong(hWnd, GWL_STYLE);

	if ( gbActive && fullscreen )
		dwNewLong &= ~WS_SYSMENU;
	else
		dwNewLong |= WS_SYSMENU;

	SetWindowLong(hWnd, GWL_STYLE, dwNewLong);

	if ( gbActive )
	{
		force_redraw = 255;
		ResetPal();
	}
}

LRESULT __stdcall WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result; // eax

	if ( CurrentProc )
		result = CurrentProc(hWnd, Msg, wParam, lParam);
	else
		result = MainWndProc(hWnd, Msg, wParam, lParam);
	return result;
}

WNDPROC SetWindowProc(WNDPROC NewProc)
{
	WNDPROC OldProc; // eax

	OldProc = CurrentProc;
	CurrentProc = NewProc;
	return OldProc;
}
