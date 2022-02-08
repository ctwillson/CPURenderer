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
    // Eigen::Vector3f getColor(float u, float v)
    // {
    //     auto u_img = u * width;
    //     auto v_img = (1 - v) * height;
    //     auto color = image_data.at<cv::Vec3b>(v_img, u_img);
    //     return Eigen::Vector3f(color[0], color[1], color[2]);
    // }
TGAColor getcolor(float u,float v,TGAImage &texure) {
    int width = texure.get_width();
    int height = texure.get_height();
    return texure.get(u * width ,v * height);

}

// static Eigen::Vector2f interpolate(float alpha, float beta, float gamma, const Eigen::Vector2f& vert1, const Eigen::Vector2f& vert2, const Eigen::Vector2f& vert3, float weight)
// {
//     auto u = (alpha * vert1[0] + beta * vert2[0] + gamma * vert3[0]);
//     auto v = (alpha * vert1[1] + beta * vert2[1] + gamma * vert3[1]);

//     u /= weight;
//     v /= weight;

//     return Eigen::Vector2f(u, v);
// }
Vec2f interpolate(Vec3f inter,Vec3f* tex_coords) {
    float u = inter[0] * tex_coords[0].x + inter[1] * tex_coords[1].x + inter[2] * tex_coords[2].x;
    // float v = inter[0] * (1 - tex_coords[0].y) + inter[1] * (1 - tex_coords[1].y) + inter[2] * (1 - tex_coords[2].y);
    float v = inter[0] * tex_coords[0].y + inter[1] * tex_coords[1].y + inter[2] * tex_coords[2].y;
    return Vec2f(u,v);
}
void drawTriangle(Vec3f* t,Vec3f* tex_coords,std::vector<float> &zbuffer,TGAImage &image, TGAImage &texure,TGAColor color) {
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
                    Vec2f uv = interpolate(bc_screen,tex_coords);
                    image.set(i, j, getcolor(uv[0],uv[1],texure));
                    // image.set(i, j, color);
                }
                // image.set(i, j, color);
            }
        }
}
