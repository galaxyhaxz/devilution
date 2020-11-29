// temporary file

#ifndef _TYPES_H
#define _TYPES_H

#define WIN32_LEAN_AND_MEAN

#include "resource.h"

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <ddraw.h>
#include <dsound.h>
#include <io.h>
#include <math.h>
#include <time.h>
#include <process.h>
#include <shlobj.h>
#include <shellapi.h>

#ifdef __GNUC__
#include <ctype.h>
#endif

typedef unsigned char _bool;

// tell Visual C++ to shut the hell up
#ifdef _MSC_VER
#pragma warning (disable : 4309) // truncation of constant value
#pragma warning (disable : 4305) // truncation of int
#pragma warning (disable : 4018) // signed/unsigned mismatch
#pragma warning (disable : 4700) // used without having been initialized
#pragma warning (disable : 4804) // unsafe use of type 'bool' in operation
#pragma warning (disable : 4805) // unsafe bool mix
#pragma warning (disable : 4244) // conversion loss
#pragma warning (disable : 4800) // bool perf
#pragma warning (disable : 4146) // negative unsigned
#endif

#include "defs.h"
#include "enums.h"
#include "structs.h"

#if (_MSC_VER >= 800) && (_MSC_VER <= 1200)
#define USE_ASM
#endif

// If defined, use copy protection [Default -> Defined]
//#define COPYPROT

// If defined, don't reload for debuggers [Default -> Undefined]
// Note that with patch 1.03 the command line was hosed, this is required to pass arguments to the game
#ifdef _DEBUG
#define DEBUGGER
#endif

// If defined, don't fry the CPU [Default -> Undefined]
#ifdef _DEBUG
#define SLEEPFIX
#endif

// If defined, fix palette glitch in Windows Vista+ [Default -> Undefined]
//#define COLORFIX

#endif
