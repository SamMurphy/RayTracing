#include "vec3.h"
#include "utilities.h"

vec3 vec3::random()
{
	return vec3(random_float(), random_float(), random_float());
}

vec3 vec3::random(float min, float max)
{
	return vec3(
		random_float(min, max),
		random_float(min, max),
		random_float(min, max));
}
