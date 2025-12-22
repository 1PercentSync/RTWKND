#include <filesystem>
#include <fstream>
#include <iostream>
#include "color.h"
#include "vec3.h"

int main() {
    // Image
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 400;

    // Calculate the image height and ensure that it's at least 1.
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Viewport widths less than one are ok since they are really valued.
    constexpr auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);

    // Write to the project root
    const std::filesystem::path out_path = std::filesystem::current_path().parent_path() / "output.ppm";
    std::ofstream out(out_path);
    if (!out) {
        std::cerr << "Open failed: " << out_path << '\n';
        return 1;
    }

    // Render
    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++) {
        std::clog << "Scanline remaining: " << (image_height - j) << '\n' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_color = color(static_cast<double>(i) / (image_width - 1),
                                     static_cast<double>(j) / (image_height - 1), 0);
            write_color(out, pixel_color);
        }
    }

    std::clog << "Done.\n";
}
