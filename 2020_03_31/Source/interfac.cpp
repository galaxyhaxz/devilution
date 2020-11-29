#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

void *sgpBackCel;
int sgdwProgress;
int progress_id; // idb

const unsigned char progress_bar_colours[3] = { 138u, 43u, 254u };
const int progress_bar_screen_pos[3][2] = { { 53, 37 }, { 53, 421 }, { 53, 37 } };

void interface_msg_pump()
{
	MSG Msg; // [esp+8h] [ebp-1Ch]

	while ( PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) )
	{
		if ( Msg.message != WM_QUIT )
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
}

BOOL IncProgress()
{
	interface_msg_pump();
	sgdwProgress += 15;
	if ( (unsigned int)sgdwProgress > 0x216 )
		sgdwProgress = 534;
	if ( sgpBackCel )
		DrawCutscene();
	return (unsigned int)sgdwProgress >= 0x216;
}

void DrawCutscene()
{
	unsigned int v0; // esi

	lock_buf(1);
	CelDecodeOnly(64, 639, (BYTE *)sgpBackCel, 1, 640);
	v0 = 0;
	if ( sgdwProgress )
	{
		do
			DrawProgress(
				progress_bar_screen_pos[progress_id][0] + v0++ + 64,
				progress_bar_screen_pos[progress_id][1] + 160,
				progress_id);
		while ( v0 < sgdwProgress );
	}
	unlock_buf(1);
	force_redraw = 255;
	scrollrt_draw_game_screen(0);
}

void DrawProgress(int screen_x, int screen_y, int progress_id)
{
	_BYTE *v3; // eax
	signed int v4; // ecx

	v3 = &gpBuffer[PitchTbl[screen_y] + screen_x];
	v4 = 22;
	do
	{
		*v3 = progress_bar_colours[progress_id];
		v3 += 768;
		--v4;
	}
	while ( v4 );
}

void ShowProgress(unsigned int uMsg)
{
	WNDPROC saveProc;

	gbSomebodyWonGameKludge = FALSE;
	plrmsg_delay(TRUE);

	/// ASSERT: assert(ghMainWnd);
	saveProc = SetWindowProc(DisableInputWndProc);

	interface_msg_pump();
	ClearScreenBuffer();
	scrollrt_draw_game_screen(TRUE);
	InitCutscene(uMsg);
	BlackPalette();
	DrawCutscene();
	PaletteFadeIn(8);
	IncProgress();
	sound_init();
	IncProgress();

	switch(uMsg) {
	case WM_DIABLOADGAME:
		IncProgress();
		LoadGame(TRUE);
		IncProgress();
		break;
	case WM_DIABNEWGAME:
		IncProgress();
		FreeGameMem();
		IncProgress();
		pfile_remove_temp_files();
		LoadGameLevel(TRUE, 0);
		IncProgress();
		break;
	case WM_DIABNEXTLVL:
		IncProgress();
		if(gbMaxPlayers == 1) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		FreeGameMem();
		currlevel++;
		leveltype = gnLevelTypeTbl[currlevel];
		/// ASSERT: assert(plr[myplr].plrlevel == currlevel);
		IncProgress();
		LoadGameLevel(FALSE, 0);
		IncProgress();
		break;
	case WM_DIABPREVLVL:
		IncProgress();
		if(gbMaxPlayers == 1) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		IncProgress();
		FreeGameMem();
		currlevel--;
		leveltype = gnLevelTypeTbl[currlevel];
		/// ASSERT: assert(plr[myplr].plrlevel == currlevel);
		IncProgress();
		LoadGameLevel(FALSE, 1);
		IncProgress();
		break;
	case WM_DIABSETLVL:
		SetReturnLvlPos();
		if(gbMaxPlayers == 1) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		setlevel = TRUE;
		leveltype = setlvltype;
		FreeGameMem();
		IncProgress();
		LoadGameLevel(FALSE, 2);
		IncProgress();
		break;
	case WM_DIABRTNLVL:
		if(gbMaxPlayers == 1) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		setlevel = FALSE;
		FreeGameMem();
		IncProgress();
		GetReturnLvlPos();
		LoadGameLevel(FALSE, 3);
		IncProgress();
		break;
	case WM_DIABWARPLVL:
		IncProgress();
		if(gbMaxPlayers == 1) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		FreeGameMem();
		GetPortalLevel();
		IncProgress();
		LoadGameLevel(FALSE, 5);
		IncProgress();
		break;
	case WM_DIABTOWNWARP:
		IncProgress();
		if(gbMaxPlayers == 1) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		FreeGameMem();
		currlevel = plr[myplr].plrlevel;
		leveltype = gnLevelTypeTbl[currlevel];
		/// ASSERT: assert(plr[myplr].plrlevel == currlevel);
		IncProgress();
		LoadGameLevel(FALSE, 6);
		IncProgress();
		break;
	case WM_DIABTWARPUP:
		IncProgress();
		if(gbMaxPlayers == 1) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		FreeGameMem();
		currlevel = plr[myplr].plrlevel;
		leveltype = gnLevelTypeTbl[currlevel];
		/// ASSERT: assert(plr[myplr].plrlevel == currlevel);
		IncProgress();
		LoadGameLevel(FALSE, 7);
		IncProgress();
		break;
	case WM_DIABRETOWN:
		IncProgress();
		if(gbMaxPlayers == 1) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		FreeGameMem();
		currlevel = plr[myplr].plrlevel;
		leveltype = gnLevelTypeTbl[currlevel];
		/// ASSERT: assert(plr[myplr].plrlevel == currlevel);
		IncProgress();
		LoadGameLevel(FALSE, 0);
		IncProgress();
		break;
	}

	/// ASSERT: assert(ghMainWnd);

	PaletteFadeOut(8);
	FreeInterface();

	saveProc = SetWindowProc(saveProc);
	/// ASSERT: assert(saveProc == DisableInputWndProc);

	NetSendCmdLocParam1(TRUE, CMD_PLAYER_JOINLEVEL, plr[myplr].WorldX, plr[myplr].WorldY, plr[myplr].plrlevel);
	plrmsg_delay(FALSE);
	ResetPal();

	if(gbSomebodyWonGameKludge && plr[myplr].plrlevel == 16) {
		PrepDoEnding();
	}

	gbSomebodyWonGameKludge = FALSE;
}

void FreeInterface()
{
	void *v0; // ecx

	v0 = sgpBackCel;
	sgpBackCel = 0;
	mem_free_dbg(v0);
}

void InitCutscene(unsigned int uMsg)
{
	/// ASSERT: assert(! sgpBackCel);

	switch(uMsg) {
	case WM_DIABNEXTLVL:
		switch(gnLevelTypeTbl[currlevel]) {
		case 0:
			sgpBackCel = DiabLoad("Gendata\\Cuttt.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cuttt.pal");
			progress_id = 1;
			break;
		case 1:
			sgpBackCel = DiabLoad("Gendata\\Cutl1d.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cutl1d.pal");
			progress_id = 0;
			break;
		case 2:
			sgpBackCel = DiabLoad("Gendata\\Cut2.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cut2.pal");
			progress_id = 2;
			break;
		case 3:
			sgpBackCel = DiabLoad("Gendata\\Cut3.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cut3.pal");
			progress_id = 1;
			break;
		case 4:
			if(currlevel < 15) {
				sgpBackCel = DiabLoad("Gendata\\Cut4.CEL", NULL, 'PROG');
				LoadPalette("Gendata\\Cut4.pal");
				progress_id = 1;
			} else {
				sgpBackCel = DiabLoad("Gendata\\Cutgate.CEL", NULL, 'PROG');
				LoadPalette("Gendata\\Cutgate.pal");
				progress_id = 1;
			}
			break;
		default:
			sgpBackCel = DiabLoad("Gendata\\Cutl1d.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cutl1d.pal");
			progress_id = 0;
			break;
		}
		break;
	case WM_DIABPREVLVL:
		if(gnLevelTypeTbl[currlevel - 1] == 0) {
			sgpBackCel = DiabLoad("Gendata\\Cuttt.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cuttt.pal");
			progress_id = 1;
		} else {
			switch(gnLevelTypeTbl[currlevel]) {
			case 0:
				sgpBackCel = DiabLoad("Gendata\\Cuttt.CEL", NULL, 'PROG');
				LoadPalette("Gendata\\Cuttt.pal");
				progress_id = 1;
				break;
			case 1:
				sgpBackCel = DiabLoad("Gendata\\Cutl1d.CEL", NULL, 'PROG');
				LoadPalette("Gendata\\Cutl1d.pal");
				progress_id = 0;
				break;
			case 2:
				sgpBackCel = DiabLoad("Gendata\\Cut2.CEL", NULL, 'PROG');
				LoadPalette("Gendata\\Cut2.pal");
				progress_id = 2;
				break;
			case 3:
				sgpBackCel = DiabLoad("Gendata\\Cut3.CEL", NULL, 'PROG');
				LoadPalette("Gendata\\Cut3.pal");
				progress_id = 1;
				break;
			case 4:
				sgpBackCel = DiabLoad("Gendata\\Cut4.CEL", NULL, 'PROG');
				LoadPalette("Gendata\\Cut4.pal");
				progress_id = 1;
				break;
			default:
				sgpBackCel = DiabLoad("Gendata\\Cutl1d.CEL", NULL, 'PROG');
				LoadPalette("Gendata\\Cutl1d.pal");
				progress_id = 0;
				break;
			}
		}
		break;
	case WM_DIABSETLVL:
		if(setlvlnum == SL_BONECHAMB) {
			sgpBackCel = DiabLoad("Gendata\\Cut2.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cut2.pal");
			progress_id = 2;
		} else if(setlvlnum == SL_VILEBETRAYER) {
			sgpBackCel = DiabLoad("Gendata\\Cutportr.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cutportr.pal");
			progress_id = 1;
		} else {
			sgpBackCel = DiabLoad("Gendata\\Cutl1d.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cutl1d.pal");
			progress_id = 0;
		}
		break;
	case WM_DIABRTNLVL:
		if(setlvlnum == SL_BONECHAMB) {
			sgpBackCel = DiabLoad("Gendata\\Cut2.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cut2.pal");
			progress_id = 2;
		} else if(setlvlnum == SL_VILEBETRAYER) {
			sgpBackCel = DiabLoad("Gendata\\Cutportr.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cutportr.pal");
			progress_id = 1;
		} else {
			sgpBackCel = DiabLoad("Gendata\\Cutl1d.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cutl1d.pal");
			progress_id = 0;
		}
		break;
	case WM_DIABWARPLVL:
		sgpBackCel = DiabLoad("Gendata\\Cutportl.CEL", NULL, 'PROG');
		LoadPalette("Gendata\\Cutportl.pal");
		progress_id = 1;
		break;
	case WM_DIABLOADGAME:
		sgpBackCel = DiabLoad("Gendata\\Cutstart.CEL", NULL, 'PROG');
		LoadPalette("Gendata\\Cutstart.pal");
		progress_id = 1;
		break;
	case WM_DIABNEWGAME:
		sgpBackCel = DiabLoad("Gendata\\Cutstart.CEL", NULL, 'PROG');
		LoadPalette("Gendata\\Cutstart.pal");
		progress_id = 1;
		break;
	case WM_DIABTOWNWARP:
	case WM_DIABTWARPUP:
		switch(gnLevelTypeTbl[plr[myplr].plrlevel]) {
		case 0:
			sgpBackCel = DiabLoad("Gendata\\Cuttt.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cuttt.pal");
			progress_id = 1;
			break;
		case 2:
			sgpBackCel = DiabLoad("Gendata\\Cut2.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cut2.pal");
			progress_id = 2;
			break;
		case 3:
			sgpBackCel = DiabLoad("Gendata\\Cut3.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cut3.pal");
			progress_id = 1;
			break;
		case 4:
			sgpBackCel = DiabLoad("Gendata\\Cut4.CEL", NULL, 'PROG');
			LoadPalette("Gendata\\Cut4.pal");
			progress_id = 1;
			break;
		}
		break;
	case WM_DIABRETOWN:
		sgpBackCel = DiabLoad("Gendata\\Cuttt.CEL", NULL, 'PROG');
		LoadPalette("Gendata\\Cuttt.pal");
		progress_id = 1;
		break;
	default:
		app_fatal("Unknown progress mode");
		break;
	}

	sgdwProgress = 0;
}
