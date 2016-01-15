//
// Created by Devan Kuleindiren on 15/01/2016.
//

#import "Vector3D.cpp"

template <class T> class Ray {
public:
    Ray (Vector3D<T> position, Vector3D<T> direction) : position(position), direction(direction.normalise()) {}
    Vector3D<T> position;
    Vector3D<T> direction;
};