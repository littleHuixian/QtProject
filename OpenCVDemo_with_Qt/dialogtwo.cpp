#include "dialogtwo.h"
#include "ui_dialogtwo.h"

#include<QDebug>

DialogTwo::DialogTwo(QWidget *parent) : QDialog(parent),  ui(new Ui::DialogTwo)
{
    ui->setupUi(this);

    //load images
    Mat image = imread("E:/QtProjectFiles/opencvTest/1.jpg");
    if (!image.data){
        qDebug()<<"could not load image....";
        return;
    }
    int cols = (image.cols-1) * image.channels();
    int offsetx = image.channels();
    int rows = image.rows;

    Mat dst = Mat::zeros(image.size(), image.type());
    for (int row=1; row<(row-1); row++) {
        const uchar* previous = image.ptr<uchar>(row - 1);
        const uchar* current = image.ptr<uchar>(row);
        const uchar* next = image.ptr<uchar>(row + 1);
        uchar* output = dst.ptr<uchar>(row);
        for (int col=offsetx; col<cols; col++) {
            output[col] = saturate_cast<uchar>(0.5*current[col] - (current[col- offsetx] + current[col+ offsetx] + previous[col] + next[col]));
        }
    }

    cvtColor(image, image, COLOR_BGR2RGB);   //Mat的图片是BGR格式，需要先转换为RGB格式
    myImg = image;

    Mat dstImage = Mat::zeros(image.size(), image.type());
    //定义掩膜
    Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    filter2D(image, dstImage, image.depth(), kernel);

    //在窗口控件中显示图片
    blurQImg = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_RGB888);
    gausssianQImg = QImage((const unsigned char*)(dst.data), dst.cols, dst.rows, QImage::Format_RGB888);
    medianQImg = QImage((const unsigned char*)(dstImage.data), dstImage.cols, dstImage.rows, QImage::Format_RGB888);
    imageShow();
}

DialogTwo::~DialogTwo()
{
    delete ui;
}

void DialogTwo::imageShow()
{
    ui->labelImage1->setPixmap(QPixmap::fromImage(blurQImg.scaled(ui->labelImage1->size(), Qt::KeepAspectRatio)));
    ui->labelImage2->setPixmap(QPixmap::fromImage(gausssianQImg.scaled(ui->labelImage2->size(), Qt::KeepAspectRatio)));
    ui->labelImage3->setPixmap(QPixmap::fromImage(medianQImg.scaled(ui->labelImage3->size(), Qt::KeepAspectRatio)));
}

void DialogTwo::on_verticalSlider_sliderMoved(int position)
{

}


void DialogTwo::on_verticalSlider_valueChanged(int value)
{

}

