/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 11/27/2011 1:59:08 PM
 */

#ifndef TMA_H_
#define TMA_H_

#if defined(_WIN32)
#  define TMA_CAPI(type) extern "C" __declspec(dllexport) type
#else
#  define TMA_CAPI(type) extern "C" type
#endif

#endif  // TMA_H_
