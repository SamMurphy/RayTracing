#pragma once
#include "utilities.h"
#include "colour.h"
#include "hittable.h"

#define DEFAULT_IMAGE_WIDTH 1200
#define DEFAULT_ASPECT_RATIO 16.0f / 9.0f
#define DEFAULT_CHANNELS 3
#define SKY_COLOUR colour(0.5f, 0.7f, 1.0f)
#define LAMBERT_REFLECTIONS 1
#define VISUALIZE_NORMALS 0
//#define SKY_COLOUR colour(1.0f, 0.46f, 0.0f)

class camera
{
public:
	camera() { initialize(); }

	void render(const hittable& world);

private:
	void initialize();

	colour ray_colour(const Ray& r, int depth, const hittable& world) const;

	colour sky_box(const Ray& r) const;

	Ray get_ray(int x, int y) const;
	vec3 pixel_sample_square() const;

	point3 defocus_disk_sample() const;

public:
	float aspect_ratio;
	int image_width;
	int image_height;

	int channels;
	int stride;

	float focal_length;
	point3 camera_centre;

	float vfov = 90.0f;
	point3 lookFrom;
	point3 lookAt;
	vec3 viewUp;

	float defocus_angle; // Variation angle of rays through each pixel
	float focus_dist; // Distance from camera lookfrom point to plane of perfect focus

	int samples_per_pixel;
	int max_depth;

	float viewport_height;
	float viewport_width;

private:
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	point3 top_left_pixel;
	vec3   u, v, w;        // Camera frame basis vectors
	vec3 defocus_disk_u; // Defocus disk horizontal radius
	vec3 defocus_disk_v; // Defocus disk vertical radius
};

