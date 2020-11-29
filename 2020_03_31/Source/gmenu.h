//HEADER_GOES_HERE
#ifndef __GMENU_H__
#define __GMENU_H__

extern void *optbar_cel;
extern BOOLEAN mouse_in_slider; // weak
extern void *PentSpin_cel;
extern TMenuItem *sgpCurrItem;
extern void *BigTGold_cel;
extern int menu_draw_ticks; // weak
extern char menu_spinner_frame; // weak
extern void (*sgpUpdateFunc)(TMenuItem *);
extern TMenuItem *sgpMenuItems; // idb
extern void *option_cel;
extern void *sgpLogo;
extern int num_menu_items; // weak

void gmenu_draw_pause();
void gmenu_print_text(int x, int y, char *pszStr);
void FreeGMenu();
void gmenu_init_menu();
BOOL gmenu_is_active();
void gmenu_set_items(TMenuItem *pMenuItems, void (*fnUpdate)(TMenuItem *));
void gmenu_up_down(BOOL down);
void gmenu_draw();
void gmenu_draw_menu_item(TMenuItem *pItem, int y);
void gmenu_clear_buffer(int x, int y, int width, int height);
int gmenu_get_lfont(TMenuItem *pItem);
BOOL gmenu_presskeys(int vkey);
void gmenu_left_right(BOOL right);
BOOL gmenu_on_mouse_move();
BOOLEAN gmenu_get_mouse_slider(long *plOffset);
BOOL gmenu_left_mouse(BOOL down);
void gmenu_enable(TMenuItem *pMenuItem, BOOL enable);
void gmenu_set_slider_ticks(TMenuItem *pItem, int min, int max, int ticks);
int gmenu_get_slider_ticks(TMenuItem *pItem, int min, int max);
void gmenu_set_max_ticks(TMenuItem *pItem, DWORD dwTicks);

/* rdata */

extern const unsigned char lfontframe[127];
extern const unsigned char lfontkern[56];

#endif /* __GMENU_H__ */
