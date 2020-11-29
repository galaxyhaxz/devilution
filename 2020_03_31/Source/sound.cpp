#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

IDirectSoundBuffer *DSBs[8];
IDirectSound *sglpDS;
char gbSndInited;
long sglMusicVolume;
long sglSoundVolume;
HMODULE hDsound_dll; // idb
HANDLE sghMusic;
IDirectSoundBuffer *sglpDSB;

/* data */

char gbMusicOn = 1; // weak
char gbSoundOn = 1; // weak
char gbDupSounds = 1; // weak
int sgnMusicTrack = 6;
char *sgszMusicTracks[6] =
{
  "Music\\DTowne.wav",
  "Music\\DLvlA.wav",
  "Music\\DLvlB.wav",
  "Music\\DLvlC.wav",
  "Music\\DLvlD.wav",
  "Music\\Dintro.wav"
};
char unk_volume[4][2] =
{
	{ 15, -16 },
	{ 15, -16 },
	{ 30, -31 },
	{ 30, -31 }
};

void snd_update(BOOL bStopAll)
{
	int i;
	HRESULT hDSVal;
	DWORD dwStatus;

	for(i = 0; (DWORD)i < 8; i++) {
		if(DSBs[i] == NULL) {
			continue;
		}
		if(!bStopAll) {
			hDSVal = DSBs[i]->GetStatus(&dwStatus);
			if(hDSVal == DS_OK && dwStatus == DSBSTATUS_PLAYING) {
				continue;
			}
		}
		DSBs[i]->Stop();
		DSBs[i]->Release();
		DSBs[i] = NULL;
	}
}

static IDirectSoundBuffer *sound_dup_channel(IDirectSoundBuffer *pDSB)
{
	int i;
	HRESULT hDSVal;

	if(!gbDupSounds) {
		return NULL;
	}

	for(i = 0; (DWORD)i < 8; i++) {
		if(DSBs[i] == NULL) {
			hDSVal = sglpDS->DuplicateSoundBuffer(pDSB, &DSBs[i]);
			if(hDSVal == DS_OK) {
				return DSBs[i];
			}
			break;
		}
	}

	return NULL;
}

static void snd_get_volume(const char *pszKey, long *plVolume)
{
	DWORD dwTemp;

	dwTemp = *plVolume;
	if(!SRegLoadValue("Diablo", pszKey, 0, &dwTemp)) {
		dwTemp = VOLUME_MAX;
	}
	*plVolume = dwTemp;

	if(*plVolume < VOLUME_MIN) {
		*plVolume = VOLUME_MIN;
	} else if(*plVolume > VOLUME_MAX) {
		*plVolume = VOLUME_MAX;
	}

	*plVolume -= *plVolume % 100;
}

static void snd_set_volume(const char *pszKey, long lVolume)
{
	SRegSaveValue("Diablo", pszKey, 0, lVolume);
}

static BOOL sound_file_reload(TSnd *pSnd, IDirectSoundBuffer *pDSB)
{
	BOOL rv;
	HRESULT hDSVal;
	HANDLE hsFile;
	DWORD dwAudioBytes1, dwAudioBytes2;
	void *vAudioPtr1, *vAudioPtr2;

	/// ASSERT: assert(pSnd);
	/// ASSERT: assert(pDSB);

	if(pDSB->Restore()) {
		return FALSE;
	}

	rv = FALSE;
	WOpenFile(pSnd->sound_path, &hsFile, 0);
	WSetFilePointer(hsFile, pSnd->chunk.dwOffset, NULL, 0);

	hDSVal = pDSB->Lock(0, pSnd->chunk.dwSize, &vAudioPtr1, &dwAudioBytes1, &vAudioPtr2, &dwAudioBytes2, 0);
	if(hDSVal != DS_OK) {
#ifdef _DEBUG
		DSErrMsg(hDSVal, 163, "C:\\Diablo\\Direct\\SOUND.CPP");
#endif
	} else {
		WReadFile(hsFile, (char *)vAudioPtr1, dwAudioBytes1);
		hDSVal = pDSB->Unlock(vAudioPtr1, dwAudioBytes1, vAudioPtr2, dwAudioBytes2);
		if(hDSVal != DS_OK) {
#ifdef _DEBUG
			DSErrMsg(hDSVal, 169, "C:\\Diablo\\Direct\\SOUND.CPP");
#endif
		} else {
			rv = TRUE;
		}
	}

	WCloseFile(hsFile);
	return rv;
}

void snd_stop_snd(TSnd *pSnd)
{
	if(pSnd == NULL) {
		return;
	}
	if(pSnd->DSB == NULL) {
		return;
	}

	pSnd->DSB->Stop();
}

BOOL snd_playing(TSnd *pSnd)
{
	HRESULT hDSVal;
	DWORD dwStatus;

	if(pSnd == NULL) {
		return FALSE;
	}
	if(pSnd->DSB == NULL) {
		return FALSE;
	}

	hDSVal = pSnd->DSB->GetStatus(&dwStatus);
	if(hDSVal != DS_OK) {
#ifdef _DEBUG
		DSErrMsg(hDSVal, 195, "C:\\Diablo\\Direct\\SOUND.CPP");
#endif
		return FALSE;
	}

	return dwStatus == DSBSTATUS_PLAYING;
}

void snd_play_snd(TSnd *pSnd, long lVolume, long lPan)
{
	DWORD dwTicks;
	HRESULT hDSVal;
	IDirectSoundBuffer *pDSB;

	if(pSnd == NULL) {
		return;
	}
	if(!gbSoundOn) {
		return;
	}
	pDSB = pSnd->DSB;
	if(pDSB == NULL) {
		return;
	}

	dwTicks = GetTickCount();
	if(dwTicks - pSnd->start_tc < 80) {
		GetTickCount(); /// BUGFIX: call has no purpose
		return;
	}
	if(snd_playing(pSnd)) {
		pDSB = sound_dup_channel(pSnd->DSB);
		if(pDSB == NULL) {
			return;
		}
	}

	lVolume += sglSoundVolume;
	if(lVolume < VOLUME_MIN) {
		lVolume = VOLUME_MIN;
	} else if(lVolume > VOLUME_MAX) {
		lVolume = VOLUME_MAX;
	}

	hDSVal = pDSB->SetVolume(lVolume);
#ifdef _DEBUG
	if(hDSVal != DS_OK) {
		DSErrMsg(hDSVal, 235, "C:\\Diablo\\Direct\\SOUND.CPP");
	}
#endif
	hDSVal = pDSB->SetPan(lPan);
#ifdef _DEBUG
	if(hDSVal != DS_OK) {
		DSErrMsg(hDSVal, 238, "C:\\Diablo\\Direct\\SOUND.CPP");
	}
#endif

	hDSVal = pDSB->Play(0, 0, 0);
	if(hDSVal != DSERR_BUFFERLOST) {
		if(hDSVal != DS_OK) {
			DSErrMsg(hDSVal, 261, "C:\\Src\\Diablo\\Source\\SOUND.CPP");
		}
	} else if(sound_file_reload(pSnd, pDSB)) {
		hDSVal = pDSB->Play(0, 0, 0);
	}

	pSnd->start_tc = dwTicks;
}

static void sound_CreateSoundBuffer(TSnd *pSnd)
{
	HRESULT hDSVal;
	DSBUFFERDESC dsbd;

	/// ASSERT: assert(sglpDS);

	memset(&dsbd, 0, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_STATIC;
	dsbd.dwBufferBytes = pSnd->chunk.dwSize;
	dsbd.lpwfxFormat = &pSnd->fmt;

	hDSVal = sglpDS->CreateSoundBuffer(&dsbd, &pSnd->DSB, NULL);
	if(hDSVal != DS_OK) {
		DSErrMsg(hDSVal, 282, "C:\\Src\\Diablo\\Source\\SOUND.CPP");
	}
}

TSnd *sound_file_load(const char *pszFile)
{
	HRESULT hDSVal;
	HANDLE hsFile;
	DWORD dwAudioBytes1, dwAudioBytes2;
	void *vAudioPtr1, *vAudioPtr2;
	TSnd *pSnd;
	BYTE *p;

	if(sglpDS == NULL) {
		return NULL;
	}

	WOpenFile((char *)pszFile, &hsFile, 0);

	pSnd = (TSnd *)DiabloAllocPtr(sizeof(*pSnd));
	memset(pSnd, 0, sizeof(*pSnd));
	pSnd->sound_path = (char *)pszFile;
	pSnd->start_tc = GetTickCount() - 81;

	p = (BYTE *)LoadWaveFile(hsFile, &pSnd->fmt, &pSnd->chunk);
	if(p == NULL) {
		app_fatal("Invalid sound format on file %s", pSnd->sound_path);
	}

	sound_CreateSoundBuffer(pSnd);

	hDSVal = pSnd->DSB->Lock(0, pSnd->chunk.dwSize, &vAudioPtr1, &dwAudioBytes1, &vAudioPtr2, &dwAudioBytes2, 0);
	if(hDSVal != DS_OK) {
		DSErrMsg(hDSVal, 318, "C:\\Src\\Diablo\\Source\\SOUND.CPP");
	}

	memcpy(vAudioPtr1, &p[pSnd->chunk.dwOffset], dwAudioBytes1);

	hDSVal = pSnd->DSB->Unlock(vAudioPtr1, dwAudioBytes1, vAudioPtr2, dwAudioBytes2);
	if(hDSVal != DS_OK) {
		DSErrMsg(hDSVal, 325, "C:\\Src\\Diablo\\Source\\SOUND.CPP");
	}

	MemFreeDbg(p);
	WCloseFile(hsFile);
	return pSnd;
}

void sound_file_cleanup(TSnd *pSnd)
{
	if(pSnd != NULL) {
		if(pSnd->DSB != NULL) {
			pSnd->DSB->Stop();
			pSnd->DSB->Release();
			pSnd->DSB = NULL;
		}
		MemFreeDbg(pSnd);
	}
}

static void sound_create_primary_buffer(HANDLE hMusic)
{
	HRESULT hDSVal;
	DSCAPS caps;
	DSBUFFERDESC dsbd;
	WAVEFORMATEX fmt;

	/// ASSERT: assert(sglpDS);

	if(hMusic == NULL) {
		memset(&dsbd, 0, sizeof(dsbd));
		dsbd.dwSize = sizeof(dsbd);
		dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
		hDSVal = sglpDS->CreateSoundBuffer(&dsbd, &sglpDSB, NULL);
		if(hDSVal != DS_OK) {
			DSErrMsg(hDSVal, 375, "C:\\Src\\Diablo\\Source\\SOUND.CPP");
		}
	}

	if(sglpDSB == NULL) {
		return;
	}

	caps.dwSize = sizeof(caps);
	hDSVal = sglpDS->GetCaps(&caps);
	if(hDSVal != DS_OK) {
		DSErrMsg(hDSVal, 383, "C:\\Src\\Diablo\\Source\\SOUND.CPP");
	}

	if(hMusic == NULL || !LoadWaveFormat(hMusic, &fmt)) {
		memset(&fmt, 0, sizeof(fmt));
		fmt.wFormatTag = WAVE_FORMAT_PCM;
		fmt.nSamplesPerSec = 22050;
		fmt.wBitsPerSample = 16;
		fmt.nChannels = 2;
		fmt.cbSize = 0;
	}
	fmt.nChannels = 2;
	fmt.nBlockAlign = fmt.nChannels * fmt.wBitsPerSample / 8;
	fmt.nAvgBytesPerSec = fmt.nSamplesPerSec * fmt.nBlockAlign;
	hDSVal = sglpDSB->SetFormat(&fmt);
#ifdef _DEBUG
	if(hDSVal != DS_OK && hDSVal != DSERR_BADFORMAT) {
		if(hDSVal != DS_OK) {
			DSErrMsg(hDSVal, 383, "C:\\Diablo\\Direct\\SOUND.CPP");
		}
	}
#endif
}

static HRESULT sound_DirectSoundCreate(GUID FAR *lpGUID, LPDIRECTSOUND FAR *lplpDS, IUnknown FAR *pUnkOuter)
{
	DSCREATEPROC dscp;

	if(hDsound_dll == NULL) {
		hDsound_dll = LoadLibrary("dsound.dll");
		if(hDsound_dll == NULL) {
			ErrDlg(IDD_DIALOG5, GetLastError(), "C:\\Src\\Diablo\\Source\\SOUND.CPP", 422);
		}
	}
	dscp = (DSCREATEPROC)GetProcAddress(hDsound_dll, "DirectSoundCreate");
	if(dscp == NULL) {
		ErrDlg(IDD_DIALOG5, GetLastError(), "C:\\Src\\Diablo\\Source\\SOUND.CPP", 427);
	}

	return dscp(lpGUID, lplpDS, pUnkOuter);
}

void snd_init(HWND hWnd)
{
	HRESULT hDSVal;
	BOOL bSuccess;

	snd_get_volume("Sound Volume", &sglSoundVolume);
	gbSoundOn = sglSoundVolume > VOLUME_MIN;
	snd_get_volume("Music Volume", &sglMusicVolume);
	gbMusicOn = sglMusicVolume > VOLUME_MIN;

	/// ASSERT: assert(! sglpDS);
	hDSVal = sound_DirectSoundCreate(NULL, &sglpDS, NULL);
	if(hDSVal != DS_OK) {
		sglpDS = NULL;
	}
	if(sglpDS != NULL && sglpDS->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE) == DS_OK) {
		sound_create_primary_buffer(NULL);
	}

	bSuccess = SVidInitialize(sglpDS);
	/// ASSERT: assert(! sglpDS || bSuccess);

	bSuccess = SFileDdaInitialize(sglpDS);
	/// ASSERT: assert(! sglpDS || bSuccess);

	gbSndInited = sglpDS != NULL;
}

void sound_cleanup()
{
	snd_update(TRUE);
	SVidDestroy();
	SFileDdaDestroy();

	if(sglpDS != NULL) {
		sglpDS->Release();
		sglpDS = NULL;
	}
	if(gbSndInited) {
		gbSndInited = 0;
		snd_set_volume("Sound Volume", sglSoundVolume);
		snd_set_volume("Music Volume", sglMusicVolume);
	}
}

void music_stop()
{
	if(sghMusic != NULL) {
		SFileDdaEnd(sghMusic);
		SFileCloseFile(sghMusic);
		sghMusic = NULL;
		sgnMusicTrack = NUM_MUSIC;
	}
}

void music_start(int nTrack)
{
	BOOL bSuccess;

	/// ASSERT: assert((DWORD) nTrack < NUM_MUSIC);

	music_stop();

	if(sglpDS == NULL) {
		return;
	}
	if(!gbMusicOn) {
		return;
	}

#ifdef _DEBUG
	SFileEnableDirectAccess(FALSE);
#endif
	bSuccess = SFileOpenFile(sgszMusicTracks[nTrack], &sghMusic);
#ifdef _DEBUG
	SFileEnableDirectAccess(TRUE);
#endif

	sound_create_primary_buffer(sghMusic);

	if(!bSuccess) {
		sghMusic = NULL;
		return;
	}

	SFileDdaBeginEx(sghMusic, 0x40000, 0x40000, 0, sglMusicVolume, 0, 0);
	sgnMusicTrack = nTrack;
}

void sound_disable_music(BOOL disable)
{
	if(disable) {
		music_stop();
	} else if(sgnMusicTrack != NUM_MUSIC) {
		music_start(sgnMusicTrack);
	}
}

long sound_get_or_set_music_volume(long lVolume)
{
	if(lVolume == 1) {
		return sglMusicVolume;
	}

	/// ASSERT: assert(lVolume >= VOLUME_MIN);
	/// ASSERT: assert(lVolume <= VOLUME_MAX);
	sglMusicVolume = lVolume;

	if(sghMusic != NULL) {
		SFileDdaSetVolume(sghMusic, sglMusicVolume, 0);
	}

	return sglMusicVolume;
}

long sound_get_or_set_sound_volume(long lVolume)
{
	if(lVolume == 1) {
		return sglSoundVolume;
	}

	/// ASSERT: assert(lVolume >= VOLUME_MIN);
	/// ASSERT: assert(lVolume <= VOLUME_MAX);
	sglSoundVolume = lVolume;

	return sglSoundVolume;
}
