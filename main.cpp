#include <iostream>
#include <fstream>
#include <iomanip>
#include <float.h>
#include "IntersectionPoint.h"
#include "Light.h"

#define RECURSION_LIMIT 1

#define SCENE_WIDTH     500                                                 // Width of scene
#define SCENE_HEIGHT    500                                                 // Height of scene
#define NUM_OBJECTS     27                                                  // Number of scene objects
#define NUM_LIGHTS      1                                                   // Number of lights

#define KA              0.3                                                 // Ambient reflection constant
#define KD              0.5                                                 // Diffuse reflection constant
#define KS              0.3                                                 // Specular reflection constant
#define SHININESS       5                                                   // nth power in specular reflection equation
#define KR              0.8                                                 // Reflection constant for each object

const double xFieldOfView = M_PI_4;                                         // x field of view = pi / 2
const double yFieldOfView = (SCENE_HEIGHT / SCENE_WIDTH) * xFieldOfView;    // y field of view

Vector3D<double>    *backgroundColor        = new Vector3D<double>(0, 0, 0);// White background
SceneObject         *objects[NUM_OBJECTS];                                  // Scene objects
Light               *lights[NUM_LIGHTS];                                    // Scene lights
Vector3D<double>    *pixels[SCENE_HEIGHT][SCENE_WIDTH];                     // Screen pixels

void populateScene () {
    // Add objects
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                objects[(i * 9) + (j * 3) + k] =
                        new Sphere(Vector3D<double>((i - 1) * 5, (j - 1) * 5, 25 + (k - 1) * 5),
                                   2,
                                   Vector3D<double>(51 + 68 * i, 51 + 68 * j, 51 + 68 * (2 - k)));
            }
        }
    }

    // Add lights
    lights[0] = new Light(Vector3D<double>(0, 10, 0), Vector3D<double>(255, 255, 255));
}

int colourCap (double colourChannel) {
    if (colourChannel > 255.0) colourChannel = 255.0;
    if (colourChannel < 0.0) colourChannel = 0.0;
    return (int) round(colourChannel);
}

Vector3D<double> *trace (Ray<double> *ray, int recursionInvocationNumber);


Vector3D<double> *shadeObject (Vector3D<double> &intersectionPoint,
                               Vector3D<double> *rayDirection,
                               SceneObject &object,
                               int recursionInvocationNumber) {

    Vector3D<double> viewingVector = *rayDirection * -1;
    Vector3D<double> objectNormal = object.getNormal(&intersectionPoint);

    Vector3D<double> ambient = object.colour * KA;
    Vector3D<double> diffuse = Vector3D<double>(0, 0, 0);
    Vector3D<double> specular = Vector3D<double>(0, 0, 0);

    // For each light, add appropriate diffuse and specular reflection
    for (Light *light : lights) {

        Vector3D<double> lightVector = light->position - intersectionPoint;
        double distanceToLight = lightVector.magnitude();
        lightVector.normalise();

        // Check whether the path to the light source is blocked
        Ray<double> rayToLight = Ray<double> (intersectionPoint, lightVector);

        // Find the closest intersection point
        IntersectionPoint closestPoint(DBL_MAX, NULL, false);
        for (SceneObject *obj : objects) {
            IntersectionPoint intersectionPt = obj->nearestIntersection(rayToLight);
            if (intersectionPt.isIntersection && intersectionPt.distance < closestPoint.distance) {
                closestPoint = intersectionPt;
            }
        }

        // Check no objects block the light
        if (!(closestPoint.isIntersection && closestPoint.distance < distanceToLight)) {

            // Precalculate the dot product between the light vector and object normal
            double lightVectorDotObjectNormal = lightVector * objectNormal;

            // If the light isn't behind the surface, add diffuse and specular reflection
            if (lightVectorDotObjectNormal > 0) {
                Vector3D<double> reflectedVector = (lightVector - (objectNormal * lightVectorDotObjectNormal * 2)).normalise();

                diffuse = diffuse + ((light->colour * (lightVectorDotObjectNormal)) * KD);
                specular = specular + ((light->colour * pow((reflectedVector * viewingVector * -1), SHININESS)) * KS);
            }

        }
    }

    // Calculate the reflected ray
    Vector3D<double> reflectedRayVector =
            (*rayDirection - (objectNormal * (*rayDirection * objectNormal) * 2)).normalise();
    Ray<double> *reflectedRay = new Ray<double> (intersectionPoint, reflectedRayVector);

    // Calculate the amount of reflection by recursing on the above ray
    Vector3D<double> reflections = *trace(reflectedRay, recursionInvocationNumber + 1) * KR;

    return new Vector3D<double>(ambient + diffuse + specular + reflections);
}


Vector3D<double> *trace (Ray<double> *ray, int recursionInvocationNumber) {
    if (recursionInvocationNumber > RECURSION_LIMIT) return backgroundColor;

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

    return shadeObject(intersectionPoint, &ray->direction, *closestPoint.sceneObject, recursionInvocationNumber);
}


int main() {

    populateScene();

    double screenWidth  = 2 * tan(xFieldOfView / 2);
    double screenHeight = 2 * tan(yFieldOfView / 2);
    double xStep = screenWidth / SCENE_WIDTH;
    double yStep = screenHeight / SCENE_HEIGHT;
    double x0 = -screenWidth / 2;
    double y0 = -screenHeight / 2;

    for (int row = 0; row < SCENE_HEIGHT; row++) {
        for (int col = 0; col < SCENE_WIDTH; col++) {

            if (row == 118 && col == 348) {
                std::cout << "HOLD UP" << std::endl;
            }

            // Determine RAY
            Ray<double> *rayThroughPixel =
                    new Ray<double>(Vector3D<double>(0, 0, 0),
                                    Vector3D<double>(x0 + (col * xStep), y0 + (row * yStep), 1));

            pixels[row][col] = trace(rayThroughPixel, 0);

            delete rayThroughPixel;
        }
    }

    // Output image to file
    std::ofstream fileStream;
    fileStream.open("/Users/devankuleindiren/Documents/Work/University/Part IB/Computer Graphics and Image "
                            "Processing/Supervisions/Supervision 4/Ray Tracer/scene.ppm");
    fileStream << "P3" << std::endl;
    fileStream << SCENE_WIDTH << " " << SCENE_HEIGHT << std::endl;
    fileStream << "255" << std::endl;
    for (int row = SCENE_HEIGHT - 1; row >=0; row--) {
        for (int col = 0; col < SCENE_WIDTH; col++) {
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