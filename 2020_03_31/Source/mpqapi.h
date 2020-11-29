//HEADER_GOES_HERE
#ifndef __MPQAPI_H__
#define __MPQAPI_H__

extern int sgdwMpqOffset; // idb
extern char mpq_buf[4096];
extern _HASHENTRY *sgpHashTbl;
extern _bool save_archive_modified; // weak
extern _BLOCKENTRY *sgpBlockTbl;
extern _bool save_archive_open; // weak

_bool mpqapi_set_hidden(char *pszArchive, _bool hidden);
void mpqapi_store_creation_time(char *pszArchive, int dwChar);
_bool mpqapi_reg_load_modification_time(char *dst, int size);
void mpqapi_xor_buf(char *pbData);
void mpqapi_store_default_time(DWORD dwChar);
_bool mpqapi_reg_store_modification_time(char *pbData, int dwLen);
_BLOCKENTRY *j_mpqapi_remove_hash_entry(char *pszName);
void mpqapi_remove_hash_entry(char *pszName);
void mpqapi_free_block(int block_offset, int block_size);
_BLOCKENTRY *mpqapi_new_block(int *block_index);
int mpqapi_get_hash_index_of_path(char *pszName);
int mpqapi_get_hash_index(short index, int hash_a, int hash_b, int locale);
void mpqapi_remove_hash_entries(_bool (__stdcall *fnGetName)(int, char *));
_bool mpqapi_write_file(char *pszName, char *pbData, int dwLen);
_BLOCKENTRY *mpqapi_add_file(char *pszName, _BLOCKENTRY *pBlk, int block_index);
_bool mpqapi_write_file_contents(char *pszName, char *pbData, int dwLen, _BLOCKENTRY *pBlk);
int mpqapi_find_free_block(int size, int *block_size);
void mpqapi_rename(char *pszOld, char *pszNew);
_bool mpqapi_has_file(char *pszName);
_bool mpqapi_open_archive(char *pszArchive, _bool hidden, int dwChar);
_bool mpqapi_parse_archive_header(_FILEHEADER *pHdr, int *pdwNextFileStart);
void mpqapi_close_archive(char *pszArchive, _bool bFree, int dwChar);
void mpqapi_store_modified_time(char *pszArchive, int dwChar);
void mpqapi_flush_and_close(char *pszArchive, _bool bFree, int dwChar);
_bool mpqapi_write_header();
_bool mpqapi_write_block_table();
_bool mpqapi_write_hash_table();
_bool mpqapi_can_seek();

/* data */

extern HANDLE sghArchive; // idb

#endif /* __MPQAPI_H__ */
