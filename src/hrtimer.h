/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 11/25/2011 10:25:59 PM
 */

// TODO list:
// * relook at `SetThreadAffinityMask` set/reset usage in `start` & `stop`

#ifndef HIRES_TIMER_H_
#define HIRES_TIMER_H_

#include <windows.h>
#include <stdexcept>

namespace HiRes {

enum TimeUnits { s=1, ms=1000, us=1000000, ns=1000000000 };

// Timer class template declaration
template <TimeUnits tu>
class Timer {
public:
    Timer();
    ~Timer();

    void start();
    double stop();
    double get_frequency();

private:
    double frequency;
    LARGE_INTEGER start_count;
    LARGE_INTEGER stop_count;
};


// Separate Timer class template definition for possible extraction to .cpp
template<TimeUnits tu>
Timer<tu>::Timer()
:   frequency(),
    start_count(),
    stop_count()
{
    frequency = this->get_frequency();
}

template<TimeUnits tu>
Timer<tu>::~Timer()
{
}

template<TimeUnits tu>
double Timer<tu>::get_frequency()
{
    LARGE_INTEGER freq;

    if (!::QueryPerformanceFrequency(&freq))
        throw std::runtime_error(
            "[ERROR] QueryPerformanceFrequency() failed; your platform may lack support.");

    return freq.QuadPart;
}

template<TimeUnits tu>
void Timer<tu>::start()
{
    DWORD_PTR prev_am = ::SetThreadAffinityMask(::GetCurrentThread(), 0);

    ::QueryPerformanceCounter(&start_count);

    ::SetThreadAffinityMask(::GetCurrentThread(), prev_am);
}

template<TimeUnits tu>
double Timer<tu>::stop()
{
    DWORD_PTR prev_am= ::SetThreadAffinityMask(::GetCurrentThread(), 0);

    ::QueryPerformanceCounter(&stop_count);

    ::SetThreadAffinityMask(::GetCurrentThread(), prev_am);

    return ((stop_count.QuadPart - start_count.QuadPart) / frequency * tu);
}

}  // namespace HiRes

#endif  // HIRES_TIMER_H_
