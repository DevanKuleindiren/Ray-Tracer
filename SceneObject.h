//
// Created by Devan Kuleindiren on 15/01/2016.
//

#ifndef RAY_TRACER_SCENEOBJECT_H
#define RAY_TRACER_SCENEOBJECT_H

#include "Vector3D.cpp"
#include "Ray.cpp"
#include "Colour.h"
#include "IntersectionPoint.h"

class SceneObject {
protected:
    Colour *colour;
public:
    SceneObject (Colour *colour) : colour(colour) {}
    virtual IntersectionPoint nearestIntersection (Ray<double> ray) = 0;
    virtual ~SceneObject ();
};


#endif //RAY_TRACER_SCENEOBJECT_H
