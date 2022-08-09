#include "dialogthree.h"
#include "ui_dialogthree.h"

DialogThree::DialogThree(QWidget *parent) :  QDialog(parent), ui(new Ui::DialogThree)
{
    ui->setupUi(this);
    Mat srcImage = imread("E:/QtProjectFiles/opencvTest/1.jpg");
    if (!srcImage.data){
        qDebug()<<"could not load image....";
        return;
    }
    QImage QImg1 = QImage((const unsigned char*)(srcImage.data), srcImage.cols, srcImage.rows, QImage::Format_RGB888);
    ui->labelImage1->setPixmap(QPixmap::fromImage(QImg1));

    CV_Assert(srcImage.depth() == CV_8U);
    //定义掩膜
    Mat result;
    Mat kernel = (Mat_<char>(3,3) <<0, -1, 0, -1, 5, -1, 0, -1, 0);
    filter2D(srcImage, result, srcImage.depth(), kernel);

    QImage QImg2 = QImage((const unsigned char*)(result.data), result.cols, result.rows, QImage::Format_RGB888);
    ui->labelImage2->setPixmap(QPixmap::fromImage(QImg2));
}

DialogThree::~DialogThree()
{
    delete ui;
}
