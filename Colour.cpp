//
// Created by Devan Kuleindiren on 15/01/2016.
//

#include <math.h>
#include <iostream>
#include "Colour.h"

int cap (int original) {
    if (original < 0) return 0;
    if (original > 255) return 255;
    return original;
}

void Colour::legalise() {
    this->r = cap(r);
    this->g = cap(g);
    this->b = cap(b);
}

Colour &Colour::operator*(double scalar) {
    Colour *colour = new Colour(this->r, this->g, this->b);
    colour->r = (int) round (this->r * scalar);
    colour->g = (int) round (this->g * scalar);
    colour->b = (int) round (this->b * scalar);
    colour->legalise();
    return *colour;
}

Colour &Colour::operator+(double scalar) {
    Colour *colour = new Colour(this->r, this->g, this->b);
    colour->r = (int) round (this->r + scalar);
    colour->g = (int) round (this->g + scalar);
    colour->b = (int) round (this->b + scalar);
    colour->legalise();
    return *colour;
}

Colour &Colour::operator+(Colour &toAdd) {
    Colour *colour = new Colour(this->r + toAdd.r, this->g + toAdd.g, this->b + toAdd.b);
    colour->legalise();
    return *colour;
}

void Colour::printColour() {
    std::cout << "[" << r << "," << g << "," << b << "]" << std::endl;
}