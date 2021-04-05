#ifndef FILENAME_ENTER_H
#define FILENAME_ENTER_H

#include <QPlainTextEdit>
#include <QKeyEvent>

class filename_enter : public QPlainTextEdit
{
    Q_OBJECT
public:
    filename_enter (QWidget *parent = nullptr);
public slots:
    void keyPressEvent (QKeyEvent *e);
};

#endif // FILENAME_ENTER_H
