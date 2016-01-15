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