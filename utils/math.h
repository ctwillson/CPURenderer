#ifndef __MATH_H__
#define __MATH_H__
#include <cassert>
template<class T> struct Vec2 {
    T x,y;
    Vec2() = default;
    Vec2(T X, T Y) : x(X), y(Y) {}
    T  operator[](const int i) const { assert(i>=0 && i<2); return i==0 ? x : y; }
};

template<class T> struct Vec3 {
    T x,y,z;
    Vec3() = default;
    Vec3(T X, T Y,T Z) : x(X), y(Y), z(Z) {}
    T  operator[](const int i) const { assert(i>=0 && i<3); return i==0 ? x : (1==i ? y : z); }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

#endif