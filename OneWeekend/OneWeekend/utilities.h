#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const float infinity = std::numeric_limits<float>::infinity();
constexpr float pi = 3.1415926535897932385f;

// Utility functions

inline float degrees_to_radians(float degrees)
{
	return degrees * pi / 180.0f;
}

inline float random_float()
{
	return static_cast<float>(rand()) / (RAND_MAX + 1.0f);
}

inline float random_float(const float min, const float max)
{
	return min + (max-min) * random_float();
}

// Common Headers

#include "Ray.h"
#include "vec3.h"

inline vec3 random_in_unit_disk()
{
	while (true)
	{
		point3 p = point3(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f), 0.0f);
		if (p.length_squared() < 1)
		{
			return p;
		}
	}
}