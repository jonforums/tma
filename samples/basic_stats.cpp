/* Copyright (c) 2011, Jon Maken
 * License: 3-clause BSD
 * Revision: 12/09/2011 2:36:08 PM
 *
 * Example:
 *
 *      HiRes::Statistics<double> stats =
 *          for_each(istream_iterator<double>(cin),
 *                   istream_iterator<double><(),
 *                   HiRes::Statistics<double>());
 */

#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <vector>

#if defined(_WIN32)
#  include <io.h>
#  define executable "basic_stats.exe"
#  define access _access
#else
#  include <unistd.h>
#  define executable "basic_stats"
#endif

#include "hrtimer.h"
#include "statistics.h"

const int TEST_COUNT = 20;

int main(int argc, char* argv[])
{
    using namespace std;

    double rv = 0.0;
    vector<double> data;
    HiRes::Timer<HiRes::us> tmr_us;

    for (int i = 0; i < TEST_COUNT; i++)
    {
        tmr_us.start();
        access(executable, 4);
        rv = tmr_us.stop();
        data.push_back(rv);
    }

    HiRes::Statistics<double> stats =
        for_each(data.begin(), data.end(), HiRes::Statistics<double>());

    cout    << "Count = " << stats.count() << '\n'
            << "Min = " << stats.min() << '\n'
            << "Max = " << stats.max() << '\n'
            << "Sum = " << stats.sum() << '\n'
            << "Sum of Squares = " << stats.sum_of_squares() << '\n'
            << "Mean = " << stats.mean() << '\n'
            << "Variance = " << stats.variance() << '\n'
            << "Stdev = " << stats.stdev() << '\n'
            << "Coefficient of Variation = " << stats.cv() << endl;

    return 0;
}
