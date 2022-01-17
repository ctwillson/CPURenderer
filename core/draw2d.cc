#include "tgaimage.h"
#include "../utils/math.h"
#include <iostream>

void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

Vec3f barycentric_coords(int a,int b,Vec2i* v) {
    float x = a;
    float y = b;
    float alpha = (x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*y + v[1].x*v[2].y - v[2].x*v[1].y) / (v[0].x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*v[0].y + v[1].x*v[2].y - v[2].x*v[1].y);
    float beta = (x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*y + v[2].x*v[0].y - v[0].x*v[2].y) / (v[1].x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*v[1].y + v[2].x*v[0].y - v[0].x*v[2].y);
    float gamma = (x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*y + v[0].x*v[1].y - v[1].x*v[0].y) / (v[2].x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*v[2].y + v[0].x*v[1].y - v[1].x*v[0].y);
    // std::cout << alpha << " " << beta << " " << gamma << std::endl;
    return {alpha,beta,gamma};
}
bool insideTriangle(int x,int y ,Vec2i t0,Vec2i t1,Vec2i t2) {
    Vec2i v[3] = {t0,t1,t2};
    Vec3f u = barycentric_coords(x,y,v);
    return u.x > 0 && u.y > 0 && u.z > 0;
    // Vec3f u = cross(Vec3f(t2[0]-pts[0][0], pts[1][0]-pts[0][0], pts[0][0]-P[0]), Vec3f(pts[2][1]-pts[0][1], pts[1][1]-pts[0][1], pts[0][1]-P[1]));
}
void drawTriangle(Vec2i t0,Vec2i t1,Vec2i t2,TGAImage &image, TGAColor color) {
    //bounding box
    int x_max = std::max(t0.x,std::max(t1.x,t2.x));
    int x_min = std::min(t0.x,std::min(t1.x,t2.x));
    int y_max = std::max(t0.y,std::max(t1.y,t2.y));
    int y_min = std::min(t0.y,std::min(t1.y,t2.y));
    std::cout << x_min <<" "<<x_max << " " << y_min << " " << y_max <<std::endl;
    for(int i = x_min;i <= x_max;i++)
        for(int j = y_min;j <= y_max;j++) {
            if(insideTriangle(i,j,t0,t1,t2)) {
                image.set(i, j, color);
            }
        }
}
