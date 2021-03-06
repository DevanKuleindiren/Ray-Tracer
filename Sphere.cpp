//
// Created by Devan Kuleindiren on 15/01/2016.
//

#include "Sphere.h"
#include "IntersectionPoint.h"

IntersectionPoint Sphere::nearestIntersection (Ray<double> &ray) {

    // Calculate cooefficients of intersection quadratic
    Vector3D<double> rayPositionMinusCenter = ray.position - this->center;
    double a = (ray.direction * ray.direction);
    double b = (ray.direction * rayPositionMinusCenter) * 2;
    double c = (rayPositionMinusCenter * rayPositionMinusCenter) - (radius * radius);

    // Check discriminant
    double epsilon = 0.001;
    double discriminant = (b * b) - (4 * a * c);

    if (discriminant > epsilon) {
        // Two points of intersection, return the closest
        double x1 = (-b + sqrt(discriminant)) / (2 * a);
        double x2 = (-b - sqrt(discriminant)) / (2 * a);

        double result = (x1 < x2 ? x1 : x2);

        return (result < 0 ? IntersectionPoint(0, NULL, false) : IntersectionPoint(result, this, true));
    } else if (discriminant < -epsilon) {
        // No intersection
        return IntersectionPoint(0, NULL, false);
    } else {
        // One point of intersection
        double result = -b / (2 * a);
        return (result < 0 ? IntersectionPoint(0, NULL, false) : IntersectionPoint(-b / (2 * a), this, true));
    }
}

Vector3D<double> Sphere::getNormal(Vector3D<double> *fromPoint) {
    Vector3D<double> normal = *fromPoint - center;
    normal.normalise();

    return normal;
}