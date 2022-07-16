#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"dialogone.h"

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

private:
    Ui::MainWindow *ui;
    DialogOne *DemoOne;
};
#endif // MAINWINDOW_H
