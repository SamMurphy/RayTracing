#pragma once

#include "hittable.h"

class sphere : public hittable
{
public:
	sphere(point3 _centre, float _radius, shared_ptr<material> _material) 
		: centre(_centre), radius(_radius), mat(_material) {}

	bool hit(const Ray& r, interval ray_t, hit_record& rec) const override;

private:
	point3 centre;
	float radius;
	shared_ptr<material> mat;
};

