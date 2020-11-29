#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

BOOLEAN movie_playing;
BOOL loop_movie;

void play_movie(const char *pszMovie, BOOL bCanSkip)
{
	WNDPROC saveProc;
	MSG msg;
	HANDLE hVideo;

	if(!gbActive) {
		return;
	}

	/// ASSERT: assert(ghMainWnd);
	saveProc = SetWindowProc(MovieWndProc);
	InvalidateRect(ghMainWnd, NULL, FALSE);
	UpdateWindow(ghMainWnd);
	movie_playing = TRUE;
	sound_disable_music(TRUE);
	stream_stop();
	effects_play_sound("Sfx\\Misc\\blank.wav");
	SVidPlayBegin(pszMovie, 0, 0, 0, 0, loop_movie ? 0x100C0808 : 0x10280808, &hVideo);

	while(hVideo != NULL) {
		if(!gbActive || bCanSkip && !movie_playing) {
			break;
		}
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if(msg.message != WM_QUIT) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if(!SVidPlayContinue()) {
			break;
		}
	}
	if(hVideo != NULL) {
		SVidPlayEnd(hVideo);
	}

	saveProc = SetWindowProc(saveProc);
	/// ASSERT: assert(saveProc == MovieWndProc);
	sound_disable_music(FALSE);
}

LRESULT CALLBACK MovieWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) {
	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		movie_playing = FALSE;
		break;
	case WM_SYSCOMMAND:
		if(wParam == SC_CLOSE) {
			movie_playing = FALSE;
			return 0;
		}
		break;
	}

	return MainWndProc(hWnd, uMsg, wParam, lParam);
}
