/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 12/07/2011 7:47:35 PM
 */

// TODO
// * check return values
// * verify default `Timer(Timer& src)` and `void operator=(Timer& src)`
// * add Linux impl for thread priority boost

#ifndef HIRES_TIMER_H_
#define HIRES_TIMER_H_

#include <ostream>
#include <stdexcept>
#include <string>

#if defined(_WIN32)
#  include <windows.h>
#else
#  include <time.h>
#  include <sched.h>
#endif

namespace HiRes {

enum TimeUnits { s=1, ms=1000, us=1000000, ns=1000000000 };

// Timer class template declaration
template <TimeUnits Units = HiRes::s>
class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    double stop();
    double get_frequency();
    void print(std::ostream& os);

private:
    double _frequency;
#if defined(_WIN32)
    DWORD_PTR _affinity_mask;
    LARGE_INTEGER _start_count;
    LARGE_INTEGER _stop_count;
#else
    cpu_set_t _affinity_mask;
    timespec _start_count;
    timespec _stop_count;
#endif
};


// Separate Timer class template definition for possible extraction to .cpp
template <TimeUnits Units>
Timer<Units>::Timer()
: _frequency()
, _affinity_mask()
, _start_count()
, _stop_count()
{
    _frequency = get_frequency();

#if !defined(_WIN32)
    if (::sched_getaffinity(0, sizeof(_affinity_mask), &_affinity_mask) == -1)
        std::cerr << "[WARN] unable to get default CPU affinity mask" << std::endl;
#endif
}

template <TimeUnits Units>
Timer<Units>::~Timer()
{
}

template <TimeUnits Units>
void Timer<Units>::start()
{
#if defined(_WIN32)
    if (!(_affinity_mask = ::SetThreadAffinityMask(::GetCurrentThread(), 1)))
        std::cerr << "[WARN] unable to set current thread's CPU affinity" << std::endl;
    if (!::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL))
        std::cerr << "[WARN] unable to increase current thread's priority" << std::endl;

    ::QueryPerformanceCounter(&_start_count);
#else
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    if (::sched_setaffinity(0, sizeof(mask), &mask) == -1)
        std::cerr << "[WARN] unable to set current thread's CPU affinity" << std::endl;

    ::clock_gettime(CLOCK_MONOTONIC, &_start_count);
#endif
}

template <TimeUnits Units>
double Timer<Units>::stop()
{
#if defined(_WIN32)
    ::QueryPerformanceCounter(&_stop_count);

    if (!::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_NORMAL))
        std::cerr << "[WARN] unable to normalize current thread's priority" << std::endl;
    if (!::SetThreadAffinityMask(::GetCurrentThread(), _affinity_mask))
        std::cerr << "[WARN] unable to reset current thread's CPU affinity" << std::endl;

    return ((_stop_count.QuadPart - _start_count.QuadPart) / _frequency * Units);
#else
    ::clock_gettime(CLOCK_MONOTONIC, &_stop_count);

    if (::sched_setaffinity(0, sizeof(_affinity_mask), &_affinity_mask) == -1)
        std::cerr << "[WARN] unable to reset current thread's CPU affinity" << std::endl;

    time_t delta_sec = _stop_count.tv_sec - _start_count.tv_sec;
    long delta_nsec = _stop_count.tv_nsec - _start_count.tv_nsec;

    if (delta_nsec < 0) {
        delta_sec = _stop_count.tv_sec - _start_count.tv_sec - 1;
        delta_nsec = (ns + _stop_count.tv_nsec) - _start_count.tv_nsec;
    }

    // TODO dodgy, review type limits
    return (delta_sec + (delta_nsec / static_cast<double>(ns)) * Units);
#endif
}

template <TimeUnits Units>
double Timer<Units>::get_frequency()
{
#if defined(_WIN32)
    LARGE_INTEGER freq;

    if (!::QueryPerformanceFrequency(&freq))
        throw std::runtime_error(
            "[ERROR] QueryPerformanceFrequency() failed; your platform may lack support.");

    return freq.QuadPart;
#else
    // TODO implement rv
    //      throw runtime exception if timer isn't high resolution?
    timespec freq;

    if (clock_getres(CLOCK_MONOTONIC, &freq))
        throw std::runtime_error("[ERROR] clock_getres() failed.");

    return 0;
#endif
}

template <TimeUnits Units>
void Timer<Units>::print(std::ostream& os)
{
    using std::string;
    string time_unit;

    switch(Units) {
        case s:
            time_unit = "s";
            break;
        case ms:
            time_unit = "ms";
            break;
        case us:
            time_unit = "us";
            break;
        case ns:
            time_unit = "ns";
            break;
        default:
            time_unit = "??";
            break;
    };

    os  << "Timer<"
        << this << ", "
        << "units: "
        << time_unit << ", "
        << "freq: "
        << _frequency
        << ">";
}

}  // namespace HiRes


/**
 * Global function template for writing a HiRes::Timer instance to an output
 * stream similar to:
 *
 *      HiRes::Timer<HiRes::ms> timer_ms;
 *      cout << timer_ms << endl;
 */
template <HiRes::TimeUnits Units, typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, HiRes::Timer<Units>& timer)
{
    timer.print(os);
    return os;
}

#endif  // HIRES_TIMER_H_
