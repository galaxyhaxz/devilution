//HEADER_GOES_HERE
#ifndef __GAMEMENU_H__
#define __GAMEMENU_H__

void gamemenu_on();
void gamemenu_update_single(TMenuItem *pMenuItems);
void gamemenu_update_multi(TMenuItem *pMenuItems);
void gamemenu_off();
void gamemenu_handle_previous();
void gamemenu_previous(BOOL bActivate);
void gamemenu_new_game(BOOL bActivate);
void gamemenu_quit_game(BOOL bActivate);
void gamemenu_load_game(BOOL bActivate);
void gamemenu_save_game(BOOL bActivate);
void gamemenu_restart_town(BOOL bActivate);
void gamemenu_options(BOOL bActivate);
void gamemenu_get_music();
void gamemenu_sound_music_toggle(char **names, TMenuItem *pItem, int ticks);
void gamemenu_get_sound();
void gamemenu_get_color_cycling();
void gamemenu_get_gamma();
void gamemenu_music_volume(BOOL bActivate);
int gamemenu_slider_music_sound(TMenuItem *pItem);
void gamemenu_sound_volume(BOOL bActivate);
void gamemenu_gamma(BOOL bActivate);
int gamemenu_slider_gamma();
void gamemenu_color_cycling(BOOL bActivate);

/* rdata */
extern TMenuItem sgSingleMenu[6];
extern TMenuItem sgMultiMenu[5];
extern TMenuItem sgOptionMenu[6];
extern char *music_toggle_names[];
extern char *sound_toggle_names[];
extern char *color_cycling_toggle_names[];

#endif /* __GAMEMENU_H__ */
