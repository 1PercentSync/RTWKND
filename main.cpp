#include <filesystem>
#include <fstream>
#include <iostream>

int main() {
    // Image
    constexpr int image_width = 256;
    constexpr int image_height = 256;

    // Render
    const std::filesystem::path out_path = std::filesystem::current_path().parent_path() / "output.ppm";
    std::ofstream out(out_path);
    if (!out) {
        std::cerr << "Open failed: " << out_path << '\n';
        return 1;
    }

    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            const double r = static_cast<double>(i) / (image_width - 1);
            const double g = static_cast<double>(j) / (image_height - 1);
            constexpr double b = 0.0;

            const int ir = static_cast<int>(255.999 * r);
            const int ig = static_cast<int>(255.999 * g);
            constexpr int ib = static_cast<int>(255.999 * b);

            out << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}
