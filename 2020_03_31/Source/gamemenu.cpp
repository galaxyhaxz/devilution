#include "diablo.h"

TMenuItem sgSingleMenu[6] =
{
  { 0x80000000, "Save Game", &gamemenu_save_game },
  { 0x80000000, "Options", &gamemenu_options },
  { 0x80000000, "New Game", &gamemenu_new_game },
  { 0x80000000, "Load Game", &gamemenu_load_game },
  { 0x80000000, "Quit Diablo", &gamemenu_quit_game },
  { 0x80000000, NULL, NULL }
};
TMenuItem sgMultiMenu[5] =
{
  { 0x80000000, "Options", &gamemenu_options },
  { 0x80000000, "New Game", &gamemenu_new_game },
  { 0x80000000, "Restart In Town", &gamemenu_restart_town },
  { 0x80000000, "Quit Diablo", &gamemenu_quit_game },
  { 0x80000000, NULL, NULL }
};
TMenuItem sgOptionMenu[6] =
{
  { 0xC0000000, NULL, &gamemenu_music_volume },
  { 0xC0000000, NULL, &gamemenu_sound_volume },
  { 0xC0000000, "Gamma", &gamemenu_gamma },
  { 0x80000000, NULL, &gamemenu_color_cycling },
  { 0x80000000, "Previous Menu", &gamemenu_previous },
  { 0x80000000, NULL, NULL }
};
char *music_toggle_names[] = { "Music", "Music Disabled" };
char *sound_toggle_names[] = { "Sound", "Sound Disabled" };
char *color_cycling_toggle_names[] = { "Color Cycling Off", "Color Cycling On" };

void gamemenu_on()
{
	if(gbMaxPlayers == 1) {
		gmenu_set_items(sgSingleMenu, gamemenu_update_single);
	} else {
		gmenu_set_items(sgMultiMenu, gamemenu_update_multi);
	}

	PressEscKey();
}

void gamemenu_update_single(TMenuItem *pMenuItems)
{
	/// ASSERT: assert(pMenuItems == sgSingleMenu);
	gmenu_enable(&sgSingleMenu[3], gbValidSaveFile);
	gmenu_enable(&sgSingleMenu[0], plr[myplr]._pmode != PM_DEATH && !deathflag);
}

void gamemenu_update_multi(TMenuItem *pMenuItems)
{
	/// ASSERT: assert(pMenuItems == sgMultiMenu);
	gmenu_enable(&sgMultiMenu[2], deathflag);
}

void gamemenu_off()
{
	gmenu_set_items(NULL, NULL);
}

void gamemenu_handle_previous()
{
	if(gmenu_is_active()) {
		gamemenu_off();
	} else {
		gamemenu_on();
	}
}

void gamemenu_previous(BOOL bActivate)
{
	/// ASSERT: assert(bActivate);
	gamemenu_on();
}

void gamemenu_new_game(BOOL bActivate)
{
	int i;

	/// ASSERT: assert(bActivate);

	for(i = 0; i < MAX_PLRS; i++) {
		plr[i]._pmode = PM_QUIT;
		plr[i]._pInvincible = 1;
	}

	deathflag = 0;
	force_redraw = 255;
	scrollrt_draw_game_screen(TRUE);
	gbRunGame = 0;
	gamemenu_off();
}

void gamemenu_quit_game(BOOL bActivate)
{
	/// ASSERT: assert(bActivate);
	gamemenu_new_game(bActivate);
	gbRunGameResult = 0;
}

void gamemenu_load_game(BOOL bActivate)
{
	WNDPROC saveProc;

	/// ASSERT: assert(bActivate);
	/// ASSERT: assert(gbMaxPlayers == 1);
	/// ASSERT: assert(gbValidSaveFile);

	/// ASSERT: assert(ghMainWnd);
	saveProc = SetWindowProc(DisableInputWndProc);
	gamemenu_off();
	SetCursor_(0);
	InitDiabloMsg(10);
	force_redraw = 255;
	DrawAndBlit();
	LoadGame(FALSE);
	ClrDiabloMsg();
	PaletteFadeOut(8);
	deathflag = 0;
	force_redraw = 255;
	DrawAndBlit();
	PaletteFadeIn(8);
	SetCursor_(CURSOR_HAND);
	interface_msg_pump();
	saveProc = SetWindowProc(saveProc);
	/// ASSERT: assert(saveProc == DisableInputWndProc);
}

void gamemenu_save_game(BOOL bActivate)
{
	WNDPROC saveProc;

	/// ASSERT: assert(bActivate);
	/// ASSERT: assert(gbMaxPlayers == 1);

	if(pcurs != CURSOR_HAND) {
		return;
	}
	if(plr[myplr]._pmode == PM_DEATH || deathflag) {
		gamemenu_off();
		return;
	}

	/// ASSERT: assert(ghMainWnd);
	saveProc = SetWindowProc(DisableInputWndProc);
	SetCursor_(0);
	gamemenu_off();
	InitDiabloMsg(11);
	force_redraw = 255;
	DrawAndBlit();
	SaveGame();
	ClrDiabloMsg();
	force_redraw = 255;
	SetCursor_(CURSOR_HAND);
	interface_msg_pump();
	saveProc = SetWindowProc(saveProc);
	/// ASSERT: assert(saveProc == DisableInputWndProc);
}

void gamemenu_restart_town(BOOL bActivate)
{
	/// ASSERT: assert(bActivate);
	NetSendCmd(TRUE, CMD_RETOWN);
}

void gamemenu_options(BOOL bActivate)
{
	/// ASSERT: assert(bActivate);
	gamemenu_get_music();
	gamemenu_get_sound();
	gamemenu_get_gamma();
	gamemenu_get_color_cycling();
	gmenu_set_items(sgOptionMenu, NULL);
}

void gamemenu_get_music()
{
	gamemenu_sound_music_toggle(music_toggle_names, &sgOptionMenu[0], sound_get_or_set_music_volume(1));
}

void gamemenu_sound_music_toggle(char **names, TMenuItem *pItem, int ticks)
{
	if(gbSndInited) {
		pItem->dwFlags |= 0xC0000000;
		pItem->pszStr = names[0];
		gmenu_set_max_ticks(pItem, 17);
		gmenu_set_slider_ticks(pItem, -1600, 0, ticks);
	} else {
		pItem->dwFlags &= ~0xC0000000;
		pItem->pszStr = names[1];
	}
}

void gamemenu_get_sound()
{
	gamemenu_sound_music_toggle(sound_toggle_names, &sgOptionMenu[1], sound_get_or_set_sound_volume(1));
}

void gamemenu_get_color_cycling()
{
	BOOL bCycle;

	bCycle = palette_get_colour_cycling();
	sgOptionMenu[3].pszStr = color_cycling_toggle_names[bCycle & 1];
}

void gamemenu_get_gamma()
{
	gmenu_set_max_ticks(&sgOptionMenu[2], 15);
	gmenu_set_slider_ticks(&sgOptionMenu[2], 30, 100, UpdateGamma(0));
}

void gamemenu_music_volume(BOOL bActivate)
{
	int volume;

	if(bActivate) {
		if(gbMusicOn) {
			gbMusicOn = 0;
			music_stop();
			sound_get_or_set_music_volume(-1600);
		} else {
			gbMusicOn = 1;
			sound_get_or_set_music_volume(0);
			music_start(leveltype);
		}
	} else {
		volume = gamemenu_slider_music_sound(&sgOptionMenu[0]);
		sound_get_or_set_music_volume(volume);
		if(volume == -1600) {
			if(gbMusicOn) {
				gbMusicOn = 0;
				music_stop();
			}
		} else {
			if(!gbMusicOn) {
				gbMusicOn = 1;
				music_start(leveltype);
			}
		}
	}

	gamemenu_get_music();
}

int gamemenu_slider_music_sound(TMenuItem *pItem)
{
	return gmenu_get_slider_ticks(pItem, -1600, 0);
}

void gamemenu_sound_volume(BOOL bActivate)
{
	int volume;

	if(bActivate) {
		if(gbSoundOn) {
			gbSoundOn = 0;
			sound_stop();
			sound_get_or_set_sound_volume(-1600);
		} else {
			gbSoundOn = 1;
			sound_get_or_set_sound_volume(0);
		}
	} else {
		volume = gamemenu_slider_music_sound(&sgOptionMenu[1]);
		sound_get_or_set_sound_volume(volume);
		if(volume == -1600) {
			if(gbSoundOn) {
				gbSoundOn = 0;
				sound_stop();
			}
		} else {
			if(!gbSoundOn) {
				gbSoundOn = 1;
			}
		}
	}

	PlaySFX(IS_TITLEMOV);
	gamemenu_get_sound();
}

void gamemenu_gamma(BOOL bActivate)
{
	int gamma;

	if(bActivate) {
		if(UpdateGamma(0) == 30) {
			gamma = 100;
		} else {
			gamma = 30;
		}
	} else {
		gamma = gamemenu_slider_gamma();
	}

	UpdateGamma(gamma);
	gamemenu_get_gamma();
}

int gamemenu_slider_gamma()
{
	return gmenu_get_slider_ticks(&sgOptionMenu[2], 30, 100);
}

void gamemenu_color_cycling(BOOL bActivate)
{
	BOOL bCycle;

	bCycle = palette_set_color_cycling(!palette_get_colour_cycling());
	sgOptionMenu[3].pszStr = color_cycling_toggle_names[bCycle & 1];
}
