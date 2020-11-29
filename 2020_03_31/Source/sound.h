//HEADER_GOES_HERE
#ifndef __SOUND_H__
#define __SOUND_H__

extern IDirectSoundBuffer *DSBs[8];
extern IDirectSound *sglpDS;
extern char gbSndInited;
extern long sglMusicVolume;
extern long sglSoundVolume;
extern HMODULE hDsound_dll; // idb
extern HANDLE sghMusic;
extern IDirectSoundBuffer *sglpDSB;

void snd_update(BOOL bStopAll);
void snd_stop_snd(TSnd *pSnd);
BOOL snd_playing(TSnd *pSnd);
void snd_play_snd(TSnd *pSnd, long lVolume, long lPan);
TSnd *sound_file_load(const char *pszFile);
void sound_file_cleanup(TSnd *pSnd);
void snd_init(HWND hWnd);
void sound_cleanup();
void music_stop();
void music_start(int nTrack);
void sound_disable_music(BOOL disable);
long sound_get_or_set_music_volume(long lVolume);
long sound_get_or_set_sound_volume(long lVolume);

/* data */

extern char gbMusicOn; // weak
extern char gbSoundOn; // weak
extern char gbDupSounds; // weak
extern int sgnMusicTrack;
extern char *sgszMusicTracks[6];
extern char unk_volume[4][2];

#endif /* __SOUND_H__ */
