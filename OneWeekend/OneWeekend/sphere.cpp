#include "sphere.h"
#include "interval.h"

bool sphere::hit(const Ray& r, interval ray_t, hit_record& rec) const
{
    vec3 oc = r.Origin() - centre;
    float a = r.Direction().length_squared();
    float half_b = dot(oc, r.Direction());
    float c = oc.length_squared() - radius * radius;

    float discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
    {
        return false;
    }

    float sqrtd = sqrt(discriminant);

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
    vec3 outward_normal = (rec.p - centre) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return true;
}
