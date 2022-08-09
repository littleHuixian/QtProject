#ifndef DIALOGONE_H
#define DIALOGONE_H

#include <QDialog>
#include<opencv2/opencv.hpp>
#include<opencv2\highgui.hpp>

using namespace cv;

namespace Ui {
class DialogOne;
}

class DialogOne : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOne(QWidget *parent = nullptr);
    ~DialogOne();

    void imageShow();
    void imgProc(float con, int bri);

private slots:
    void on_contrastVerticalSlider_sliderMoved(int position);

    void on_contrastVerticalSlider_valueChanged(int value);

    void on_brightnessVerticalSlider_sliderMoved(int position);

    void on_brightnessVerticalSlider_valueChanged(int value);

private:
    Ui::DialogOne *ui;

    Mat myImg;
    QImage myQImg;
};

#endif // DIALOGONE_H
