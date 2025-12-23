#ifndef RTWKND_CAMERA_H
#define RTWKND_CAMERA_H

#include <filesystem>
#include <fstream>
#include "hittable.h"
#include "material.h"
#include "vec3.h"

class camera {
public:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 10;
    int max_depth = 10;

    double vfov = 90;
    point3 lookfrom = point3(0, 0, 0); // Point camera is looking from
    point3 lookat = point3(0, 0, -1); // Point camera is looking at
    vec3 vup = vec3(0, 1, 0); // Camera-relative "up" direction

    double defocus_angle = 0; // Variation angle of rays through each pixel
    double focus_dist = 10; // Distance from the camera lookfrom point to plane of perfect focus

    void render(const hittable &world) {
        initialize();

        // Write to the project root
        const std::filesystem::path out_path = std::filesystem::current_path().parent_path() / "output.ppm";
        std::ofstream out(out_path);
        if (!out) {
            std::cerr << "Open failed: " << out_path << '\n';
            return;
        }

        // Render
        out << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        for (int j = 0; j < image_height; j++) {
            std::clog << "Scanline remaining: " << (image_height - j) << '\n' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(out, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "Done.\n";
    }

private:
    int image_height = 1;
    double pixel_samples_scale = 0.1;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;
    vec3 defocus_disk_u; // Defocus disk horizontal radius
    vec3 defocus_disk_v; // Defocus disk vertical radius

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        // Determine viewport dimensions.
        const auto theta = degrees_to_radians(vfov);
        const auto h = std::tan(theta / 2);
        const auto viewport_height = 2 * h * focus_dist;
        const auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        const vec3 viewport_u = viewport_width * u; // Vector across viewport horizontal edge
        const vec3 viewport_v = viewport_height * -v; // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    [[nodiscard]] ray get_ray(const int i, const int j) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        const auto offset = sample_square();
        const auto pixel_sample = pixel00_loc
                                  + ((i + offset.x()) * pixel_delta_u)
                                  + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return {ray_origin, ray_direction};
    }

    static vec3 sample_square() {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return {random_double() - 0.5, random_double() - 0.5, 0};
    }

    [[nodiscard]] point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    static color ray_color(const ray &r, const int depth, const hittable &world) {
        if (depth <= 0)
            return {0, 0, 0};

        if (hit_record rec; world.hit(r, interval(0.001, infinity), rec)) {
            color attenuation;
            if (ray scattered; rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return {0, 0, 0};
        }

        const vec3 unit_direction = unit_vector(r.direction());
        const auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif //RTWKND_CAMERA_H
