#ifndef SPHERE_H
#define SPHERE_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_0>
#include <cmath>

union point {
    GLfloat a[3];
};

class sphere
{
public:
    sphere(float center_x, float center_y, float center_z, float r, unsigned in_hpoints, unsigned in_rpoints, QOpenGLFunctions_3_0 *in_f30);
    ~sphere();
    void draw();
    //static not needed as stack is unavailable due to dynamic sizes
private:
    void addpoint (GLfloat *vertexes, GLfloat *point);
    point *vertexes;
    QOpenGLFunctions_3_0 *f30;
    unsigned hpoints;
    unsigned rpoints;
};

#endif // SPHERE_H
