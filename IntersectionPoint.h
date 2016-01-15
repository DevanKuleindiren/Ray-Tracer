//
// Created by Devan Kuleindiren on 15/01/2016.
//

#ifndef RAY_TRACER_INTERSECTIONPOINT_H
#define RAY_TRACER_INTERSECTIONPOINT_H

#include "Colour.h"

class IntersectionPoint {
public:
    bool isIntersection;
    double distance;
    Colour *colour;
    IntersectionPoint (double distance, Colour *colour, bool isIntersection) : distance(distance), colour(colour),
                                                                              isIntersection(isIntersection) {}
};


#endif //RAY_TRACER_INTERSECTIONPOINT_H
