/* This is a generated file */

#ifndef __debug_h__
#define __debug_h__

/** This file provides some CPP (C pre processor) macro debug functions:
 *
 *    SPEW() ERROR() ASSERT() FAIL()
 *
 * and compile time conditionally:
 *
 *    WARN() NOTICE() INFO() DSPEW() DASSERT()
 *
 * see details below.
 *
 *
 *
 * It's really not much code but is a powerful development
 * tool like assert.  See 'man assert'.
 *
 * CRTSFilter builders do not have to include this file in their
 * plugin modules.  It's pretty handy for developing/debugging.
 *
 */
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>


#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////
// This @ DEBUG @ will be either:  #define DEBUG
//       or              // DEBUG is not defined
/////////////////////////////////////////////////////////////////////////
#define DEBUG


/////////////////////////////////////////////////////////////////////////
// This @ SPEW_LEVEL @ will be something like:
//    #define SPEW_LEVEL_DEBUG
// or #define SPEW_LEVEL_INFO
// or #define SPEW_LEVEL_NOTICE
// or #define SPEW_LEVEL_WARN
// or #define SPEW_LEVEL_ERROR
/////////////////////////////////////////////////////////////////////////
#define SPEW_LEVEL_DEBUG



#define SPEW_FILE stderr

///////////////////////////////////////////////////////////////////////////
//
// ##__VA_ARGS__ comma not eaten with -std=c++0x
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=44317
//
// There is a GCC bug where GCC wrongly warns about ##__VA_ARGS__, so using
// #pragma GCC system_header suppresses these warnings.  Should this GCC
// bug get fixed, it'd be good to remove this next code line.
// See also: https://gcc.gnu.org/onlinedocs/cpp/System-Headers.html
#pragma GCC system_header

// We would like to be able to just call SPEW() with no arguments
// which can make a zero length printf format.
#pragma GCC diagnostic ignored "-Wformat-zero-length"

//
///////////////////////////////////////////////////////////////////////////
//
// The macros SPEW() ERROR() ASSERT() FAIL() are always on 
//

extern void _spew(FILE *stream, int errn, const char *pre, const char *file,
        int line, const char *func, const char *fmt, ...)
         // check printf format errors at compile time:
        __attribute__ ((format (printf, 7, 8)));

extern void _assertAction(FILE *stream);



#  define _SPEW(errn, pre, fmt, ... )\
     _spew(SPEW_FILE, errn, pre, __BASE_FILE__, __LINE__,\
        __func__, fmt "\n", ##__VA_ARGS__)

#  define ERROR(fmt, ...)\
     _SPEW(errno, "ERROR: ", fmt, ##__VA_ARGS__)

// SPEW is like ERROR but just called SPEW
#  define SPEW(fmt, ...)\
     _SPEW(0, "SPEW: ", fmt, ##__VA_ARGS__)

#  define ASSERT(val, ...) \
    do {\
        if(!((bool) (val))) {\
            _SPEW(errno, "ASSERTION("#val") failed: ", ##__VA_ARGS__);\
            _assertAction(SPEW_FILE);\
        }\
    }\
    while(0)

#  define FAIL(fmt, ...) \
    do {\
        _SPEW(errno, "FAIL: ", fmt, ##__VA_ARGS__);\
        _assertAction(SPEW_FILE);\
    } while(0)


///////////////////////////////////////////////////////////////////////////


#ifdef SPEW_LEVEL_DEBUG // The highest verbosity
#  ifndef SPEW_LEVEL_INFO
#    define SPEW_LEVEL_INFO
#  endif
#endif
#ifdef SPEW_LEVEL_INFO
#  ifndef SPEW_LEVEL_NOTICE
#    define SPEW_LEVEL_NOTICE
#  endif
#endif
#ifdef SPEW_LEVEL_NOTICE
#  ifndef SPEW_LEVEL_WARN
#    define SPEW_LEVEL_WARN
#  endif
#endif
#ifdef SPEW_LEVEL_WARN
#  ifndef SPEW_LEVEL_ERROR
#    define SPEW_LEVEL_ERROR
#  endif
#endif


// Setting SPEW_LEVEL_ERROR is the least verbose level
// SPEW() and ERROR() will both print at any level
//

#ifndef SPEW_LEVEL_ERROR
// We have no level SPEW_LEVEL_(DEBUG|INFO|NOTICE|WARN|ERROR)
// so we set a default here:
//#  define SPEW_LEVEL_DEBUG
#  define SPEW_LEVEL_INFO
#  define SPEW_LEVEL_NOTICE
#  define SPEW_LEVEL_WARN
#endif


#ifdef DEBUG
#  define DASSERT(val, fmt, ...) ASSERT(val, fmt, ##__VA_ARGS__)
#else
#  define DASSERT(val, fmt, ...) /*empty marco*/
#endif

#ifdef SPEW_LEVEL_WARN
#  define WARN(fmt, ...) _SPEW(errno, "WARN: ", fmt, ##__VA_ARGS__)
#else
#  define WARN(fmt, ...) /*empty macro*/
#endif 

#ifdef SPEW_LEVEL_NOTICE
#  define NOTICE(fmt, ...) _SPEW(errno, "NOTICE: ", fmt, ##__VA_ARGS__)
#else
#  define NOTICE(fmt, ...) /*empty macro*/
#endif

#ifdef SPEW_LEVEL_INFO
#  define INFO(fmt, ...)   _SPEW(0, "INFO: ", fmt, ##__VA_ARGS__)
#else
#  define INFO(fmt, ...) /*empty macro*/
#endif

#ifdef SPEW_LEVEL_DEBUG
#  define DSPEW(fmt, ...)  _SPEW(0, "DEBUG: ", fmt, ##__VA_ARGS__)
#else
#  define DSPEW(fmt, ...) /*empty macro*/
#endif



#ifdef __cplusplus
}
#endif

#endif // #ifndef __debug_h__
