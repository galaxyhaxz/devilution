// some global definitions, found in debug release

#define DMAXX					40
#define DMAXY					40

#define LIGHTSIZE				6912 // 27 * 256

#define MAX_PLRS				4
#define MAX_CHARACTERS			10
#define MAX_LVLMTYPES			16
// #define MAX_PATH				260
#define MAX_SEND_STR_LEN		80

#define MAXDEAD					31
#define MAXDUNX					112
#define MAXDUNY					112
#define MAXITEMS				127
#define MAXLIGHTS				32
#define MAXMISSILES				125
#define MAXMONSTERS				200
#define MAXMULTIQUESTS			4
#define MAXOBJECTS				127
#define MAXPORTAL				4
#define MAXQUESTS				16
#define MAXTHEMES				50
#define MAXTILES				2048
#define MAXTRIGGERS				5
#define MAXVISION				32
#define MDMAXX					40
#define MDMAXY					40

// gmenu
#define MIN_SLIDER_TICKS		2
#define MAX_SLIDER_TICKS		0xFFF

// nthread
#define MIN_MSG_SIZE			128
//#define MAX_MSG_SIZE			512

// sound
#define VOLUME_MIN				-1600
#define VOLUME_MAX				0

// todo: enums
#define NUM_INVLOC				7
#define NUM_SFX					858
#define NUMLEVELS				17

// from diablo 2 beta
#define MAXEXP					2000000000

#define PLR_NAME_LEN			32

// 256 kilobytes + 3 bytes (demo leftover) for file magic (262147)
// final game uses 4-byte magic instead of 3
#define FILEBUFF				((256*1024)+3)

// Diablo uses a 256 color palette
// Entry 0-127 (0x00-0x7F) are level specific
// Entry 128-255 (0x80-0xFF) are global

// standard palette for all levels
// 8 or 16 shades per color
// example (dark blue): PAL16_BLUE+14, PAL8_BLUE+7
// example (light red): PAL16_RED+2, PAL8_RED
// example (orange): PAL16_ORANGE+8, PAL8_ORANGE+4
#define PAL8_BLUE		128
#define PAL8_RED		136
#define PAL8_YELLOW		144
#define PAL8_ORANGE		152
#define PAL16_BEIGE		160
#define PAL16_BLUE		176
#define PAL16_YELLOW	192
#define PAL16_ORANGE	208
#define PAL16_RED		224
#define PAL16_GRAY		240


#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

// If defined, use 32-bit colors instead of 8-bit [Default -> Undefined]
//#define RGBMODE

#ifndef RGBMODE
#define SCREEN_BPP		8
#else
#define SCREEN_BPP		32
#endif

#define BORDER_LEFT		64
#define BORDER_TOP		160
#define BORDER_RIGHT	64
#define BORDER_BOTTOM	16

#define SCREEN_X		BORDER_LEFT
#define SCREEN_Y		BORDER_TOP

#define BUFFER_WIDTH	(BORDER_LEFT + SCREEN_WIDTH + BORDER_RIGHT)
#define BUFFER_HEIGHT	(BORDER_TOP + SCREEN_HEIGHT + BORDER_BOTTOM)
#define TILE_SIZE		32

#define SCREENXY(x, y)	((x) + SCREEN_X + ((y) + SCREEN_Y) * BUFFER_WIDTH)

// debug
// #define DiabLoad(f, s, h) LoadFileInMem(f, s, h, __LINE__, __FILE__)
#define DiabLoad(f, s, h) LoadFileInMem(f, s)

#define MemFreeDbg(p)	\
{						\
	void *p__p;			\
	p__p = p;			\
	p = NULL;			\
	mem_free_dbg(p__p);	\
}

#undef assert

#ifndef _DEBUG
#define assert(exp) ((void)0)
#else
#define assert(exp) (void)( (exp) || (assert_fail(__LINE__, __FILE__, #exp), 0) )
#endif

/////////////////////////////////////////////////////////////////////////
/* temporary stuff from the decompiler */
/* remove all the garbage below in the future */
/////////////////////////////////////////////////////////////////////////
#ifndef IDA_GARBAGE
#define IDA_GARBAGE

__inline void memset32(void *s, unsigned int c, size_t n)
{
	int i;
	unsigned int *p = (unsigned int *)s;
	for (i = 0; i < n; i++) {
		p[i] = c;
	}
}

typedef          __int64 ll;
typedef unsigned __int64 ull;

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

typedef          char   int8;
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef          int    int32;
typedef   signed int    sint32;
typedef unsigned int    uint32;
typedef ll              int64;
typedef ll              sint64;
typedef ull             uint64;

// Partially defined types. They are used when the decompiler does not know
// anything about the type except its size.
#define _BYTE  uint8
#define _WORD  uint16
#define _DWORD uint32
#define _QWORD uint64

// Some convenience macros to make partial accesses nicer
#define LAST_IND(x,part_type)    (sizeof(x)/sizeof(part_type) - 1)
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
#  define LOW_IND(x,part_type)   LAST_IND(x,part_type)
#  define HIGH_IND(x,part_type)  0
#else
#  define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
#  define LOW_IND(x,part_type)   0
#endif
// first unsigned macros:
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define DWORDn(x, n)  (*((_DWORD*)&(x)+n))

#define _LOBYTE(x)  BYTEn(x,LOW_IND(x,_BYTE))
#define _LOWORD(x)  WORDn(x,LOW_IND(x,_WORD))
#define LODWORD(x) DWORDn(x,LOW_IND(x,_DWORD))
#define _HIBYTE(x)  BYTEn(x,HIGH_IND(x,_BYTE))
#define _HIWORD(x)  WORDn(x,HIGH_IND(x,_WORD))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,_DWORD))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)


// now signed macros (the same but with sign extension)
#define SBYTEn(x, n)   (*((int8*)&(x)+n))
#define SWORDn(x, n)   (*((int16*)&(x)+n))

#define SLOBYTE(x)  SBYTEn(x,LOW_IND(x,int8))
#define SHIWORD(x)  SWORDn(x,HIGH_IND(x,int16))



// Helper functions to represent some assembly instructions.

#ifdef __cplusplus

__inline void *qmemcpy(void *dst, const void *src, size_t cnt)
{
  char *out = (char *)dst;
  const char *in = (const char *)src;
  while ( cnt > 0 )
  {
    *out++ = *in++;
    --cnt;
  }
  return dst;
}

// sign flag
template<class T> int8 __SETS__(T x)
{
  if ( sizeof(T) == 1 )
    return int8(x) < 0;
  if ( sizeof(T) == 2 )
    return int16(x) < 0;
  if ( sizeof(T) == 4 )
    return int32(x) < 0;
  return int64(x) < 0;
}

// overflow flag of subtraction (x-y)
template<class T, class U> int8 __OFSUB__(T x, U y)
{
  if ( sizeof(T) < sizeof(U) )
  {
    U x2 = x;
    int8 sx = __SETS__(x2);
    return (sx ^ __SETS__(y)) & (sx ^ __SETS__(x2-y));
  }
  else
  {
    T y2 = y;
    int8 sx = __SETS__(x);
    return (sx ^ __SETS__(y2)) & (sx ^ __SETS__(x-y2));
  }
}

#endif

#endif /* IDA_GARBAGE */

// Typedef for the function pointer
typedef void (*_PVFV)(void);

#if defined(_MSC_VER) && !defined(__APPLE__)
// Define our segment names
#define SEGMENT_C_INIT ".CRT$XCU"

// Build our various function tables and insert them into the correct segments.
#pragma data_seg(SEGMENT_C_INIT)
#pragma data_seg() // Switch back to the default segment
// Call function pointer arrays and place them in the segments created above
#define SEG_ALLOCATE(SEGMENT) __declspec(allocate(SEGMENT))
#else
#define SEG_ALLOCATE(SEGMENT)
#endif
