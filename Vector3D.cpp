//
// Created by Devan Kuleindiren on 14/01/2016.
//

#include <iostream>
#include "Math.h"

template <class T> class Vector3D {
public:
    Vector3D<T> () {};
    Vector3D<T> (const Vector3D<T> &toCopy);
    Vector3D<T> (T x, T y, T z) : x(x), y(y), z(z) {}
    T x;
    T y;
    T z;
    Vector3D<T> &operator=(const Vector3D<T> &toAssign);
    Vector3D<T> &operator+(const Vector3D<T> &toAdd);
    Vector3D<T> &operator-(const Vector3D<T> &toSub);
    Vector3D<T> &operator*(const double toMultiply);
    T operator*(const Vector3D<T> &toDotProd);
    T magnitude ();
    Vector3D<T> &normalise ();
    void printVector ();
};

template <class T> Vector3D<T>::Vector3D(const Vector3D<T> &toCopy) {
    *this = toCopy;
}

template <class T> Vector3D<T> &Vector3D<T>::operator=(const Vector3D<T> &toAssign) {
    this->x = toAssign.x;
    this->y = toAssign.y;
    this->z = toAssign.z;

    return *this;
}

template <class T> Vector3D<T> &Vector3D<T>::operator+(const Vector3D<T> &toAdd) {
    return *(new Vector3D(this->x + toAdd.x, this->y + toAdd.y, this->z + toAdd.z));
}

template <class T> Vector3D<T> &Vector3D<T>::operator-(const Vector3D<T> &toSub) {
    return *(new Vector3D(this->x - toSub.x, this->y - toSub.y, this->z - toSub.z));
}

template <class T> T Vector3D<T>::operator*(const Vector3D<T> &toDotProd) {
    return (this->x * toDotProd.x) + (this->y * toDotProd.y) + (this->z * toDotProd.z);
}

template <class T> Vector3D<T> &Vector3D<T>::operator*(const double toMultiply) {
    return *(new Vector3D(this->x * toMultiply, this->y * toMultiply, this->z * toMultiply));
}

template <class T> T Vector3D<T>::magnitude() {
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

template <class T> Vector3D<T> &Vector3D<T>::normalise() {
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