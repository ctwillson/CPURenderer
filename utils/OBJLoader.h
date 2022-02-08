#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include <vector>
#include <string>
#include "math.h"

class OBJLoader{
private:
    std::vector<Vec3f> vertices_;
    std::vector<Vec3f> texcoords_;
    std::vector<Vec3f> normals_;
    std::vector<Vec3i> facesV_;
    std::vector<Vec3i> facesT_;
    std::vector<Vec3i> facesN_;
public:
    OBJLoader(const char* path);
    int nfaces();
    Vec3f vert(int idx);
    Vec3f texture(int idx);
    Vec3f normal(int idx);
    Vec3i face(int idx);
    Vec3i faceV(int idx);
    Vec3i faceT(int idx);
    void dump();
};

#endif