cmake_minimum_required(VERSION 4.1)
project(RTWKND)

set(CMAKE_CXX_STANDARD 23)

# Enable OpenMP for multi-threading
find_package(OpenMP REQUIRED)

add_executable(RTWKND main.cpp
        vec3.h
        color.h
        ray.h
        hittable.h
        sphere.h
        hittable_list.h
        rtweekend.h
        interval.h
        camera.h
        material.h
        aabb.h
        bvh.h
        texture.h)

# Link OpenMP
target_link_libraries(RTWKND PRIVATE OpenMP::OpenMP_CXX)
