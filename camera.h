#ifndef RTWKND_CAMERA_H
#define RTWKND_CAMERA_H

#include <filesystem>
#include <fstream>
#include "hittable.h"
#include "vec3.h"

class camera {
public:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 10;
    int max_depth = 10;

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

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = point3(0, 0, 0);

        pixel_samples_scale = 1.0 / samples_per_pixel;

        // Determine viewport dimensions.
        constexpr auto focal_length = 1.0;
        constexpr auto viewport_height = 2.0;
        const auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        const auto viewport_u = vec3(viewport_width, 0, 0);
        const auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        const auto viewport_upper_left =
                center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    [[nodiscard]] ray get_ray(const int i, const int j) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        const auto offset = sample_square();
        const auto pixel_sample = pixel00_loc
                                  + ((i + offset.x()) * pixel_delta_u)
                                  + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return {ray_origin, ray_direction};
    }

    static vec3 sample_square() {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return {random_double() - 0.5, random_double() - 0.5, 0};
    }

    static color ray_color(const ray &r, int depth, const hittable &world) {
        if (depth <= 0)
            return {0, 0, 0};

        if (hit_record rec; world.hit(r, interval(0.001, infinity), rec)) {
            const vec3 direction = rec.normal + random_unit_vector();
            return 0.5 * ray_color(ray(rec.p, direction), depth - 1, world);
        }

        const vec3 unit_direction = unit_vector(r.direction());
        const auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif //RTWKND_CAMERA_H
