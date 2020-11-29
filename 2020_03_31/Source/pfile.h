//HEADER_GOES_HERE
#ifndef __PFILE_H__
#define __PFILE_H__

extern char hero_names[MAX_CHARACTERS][PLR_NAME_LEN];
extern BOOL gbValidSaveFile; // idb
extern int save_prev_tc; // weak

void pfile_init_save_directory();
void pfile_check_available_space(char *pszDir);
void pfile_write_hero();
int pfile_get_save_num_from_name(char *name);
void pfile_encode_hero(PkPlayerStruct *pPack);
_bool pfile_open_archive(_bool a1, int save_num);
void pfile_get_save_path(char *pszBuf, int dwBufSize, int save_num);
void pfile_flush(_bool is_single_player, int save_num);
_bool pfile_create_player_description(char *dst, int len);
int pfile_create_save_file(char *name_1, char *name_2);
void pfile_flush_W();
void game_2_ui_player(PlayerStruct *p, _uiheroinfo *heroinfo, BOOL bHasSaveFile);
char game_2_ui_class(PlayerStruct *p);
BOOL __stdcall pfile_ui_set_hero_infos(BOOL (__stdcall *ui_add_hero_info)(_uiheroinfo *));
char *GetSaveDirectory(char *dst, int dst_size, int save_num);
_bool pfile_read_hero(void *archive, PkPlayerStruct *pPack);
void *pfile_open_save_archive(int *unused, int save_num);
void pfile_SFileCloseArchive(void *hsArchive);
BOOL pfile_archive_contains_game(void *hsArchive);
BOOL __stdcall pfile_ui_set_class_stats(int player_class_nr, _uidefaultstats *class_stats);
int pfile_get_player_class(int player_class_nr);
BOOL __stdcall pfile_ui_save_create(_uiheroinfo *heroinfo);
_bool __stdcall pfile_get_file_name(int lvl, char *dst);
BOOL __stdcall pfile_delete_save(_uiheroinfo *hero_info);
void pfile_read_player_from_save();
void GetTempLevelNames(char *szTemp);
void GetPermLevelNames(char *szPerm);
void pfile_get_game_name(char *dst);
void pfile_remove_temp_files();
_bool __stdcall GetTempSaveNames(int dwIndex, char *szTemp);
void pfile_rename_temp_to_perm();
_bool __stdcall GetPermSaveNames(int dwIndex, char *szPerm);
void pfile_write_save_file(char *pszName, void *pbData, int dwLen, int qwLen);
void pfile_strcpy(char *dst, char *src);
char *pfile_read(char *pszName, int *pdwLen);
void pfile_update(BOOL force_save);

#endif /* __PFILE_H__ */
