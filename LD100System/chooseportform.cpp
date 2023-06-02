#include "chooseportform.h"
#include "ui_chooseportform.h"

extern QString reportTitle;
extern double lowValue, highValue;
extern int paperSize;

extern QString configFilePath;
extern QStringList doctorNames, inspectorNames, reviewerNames;

choosePortForm::choosePortForm(QWidget *parent) : QWidget(parent), ui(new Ui::choosePortForm)
{
    ui->setupUi(this);

    ui->comboBox2->setCurrentIndex(7);

    ui->comboBox3->setCurrentIndex(paperSize);
    ui->lineEdit->setText(reportTitle);
//    ui->lineEdit1->setText(QString("%1").arg( lowValue));
//    ui->lineEdit2->setText(QString("%1").arg( highValue));
    ui->lineEdit1->setText( QString::number(lowValue,'f',1) );
    ui->lineEdit2->setText( QString::number(highValue,'f',1) );

    ui->comboBox5->setCurrentIndex(0);
    ui->comboBox4->clear();
    ui->comboBox4->addItems(doctorNames);

    findPort();

    if (!usePortName.isEmpty()){
        //ui->comboBox1->setCurrentText(usePortName);
        ui->comboBox1->addItem(usePortName);        //显示串口
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
//确定按钮
void choosePortForm::on_btnPrintSet_clicked()
{
    paperSize = ui->comboBox3->currentIndex();
    reportTitle = ui->lineEdit->text();
    lowValue = ui->lineEdit1->text().toDouble();
    highValue = ui->lineEdit2->text().toDouble();

//    if (ui->comboBox5->currentIndex() == 0){
//        emit sendChangeData(0, doctorNames);

//    }else if (ui->comboBox5->currentIndex() == 1) {
//        emit sendChangeData(1, inspectorNames);

//    }else if (ui->comboBox5->currentIndex() == 2) {
//        emit sendChangeData(2, reviewerNames);

//    }
    emit sendChangeData(doctorNames, inspectorNames, reviewerNames);

    QSettings configWrite(configFilePath, QSettings::IniFormat);

    configWrite.beginGroup("PRINT_SET");
    //1对应A4纸张，2对应A5纸张
    configWrite.setValue("PAGE_SIZE", paperSize);
    configWrite.setValue("PRINT_TITLE", reportTitle);
    configWrite.setValue("LOW_VALUE", lowValue);
    configWrite.setValue("HIGH_VALUE", highValue);
    configWrite.endGroup();

    configWrite.beginGroup("NAME_SET");
    configWrite.setValue("DOCTOR_NAMES", doctorNames);
    configWrite.setValue("INSPECTOR_NAMES", inspectorNames);
    configWrite.setValue("REVIEWER_NAMES", reviewerNames);
    configWrite.endGroup();

    this->close();
}

void choosePortForm::on_btnDelete_clicked()
{
//    ui->comboBox4->removeItem(ui->comboBox4->currentIndex());
    if(ui->comboBox5->currentIndex() == 0){
        doctorNames.removeAt(ui->comboBox4->currentIndex());

        ui->comboBox4->clear();
        ui->comboBox4->addItems(doctorNames);

    }else if (ui->comboBox5->currentIndex() == 1) {
        inspectorNames.removeAt(ui->comboBox4->currentIndex());

        ui->comboBox4->clear();
        ui->comboBox4->addItems(inspectorNames);

    }else if (ui->comboBox5->currentIndex() == 2) {
        reviewerNames.removeAt(ui->comboBox4->currentIndex());

        ui->comboBox4->clear();
        ui->comboBox4->addItems(reviewerNames);

    }

}

//添加按钮
void choosePortForm::on_btnAdd_clicked()
{
//    namesList.append(ui->lineEdit3->text());
    if(ui->lineEdit3->text().isEmpty()){
        QMessageBox::information(this, tr("提示"), tr("输入为空！"), QMessageBox::Ok);
        return;
    }

//    doctorNames.insert(0, ui->lineEdit3->text());
//    qDebug()<<namesList;
//    ui->comboBox4->clear();
//    ui->comboBox4->addItems(doctorNames);

    if(ui->comboBox5->currentIndex() == 0){
        doctorNames.insert(0, ui->lineEdit3->text());
        ui->comboBox4->clear();
        ui->comboBox4->addItems(doctorNames);

    }else if (ui->comboBox5->currentIndex()==1) {
        inspectorNames.insert(0, ui->lineEdit3->text());
        ui->comboBox4->clear();
        ui->comboBox4->addItems(inspectorNames);

    }else if (ui->comboBox5->currentIndex()==2) {
        reviewerNames.insert(0, ui->lineEdit3->text());
        ui->comboBox4->clear();
        ui->comboBox4->addItems(reviewerNames);

    }

}

//
//void choosePortForm::on_comboBox4_currentIndexChanged(int index)
//{
//    if (ui->comboBox5->currentIndex() == 0){
//        QString doctorName = ui->comboBox4->currentText();
//        emit sendChangeData(0, doctorName);

//    }else if (ui->comboBox5->currentIndex() == 1) {
//        QString inspectorName = ui->comboBox4->currentText();
//        emit sendChangeData(1, inspectorName);

//    }else if (ui->comboBox5->currentIndex() == 2) {
//        QString reviewerName = ui->comboBox4->currentText();
//        emit sendChangeData(2, reviewerName);

//    }

//}


void choosePortForm::on_comboBox5_currentIndexChanged(int index)
{
    if(index == 0){
        ui->comboBox4->clear();
        ui->comboBox4->addItems(doctorNames);

    }else if (index == 1) {
        ui->comboBox4->clear();
        ui->comboBox4->addItems(inspectorNames);

    }else if (index == 2) {
        ui->comboBox4->clear();
        ui->comboBox4->addItems(reviewerNames);

    }
}

