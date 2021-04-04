#ifndef OPENDIALOG_H
#define OPENDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QFileDialog>
#include "filename_enter.h"

namespace Ui {
class OpenDialog;
}

enum flag_saveload {
    save,
    load
};

class OpenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenDialog(QWidget *parent, flag_saveload in_flag);
    ~OpenDialog();

private slots:
    void on_opendialog_openButton_clicked();

    void on_opendialog_cancelButton_clicked();

    void on_actionMainAction_triggered();

    void on_opendialog_selectButton_clicked();

private:
    Ui::OpenDialog *ui;
    flag_saveload flag;
    filename_enter *filename_field;
    std::string filename;
    QLayout *filename_layout;

signals:
    void filename_read(const std::string &filename, flag_saveload flag);
};

#endif // OPENDIALOG_H
