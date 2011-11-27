/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 11/27/2011 1:58:32 PM
 */

#ifndef LOAD_H_
#define LOAD_H_

#if defined(_WIN32)
#  include <windows.h>
#else
#  include <dlfcn.h>
#  define BOOL int
#  define HANDLE void*
#  define HMODULE void*
#  define FARPROC void*
#  define LPCSTR const char*
#endif

inline HMODULE load_dll(LPCSTR filename)
{
#if defined(_WIN32)
    return LoadLibraryA(filename);
#else
    return dlopen(filename, RTLD_LAZY|RTLD_GLOBAL);
#endif
}

inline FARPROC symbol_from_dll(HMODULE dll, LPCSTR symbol)
{
#if defined(_WIN32)
    return GetProcAddress(dll, symbol);
#else
    return dlsym(dll, symbol);
#endif
}

inline BOOL close_dll(HMODULE dll)
{
#if defined(_WIN32)
    return FreeLibrary(dll);
#else
    return dlclose(dll);
#endif
}

#endif  // LOAD_H_
