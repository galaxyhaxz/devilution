#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"
#include "../DiabloUI/diabloui.h"

char gszHero[16];

/* data */

int menu_music_track_id = 5; // idb

void mainmenu_refresh_music()
{
	int v0; // eax

	music_start(menu_music_track_id);
	v0 = menu_music_track_id;
	do
	{
		if ( ++v0 == 6 )
			v0 = 0;
	}
	while ( !v0 || v0 == 1 );
	menu_music_track_id = v0;
}

void __stdcall mainmenu_create_hero(int a1, int a2, int a3, int a4, char *name_1, char *name_2)
{
	if ( UiValidPlayerName(name_2) )
		pfile_create_save_file(name_1, name_2);
}

int __stdcall mainmenu_select_hero_dialog(int u1, int u2, int u3, int u4, int mode, char *cname, int clen, char *cdesc, int cdlen, int *multi) /* fix args */
{
	int v10; // eax
	int a6; // [esp+8h] [ebp-8h]
	int a5; // [esp+Ch] [ebp-4h]

	a6 = 1;
	a5 = 0;
	if ( gbMaxPlayers == 1 )
	{
		if ( !UiSelHeroSingDialog(
				  pfile_ui_set_hero_infos,
				  pfile_ui_save_create,
				  pfile_delete_save,
				  pfile_ui_set_class_stats,
				  &a5,
				  gszHero,
				  &gnDifficulty) )
			app_fatal("Unable to display SelHeroSing");
		if ( a5 == SELHERO_CONTINUE )
		{
			gbLoadGame = 1;
			goto LABEL_6;
		}
		gbLoadGame = 0; // SELHERO_NEW_DUNGEON
	}
	else if ( !UiSelHeroMultDialog(
				   pfile_ui_set_hero_infos,
				   pfile_ui_save_create,
				   pfile_delete_save,
				   pfile_ui_set_class_stats,
				   &a5,
				   &a6,
				   gszHero) )
	{
		app_fatal("Can't load multiplayer dialog");
	}
	if ( a5 == SELHERO_PREVIOUS )
	{
		SErrSetLastError(1223);
		return 0;
	}
LABEL_6:
	pfile_create_player_description(cdesc, cdlen);
	if ( multi )
	{
		if ( mode == 'BNET' )
			v10 = a6 || !plr[myplr].pBattleNet;
		else
			v10 = a6;
		*multi = v10;
	}
	if ( cname )
	{
		if ( clen )
			SStrCopy(cname, gszHero, clen);
	}
	return 1;
}

void mainmenu_loop()
{
	BOOL done;
	int menu;

	mainmenu_refresh_music();
	done = FALSE;

	while(!done) {
		menu = 0;
		if(!UiMainMenuDialog(gszProductName, &menu, effects_play_sound, 30)) {
			app_fatal("Unable to display mainmenu");
		}
		switch(menu) {
		case MAINMENU_SINGLE_PLAYER:
			if(!mainmenu_single_player()) {
				done = TRUE;
			}
			break;
		case MAINMENU_MULTIPLAYER:
			if(!mainmenu_multi_player()) {
				done = TRUE;
			}
			break;
		case MAINMENU_REPLAY_INTRO:
		case MAINMENU_ATTRACT_MODE:
			if(gbActive) {
				mainmenu_play_intro();
			}
			break;
		case MAINMENU_SHOW_CREDITS:
			UiCreditsDialog(16);
			break;
		case MAINMENU_EXIT_DIABLO:
			done = TRUE;
			break;
		}
	}

	music_stop();
}

int mainmenu_single_player()
{
	gbMaxPlayers = 1;
	return mainmenu_init_menu(SELHERO_NEW_DUNGEON);
}

int mainmenu_init_menu(int a1)
{
	int v1; // esi
	int v3; // esi

	v1 = a1;
	if ( a1 == SELHERO_PREVIOUS )
		return 1;
	music_stop();
	v3 = StartGame(v1 != SELHERO_CONTINUE, v1 != SELHERO_CONNECT);
	if ( v3 )
		mainmenu_refresh_music();
	return v3;
}

int mainmenu_multi_player()
{
	gbMaxPlayers = 4;
	return mainmenu_init_menu(SELHERO_CONNECT);
}

void mainmenu_play_intro()
{
	music_stop();
	play_movie("gendata\\diablo1.smk", 1);
	mainmenu_refresh_music();
}
