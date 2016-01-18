#include <iostream>
#include <fstream>
#include <iomanip>
#include <float.h>
#include "IntersectionPoint.h"
#include "Light.h"

const int sceneWidth   = 500;                                               // Width of scene
const int sceneHeight  = 500;                                               // Height of scene
const int numberOfObjects = 2;                                             // Number of scene objects
const int numberOfLights = 1;                                               // Number of lights

const double xFieldOfView = M_PI_4;                                         // x field of view = pi / 2
const double yFieldOfView = (sceneHeight / sceneWidth) * xFieldOfView;      // y field of view

Vector3D<double> *backgroundColor = new Vector3D<double>(0, 0, 0);          // White background
SceneObject *objects[numberOfObjects];                                      // Scene objects
Light *lights[numberOfLights];                                              // Scene lights
Vector3D<double> *pixels[sceneHeight][sceneWidth];                          // Screen pixels

const double ka = 0.3;                                                      // Ambient reflection constant
const double kd = 0.5;                                                      // Diffuse reflection constant
const double ks = 0.3;                                                      // Specular reflection constant
const double shininess = 5;                                                 // nth power in specular reflection equation
const double kr = 0.5;                                                      // Reflection constant for each object

void populateScene () {
//    // Add objects
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++) {
//            for (int k = 0; k < 3; k++) {
//                std::cout << (i * 9) + (j * 3) + k << std::endl;
//                objects[(i * 9) + (j * 3) + k] =
//                        new Sphere(Vector3D<double>((i - 1) * 5, (j - 1) * 5, 25 + (k - 1) * 5),
//                                   2,
//                                   Vector3D<double>(51 + 68 * i, 51 + 68 * j, 51 + 68 * (2 - k)));
//            }
//        }
//    }
    objects[0] = new Sphere(Vector3D<double>(-2, 0, 16), 2, Vector3D<double>(255, 0, 0));
    objects[1] = new Sphere(Vector3D<double>(2, 0, 10), 2, Vector3D<double>(0, 255, 0));

    // Add lights
    lights[0] = new Light(Vector3D<double>(-10, -10, 20), Vector3D<double>(255, 255, 255));
}

int colourCap (double colourChannel) {
    if (colourChannel > 255.0) colourChannel = 255.0;
    if (colourChannel < 0.0) colourChannel = 0.0;
    return (int) round(colourChannel);
}

Vector3D<double> *trace (Ray<double> *ray);


Vector3D<double> *shadeObject (Vector3D<double> &intersectionPoint,
                               Vector3D<double> *rayDirection,
                               SceneObject &object) {

    Vector3D<double> viewingVector = *rayDirection * -1;
    Vector3D<double> objectNormal = object.getNormal(&intersectionPoint);

    Vector3D<double> ambient = object.colour * ka;
    Vector3D<double> diffuse = Vector3D<double>(0, 0, 0);
    Vector3D<double> specular = Vector3D<double>(0, 0, 0);

    // For each light, add appropriate diffuse and specular reflection
    for (Light *light : lights) {

        Vector3D<double> lightVector = light->position - intersectionPoint;
        double distanceToLight = lightVector.magnitude();
        lightVector.normalise();

        // Check whether the path to the light source is blocked
        Ray<double> rayToLight = Ray<double> (intersectionPoint, lightVector);
        IntersectionPoint intersectionTowardLight = object.nearestIntersection(rayToLight);

        // Check no objects block the light
        if (!(intersectionTowardLight.isIntersection && intersectionTowardLight.distance < distanceToLight)) {

            // Make light vector point at surface instead of light
            lightVector = lightVector * -1;

            // Precalculate the dot product between the light vector and object normal
            double lightVectorDotObjectNormal = lightVector * objectNormal;

            // If the light isn't behind the surface, add diffuse and specular reflection
            if (lightVectorDotObjectNormal > 0) {
                Vector3D<double> reflectedVector = (lightVector - (objectNormal * lightVectorDotObjectNormal * 2)).normalise();

                diffuse = diffuse + ((light->colour * (lightVectorDotObjectNormal)) * kd);
                specular = specular + ((light->colour * pow((reflectedVector * viewingVector * -1), shininess)) * ks);
            }

        }
    }

    // Calculate the reflected ray
    Vector3D<double> reflectedRayVector =
            (*rayDirection - (objectNormal * (*rayDirection * objectNormal) * 2)).normalise();
    Ray<double> *reflectedRay = new Ray<double> (intersectionPoint, reflectedRayVector);

    // Calculate the amount of reflection by recursing on the above ray
    Vector3D<double> reflections = *trace(reflectedRay) * kr;

    return new Vector3D<double>(ambient + diffuse + specular + reflections);
}


Vector3D<double> *trace (Ray<double> *ray) {
    // Find the closest intersection point
    IntersectionPoint closestPoint(DBL_MAX, NULL, false);
    for (SceneObject *object : objects) {
        IntersectionPoint intersectionPoint = object->nearestIntersection(*ray);
        if (intersectionPoint.isIntersection && intersectionPoint.distance < closestPoint.distance) {
            closestPoint = intersectionPoint;
        }
    }

    // If there was no intersection, return the background colour
    if (!closestPoint.isIntersection) return backgroundColor;

    // Otherwise, compute the intensity and apply that to the nearest object's colour
    Vector3D<double> intersectionPoint = ray->position + (ray->direction * closestPoint.distance);

    return shadeObject(intersectionPoint, &ray->direction, *closestPoint.sceneObject);
}


int main() {

    populateScene();

    double screenWidth  = 2 * tan(xFieldOfView / 2);
    double screenHeight = 2 * tan(yFieldOfView / 2);
    double xStep = screenWidth / sceneWidth;
    double yStep = screenHeight / sceneHeight;
    double x0 = -screenWidth / 2;
    double y0 = -screenHeight / 2;

    for (int row = 0; row < sceneHeight; row++) {
        for (int col = 0; col < sceneWidth; col++) {
            // Determine RAY
            Ray<double> *rayThroughPixel =
                    new Ray<double>(Vector3D<double>(0, 0, 0),
                                    Vector3D<double>(x0 + (col * xStep), y0 + (row * yStep), 1));

            pixels[row][col] = trace(rayThroughPixel);

            if (row == 240 && col == 245) {
                std::cout << row << ", " << col << std::endl;
            }

            delete rayThroughPixel;
        }
    }

    // Output image to file
    std::ofstream fileStream;
    fileStream.open("/Users/devankuleindiren/Documents/Work/University/Part IB/Computer Graphics and Image "
                            "Processing/Supervisions/Supervision 4/Ray Tracer/scene.ppm");
    fileStream << "P3" << std::endl;
    fileStream << sceneWidth << " " << sceneHeight << std::endl;
    fileStream << "255" << std::endl;
    for (int row = sceneHeight - 1; row >=0; row--) {
        for (int col = 0; col < sceneWidth; col++) {
            fileStream << std::setw(4) << colourCap(pixels[row][col]->x);
            fileStream << std::setw(4) << colourCap(pixels[row][col]->y);
            fileStream << std::setw(4) << colourCap(pixels[row][col]->z);
        }
        fileStream << std::endl;
    }

    // Close the file
    fileStream.close();
    return 0;
}