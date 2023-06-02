#include "usedialog.h"
#include "ui_usedialog.h"

UseDialog::UseDialog(QWidget *parent) : QDialog(parent), ui(new Ui::UseDialog)
{
    ui->setupUi(this);

    QString helpStr;

    this->setFixedWidth(500);
    this->setFixedHeight(400);
    helpStr.append(tr("       当前版本：version20230506\n"));
    helpStr.append(tr("       更新说明：1.调整打印内容\n"));
    helpStr.append(tr("                        2.新增导出excel数据表\n"));
    helpStr.append(tr("                        3.修改数据库类目和打印类目\n"));
    helpStr.append(tr("\n"));
    helpStr.append(tr("       更新时间：2023-05-06\n"));
    helpStr.append(tr("\n"));
    helpStr.append(tr("            深圳市雷诺华科技实业有限公司\n"));

    ui->label->setText(helpStr);
}

UseDialog::~UseDialog()
{
    delete ui;
}
