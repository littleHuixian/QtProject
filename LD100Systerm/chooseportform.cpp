#include "chooseportform.h"
#include "ui_chooseportform.h"

extern QString reportTitle;
extern double lowValue, highValue;
extern int paperChoose;

choosePortForm::choosePortForm(QWidget *parent) : QWidget(parent), ui(new Ui::choosePortForm)
{
    ui->setupUi(this);

    ui->comboBox2->setCurrentIndex(7);

    ui->lineEdit->setText(tr("检验报告单"));
    ui->lineEdit1->setText("4.0");
    ui->lineEdit2->setText("6.0");
    reportTitle = ui->lineEdit->text();
    lowValue = ui->lineEdit1->text().toDouble();
    highValue = ui->lineEdit2->text().toDouble();

    findPort();

    if (!usePortName.isEmpty()){
        //ui->comboBox1->setCurrentText(usePortName);
        ui->comboBox1->addItem(usePortName);        //显示串口name
    }
}

choosePortForm::~choosePortForm()
{
    delete ui;
}

void choosePortForm::on_btnCertain_clicked()
{
    usePortName = ui->comboBox1->currentText();
    useBaud = ui->comboBox2->currentText();
    QStringList portSet;
    portSet<<usePortName<<useBaud;
    emit sendPortSetData(portSet);
    this->close();
}

void choosePortForm::findPort()
{
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QSerialPort serial;
        serial.setPort(info);   //设置串口
        if( serial.open(QIODevice::ReadWrite) ) {
            ui->comboBox1->addItem(serial.portName());        //显示串口name

            serial.close();
        }
    }
}

void choosePortForm::on_btnDefaultPrint_clicked()
{
    ui->lineEdit->setText(tr("检验报告单"));
    ui->lineEdit1->setText("4.0");
    ui->lineEdit2->setText("6.0");
}

void choosePortForm::on_btnPrintSet_clicked()
{
    paperChoose = ui->comboBox3->currentIndex();
    reportTitle = ui->lineEdit->text();
    lowValue = ui->lineEdit1->text().toDouble();
    highValue = ui->lineEdit2->text().toDouble();
    this->close();
}
