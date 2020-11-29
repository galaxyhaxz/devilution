#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

int sfxdelay; // weak
int sfxdnum;
HANDLE sghStream;
TSFX *sgpStreamSFX;

const char monster_action_sounds[] = { 'a', 'h', 'd', 's' }; // idb

/* data */

TSFX sgSFX[NUM_SFX] =
{
#include "Data/xl_sfx.cpp"
};

BOOL effect_is_playing(int nSFX)
{
	TSFX *pSFX;

	/// ASSERT: assert(nSFX < NUM_SFX);
	pSFX = &sgSFX[nSFX];

	if(pSFX->pSnd != NULL) {
		return snd_playing(pSFX->pSnd);
	}
	if(pSFX->bFlags & 1) {
		return pSFX == sgpStreamSFX;
	}

	return FALSE;
}

void stream_stop()
{
	if(sghStream != NULL) {
		SFileDdaEnd(sghStream);
		SFileCloseFile(sghStream);
		sghStream = NULL;
		sgpStreamSFX = NULL;
	}
}

static void stream_play(TSFX *pSFX, long lVolume, long lPan)
{
	BOOL bSuccess;

	/// ASSERT: assert(pSFX);
	/// ASSERT: assert(pSFX->bFlags & sfx_STREAM);

	stream_stop();

	lVolume += sound_get_or_set_sound_volume(1);
	if(lVolume < VOLUME_MIN) {
		return;
	}
	if(lVolume > VOLUME_MAX) {
		lVolume = VOLUME_MAX;
	}

#ifdef _DEBUG
	SFileEnableDirectAccess(FALSE);
#endif
	bSuccess = SFileOpenFile(pSFX->pszName, &sghStream);
#ifdef _DEBUG
	SFileEnableDirectAccess(TRUE);
#endif

	if(!bSuccess) {
		sghStream = NULL;
		return;
	}
	if(!SFileDdaBeginEx(sghStream, 0x40000, 0, 0, lVolume, lPan, 0)) {
		stream_stop();
		return;
	}

	sgpStreamSFX = pSFX;
}

static void stream_update()
{
	DWORD dwStart, dwEnd;

	if(sghStream == NULL) {
		return;
	}
	if(!SFileDdaGetPos(sghStream, &dwStart, &dwEnd)) {
		return;
	}
	if(dwStart < dwEnd) {
		return;
	}

	stream_stop();
}

static void sfx_stop()
{
	int i;
	TSFX *pSFX;

	pSFX = sgSFX;
	i = NUM_SFX;
	while(i--) {
		if(pSFX->pSnd != NULL) {
			snd_stop_snd(pSFX->pSnd);
		}
		pSFX++;
	}
}

void InitMonsterSND(int monst)
{
	int mtype, mode, nr;
	TSnd *pSnd;
	char *pszFile;
	char szName[MAX_PATH];

	if(!gbSndInited) {
		return;
	}

	mtype = Monsters[monst].mtype;

	for(mode = 0; mode < 4; mode++) {
		if(monster_action_sounds[mode] == 's' && !monsterdata[mtype].snd_special) {
			continue;
		}
		for(nr = 0; nr < 2; nr++) {
			sprintf(szName, monsterdata[mtype].sndfile, monster_action_sounds[mode], nr + 1);
			pszFile = (char *)DiabloAllocPtr(strlen(szName) + 1);
			strcpy(pszFile, szName);
			pSnd = sound_file_load(pszFile);
			Monsters[monst].Snds[mode][nr] = pSnd;
			if(pSnd == NULL) {
				MemFreeDbg(pszFile);
			}
		}
	}
}

void FreeMonsterSnd()
{
	int mi, mode, nr;
	TSnd *pSnd;
	char *pszFile;

	for(mi = 0; mi < nummtypes; mi++) {
		for(mode = 0; mode < 4; mode++) {
			for(nr = 0; nr < 2; nr++) {
				pSnd = Monsters[mi].Snds[mode][nr];
				if(pSnd == NULL) {
					continue;
				}
				Monsters[mi].Snds[mode][nr] = NULL;
				pszFile = pSnd->sound_path;
				pSnd->sound_path = NULL;
				sound_file_cleanup(pSnd);
				MemFreeDbg(pszFile);
			}
		}
	}
}

static BOOL calc_snd_position(int x, int y, long *plVolume, long *plPan)
{
	long volume, pan;

	x -= plr[myplr].WorldX;
	y -= plr[myplr].WorldY;

	pan = x - y;
	*plPan = pan << 8;
	if(abs(*plPan) > 6400) {
		return FALSE;
	}

	volume = abs(x) > abs(y) ? abs(x) : abs(y);
	*plVolume = volume << 6;
	if(*plVolume >= 6400) {
		return FALSE;
	}

	*plVolume = -*plVolume;
	return TRUE;
}

static void PlaySFX_priv(TSFX *pSFX, BOOL loc, int x, int y)
{
	long lPan, lVolume;

	if(plr[myplr].pLvlLoad != 0 && gbMaxPlayers != 1) {
		return;
	}
	if(!gbSndInited || !gbSoundOn) {
		return;
	}
	if(gbBufferMsgs != 0) {
		return;
	}
	if(!(pSFX->bFlags & 3) && pSFX->pSnd != NULL && snd_playing(pSFX->pSnd)) {
		return;
	}

	lPan = 0;
	lVolume = 0;
	if(loc && !calc_snd_position(x, y, &lVolume, &lPan)) {
		return;
	}

	if(pSFX->bFlags & 1) {
		stream_play(pSFX, lVolume, lPan);
	} else {
		if(pSFX->pSnd == NULL) {
			pSFX->pSnd = sound_file_load(pSFX->pszName);
		}
		if(pSFX->pSnd != NULL) {
			snd_play_snd(pSFX->pSnd, lVolume, lPan);
		}
	}
}

void PlayEffect(int i, int mode)
{
	int mi, nr;
	long lPan, lVolume;
	TSnd *pSnd;

	if(plr[myplr].pLvlLoad != 0) {
		return;
	}

	nr = random(164, 2);

	if(!gbSndInited || !gbSoundOn) {
		return;
	}
	if(gbBufferMsgs != 0) {
		return;
	}

	mi = monster[i]._mMTidx;
	pSnd = Monsters[mi].Snds[mode][nr];
	if(pSnd == NULL) {
#ifdef _DEBUG
		app_fatal("Monster sound problem\n:%s playing %i", Monsters[mi].MData->mName, mode);
#endif
		return;
	}
	if(snd_playing(pSnd)) {
		return;
	}
	if(!calc_snd_position(monster[i]._mx, monster[i]._my, &lVolume, &lPan)) {
		return;
	}

	snd_play_snd(pSnd, lVolume, lPan);
}

static int RndSFX(int psfx)
{
	int nRand;

	if(psfx == PS_WARR69) {
		nRand = 2;
	} else if(psfx == PS_WARR14) {
		nRand = 3;
	} else if(psfx == PS_WARR15) {
		nRand = 3;
	} else if(psfx == PS_WARR16) {
		nRand = 3;
	} else if(psfx == PS_MAGE69) {
		nRand = 2;
	} else if(psfx == PS_ROGUE69) {
		nRand = 2;
	} else if(psfx == PS_SWING) {
		nRand = 2;
	} else if(psfx == LS_ACID) {
		nRand = 2;
	} else if(psfx == IS_FMAG) {
		nRand = 2;
	} else if(psfx == IS_MAGIC) {
		nRand = 2;
	} else if(psfx == IS_BHIT) {
		nRand = 2;
	} else if(psfx == PS_WARR2) {
		nRand = 3;
	} else {
		return psfx;
	}

	return random(165, nRand) + psfx;
}

void PlaySFX(int psfx)
{
	psfx = RndSFX(psfx);
	/// ASSERT: assert(psfx < NUM_SFX);
	PlaySFX_priv(&sgSFX[psfx], FALSE, 0, 0);
}

void PlaySfxLoc(int psfx, int x, int y)
{
	psfx = RndSFX(psfx);
	/// ASSERT: assert(psfx < NUM_SFX);

	if(psfx >= PS_WALK1 && psfx <= PS_WALK4 && sgSFX[psfx].pSnd != NULL) {
		sgSFX[psfx].pSnd->start_tc = 0;
	}

	PlaySFX_priv(&sgSFX[psfx], TRUE, x, y);
}

void sound_stop()
{
	int mi, mode, nr;

	snd_update(TRUE);
	stream_stop();
	sfx_stop();

	for(mi = 0; mi < nummtypes; mi++) {
		for(mode = 0; mode < 4; mode++) {
			for(nr = 0; nr < 2; nr++) {
				snd_stop_snd(Monsters[mi].Snds[mode][nr]);
			}
		}
	}
}

void sound_update()
{
	if(!gbSndInited) {
		return;
	}

	snd_update(FALSE);
	stream_update();
}

void effects_cleanup_sfx()
{
	int i;

	sound_stop();

	for(i = 0; (DWORD)i < NUM_SFX; i++) {
		if(sgSFX[i].pSnd == NULL) {
			continue;
		}
		sound_file_cleanup(sgSFX[i].pSnd);
		sgSFX[i].pSnd = NULL;
	}
}

static void priv_sound_init(BYTE bLoadMask)
{
	BYTE pc;
	int i;

	if(!gbSndInited) {
		return;
	}

	pc = bLoadMask & (0x20 | 0x10 | 0x40);
	bLoadMask ^= pc;

	for(i = 0; (DWORD)i < NUM_SFX; i++) {
		if(sgSFX[i].pSnd != NULL) {
			continue;
		}
		if(sgSFX[i].bFlags & 1) {
			continue;
		}
		if(bLoadMask != 0 && !(sgSFX[i].bFlags & bLoadMask)) {
			continue;
		}
		if(sgSFX[i].bFlags & (0x20 | 0x10 | 0x40) && !(sgSFX[i].bFlags & pc)) {
			continue;
		}
		sgSFX[i].pSnd = sound_file_load(sgSFX[i].pszName);
	}
}

void sound_init()
{
	BYTE bLoadMask;

	bLoadMask = 0;

	if(gbMaxPlayers > 1) {
		bLoadMask = 0x20 | 0x10 | 0x40;
	} else {
		if(plr[myplr]._pClass == PC_WARRIOR) {
			bLoadMask = 0x20;
		} else if(plr[myplr]._pClass == PC_ROGUE) {
			bLoadMask = 0x10;
		} else if(plr[myplr]._pClass == PC_SORCERER) {
			bLoadMask = 0x40;
		} else {
			app_fatal("effects:1");
		}
	}

	priv_sound_init(bLoadMask);
}

void ui_sound_init()
{
	priv_sound_init(4);
}

void __stdcall effects_play_sound(const char *pszFile)
{
	int i;

	if(!gbSndInited || !gbSoundOn) {
		return;
	}

	for(i = 0; (DWORD)i < NUM_SFX; i++) {
		if(_strcmpi(sgSFX[i].pszName, pszFile) == 0 && sgSFX[i].pSnd != NULL) {
			if(!snd_playing(sgSFX[i].pSnd)) {
				snd_play_snd(sgSFX[i].pSnd, 0, 0);
			}
			return;
		}
	}
}
