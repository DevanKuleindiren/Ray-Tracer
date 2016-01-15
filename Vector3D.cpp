//
// Created by Devan Kuleindiren on 14/01/2016.
//

#include <iostream>
#include "Math.h"

template <class T> class Vector3D {
public:
    Vector3D<T> () {};
    Vector3D<T> (T x, T y, T z) : x(x), y(y), z(z) {}
    T x;
    T y;
    T z;
    virtual Vector3D<T> operator+(Vector3D<T> toAdd);
    virtual Vector3D<T> operator-(Vector3D<T> toAdd);
    virtual Vector3D<T> operator*(double toMultiply);
    virtual T operator*(Vector3D<T> toDotProd);
    virtual T magnitude ();
    virtual Vector3D<T> normalise ();
    virtual void printVector ();
};

template <class T> Vector3D<T> Vector3D<T>::operator+(Vector3D<T> toAdd) {
    return Vector3D(this->x + toAdd.x, this->y + toAdd.y, this->z + toAdd.z);
}

template <class T> Vector3D<T> Vector3D<T>::operator-(Vector3D<T> toSub) {
    return Vector3D(this->x - toSub.x, this->y - toSub.y, this->z - toSub.z);
}

template <class T> T Vector3D<T>::operator*(Vector3D<T> toDotProd) {
    return (this->x * toDotProd.x) + (this->y * toDotProd.y) + (this->z * toDotProd.z);
}

template <class T> Vector3D<T> Vector3D<T>::operator*(double toMultiply) {
    return Vector3D(this->x * toMultiply, this->y * toMultiply, this->z * toMultiply);
}

template <class T> T Vector3D<T>::magnitude() {
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

template <class T> Vector3D<T> Vector3D<T>::normalise() {
    T magnitude = this->magnitude();

    if (magnitude > 0) {
        this->x /= magnitude;
        this->y /= magnitude;
        this->z /= magnitude;
    }

    return *this;
}

template <class T> void Vector3D<T>::printVector() {
    std::cout << "[" << this->x << "," << this->y << "," << this->z << "]" << std::endl;
}