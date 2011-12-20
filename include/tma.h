/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 12/18/2011 9:35:42 PM
 */

#ifndef TMA_H_
#define TMA_H_

#if defined(_WIN32)
#  define TMA_CAPI(type) extern "C" __declspec(dllexport) type
#  define TIMING_ENTRY_POINT extern "C" __declspec(dllexport) void
#else
#  define TMA_CAPI(type) extern "C" type
#  define TIMING_ENTRY_POINT extern "C" void
#endif

// Function pointer to the timed library's entry point. The library's entry point
// function *must* have the following prototype:
//
//      TIMING_ENTRY_POINT timed_main(int argc, char* argv[]);
typedef void (*timed_fcn)(int, char* []);

#endif  // TMA_H_
