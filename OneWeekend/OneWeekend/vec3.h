#pragma once

#include <cmath>
#include <iostream>

class vec3
{
public:
	float data[3];

	vec3() : data{ 0,0,0 } {}
	vec3(const float x, const float y, const float z) : data{ x, y, z } {}

	float X() const { return data[0]; }
	float Y() const { return data[1]; }
	float Z() const { return data[2]; }

	float R() const { return data[0]; }
	float G() const { return data[1]; }
	float B() const { return data[2]; }

	vec3 operator-() const { return {-data[0], -data[1], -data[2]}; }
	float operator[](const int i) const { return data[i]; }
	float& operator[](const int i) { return data[i]; }

	vec3& operator+=(const vec3& v)
	{
		data[0] += v.data[0];
		data[1] += v.data[1];
		data[2] += v.data[2];

		return *this;
	}

	vec3& operator+=(const float f)
	{
		data[0] += f;
		data[1] += f;
		data[2] += f;

		return *this;
	}

	vec3& operator*=(const float t)
	{
		data[0] *= t;
		data[1] *= t;
		data[2] *= t;

		return *this;
	}

	vec3& operator/=(const float t)
	{
		return *this *= 1 / t;
	}

	float length() const
	{
		return sqrt(length_squared());
	}

	float length_squared() const
	{
		return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
	}

	bool near_zero() const
	{
		// Return true if the vector is close to zero in all dimensions.
		constexpr float s = 1e-8f;
		return (fabs(data[0]) < s) && (fabs(data[1]) < s) && (fabs(data[2]) < s);
	}

	static vec3 random();

	static vec3 random(float min, float max);
};

// point3 is an alias for vec3, but useful for geometric clarity
using point3 = vec3;

// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v.data[0] << ' ' << v.data[1] << ' ' << v.data[2];
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
	return {
		u.data[0] + v.data[0],
		u.data[1] + v.data[1],
		u.data[2] + v.data[2]
	};
}

inline vec3 operator+(const vec3& v, const float f)
{
	return {
		v.data[0] + f,
		v.data[1] + f,
		v.data[2] + f
	};
}

inline vec3 operator+(const float f, const vec3& v)
{
	return v + f;
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
	return {
		u.data[0] - v.data[0],
		u.data[1] - v.data[1],
		u.data[2] - v.data[2]
	};
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
	return {
		u.data[0] * v.data[0],
		u.data[1] * v.data[1],
		u.data[2] * v.data[2]
	};
}

inline vec3 operator*(const float t, const vec3& v) 
{
	return {
		t * v.data[0],
		t * v.data[1],
		t * v.data[2]
	};
}

inline vec3 operator*(const vec3& v, const float t) 
{
	return t * v;
}

inline vec3 operator/(const vec3 v, const float t)
{
	return (1 / t) * v;
}

inline float dot(const vec3& u, const vec3& v)
{
	return u.data[0] * v.data[0]
		+ u.data[1] * v.data[1]
		+ u.data[2] * v.data[2];
}

inline vec3 cross(const vec3& u, const vec3& v)
{
	return {
		u.data[1] * v.data[2] - u.data[2] * v.data[1],
		u.data[2] * v.data[0] - u.data[0] * v.data[2],
		u.data[0] * v.data[1] - u.data[1] * v.data[0]
	};
}

inline vec3 unit_vector(const vec3 v)
{
	return v / v.length();
}

inline vec3 normalise(const vec3 v)
{
	return unit_vector(v);
}

inline vec3 random_in_unit_sphere()
{
	while (true)
	{
		point3 p = point3::random(-1, 1);
		if (p.length_squared() < 1)
		{
			return p;
		}
	}
}

inline vec3 random_unit_vector()
{
	return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal)
{
	const vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as normal
	{
		return on_unit_sphere;
	}
	else
	{
		return -on_unit_sphere;
	}
}

inline vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, const float etai_over_etai)
{
	const float cos_theta = fmin(dot(-uv, n), 1.0f);
	const vec3 r_out_perp = etai_over_etai * (uv + cos_theta * n);
	const vec3 r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}