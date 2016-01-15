#include <iostream>
#include <fstream>
#include <iomanip>
#include <float.h>
#include "IntersectionPoint.h"

const int width   = 500;                                // Width of screen
const int height  = 500;                                // Height of screen
const int numberOfObjects = 1;                          // Number of scene objects
const int numberOfLights = 1;                           // Number of lights

Colour *backgroundColor = new Colour (255, 255, 255);   // White background
SceneObject *objects[numberOfObjects];                  // Scene objects
Vector3D<double> *lights[numberOfLights];               // Scene lights
Colour *pixels[height][width];                          // Screen pixels

const double ka = 0.3;                                  // Ambient reflection constant
const double kd = 0.5;                                  // Diffuse reflection constant
const double ks = 0.3;                                  // Specular reflection constant
const double shininess = 5;                             // The power n in the specular reflection equation

void populateScene () {
    // Add objects
    objects[0] = new Sphere(Vector3D<double>(0, 0, 20), 5, new Colour(255, 0, 0));

    // Add lights
    lights[0] = new Vector3D<double>(10, 10, 5);
}


Colour *trace (Ray<double> ray) {
    // Find the closest intersection point
    IntersectionPoint closestPoint(DBL_MAX, NULL, false);
    for (SceneObject *object : objects) {
        IntersectionPoint intersectionPoint = object->nearestIntersection(ray);
        if (intersectionPoint.isIntersection && intersectionPoint.distance < closestPoint.distance) {
            closestPoint = intersectionPoint;
        }
    }

    if (!closestPoint.isIntersection) return backgroundColor;

    return closestPoint.sceneObject->colour;
}


int main() {
    populateScene();

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // Determine RAY
            Ray<double> rayThroughPixel = Ray<double>(Vector3D<double>(0, 0, 0),
                                      Vector3D<double>(col - (width / 2), row - (height / 2), width / 2));

            pixels[row][col] = trace(rayThroughPixel);
        }
    }

    // Output image to file
    std::ofstream fileStream;
    fileStream.open("/Users/devankuleindiren/Documents/Work/University/Part IB/Computer Graphics and Image Processing/Supervisions/Supervision 4/Ray Tracer/test.ppm");
    fileStream << "P3" << std::endl;
    fileStream << width << " " << height << std::endl;
    fileStream << "255" << std::endl;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            fileStream << std::setw(4) << pixels[row][col]->r;
            fileStream << std::setw(4) << pixels[row][col]->g;
            fileStream << std::setw(4) << pixels[row][col]->b;
        }
        fileStream << std::endl;
    }

    // Clear up memory
    delete backgroundColor;
    for (SceneObject *object : objects) {
        delete object;
    }

    // Close the file
    fileStream.close();
    return 0;
}