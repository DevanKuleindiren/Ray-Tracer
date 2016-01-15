//
// Created by Devan Kuleindiren on 15/01/2016.
//

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
    if (scalar >= 0) {
        colour->r = this->r *= scalar;
        colour->g = this->g *= scalar;
        colour->b = this->b *= scalar;
        colour->legalise();
    }
    return *colour;
}