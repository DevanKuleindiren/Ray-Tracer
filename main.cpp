#include <iostream>
#include <fstream>
#include <iomanip>
#include <float.h>
#include "IntersectionPoint.h"
#include "Light.h"

const int sceneWidth   = 500;                                               // Width of scene
const int sceneHeight  = 500;                                               // Height of scene
const int numberOfObjects = 1;                                              // Number of scene objects
const int numberOfLights = 1;                                               // Number of lights

const double xFieldOfView = M_PI_4;                                         // x field of view = pi / 2
const double yFieldOfView = (sceneHeight / sceneWidth) * xFieldOfView;      // y field of view

Vector3D<double> *backgroundColor = new Vector3D<double>(255, 255, 255);    // White background
SceneObject *objects[numberOfObjects];                                      // Scene objects
Light *lights[numberOfLights];                                              // Scene lights
Vector3D<double> *pixels[sceneHeight][sceneWidth];                          // Screen pixels

const double ka = 0.3;                                                      // Ambient reflection constant
const double kd = 0.5;                                                      // Diffuse reflection constant
const double ks = 0.3;                                                      // Specular reflection constant
const double shininess = 5;                                                 // nth power in specular reflection equation

void populateScene () {
    // Add objects
    objects[0] = new Sphere(Vector3D<double>(0, 0, 10), 2, Vector3D<double>(255, 0, 0));

    // Add lights
    lights[0] = new Light(Vector3D<double>(-10, -10, 10), Vector3D<double>(255, 255, 255));
}

int colourCap (double colourChannel) {
    if (colourChannel > 255.0) colourChannel = 255.0;
    if (colourChannel < 0.0) colourChannel = 0.0;
    return (int) round(colourChannel);
}


Vector3D<double> *trace (Ray<double> &ray) {
    // Find the closest intersection point
    IntersectionPoint closestPoint(DBL_MAX, NULL, false);
    for (SceneObject *object : objects) {
        IntersectionPoint intersectionPoint = object->nearestIntersection(ray);
        if (intersectionPoint.isIntersection && intersectionPoint.distance < closestPoint.distance) {
            closestPoint = intersectionPoint;
        }
    }

    // If there was no intersection, return the background colour
    if (!closestPoint.isIntersection) return backgroundColor;

    // Otherwise, compute the intensity and apply that to the nearest object's colour
    Vector3D<double> intersectionPoint = ray.position + (ray.direction * closestPoint.distance);
    Vector3D<double> objectNormal = closestPoint.sceneObject->getNormal(&intersectionPoint);
    Vector3D<double> viewingVector = ray.direction * -1;

    Vector3D<double> diffuse = Vector3D<double>(0, 0, 0);
    Vector3D<double> specular = Vector3D<double>(0, 0, 0);
    for (Light *light : lights) {
        Vector3D<double> lightVector = (intersectionPoint - light->position).normalise();
        double lightVectorDotObjectNormal = lightVector * objectNormal;

        if (lightVectorDotObjectNormal > 0) {
            Vector3D<double> reflectedVector = (lightVector - (objectNormal * lightVectorDotObjectNormal * 2)).normalise();

            diffuse = diffuse + ((light->colour * (lightVectorDotObjectNormal)) * kd);
            specular = specular + ((light->colour * pow((reflectedVector * viewingVector * -1), shininess)) * ks);
        }
    }

    return new Vector3D<double>((closestPoint.sceneObject->colour * ka) + diffuse + specular);
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
            Ray<double> rayThroughPixel =
                    Ray<double>(Vector3D<double>(0, 0, 0),
                                Vector3D<double>(x0 + (col * xStep), y0 + (row * yStep), 1));

            pixels[row][col] = trace(rayThroughPixel);
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