//
// Created by Devan Kuleindiren on 15/01/2016.
//

#ifndef RAY_TRACER_SCENEOBJECT_H
#define RAY_TRACER_SCENEOBJECT_H

#include "Ray.cpp"

class IntersectionPoint;

class SceneObject {
public:
    Vector3D<double> colour;
    SceneObject (Vector3D<double> colour) : colour(colour) {}
    virtual IntersectionPoint nearestIntersection (Ray<double> &ray) = 0;
    virtual Vector3D<double> getNormal (Vector3D<double> *fromPoint) = 0;
};


#endif //RAY_TRACER_SCENEOBJECT_H
