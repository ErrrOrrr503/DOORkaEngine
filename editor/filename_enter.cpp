#include "filename_enter.h"

filename_enter::filename_enter (QWidget *parent) : QPlainTextEdit (parent)
{

}


void filename_enter::keyPressEvent (QKeyEvent *e)
{
    if (e->key () == Qt::Key_Return || e->key () == Qt::Key_Enter)
        return;
    QPlainTextEdit::keyPressEvent (e);
}
