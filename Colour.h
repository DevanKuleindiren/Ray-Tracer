//
// Created by Devan Kuleindiren on 15/01/2016.
//

#ifndef RAY_TRACER_COLOUR_H
#define RAY_TRACER_COLOUR_H

#include "Vector3D.cpp"

class Colour : public Vector3D<int> {
public:
    Colour (int r, int g, int b) : r(r), g(g), b(b) {}
    int r;
    int g;
    int b;
    void legalise ();
};

#endif //RAY_TRACER_COLOUR_H
