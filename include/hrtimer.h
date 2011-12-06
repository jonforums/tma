/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 12/05/2011 3:22:05 PM
 */

// TODO
// * check return values
// * verify default `Timer(Timer& src)` and `void operator=(Timer& src)`
// * add `int sched_setaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask)`
//   with pid = 0 to Linux impl of start/stop
// * extract CPU affinity code from start/stop timing code paths; always run
//   timer and timed code on the first CPU?

#ifndef HIRES_TIMER_H_
#define HIRES_TIMER_H_

#include <ostream>
#include <stdexcept>
#include <string>

#if defined(_WIN32)
#  include <windows.h>
#else
#  include <time.h>
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
#if defined(_WIN32)
    DWORD_PTR affinity_mask;
    LARGE_INTEGER start_count;
    LARGE_INTEGER stop_count;
#else
    timespec start_count;
    timespec stop_count;
#endif
};


// Separate Timer class template definition for possible extraction to .cpp
template<TimeUnits Units>
Timer<Units>::Timer()
:   frequency(),
    affinity_mask(),
    start_count(),
    stop_count()
{
    frequency = get_frequency();
}

template<TimeUnits Units>
Timer<Units>::~Timer()
{
}

template<TimeUnits Units>
void Timer<Units>::start()
{
#if defined(_WIN32)
    if (!(affinity_mask = ::SetThreadAffinityMask(::GetCurrentThread(), 1)))
        std::cerr << "[WARN] unable to set current thread's CPU affinity" << std::endl;

    ::QueryPerformanceCounter(&start_count);
#else
    ::clock_gettime(CLOCK_MONOTONIC, &start_count);
#endif
}

template<TimeUnits Units>
double Timer<Units>::stop()
{
#if defined(_WIN32)
    ::QueryPerformanceCounter(&stop_count);

    if (!::SetThreadAffinityMask(::GetCurrentThread(), affinity_mask))
        std::cerr << "[WARN] unable to reset current thread's CPU affinity" << std::endl;

    return ((stop_count.QuadPart - start_count.QuadPart) / frequency * Units);
#else
    ::clock_gettime(CLOCK_MONOTONIC, &stop_count);

    time_t delta_sec = stop_count.tv_sec - start_count.tv_sec;
    long delta_nsec = stop_count.tv_nsec - start_count.tv_nsec;

    if (delta_nsec < 0) {
        delta_sec = stop_count.tv_sec - start_count.tv_sec - 1;
        delta_nsec = (ns + stop_count.tv_nsec) - start_count.tv_nsec;
    }

    // TODO dodgy, review type limits
    return (delta_sec + (delta_nsec / static_cast<double>(ns)) * Units);
#endif
}

template<TimeUnits Units>
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

template<TimeUnits Units>
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
        << frequency
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
template<HiRes::TimeUnits Units, typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, HiRes::Timer<Units>& timer)
{
    timer.print(os);
    return os;
}

#endif  // HIRES_TIMER_H_
