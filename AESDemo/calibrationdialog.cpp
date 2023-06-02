#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"

extern QSqlDatabase db;

calibrationDialog::calibrationDialog(QWidget *parent) : QDialog(parent), ui(new Ui::calibrationDialog)
{
    ui->setupUi(this);
    currentTime = QDateTime::currentDateTime();
    ui->lineEdit1->setText(currentTime.toString("yyyyMMdd"));


}

calibrationDialog::~calibrationDialog()
{
    delete ui;
}

void calibrationDialog::on_btnCertain_clicked()
{
    QString CodeNums = ui->lineEdit1->text();
    QString numbers = ui->lineEdit2->text();
    QString lowTarget1 = ui->lineEdit3->text();
    QString lowTarget2 = ui->lineEdit5->text();
    QString highTarget1 = ui->lineEdit4->text();
    QString highTarget2 = ui->lineEdit6->text();
//    qDebug()<<CodeNums.isEmpty()<<CodeNums.isNull();
    if ( CodeNums.isEmpty()|| numbers.isEmpty()||lowTarget1.isEmpty()
         ||lowTarget2.isEmpty()||highTarget1.isEmpty()||highTarget2.isEmpty()){

        QMessageBox::information(this, "提示", "输入框为空！");
        return;
    }
    ui->textEdit->clear();
    int lowValue1 = floor( lowTarget1.toDouble()*10);
    int lowValue2 = floor( lowTarget2.toDouble()*10);

    int highValue1 = floor( highTarget1.toDouble()*10);
    int highValue2 = floor( highTarget2.toDouble()*10);
    QString highStr1 = QString("%1").arg(highValue1, 3, 10, QLatin1Char('0'));
    QString highStr2 = QString("%1").arg(highValue2, 3, 10, QLatin1Char('0'));
    QString finalStr = QString("%1%2%3%4%5").arg(CodeNums).arg(lowValue1).arg(lowValue2).arg(highStr1).arg(highStr2);
    for (int i = 0; i < numbers.toInt(); i++){
        ui->textEdit->append(finalStr);
    }

}

void calibrationDialog::on_btnSave_clicked()
{
    if (ui->textEdit->toPlainText().isEmpty()) {
        QMessageBox::information(this, "提示", "内容为空！");
        return;
    }

    QDateTime todayDate = QDateTime::currentDateTime();
    QString dateString = todayDate.toString("yyyyMMddHHmmss");
    QString aFileName = QString("试剂条码%1.txt").arg(dateString);

    //保存文本
    QFile aFile(aFileName);
    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream aStream(&aFile);                               //用文本流读取文件
    QString str=ui->textEdit->toPlainText(); //条码转换为字符串
    aStream<<str;      //写入文本流
    aFile.close();          //关闭文件

    QMessageBox::information(this, "提示", "保存成功！");
}
