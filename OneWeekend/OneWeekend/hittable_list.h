#pragma once

#include <memory>
#include <vector>

#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable
{
public:
	std::vector<shared_ptr<hittable>> objects;

	hittable_list() = default;
	hittable_list(const shared_ptr<hittable>& object) { add(object); }

	void clear() { objects.clear(); }

	void add(const shared_ptr<hittable>& object)
	{
		objects.push_back(object);
	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
};

