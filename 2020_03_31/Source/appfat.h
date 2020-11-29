//HEADER_GOES_HERE
#ifndef __APPFAT_H__
#define __APPFAT_H__

extern char sz_error_buf[256];
extern int terminating; // weak
extern int cleanup_thread_id; // weak

void TriggerBreak();
#ifdef _DEBUG
LONG __stdcall BreakFilter(PEXCEPTION_POINTERS pExc);
#endif
char *GetErrorStr(int error_code);
void TraceErrorDD(HRESULT hError, char *pszBuffer, DWORD dwMaxChars);
void TraceErrorDS(HRESULT hError, char *pszBuffer, DWORD dwMaxChars);
char *TraceLastError();
void __cdecl app_fatal(const char *pszFmt, ...);
void MsgBox(const char *pszFmt, va_list va);
void FreeDlg();
void __cdecl DrawDlg(char *pszFmt, ...);
#ifdef _DEBUG
void assert_fail(int nLineNo, const char *pszFile, const char *pszFail);
#endif
void DDErrMsg(HRESULT hError, int nLineNo, const char *pszFile);
void DSErrMsg(HRESULT hError, int nLineNo, const char *pszFile);
void center_window(HWND hWnd);
void ErrDlg(int nResource, DWORD dwError, const char *pszFile, int nLineNo);
BOOL __stdcall FuncDlg(HWND hDlg, UINT uMsg, WPARAM wParam, char *text);
void TextDlg(HWND hDlg, char *text);
void ErrOkDlg(int template_id, int error_code, char *log_file_path, int log_line_nr);
void FileErrDlg(char *error);
void DiskFreeDlg(char *error);
_bool InsertCDDlg();
void DirErrorDlg(char *error);

#endif /* __APPFAT_H__ */
