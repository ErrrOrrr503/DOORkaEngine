#include "sphere.h"

sphere::sphere(float center_x, float center_y, float center_z, float r, unsigned in_hpoints, unsigned in_rpoints)
{
    //vertexes calculations (unoptimized) :
    /*
     *         0
     *       1   2
     *         3
    */
    hpoints = in_hpoints;
    rpoints = in_rpoints;
    vertices = new point[hpoints * rpoints + 2]; //plus 1 duplicate for easier constructions
    size_ = hpoints * rpoints + 2;
    vertices[0] = {center_x, center_y, center_z - r};
    unsigned end = hpoints * rpoints + 1;
    vertices[end] = {center_x, center_y, center_z + r};
    const float dh = 2 * r / (float)(hpoints + 1);
    const float da = 2 * (float)M_PI / (float)rpoints;
    float z = center_z - r;
    for (unsigned i = 0; i < hpoints; i++) {
        z += dh;
        float a = 0;
        float curr_r = r * sqrtf (1 - (z - center_z) * (z - center_z) / r / r);
        for (unsigned j = 0; j < rpoints; j++) {
            point tmp_point;
            tmp_point.a[0] = curr_r * cosf(a);
            tmp_point.a[1] = curr_r * sinf(a);
            tmp_point.a[2] = z;
            vertices[1 + i * rpoints + j] = tmp_point;
            a += da;
        }
    }
}

void sphere::draw()
{
    unsigned end = hpoints * rpoints + 1;
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    for (unsigned i = 0; i < hpoints; i++)
        glDrawArrays(GL_LINE_LOOP, 1 + i * rpoints, rpoints);
    for (unsigned i = 0; i < rpoints; i++) {
        glVertexPointer(3, GL_FLOAT, (int)rpoints * (int)sizeof(point), &vertices[i + 1]);
        glDrawArrays(GL_LINE_STRIP, 0, hpoints);
    }
    glBegin(GL_LINES);
    for (unsigned i = 1; i <= rpoints; i++) {
        glVertex3f(vertices[0].a[0], vertices[0].a[1], vertices[0].a[2]);
        glVertex3f(vertices[i].a[0], vertices[i].a[1], vertices[i].a[2]);
    }
    for (unsigned i = 1; i <= rpoints; i++) {
        glVertex3f(vertices[end - i].a[0], vertices[end - i].a[1], vertices[end - i].a[2]);
        glVertex3f(vertices[end].a[0], vertices[end].a[1], vertices[end].a[2]);
    }
    glEnd();
}

sphere::~sphere()
{
    delete[] vertices;
}
