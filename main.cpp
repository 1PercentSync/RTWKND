#include <filesystem>
#include <fstream>
#include <iostream>
#include "color.h"
#include "vec3.h"

int main() {
    // Image
    constexpr int image_width = 256;
    constexpr int image_height = 256;

    // Write to project root
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
