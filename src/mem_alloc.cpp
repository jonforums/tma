/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 11/25/2011 6:29:53 PM
 *
 * build recipes (Windows):
 *      cl /nologo /O2 /EHsc mem_alloc.cpp
 *      g++ -Wall -O3 -s -o mem_alloc.exe mem_alloc.cpp
 */

// TODO list:
// * check memory allocation return values

#include <ios>
#include <iostream>
#include <ostream>
#include <iomanip>
#include <stdexcept>

#include <windows.h>

#include "hrtimer.h"

const int BUF_SIZE = 32767;

int main(int argc, char* argv[])
{
    // hires timer returning usecond times
    HiRes::Timer<HiRes::us> t_us;

    // CRT malloc()
    t_us.start();
    wchar_t *buf = static_cast<wchar_t*>(malloc(BUF_SIZE));
    if (buf == NULL) return(-1);
    double rv = t_us.stop();
    std::cout   << std::left
                << std::setw(18)
                << "malloc() time: "
                << rv << std::endl;

    // CRT free() - dependent upon previous malloc()
    t_us.start();
    free(static_cast<void*>(buf));
    rv = t_us.stop();
    buf = NULL;
    std::cout   << std::left
                << std::setw(18)
                << "free() time: "
                << rv << std::endl;

    // CRT calloc()
    t_us.start();
    buf = static_cast<wchar_t*>(calloc(BUF_SIZE, sizeof(char)));
    rv = t_us.stop();
    std::cout   << std::left
                << std::setw(18)
                << "calloc() time: "
                << rv << std::endl;
    free(static_cast<void*>(buf));
    buf = NULL;

    // Win32 HeapAlloc()
    HANDLE heap = ::HeapCreate(0, 2*BUF_SIZE, 4*BUF_SIZE);
    if (heap == NULL) return(-1);
    t_us.start();
    buf = static_cast<wchar_t*>(::HeapAlloc(heap, 0, BUF_SIZE));
    rv = t_us.stop();
    std::cout   << std::left
                << std::setw(18)
                << "HeapAlloc() time: "
                << rv << std::endl;

    // Win32 HeapFree() - dependent upon previous HeapAlloc()
    t_us.start();
    ::HeapFree(heap, 0, buf);
    rv = t_us.stop();
    buf = NULL;
    std::cout   << std::left
                << std::setw(18)
                << "HeapFree() time: "
                << rv << std::endl;
    ::HeapDestroy(heap);

    return 0;
}
