#pragma once

#include "hittable.h"

class sphere : public hittable
{
public:
	sphere(const point3 _centre, const float _radius, const shared_ptr<material>& _material) 
		: centre(_centre), radius(_radius), mat(_material) {}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

private:
	point3 centre;
	float radius;
	shared_ptr<material> mat;
};

