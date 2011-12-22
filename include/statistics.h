/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 12/21/2011 10:11:44 PM
 */

#ifndef HIRES_STATISTICS_H_
#define HIRES_STATISTICS_H_

// XXX needed to prevent build failures on VC++/Windows SDK but not MinGW
#if defined (_MSC_VER)
#  undef min
#  undef max
#endif

#include <cmath>
#include <limits>

namespace HiRes {

template <typename T>
class Statistics
{
public:
    Statistics()
    : _n(0)
    , _min(std::numeric_limits<T>::max())
    , _max(std::numeric_limits<T>::min())
    , _sum(0)
    , _sum_squares(0)
    {
    }

    // operator overrides
    void operator()(double i)
    {
        ++_n;
        _sum += i;
        _sum_squares += i * i;
        _min = i < _min ? i : _min;
        _max = i > _max ? i : _max;
    }

    // methods
    // TODO implement median
    std::size_t count() const { return _n; }
    T min() const { return _min; }
    T max() const { return _max; }
    T sum() const { return _sum; }
    T sum_of_squares() const { return _sum_squares; }
    T mean() const { return _sum / _n; }
    T variance() const
    {
        return (_sum_squares - _sum * _sum / _n) / (_n - 1);
    }
    T stdev() const { return std::sqrt(variance()); }
    T cv() const { return stdev() / mean(); };

private:
    std::size_t _n;
    T _min;
    T _max;
    T _sum;
    T _sum_squares;
};

}  // namespace HiRes

#endif  // HIRES_STATISTICS_H_
