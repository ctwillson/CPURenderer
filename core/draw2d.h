#ifndef __DRAW2D_H__
#define __DRAW2D_H__
#include "../utils/math.h"
#include <vector>
void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void drawTriangle(Vec3f* t,Vec3f* tex_coords,std::vector<float> &zbuffer,TGAImage &image, TGAImage &texure,TGAColor color);
Vec3f viewPort(Vec3f world_coord,int width,int height);
#endif