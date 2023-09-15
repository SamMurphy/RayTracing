#pragma once
#include "colour.h"
#include "Ray.h"

#define LAMBERT_REFLECTIONS 1

class hit_record;

class material
{
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
	lambertian(const colour& a) : albedo(a) {}

	bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override;

private:
	colour albedo;
};

class metal : public material
{
public:
	metal(const colour& a, const float _fuzz) : albedo(a), fuzz(_fuzz < 1.0f ? _fuzz : 1.0f) {}

	bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override;

private:
	colour albedo;
	float fuzz;
};

class dielectric : public material
{
public:
	dielectric(const float index_of_refraction) : ir(index_of_refraction) {}

	bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override;

private:
	float ir; // Index of refraction

	// Sclick's approximation for reflectance
	static float reflectance(const float cosine, const float ref_idx)
	{
		float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
		r0 = r0 * r0;
		return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
	}
};