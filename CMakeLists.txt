cmake_minimum_required(VERSION 3.3)
project(Ray_Tracer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Vector3D.cpp Ray.cpp SceneObject.cpp SceneObject.h Sphere.cpp Sphere.h Colour.h IntersectionPoint.cpp IntersectionPoint.h Colour.cpp)
add_executable(Ray_Tracer ${SOURCE_FILES})