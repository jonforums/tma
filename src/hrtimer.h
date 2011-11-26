/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 11/26/2011 12:07:41 PM
 */

// TODO
// * relook at `SetThreadAffinityMask` set/reset usage in `start` & `stop`
// * verify default `Timer(Timer& src)` and `void operator=(Timer& src)`
// * finish `print` impl
// * impl global `ostream& operator<<(ostream& os, Timer& timer)`
// * impl Linux hires timing support

#ifndef HIRES_TIMER_H_
#define HIRES_TIMER_H_

#include <ostream>
#include <stdexcept>

#if defined(_WIN32)
#  include <windows.h>
#endif

namespace HiRes {

enum TimeUnits { s=1, ms=1000, us=1000000, ns=1000000000 };

// Timer class template declaration
template <TimeUnits Units = HiRes::s>
class Timer {
public:
    Timer();
    ~Timer();

    void start();
    double stop();
    double get_frequency();
    void print(std::ostream& os);

private:
    double frequency;
    LARGE_INTEGER start_count;
    LARGE_INTEGER stop_count;
};


// Separate Timer class template definition for possible extraction to .cpp
template<TimeUnits Units>
Timer<Units>::Timer()
:   frequency(),
    start_count(),
    stop_count()
{
    frequency = this->get_frequency();
}

template<TimeUnits Units>
Timer<Units>::~Timer()
{
}

template<TimeUnits Units>
void Timer<Units>::start()
{
    DWORD_PTR prev_am = ::SetThreadAffinityMask(::GetCurrentThread(), 0);

    ::QueryPerformanceCounter(&start_count);

    ::SetThreadAffinityMask(::GetCurrentThread(), prev_am);
}

template<TimeUnits Units>
double Timer<Units>::stop()
{
    DWORD_PTR prev_am= ::SetThreadAffinityMask(::GetCurrentThread(), 0);

    ::QueryPerformanceCounter(&stop_count);

    ::SetThreadAffinityMask(::GetCurrentThread(), prev_am);

    return ((stop_count.QuadPart - start_count.QuadPart) / frequency * Units);
}

template<TimeUnits Units>
double Timer<Units>::get_frequency()
{
    LARGE_INTEGER freq;

    if (!::QueryPerformanceFrequency(&freq))
        throw std::runtime_error(
            "[ERROR] QueryPerformanceFrequency() failed; your platform may lack support.");

    return freq.QuadPart;
}

template<TimeUnits Units>
void Timer<Units>::print(std::ostream& os)
{
    os  << "Timer<units: "
        << "TODO" << ", "
        << "freq: "
        << frequency
        << ">";
}

}  // namespace HiRes

#endif  // HIRES_TIMER_H_
