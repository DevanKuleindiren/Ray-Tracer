#include <iostream>
#include <fstream>
#include <iomanip>
#include <float.h>
#include "IntersectionPoint.h"
#include "Light.h"

const int width   = 500;                                                    // Width of screen
const int height  = 500;                                                    // Height of screen
const int numberOfObjects = 1;                                              // Number of scene objects
const int numberOfLights = 1;                                               // Number of lights

Vector3D<double> *backgroundColor = new Vector3D<double>(255, 255, 255);    // White background
SceneObject *objects[numberOfObjects];                                      // Scene objects
Light *lights[numberOfLights];                                              // Scene lights
Vector3D<double> *pixels[height][width];                                    // Screen pixels

const double ka = 0.3;                                                      // Ambient reflection constant
const double kd = 0.5;                                                      // Diffuse reflection constant
const double ks = 0.3;                                                      // Specular reflection constant
const double shininess = 5;                                                 // nth power in specular reflection equation

void populateScene () {
    // Add objects
    objects[0] = new Sphere(Vector3D<double>(0, 0, 20), 5, Vector3D<double>(255, 0, 0));

    // Add lights
    lights[0] = new Light(Vector3D<double>(10, 10, 10), Vector3D<double>(255, 255, 255));
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
    fileStream.open("/Users/devankuleindiren/Documents/Work/University/Part IB/Computer Graphics and Image "
                            "Processing/Supervisions/Supervision 4/Ray Tracer/scene.ppm");
    fileStream << "P3" << std::endl;
    fileStream << width << " " << height << std::endl;
    fileStream << "255" << std::endl;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
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