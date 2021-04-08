#ifndef FILENAME_ENTER_H
#define FILENAME_ENTER_H
//! \file filename_enter.h
#include <QPlainTextEdit>
#include <QKeyEvent>

/*!
 * \brief The filename_enter class
 * \details Text edit with custom key events.
 */
class filename_enter : public QPlainTextEdit
{
    Q_OBJECT
public:
    filename_enter (QWidget *parent = nullptr);
public slots:
    void keyPressEvent (QKeyEvent *e);
};

#endif // FILENAME_ENTER_H
