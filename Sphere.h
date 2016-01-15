//
// Created by Devan Kuleindiren on 15/01/2016.
//

#ifndef RAY_TRACER_SPHERE_H
#define RAY_TRACER_SPHERE_H

#include "SceneObject.h"
#include "Math.h"

class Sphere : public SceneObject {
private:
    Vector3D<double> center;
    double radius;
public:
    Sphere (Vector3D<double> center, double radius, Colour *colour) : SceneObject(colour), center(center), radius
            (radius) {}
    IntersectionPoint nearestIntersection (Ray<double> ray);
};


#endif //RAY_TRACER_SPHERE_H
