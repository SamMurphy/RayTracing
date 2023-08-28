#pragma once

#include "vec3.h"

class Ray
{
public:
    Ray() {}

    Ray(const point3& origin, const vec3& direction) : origin_point(origin), ray_direction(direction) {}

    point3 Origin() const { return origin_point; }
    vec3 Direction() const { return ray_direction; }

    point3 at(float t) const
    {
        return origin_point + t * ray_direction;
    }

private:
    point3 origin_point;
    vec3 ray_direction;
};
