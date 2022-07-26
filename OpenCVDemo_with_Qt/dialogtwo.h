#ifndef DIALOGTWO_H
#define DIALOGTWO_H

#include <QDialog>
#include<opencv2/opencv.hpp>

using namespace cv;

namespace Ui {
class DialogTwo;
}

class DialogTwo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTwo(QWidget *parent = nullptr);
    ~DialogTwo();

protected:
    void imageShow();

private slots:
    void on_verticalSlider_sliderMoved(int position);

    void on_verticalSlider_valueChanged(int value);

private:
    Ui::DialogTwo *ui;

    Mat myImg;
    QImage blurQImg;
    QImage gausssianQImg;
    QImage medianQImg;
    QImage bilateralQImg;
};

#endif // DIALOGTWO_H
