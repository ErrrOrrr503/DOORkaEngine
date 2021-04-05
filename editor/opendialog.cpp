#include "opendialog.h"
#include "ui_opendialog.h"

OpenDialog::OpenDialog(QWidget *parent, flag_saveload in_flag) : QDialog(parent), ui(new Ui::OpenDialog)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);
    filename_layout = new QHBoxLayout(ui->frame_filename);
    filename_layout->setContentsMargins(0, 0, 0, 0);
    filename_layout->setSpacing(0);
    filename_field = new filename_enter (ui->frame_filename);
    filename_layout->addWidget (filename_field);
    filename_field->show ();
    filename_field->setCursorWidth (1);
    filename_field->ensureCursorVisible ();
    flag = in_flag;
}

OpenDialog::~OpenDialog()
{
    delete ui;
}

void OpenDialog::on_opendialog_openButton_clicked()
{
    filename = filename_field->toPlainText().toStdString();
    emit filename_read (filename, flag);
    close();
}

void OpenDialog::on_opendialog_cancelButton_clicked()
{
    close();
}

void OpenDialog::on_actionMainAction_triggered()
{
    on_opendialog_openButton_clicked();
}

void OpenDialog::on_opendialog_selectButton_clicked()
{
    filename = QFileDialog::getOpenFileName (this, "Open level").toUtf8 ().constData ();
    filename_field->clear ();
    filename_field->insertPlainText (QString::fromStdString (filename));
}
