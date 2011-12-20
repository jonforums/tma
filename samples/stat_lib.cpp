/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 12/19/2011 7:17:41 PM
 */

#include <ios>
#include <iostream>
#include <ostream>
#include <iomanip>

#if defined(_WIN32)
# define testfile "tma.exe"
# include <windows.h>
# include <sys/stat.h>
#else
# define testfile "tma"
# include <sys/stat.h>
#endif

#include "hrtimer.h"
#include "tma.h"

TIMING_ENTRY_POINT timed_main(int argc, char* argv[])
{
    using namespace std;

    double rv = 0.0;

    // hires timer returning usecond times
    HiRes::Timer<HiRes::us> t_us;

    struct stat statbuf;
    t_us.start();
    stat(testfile, &statbuf);
    rv = t_us.stop();
    cout    << left
            << setw(26)
            << "stat() time: "
            << rv << endl;

#if defined (_WIN32)
    t_us.start();
    GetFileAttributes(testfile);
    rv = t_us.stop();
    cout    << left
            << setw(26)
            << "GetFileAttributes() time: "
            << rv << endl;
#endif

}
