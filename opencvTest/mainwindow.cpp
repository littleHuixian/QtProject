#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //图片测试
    cv::Mat image = imread("E:/QtProjectFiles/opencvTest/1.jpg");
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow("Display window",image);
    waitKey(0);

    //视频播放测试
    //    namedWindow("Display", WINDOW_AUTOSIZE);
    //    VideoCapture cap;
    //    cap.open("E:/QtProjectFiles/opencvTest/video1.mp4");
    //    if(cap.isOpened())
    //        qDebug()<<"Success!";
    //    else
    //        qDebug()<<"Failure!";
    //    Mat frame;
    //    for(;;){
    //        cap >> frame;
    //        if(frame.empty())
    //            break;
    //        imshow("Display", frame);
    //        if(waitKey(33) >= 0)
    //            break;
    //    }

    //测试opencv-contrib
    //    Mat image = imread("E:/QtProjectFiles/opencvTest/1.jpg",  IMREAD_ANYCOLOR);
    //    Mat g_srcImage, g_grayImage, g_dstImage;

    //    if (nullptr == image.data){
    //        QMessageBox::critical(this, "Error", "img no data");

    //    } else {
    //        Mat z1, z2;
    //        cvtColor(image, z1, COLOR_BGR2GRAY);
    //        adaptiveThreshold(z1, g_grayImage, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 7, 25);
    //        threshold(g_grayImage, g_dstImage, g_nThresholdValue, 255, g_nThresholdType);
    //        imshow("Image Source", image);
    //        imshow("Image Output", g_dstImage);

    //    }

}

MainWindow::~MainWindow()
{
    delete ui;
}



