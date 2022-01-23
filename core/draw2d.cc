#include "tgaimage.h"
#include "draw2d.h"
#include <iostream>

int width = 800;
int height = 800;

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

Vec3f barycentric_coords(int a,int b,Vec3f* v) {
    float x = a;
    float y = b;
    float alpha = (x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*y + v[1].x*v[2].y - v[2].x*v[1].y) / (v[0].x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*v[0].y + v[1].x*v[2].y - v[2].x*v[1].y);
    float beta = (x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*y + v[2].x*v[0].y - v[0].x*v[2].y) / (v[1].x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*v[1].y + v[2].x*v[0].y - v[0].x*v[2].y);
    float gamma = (x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*y + v[0].x*v[1].y - v[1].x*v[0].y) / (v[2].x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*v[2].y + v[0].x*v[1].y - v[1].x*v[0].y);
    // std::cout << alpha << " " << beta << " " << gamma << std::endl;
    return {alpha,beta,gamma};
}

bool insideTriangle(int x,int y ,Vec3f t0,Vec3f t1,Vec3f t2) {
    Vec3f v[3] = {t0,t1,t2};
    Vec3f u = barycentric_coords(x,y,v);
    return u.x >= 0 && u.y >= 0 && u.z >= 0;
    // Vec3f u = cross(Vec3f(t2[0]-pts[0][0], pts[1][0]-pts[0][0], pts[0][0]-P[0]), Vec3f(pts[2][1]-pts[0][1], pts[1][1]-pts[0][1], pts[0][1]-P[1]));
}

bool insideTriangle(Vec3f u) {
    return u.x >= 0 && u.y >= 0 && u.z >= 0;
    // Vec3f u = cross(Vec3f(t2[0]-pts[0][0], pts[1][0]-pts[0][0], pts[0][0]-P[0]), Vec3f(pts[2][1]-pts[0][1], pts[1][1]-pts[0][1], pts[0][1]-P[1]));
}
//Viewport transform: ViewPort transfer world space to screen
Vec3f viewPort(Vec3f world_coord,int width,int height) {
    int wx = world_coord.x * width/2 + width/2 + 0.5;
    int wy = world_coord.y * height/2 + height/2 + 0.5;
    return Vec3f(wx,wy,world_coord.z);
}
void drawTriangle(Vec3f* t,std::vector<float> &zbuffer,TGAImage &image, TGAColor color) {
    //bounding box
    int x_max = std::max(t[0].x,std::max(t[1].x,t[2].x));
    int x_min = std::min(t[0].x,std::min(t[1].x,t[2].x));
    int y_max = std::max(t[0].y,std::max(t[1].y,t[2].y));
    int y_min = std::min(t[0].y,std::min(t[1].y,t[2].y));

    for(int i = x_min;i <= x_max;i++)
        for(int j = y_min;j <= y_max;j++) {
            // Vec3f barycentric_coords(x,y,);
            Vec3f bc_screen  = barycentric_coords(i,j,t);
            if(insideTriangle(bc_screen)) {
                float z = 0;
                int index = i + j * image.get_width();
                for (int i=0; i<3; i++) z += t[i][2]*bc_screen[i];
                if (zbuffer[index]<z) {
                    zbuffer[index] = z;
                    image.set(i, j, color);
                }
                // image.set(i, j, color);
            }
        }
}
