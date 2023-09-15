#include "material.h"

#include "hittable.h"

bool lambertian::scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const
{
#if LAMBERT_REFLECTIONS // more likely to scatter in a direction near the surface normal
    vec3 scatter_direction = rec.normal + random_unit_vector();
#else // random reflections 
    vec3 scatter_direction = random_on_hemisphere(rec.normal);
#endif

    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
    {
        scatter_direction = rec.normal;
    }

    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;

    return true;
}

bool metal::scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const
{
    const vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
    attenuation = albedo;

    return (dot(scattered.direction(), rec.normal) > 0);
}

bool dielectric::scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const
{
    attenuation = colour(1.0f, 1.0f, 1.0f);
    const float refraction_ratio = rec.front_face ? (1.0f / ir) : ir;

    const vec3 unit_direction = unit_vector(r_in.direction());
    const float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0f);
    const float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

    const bool cannot_refract = (refraction_ratio * sin_theta) > 1.0f;
    vec3 direction;
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float())
    {
        direction = reflect(unit_direction, rec.normal);
    }
    else
    {
        direction = refract(unit_direction, rec.normal, refraction_ratio);
    }

    scattered = ray(rec.p, direction);
    return true;
}
