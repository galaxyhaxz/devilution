//HEADER_GOES_HERE
#ifndef __MAINMENU_H__
#define __MAINMENU_H__

extern char gszHero[16];

void mainmenu_refresh_music();
void __stdcall mainmenu_create_hero(int a1, int a2, int a3, int a4, char *name_1, char *name_2);
int __stdcall mainmenu_select_hero_dialog(int u1, int u2, int u3, int u4, int mode, char *cname, int clen, char *cdesc, int cdlen, int *multi);
void mainmenu_loop();
int mainmenu_single_player();
int mainmenu_init_menu(int a1);
int mainmenu_multi_player();
void mainmenu_play_intro();

/* data */

extern int menu_music_track_id; // idb

#endif /* __MAINMENU_H__ */
