#ifndef RTWKND_TEXTURE_H
#define RTWKND_TEXTURE_H

#include <utility>

class texture {
public:
    virtual ~texture() = default;

    [[nodiscard]] virtual color value(double u, double v, const point3 &p) const = 0;
};

class solid_color : public texture {
public:
    explicit solid_color(const color &albedo) : albedo(albedo) {
    }

    solid_color(const double red, const double green, const double blue) : solid_color(color(red, green, blue)) {
    }

    [[nodiscard]] color value(double u, double v, const point3 &p) const override {
        return albedo;
    }

private:
    color albedo;
};

class checker_texture : public texture {
public:
    checker_texture(const double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
        : inv_scale(1.0 / scale), even(std::move(even)), odd(std::move(odd)) {
    }

    checker_texture(const double scale, const color &c1, const color &c2)
        : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {
    }

    [[nodiscard]] color value(const double u, const double v, const point3 &p) const override {
        const auto xInteger = static_cast<int>(std::floor(inv_scale * p.x()));
        const auto yInteger = static_cast<int>(std::floor(inv_scale * p.y()));
        const auto zInteger = static_cast<int>(std::floor(inv_scale * p.z()));

        const bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

#endif //RTWKND_TEXTURE_H
