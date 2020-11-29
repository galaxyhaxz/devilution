//HEADER_GOES_HERE
#ifndef __MOVIE_H__
#define __MOVIE_H__

extern BOOLEAN movie_playing;
extern BOOL loop_movie;

void play_movie(const char *pszMovie, BOOL bCanSkip);
LRESULT CALLBACK MovieWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif /* __MOVIE_H__ */
