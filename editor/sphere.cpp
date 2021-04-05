#include "sphere.h"

sphere::sphere(float center_x, float center_y, float center_z, float r, unsigned in_hpoints, unsigned in_rpoints, QOpenGLFunctions_3_0 *in_f30)
{
    //vertexes calculations (unoptimized) :
    /*
     *         0
     *       1   2
     *         3
    */
    hpoints = in_hpoints;
    rpoints = in_rpoints;
    f30 = in_f30;
    vertexes = new point[hpoints * rpoints + 2]; //plus 1 duplicate for easier constructions
    vertexes[0] = {center_x, center_y, center_z - r};
    unsigned end = hpoints * rpoints + 1;
    vertexes[end] = {center_x, center_y, center_z + r};
    const float dh = 2 * r / (hpoints + 1);
    const float da = 2 * M_PI / rpoints;
    float z = center_z - r;
    for (unsigned i = 0; i < hpoints; i++) {
        z += dh;
        float a = 0;
        float curr_r = r * sqrt (1 - (z - center_z) * (z - center_z) / r / r);
        for (unsigned j = 0; j < rpoints; j++) {
            point tmp_point;
            tmp_point.a[0] = curr_r * cos(a);
            tmp_point.a[1] = curr_r * sin(a);
            tmp_point.a[2] = z;
            vertexes[1 + i * rpoints + j] = tmp_point;
            a += da;
        }
    }
}

void sphere::draw()
{
    unsigned end = hpoints * rpoints + 1;
    f30->glEnableClientState(GL_VERTEX_ARRAY);
    f30->glVertexPointer(3, GL_FLOAT, 0, vertexes);
    for (unsigned i = 0; i < hpoints; i++)
        f30->glDrawArrays(GL_LINE_LOOP, 1 + i * rpoints, rpoints);
    for (unsigned i = 0; i < rpoints; i++) {
        f30->glVertexPointer(3, GL_FLOAT, rpoints * sizeof(point), &vertexes[i + 1]);
        f30->glDrawArrays(GL_LINE_STRIP, 0, hpoints);
    }
    f30->glBegin(GL_LINES);
    for (unsigned i = 1; i <= rpoints; i++) {
        f30->glVertex3f(vertexes[0].a[0], vertexes[0].a[1], vertexes[0].a[2]);
        f30->glVertex3f(vertexes[i].a[0], vertexes[i].a[1], vertexes[i].a[2]);
    }
    for (unsigned i = 1; i <= rpoints; i++) {
        f30->glVertex3f(vertexes[end - i].a[0], vertexes[end - i].a[1], vertexes[end - i].a[2]);
        f30->glVertex3f(vertexes[end].a[0], vertexes[end].a[1], vertexes[end].a[2]);
    }
    f30->glEnd();
}

sphere::~sphere()
{
    delete vertexes;
}
