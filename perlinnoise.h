//
// Created by giuseppe on 20/08/18.
//

// THIS IS A DIRECT TRANSLATION TO C++11 FROM THE REFERENCE
// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

#ifndef PROJGRAF_PERLINNOISE_H
#define PROJGRAF_PERLINNOISE_H

#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>


class PerlinNoise {
// The permutation vector
    std::vector<int> p;
public:
    // Initialize with the reference values for the permutation vector
    PerlinNoise();
    // Get a noise value, for 2D images z can have any value
    double noise(double x, double y, double z);
private:
    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
};


#endif //PROJGRAF_PERLINNOISE_H

