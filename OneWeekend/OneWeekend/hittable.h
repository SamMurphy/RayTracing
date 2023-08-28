#pragma once
#include "Ray.h"
#include "utilities.h"

class interval;
class material;

class hit_record
{
public:
	point3 p;
	vec3 normal;
	shared_ptr<material> mat;
	float t;
	bool front_face;

	void set_face_normal(const Ray& r, const vec3& outward_normal)
	{
		// Sets the hit record normal vector
		// NOTE: the parameter `outward_normal` is assumed to have unit length

		front_face = dot(r.Direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable
{
public:
	virtual ~hittable() = default;

	virtual bool hit(const Ray& r, interval ray_t, hit_record& rec) const = 0;
};
