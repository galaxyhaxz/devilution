//HEADER_GOES_HERE
#ifndef __CODEC_H__
#define __CODEC_H__

DWORD codec_decode(BYTE *pbSrcDst, DWORD dwSrcBytes, char *pszPassword);
void codec_init_key(BOOL unused, char *pszPassword);
DWORD codec_get_encoded_len(DWORD dwSrcBytes);
void codec_encode(BYTE *pbSrcDst, DWORD dwSrcBytes, DWORD dwSize64, char *pszPassword);

#endif /* __CODEC_H__ */
