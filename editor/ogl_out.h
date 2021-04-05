#ifndef OGL_OUT_H
#define OGL_OUT_H

#include <QMouseEvent>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_0>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_1_2> // riva tnt2
#include <cmath>
#include "cursor.h"
#include "level.h"

class oGL_out : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit oGL_out (QWidget *parent, Level *in_level);
    ~oGL_out ();
protected:
    void initializeGL ();
    void resizeGL (int nWidth, int nHeight);
    void paintGL ();

private:
    void test_paint ();
    void level_paint ();
    void pointers_paint ();

private slots:
    void keyPressEvent (QKeyEvent *ke);
    void mouseMoveEvent (QMouseEvent *me);
    void mousePressEvent (QMouseEvent *me);
    void mouseReleaseEvent (QMouseEvent *me);
    void wheelEvent (QWheelEvent *me);

public slots:
    void ogl_change_mode (edit_mode in_mode);

signals:
    void print_console (const std::string &s);

private:
    QOpenGLFunctions *f;
    QOpenGLFunctions_3_0 *f30;
    QOpenGLContext *context;

    Level *level;
    float center_x;
    float center_y;
    float old_center_x;
    float old_center_y;
    float z_angle;
    float scale;
    Cursor cursor;
};

#endif // OGL_OUT_H
