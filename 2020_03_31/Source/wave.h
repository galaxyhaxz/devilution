//HEADER_GOES_HERE
#ifndef __WAVE_H__
#define __WAVE_H__

//int dword_6ABB9C; // weak

_bool WCloseFile(void *file);
int WGetFileSize(HANDLE hsFile, unsigned long *a2);
void WGetFileArchive(HANDLE hsFile, int *a2, char *dwInitParam);
int WOpenFile(char *dwInitParam, HANDLE *phsFile, int a3);
void WReadFile(HANDLE hsFile, char *buf, int a3);
int WSetFilePointer(HANDLE file1, int offset, HANDLE file2, int whence);
int LoadWaveFormat(HANDLE hsFile, WAVEFORMATEX *pwfx);
void *AllocateMemFile(HANDLE hsFile, MEMFILE *pMemFile, unsigned int dwPos);
void FreeMemFile(MEMFILE *pMemFile);
int ReadWaveFile(MEMFILE *pMemFile, WAVEFORMATEX *pwfx, CKINFO *a3);
int ReadMemFile(MEMFILE *pMemFile, void *lpBuf, size_t a3);
void FillMemFile(MEMFILE *pMemFile);
int SeekMemFile(MEMFILE *pMemFile, unsigned int lDist, int dwMethod);
int ReadWaveSection(MEMFILE *pMemFile, int a2, CKINFO *a3);
void *LoadWaveFile(HANDLE hsFile, WAVEFORMATEX *pwfx, CKINFO *a3);
void j_engine_mem_free(void *ptr);

#endif /* __WAVE_H__ */
