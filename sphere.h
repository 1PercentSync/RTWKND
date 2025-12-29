#ifndef RTWKND_SPHERE_H
#define RTWKND_SPHERE_H

#include <utility>

#include "hittable.h"

class sphere : public hittable {
public:
    // Stationary Sphere
    sphere(const point3 &static_center, const double radius, shared_ptr<material> mat)
        : center(static_center, vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(std::move(mat)) {
    }

    // Moving Sphere
    sphere(const point3 &center1, const point3 &center2, const double radius,
           shared_ptr<material> mat)
        : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(std::move(mat)) {
    }

    bool hit(const ray &r, const interval ray_t, hit_record &rec) const override {
        const point3 current_center = center.at(r.time());
        const vec3 oc = current_center - r.origin();
        const auto a = r.direction().length_squared();
        const auto h = dot(r.direction(), oc);
        const auto c = oc.length_squared() - radius * radius;

        const auto discriminant = h * h - a * c;
        if (discriminant < 0) {
            return false;
        }
        const auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        const vec3 outward_normal = (rec.p - current_center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

private:
    ray center;
    double radius;
    shared_ptr<material> mat;
};

#endif //RTWKND_SPHERE_H
