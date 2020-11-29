//HEADER_GOES_HERE
#ifndef __DX_H__
#define __DX_H__

extern void *sgpBackBuf;
extern IDirectDraw *lpDDInterface;
extern IDirectDrawPalette *lpDDPalette; // idb
extern int sgdwLockCount;
extern BYTE *gpBuffer;
extern IDirectDrawSurface *lpDDSBackBuf;
extern IDirectDrawSurface *lpDDSPrimary;
extern char gbBackBuf; // weak
extern char gbEmulate; // weak
extern HINSTANCE sghInstDD; // idb

void dx_init(HWND hWnd);
void lock_buf(BYTE idx);
void unlock_buf(BYTE idx);
void dx_cleanup();
void dx_reinit();
void j_dx_reinit();

#endif /* __DX_H__ */
