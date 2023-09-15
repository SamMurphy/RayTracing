#include "camera.h"

#include <shlobj.h>
#include <shellapi.h>


#include "interval.h"
#include "material.h"
#include "stb/stb_image_write.h"


void camera::initialize()
{
    // Camera parameters
    vfov = 20.0f;
    lookFrom = point3(13,2,3);
    lookAt = point3(0,0,0);
    viewUp = vec3(0, 1, 0);

    defocus_angle = 0.6f;
    focus_dist = 10.0f;

    camera_centre = lookFrom;
    focal_length = (lookFrom - lookAt).length();

    float theta = degrees_to_radians(vfov);
    float h = tan(theta / 2);

    samples_per_pixel = 10;
    max_depth = 50;

    // Image parameters
    aspect_ratio = DEFAULT_ASPECT_RATIO;
    image_width = DEFAULT_IMAGE_WIDTH;
    image_height = static_cast<int>(image_width / aspect_ratio);
    channels = DEFAULT_CHANNELS;
    stride = image_width * channels;

    // Viewport parameters
    viewport_height = 2.0f * h * focus_dist;
    viewport_width = viewport_height * (static_cast<float>(image_width) / image_height);

    // Calculate camera basis
    w = unit_vector(lookFrom - lookAt);
    u = unit_vector(cross(viewUp, w));
    v = cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = viewport_width * u;
    vec3 viewport_v = viewport_height * -v;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u = viewport_u / static_cast<float>(image_width);
    pixel_delta_v = viewport_v / static_cast<float>(image_height);

    // Calculate the location of the upper left pixel
    point3 viewport_upper_left = camera_centre - (focal_length * w) - viewport_u / 2.0f - viewport_v / 2.0f;
    top_left_pixel = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

    // Calculate the camera defocus disk basis vectors
    float defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2.0f));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

void camera::render(const hittable& world)
{
    initialize(); // re-init the camera in case anything has changed.

    // Render Loop
    const auto pixels = new uint8_t[image_width * image_height * channels];
    int index = 0;
    for (int y = 0; y < image_height; ++y)
    {
        std::clog << "\rScanlines remaining: " << (image_height - y) << " " << std::flush;
        for (int x = 0; x < image_width; ++x)
        {
            colour pixel_colour(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; ++sample)
            {
                ray r = get_ray(x, y);
                pixel_colour += ray_colour(r, max_depth, world);
            }

            colour converted_colour = convert_colour(pixel_colour, samples_per_pixel);

            pixels[index++] = static_cast<uint8_t>(converted_colour.R());
            pixels[index++] = static_cast<uint8_t>(converted_colour.G());
            pixels[index++] = static_cast<uint8_t>(converted_colour.B());
        }
    }
    std::clog << "\rDone.                           \n";

    stbi_write_png("output.png", image_width, image_height, channels, pixels, stride);
    delete[] pixels;

    ShellExecute(0, 0, L"output.png", 0, 0, SW_SHOW);
}

colour camera::ray_colour(const ray& r, int depth, const hittable& world) const
{
    // If exceeded the bound limit, gather no more light
    if (depth <= 0)
        return colour(0, 0, 0);

    hit_record hit;

    if (world.hit(r, interval(0.001f, infinity), hit))
    {
#if VISUALIZE_NORMALS
        colour colourized_normal = 0.5f * (hit.normal + 1.0f);
        return colourized_normal;
#endif
        ray scattered;
        colour attenuation;
        if (hit.mat->scatter(r, hit, attenuation, scattered))
        {
            return attenuation * ray_colour(scattered, depth - 1, world);
        }
        return colour(0, 0, 0);
    }

    return sky_box(r);
}

colour camera::sky_box(const ray& r) const
{
    vec3 unit_direction = unit_vector(r.direction());
    float a = 0.5f * (unit_direction.Y() + 1.0f);
    return (1.0f - a) * colour(1.0f, 1.0f, 1.0f) + a * SKY_COLOUR;
}

ray camera::get_ray(int x, int y) const
{
    // Get a randomly sampled camera ray for the pixel at location x,y originating from the camera defocus disk.

    point3 pixel_centre = top_left_pixel 
        + (static_cast<float>(x) * pixel_delta_u) 
        + (static_cast<float>(y) * pixel_delta_v);
    point3 pixel_sample = pixel_centre + pixel_sample_square();

    point3 ray_origin = (defocus_angle <= 0) ? camera_centre : defocus_disk_sample();
    vec3 ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

vec3 camera::pixel_sample_square() const
{
    // Random point in a square surrounding a pixel at the origin
    // TODO: Investigate pixel_sample_disk() and random_in_unit_disk()

    float px = -0.5f + random_float();
    float py = -0.5f + random_float();

    return (px * pixel_delta_u) + (py * pixel_delta_v);
}

point3 camera::defocus_disk_sample() const
{
    point3 p = random_in_unit_disk();
    return camera_centre + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}
