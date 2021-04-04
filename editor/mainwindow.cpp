#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->console->setReadOnly(true);

    ogl_layout = new QHBoxLayout(ui->frame_ogl);
    ogl_layout->setContentsMargins(0, 0, 0, 0);
    ogl_layout->setSpacing(0);
    ogl_out = new oGL_out(ui->frame_ogl, &level);
    ogl_layout->addWidget(ogl_out);
    ogl_out->show();

    QObject::connect(ogl_out, &oGL_out::print_console,
                     this, &MainWindow::print_console);
    QObject::connect(&level, &Level::print_console,
                     this, &MainWindow::print_console);
    QObject::connect(this, &MainWindow::ogl_change_mode,
                     ogl_out, &oGL_out::ogl_change_mode);

    change_mode(draw);
    print_console("Ready");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ogl_out;
    delete ogl_layout;
    outfile.close();
}


void MainWindow::on_drawButton_clicked()
{
    change_mode (draw);
}

void MainWindow::on_selectButton_clicked()
{
    change_mode (sel);
}

void MainWindow::print_console (const std::string &s)
{
    ui->console->insertPlainText ("[");
    ui->console->insertPlainText (QTime::currentTime().toString("h:m:s"));
    ui->console->insertPlainText ("]> ");
    ui->console->insertPlainText (QString::fromStdString (s));
    ui->console->insertPlainText ("\n");
    ui->console->ensureCursorVisible ();
}

void MainWindow::change_mode (edit_mode in_mode)
{
    mode = in_mode;
    std::string console = "switched to ";
    switch (in_mode) {
    case draw:
        console += "'draw'";
        ui->text_tool->setText("draw");
        ui->drawButton->setDown(1);
        ui->selectButton->setDown(0);
        break;
    case sel:
        console += "'select'";
        ui->text_tool->setText("select");
        ui->selectButton->setDown(1);
        ui->drawButton->setDown(0);
        break;
    default:
        break;
    }
    console += " mode";
#ifdef DEBUG_MISC
    print_console(console);
#endif
    emit ogl_change_mode(in_mode);
}

void MainWindow::on_actionSave_triggered()
{
    if (outfile.is_open ()) {
        outfile.close ();
        outfile.open (outfilename, outfile.binary | outfile.out | outfile.trunc); //truncate
        if (!outfile.is_open()) {
            print_console ("failed to truncate file while saving");
            return;
        }
        level.save_level (outfile);
    }
    else {
        open_file_dialog (save);
    }
}

void MainWindow::on_actionLoad_triggered ()
{
    if (0){
        //fixme::unsaved dialog
    }
    else {
        open_file_dialog (load);
    }
}

void MainWindow::on_actionDelete_wall_triggered ()
{
    level.delete_wall ();
    ogl_out->update ();
}

void MainWindow::open_file_dialog (flag_saveload flag)
{
    opendialog = new OpenDialog(this, flag);
    QObject::connect(opendialog, &OpenDialog::filename_read,
                     this, &MainWindow::on_opendialog_finish);
    opendialog->show();
}

void MainWindow::on_opendialog_finish(const std::string &filename, flag_saveload flag)
{
    outfilename = filename;
    std::string console;
    console = "opening for ";
    if (flag == save)
        console += "save '";
    else
        console += "load '";
    console += filename;
    console += "' ";
    if (!std::filesystem::exists(filename) && flag == load){ // if no file to load
        console += "FAILED: check existing";
        print_console (console);
        return;
    }
    //file exists
    if (flag == save) {
        outfile.open(filename, outfile.binary | outfile.out | outfile.trunc);
        if (!outfile.is_open()) {
            print_console(console + "FAILED");
            return;
        }
        print_console(console + "SUCCESS");
        level.save_level (outfile);
    }
    if (flag == load) {
        std::ifstream infile;
        infile.open(filename, infile.binary | infile.in);
        if (!infile.is_open()) {
            print_console(console + "FAILED for read");
            return;
        }
        if (level.load_level (infile)) {
            infile.close ();
            print_console (console + "SUCCESS for read");
            print_console ("level loading failed!");
            return;
        }
        infile.close ();
        outfile.open(filename, outfile.binary | outfile.out | outfile.in);
        if (!outfile.is_open()) {
            print_console(console + "FAILED for write");
            return;
        }
        print_console (console += "SUCCESS");
        ogl_out->update ();
    }
}
