#include "vec3.h"

#include "utilities.h"

vec3 vec3::random()
{
	return { random_float(), random_float(), random_float()};
}

vec3 vec3::random(const float min, const float max)
{
	return {
		random_float(min, max),
		random_float(min, max),
		random_float(min, max)
	};
}
