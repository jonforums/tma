/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 11/25/2011 6:44:32 PM
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
    LARGE_INTEGER start_time;
    LARGE_INTEGER stop_time;
};


// Separate Timer class template definition for possible extraction to .cpp
template<TimeUnits tu>
Timer<tu>::Timer()
:   frequency(),
    start_time(),
    stop_time()
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

    ::QueryPerformanceCounter(&start_time);

    ::SetThreadAffinityMask(::GetCurrentThread(), prev_am);
}

template<TimeUnits tu>
double Timer<tu>::stop()
{
    DWORD_PTR prev_am= ::SetThreadAffinityMask(::GetCurrentThread(), 0);

    ::QueryPerformanceCounter(&stop_time);

    ::SetThreadAffinityMask(::GetCurrentThread(), prev_am);

    return ((stop_time.QuadPart - start_time.QuadPart) / frequency * tu);
}

}  // namespace HiRes

#endif  // HIRES_TIMER_H_
