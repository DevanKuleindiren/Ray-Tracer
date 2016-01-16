//
// Created by Devan Kuleindiren on 15/01/2016.
//

#ifndef RAY_TRACER_LIGHT_H
#define RAY_TRACER_LIGHT_H


#include "Colour.h"

class Light {
public:
    Light (Vector3D<double> position, Colour *colour) : position(position), colour(colour) {}
    Vector3D<double> position;
    Colour *colour;
};


#endif //RAY_TRACER_LIGHT_H
