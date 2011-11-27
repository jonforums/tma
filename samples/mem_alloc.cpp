/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 11/27/2011 6:36:22 PM
 *
 * build recipes (Windows):
 *      cl /nologo /O2 /EHsc mem_alloc.cpp
 *      g++ -Wall -O3 -s -o mem_alloc.exe mem_alloc.cpp
 */

// TODO
// * change to DLL to be called by runner exe

#include <ios>
#include <iostream>
#include <ostream>
#include <iomanip>
#include <stdexcept>

#if defined (_WIN32)
#  include <windows.h>
#endif

#include "hrtimer.h"

const int BUF_SIZE = 32767;

int main(int argc, char* argv[])
{
    using namespace std;

    wchar_t* buf = NULL;
    double rv = 0.0;

    // hires timer returning usecond times
    HiRes::Timer<HiRes::us> t_us;

    // CRT malloc()
    t_us.start();
    buf = static_cast<wchar_t*>(malloc(BUF_SIZE));
    rv = t_us.stop();
    if (buf == NULL) throw runtime_error("[ERROR] malloc() failed.");
    cout    << left
            << setw(18)
            << "malloc() time: "
            << rv << endl;

    // CRT free() - dependent upon previous malloc()
    t_us.start();
    free(static_cast<void*>(buf));
    rv = t_us.stop();
    buf = NULL;
    cout    << left
            << setw(18)
            << "free() time: "
            << rv << endl;

    // CRT calloc()
    t_us.start();
    buf = static_cast<wchar_t*>(calloc(BUF_SIZE, sizeof(char)));
    rv = t_us.stop();
    cout    << left
            << setw(18)
            << "calloc() time: "
            << rv << endl;
    free(static_cast<void*>(buf));
    buf = NULL;

#if defined(_WIN32)
    // Win32 HeapAlloc()
    HANDLE heap = ::HeapCreate(0, 2*BUF_SIZE, 4*BUF_SIZE);
    if (heap == NULL) throw runtime_error("[ERROR] HeapCreate() failed.");
    t_us.start();
    buf = static_cast<wchar_t*>(::HeapAlloc(heap, 0, BUF_SIZE));
    rv = t_us.stop();
    if (buf == NULL) throw runtime_error("[ERROR] HeapAlloc() failed.");
    cout    << left
            << setw(18)
            << "HeapAlloc() time: "
            << rv << endl;

    // Win32 HeapFree() - dependent upon previous HeapAlloc()
    t_us.start();
    ::HeapFree(heap, 0, buf);
    rv = t_us.stop();
    buf = NULL;
    cout    << left
            << setw(18)
            << "HeapFree() time: "
            << rv << endl;
    ::HeapDestroy(heap);
#endif

    return 0;
}
