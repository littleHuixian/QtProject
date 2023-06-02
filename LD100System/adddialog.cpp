#include "adddialog.h"
#include "ui_adddialog.h"

extern QSqlQueryModel sqlModel;
extern QString configFilePath;

addDialog::addDialog(QWidget *parent) : QDialog(parent), ui(new Ui::addDialog)
{
    ui->setupUi(this);

    ui->lineEdit12->setText("LD-100");
    QDateTime reportTime = QDateTime::currentDateTime();
    QString searchTime = reportTime.toString("yyyy-MM-dd");
    //报告时间
    ui->lineEdit13->setText(searchTime);
    ui->lineEdit14->setText(searchTime);

}

addDialog::~addDialog()
{
    delete ui;
}

void addDialog::getDataString(QStringList a, QStringList b, QStringList c)
{
    ui->comboBox3->clear();
    ui->comboBox3->addItems(a);

    ui->comboBox4->clear();
    ui->comboBox4->addItems(b);

    ui->comboBox5->clear();
    ui->comboBox5->addItems(c);
}

void addDialog::on_btnAdd_clicked()
{
    int dataRows = sqlModel.rowCount();

    QString name            = ui->lineEdit1->text();
    QString gender          = ui->comboBox2->currentText();
    QString age               = ui->lineEdit3->text();
    QString sickId            = ui->lineEdit4->text();
    QString bedId            = ui->lineEdit5->text();
    QString sampleCode  = ui->lineEdit6->text();
    QString sampleNum   = ui->lineEdit7->text();
    QString department   = ui->lineEdit8->text();
    QString doctor           = ui->comboBox3->currentText();

    QString diagnosis       = ui->lineEdit10->text();
    QString note              = ui->lineEdit11->text();

    QString machin         = ui->lineEdit12->text();
    QString testTime       = ui->lineEdit13->text(); //取日期
    QString reportTime   = ui->lineEdit14->text();
    QString checkName = ui->comboBox4->currentText();
    QString auditor        = ui->comboBox5->currentText();

    QString testResult = ui->lineEdit17->text();

    QString execSql = QString("insert into saveTable(ID, 日期, 姓名,性别,年龄,病例号,床号,样本条码, 样本编号,科室,医生,"
                              "检测机器,HbA1c, 检验者, 审核者, 备注, 临床诊断)"
                              " values ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15', '%16', '%17')")
            .arg(dataRows+1).arg(testTime).arg(name).arg(gender).arg(age).arg(sickId).arg(bedId).arg(sampleCode)
            .arg(sampleNum).arg(department).arg(doctor).arg(machin).arg(testResult).arg(checkName).arg(auditor).arg(note).arg(diagnosis);

    emit sendString(execSql);
}


