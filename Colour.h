//
// Created by Devan Kuleindiren on 15/01/2016.
//

#ifndef RAY_TRACER_COLOUR_H
#define RAY_TRACER_COLOUR_H

class Colour {
public:
    Colour (int r, int g, int b) : r(r), g(g), b(b) {}
    int r;
    int g;
    int b;
    void legalise ();
    Colour &operator*(double scalar);
    Colour &operator+(double scalar);
    Colour &operator+(Colour &toAdd);
    void printColour ();
};

#endif //RAY_TRACER_COLOUR_H
