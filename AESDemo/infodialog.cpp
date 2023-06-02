#include "infodialog.h"
#include "ui_infodialog.h"

infoDialog::infoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::infoDialog)
{
    ui->setupUi(this);
    ui->infoLabel->setText("当前版本：V 3.0 \n发布时间：2020-02-26 \n更新时间：2022-03-03");
}

infoDialog::~infoDialog()
{
    delete ui;
}
