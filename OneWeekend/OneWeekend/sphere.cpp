#include "sphere.h"

#include "interval.h"

bool sphere::hit(const ray& r, const interval ray_t, hit_record& rec) const
{
    const vec3 oc = r.origin() - centre;
    const float a = r.direction().length_squared();
    const float half_b = dot(oc, r.direction());
    const float c = oc.length_squared() - radius * radius;

    const float discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
    {
        return false;
    }

    const float sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (-half_b - sqrtd) / a;
    if (!ray_t.surrounds(root))
    {
        root = (-half_b + sqrtd) / a;
        if (!ray_t.surrounds(root))
        {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    const vec3 outward_normal = (rec.p - centre) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return true;
}
