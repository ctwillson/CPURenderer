#ifndef __MATH_H__
#define __MATH_H__
#include <cassert>
#include <cmath>
template<class T> struct Vec2 {
    T x,y;
    Vec2() = default;
    Vec2(T X, T Y) : x(X), y(Y) {}
    double norm2() const { return (*this)*(*this) ; }
    double norm()  const { return std::sqrt(norm2()); }
    Vec2<T> & normalize(T l = 1) { *this = (*this) * (l/norm()); return *this; }
    T operator*(Vec2<T> v) const {return x * v.x + y * v.y;}
    Vec2<T> operator *(float f)  const { return Vec2<T>(x*f, y*f); }
    Vec2<T> operator-(Vec2<T> v) const {return {x - v.x,y - v.y};}
    Vec2<T> operator+(Vec2<T> v) const {return {x + v.x,y + v.y};}
    T  operator[](const int i) const { assert(i>=0 && i<2); return i==0 ? x : y; }
};

template<class T> struct Vec3 {
    T x,y,z;
    Vec3() = default;
    Vec3(T X, T Y,T Z) : x(X), y(Y), z(Z) {}
    T norm2() const { return (*this)*(*this) ; }
    T norm()  const { return std::sqrt(norm2()); }
    Vec3<T> & normalize(T l = 1) { *this = (*this) * ((T)l/norm()); return *this; }
    Vec3<T> operator-(Vec3<T> v) const {return {x - v.x,y - v.y,z - v.z};}
    Vec3<T> operator+(Vec3<T> v) const {return {x + v.x,y - v.y,z + v.z};}
    T operator*(Vec3<T> v) const {return x * v.x + y * v.y + z * v.z;}
    Vec3<T> operator *(float f)  const { return Vec3<T>(x*f, y*f, z*f); }
    T operator[](const int i) const { assert(i>=0 && i<3); return i==0 ? x : (1==i ? y : z); }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

Vec3f cross(Vec3f &v1, Vec3f &v2);
#endif