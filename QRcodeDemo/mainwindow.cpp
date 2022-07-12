#include "mainwindow.h"
#include "ui_mainwindow.h"

#include"src/myqrcodeheader.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*******************************************************************
     *
     *  Autor:
     *  Time :   2018.3.7
     *  Version: 1.0.0
     *
     *  函数：
     *    MyQRcode(const QByteArray text,const QSize size)；构造函数，参数信息为二维码数据，二维码的大小
     *
     *    setQRcodeMargin(const int &margin=10)；设置图片中二维码距离四周边缘的距离
     *
     *    setQRcodeIcon(const QPixmap &icon, qreal percent=0.23)；设置二维码中心图片，参数为图片内容，图片在整个二维码高度的比例
     *
     *    setQRcodeInfo(QByteArray str);设置二维码的内容
     *
     *    setQRcodeSize(QSize size)；设置二维码的大小
     *
     *    QRCodeGenerate(QPixmap &pix);生成二维码并保存在pixmap中
     *
     *
     * 调用举例：
     *
     *   1、构造QRcode对象
     *   MyQRcode qrcode("https://www.baidu.com",QSize(300,300));
     *
     *   2、设置中心图片
     *   qrcode.setQRcodeIcon(QPixmap(":/new/prefix1/logo.png"),0.2);
     *
     *   3、设置二维码信息
     *   qrcode.setQRcodeInfo("你好，老哥！！！");
     *
     *   4、设置边缘距离
     *   qrcode.setQRcodeMargin(10);
     *
     *   5、定义一个pixmap对象用于保存生成的二维码图像
     *   QPixmap pix;
     *
     *   6、传入pixmap对象并生成二维码
     *   qrcode.QRCodeGenerate(pix);
     *
     *   7、通过pixmap对二维码进行使用
     *   ui->label->setPixmap(pix);
     *
    *******************************************************************/

    ui->labelText->setFixedSize(300, 20);
    ui->labelCode->setFixedSize(300, 300);

    QString str = "测试条码";
    QByteArray codeStr = str.toUtf8();
    MyQRcode qrcode(codeStr, QSize(300, 300));
    QPixmap pix;
    qrcode.QRCodeGenerate(pix);
    ui->labelCode->setPixmap(pix);

    QZXing zxing;
    QString str1 = zxing.decodeImage(QImage(pix.toImage()));
    ui->labelText->setText(str1);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnCode_clicked()
{
    QString str = ui->lineEdit->text();
    QByteArray codeStr = str.toUtf8();
    if ( !str.isEmpty() ){
        MyQRcode qrcode(codeStr, QSize(300, 300));
        QPixmap pix;
        qrcode.QRCodeGenerate(pix);
        ui->labelCode->setPixmap(pix);
        //二维码解码

        QZXing zxing;
        QString str1 = zxing.decodeImage(QImage(pix.toImage()));
        ui->labelText->setText(str1);
    }
}

void MainWindow::on_btnClear_clicked()
{
    ui->labelText->clear();
    ui->labelCode->clear();
    ui->lineEdit->clear();
}
