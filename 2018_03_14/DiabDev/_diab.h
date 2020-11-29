
char *__fastcall GetErr(int error_code);
void __fastcall GetDDErr(int error_code, char *error_buf, int error_buf_len);
void __fastcall GetDSErr(int error_code, char *error_buf, int error_buf_len);
char *__cdecl GetLastErr();
void TermMsg(char *format, ...);
void __fastcall MsgBox(char *format, va_list va);
void __cdecl FreeDlg();
void DrawDlg(char *format, ...);
void __fastcall DDErrDlg(int error_code, int log_line_nr, char *log_file_path);
void __fastcall DSErrDlg(int error_code, int log_line_nr, char *log_file_path);
void __fastcall CenterDlg(HWND hDlg);
void __fastcall TermDlg(int template_id, int error_code, char *log_file_path, int log_line_nr);
bool __stdcall FuncDlg(HWND hDlg, UINT uMsg, WPARAM wParam, char *text);
void __fastcall TextDlg(HWND hDlg, char *text);
void __fastcall ErrDlg(template_id template_id, int error_code, char *log_file_path, int log_line_nr);
void __fastcall FileErrDlg(char *error);
void __fastcall DiskFreeDlg(char *error);
bool __cdecl InsertCDDlg();
void __fastcall DirErrDlg(char *error);
void __cdecl InitAutomapOnce();
void __cdecl InitAutomap();
void __cdecl StartAutomap();
void __cdecl AutomapUp();
void __cdecl AutomapDown();
void __cdecl AutomapLeft();
void __cdecl AutomapRight();
void __cdecl AutomapZoomIn();
void __cdecl AutomapZoomOut();
void __cdecl DrawAutomap();
void __fastcall DrawAutomapType(int screen_x, int screen_y, short automap_type);
void __cdecl DrawAutomapPlr();
short __fastcall GetAutomapType(int tx, int ty, bool view);
void __cdecl DrawAutomapGame();
void __fastcall SetAutomapView(int x, int y);
void __cdecl AutomapZoomReset();
void __cdecl CaptureScreen();
bool __fastcall CaptureHdr(HANDLE hFile, short width, int height);
bool __fastcall CapturePal(HANDLE hFile, PALETTEENTRY *palette);
bool __fastcall CapturePix(HANDLE hFile, short width, short height, short stride, char *pixels);
char *__fastcall CaptureEnc(char *src, void *dst, int width);
HANDLE __fastcall CaptureFile(char *dst_path);
void __fastcall CaptureRedPal(PALETTEENTRY *palette_orig);
int __fastcall codec_decode(void *src_dst, int size, char *password);
void __cdecl j_sha1_reset();
void __fastcall codec_init_key(int unused, char *password);
int __fastcall codec_get_encoded_len(int n);
void __fastcall codec_encode(void *src_dst, int size, int size_64, char *password);
void __fastcall DrawSpellCel(int xp, int yp, char *Trans, int nCel, int w);
void __fastcall SetSpellTrans(char t);
void __cdecl DrawSpell();
void __cdecl DrawSpellList();
void __cdecl SetSpell();
void __fastcall SetSpeedSpell(int slot);
void __fastcall ToggleSpell(int slot);
int __fastcall CPrintString(int No, char *pszStr, int Just);
void __fastcall AddPanelString(char *str, int just);
void __cdecl ClearPanel();
void __fastcall DrawPanelBox(int x, int y, unsigned short a3, unsigned short a4, int sx, int sy);
void __cdecl InitPanelStr();
void __fastcall SetFlaskHeight(char *buf, int min, int max, int c, int r);
void __fastcall DrawFlask(void *a1, int a2, int a3, void *a4, int a5, int a6);
void __cdecl DrawLifeFlask();
void __cdecl UpdateLifeFlask();
void __cdecl DrawManaFlask();
void __cdecl control_update_life_mana();
void __cdecl UpdateManaFlask();
void __cdecl InitControlPan();
void __cdecl ClearCtrlPan();
void __cdecl DrawCtrlPan();
void __cdecl DoSpeedBook();
void __cdecl DoPanBtn();
void __fastcall control_set_button_down(int btn_id);
void __cdecl control_check_btn_press();
void __cdecl DoAutoMap();
void __cdecl CheckPanelInfo();
void __cdecl CheckBtnUp();
void __cdecl FreeControlPan();
int __fastcall control_WriteStringToBuffer(char *str);
void __cdecl DrawInfoBox();
void __fastcall control_print_info_str(int y, char *str, int a3, int lines);
void __fastcall PrintGameStr(int x, int y, char *str, int color);
void __cdecl DrawChr();
void __fastcall ADD_PlrStringXY(int x, int y, int width, char *pszStr, char col);
void __fastcall MY_PlrStringXY(int x, int y, int width, char *pszStr, char col, int base);
void __cdecl CheckLvlBtn();
void __cdecl ReleaseLvlBtn();
void __cdecl DrawLevelUpIcon();
void __cdecl CheckChrBtns();
void __cdecl ReleaseChrBtns();
void __cdecl DrawDurIcon();
int __fastcall DrawDurIcon4Item(ItemStruct *pItem, int x, int c);
void __cdecl RedBack();
int __fastcall GetSBookTrans(int ii, unsigned char townok);
void __cdecl DrawSpellBook();
void __fastcall PrintSBookStr(int x, int y, bool cjustflag, char *pszStr, int bright);
void __cdecl CheckSBook();
char *__fastcall get_pieces_str(int nGold);
void __fastcall DrawGoldSplit(int amount);
void __fastcall control_drop_gold(int a1);
void __fastcall control_remove_gold(int pnum, int gold_index);
void __fastcall control_set_gold_curs(int pnum);
void __cdecl DrawTalkPan();
void __fastcall control_print_talk_msg(char *msg, int x, int y, int *a4, int just);
int __cdecl control_check_talk_btn();
void __cdecl control_release_talk_btn();
void __cdecl control_reset_talk_msg();
void __cdecl control_type_message();
void __cdecl control_reset_talk();
int __fastcall control_talk_last_key(int a1);
int __fastcall control_presskeys(int a1);
void __cdecl control_press_enter();
void __fastcall control_up_down(char a1);
void __cdecl InitCursor();
void __cdecl FreeCursor();
void __fastcall SetICursor(int i);
void __fastcall SetCursor(int i);
void __fastcall NewCursor(int i);
void __cdecl InitLevelCursor();
void __cdecl CheckTown();
void __cdecl CheckRportal();
void __cdecl CheckCursMove();
void __cdecl InitDead();
void __fastcall AddDead(int dx, int dy, char dv, direction ddir);
void __cdecl SetDead();
void __cdecl LoadDebugGFX();
void __cdecl FreeDebug();
void __cdecl CheckClearDbg();
void __cdecl diablo_cpp_init();
void __cdecl FreeGameMem();
int __fastcall diablo_init_menu(int a1, int bSinglePlayer);
void __fastcall run_game_loop(int interface_mode);
void __fastcall start_game(int interface_mode);
void __cdecl free_game();
bool __cdecl diablo_get_not_running();
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
void __fastcall diablo_parse_flags(char *args);
void __cdecl diablo_init_screen();
HWND __fastcall diablo_find_window(LPCSTR lpClassName);
void __fastcall diablo_reload_process(HMODULE hModule);
int __cdecl PressEscKey();
LRESULT __stdcall DisableInputWndProc(HWND hWnd, int uMsg, int wParam, int lParam);
int __stdcall GM_Game(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void __cdecl LeftMouseDown();
int __cdecl TryIconCurs();
void __cdecl LeftMouseUp();
void __cdecl RightMouseDown();
void __fastcall j_gmenu_run_item(LPARAM lParam);
int __fastcall PressSysKey(int wParam);
void __fastcall diablo_hotkey_msg(int dwMsg);
void __fastcall ReleaseKey(int vkey);
void __fastcall PressKey(int vkey);
void __cdecl diablo_pause_game();
void __fastcall PressChar(int vkey);
void __cdecl LoadLvlGFX();
void __cdecl LoadAllGFX();
void __fastcall CreateLevel(int entry);
void __fastcall LoadGameLevel(bool from_save, int entry);
void __fastcall game_loop(bool startup);
void __cdecl game_logic();
void __fastcall timeout_cursor(bool timeout);
void __cdecl diablo_color_cyc_logic();
int __cdecl doom_get_frame_from_time();
void __cdecl doom_alloc_cel();
void __cdecl doom_cleanup();
void __cdecl doom_load_graphics();
void __cdecl doom_init();
void __cdecl doom_close();
void __cdecl doom_draw();
void __cdecl DRLG_Init_Globals();
void __fastcall LoadL1Dungeon(char *dun_path, int view_x, int view_y);
void __cdecl DRLG_L1Floor();
void __cdecl DRLG_L1Pass3();
void __cdecl DRLG_InitL1Vals();
void __fastcall LoadPreL1Dungeon(char *dun_path, int view_x, int view_y);
void __fastcall CreateL5Dungeon(int seed, int entry);
void __cdecl DRLG_LoadL1SP();
void __cdecl DRLG_FreeL1SP();
void __fastcall DRLG_L5(int entry);
void __fastcall DRLG_PlaceDoor(int tx, int ty);
void __cdecl DRLG_L1Shadows();
int __fastcall DRLG_PlaceMiniSet(char *miniset, int tmin, int tmax, int cx, int cy, bool set_view, int noquad, int ldir);
void __cdecl InitL5Dungeon();
void __cdecl L5ClearFlags();
void __cdecl L5firstRoom();
void __fastcall L5drawRoom(int tx, int ty, int tw, int th);
void __fastcall L5roomGen(int tx, int ty, int tw, int th, bool dir_horiz);
bool __fastcall L5checkRoom(int tx, int ty, int tw, int th);
int __cdecl L5GetArea();
void __cdecl L5makeDungeon();
void __cdecl L5makeDmt();
void __cdecl L5AddWall();
int __fastcall L5HWallOk(int tx, int ty);
int __fastcall L5VWallOk(int tx, int ty);
void __fastcall L5HorizWall(int tx, int ty, int tile_id, int tw);
void __fastcall L5VertWall(int tx, int ty, int tile_id, int th);
void __cdecl L5tileFix();
void __cdecl DRLG_L5Subs();
void __cdecl L5FillChambers();
void __fastcall DRLG_L5GChamber(int tx, int ty, bool top_right, bool bottom_left, bool top_left, bool bottom_right);
void __fastcall DRLG_L5GHall(int tx_start, int ty_start, int tx_end, int ty_end);
void __fastcall DRLG_L5SetRoom(int tx, int ty);
void __cdecl DRLG_L5FloodTVal();
void __fastcall DRLG_L5FTVR(int tx, int ty, int x, int y, int direction);
void __cdecl DRLG_L5TransFix();
void __cdecl DRLG_L5DirtFix();
void __cdecl DRLG_L5CornerFix();
void __cdecl InitDungeon();
void __cdecl L2LockoutFix();
void __cdecl L2DoorFix();
void __fastcall LoadL2Dungeon(char *sFileName, int vx, int vy);
void __cdecl DRLG_L2Pass3();
void __fastcall LoadPreL2Dungeon(char *sFileName, int vx, int vy);
void __fastcall CreateL2Dungeon(int seed, int entry);
void __cdecl DRLG_LoadL2SP();
void __cdecl DRLG_FreeL2SP();
void __fastcall DRLG_L2(int entry);
bool __fastcall DRLG_L2PlaceMiniSet(char *miniset, int tmin, int tmax, int cx, int cy, bool set_view, int ldir);
void __fastcall DRLG_L2PlaceRndSet(char *miniset, int probability);
void __cdecl DRLG_L2Subs();
void __cdecl DRLG_L2Shadows();
void __fastcall DRLG_L2SetRoom(int rx1, int ry1);
void __cdecl L2TileFix();
void __cdecl CreateDungeon();
void __fastcall CreateRoom(int nX1, int nY1, int nX2, int nY2, int nRDest, int nHDir, int ForceHW, int nH, int nW);
void __fastcall DefineRoom(int nX1, int nY1, int nX2, int nY2, int ForceHW);
void __fastcall AddHall(int nX1, int nY1, int nX2, int nY2, int nHd);
void __fastcall GetHall(int *nX1, int *nY1, int *nX2, int *nY2, int *nHd);
void __fastcall ConnectHall(int nX1, int nY1, int nX2, int nY2, int nHd);
void __fastcall CreateDoorType(int nX, int nY);
void __fastcall PlaceHallExt(int nX, int nY);
void __fastcall DoPatternCheck(int i, int j);
void __cdecl DL2_FillVoids();
int __fastcall DL2_Cont(unsigned char x1f, unsigned char y1f, unsigned char x2f, unsigned char y2f);
int __cdecl DL2_NumNoChar();
void __fastcall DL2_DrawRoom(int x1, int y1, int x2, int y2);
void __fastcall DL2_KnockWalls(int x1, int y1, int x2, int y2);
void __cdecl DRLG_L2FloodTVal();
void __fastcall DRLG_L2FTVR(int i, int j, int x, int y, int d);
void __cdecl DRLG_L2TransFix();
void __cdecl L2DirtFix();
void __cdecl DRLG_InitL2Vals();
void __cdecl AddFenceDoors();
void __cdecl FenceDoorFix();
void __cdecl DRLG_L3Anvil();
void __cdecl FixL3Warp();
void __cdecl FixL3HallofHeroes();
void __fastcall DRLG_L3LockRec(int x, int y);
void __cdecl DRLG_L3Lockout();
void __fastcall CreateL3Dungeon(int seed, int entry);
void __fastcall DRLG_L3(int entry);
void __cdecl InitL3Dungeon();
int __fastcall DRLG_L3FillRoom(int x1, int y1, int x2, int y2);
void __fastcall DRLG_L3CreateBlock(int x, int y, int obs, int dir);
void __fastcall DRLG_L3FloorArea(int x1, int y1, int x2, int y2);
void __cdecl DRLG_L3FillDiags();
void __cdecl DRLG_L3FillSingles();
void __cdecl DRLG_L3FillStraights();
void __cdecl DRLG_L3Edges();
void __cdecl DRLG_L3GetFloorArea();
void __cdecl DRLG_L3MakeMegas();
void __cdecl DRLG_L3River();
void __cdecl DRLG_L3Pool();
int __fastcall DRLG_L3SpawnEdge(int x, int y, int *totarea);
int __fastcall DRLG_L3Spawn(int x, int y, int *totarea);
void __cdecl DRLG_L3PoolFix();
bool __fastcall DRLG_L3PlaceMiniSet(char *miniset, int tmin, int tmax, int cx, int cy, bool set_view, int ldir);
void __fastcall DRLG_L3PlaceRndSet(unsigned char *miniset, int rndper);
void __cdecl DRLG_L3Wood();
int __fastcall WoodVertU(int i, int y);
int __fastcall WoodVertD(int i, int y);
int __fastcall WoodHorizL(int x, int j);
int __fastcall WoodHorizR(int x, int j);
void __cdecl DRLG_L3Pass3();
void __fastcall LoadL3Dungeon(char *sFileName, int vx, int vy);
void __fastcall LoadPreL3Dungeon(char *sFileName, int vx, int vy);
void __cdecl DRLG_LoadL4SP();
void __cdecl DRLG_FreeL4SP();
void __fastcall DRLG_L4SetSPRoom(int rx1, int ry1);
void __cdecl L4SaveQuads();
void __fastcall DRLG_L4SetRoom(unsigned char *pSetPiece, int rx1, int ry1);
void __fastcall DRLG_LoadDiabQuads(char preflag);
unsigned char __fastcall IsDURWall(char d);
unsigned char __fastcall IsDLLWall(char dd);
void __cdecl L4FixRim();
void __cdecl DRLG_L4GeneralFix();
void __fastcall CreateL4Dungeon(int seed, int entry);
void __fastcall DRLG_L4(int entry);
void __cdecl DRLG_L4Shadows();
void __cdecl InitL4Dungeon();
void __cdecl L4makeDmt();
void __cdecl L4AddWall();
int __fastcall L4HWallOk(int i, int j);
int __fastcall L4VWallOk(int i, int j);
void __fastcall L4HorizWall(int i, int j, int dx);
void __fastcall L4VertWall(int i, int j, int dy);
void __cdecl L4tileFix();
void __cdecl DRLG_L4Subs();
void __cdecl L4makeDungeon();
void __cdecl uShape();
int __cdecl GetArea();
void __cdecl L4firstRoom();
void __fastcall L4drawRoom(int x, int y, int width, int height);
void __fastcall L4roomGen(int x, int y, int w, int h, int dir);
int __fastcall L4checkRoom(int x, int y, int width, int height);
unsigned char __fastcall DRLG_L4PlaceMiniSet(unsigned char *miniset, int tmin, int tmax, int cx, int cy, int setview, int ldir);
void __cdecl DRLG_L4FloodTVal();
void __fastcall DRLG_L4FTVR(int i, int j, int x, int y, int d);
void __cdecl DRLG_L4TransFix();
void __cdecl DRLG_L4Corners();
void __cdecl DRLG_L4Pass3();
void __cdecl dthread_cpp_init_1();
void __cdecl dthread_cpp_init_2();
void __cdecl dthread_init_mutex();
void __cdecl dthread_cleanup_mutex_atexit();
void __cdecl dthread_cleanup_mutex();
void __fastcall dthread_remove_player(int player_num);
void __fastcall dthread_send_delta(int player_num, int cmd, void *src, int len);
void __cdecl dthread_start();
unsigned int __stdcall dthread_handler(void *a1);
void __cdecl dthread_cleanup();
void __cdecl dx_cpp_init_1();
void __cdecl dx_cpp_init_2();
void __cdecl dx_init_mutex();
void __cdecl dx_cleanup_mutex_atexit();
void __cdecl dx_cleanup_mutex();
void __fastcall dx_init(HWND hWnd);
void __cdecl dx_create_back_buffer();
void __cdecl dx_create_primary_surface();
HRESULT __fastcall dx_DirectDrawCreate(GUID *guid, IDirectDraw **DD, void *unknown);
void __cdecl j_dx_lock_mutex();
void __cdecl dx_lock_mutex();
void __cdecl j_dx_unlock_mutex();
void __cdecl dx_unlock_mutex();
void __cdecl dx_cleanup();
void __cdecl dx_reinit();
void __cdecl j_dx_reinit();
void __cdecl effects_cpp_init();
bool __fastcall effect_is_playing(int nSFX);
void __cdecl sfx_stop();
void __fastcall InitMonsterSND(int monst);
void __cdecl FreeEffects();
void __fastcall PlayEffect(int i, int mode);
int __fastcall calc_snd_position(int x, int y, int *plVolume, int *plPan);
void __fastcall PlaySFX(int psfx);
void __fastcall PlaySFX_priv(TSFX *pSFX, char loc, int x, int y);
void __fastcall stream_play(TSFX *pSFX, int lVolume, int lPan);
int __fastcall RndSFX(int psfx);
void __fastcall PlaySfxLoc(int psfx, int x, int y);
void __cdecl FreeMonsterSnd();
void __cdecl sound_stop();
void __cdecl sound_update();
void __cdecl effects_cleanup_sfx();
void __cdecl stream_update();
void __fastcall priv_sound_init(int bLoadMask);
void __cdecl sound_init();
void __stdcall effects_play_sound(char *snd_file);
void __fastcall encrypt_decrypt_block(void *block, int size, int key);
void __fastcall encrypt_encrypt_block(void *block, int size, int key);
int __fastcall encrypt_hash(char *s, int type);
void __cdecl encrypt_init_lookup_table();
int __fastcall encrypt_compress(void *buf, int size);
int __cdecl encrypt_pkware_read(void *buf, int *size, void *param);
int __cdecl encrypt_pkware_write(void *buf, int *size, void *param);
void __fastcall encrypt_decompress(void *param, int a2, int size);
void __cdecl engine_cpp_init_1();
void __fastcall Cel_content_into_buf(char *pDecodeTo, char *pRLEBytes, int dwRLESize, int dwRLEWdt);
void __fastcall Cel_decode(int screen_x, int screen_y, void *pCelBuff, int frame, int frame_width);
void __fastcall Cel_into_buf(char *pBuff, char *pCelBuff, int frame, int frame_width);
void __fastcall Cel_header(int screen_x, int screen_y, char *pCelBuff, int frame, int frame_width, int always_0, int direction);
void __fastcall Cel_header_into_buf(char *pBuff, char *pCelBuff, int frame, int frame_width, int always_0, int direction);
void __fastcall Cel_content_light_into_buf(char *pDecodeTo, char *pRLEBytes, int frame_content_size, int frame_width);
void __fastcall Cel_content_light_entry_into_buf(int a1, int a2);
void __fastcall Cel_content_light_trans_into_buf(char *pDecodeTo, char *pRLEBytes, int frame_content_size, int frame_width);
void __fastcall Cel_light(int screen_x, int screen_y, char *pCelBuff, int frame, int frame_width);
void __fastcall Cel_header_and_light(int screen_x, int screen_y, char *pCelBuff, int frame, int frame_width, int always_0, int direction);
void __fastcall Cel_header_light_and_trans_into_buf(char *pBuff, char *pCelBuff, int frame, int frame_width, int always_0, int direction);
void __fastcall Cel_header_and_light_not_equipable(int screen_x, int screen_y, char *pCelBuff, int frame, int frame_width, int always_0, int direction, char always_1);
void __fastcall Cel2_content_into_buf(char *pDecodeTo, char *pRLEBytes, int frame_content_size, int frame_width);
void __fastcall Cel2_header(int screen_x, int screen_y, char *pCelBuff, int frame, int frame_width, int a6, int direction);
void __fastcall Cel2_header_into_buf(char *pBuff, char *pCelBuff, int frame, int frame_width, int a5, int direction);
void __fastcall Cel2_content_light_into_buf(char *pDecodeTo, char *pRLEBytes, int frame_content_size, int frame_width);
void __fastcall Cel2_content_light_entry_into_buf(int a1, int a2);
void __fastcall Cel2_content_light_trans_into_buf(char *pDecodeTo, char *pRLEBytes, int frame_content_size, int frame_width);
void __fastcall Cel2_header_and_light(int screen_x, int screen_y, char *pCelBuff, int frame, int frame_width, int a6, int direction);
void __fastcall Cel2_header_light_and_trans_into_buf(char *dst_buf, char *pCelBuff, int frame, int frame_width, int a5, int direction);
void __fastcall Cel2_header_and_light_not_equipable(int screen_x, int screen_y, char *pCelBuff, int frame, int frame_width, int always_0, int direction, char always_1);
void __fastcall Cel_into_rect_of_buf(char *pBuff, int always_0, int dst_height, int dst_width, char *pCelBuff, int frame, int frame_width);
void __fastcall Cel_colour(char colour, int screen_x, int screen_y, char *pCelBuff, int frame, int frame_width, int a7, int direction);
void __fastcall Cel_header_and_colour_highlight(char colour, int screen_x, int screen_y, char *pCelBuff, int frame, int frame_width, int a7, int direction);
void __fastcall ENG_set_pixel(int screen_x, int screen_y, char pixel);
void __fastcall engine_417034(int x, int y);
void __fastcall engine_4170BD(int x1, int y1, int x2, int y2, char a5);
int __fastcall GetDirection(int x1, int y1, int x2, int y2);
void __fastcall SetRndSeed(int s);
int __cdecl GetRndSeed();
int __fastcall random(int unused, int max);
void __cdecl engine_cpp_init_2();
void __cdecl mem_init_mutex();
void __cdecl mem_atexit_mutex();
void __cdecl mem_free_mutex();
void *__fastcall DiabloAllocPtr(int size);
void __fastcall mem_free_dbg(void *ptr);
unsigned char *__fastcall LoadFileInMem(char *file_path, void *size);
void __fastcall LoadFileWithMem(char *pszName, void *buf);
void __fastcall engine_cel_trn(int p, int ttbl, int a3);
void __fastcall engine_417745(int a1, int a2, void *pCelBuff, int nCel, int a5, int a6, int a7);
void __fastcall engine_4177BF(char *buffer, char *a2, int a3, int a4);
void __fastcall engine_417847(char a1, int a2, int a3, void *pCelBuff, int nCel, int a6, int a7, int a8);
void __fastcall engine_4178C5(char *buffer, char *a2, char *a3, int a4, char a5);
void __fastcall engine_417981(int a1, int a2, void *pCelBuff, int nCel, int a5, int a6, int a7, char a8);
void __fastcall engine_417A44(char *a1, char *a2, int a3, int a4, int unused_lindex);
void __fastcall engine_417AE9(int a1, int a2, void *pCelBuff, int nCel, int a5, int a6, int a7);
void __fastcall engine_417B83(int a1, int a2, void *pCelBuff, int nCel, int a5, int a6, int a7);
void __fastcall engine_417BFD(char *a1, char *a2, int a3, int a4);
void __fastcall engine_417C99(char a1, int a2, int a3, void *pCelBuff, int nCel, int a6, int a7, int a8);
void __fastcall engine_417D28(char *a1, char *a2, int a3, int a4, char a5);
void __fastcall engine_417DF8(int a1, int a2, void *pCelBuff, int nCel, int a5, int a6, int a7, char a8);
void __fastcall engine_417EBB(char *a1, char *a2, int a3, int a4, int a5);
void __fastcall engine_417F78(int a1, int a2, void *pCelBuff, int nCel, int a5, int a6, int a7);
void __fastcall PlayInGameMovie(char *pszMovie);
void __fastcall InitDiabloMsg(int error_id);
void __cdecl ClrDiabloMsg();
void __cdecl DrawDiabloMsg();
void __cdecl exception_cpp_init();
int *__cdecl exception_install_filter();
void __cdecl j_exception_init_filter();
void __cdecl exception_init_filter();
LONG __fastcall TopLevelExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo);
int __fastcall exception_hex_format(char *a1, char a2);
int __fastcall exception_unknown_module(LPCVOID lpAddress, LPSTR lpString1, int iMaxLength, int a4, int a5);
void __fastcall exception_call_stack(void *a1, LPVOID lp);
int __fastcall exception_get_error_type(DWORD dwMessageId, LPSTR lpString1, DWORD nSize);
void __fastcall exception_set_filter();
LPTOP_LEVEL_EXCEPTION_FILTER __cdecl exception_set_filter_ptr();
LPTOP_LEVEL_EXCEPTION_FILTER __cdecl exception_get_filter();
void __cdecl gamemenu_previous();
void __cdecl gamemenu_enable_single();
void __cdecl gamemenu_enable_multi();
void __cdecl gamemenu_off();
void __cdecl gamemenu_handle_previous();
void __cdecl gamemenu_new_game();
void __cdecl gamemenu_quit_game();
void __cdecl gamemenu_load_game();
void __cdecl gamemenu_save_game();
void __cdecl gamemenu_restart_town();
void __cdecl gamemenu_options();
void __cdecl gamemenu_get_music();
void __fastcall gamemenu_sound_music_toggle(char **names, TMenuItem *menu_item);
void __cdecl gamemenu_get_sound();
void __cdecl gamemenu_get_color_cycling();
void __cdecl gamemenu_get_gamma();
void __cdecl gamemenu_music_volume();
int __fastcall gamemenu_slider_music_sound(TMenuItem *menu_item);
void __cdecl gamemenu_sound_volume();
void __cdecl gamemenu_gamma();
int __cdecl gamemenu_slider_gamma();
void __cdecl gamemenu_color_cycling();
void __cdecl FillSolidBlockTbls();
void __cdecl gendung_418D91();
void __fastcall gendung_4191BF(int frames);
void __fastcall gendung_4191FB(int a1, int a2);
int __fastcall gendung_41927A(int x, int y);
void __cdecl gendung_4192C2();
void __cdecl SetDungeonMicros();
void __cdecl DRLG_InitTrans();
void __fastcall Make_RectTrans(int tx_start, int ty_start, int tx_end, int ty_end);
void __fastcall DRLG_RectTrans(int x_start, int y_start, int x_end, int y_end);
void __fastcall DRLG_CopyTrans(int src_x, int src_y, int dst_x, int dst_y);
void __fastcall DRLG_ListTrans(int num, unsigned char *List);
void __fastcall DRLG_AreaTrans(int num, unsigned char *List);
void __cdecl DRLG_InitSetPC();
void __cdecl DRLG_SetPC();
void __fastcall Make_SetPC(int x, int y, int w, int h);
unsigned char __fastcall DRLG_WillThemeRoomFit(int floor, int x, int y, int minSize, int maxSize, int *width, int *height);
void __fastcall DRLG_CreateThemeRoom(int themeIndex);
void __fastcall DRLG_PlaceThemeRooms(int min_size, int max_size, int floor, int frequency, int rnd_size);
void __cdecl DRLG_HoldThemeRooms();
unsigned char __fastcall SkipThemeRoom(int x, int y);
void __cdecl InitLevels();
void __cdecl gmenu_draw_pause();
int __fastcall gmenu_print_text(int x, int y, char *str);
void __cdecl FreeGMenu();
void __cdecl gmenu_init_menu();
bool __cdecl gmenu_exception();
void __fastcall gmenu_call_proc(int a1, void (__cdecl *gmFunc)());
void __fastcall gmenu_up_down(int a1);
void __cdecl gmenu_draw();
void __fastcall gmenu_spinners(int a1, int a2);
void __fastcall gmenu_clear_buffer(int x, int y, int size, int a4);
int __fastcall gmenu_get_lfont(int a1);
int __fastcall gmenu_presskeys(int a1);
void __fastcall gmenu_left_right(int a1);
int __fastcall gmenu_run_item(LPARAM lParam);
char __fastcall gmenu_valid_mouse_pos(int a1);
int __fastcall gmenu_left_mouse(int a1);
void __fastcall gmenu_enable(TMenuItem *menu_item, bool enable);
int __fastcall gmenu_slider_1(int *a1, int min, int max, int gamma);
int __fastcall gmenu_slider_get(TMenuItem *menu_item, int min, int max);
void __fastcall gmenu_slider_3(int *a1, int a2);
void __cdecl InitHelp();
void __cdecl DrawHelp();
void __fastcall DrawHelpLine(int always_0, int help_line_nr, char *text, text_color color);
void __cdecl DisplayHelp();
void __cdecl HelpScrollUp();
void __cdecl HelpScrollDown();
void __cdecl init_cpp_init();
void __fastcall init_cleanup(bool show_cursor);
void __cdecl init_run_office_from_start_menu();
void __fastcall init_run_office(char *dir);
void __fastcall init_disable_screensaver(bool disable);
void __cdecl init_create_window();
void __cdecl init_kill_mom_parent();
HWND __cdecl init_find_mom_parent();
void __cdecl init_await_mom_parent_exit();
void __cdecl init_archives();
void *__fastcall init_test_access(char *mpq_path, char *mpq_name, char *reg_loc, int flags, bool on_cd);
char *__fastcall init_strip_trailing_slash(char *path);
int __fastcall init_read_test_file(char *mpq_path, char *mpq_name, int flags, void **archive);
void __cdecl init_get_file_info();
LRESULT __stdcall init_palette(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
void __fastcall init_activate_window(HWND hWnd, bool activated);
LRESULT __stdcall init_redraw_window(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT (__stdcall *__fastcall SetWindowProc(void *func))(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void __cdecl interfac_cpp_init();
void __cdecl interface_msg_pump();
bool __cdecl IncProgress();
void __cdecl DrawCutscene();
void __fastcall DrawProgress(int screen_x, int screen_y, int progress_id);
void __fastcall ShowProgress(int uMsg);
void __cdecl FreeInterface();
void __fastcall InitCutscene(int interface_mode);
void __cdecl FreeInvGFX();
void __cdecl InitInv();
void __fastcall InvDrawSlotBack(int X, int Y, int W, int H);
void __cdecl DrawInv();
void __cdecl DrawInvBelt();
int __fastcall AutoPlace(int pnum, int ii, int sx, int sy, int saveflag);
int __fastcall SpecialAutoPlace(int pnum, int ii, int sx, int sy, int saveflag);
int __fastcall GoldAutoPlace(int pnum);
int __fastcall WeaponAutoPlace(int pnum);
int __fastcall SwapItem(ItemStruct *a, ItemStruct *b);
void __fastcall CheckInvPaste(int pnum, int mx, int my);
void __fastcall CheckInvSwap(int pnum, int bLoc, int idx, int wCI, int seed, int bId);
void __fastcall CheckInvCut(int pnum, int mx, int my);
void __fastcall inv_update_rem_item(int pnum, int iv);
void __fastcall RemoveInvItem(int pnum, int iv);
void __fastcall RemoveSpdBarItem(int pnum, int iv);
void __cdecl CheckInvItem();
void __cdecl CheckInvScrn();
void __fastcall CheckItemStats(int pnum);
void __fastcall CheckBookLevel(int pnum);
void __fastcall CheckQuestItem(int pnum);
void __fastcall InvGetItem(int pnum, int ii);
void __fastcall AutoGetItem(int pnum, int ii);
void __fastcall FindGetItem(int indx, int ci, int iseed);
void __fastcall SyncGetItem(int x, int y, int idx, unsigned short ci, int iseed);
int __fastcall CanPut(int i, int j);
int __cdecl TryInvPut();
void __fastcall DupeInvMsg(char *msg);
int __fastcall InvPutItem(int pnum, int x, int y);
int __fastcall SyncPutItem(int pnum, int x, int y, int idx, int icreateinfo, int iseed, int Id, int dur, int mdur, int ch, int mch, int ivalue, unsigned __int32 ibuff);
int __cdecl CheckInvHLight();
void __fastcall RemoveScroll(int pnum);
void __cdecl UseScroll();
void __fastcall UseStaffCharge(int pnum);
void __cdecl UseStaff();
void __cdecl StartGoldDrop();
int __fastcall UseInvItem(int pnum, int cii);
void __cdecl DoTelekinesis();
int __fastcall CalculateGold(int pnum);
int __cdecl DropItemBeforeTrig();
void __cdecl InitItemGFX();
bool __fastcall ItemPlace(int x, int y);
void __cdecl AddInitItems();
void __cdecl InitItems();
void __fastcall CalcPlrItemVals(int player_num, bool load_gfx);
void __fastcall CalcPlrScrolls(int p);
void __fastcall CalcPlrStaff(int pnum);
void __fastcall CalcSelfItems(int pnum);
void __fastcall CalcPlrItemMin(int pnum);
unsigned char __fastcall ItemMinStats(PlayerStruct *p, ItemStruct *x);
void __fastcall CalcPlrBookVals(int p);
void __fastcall CalcPlrInv(int p, unsigned char Loadgfx);
void __fastcall SetPlrHandItem(ItemStruct *item, int item_id);
void __fastcall GetPlrHandSeed(ItemStruct *item);
void __fastcall GetGoldSeed(int player_num, int *item_seed);
void __fastcall SetPlrHandSeed(ItemStruct *h, int iseed);
void __fastcall SetPlrHandGoldCurs(ItemStruct *h);
void __fastcall CreatePlrItems(int player_num);
unsigned char __fastcall ItemSpaceOk(int i, int j);
unsigned char __fastcall GetItemSpace(int x, int y, char inum);
void __fastcall GetSuperItemSpace(int x, int y, char inum);
void __fastcall GetSuperItemLoc(int x, int y, int *xx, int *yy);
void __fastcall CalcItemValue(int i);
void __fastcall GetBookSpell(int i, int lvl);
void __fastcall GetStaffPower(int i, int lvl, int bs, unsigned char onlygood);
void __fastcall GetStaffSpell(int i, int lvl, unsigned char onlygood);
void __fastcall GetItemAttrs(int i, int idata, int lvl);
int __fastcall RndPL(int param1, int param2);
int __fastcall PLVal(int pv, int p1, int p2, int minv, int maxv);
void __fastcall SaveItemPower(int i, int power, int param1, int param2, int minval, int maxval, int multval);
void __fastcall GetItemPower(int i, int minlvl, int maxlvl, __int32 flgs, int onlygood);
void __fastcall GetItemBonus(int i, int idata, int minlvl, int maxlvl, int onlygood);
void __fastcall SetupItem(int i);
int __fastcall RndItem(int monster_num);
int __fastcall RndUItem(int m);
int __cdecl RndAllItems();
int __fastcall RndTypeItems(int itype, int imid);
void __fastcall GetUniqueItem(int i, int uid);
void __fastcall SpawnUnique(int uid, int x, int y);
int __fastcall CheckUnique(int i, int lvl, int uper, unsigned char recreate);
void __fastcall ItemRndDur(int ii);
void __fastcall SetupAllItems(int ii, int idx, int iseed, int lvl, int uper, int onlygood, int recreate, int pregen);
void __fastcall SpawnItem(int m, int x, int y, unsigned char sendmsg);
void __fastcall CreateItem(int uid, int x, int y);
void __fastcall CreateRndItem(int x, int y, unsigned char onlygood, unsigned char sendmsg, int delta);
void __fastcall SetupAllUseful(int ii, int iseed, int lvl);
void __fastcall CreateRndUseful(int pnum, int x, int y, unsigned char sendmsg);
void __fastcall CreateTypeItem(int x, int y, unsigned char onlygood, int itype, int imisc, int sendmsg, int delta);
void __fastcall TempItemGeneration(int ii, int idx, int wCI, int seed, int value);
void __fastcall RecreateEar(int ii, unsigned short ic, int iseed, unsigned char Id, int dur, int mdur, int ch, int mch, int ivalue, int ibuff);
void __fastcall SpawnQuestItem(int itemid, int x, int y, int randarea, int selflag);
void __cdecl SpawnRock();
void __fastcall RespawnItem(int i, unsigned char FlipFlag);
void __fastcall DeleteItem(int ii, int i);
void __cdecl ItemDoppel();
void __cdecl ProcessItems();
void __cdecl FreeItemGFX();
void __fastcall items_get_drop_cel(int i);
void __fastcall GetItemStr(int i);
void __fastcall CheckIdentify(int pnum, int cii);
void __fastcall DoRepair(int pnum, int cii);
void __fastcall RepairItem(ItemStruct *i, int lvl);
void __fastcall DoRecharge(int pnum, int cii);
void __fastcall RechargeItem(ItemStruct *i, int r);
void __fastcall PrintItemOil(char IDidx);
void __fastcall PrintItemPower(char plidx, ItemStruct *x);
void __cdecl items_unique_info_cel();
void __fastcall PrintUString(int x, int y, int cjustflag, char *str, int col);
void __fastcall items_unique_info_box(int y);
void __cdecl DrawUniqueInfo();
void __fastcall PrintItemMisc(ItemStruct *x);
void __fastcall PrintItemDetails(ItemStruct *x);
void __fastcall PrintItemDur(ItemStruct *x);
void __fastcall UseItem(int p, int Mid, int spl);
bool __fastcall StoreStatOk(ItemStruct *h);
int __fastcall SmithItemOk(int i);
int __fastcall RndSmithItem(int lvl);
void __fastcall BubbleSwapItem(ItemStruct *a, ItemStruct *b);
void __cdecl SortSmith();
void __fastcall SpawnSmith(int lvl);
int __fastcall PremiumItemOk(int i);
int __fastcall RndPremiumItem(int minlvl, int maxlvl);
void __fastcall SpawnOnePremium(int i, int plvl);
void __fastcall SpawnPremium(int lvl);
int __fastcall WitchItemOk(int i);
int __fastcall RndWitchItem(int lvl);
void __cdecl SortWitch();
void __fastcall WitchBookLevel(int ii);
void __fastcall SpawnWitch(int lvl);
int __fastcall RndBoyItem(int lvl);
void __fastcall SpawnBoy(int lvl);
int __fastcall HealerItemOk(int i);
int __fastcall RndHealerItem(int lvl);
void __cdecl SortHealer();
void __fastcall SpawnHealer(int lvl);
void __cdecl SpawnStoreGold();
void __fastcall RecreateSmithItem(int ii, int idx, int plvl, int iseed);
void __fastcall RecreatePremiumItem(int ii, int idx, int lvl, int iseed);
void __fastcall RecreateBoyItem(int ii, int idx, int lvl, int iseed);
void __fastcall RecreateWitchItem(int ii, int idx, int lvl, int iseed);
void __fastcall RecreateHealerItem(int ii, int idx, int lvl, int iseed);
void __fastcall RecreateTownItem(int ii, int idx, unsigned short icreateinfo, int iseed, int ivalue);
void __cdecl RecalcStoreStats();
void __cdecl ItemNoFlippy();
void __fastcall CreateSpellBook(int x, int y, int ispell, unsigned char sendmsg, int delta);
void __fastcall CreateMagicItem(int x, int y, int imisc, int icurs, int sendmsg, int delta);
int __fastcall GetItemRecord(int dwSeed, int CI, int indx);
void __fastcall NextItemRecord(int a1);
void __fastcall SetItemRecord(int dwSeed, int CI, int indx);
void __fastcall PutItemRecord(int seed, int ci, int index);
void __fastcall SetLightFX(int x, int y, short s_r, short s_g, int s_b, int d_r, int d_g, int d_b);
void __fastcall DoLighting(int nXPos, int nYPos, int nRadius, int Lnum);
void __fastcall DoUnLight(int nXPos, int nYPos, int nRadius);
void __fastcall DoUnVision(int nXPos, int nYPos, int nRadius);
void __fastcall DoVision(int nXPos, int nYPos, int nRadius, unsigned char doautomap, int visible);
void __cdecl FreeLightTable();
void __cdecl InitLightTable();
void __cdecl MakeLightTable();
void __cdecl InitLightMax();
void __cdecl InitLighting();
int __fastcall AddLight(int x, int y, int r);
void __fastcall AddUnLight(int i);
void __fastcall ChangeLightRadius(int i, int r);
void __fastcall ChangeLightXY(int i, int x, int y);
void __fastcall ChangeLightOff(int i, int x, int y);
void __fastcall ChangeLight(int i, int x, int y, int r);
void __cdecl ProcessLightList();
void __cdecl SavePreLighting();
void __cdecl InitVision();
int __fastcall AddVision(int x, int y, int r, unsigned char mine);
void __fastcall ChangeVisionRadius(int id, int r);
void __fastcall ChangeVisionXY(int id, int x, int y);
void __cdecl ProcessVisionList();
void __cdecl lighting_color_cycling();
void __fastcall LoadGame(void *a1);
char __cdecl BLoad();
int __cdecl ILoad();
int __cdecl ILoad_2();
bool __cdecl OLoad();
void __fastcall LoadPlayer(int player_num);
void __fastcall LoadMonster(int monster_num);
void __fastcall LoadMissile(int missile_num);
void __fastcall LoadObject(int object_num);
void __fastcall LoadItem(int item_num);
void __fastcall LoadPremium(int griswold_premium_item_num);
void __fastcall LoadQuest(int i);
void __fastcall LoadLighting(int light_num);
void __fastcall LoadVision(int vision_num);
void __fastcall LoadPortal(int portal_num);
void __cdecl SaveGame();
void __fastcall BSave(char v);
void __fastcall ISave(int v);
void __fastcall ISave_2(int v);
void __fastcall OSave(unsigned char v);
void __fastcall SavePlayer(int i);
void __fastcall SaveMonster(int i);
void __fastcall SaveMissile(int i);
void __fastcall SaveObject(int i);
void __fastcall SaveItem(int i);
void __fastcall SavePremium(int i);
void __fastcall SaveQuest(int i);
void __fastcall SaveLighting(int i);
void __fastcall SaveVision(int i);
void __fastcall SavePortal(int i);
void __cdecl SaveLevel();
void __cdecl LoadLevel();
void __cdecl log_cpp_init_1();
void __cdecl log_cpp_init_2();
void __cdecl log_init_mutex();
void __cdecl j_log_cleanup_mutex();
void __cdecl log_cleanup_mutex();
void __cdecl log_flush(bool force_close);
void *__cdecl log_create();
void __fastcall log_get_version(VS_FIXEDFILEINFO *file_info);
void log_printf(char *format, ...);
void __cdecl log_dump_computer_info();
void __cdecl mainmenu_cpp_init();
void __cdecl mainmenu_refresh_music();
void __stdcall mainmenu_create_hero(char *, char *);
int __stdcall mainmenu_select_hero_dialog(int u1, int u2, int u3, int u4, int mode, char *cname, int clen, char *cdesc, int cdlen, int *multi);
void __fastcall mainmenu_action(int option);
int __cdecl mainmenu_single_player();
int __fastcall mainmenu_init_menu(int a1);
void __cdecl mainmenu_multi_player();
void __cdecl mainmenu_play_intro();
void __cdecl FreeQuestText();
void __cdecl InitQuestText();
void __fastcall InitQTextMsg(int m);
void __cdecl DrawQTextBack();
void __fastcall PrintQTextChr(int screen_x, int screen_y, char *cel_buf, int frame);
void __cdecl DrawQText();
void __fastcall GetDamageAmt(int i, int *mind, int *maxd);
int __fastcall CheckBlock(int fx, int fy, int tx, int ty);
int __fastcall FindClosest(int sx, int sy, int rad);
int __fastcall GetSpellLevel(int id, int sn);
int __fastcall GetDirection8(int x1, int y1, int x2, int y2);
int __fastcall GetDirection16(int x1, int y1, int x2, int y2);
void __fastcall DeleteMissile(int mi, int i);
void __fastcall GetMissileVel(int i, int sx, int sy, int dx, int dy, int v);
void __fastcall PutMissile(int i);
void __fastcall GetMissilePos(int i);
void __fastcall MoveMissilePos(int i);
unsigned char __fastcall MonsterTrapHit(int m, int mindam, int maxdam, int dist, int t, int shift);
unsigned char __fastcall MonsterMHit(int pnum, int m, int mindam, int maxdam, int dist, int t, int shift);
unsigned char __fastcall PlayerMHit(int pnum, int m, int dist, int mind, int maxd, int mtype, int shift, int earflag);
unsigned char __fastcall Plr2PlrMHit(int pnum, int p, int mindam, int maxdam, int dist, int mtype, int shift);
void __fastcall CheckMissileCol(int i, int mindam, int maxdam, unsigned char shift, int mx, int my, int nodel, bool HurtPlr);
void __fastcall SetMissAnim(int mi, int animtype);
void __fastcall SetMissDir(int mi, int dir);
void __fastcall LoadMissileGFX(int mi);
void __cdecl InitMissileGFX();
void __fastcall FreeMissileGFX(int mi);
void __cdecl FreeMissiles();
void __cdecl FreeMissiles2();
void __cdecl InitMissiles();
void __fastcall AddLArrow(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddArrow(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall GetVileMissPos(int mi, int dx, int dy);
void __fastcall AddRndTeleport(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddFirebolt(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int dam);
void __fastcall AddMagmaball(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall miss_null_33(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddTeleport(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddLightball(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddFirewall(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddFireball(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddLightctrl(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddLightning(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddMisexp(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddWeapexp(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
unsigned char __fastcall CheckIfTrig(int x, int y);
void __fastcall AddTown(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddFlash(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddFlash2(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddManashield(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddFiremove(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddGuardian(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddChain(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall miss_null_11(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall miss_null_12(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall miss_null_13(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddRhino(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall miss_null_32(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddFlare(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddAcid(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall miss_null_1D(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddAcidpud(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddStone(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddGolem(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddEtherealize(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall miss_null_1F(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall miss_null_23(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddBoom(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddHeal(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddHealOther(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddElement(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddIdentify(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddFirewallC(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddInfra(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddWave(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddNova(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddRepair(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddRecharge(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddDisarm(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddApoca(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddFlame(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddFlamec(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddCbolt(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int dam);
void __fastcall AddHbolt(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int dam);
void __fastcall AddResurrect(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddResurrectBeam(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddTelekinesis(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddBoneSpirit(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddRportal(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
void __fastcall AddDiabApoca(int mi, int sx, int sy, int dx, int dy, int midir, int mienemy, int id, int dam);
int __fastcall AddMissile(int sx, int sy, int v1, int v2, int midir, int mitype, int micaster, int id, int v3, int spllvl);
int __fastcall Sentfire(int i, int sx, int sy);
void __fastcall MI_Dummy(int i);
void __fastcall MI_Golem(int i);
void __fastcall MI_SetManashield(int i);
void __fastcall MI_LArrow(int i);
void __fastcall MI_Arrow(int i);
void __fastcall MI_Firebolt(int i);
void __fastcall MI_Lightball(int i);
void __fastcall mi_null_33(int i);
void __fastcall MI_Acidpud(int i);
void __fastcall MI_Firewall(int i);
void __fastcall MI_Fireball(int i);
void __fastcall MI_Lightctrl(int i);
void __fastcall MI_Lightning(int i);
void __fastcall MI_Town(int i);
void __fastcall MI_Flash(int i);
void __fastcall MI_Flash2(int i);
void __fastcall MI_Manashield(int i);
void __fastcall MI_Etherealize(int i);
void __fastcall MI_Firemove(int i);
void __fastcall MI_Guardian(int i);
void __fastcall MI_Chain(int i);
void __fastcall mi_null_11(int i);
void __fastcall MI_Weapexp(int i);
void __fastcall MI_Misexp(int i);
void __fastcall MI_Acidsplat(int i);
void __fastcall MI_Teleport(int i);
void __fastcall MI_Stone(int i);
void __fastcall MI_Boom(int i);
void __fastcall MI_Rhino(int i);
void __fastcall mi_null_32(int i);
void __fastcall MI_FirewallC(int i);
void __fastcall MI_Infra(int i);
void __fastcall MI_Apoca(int i);
void __fastcall MI_Wave(int i);
void __fastcall MI_Nova(int i);
void __fastcall MI_Blodboil(int i);
void __fastcall MI_Flame(int i);
void __fastcall MI_Flamec(int i);
void __fastcall MI_Cbolt(int i);
void __fastcall MI_Hbolt(int i);
void __fastcall MI_Element(int i);
void __fastcall MI_Bonespirit(int i);
void __fastcall MI_ResurrectBeam(int i);
void __fastcall MI_Rportal(int i);
void __cdecl ProcessMissiles();
void __cdecl missiles_process_charge();
void __fastcall ClearMissileSpot(int i);
void __cdecl monster_cpp_init();
void __fastcall monster_init_special(int mon_id, int special);
void __cdecl InitLevelMonsters();
int __fastcall AddMonsterType(int type, int placeflag);
void __cdecl GetLevelMTypes();
void __fastcall InitMonsterGFX(int monst);
void __fastcall ClearMVars(int i);
void __fastcall InitMonster(int i, int rd, int mtype, int x, int y);
void __cdecl ClrAllMonsters();
int __fastcall MonstPlace(int xp, int yp);
void __fastcall PlaceMonster(int i, int mtype, int x, int y);
void __fastcall PlaceUniqueMonst(int uniqindex, int miniontype, int unpackfilesize);
void __cdecl PlaceQuestMonsters();
void __fastcall PlaceGroup(int mtype, int num, unsigned char leaderf, int leader);
void __cdecl LoadDiabMonsts();
void __cdecl InitMonsters();
void __cdecl PlaceUniques();
void __fastcall SetMapMonsters(char *pMap, int startx, int starty);
void __fastcall DeleteMonster(int i);
int __fastcall AddMonster(int x, int y, int dir, int mtype, int InMap);
void __fastcall NewMonsterAnim(int i, AnimStruct *anim, int md);
int __fastcall M_Ranged(int i);
int __fastcall M_Talker(int i);
void __fastcall M_Enemy(int i);
int __fastcall M_GetDir(int i);
void __fastcall M_CheckEFlag(int i);
void __fastcall M_StartStand(int i, int md);
void __fastcall M_StartDelay(int i, int len);
void __fastcall M_StartSpStand(int i, int md);
void __fastcall M_StartWalk(int i, int xvel, int yvel, int xadd, int yadd, int EndDir);
void __fastcall M_StartWalk2(int i, int xvel, int yvel, int a4, int a5, int a6, int a7, int EndDir);
void __fastcall M_StartWalk3(int i, int xvel, int yvel, int a4, int a5, int a6, int a7, int a8, int a9, int EndDir);
void __fastcall M_StartAttack(int i);
void __fastcall M_StartRAttack(int i, int missile_type, int dam);
void __fastcall M_StartRSpAttack(int i, int missile_type, int dam);
void __fastcall M_StartSpAttack(int i);
void __fastcall M_StartEat(int i);
void __fastcall M_ClearSquares(int i);
void __fastcall M_GetKnockback(int i);
void __fastcall M_StartHit(int i, int pnum, int dam);
void __fastcall M_DiabloDeath(int i, unsigned char sendmsg);
void __fastcall M2MStartHit(int mid, int i, int dam);
void __fastcall MonstStartKill(int i, int pnum, unsigned char sendmsg);
void __fastcall M2MStartKill(int i, int mid);
void __fastcall M_StartKill(int i, int pnum);
void __fastcall M_SyncStartKill(int i, int x, int y, int pnum);
void __fastcall M_StartFadein(int i, int md, unsigned char backwards);
void __fastcall M_StartFadeout(int i, int md, unsigned char backwards);
void __fastcall M_StartHeal(int i);
void __fastcall M_ChangeLightOffset(int monst);
int __fastcall M_DoStand(int i);
int __fastcall M_DoWalk(int i);
int __fastcall M_DoWalk2(int i);
int __fastcall M_DoWalk3(int i);
void __fastcall M_TryM2MHit(int i, int mid, int hper, int mind, int maxd);
void __fastcall M_TryH2HHit(int i, int pnum, int Hit, int MinDam, int MaxDam);
int __fastcall M_DoAttack(int i);
int __fastcall M_DoRAttack(int i);
int __fastcall M_DoRSpAttack(int i);
int __fastcall M_DoSAttack(int i);
int __fastcall M_DoFadein(int i);
int __fastcall M_DoFadeout(int i);
int __fastcall M_DoHeal(int i);
int __fastcall M_DoTalk(int i);
void __fastcall M_Teleport(int i);
int __fastcall M_DoGotHit(int i);
int __fastcall M_UpdateLeader(int i);
void __cdecl DoEnding();
void __cdecl PrepDoEnding();
int __fastcall M_DoDeath(int i);
int __fastcall M_DoSpStand(int i);
int __fastcall M_DoDelay(int i);
int __fastcall M_DoStone(int i);
void __fastcall M_WalkDir(int i, int md);
void __fastcall GroupUnity(int i);
unsigned char __fastcall M_CallWalk(int i, int md);
int __fastcall M_PathWalk(int i);
unsigned char __fastcall M_CallWalk2(int i, int md);
unsigned char __fastcall M_DumbWalk(int i, int md);
unsigned char __fastcall M_RoundWalk(int i, int md, int *dir);
void __fastcall MAI_Zombie(int i);
void __fastcall MAI_SkelSd(int i);
void __fastcall MAI_Path(int i);
void __fastcall MAI_Snake(int i);
void __fastcall MAI_Bat(int i);
void __fastcall MAI_SkelBow(int i);
void __fastcall MAI_Fat(int i);
void __fastcall MAI_Sneak(int i);
void __fastcall MAI_Fireman(int i);
void __fastcall MAI_Fallen(int i);
void __fastcall MAI_Cleaver(int i);
void __fastcall MAI_Round(int i, unsigned char special);
void __fastcall MAI_GoatMc(int i);
void __fastcall MAI_Ranged(int i, int missile_type, unsigned char special);
void __fastcall MAI_GoatBow(int i);
void __fastcall MAI_Succ(int i);
void __fastcall MAI_AcidUniq(int i);
void __fastcall MAI_Scav(int i);
void __fastcall MAI_Garg(int i);
void __fastcall MAI_RoundRanged(int i, int missile_type, unsigned char checkdoors, int dam, int lessmissiles);
void __fastcall MAI_Magma(int i);
void __fastcall MAI_Storm(int i);
void __fastcall MAI_Acid(int i);
void __fastcall MAI_Diablo(int i);
void __fastcall MAI_RR2(int i, int mistype, int dam);
void __fastcall MAI_Mega(int i);
void __fastcall MAI_Golum(int i);
void __fastcall MAI_SkelKing(int i);
void __fastcall MAI_Rhino(int i);
void __fastcall MAI_Counselor(int i);
void __fastcall MAI_Garbud(int i);
void __fastcall MAI_Zhar(int i);
void __fastcall MAI_SnotSpil(int i);
void __fastcall MAI_Lazurus(int i);
void __fastcall MAI_Lazhelp(int i);
void __fastcall MAI_Lachdanan(int i);
void __fastcall MAI_Warlord(int i);
void __cdecl DeleteMonsterList();
void __cdecl ProcessMonsters();
void __cdecl FreeMonsters();
unsigned char __fastcall DirOK(int i, int mdir);
unsigned char __fastcall PosOkMissile(int x, int y);
unsigned char __fastcall CheckNoSolid(int x, int y);
unsigned char __fastcall LineClearF(unsigned char (__cdecl *Clear)(), int x1, int y1, int x2, int y2);
unsigned char __fastcall LineClear(int x1, int y1, int x2, int y2);
unsigned char __fastcall LineClearF1(unsigned char (__cdecl *Clear)(), int monst, int x1, int y1, int x2, int y2);
int __fastcall SyncMonsterAnim(int monst);
void __fastcall M_FallenFear(int x, int y);
void __fastcall PrintMonstHistory(int mt);
void __cdecl PrintUniqueHistory();
void __fastcall MissToMonst(int i, int x, int y);
int __fastcall PosOkMonst(int i, int x, int y);
int __fastcall PosOkMonst2(int i, int x, int y);
int __fastcall PosOkMonst3(int i, int x, int y);
BOOL __fastcall IsSkel(int mt);
BOOL __fastcall IsGoat(int mt);
int __fastcall M_SpawnSkel(int x, int y, int dir);
void __fastcall ActivateSpawn(int i, int x, int y, int dir);
int __fastcall SpawnSkeleton(int ii, int x, int y);
int __cdecl PreSpawnSkeleton();
void __fastcall TalktoMonster(int i);
void __fastcall SpawnGolum(int i, int x, int y, int mi);
unsigned char __fastcall CanTalkToMonst(int m);
unsigned char __fastcall CheckMonsterHit(int m, unsigned char *ret);
int __cdecl encode_enemy(int m);
void __fastcall decode_enemy(int m, int enemy);
void __cdecl movie_cpp_init();
void __fastcall play_movie(char *pszMovie, bool user_can_close);
LRESULT __stdcall MovieWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
void __cdecl mpqapi_cpp_init();
bool __fastcall mpqapi_set_hidden(char *save_path, bool hidden);
void __fastcall mpqapi_store_creation_time(char *save_path, int save_num);
bool __fastcall mpqapi_reg_load_modification_time(char *dst, int size);
void __fastcall mpqapi_xor_buf(char *buf);
bool __fastcall mpqapi_reg_store_modification_time(char *src, int len);
_BLOCKENTRY *__fastcall j_mpqapi_remove_hash_entry(char *a1);
void __fastcall mpqapi_remove_hash_entry(char *path);
void __fastcall mpqapi_alloc_block(int block_offset, int block_size);
_BLOCKENTRY *__fastcall mpqapi_new_block(int *block_index);
int __fastcall mpqapi_get_hash_index_of_path(char *path);
int __fastcall mpqapi_get_hash_index(short index, int hash_a, int hash_b, int locale);
void __fastcall mpqapi_remove_hash_entries(bool (__stdcall *get_file_name)(int lvl, char *file_name));
bool __fastcall mpqapi_write_file(char *file_name, char *buf, int len);
_BLOCKENTRY *__fastcall mpqapi_add_file(char *path, _BLOCKENTRY *block, int block_index);
bool __fastcall mpqapi_write_file_contents(char *path, char *buf, int len, _BLOCKENTRY *block);
int __fastcall mpqapi_find_free_block(int size, int *block_size);
void __fastcall mpqapi_rename(char *old_name, char *new_name);
bool __fastcall mpqapi_has_file(char *path);
bool __fastcall mpqapi_open_archive(char *save_path, bool hidden, int save_num);
bool __fastcall mpqapi_parse_archive_header(TMPQHeader *header, int *mpq_offset);
void __fastcall mpqapi_close_archive(char *save_path, bool free_tables, int save_num);
void __fastcall mpqapi_store_modified_time(char *save_path, int save_num);
void __fastcall mpqapi_flush_and_close(char *save_path, bool is_single_player, int save_num);
bool __cdecl mpqapi_write_header();
bool __cdecl mpqapi_write_block_table();
bool __cdecl mpqapi_write_hash_table();
bool __cdecl mpqapi_can_seek();
void __cdecl msg_cpp_init();
void __fastcall msg_send_drop_pkt(int pnum, int reason);
void __fastcall msg_send_packet(int pnum, void *packet, int dwSize);
TMegaPkt *__cdecl msg_get_next_packet();
int __cdecl msg_wait_resync();
void __cdecl msg_free_packets();
int __cdecl msg_wait_for_turns();
void __cdecl msg_process_net_packets();
void __cdecl msg_pre_packet();
void __fastcall msg_do_sync(int pnum);
void *__fastcall msg_sync_items(void *dst, void *src);
void *__fastcall msg_sync_objects(void *dst, void *src);
void *__fastcall msg_sync_monsters(void *dst, void *src);
char *__fastcall msg_sync_portals(char *a1);
int __fastcall msg_comp_level(char *buffer, int size);
void __cdecl delta_init();
void __fastcall delta_kill_monster(int mi, unsigned char x, unsigned char y, unsigned char bLevel);
void __fastcall delta_monster_hp(int mi, __int32 hp, unsigned char bLevel);
void __fastcall delta_sync_monster(TCmdLocParam1 *packet, char level);
void __fastcall delta_sync_golem(TCmdGolem *pG, int pnum, int bLevel);
void __fastcall delta_leave_sync(unsigned char bLevel);
bool __fastcall delta_portal_inited(int portal_num);
bool __fastcall delta_quest_inited(int quest_num);
void __fastcall DeltaAddItem(int ii);
void __cdecl DeltaSaveLevel();
void __cdecl DeltaLoadLevel();
void __fastcall NetSendCmd(unsigned char bHiPri, unsigned char bCmd);
void __fastcall NetSendCmdGolem(unsigned char mx, unsigned char my, unsigned char dir, unsigned char menemy, __int32 hp, int cl);
void __fastcall NetSendCmdLoc(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y);
void __fastcall NetSendCmdLocParam1(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1);
void __fastcall NetSendCmdLocParam2(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1, int wParam2);
void __fastcall NetSendCmdLocParam3(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1, int wParam2, int wParam3);
void __fastcall NetSendCmdParam1(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1);
void __fastcall NetSendCmdParam2(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1, unsigned short wParam2);
void __fastcall NetSendCmdParam3(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1, unsigned short wParam2, int wParam3);
void __fastcall NetSendCmdQuest(unsigned char bHiPri, unsigned char q);
void __fastcall NetSendCmdGItem(unsigned char bHiPri, unsigned char bCmd, unsigned char mast, unsigned char pnum, int ii);
void __fastcall NetSendCmdGItem2(unsigned char usonly, unsigned char bCmd, unsigned char mast, unsigned char pnum, struct TCmdGItem *p);
unsigned char __fastcall NetSendCmdReq2(unsigned char bCmd, unsigned char mast, unsigned char pnum, struct TCmdGItem *p);
void __fastcall NetSendCmdExtra(struct TCmdGItem *p);
void __fastcall NetSendCmdPItem(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y);
void __fastcall NetSendCmdChItem(unsigned char bHiPri, unsigned char bLoc);
void __fastcall NetSendCmdDelItem(unsigned char bHiPri, unsigned char bLoc);
void __fastcall NetSendCmdDItem(unsigned char bHiPri, int ii);
void __fastcall NetSendCmdDamage(unsigned char bHiPri, unsigned char bPlr, unsigned __int32 dwDam);
void __fastcall msg_init_msg(int a1, const char *pszStr);
void __fastcall RemovePlrPortal(int pnum);
int __fastcall ParseCmd(int pnum, TCmd *pCmd);
void __fastcall DoCopySync(unsigned char cmd, int recv_offset);
void *__fastcall msg_copy_items(void *src, void *dst);
void *__fastcall msg_copy_objects(void *src, void *dst);
void *__fastcall msg_copy_monsters(void *src, void *dst);
char __fastcall msg_copy_portals(int a1);
int __fastcall On_SYNCDATA(void *packet, int pnum);
int __fastcall On_WALKXY(struct TCmdLoc *pCmd, int pnum);
int __fastcall On_ADDSTR(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_ADDMAG(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_ADDDEX(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_ADDVIT(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_SBSPELL(struct TCmdParam1 *pCmd, int pnum);
void msg_errorf(char *format, ...);
int __fastcall On_GOTOGETITEM(struct TCmdLocParam1 *pCmd, int pnum);
int __fastcall On_REQUESTGITEM(struct TCmdGItem *pCmd, int pnum);
bool __fastcall i_own_level(int nReqLevel);
int __fastcall On_GETITEM(struct TCmdGItem *pCmd, int pnum);
unsigned char __fastcall delta_get_item(struct TCmdGItem *pI, unsigned char bLevel);
int __fastcall On_GOTOAGETITEM(struct TCmdLocParam1 *pCmd, int pnum);
int __fastcall On_REQUESTAGITEM(struct TCmdGItem *pCmd, int pnum);
int __fastcall On_AGETITEM(struct TCmdGItem *pCmd, int pnum);
int __fastcall On_ITEMEXTRA(struct TCmdGItem *pCmd, int pnum);
int __fastcall On_PUTITEM(struct TCmdPItem *pCmd, int pnum);
void __fastcall delta_put_item(struct TCmdPItem *pI, int x, int y, unsigned char bLevel);
void __fastcall check_update_plr(int pnum);
int __fastcall On_SYNCPUTITEM(struct TCmdPItem *pCmd, int pnum);
int __fastcall On_RESPAWNITEM(struct TCmdPItem *pCmd, int pnum);
int __fastcall On_ATTACKXY(struct TCmdLoc *pCmd, int pnum);
int __fastcall On_SATTACKXY(struct TCmdLoc *pCmd, int pnum);
int __fastcall On_RATTACKXY(struct TCmdLoc *pCmd, int pnum);
int __fastcall On_SPELLXYD(struct TCmdLocParam3 *pCmd, int pnum);
int __fastcall On_SPELLXY(struct TCmdLocParam2 *pCmd, int pnum);
int __fastcall On_TSPELLXY(struct TCmdLocParam2 *pCmd, int pnum);
int __fastcall On_OPOBJXY(struct TCmdLocParam1 *pCmd, int pnum);
int __fastcall On_DISARMXY(struct TCmdLocParam1 *pCmd, int pnum);
int __fastcall On_OPOBJT(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_ATTACKID(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_ATTACKPID(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_RATTACKID(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_RATTACKPID(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_SPELLID(struct TCmdLocParam2 *pCmd, int pnum);
int __fastcall On_SPELLPID(struct TCmdLocParam2 *pCmd, int pnum);
int __fastcall On_TSPELLID(struct TCmdLocParam2 *pCmd, int pnum);
int __fastcall On_TSPELLPID(struct TCmdLocParam2 *pCmd, int pnum);
int __fastcall On_KNOCKBACK(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_RESURRECT(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_HEALOTHER(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_TALKXY(struct TCmdLocParam1 *pCmd, int pnum);
int __fastcall On_NEWLVL(struct TCmdParam2 *pCmd, int pnum);
int __fastcall On_WARP(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_MONSTDEATH(struct TCmdLocParam1 *pCmd, int pnum);
int __fastcall On_KILLGOLEM(struct TCmdLocParam1 *pCmd, int pnum);
int __fastcall On_AWAKEGOLEM(struct TCmdGolem *pCmd, int pnum);
int __fastcall On_MONSTDAMAGE(struct TCmdLocParam1 *pCmd, int pnum);
int __fastcall On_PLRDEAD(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_PLRDAMAGE(struct TCmdDamage *pCmd, int pnum);
int __fastcall On_OPENDOOR(struct TCmdParam1 *pCmd, int pnum);
void __fastcall delta_sync_object(int oi, unsigned char bCmd, unsigned char bLevel);
int __fastcall On_CLOSEDOOR(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_OPERATEOBJ(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_PLROPOBJ(struct TCmdParam2 *pCmd, int pnum);
int __fastcall On_BREAKOBJ(struct TCmdParam2 *pCmd, int pnum);
int __fastcall On_CHANGEPLRITEMS(struct TCmdChItem *pCmd, int pnum);
int __fastcall On_DELPLRITEMS(struct TCmdDelItem *pCmd, int pnum);
int __fastcall On_PLRLEVEL(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_DROPITEM(struct TCmdPItem *pCmd, int pnum);
int __fastcall On_SEND_PLRINFO(struct TCmdPlrInfoHdr *pCmd, int pnum);
int __fastcall On_ACK_PLRINFO(struct TCmdPlrInfoHdr *pCmd, int pnum);
int __fastcall On_PLAYER_JOINLEVEL(struct TCmdLocParam1 *pCmd, int pnum);
int __fastcall On_ACTIVATEPORTAL(DJunk *pCmd, int pnum);
void __fastcall delta_open_portal(int pnum, int x, int y, int bLevel, int bLType, int bSetLvl);
int __fastcall On_DEACTIVATEPORTAL(struct TCmd *pCmd, int pnum);
int __fastcall On_RETOWN(struct TCmd *pCmd, int pnum);
int __fastcall On_SETSTR(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_SETDEX(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_SETMAG(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_SETVIT(struct TCmdParam1 *pCmd, int pnum);
int __fastcall On_STRING(struct TCmdString *pCmd, int pnum);
int __fastcall On_SYNCQUEST(struct TCmdQuest *pCmd, int pnum);
int __fastcall On_ENDSHIELD(int a1, int pnum);
int __cdecl On_DEBUG();
int __fastcall On_NOVA(struct TCmdLoc *pCmd, int pnum);
int __fastcall On_SETSHIELD(int unused, int pnum);
int __fastcall On_REMSHIELD(int unused, int pnum);
void __cdecl msgcmd_cpp_init_1();
void __cdecl msgcmd_cpp_init_2();
_SNETEVENT *__cdecl msgcmd_init_event();
int __cdecl msgcmd_init_exit_event();
void __cdecl msgcmd_clear_all_events();
void __cdecl msgcmd_free_pevent();
void __cdecl msgcmd_send_chat();
int __fastcall msgcmd_check_set_event(int a1);
int __fastcall msgcmd_set_event(const char *a1);
_SNETEVENT *__fastcall msgcmd_clear_event(_SNETEVENT *a1);
bool __fastcall msgcmd_free_event(_SNETEVENT *a1);
int __fastcall msgcmd_free_str_event(_SNETEVENT *a1);
int __fastcall msgcmd_alloc_event(int *a1, int a2, int a3, int a4);
char *__fastcall msgcmd_remove_event(_SNETEVENT *a1);
char *__fastcall msgcmd_event_type(_SNETEVENT *a1, int a2, int *a3, int a4);
void __fastcall msgcmd_wait_zero_event(_SNETEVENT *a1);
void __fastcall msgcmd_zero_event(_SNETEVENT *a1);
void __cdecl multi_cpp_init();
void __fastcall multi_msg_add(char *a1, unsigned char a2);
void __fastcall NetSendLoPri(unsigned char *pbMsg, unsigned char bLen);
void __fastcall multi_copy_packet(void *a1, void *packet, int size);
void __fastcall multi_send_packet(void *packet, int dwSize);
void __fastcall NetRecvPlrData(TPkt *pkt);
void __fastcall NetSendHiPri(unsigned char *pbMsg, unsigned char bLen);
char *__fastcall multi_recv_packet(void *packet, char *a2, int *a3);
void __fastcall multi_send_msg_packet(int a1, char *a2, unsigned char len);
void __cdecl multi_msg_countdown();
int __fastcall multi_start_countdown(int pnum, int a2);
void __fastcall multi_wait_delta_send(int pnum);
void __fastcall multi_player_left(int pnum, int reason);
void __cdecl multi_clear_left_tbl();
void __fastcall multi_player_left_msg(int pnum, int left);
void __cdecl multi_net_ping();
int __cdecl multi_handle_delta();
int __fastcall multi_check_pkt_valid(char *a1);
void __cdecl multi_mon_seeds();
void __cdecl multi_begin_timeout();
void __cdecl multi_check_drop_player();
void __cdecl multi_process_network_packets();
void __fastcall multi_handle_all_packets(int players, TPkt *packet, int a3);
void __cdecl multi_start_packets();
void __fastcall multi_send_zero_packet(int pnum, char a2, void *ptr, int len);
void __cdecl NetClose();
char __fastcall multi_event_handler(int a1);
void __stdcall multi_handle_events(_SNETEVENT *event);
int __fastcall NetInit(int bSinglePlayer, int *pfExitProgram);
void __fastcall multi_clear_pkt(char *a1);
void __fastcall multi_send_pinfo(int pnum, TCmdPlrInfoHdr *cmd);
int __fastcall InitNewSeed(int newseed);
void __cdecl SetupLocalCoords();
int __fastcall multi_init_single(_SNETPROGRAMDATA *client_info, _SNETPLAYERDATA *user_info, _SNETUIDATA *ui_info);
int __fastcall multi_init_multi(_SNETPROGRAMDATA *client_info, _SNETPLAYERDATA *user_info, _SNETUIDATA *ui_info, int *a4);
int __fastcall multi_upgrade(int *a1);
void __fastcall multi_player_joins(int pnum, TCmdPlrInfoHdr *cmd, int a3);
void __cdecl nthread_cpp_init_1();
void __cdecl nthread_cpp_init_2();
void __cdecl nthread_init_mutex();
void __cdecl nthread_cleanup_mutex_atexit();
void __cdecl nthread_cleanup_mutex();
void __fastcall nthread_terminate_game(char *func_name);
int __fastcall nthread_send_and_recv_turn(int cur_turn, int turn_delta);
void __fastcall nthread_recv_turns(bool *recieved);
void __cdecl nthread_set_turn_upper_bit();
void __fastcall nthread_start(bool set_turn_upper_bit);
unsigned int __stdcall nthread_handler(void *a1);
void __cdecl nthread_cleanup();
void __fastcall nthread_ignore_mutex(bool ignore_mutex);
bool __cdecl nthread_has_500ms_passed();
void __cdecl InitObjectGFX();
void __cdecl FreeObjectGFX();
int __fastcall RndLocOk(int xp, int yp);
void __fastcall InitRndLocObj(int min, int max, int objtype);
void __fastcall InitRndLocBigObj(int min, int max, int objtype);
void __fastcall InitRndLocObj5x5(int min, int max, int objtype);
void __cdecl ClrAllObjects();
void __cdecl AddTortures();
void __cdecl AddCandles();
void __fastcall AddBookLever(int lx1, int ly1, int lx2, int ly2, int x1, int y1, int x2, int y2, int msg);
void __cdecl InitRndBarrels();
void __fastcall AddL1Objs(int x1, int y1, int x2, int y2);
void __fastcall AddL2Objs(int x1, int y1, int x2, int y2);
void __fastcall AddL3Objs(int x1, int y1, int x2, int y2);
unsigned char __fastcall WallTrapLocOk(int xp, int yp);
void __cdecl AddL2Torches();
unsigned char __fastcall TorchLocOK(int xp, int yp);
void __cdecl AddObjTraps();
void __cdecl AddChestTraps();
void __fastcall LoadMapObjects(unsigned char *pMap, int startx, int starty, int x1, int y1, int w, int h, int leveridx);
void __fastcall LoadMapObjs(unsigned char *pMap, int startx, int starty);
void __cdecl AddDiabObjs();
void __cdecl AddStoryBooks();
void __fastcall AddHookedBodies(int freq);
void __cdecl AddL4Goodies();
void __cdecl AddLazStand();
void __fastcall InitObjects(int a1);
void __fastcall SetMapObjects(char *pMap, int startx, int starty);
void __fastcall DeleteObject(int oi, int i);
void __fastcall SetupObject(int i, int x, int y, int ot);
void __fastcall SetObjMapRange(int i, int x1, int y1, int x2, int y2, int v);
void __fastcall SetBookMsg(int i, int msg);
void __fastcall AddL1Door(int i, int x, int y, int ot);
void __fastcall AddSCambBook(int i);
void __fastcall AddChest(int i, int t);
void __fastcall AddL2Door(int i, int x, int y, int ot);
void __fastcall AddL3Door(int i, int x, int y, int ot);
void __fastcall AddSarc(int i);
void __fastcall AddFlameTrap(int i);
void __fastcall AddFlameLvr(int i);
void __fastcall AddTrap(int i);
void __fastcall AddObjLight(int i, int r);
void __fastcall AddBarrel(int i);
void __fastcall AddShrine(int i);
void __fastcall AddBookcase(int i);
void __fastcall AddPurifyingFountain(int i);
void __fastcall AddArmorStand(int i);
void __fastcall AddDecap(int i);
void __fastcall AddVilebook(int i);
void __fastcall AddMagicCircle(int i);
void __fastcall AddBookstand(int i);
void __fastcall AddPedistal(int i);
void __fastcall AddStoryBook(int i);
void __fastcall AddWeaponRack(int i);
void __fastcall AddTorturedBody(int i);
void __fastcall GetRndObjLoc(int randarea, int *xx, int *yy);
void __cdecl AddMushPatch();
void __cdecl AddSlainHero();
void __fastcall AddObject(int ot, int ox, int oy);
void __fastcall Obj_Light(int i, int lr);
void __fastcall Obj_Circle(int i);
void __fastcall Obj_StopAnim(int i);
void __fastcall Obj_Door(int i);
void __fastcall Obj_Sarc(int i);
void __fastcall ActivateTrapLine(int ttype, int tid);
void __fastcall Obj_FlameTrap(int i);
void __fastcall Obj_Trap(int i);
void __fastcall Obj_BCrossDamage(int i);
void __cdecl ProcessObjects();
void __fastcall ObjSetMicro(int dx, int dy, int pn);
void __fastcall objects_set_door_piece(int x, int y);
void __fastcall ObjSetMini(int x, int y, int v);
void __fastcall ObjL1Special(int x1, int y1, int x2, int y2);
void __fastcall ObjL2Special(int x1, int y1, int x2, int y2);
void __fastcall DoorSet(int oi, int dx, int dy);
void __cdecl RedoPlayerVision();
void __fastcall OperateL1RDoor(int pnum, int oi, unsigned char sendflag);
void __fastcall OperateL1LDoor(int pnum, int oi, unsigned char sendflag);
void __fastcall OperateL2RDoor(int pnum, int oi, unsigned char sendflag);
void __fastcall OperateL2LDoor(int pnum, int oi, unsigned char sendflag);
void __fastcall OperateL3RDoor(int pnum, int oi, unsigned char sendflag);
void __fastcall OperateL3LDoor(int pnum, int oi, unsigned char sendflag);
void __fastcall MonstCheckDoors(int m);
void __fastcall DRLG_MRectTrans(int x1, int y1, int x2, int y2);
void __fastcall ObjChangeMapResync(int x1, int y1, int x2, int y2);
void __fastcall OperateL1Door(int pnum, int i, unsigned char sendflag);
void __fastcall OperateLever(int pnum, int i);
void __fastcall OperateBook(int pnum, int i);
void __fastcall OperateBookLever(int pnum, int i);
void __fastcall OperateSChambBk(int pnum, int i);
void __fastcall OperateChest(int pnum, int i, unsigned char sendmsg);
void __fastcall OperateMushPatch(int pnum, int i);
void __fastcall OperateInnSignChest(int pnum, int i);
void __fastcall OperateSlainHero(int pnum, int i, unsigned char sendmsg);
void __fastcall OperateTrapLvr(int i);
void __fastcall OperateSarc(int pnum, int i, unsigned char sendmsg);
void __fastcall OperateL2Door(int pnum, int i, unsigned char sendflag);
void __fastcall OperateL3Door(int pnum, int i, unsigned char sendflag);
void __fastcall OperatePedistal(int pnum, int i);
void __fastcall TryDisarm(int pnum, int i);
int __fastcall ItemMiscIdIdx(int imiscid);
void __fastcall OperateShrine(int player_num, int object_num, int sfx_id);
void __fastcall OperateSkelBook(int pnum, int i, unsigned char sendmsg);
void __fastcall OperateBookCase(int pnum, int i, unsigned char sendmsg);
void __fastcall OperateDecap(int pnum, int i, unsigned char sendmsg);
void __fastcall OperateArmorStand(int pnum, int i, unsigned char sendmsg);
void __fastcall FindValidShrine();
void __fastcall OperateGoatShrine(int pnum, int i, int sType);
void __fastcall OperateCauldron(int pnum, int i, int sType);
unsigned char __fastcall OperateFountains(int pnum, int i);
void __fastcall OperateWeaponRack(int pnum, int i, unsigned char sendmsg);
void __fastcall OperateStoryBook(int pnum, int i);
void __fastcall OperateLazStand(int pnum, int i);
void __fastcall OperateObject(int pnum, int i, unsigned char TeleFlag);
void __fastcall SyncOpL1Door(int pnum, int cmd, int i);
void __fastcall SyncOpL2Door(int pnum, int cmd, int i);
void __fastcall SyncOpL3Door(int pnum, int cmd, int i);
void __fastcall SyncOpObject(int pnum, int cmd, int i);
void __fastcall BreakCrux(int i);
void __fastcall BreakBarrel(int pnum, int i, int dam, unsigned char forcebreak, int sendmsg);
void __fastcall BreakObject(int pnum, int oi);
void __fastcall SyncBreakObj(int pnum, int oi);
void __fastcall SyncL1Doors(int i);
void __fastcall SyncCrux(int i);
void __fastcall SyncLever(int i);
void __fastcall SyncQSTLever(int i);
void __fastcall SyncPedistal(int i);
void __fastcall SyncL2Doors(int i);
void __fastcall SyncL3Doors(int i);
void __fastcall SyncObjectAnim(int o);
void __fastcall GetObjectStr(int i);
void __cdecl pack_cpp_init();
void __fastcall PackPlayer(PkPlayerStruct *pPack, int pnum, bool manashield);
void __fastcall PackItem(PkItemStruct *id, ItemStruct *is);
void __fastcall VerifyGoldSeeds(PlayerStruct *pPlayer);
void __fastcall UnPackPlayer(PkPlayerStruct *pPack, int pnum, bool killok);
void __fastcall UnPackItem(PkItemStruct *is, ItemStruct *id);
void __cdecl palette_cpp_init();
void __cdecl palette_save_gamme();
void __cdecl palette_init();
void __cdecl palette_load_gamma();
void __cdecl palette_load_system_palette();
void __fastcall LoadPalette(char *pal_path);
void __fastcall LoadRndLvlPal(char dtype);
void __cdecl ResetPal();
void __cdecl palette_inc_gamma();
void __cdecl palette_update();
void __fastcall palette_apply_gamma_correction(PALETTEENTRY *dst, PALETTEENTRY *src, int n);
void __cdecl palette_dec_gamma();
int __fastcall palette_update_gamma(int gamma);
void __cdecl BlackPalette();
void __fastcall palette_set_brightness(int brightness);
void __fastcall PaletteFadeIn(int inc);
void __fastcall PaletteFadeOut(int dec);
void __cdecl palette_update_caves();
void __fastcall palette_update_quest_palette(int n);
bool __cdecl palette_get_colour_cycling();
void __fastcall palette_set_color_cycling(bool enabled);
int __fastcall FindPath(int (__fastcall *PosOk)(int, int, int), int PosOkArg, int sx, int sy, int dx, int dy, char *path);
int __fastcall path_xycoord(int x, int y, int xx, int yy);
int __fastcall path_check_equal(PATHNODE *pPath, int dx, int dy);
PATHNODE *__cdecl GetNextPath();
PATHNODE *__fastcall path_solid_pieces(PATHNODE *pPath, int dx, int dy);
int __fastcall path_get_path(int (__fastcall *PosOk)(int, int, int), int PosOkArg, PATHNODE *pPath, int x, int y);
PATHNODE *__fastcall path_parent_path(PATHNODE *pPath, int dx, int dy, int sx, int sy);
PATHNODE *__fastcall path_get_node_xy(int dx, int dy);
PATHNODE *__fastcall path_get_node_xyptr(int dx, int dy);
PATHNODE *__fastcall path_get_node2(PATHNODE *pPath);
PATHNODE *__fastcall path_set_coords(PATHNODE *pPath);
PATHNODE *__fastcall path_set_node_ptr(PATHNODE *pPath);
PATHNODE *__cdecl path_decrease_node();
PATHNODE *__cdecl path_clear_node();
void __cdecl pfile_cpp_init();
void __cdecl pfile_init_save_directory();
void __fastcall pfile_check_available_space(char *pszDir);
void __cdecl pfile_write_hero();
int __fastcall pfile_get_save_num_from_name(char *name);
void __fastcall pfile_encode_hero(PkPlayerStruct *pPack);
bool __fastcall pfile_open_archive(bool a1, int save_num);
void __fastcall pfile_get_save_path(char *pszBuf, int dwBufSize, int save_num);
void __fastcall pfile_flush(bool is_single_player, int save_num);
bool __fastcall pfile_create_player_description(char *dst, int len);
int __fastcall pfile_create_save_file(char *name_1, char *name_2);
void __cdecl pfile_flush_W();
void __fastcall game_2_ui_player(PlayerStruct *p, _uiheroinfo *heroinfo, bool bHasSaveFile);
char __fastcall game_2_ui_class(PlayerStruct *p);
bool __stdcall pfile_ui_set_hero_infos(void (__stdcall *ui_add_hero_info)(_uiheroinfo *));
char *__fastcall pfile_get_archive_path(char *dst, int dst_size, int save_num);
bool __fastcall pfile_read_hero(void *archive, PkPlayerStruct *hero);
void *__fastcall pfile_open_save_archive(int *unused, int save_num);
void __fastcall pfile_SFileCloseArchive(void *hsArchive);
bool __fastcall pfile_archive_contains_game(void *hsArchive);
bool __stdcall pfile_ui_set_class_stats(int player_class_nr, _uidefaultstats *class_stats);
int __fastcall pfile_get_player_class(int player_class_nr);
bool __stdcall pfile_ui_save_create(_uiheroinfo *heroinfo);
bool __stdcall pfile_get_file_name(int lvl, char *dst);
bool __stdcall pfile_delete_save(_uiheroinfo *hero_info);
void __cdecl pfile_read_player_from_save();
void __fastcall pfile_get_temp_level_name(char *dst);
void __fastcall pfile_get_perm_level_name(char *dst);
void __fastcall pfile_get_game_name(char *dst);
void __cdecl pfile_remove_temp_files();
bool __stdcall pfile_get_temp_name(int a1, char *dst);
void __cdecl pfile_rename_temp_to_perm();
bool __stdcall GetPermSaveNames(int dwIndex, char *szPerm);
void __fastcall pfile_write_save_file(char *pszName, void *pbData, int dwLen, int qwLen);
void __fastcall pfile_strcpy(char *dst, char *src);
char *__fastcall pfile_read(char *pszName, int *pdwLen);
void __fastcall pfile_update(bool force_save);
void __cdecl player_cpp_init();
void __fastcall player_copy_frames(char *src, char *dst);
void __fastcall LoadPlrGFX(int pnum, int a2);
void __fastcall InitPlayerGFX(int pnum);
void __fastcall InitPlrGFXMem(int pnum);
int __fastcall GetPlrGFXSize(char *szCel);
void __fastcall FreePlayerGFX(int pnum);
void __fastcall NewPlrAnim(int pnum, int Peq, int numFrames, int Delay);
void __fastcall ClearPlrPVars(int pnum);
void __fastcall SetPlrAnims(int pnum);
void __fastcall ClearPlrRVars(PlayerStruct *pPlayer);
void __fastcall CreatePlayer(int pnum, char c);
int __fastcall CalcStatDiff(int pnum);
void __fastcall NextPlrLevel(int pnum);
void __fastcall AddPlrExperience(int pnum, int lvl, __int32 exp);
void __fastcall AddPlrMonstExper(int lvl, __int32 exp, char pmask);
void __fastcall InitPlayer(int pnum, bool FirstTime);
void __cdecl InitMultiView();
int __fastcall CheckLeighSolid(int x, int y);
int __fastcall SolidLoc(int x, int y);
int __fastcall PlrDirOK(int pnum, int dir);
void __fastcall PlrClrTrans(int x, int y);
void __fastcall PlrDoTrans(int x, int y);
void __fastcall SetPlayerOld(int pnum);
void __fastcall FixPlayerLocation(int pnum, int a2);
void __fastcall StartStand(int pnum, int dir);
void __fastcall StartWalkStand(int pnum);
void __fastcall PM_ChangeLightOff(int pnum);
void __fastcall PM_ChangeOffset(int pnum);
void __fastcall StartWalk(int pnum, int xvel, int yvel, int xadd, int yadd, int EndDir, int sdir);
void __fastcall StartWalk2(int pnum, int xvel, int yvel, int xoff, int yoff, int xadd, int yadd, int EndDir, int sdir);
void __fastcall StartWalk3(int pnum, int xvel, int yvel, int xoff, int yoff, int xadd, int yadd, int mapx, int mapy, int EndDir, int sdir);
void __fastcall StartAttack(int pnum, int d);
void __fastcall StartRangeAttack(int pnum, int a2, int a3, int a4);
void __fastcall StartPlrBlock(int pnum, int dir);
void __fastcall StartSpell(int pnum, int d, int cx, int cy);
void __fastcall FixPlrWalkTags(int pnum);
void __fastcall RemovePlrFromMap(int pnum);
void __fastcall StartPlrHit(int pnum, int dam, unsigned char forcehit);
void __fastcall DestroyDupeItem(ItemStruct *pItem, int ci, int a3);
void __fastcall StartPlayerKill(int pnum, int earflag);
void __fastcall PlrDeadItem(int pnum, struct ItemStruct *itm, int xx, int yy);
void __fastcall DropHalfPlayersGold(int pnum);
void __fastcall SyncPlrKill(int pnum, int earflag);
void __fastcall j_StartPlayerKill(int pnum, int earflag);
void __fastcall RemovePlrMissiles(int pnum);
int __fastcall InitLevelChange(int pnum);
void __fastcall StartNewLvl(int pnum, int fom, int lvl);
void __fastcall RestartTownLvl(int pnum);
void __fastcall StartWarpLvl(int pnum, int pidx);
int __fastcall PM_DoNothing(int pnum);
int __fastcall PM_DoWalk(int pnum);
int __fastcall PM_DoWalk2(int pnum);
int __fastcall PM_DoWalk3(int pnum);
unsigned char __fastcall WeaponDur(int pnum, int durrnd);
unsigned char __fastcall PlrHitMonst(int pnum, int m);
unsigned char __fastcall PlrHitPlr(int pnum, char p);
unsigned char __fastcall PlrHitObj(int pnum, int mx, int my);
int __fastcall PM_DoAttack(int pnum);
int __fastcall PM_DoRangeAttack(int pnum);
void __fastcall ShieldDur(int pnum);
int __fastcall PM_DoBlock(int pnum);
int __fastcall PM_DoSpell(int pnum);
int __fastcall PM_DoGotHit(int pnum);
void __fastcall ArmorDur(int pnum);
int __fastcall PM_DoDeath(int pnum);
void __fastcall CheckNewPath(int pnum);
unsigned char __fastcall PlrDeathModeOK(int pnum);
void __cdecl ValidatePlayer();
void __cdecl ProcessPlayers();
void __fastcall CheckCheatStats(int pnum);
void __fastcall ClrPlrPath(int pnum);
int __fastcall PosOkPlayer(int pnum, int px, int py);
void __fastcall MakePlrPath(int pnum, int xx, int yy, unsigned char endspace);
void __fastcall CheckPlrSpell();
void __fastcall SyncPlrAnim(int pnum);
void __fastcall SyncInitPlrPos(int pnum);
void __fastcall SyncInitPlr(int pnum);
void __fastcall CheckStats(int pnum);
void __fastcall ModifyPlrStr(int pnum, int l);
void __fastcall ModifyPlrMag(int pnum, int l);
void __fastcall ModifyPlrDex(int pnum, int l);
void __fastcall ModifyPlrVit(int pnum, int l);
void __fastcall SetPlayerHitPoints(int pnum, int newhp);
void __fastcall SetPlrStr(int pnum, int v);
void __fastcall SetPlrMag(int pnum, int v);
void __fastcall SetPlrDex(int pnum, int v);
void __fastcall SetPlrVit(int pnum, int v);
void __fastcall InitDungMsgs(int pnum);
void __cdecl PlayDungMsgs();
void __fastcall plrmsg_delay(int a1);
char *__fastcall ErrorPlrMsg(char *a1);
size_t EventPlrMsg(char *szMsg, ...);
void __fastcall SendPlrMsg(int pnum, const char *szMsg);
void __cdecl ClearPlrMsg();
void __cdecl InitPlrMsg();
void __cdecl DrawPlrMsg();
void __fastcall PrintPlrMsg(int no, int x, int y, char *str, int just);
void __cdecl InitPortals();
void __fastcall SetPortalStats(int i, int o, int x, int y, int lvl, int lvltype);
void __fastcall AddWarpMissile(int i, int x, int y);
void __cdecl SyncPortals();
void __fastcall AddInTownPortal(int i);
void __fastcall ActivatePortal(int i, int x, int y, int lvl, int lvltype, int sp);
void __fastcall DeactivatePortal(int i);
int __fastcall PortalOnLevel(int i);
void __fastcall RemovePortalMissile(int id);
void __fastcall SetCurrentPortal(int p);
void __cdecl GetPortalLevel();
void __cdecl GetPortalLvlPos();
int __fastcall portal_pos_ok(int level, int x, int y);
void __cdecl InitQuests();
void __cdecl CheckQuests();
int __cdecl ForceQuests();
bool __fastcall QuestStatus(int i);
void __fastcall CheckQuestKill(int m, unsigned char sendmsg);
void __cdecl DrawButcher();
void __fastcall DrawSkelKing(int quest_id, int xx, int yy);
void __fastcall DrawWarLord(int xx, int yy);
void __fastcall DrawSChamber(int quest_id, int xx, int yy);
void __fastcall DrawLTBanner(int xx, int yy);
void __fastcall DrawBlind(int xx, int yy);
void __fastcall DrawBlood(int xx, int yy);
void __fastcall DRLG_CheckQuests(int xx, int yy);
void __cdecl SetReturnLvlPos();
void __cdecl GetReturnLvlPos();
void __cdecl ResyncMPQuests();
void __cdecl ResyncQuests();
void __fastcall PrintQLString(int x, int y, unsigned char cjustflag, char *str, int col);
void __cdecl DrawQuestLog();
void __cdecl StartQuestlog();
void __cdecl QuestlogUp();
void __cdecl QuestlogDown();
void __cdecl QuestlogEnter();
void __cdecl QuestlogESC();
void __fastcall SetMultiQuest(int q, int s, unsigned char l, int v1);
bool __cdecl SystemSupported();
bool __cdecl RestrictedTest();
bool __cdecl ReadOnlyTest();
void __cdecl scrollrt_cpp_init();
void __cdecl ClearCursor();
void __fastcall scrollrt_452994(int x, int y, int a3, int a4, int a5, int a6, int del_flag);
void __fastcall scrollrt_452B2A(int x, int y, int sx, int sy, int a5, int a6, int a7);
void __fastcall scrollrt_452CC0(int x, int y, int sx, int sy, int a5, int a6, int some_flag);
void __fastcall scrollrt_draw_player_a(int pnum, int x, int y, int px, int py, int animdata, int animframe, int animwidth, int a9, int a10);
void __fastcall scrollrt_draw_player_b(int pnum, int x, int y, int px, int py, int animdata, int animframe, int animwidth, int a9, int a10);
void __fastcall DrawView(int StartX, int StartY);
void __fastcall DrawGame(int x, int y);
void __fastcall scrollrt_levels(int x, int y, int sx, int sy, int a5, int some_flag);
void __fastcall scrollrt_dead(char *a1, int sx, int sy, int a4, int a5, int a6);
void __fastcall scrollrt_monsters(int x, int y, int a3, int a4, int mon_id, int a6, int a7);
void __fastcall scrollrt_objects(int x, int y, int a3, int a4, int pre_flag, int a6, int dir);
void __fastcall scrollrt_4540E5(char *buffer, int x, int y, int a4, int a5);
void __fastcall scrollrt_454229(int x, int y, int sx, int sy, int a5, int a6, int some_flag);
void __fastcall scrollrt_4545D2(char *buffer, int x, int y, int a4, int a5, int sx, int sy, int me_flag);
int __fastcall scrollrt_454C09(char *buffer, int x, int y, int a4, signed int a5, int sx, int sy);
void __fastcall scrollrt_454D9D(int x, int y, int sx, int sy, int a5, int a6, int some_flag);
void __fastcall scrollrt_dead2(char *buffer, int x, int y, int a4, int a5, int sx, int sy, int me_flag);
void __fastcall scrollrt_monsters2(int x, int y, int a3, int a4, int mon_id, int a6, int a7);
void __fastcall scrollrt_objects2(int x, int y, int a3, int a4, int pre_flag, int a6, int dir);
void __fastcall scrollrt_455A7D(char *buffer, int x, int y, int a4, int a5, int sx, int sy);
void __fastcall DrawZoom(int x, int y);
void __cdecl ClearScreenBuffer();
void __fastcall scrollrt_455E65(int some_flag);
void __cdecl scrollrt_455EC7();
void __cdecl scrollrt_455F56();
void __fastcall scrollrt_main_stuff(int dwHgt, int some_flag, int draw_hp, int draw_mana, int draw_sbar, int draw_btn);
void __fastcall scrollrt_draw_item(int dwX, int dwY, int dwWdt, int dwHgt);
void __cdecl DrawAndBlit();
int __fastcall ObjIndex(int x, int y);
void __cdecl AddSKingObjs();
void __cdecl AddSChamObjs();
void __cdecl AddVileObjs();
void __fastcall DRLG_SetMapTrans(char *dun_path);
void __cdecl LoadSetMap();
void __cdecl sha1_reset();
void __fastcall sha1_final(int ctx_id, char (*dst)[20]);
void __fastcall sha1(int ctx_id, const char *data, char (*dst)[20]);
void __fastcall sha1_update(SHA1Context *ctx, const char *data, int len);
void __fastcall sha1_transform(SHA1Context *ctx);
void __fastcall sha1_init(int ctx_id);
void __cdecl sound_cpp_init();
void __fastcall snd_update(bool bStopAll);
void __fastcall snd_stop_snd(TSnd *pSnd);
bool __fastcall snd_playing(TSnd *pSnd);
void __fastcall snd_play_snd(TSnd *pSnd, int lVolume, int lPan);
IDirectSoundBuffer *__fastcall sound_dup_channel(IDirectSoundBuffer *DSB);
bool __fastcall sound_file_reload(TSnd *sound_file, IDirectSoundBuffer *DSB);
TSnd *__fastcall sound_file_load(char *path);
void __fastcall sound_CreateSoundBuffer(TSnd *sound_file);
void __fastcall sound_file_cleanup(TSnd *sound_file);
void __fastcall snd_init(HWND hWnd);
void __fastcall sound_load_volume(char *value_name, int *value);
void __fastcall sound_create_primary_buffer(int music_track);
int __fastcall sound_DirectSoundCreate(GUID *guid, IDirectSound **DS, int always_null);
void __cdecl sound_cleanup();
void __fastcall sound_store_volume(char *key, int value);
void __cdecl music_stop();
void __fastcall music_start(int nTrack);
void __fastcall sound_disable_music(bool disable);
int __fastcall sound_get_or_set_music_volume(int volume);
int __fastcall sound_get_or_set_sound_volume(int volume);
int __fastcall GetManaAmount(int id, int sn);
void __fastcall UseMana(int id, int sn);
bool __fastcall CheckSpell(int player_num, int spell_id, int spell_type, bool mana_only);
void __fastcall CastSpell(int player_num, int spell_id, int x, int y, int target_x, int target_y, int target_num, int spell_lvl);
void __fastcall DoResurrect(int pnum, int rid);
void __fastcall PlacePlayer(int pnum);
void __fastcall DoHealOther(int pnum, int rid);
void __cdecl InitStores();
void __cdecl SetupTownStores();
void __cdecl FreeStoreMem();
void __cdecl DrawSTextBack();
void __fastcall PrintSString(int x, int y, unsigned char cjustflag, char *str, int col, int val);
void __fastcall DrawSLine(int y);
void __fastcall DrawSArrows(int a1, int a2);
void __cdecl DrawSTextHelp();
void __fastcall ClearSText(int s, int e);
void __fastcall AddSLine(int y);
void __fastcall AddSTextVal(int y, int val);
void __fastcall OffsetSTextY(int y, int yo);
void __fastcall AddSText(int x, int y, unsigned char j, char *str, int clr, int sel);
unsigned char __cdecl StoreAutoPlace();
void __cdecl S_StartSmith();
void __fastcall S_ScrollSBuy(int idx);
void __fastcall PrintStoreItem(ItemStruct *x, int l, char iclr);
void __cdecl S_StartSBuy();
void __fastcall S_ScrollSPBuy(int idx);
bool __cdecl S_StartSPBuy();
bool __fastcall SmithSellOk(int inv_num);
void __fastcall S_ScrollSSell(int idx);
void __cdecl S_StartSSell();
bool __fastcall SmithRepairOk(int i);
void __cdecl S_StartSRepair();
void __fastcall AddStoreHoldRepair(ItemStruct *itm, int i);
void __cdecl S_StartWitch();
void __fastcall S_ScrollWBuy(int idx);
void __cdecl S_StartWBuy();
bool __fastcall WitchSellOk(int i);
void __cdecl S_StartWSell();
unsigned char __fastcall WitchRechargeOk(int i);
void __fastcall AddStoreHoldRecharge(ItemStruct itm, int i);
void __cdecl S_StartWRecharge();
void __cdecl S_StartNoMoney();
void __cdecl S_StartNoRoom();
void __cdecl S_StartConfirm();
void __cdecl S_StartBoy();
void __cdecl S_StartBBoy();
void __cdecl S_StartHealer();
void __fastcall S_ScrollHBuy(int idx);
void __cdecl S_StartHBuy();
void __cdecl S_StartStory();
bool __fastcall IdItemOk(ItemStruct *item);
void __fastcall AddStoreHoldId(ItemStruct itm, int i);
void __cdecl S_StartSIdentify();
void __cdecl S_StartIdShow();
void __cdecl S_StartTalk();
void __cdecl S_StartTavern();
void __cdecl S_StartBarMaid();
void __cdecl S_StartDrunk();
void __fastcall StartStore(int talk_id);
void __cdecl DrawSText();
void __cdecl STextESC();
void __cdecl STextUp();
void __cdecl STextDown();
void __cdecl STextPrior();
void __cdecl STextNext();
void __cdecl S_SmithEnter();
void __fastcall SetGoldCurs(int pnum, int i);
void __fastcall SetSpdbarGoldCurs(int pnum, int i);
void __fastcall TakePlrsMoney(int cost);
void __cdecl SmithBuyItem();
void __cdecl S_SBuyEnter();
void __cdecl SmithBuyPItem();
void __cdecl S_SPBuyEnter();
unsigned char __fastcall StoreGoldFit(int idx);
void __fastcall PlaceStoreGold(int v);
void __cdecl StoreSellItem();
void __cdecl S_SSellEnter();
void __cdecl SmithRepairItem();
void __cdecl S_SRepairEnter();
void __cdecl S_WitchEnter();
void __cdecl WitchBuyItem();
void __cdecl S_WBuyEnter();
void __cdecl S_WSellEnter();
void __cdecl WitchRechargeItem();
void __cdecl S_WRechargeEnter();
void __cdecl S_BoyEnter();
void __cdecl BoyBuyItem();
void __cdecl HealerBuyItem();
void __cdecl S_BBuyEnter();
void __cdecl StoryIdItem();
void __cdecl S_ConfirmEnter();
void __cdecl S_HealerEnter();
void __cdecl S_HBuyEnter();
void __cdecl S_StoryEnter();
void __cdecl S_SIDEnter();
void __cdecl S_TalkEnter();
void __cdecl S_TavernEnter();
void __cdecl S_BarmaidEnter();
void __cdecl S_DrunkEnter();
void __cdecl STextEnter();
void __cdecl CheckStoreBtn();
void __cdecl ReleaseStoreBtn();
int __fastcall sync_all_monsters(TSyncHeader *packet, int size);
void __cdecl sync_one_monster();
int __fastcall sync_monster_active(TSyncMonster *packet);
int __fastcall sync_monster_pos(TSyncMonster *packet, int mon_id);
int __fastcall sync_monster_active2(TSyncMonster *packet);
char __fastcall SyncPlrInv(TSyncHeader *pItem);
int __fastcall SyncData(int pnum, TSyncHeader *packet);
void __fastcall sync_monster_data(int pnum, TSyncMonster *packet);
void __cdecl sync_clear_pkt();
unsigned char __fastcall TFit_Shrine(int i);
bool __fastcall TFit_Obj5(int theme_num);
unsigned char __fastcall TFit_SkelRoom(int i);
unsigned char __fastcall TFit_GoatShrine(int i);
unsigned char __fastcall CheckThemeObj3(int xp, int yp, int t, int f);
bool __fastcall TFit_Obj3(int theme_num);
unsigned char __fastcall CheckThemeReqs(int t);
unsigned char __fastcall SpecialThemeFit(int i, int t);
unsigned char __fastcall CheckThemeRoom(int tv);
void __cdecl InitThemes();
void __cdecl HoldThemeRooms();
void __fastcall PlaceThemeMonsts(int t, int f);
void __fastcall Theme_Barrel(int theme_num);
void __fastcall Theme_Shrine(int theme_num);
void __fastcall Theme_MonstPit(int theme_num);
void __fastcall Theme_SkelRoom(int theme_num);
void __fastcall Theme_Treasure(int theme_num);
void __fastcall Theme_Library(int theme_num);
void __fastcall Theme_Torture(int theme_num);
void __fastcall Theme_BloodFountain(int theme_num);
void __fastcall Theme_Decap(int theme_num);
void __fastcall Theme_PurifyingFountain(int theme_num);
void __fastcall Theme_ArmorStand(int theme_num);
void __fastcall Theme_GoatShrine(int theme_num);
void __fastcall Theme_Cauldron(int theme_num);
void __fastcall Theme_MurkyFountain(int theme_num);
void __fastcall Theme_TearFountain(int theme_num);
void __fastcall Theme_BrnCross(int theme_num);
void __fastcall Theme_WeaponRack(int theme_num);
void __cdecl UpdateL4Trans();
void __cdecl CreateThemeRooms();
int __fastcall tmsg_get(char *data, int size);
void __fastcall tmsg_add(char *msg, char len);
void __cdecl tmsg_cleanup();
int __fastcall town_clear_upper_buf(int a1);
void __fastcall town_clear_low_buf(int y_related);
void __fastcall town_draw_low_somepiece(void *buffer, int x, int y, int sx, int sy);
void __fastcall town_draw_low_something(void *unused, int x, int y, int sx, int sy, int some_flag);
void __fastcall town_draw_low_pieces(int x, int y, int sx, int sy, int a5, int some_flag);
void __fastcall town_draw_lower_screen(void *buffer, int x, int y, int a4, int a5, int sx, int sy);
void __fastcall town_draw_low_towners(int x, int y, int a3, int a4, int a5, int sx, int sy, int some_flag);
void __fastcall town_draw_lower(int x, int y, int sx, int sy, int a5, int a6, int some_flag);
void __fastcall town_draw_pieces(void *buffer, int x, int y, int a4, int dir, int sx, int sy);
void __fastcall town_draw_towners(void *buffer, int x, int y, int a4, int dir, int sx, int sy, int some_flag);
void __fastcall town_draw_screen(int x, int y, int sx, int sy, int a5, int a6, int some_flag);
void __fastcall T_DrawGame(int x, int y);
void __fastcall T_DrawZoom(int x, int y);
void __fastcall T_DrawView(int StartX, int StartY);
void __cdecl town_get_draw_size();
void __fastcall T_FillSector(unsigned char *P3Tiles, unsigned char *pSector, int xi, int yi, int w, int h, int AddSec);
void __fastcall T_FillTile(unsigned char *P3Tiles, int xx, int yy, int t);
void __cdecl T_Pass3();
void __fastcall CreateTown(int entry);
int __fastcall GetActiveTowner(int towner_id);
void __fastcall SetTownerGPtrs(void *towner_cel, void **cel_from_direction);
void __fastcall NewTownerAnim(int tnum, void *pAnim, int numFrames, int Delay);
void __fastcall InitTownerInfo(int i, int w, bool sel, int t, int x, int y, int ao, int tp);
void __fastcall InitQstSnds(int towner_num);
void __cdecl InitSmith();
void __cdecl InitBarOwner();
void __cdecl InitTownDead();
void __cdecl InitWitch();
void __cdecl InitBarmaid();
void __cdecl InitBoy();
void __cdecl InitHealer();
void __cdecl InitTeller();
void __cdecl InitDrunk();
void __cdecl InitCows();
void __cdecl InitTowners();
void __cdecl FreeTownerGFX();
void __fastcall TownCtrlMsg(int towner_num);
void __cdecl TownBlackSmith();
void __cdecl TownBarOwner();
void __cdecl TownDead();
void __cdecl TownHealer();
void __cdecl TownStory();
void __cdecl TownDrunk();
void __cdecl TownBoy();
void __cdecl TownWitch();
void __cdecl TownBarMaid();
void __cdecl TownCow();
void __cdecl ProcessTowners();
ItemStruct *__fastcall PlrHasItem(int pnum, int item, int *i);
void __fastcall TownerTalk(int t);
void __fastcall TalkToTowner(int p, int t);
void __fastcall CowSFX(int pnum);
void __cdecl track_cpp_init();
void __cdecl track_repeat_walk();
void __fastcall track_mouse_stance(int a1);
int __cdecl track_isscrolling();
void __cdecl InitNoTriggers();
void __cdecl InitTownTriggers();
void __cdecl InitL1Triggers();
void __cdecl InitL2Triggers();
void __cdecl InitL3Triggers();
void __cdecl InitL4Triggers();
void __cdecl InitSKingTriggers();
void __cdecl InitSChambTriggers();
void __cdecl InitPWaterTriggers();
void __cdecl InitVPTriggers();
unsigned char __cdecl ForceTownTrig();
unsigned char __cdecl ForceL1Trig();
unsigned char __cdecl ForceL2Trig();
unsigned char __cdecl ForceL3Trig();
unsigned char __cdecl ForceL4Trig();
void __cdecl Freeupstairs();
unsigned char __cdecl ForceSKingTrig();
unsigned char __cdecl ForceSChambTrig();
unsigned char __cdecl ForcePWaterTrig();
void __cdecl CheckTrigForce();
void __cdecl CheckTriggers();
void __cdecl wave_cpp_init();
bool __fastcall wave_close_file(void *file);
int __fastcall wave_get_file_size(int *a1, int *a2);
void __fastcall wave_get_file_archive(int a1, int *a2, char *dwInitParam);
int __fastcall wave_open_file(LPARAM dwInitParam, DIABFILE *a2, int a3);
char __fastcall wave_read_file(int a1, char *a2, int a3);
int __fastcall wave_file_pointer(int file1, int offset, int file2, int whence);
int __fastcall wave_do_buffer(int a1, TSnd *a2);
void *__fastcall wave_alloc_buffer(int a1, DIABFILE *a2, unsigned int a3);
void __fastcall wave_free_buffer(int *a1);
int __fastcall wave_read_buffer(DIABFILE *wave_file, TSnd *a2, int *a3);
int __fastcall wave_seek_section(DIABFILE *a1, void *a2, size_t a3);
char __fastcall wave_read_section(DIABFILE *a1);
int __fastcall wave_seek_position(DIABFILE *a1, unsigned int a2, int a3);
int __fastcall wave_get_section_data(DIABFILE *a1, int a2, int *a3);
int __fastcall wave_load_file(int a1, TSnd *a2, int a3);
void __fastcall j_engine_mem_free(void *ptr);
void __fastcall drawTopArchesUpperScreen(void *a1);
void __fastcall drawBottomArchesUpperScreen(void *a1, int a2);
void __fastcall drawUpperScreen(void *a1);
void __fastcall drawTopArchesLowerScreen(void *a1);
void __fastcall drawBottomArchesLowerScreen(void *a1, int a2);
void __fastcall drawLowerScreen(void *a1);
void __fastcall world_levelrelated(char *a1);

int appfat_terminated = 0; // weak
unsigned char error_buf[256];
int terminating; // weak
int cleanup_thread_id; // weak
char empty_string;
short automaptype[512];
int AMdword_4B7E40; // weak
int AMdword_4B7E44; // weak
unsigned char automapflag; // idb
char AMbyte_4B7E4C[32];
char automapview[40][40];
int AutoMapScale; // idb
int AutoMapXOfs; // weak
int AutoMapYOfs; // weak
int CT_4B84B8; // weak
int CT_4B84BC; // weak
int CT_4B84C0; // weak
int CT_4B84C4; // weak
int CT_4B84C8; // weak
char sgbNextTalkSave; // weak
char sgbTalkSavePos; // weak
void *pDurIcons;
void *pChrButtons;
int drawhpflag; // idb
int dropGoldFlag; // weak
int panbtn[8];
int chrbtn[4];
void *pMultiBtns;
void *pPanelButtons;
void *pChrPanel;
int lvlbtndown; // weak
char sgszTalkSave[8][80];
int dropGoldValue; // idb
int drawmanaflag; // idb
int chrbtnactive; // weak
char sgszTalkMsg[80];
void *pPanelText;
int frame_4B8800; // idb
void *pLifeBuff;
void *pBtmBuff;
void *pTalkBtns;
int pstrjust[4];
int pnumlines; // idb
int pinfoflag; // weak
int talkbtndown[3];
int pSpell; // weak
void *pManaBuff;
int infoclr; // weak
int sgbPlrTalkTbl; // weak
void *pGBoxBuff;
void *pSBkBtnCel;
char tempstr[260];
int sbooktab; // weak
int pSplType; // weak
int frame; // idb
int initialDropGoldIndex; // idb
int talkflag; // weak
void *pSBkIconCels;
int sbookflag; // weak
int chrflag;
int drawbtnflag; // idb
void *pSpellBkCel;
char infostr[260];
int numpanbtns; // weak
void *pStatusPanel;
char panelstr[256];
int panelflag; // weak
char byte_4B8B88[256];
int initialDropGoldValue; // idb
void *pSpellCels;
int panbtndown; // weak
void *pTalkPanel; // idb
int spselflag; // weak
int cursH; // weak
int icursH28; // idb
int cursW; // idb
unsigned short pcursmonst; // idb
int icursW28; // idb
void *pCursCels;
int icursH; // weak
char pcursinvitem; // weak
int icursW; // weak
char pcursitem; // weak
char pcursobj; // weak
char pcursplr; // weak
int cursmx;
int cursmy;
int dword_4B8CCC; // weak
int pcurs; // idb
int spurtndx; // weak
DeadStruct dead[31];
int stonendx;
void *pSquareCel;
char debug_dMonster[17][112][112];
char debug_dflags[17][112][112];
int diablo_cpp_init_value; // weak
HWND ghMainWnd;
int glMid1Seed[17];
int glMid2Seed[17];
int gnLevelTypeTbl[17];
int MouseY; // idb
int MouseX; // idb
bool gbGameLoopStartup; // idb
int glSeedTbl[17];
int gbRunGame; // weak
int glMid3Seed[17];
int gbRunGameResult; // weak
int zoomflag; // weak
int gbProcessPlayers; // weak
int glEndSeed[17];
int dword_5256E8; // weak
HINSTANCE hInstance; // idb
int DebugMonsters[10];
char cineflag; // weak
int force_redraw; // weak
int visiondebug; // weak
int light4flag; // weak
int leveldebug; // weak
int monstdebug; // weak
int setseed; // weak
int debugmonsttypes; // weak
int PauseMode; // weak
int sgnTimeoutCurs;
char sgbMouseDown; // weak
int dword_52574C; // weak
int dword_525750; // weak
int dword_525754; // weak
void *pDoomCel;
int doomflag; // weak
int dword_525760; // idb
char L5dungeon[80][80];
char mydflags[40][40];
int setloadflag; // weak
int HR1;
int HR2;
int HR3;
int VR1;
int VR2;
int VR3;
void *pSetPiece; // idb
int nSx1;
int nSx2; // weak
int nSy1;
int nSy2; // weak
int nRoomCnt;
char predungeon[40][40];
ROOMNODE RoomList[81];
HALLNODE *pHallList;
char lavapool; // weak
int abyssx; // weak
int lockoutcnt; // weak
char lockout[40][40];
int diabquad1x; // weak
int diabquad1y; // weak
int diabquad3x; // idb
int diabquad3y; // idb
int diabquad2x; // idb
int diabquad2y; // idb
int diabquad4x; // idb
int diabquad4y; // idb
int hallok[20];
int l4holdx; // weak
int l4holdy; // weak
int SP4x1; // idb
int SP4x2; // weak
int SP4y1; // idb
int SP4y2; // weak
char L4dungeon[80][80];
char dung[20][20];
int dword_52A4DC; // weak
int dthread_cpp_init_value; // weak
struct _RTL_CRITICAL_SECTION CriticalSection; // idb
unsigned int ThreadId; // idb
int sgpInfoHead;
char byte_52A508; // weak
HANDLE sghWorkToDoEvent; // idb
void *sgpBackBuf;
int dx_cpp_init_value; // weak
IDirectDrawPalette *lpDDPPrimary;
IDirectDrawPalette *lpDDPRed; // idb
int sgdwLockCount;
Screen *gpBuffer;
IDirectDrawSurface *lpDDSBackBuf;
IDirectDrawSurface *lpDDSPrimary;
struct _RTL_CRITICAL_SECTION stru_52A530;
char gbBackBuf; // weak
char gbEmulate; // weak
HMODULE hModule; // idb
int effects_cpp_init_value; // weak
int sfxdelay; // weak
int sfxdnum;
void *sfx_stream;
TSFX *sfx_data_cur;
int encrypt_52A564[1024];
int encrypt_52B564[257];
int engine_cpp_init_value; // weak
char byte_52B96C; // weak
int dword_52B970; // weak
int orgseed; // weak
int dword_52B978; // weak
int sglGameSeed; // weak
struct _RTL_CRITICAL_SECTION sgMemCrit;
int SeedCount; // weak
int dword_52B99C; // weak
char msgtable[80];
char msgdelay; // weak
char msgflag; // weak
char msgcnt; // weak
LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter; // idb
short level_frame_types[2048];
int themeCount;
char nTransTable[2048];
int dword_52D204;
int dMonster[112][112];
char dungeon[40][40];
char dObject[112][112];
void *pSpeedCels;
int nlevel_frames; // weak
char pdungeon[40][40];
char dDead[112][112];
short dpiece_defs_map_1[16][112][112];
char dTransVal2[112][112];
char TransVal; // weak
int dword_5A5594;
char dflags[40][40];
int dPiece[112][112];
char dTransVal[112][112];
int setloadflag_2; // weak
Tile tile_defs[1024];
void *pMegaTiles;
DPiece dpiece_defs[2];
int gnDifficulty; // idb
char block_lvid[2049];
char byte_5B78EB;
char dung_map[112][112];
char nTrapTable[2049];
char leveltype; // weak
unsigned char currlevel; // idb
char TransList[256];
char nSolidTable[2049];
int level_frame_count[2049];
ScrollStruct ScrollInfo;
void *pDungeonCels;
int speed_cel_frame_num_from_light_index_frame_num[16][128];
THEME_LOC themeLoc[50];
char dPlayer[112][112];
int dword_5C2FF8; // weak
int dword_5C2FFC; // weak
int scr_pix_width; // weak
int scr_pix_height; // weak
char dArch[112][112];
char nBlockTable[2049];
void *level_special_cel;
char dFlags[112][112];
char dItem[112][112];
char setlvlnum; // weak
int level_frame_sizes[2048];
char nMissileTable[2049];
char *pSetPiece_2;
char setlvltype; // weak
char setlevel; // weak
int LvlViewY; // weak
int LvlViewX; // weak
int dmaxx; // weak
int dmaxy; // weak
int setpc_h; // weak
int setpc_w; // weak
int setpc_x; // idb
int ViewX; // idb
int ViewY; // idb
int setpc_y; // idb
char dMissile[112][112];
int dminx; // weak
int dminy; // weak
short dpiece_defs_map_2[16][112][112];
void *optbar_cel;
char byte_634464; // weak
void *PentSpin_cel;
void **sgpCurrItem;
void *BigTGold_cel;
int dword_634474; // weak
char byte_634478; // weak
void (__cdecl *dword_63447C)();
int dword_634480; // idb
void *option_cel;
void *sgpLogo;
int dword_63448C; // weak
int help_select_line; // weak
int dword_634494; // weak
int helpflag;
int displayinghelp[22];
int HelpTop; // weak
_SNETVERSIONDATA fileinfo;
int init_cpp_init_value; // weak
int window_activated; // weak
char diablo_exe_path[260];
void *unused_mpq;
char patch_rt_mpq_path[260];
LRESULT (__stdcall *CurrentProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void *diabdat_mpq;
char diabdat_mpq_path[260];
void *patch_rt_mpq;
int killed_mom_parent; // weak
bool screensaver_enabled_prev;
void *sgpBackCel;
float interfac_cpp_init_value;
int sgdwProgress;
int progress_id; // idb
int invflag;
void *pInvCels;
int drawsbarflag; // idb
int dupe_delay;
int itemactive[127];
int uitemflag;
int itemavail[128];
ItemStruct curruitem;
ItemGetRecordStruct itemrecords[104];
ItemStruct item_stru_6358B8;
ItemStruct items[128];
char itemhold[3][3];
char byte_641234[28];
int Item2Frm[35];
int UniqueItemFlag[128];
int numitems;
int gnNumGetRecords;
LightListStruct VisionList[32];
char lightactive[32];
LightListStruct LightList[32];
int numlights;
char dung_map_radius[2048];
int dovision; // weak
int numvision;
char lightmax; // weak
int dword_642A18; // weak
char dung_map_rgba[16384];
int visionid;
int dword_646A20;
int lightflag; // weak
void *tbuff;
int dword_646A30; // weak
struct _RTL_CRITICAL_SECTION stru_646A38;
CHAR FileName[260]; // idb
char log_buffer[388];
LPCVOID lpAddress; // idb
DWORD nNumberOfBytesToWrite; // idb
int mainmenu_cpp_init_value; // weak
char chr_name_str[16];
int qtexty; // weak
char *qtextptr;
int qtextSpd; // weak
char qtextflag; // weak
int scrolltexty; // weak
int sgLastScroll; // weak
void *pMedTextCels;
void *pTextBoxCels;
int missileactive[125];
int missileavail[125];
MissileStruct missile[125];
int nummissiles; // idb
int ManashieldFlag;
unk_missile_struct misflagstruct_unknown[125];
int MissilePreFlag; // weak
int END_unkmis_126; // weak
int MissileFileFlag; // weak
int monster_cpp_init_value; // weak
int monstkills[200];
int monstactive[200];
int nummonsters;
int sgbSaveSoundOn; // weak
MonsterStruct monster[200];
int totalmonsters; // weak
CMonster Monsters[16];
int END_Monsters_17; // weak
int monstimgtot; // weak
int uniquetrans;
int nummtypes;
int movie_cpp_init_value; // weak
int movie_playing; // weak
int loop_movie; // weak
int mpqapi_cpp_init_value; // weak
LONG lDistanceToMove; // idb
int mpq_buf[1024];
_BLOCKENTRY *sgpHashTbl;
int dword_65AB0C; // weak
_BLOCKENTRY *sgpBlockTbl;
char byte_65AB14; // weak
int sgdwOwnerWait; // weak
int msg_cpp_init_value; // weak
int sgdwRecvOffset; // idb
int sgnCurrMegaPlayer; // weak
DLevel sgLevels[17];
char sbLastCmd; // weak
TMegaPkt *sgpCurrPkt;
DLevel sgTempLevel;
unsigned char sgbRecvCmd; // idb
LocalLevel sgLocals[17];
DJunk sgJunk[4];
TMegaPkt *sgpMegaPkt;
char sgbDeltaChanged; // weak
char sgbDeltaChunks; // weak
int deltaload; // weak
char gbBufferMsgs; // weak
int dword_676198; // weak
int msg_err_timer; // weak
int msgcmd_cpp_init_value; // weak
_SNETEVENT pEvent;
char gbSomebodyWonGameKludge; // weak
char pkdata_6761C0[4100];
char szPlayerDescript[128];
short sgwPackPlrOffsetTbl[4];
PkPlayerStruct pkplr[4];
char sgbPlayerTurnBitTbl[4];
char sgbPlayerLeftGameTbl[4];
int multi_cpp_init_value; // weak
int sgbSentThisCycle; // idb
int dword_678628; // weak
char gbActivePlayers; // weak
char gbGameDestroyed; // weak
char sgbSendDeltaTbl[4];
_gamedata sgGameInitInfo;
char byte_678640; // weak
int sglTimeoutStart; // weak
int sgdwPlayerLeftReasonTbl[4];
char pkdata_678658[4100];
unsigned int sgdwGameLoops; // idb
char gbMaxPlayers; // weak
char sgbTimeout; // weak
char szPlayerName[128];
char gbDeltaSender; // weak
int sgbNetInited; // weak
int player_state[4];
int nthread_cpp_init_value; // weak
char byte_679704; // weak
int gdwMsgLenTbl[4];
struct _RTL_CRITICAL_SECTION stru_679718;
int gdwDeltaBytesSec; // weak
char byte_679734; // weak
int gdwTurnsInTransit; // weak
int glpMsgTbl[4];
int dword_67974C;
char sgbSyncCountdown; // weak
int dword_679754; // weak
char byte_679758; // weak
char sgbPacketCountdown; // weak
char sgbThreadIsRunning; // weak
int gdwLargestMsgSize; // weak
int gdwNormalMsgSize; // weak
int dword_679764; // weak
int trapid; // weak
int trapdir; // weak
int pObjCels[40];
char ObjFileList[40];
int objectactive[127];
int nobjects; // idb
int leverid; // idb
int objectavail[127];
ObjectStruct object[127];
int InitObjFlag; // weak
int numobjfiles; // weak
int hero_cpp_init_value; // weak
PALETTEENTRY logical_palette[256];
int palette_cpp_init_value; // weak
PALETTEENTRY system_palette[256];
PALETTEENTRY orig_palette[256];
UINT nsystem_reserve_palette_entries;
PATHNODE path_nodes[300];
int dword_6820C8;
int pnode_vals[26];
PATHNODE *pnode_ptr;
PATHNODE *pnode_tblptr[300];
PATHNODE path_2_nodes[300];
int pfile_cpp_init_value;
char hero_names[320];
bool gbValidSaveFile; // idb
int save_prev_tc; // weak
int plr_lframe_size; // idb
int plr_wframe_size; // idb
char plr_gfx_flag; // weak
int player_cpp_init_value; // weak
int plr_aframe_size; // idb
int myplr;
PlayerStruct plr[4];
int plr_fframe_size; // idb
int plr_qframe_size; // idb
bool deathflag; // idb
int plr_hframe_size; // idb
int plr_bframe_size; // idb
char plr_gfx_bflag; // weak
int plr_sframe_size; // idb
int deathdelay; // weak
int plr_dframe_size; // idb
int plrmsg_ticks; // weak
char plr_msg_slot; // weak
_plrmsg plr_msgs[8];
PortalStruct portal[4];
int portalindex;
int END_portalstruct; // weak
int qtopline; // idb
int questlog; // weak
void *pQLogCel;
QuestStruct quests[16];
int qline; // weak
int qlist[16];
int numqlines; // weak
int WaterDone; // idb
int ReturnLvlY; // idb
int ReturnLvlX; // idb
int ReturnLvlT; // idb
int ALLQUESTS; // idb
int ReturnLvl; // idb
int light_table_index; // weak
int screen_y_times_768[1024];
int scrollrt_cpp_init_value; // weak
unsigned int sgdwCursWdtOld; // idb
int sgdwCursX; // idb
int sgdwCursY; // idb
int screen_buf_end; // weak
int sgdwCursHgt;
int level_cel_block; // weak
int sgdwCursXOld; // idb
int sgdwCursYOld; // idb
char arch_draw_type; // weak
DDSURFACEDESC DDS_desc;
int cel_transparency_active; // weak
int level_piece_id; // weak
int sgdwCursWdt;
int (__fastcall *draw_player)(int player_num, int x, int y, int screen_x, int screen_y, void *cl2_buf, int frame, int frame_width, int a9, int a10);
char cursor_draw_back_buffer[8192];
int draw_monster_num; // weak
int sgdwCursHgtOld; // idb
SHA1Context sha1_contexts[3];
float sound_cpp_init_value;
IDirectSoundBuffer *DSBs[8];
IDirectSound *sglpDS;
char gbSndInited;
int sglMusicVolume;
int sglSoundVolume;
HMODULE hDsound_dll; // idb
void *sgpMusicTrack;
int dword_69F100; // weak
int stextup; // weak
int storenumh; // weak
int stextlhold; // weak
ItemStruct boyitem;
int stextshold; // idb
ItemStruct premiumitem[6];
void *pSTextBoxCels;
int premiumlevel; // idb
int talker; // weak
STextStruct stext[24];
char stextsize; // weak
int stextsmax; // weak
int InStoreFlag; // idb
ItemStruct storehold[48];
int gossipstart; // weak
ItemStruct witchitem[20];
int stextscrl; // weak
int numpremium; // idb
ItemStruct healitem[20];
ItemStruct golditem;
char storehidx[48];
void *pSTextSlidCels;
int stextvhold; // weak
int stextsel; // weak
char stextscrldbtn; // weak
int gossipend; // weak
void *pCelBuff;
int stextsval; // idb
int boylevel; // weak
ItemStruct smithitem[20];
int stextdown; // weak
char stextscrlubtn; // weak
char stextflag; // weak
short sync_word_6AA708[200];
int dword_6AA898; // weak
short sync_word_6AA89C[200];
int dword_6AAA2C[2];
int sgnSyncPInv; // weak
int numthemes; // idb
int armorFlag; // weak
int ThemeGoodIn[4];
int weaponFlag; // weak
int treasureFlag; // weak
int mFountainFlag; // weak
int cauldronFlag; // weak
int tFountainFlag; // weak
int zharlib; // weak
int themex; // idb
int themey; // idb
int themeVar1; // idb
ThemeStruct themes[50];
int pFountainFlag; // weak
int bFountainFlag; // weak
int bCrossFlag; // weak
TMsg sgpTimedMsgHead;
int storeflag; // weak
int sgnCowMsg; // weak
int numtowners; // idb
int sgdwCowClicks; // weak
int unused_6AAC28; // weak
int boyloadflag; // weak
void *pCowCels; // idb
TownerStruct towner[16];
char byte_6ABAB8; // weak
int track_cpp_init_value; // weak
int dword_6ABAC0; // weak
int dword_6ABAC4; // weak
int trigflag[5];
TriggerStruct trigs[5];
int TWarpFrom; // weak
int wave_cpp_init_value; // weak
int dword_6ABB9C; // weak

