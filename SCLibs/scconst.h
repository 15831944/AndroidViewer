#ifndef SCCONST
#define SCCONST

#if _MSC_VER > 1000
#	pragma once
#endif

// Carlson Grade only wants DLLs for space constraints so ignore STATIC_LINK or things just
// won't export properly.
// Added DriverAPI check too, since it was having export problems due to this logic.
#if !defined (TARGET_CARLSON_GRADE) && !defined (TARGET_DRIVER_API)
// this is a define telling shared projects to compile as with no import/export stuff for linking into office DLLs, 
// otherwise they are compiled as separate DLLs - so read it as STATICALLY LINKED INTO ONE DLL
#	ifndef STATIC_LINK
#		if defined(_DLL) || defined(STANDALONE)
#				define STATIC_LINK
#		endif
#	endif
#endif

#if defined(ICAD7) && !defined(ICAD)
  #define ICAD // ICAD7 defined, but ICAD not
#endif

#if ((defined ACRXAPP) || (defined ICAD))
# ifndef HAVE_CAD
#   define HAVE_CAD
# endif
#endif

#ifdef ACAD13
#ifndef ACAD12
#define ACAD12
#endif
#ifndef ACAD11
#define ACAD11
#endif
#ifndef ACAD10
#define ACAD10
#endif
#endif

#ifdef ACAD10
#ifndef ACAD09
#define ACAD09
#endif
#ifndef ACAD08
#define ACAD08
#endif
#ifndef ACAD07
#define ACAD07
#endif
#endif

#ifdef ACAD07
#ifndef ACAD06
#define ACAD06
#endif
#ifndef ACAD05
#define ACAD05
#endif
#ifndef ACAD04
#define ACAD04
#endif
#endif

#ifdef ACAD04
#ifndef TAHOE
#define TAHOE
#endif
#ifndef ARX20
#define ARX20
#endif
#endif

#if defined(WIN32) && !defined(_CONSOLE)
  #define WIN32_WINDOWS
#endif

#define FILENMSZ MAX_PATH     //FILENAME_MAX, from stdio.h, isn't supported on WinCE, says Romeo. Includes terminating NULL.
#define LAYERNMSZ 140         //maximum layer name length including terminating NULL; make_polyline() in crdutil.cpp was using 140.
#define HANDLESZ  17          //maximum Acad handle length including terminating NULL.
                              //(# came from an example in AutoCAD's "Entity Name Functions" in documentation)
#define TEXTSTYLESZ 256       //From the AutoCAD documentation, text style names can be 255 chars plus 1 for term. NULL.

#ifndef SCLSQ
  #ifndef PI
    #define PI 3.1415926535897932384626433832795
  #endif
#endif

#define UNEST -98765432.1
#define SMALL 0.000001
// if <math.h> is included after defining HUGE here it causes a 
// conflict and will not compile.
// We should not use standard library constants as constants in
// our software.  We should use SC_HUGE or something similar - but
// I will leave that for a Carlson developer to do.  Burke 4/06

#if ((!defined(UNDER_CE)) && (!defined(EMULATOR)))
  #define HUGE 98765432.1
  #define HUGE_L 98765432
#else
  #ifndef HUGE // Under CE we need this check
    #define HUGE 98765432.1
    #define HUGE_L 98765432
  #endif
#endif

#ifndef TRUE
  #define TRUE 1
  #define FALSE 0
  #define BOOL short
#endif

#ifndef UNUSED
  #define UNUSED(x)
#endif

#ifndef UNUSED_ARG
#	define UNUSED_ARG(x) x
#endif

#ifndef ABS
#	define ABS( x) ( (x) >= 0 ? (x) : -(x) )
#endif
#endif //SCCONST

