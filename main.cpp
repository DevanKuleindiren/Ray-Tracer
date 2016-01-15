#include <iostream>
#include <fstream>
#include <iomanip>
#include <float.h>
#include "IntersectionPoint.h"

Colour getColour (int row, int col) {
    Colour colour = {row % 255, (row % 255), (row % 255)};
    return colour;
}

int main() {

    const int width   = 500;        // Width of screen
    const int height  = 500;        // Height of screen
    const int numberOfObjects = 1;  // Number of scene objects
    const int numberOfLights = 1;   // Number of lights

    Colour *backgroundColor = new Colour (255, 255, 255);   // White background

    // Creating an array of objects
    SceneObject *objects[numberOfObjects];
    objects[0] = new Sphere(Vector3D<double>(0, 0, 20), 5, new Colour(255, 0, 0));


    Colour *pixels[height][width];

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // Determine RAY
            Ray<double> rayThroughPixel = Ray<double>(Vector3D<double>(0, 0, 0),
                                      Vector3D<double>(col - (width / 2), row - (height / 2), width / 2));

            IntersectionPoint closestPoint(DBL_MAX, NULL, false);
            for (SceneObject *object : objects) {
                IntersectionPoint intersectionPoint = object->nearestIntersection(rayThroughPixel);
                if (intersectionPoint.isIntersection && intersectionPoint.distance < closestPoint.distance) {
                    closestPoint = intersectionPoint;
                }
            }

            pixels[row][col] = (closestPoint.isIntersection ? closestPoint.sceneObject->colour : backgroundColor);
            if (row == 250 && col == 250) {
                std::cout << "A: " << pixels[row][col]->r << ", " << pixels[row][col]->g << ", " << pixels[row][col]->b
                                   << std::endl;
            }
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

            if (row == 250 && col == 250) {
                std::cout << "B: " << pixels[row][col]->r << ", " << pixels[row][col]->g << ", " << pixels[row][col]->b
                                   << std::endl;
            }

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

    fileStream.close();
    return 0;
}