/* Copyright (c) 2011, Luis Lavena, Hiroshi Shirosaki
 * License: 3-clause BSD
 * Revision: 12/07/2011 10:57:34 AM
 */

#include <ios>
#include <iostream>
#include <ostream>
#include <iomanip>
#include <stdexcept>

#if defined (_WIN32)
#  include <windows.h>
#  include <io.h>
#  include <fcntl.h>
#  define executable "file_access.exe"
#  define access _access
#  define open _open
#  define close _close
#  include <sys/stat.h>
#else
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <unistd.h>
#  define executable "file_access"
#endif

#include "hrtimer.h"

int main(int argc, char* argv[])
{
    using namespace std;

    double rv = 0.0;

    // hires timer returning usecond times
    HiRes::Timer<HiRes::us> t_us;

    string headers[] = {"file exist", "file not exist"};
    string file_path[] = {executable, "dummy"};
    for (int i = 0; i < 2; i++) {
        const char *file = file_path[i].c_str();
        cout << endl << headers[i] << endl;
        t_us.start();
        access(file, 4);
        rv = t_us.stop();
        cout    << left
                << setw(30)
                << "access() time: "
                << rv << endl;

        t_us.start();
        int fd = open(file, O_RDONLY);
        if (fd != -1) close(fd);
        rv = t_us.stop();
        cout    << left
                << setw(30)
                << "open() time: "
                << rv << endl;


        struct stat statbuf;
        t_us.start();
        stat(file, &statbuf);
        rv = t_us.stop();
        cout    << left
                << setw(30)
                << "stat() time: "
                << rv << endl;

#if defined(_WIN32)
        OFSTRUCT openbuf;
        t_us.start();
        HFILE hFile = OpenFile(file, &openbuf, OF_READ);
        if (hFile != HFILE_ERROR) CloseHandle((HANDLE) hFile);
        rv = t_us.stop();
        cout    << left
                << setw(30)
                << "OpenFile() time: "
                << rv << endl;

        t_us.start();
        HANDLE h = CreateFile(file, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
        rv = t_us.stop();
        cout    << left
                << setw(30)
                << "CreateFile() time: "
                << rv << endl;

        WIN32_FILE_ATTRIBUTE_DATA fileInfo;
        t_us.start();
        GetFileAttributesEx(file, GetFileExInfoStandard, &fileInfo);
        rv = t_us.stop();
        cout    << left
                << setw(30)
                << "GetFileAttributesEx() time: "
                << rv << endl;

        t_us.start();
        GetFileAttributes(file);
        rv = t_us.stop();
        cout    << left
                << setw(30)
                << "GetFileAttributes() time: "
                << rv << endl;
#endif
    }

    return 0;
}
