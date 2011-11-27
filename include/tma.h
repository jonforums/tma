/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 11/26/2011 7:57:44 PM
 */

#if defined(_WIN32)
#  define TMA_CAPI(type) extern "C" __declspec(dllexport) type
#else
#  define TMA_CAPI(type) extern "C" type
#endif
