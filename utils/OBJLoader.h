#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include <vector>
#include <string>
#include "math.h"

class OBJLoader{
private:
    // VertexType *vertices_;
    // VertexType *texcoords_;
    // VertexType *normals_;
    std::vector<Vec3f> vertices_;
    std::vector<Vec3i> facesV_;
    std::vector<Vec3i> facesT_;
    std::vector<Vec3i> facesN_;
public:
    OBJLoader(const char* path);
    int nfaces();
    Vec3f vert(int idx);
    Vec3i face(int idx);
    void dump();
};

#endif