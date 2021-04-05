#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QLayout>
#include "opendialog.h"
#include "ogl_out.h"
#include "level.h"
#include <fstream>
#include <filesystem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void change_mode (edit_mode in_mode);
    void open_file_dialog(flag_saveload flag);

private slots:
    void print_console (const std::string &s);
    void on_drawButton_clicked();
    void on_selectButton_clicked();
    void on_actionSave_triggered();
    void on_actionLoad_triggered();
    void on_opendialog_finish(const std::string &filename, flag_saveload flag);

    void on_actionDelete_wall_triggered();

signals:
    void ogl_change_mode (edit_mode in_mode);

private:
    Ui::MainWindow *ui;
    OpenDialog *opendialog;
    oGL_out *ogl_out;
    Level level;
    QHBoxLayout *ogl_layout;
    edit_mode mode;
    std::ofstream outfile;
    std::string outfilename;
};
#endif // MAINWINDOW_H
