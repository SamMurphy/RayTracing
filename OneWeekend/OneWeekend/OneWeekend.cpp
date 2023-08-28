// OneWeekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma once
#include "utilities.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

int main()
{
    //// Materials
    //auto lambert_mat_orange = make_shared<lambertian>(colour(0.1f, 0.2f, 0.5f));
    //auto lambert_mat_ground = make_shared<lambertian>(colour(0.2f, 0.6f, 0.1f));
    ////auto material_left = make_shared<metal>(colour(0.8f, 0.8f, 0.8f), 0.3f);
    //auto material_left = make_shared<dielectric>(1.5f);
    //auto material_right = make_shared<metal>(colour(0.8f, 0.6f, 0.2f), 0.0f);

    //// World
    //hittable_list world;
    //world.add(make_shared<sphere>(point3( 0.0f,-100.5f, -1.0f), 100.0f, lambert_mat_ground));
    //world.add(make_shared<sphere>(point3( 0.0f,   0.0f, -1.0f), 0.5f, lambert_mat_orange));
    //world.add(make_shared<sphere>(point3(-1.0f,   0.0f, -1.0f), -0.4f, material_left));
    //world.add(make_shared<sphere>(point3( 1.0f,   0.0f, -1.0f), 0.5f, material_right));


    hittable_list world;

    auto ground_material = make_shared<lambertian>(colour(0.5f, 0.5f, 0.5f));
    world.add(make_shared<sphere>(point3(0.0f, -1000.0f, 0.0f), 1000.0f, ground_material));

    for (int a = -11; a < 11; a++) 
    {
        for (int b = -11; b < 11; b++) 
        {
            auto choose_mat = random_float();
            point3 center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

            if ((center - point3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8f) 
                {
                    // diffuse
                    auto albedo = colour::random() * colour::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95f) 
                {
                    // metal
                    auto albedo = colour::random(0.5f, 1.0f);
                    auto fuzz = random_float(0.0f, 0.5f);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2f, sphere_material));
                }
                else 
                {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5f);
                    world.add(make_shared<sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5f);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0f, material1));

    auto material2 = make_shared<lambertian>(colour(0.4f, 0.2f, 0.1f));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0f, material2));

    auto material3 = make_shared<metal>(colour(0.7f, 0.6f, 0.5f), 0.0f);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0f, material3));

    // Camera
    camera cam;
    cam.render(world);
}