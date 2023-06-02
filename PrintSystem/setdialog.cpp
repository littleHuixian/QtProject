#include "setdialog.h"
#include "ui_setdialog.h"

//extern QString setAddr;
//extern int setPort;

//extern QString printTitle;
//extern int paperSize;

SetDialog::SetDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SetDialog)
{
    ui->setupUi(this);

    this->setFixedSize(680, 930);

    QPixmap pixmap1(":/help1.png");
    ui->label1->setPixmap(pixmap1.scaled(ui->label1->size(), Qt::KeepAspectRatio));

    QPixmap pixmap2(":/help2.png");
    ui->label2->setPixmap(pixmap2.scaled(ui->label2->size(), Qt::KeepAspectRatio));

}

SetDialog::~SetDialog()
{
    delete ui;
}


