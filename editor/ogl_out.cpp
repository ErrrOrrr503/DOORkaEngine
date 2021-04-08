#include "ogl_out.h"

oGL_out::oGL_out(QWidget *parent, Level *in_level) : QOpenGLWidget(parent)
{
    QSurfaceFormat ogl_format;
    QSurfaceFormat::setDefaultFormat(ogl_format);
    setFormat(ogl_format);
    center_x = 0;
    center_y = 0;
    z_angle = 0;
    scale = 1;
    level = in_level;
    setFocusPolicy(Qt::ClickFocus);
}

oGL_out::~oGL_out()
{

}

void oGL_out::initializeGL()
{
    context = QOpenGLContext::currentContext();
    f = context->functions();
    f->initializeOpenGLFunctions();
    f30 = context->versionFunctions<QOpenGLFunctions_3_0>();
    f30->initializeOpenGLFunctions();
    f30->glMatrixMode(GL_PROJECTION);
    f30->glLoadIdentity();
    f30->glOrtho(scale * (float) -width() / 2, scale * (float) width() / 2,
                 scale * (float) -height() / 2, scale * (float) height() / 2, -1, 1);
    f30->glEnable(GL_MULTISAMPLE);
    std::string console = "ogl init: x = " + std::to_string(width()) + " y = " + std::to_string(height());
    emit print_console(console);
    console.clear();
    console = "ogl max x = " + std::to_string((float) width() / 2) + " y = " + std::to_string((float) height() / 2);
    emit print_console(console);
}

void oGL_out::resizeGL(int nWidth, int nHeight)
{
#ifdef DEBUG_DRAW
    static int oWidth = 0, oHeight = 0;
    static float prev_angle;
    std::string console;
    if (nWidth == oWidth && nHeight  == oHeight && z_angle == prev_angle)
        console = "center moved: new x0 = " + std::to_string(center_x) + " new y0 = " + std::to_string(center_y);
    if (nWidth != oWidth || nHeight  != oHeight)
        console = "ogl resize: new x = " + std::to_string(nWidth) + " new y = " + std::to_string(nHeight);
    if (z_angle != prev_angle)
        console = "rotated: " + std::to_string(z_angle);
    emit print_console (console);
    console.clear();
    oWidth = nWidth;
    oHeight  = nHeight;
    prev_angle = z_angle;
#endif
    f30->glMatrixMode(GL_PROJECTION);
    f30->glLoadIdentity();
    //1 pixel is a basic value
    f30->glOrtho(scale * (center_x + ((float) -nWidth / 2)), scale * (center_x + ((float) nWidth / 2)),
                 scale * (center_y + ((float) -nHeight / 2)), scale * (center_y + ((float) nHeight / 2)),
                 -1, 1);
    f30->glRotatef(z_angle, 0, 0, 1);
    //it is possible to use glTranslate here however... what for? center_xy are still needed
}

void oGL_out::paintGL()
{
    level_paint();
    pointers_paint();
}

void oGL_out::keyPressEvent(QKeyEvent *ke)
{
    const int angle_step = 3;
    switch (ke->key()) {
    case Qt::Key_Right:
        z_angle += angle_step;
        break;
    case Qt::Key_Left:
        z_angle -= angle_step;
        break;
    default:
        break;
    }
    cursor.antirotate(z_angle);
    QResizeEvent *myResizeEvent = new QResizeEvent(QSize(size()), QSize(size())); //call resize staff that places center where needed
    QCoreApplication::postEvent(this, myResizeEvent);
    update ();
}

void oGL_out::mouseMoveEvent(QMouseEvent *me)
{
    center_x = old_center_x - me->localPos().x() + cursor.pressed_x_raw;
    center_y = old_center_y + me->localPos().y() - cursor.pressed_y_raw;
    QResizeEvent *myResizeEvent = new QResizeEvent(QSize(size()), QSize(size())); //call resize staff that places center where needed
    QCoreApplication::postEvent(this, myResizeEvent);
    update ();
}

void oGL_out::mousePressEvent(QMouseEvent *me)
{
    if (cursor.type == none)
        cursor.type = plain;
    cursor.set(me->localPos().x(), me->localPos().y(), height(), width(), center_x, center_y, z_angle, scale);
    if (cursor.type == sel_mode || cursor.type == draw_mode)
        level->select_wall(cursor.cur_x, cursor.cur_y);
    if (cursor.type == draw_mode)
        level->add_wall();
    update();
#ifdef DEBUG_DRAW
    emit print_console("mouse pointed to x = " + std::to_string(cursor.cur_x) + " y = " + std::to_string(cursor.cur_y));
#endif
    old_center_x = center_x;
    old_center_y = center_y;
}

void oGL_out::wheelEvent(QWheelEvent *me)
{
    if (scale < 0.5 && me->angleDelta().y() < 0)
        return;
    scale += (float) me->angleDelta().y() / 500;
    cursor.antiscale(scale);
    QResizeEvent *myResizeEvent = new QResizeEvent(QSize(size()), QSize(size())); //call resize staff that places center where needed
    QCoreApplication::postEvent (this, myResizeEvent);
    update ();
}

void oGL_out::mouseReleaseEvent(QMouseEvent *me)
{
    Q_UNUSED(me);
}

void oGL_out::ogl_change_mode (edit_mode in_mode)
{
    switch (in_mode) {
    case draw:
        cursor.type = draw_mode;
        break;
    case sel:
        cursor.type = sel_mode;
        break;
    default:
        break;
    }
    update ();
}

void oGL_out::pointers_paint()
{
    f30->glEnable(GL_POINT_SMOOTH);
    if (cursor.type != none) {
        f30->glPointSize(20);
        f30->glColor3f(1.0, 0.5, 0.0);
        if (cursor.type  == sel_mode)
            f30->glColor3f(0.0, 0.5, 1.0);
        f30->glBegin(GL_POINTS);
        f30->glVertex3f(cursor.cur_x, cursor.cur_y, 0);
        f30->glEnd();
        f30->glPointSize(10);
        f30->glColor3f(0.5, 0.25, 0.0);
        if (cursor.type  == sel_mode)
            f30->glColor3f(0.0, 0.25, 0.5);
        f30->glBegin(GL_POINTS);
        f30->glVertex3f(cursor.cur_x, cursor.cur_y, 0);
        f30->glEnd();
    }
}


void oGL_out::level_paint()
{
    f30->glClearColor (0.2, 0.2, 0.2, 0.0);
    f30->glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f30->glLineWidth (1);
    f30->glColor3f (0, 0.6, 0);
    f30->glBegin (GL_LINES);
    for (float i = -level->level_x; i < level->level_x; i += level->cell_size) {
        glVertex2f (i, -level->level_y);
        glVertex2f (i, level->level_y);
    }
    for (float i = -level->level_y; i < level->level_y; i += level->cell_size) {
        glVertex2f (-level->level_x, i);
        glVertex2f (level->level_y, i);
    }
    f30->glEnd ();

    f30->glLineWidth (3);
    f30->glColor3f (0.0, 0.4, 0.7);
    f30->glBegin (GL_LINES);
    for (size_t i = 0; i < level->walls.size (); i++) {
        glVertex2f(level->walls[i].x1, level->walls[i].y1);
        glVertex2f(level->walls[i].x2, level->walls[i].y2);
    }
    f30->glEnd ();
    f30->glLineWidth (3);
    f30->glColor3f (0.4, 0.8, 0);
    f30->glBegin (GL_LINES);
    glVertex2f (level->selected_wall.x1, level->selected_wall.y1);
    glVertex2f (level->selected_wall.x2, level->selected_wall.y2);
    f30->glEnd ();
}

void oGL_out::test_paint()
{
    f30->glPointSize(20);
    f30->glColor3f(0, 0.7, 0);
    f30->glEnable(GL_POINT_SMOOTH);
    f30->glLineWidth(1);
    //section figures
    static GLfloat vertexes[] = {
        100, 100, 0,
        100, -100, 0,
        -100, 100, 0,
        -100, -100, 0
    };
    static GLfloat colors[] = {
        1.0, 1.0, 0.0,
        0.0, 1.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 0.0
    };
    f30->glClearColor(0.0, 1.0, 0.0, 0.0);
    f30->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    f30->glEnableClientState(GL_VERTEX_ARRAY);
    f30->glEnableClientState(GL_COLOR_ARRAY);
    f30->glColorPointer(3, GL_FLOAT, 0, colors);
    f30->glVertexPointer(3, GL_FLOAT, 0, vertexes);
    f30->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
