//HEADER_GOES_HERE
#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

extern int hashtable[1280];

void Decrypt(void *block, int size, int key);
void Encrypt(void *block, int size, int key);
int Hash(char *s, int type);
void InitHash();
int PkwareCompress(void *buf, int size);
unsigned int __cdecl PkwareBufferRead(char *buf, unsigned int *size, void *param);
void __cdecl PkwareBufferWrite(char *buf, unsigned int *size, void *param);
void PkwareDecompress(void *param, int recv_size, int dwMaxBytes);

#endif /* __ENCRYPT_H__ */
