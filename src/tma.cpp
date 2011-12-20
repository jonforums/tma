/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 12/18/2011 8:46:18 PM
 */

#include <cstdlib>
#include <ios>
#include <iostream>
#include <iomanip>
#include <ostream>
#include <stdexcept>

#include "tma.h"
#include "load.h"

// TODO parse and pass cmd line args to timed library


enum
{
    USAGE_FAIL=1,
    DLL_LOAD_FAIL,
    ENTRY_ADDR_FAIL,
};

void usage_and_exit(int rc=0)
{
    using namespace std;

    cerr    << "usage: tma <TIMED_LIBRARY> [TIMED_OPTIONS]"
            << endl;

    exit(rc);
}

int main(int argc, char* argv[])
{
    using namespace std;

    if (argc < 2) usage_and_exit(USAGE_FAIL);

    HMODULE dll = load_dll(argv[1]);
    if (!dll)
    {
        cerr    << "[ERROR] unable to load '"
                << argv[1]
                << "' library"
                << endl;

        exit(DLL_LOAD_FAIL);
    }

    timed_fcn time_me = reinterpret_cast<timed_fcn>(symbol_from_dll(dll, "timed_main"));
    if (!time_me)
    {
        cerr    << "[ERROR] unable to retrieve the timed library entry function"
                << endl;

        close_dll(dll);
        exit(ENTRY_ADDR_FAIL);
    }

    time_me(0, NULL);
    close_dll(dll);

    return 0;

}
