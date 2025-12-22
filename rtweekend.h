#ifndef RTWKND_RTWEEKEND_H
#define RTWKND_RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <random>


// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(const double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    thread_local std::mt19937 generator(std::random_device{}());
    return distribution(generator);
}

inline double random_double(const double min, const double max) {
    return min + (max - min) * random_double();
}

// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif //RTWKND_RTWEEKEND_H
