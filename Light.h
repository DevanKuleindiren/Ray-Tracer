//
// Created by Devan Kuleindiren on 15/01/2016.
//

#ifndef RAY_TRACER_LIGHT_H
#define RAY_TRACER_LIGHT_H

#include "Colour.h"
#include "Vector3D.cpp"

class Light {
public:
    Light (Vector3D<double> position, Vector3D<double> colour) : position(position), colour(colour) {}
    Vector3D<double> position;
    Vector3D<double> colour;
};


#endif //RAY_TRACER_LIGHT_H
