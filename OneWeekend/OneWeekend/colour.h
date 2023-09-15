#pragma once

#include <iostream>
#include "interval.h"
#include "vec3.h"

using colour = vec3;

inline float linear_to_gamma(float linear_component)
{
	return sqrt(linear_component);
}

inline int convert_colour(float colour, int samples_per_pixel = 1)
{
	const float scale = 1.0f / static_cast<float>(samples_per_pixel);
	static const interval intensity(0.0f, 0.999f);

	colour = colour * scale;
	colour = linear_to_gamma(colour);
	colour = intensity.clamp(colour);
	colour = 255.999f * colour;

	return static_cast<int>(colour);
}

inline colour convert_colour(colour pixel_colour, int samples_per_pixel = 1)
{
	const colour out(
		static_cast<float>(convert_colour(pixel_colour.R(), samples_per_pixel)),
		static_cast<float>(convert_colour(pixel_colour.G(), samples_per_pixel)),
		static_cast<float>(convert_colour(pixel_colour.B(), samples_per_pixel)));

	return out;
}

inline void write_colour(std::ostream& out, colour pixel_colour, int samples_per_pixel = 1)
{
	// Write the translated [0,255] value of each colour component.
	out << convert_colour(pixel_colour.R(), samples_per_pixel) << ' '
		<< convert_colour(pixel_colour.G(), samples_per_pixel) << ' '
		<< convert_colour(pixel_colour.B(), samples_per_pixel) << '\n';
}