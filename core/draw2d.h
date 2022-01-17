#ifndef __DRAW2D_H__
#define __DRAW2D_H__
void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void drawTriangle(Vec2i t0,Vec2i t1,Vec2i t2,TGAImage &image, TGAColor color);
#endif