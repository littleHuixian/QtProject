#ifndef DIALOGTHREE_H
#define DIALOGTHREE_H

#include <QDialog>
#include<QDebug>
#include<opencv2/opencv.hpp>
#include<highgui.hpp>

using namespace cv;

namespace Ui {
class DialogThree;
}

class DialogThree : public QDialog
{
    Q_OBJECT

public:
    explicit DialogThree(QWidget *parent = nullptr);
    ~DialogThree();

private:
    Ui::DialogThree *ui;
};

#endif // DIALOGTHREE_H
