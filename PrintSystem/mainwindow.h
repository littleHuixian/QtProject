#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrintPreviewWidget>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QPageLayout>
#include <QPainter>
#include <QTcpSocket>
#include <QTcpServer>
#include <QFile>
#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "setdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void parseJsonData(QByteArray jsonData);

private slots:
    void on_btnLink_clicked();
    void on_btnPreview_clicked();
    void on_btnPrint_clicked();
    void on_btnSet_clicked();

    void initTable();
    void readConfig();

    void newChannel();
    void readBufferData();
    //void socketDisconnected();

    void printA4Slot(QPrinter *printerPixmap);
    void printA5Slot(QPrinter *printerPixmap);

    void on_btnDefault_clicked();
    void on_btnOk_clicked();

private:
    Ui::MainWindow *ui;

    QLabel *infoLabel;
    QDateTime currentTime;

    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QString setAddr;
    int setPort;
    QString printTitle;
    int paperSize;
    QString configFilePath;

    SetDialog *setWin;

};
#endif // MAINWINDOW_H
