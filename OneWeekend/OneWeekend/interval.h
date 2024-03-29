#pragma once
#include "utilities.h"

class interval
{
public:
	float min, max;

	interval() : min(+infinity), max(-infinity) {} // Default interval is empty

	interval(const float _min, const float _max) : min(_min), max(_max) {}

	bool contains(const float x) const
	{
		return min <= x && x <= max;
	}

	bool surrounds(const float x) const
	{
		return min < x && x < max;
	}

	float clamp(const float x) const
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	static const interval empty, universe;
};

const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);