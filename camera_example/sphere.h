#ifndef SPHERE_H
#define SPHERE_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

union point {
    GLfloat a[3];
};

class sphere
{
public:
    sphere() = delete;
    sphere(float center_x, float center_y, float center_z, float r, unsigned in_hpoints, unsigned in_rpoints);
    ~sphere();
    void draw();
    //static not needed as stack is unavailable due to dynamic sizes
private:
    void addpoint (GLfloat *vertices, GLfloat *point);
    point *vertices;
    size_t size_;
    unsigned hpoints;
    unsigned rpoints;
};

#endif // SPHERE_H
