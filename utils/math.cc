#include "math.h"
Vec3i cross(const Vec3i &v1, const Vec3i &v2) {
    return Vec3i{v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}