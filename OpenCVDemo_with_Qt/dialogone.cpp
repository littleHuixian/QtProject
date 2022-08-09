#include "dialogone.h"
#include "ui_dialogone.h"

DialogOne::DialogOne(QWidget *parent) :  QDialog(parent), ui(new Ui::DialogOne)
{
    ui->setupUi(this);

    //load images
    Mat image = imread("E:/QtProjectFiles/opencvTest/1.jpg");

    //cvtColor( InputArray src, OutputArray dst, int code, int dstCn = 0 );
    cvtColor(image, image, COLOR_BGR2RGB);   //Mat的图片是BGR格式，需要先转换为RGB格式
    myImg = image;
    //在窗口控件中显示图片
    myQImg = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_RGB888);

    imageShow();
}

DialogOne::~DialogOne()
{
    delete ui;
}
//显示图片
void DialogOne::imageShow()
{
    ui->labelShow->setPixmap(QPixmap::fromImage(myQImg));
}
//图像增强处理，接受参数分别为对比度和亮度系数
void DialogOne::imgProc(float con, int bri)
{
    Mat imgSrc = myImg;
    Mat imgDst = Mat::zeros(imgSrc.size(), imgSrc.type());
    imgSrc.convertTo(imgDst, -1, con, bri);
    myQImg = QImage((const unsigned char*)(imgDst.data), imgDst.cols, imgDst.rows, QImage::Format_RGB888);
    imageShow();
}
//对比度
void DialogOne::on_contrastVerticalSlider_sliderMoved(int position)
{
    imgProc(position / 33.3, 0);
}


void DialogOne::on_contrastVerticalSlider_valueChanged(int value)
{
    imgProc(value / 33.3, 0);
    ui->labelValue1->setText(QString("%1").arg(value));
}

//亮度
void DialogOne::on_brightnessVerticalSlider_sliderMoved(int position)
{
    imgProc(1.0, position);
}


void DialogOne::on_brightnessVerticalSlider_valueChanged(int value)
{
    imgProc(1.0, value);
    ui->labelValue2->setText(QString("%1").arg(value));
}

