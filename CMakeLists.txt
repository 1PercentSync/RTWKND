cmake_minimum_required(VERSION 4.1)
project(RTWKND)

set(CMAKE_CXX_STANDARD 23)

add_executable(RTWKND main.cpp
        vec3.h
        color.h
        ray.h
        hittable.h
        sphere.h)
