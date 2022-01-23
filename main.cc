#include "utils/OBJLoader.h"
#include "core/tgaimage.h"
#include "core/draw2d.h"
#include "utils/math.h"
#include <vector>
#include <limits.h>
// Model *model = NULL;
constexpr int width  = 800; // output image size
constexpr int height = 800;
Vec3f light_dir(0,0,-1);

int main(void) {
    // Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)}; 
    // Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
    // Vec2i t2[3] = {Vec2i(180, 300), Vec2i(120, 300), Vec2i(130, 180)};
    // TGAImage image(width, height, TGAImage::RGB);
    // drawTriangle(t0[0],t0[1],t0[2],image,white);
    // drawTriangle(t1[0],t1[1],t1[2],image,red);
    // drawTriangle(t2[0],t2[1],t2[2],image,PINK);
    OBJLoader *loader = new OBJLoader("/home/cs18/test/OBJ/african_head.obj");
    TGAImage image(width, height, TGAImage::RGB);
    std::vector<float> zbuffer;
    zbuffer.resize(width * height,INT_MIN);
    for (int i=0; i<loader->nfaces(); i++) {
        Vec3i face = loader->face(i);
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        for (int j=0; j<3; j++) {
            Vec3f v = loader->vert(face[j]);
            world_coords[j]  = v;
            screen_coords[j] = viewPort(world_coords[j],width,height);
            // screen_coords[j] = Vec3f((v.x+1.)*width/2., (v.y+1.)*height/2.,v.z); 
            
        }
        Vec3f v1 = world_coords[2]-world_coords[0],v2 = world_coords[1]-world_coords[0];
        Vec3f n = cross(v1,v2);
        n.normalize(); 
        float intensity = n*light_dir; 
        if (intensity>0) { 
            drawTriangle(screen_coords,zbuffer, image, TGAColor(intensity*255, intensity*255, intensity*255, 255)); 
        }
    }
    image.flip_vertically();
    image.write_tga_file("triangle.tga");
}
