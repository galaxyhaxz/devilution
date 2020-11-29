#include "diablo.h"

DWORD codec_decode(BYTE *pbSrcDst, DWORD dwSrcBytes, char *pszPassword)
{
	int i, j;
	CODECCHUNK *pChunk;
	char buff[128];
	char dst[20];

	/// ASSERT: assert(pbSrcDst);
	/// ASSERT: assert(pszPassword);
	codec_init_key(FALSE, pszPassword);

	if(dwSrcBytes <= 8) {
		return 0;
	}

	dwSrcBytes -= 8;

	if(dwSrcBytes & 0x3F) {
		return 0;
	}

	for(i = dwSrcBytes; i != 0; i -= 64) {
		memcpy(buff, pbSrcDst, 64);
		SHA1Result(0, dst);
		for(j = 0; j < 64; j++) {
			buff[j] ^= dst[j % 20];
		}
		SHA1Calculate(0, buff, 0);
		memset(dst, 0, sizeof(dst));
		memcpy(pbSrcDst, buff, 64);
		pbSrcDst += 64;
	}

	memset(buff, 0, sizeof(buff));
	pChunk = (CODECCHUNK *)pbSrcDst;

	if(pChunk->failed > 0) {
		goto error;
	}

	SHA1Result(0, dst);

	if(pChunk->checksum != ((DWORD *)dst)[0]) {
		memset(dst, 0, sizeof(dst));
		goto error;
	}

	dwSrcBytes -= 64 - pChunk->last_chunk_size;
	SHA1Clear();
	return dwSrcBytes;
error:
	SHA1Clear();
	return 0;
}

void codec_init_key(BOOL unused, char *pszPassword)
{
	int i, j, k, l, cnt;
	char *p, *p2;
	char buff[136];
	char key[64];
	char dst[20];

	srand(0x7058);

	p = buff;
	cnt = sizeof(buff);
	while(cnt--) {
		*p = rand();
		p++;
	}

	j = 0;
	for(i = 0; i < 64; i++) {
		if(pszPassword[j] == '\0') {
			j = 0;
		}
		key[i] = pszPassword[j];
		j++;
	}

	SHA1Reset(0);
	SHA1Calculate(0, key, dst);
	SHA1Clear();

	p2 = buff;
	for(k = 0; k < sizeof(buff); k++) {
		p2[k] ^= dst[k % 20];
	}

	memset(key, 0, sizeof(key));
	memset(dst, 0, sizeof(dst));

	for(l = 0; l < 3; l++) {
		SHA1Reset(l);
		SHA1Calculate(l, &buff[72], 0);
	}

	memset(buff, 0, sizeof(buff));
}

DWORD codec_get_encoded_len(DWORD dwSrcBytes)
{
	/// ASSERT: assert(dwSrcBytes);
	if(dwSrcBytes & 0x3F) {
		dwSrcBytes += 64 - (dwSrcBytes & 0x3F);
	}

	dwSrcBytes += 8;
	return dwSrcBytes;
}

void codec_encode(BYTE *pbSrcDst, DWORD dwSrcBytes, DWORD dwSize64, char *pszPassword)
{
	int i, last;
	DWORD size;
	CODECCHUNK *pChunk;
	char buff[128];
	char dst[20];
	char dst2[20];

	/// ASSERT: assert(pbSrcDst);
	/// ASSERT: assert(pszPassword);

	if(dwSize64 != codec_get_encoded_len(dwSrcBytes)) {
		app_fatal("Invalid encode parameters");
	}

	codec_init_key(TRUE, pszPassword);

	last = 0;
	while(dwSrcBytes != 0) {
		size = dwSrcBytes < 64 ? dwSrcBytes : 64;
		memcpy(buff, pbSrcDst, size);
		if(size < 64) {
			memset(&buff[size], 0, 64 - size);
		}
		SHA1Result(0, dst);
		SHA1Calculate(0, buff, 0);
		for(i = 0; i < 64; i++) {
			buff[i] ^= dst[i % 20];
		}
		memset(dst, 0, sizeof(dst));
		memcpy(pbSrcDst, buff, 64);
		pbSrcDst += 64;
		dwSrcBytes -= size;
		last = size;
	}

	memset(buff, 0, sizeof(buff));
	pChunk = (CODECCHUNK *)pbSrcDst;
	SHA1Result(0, dst2);
	pChunk->checksum = ((DWORD *)dst2)[0];
	pChunk->failed = 0;
	pChunk->last_chunk_size = last;
	pChunk->unused = 0;
	SHA1Clear();
}
