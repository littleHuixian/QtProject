#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"dialogone.h"
#include"dialogtwo.h"
#include"dialogthree.h"

#include <QMainWindow>
#include<QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnDemo1_clicked();

    void on_btnDemo2_clicked();

    void on_btnDemo3_clicked();

private:
    Ui::MainWindow *ui;
    DialogOne *DemoOne;
    DialogTwo *DemoTwo;
    DialogThree *DemoThree;
};
#endif // MAINWINDOW_H
