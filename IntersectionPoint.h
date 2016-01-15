//
// Created by Devan Kuleindiren on 15/01/2016.
//

#ifndef RAY_TRACER_INTERSECTIONPOINT_H
#define RAY_TRACER_INTERSECTIONPOINT_H

#include "Sphere.h"

class IntersectionPoint {
public:
    bool isIntersection;
    double distance;
    SceneObject *sceneObject;
    IntersectionPoint (double distance, SceneObject *sceneObject, bool isIntersection) :
            distance(distance), sceneObject(sceneObject), isIntersection(isIntersection) {}
};


#endif //RAY_TRACER_INTERSECTIONPOINT_H
