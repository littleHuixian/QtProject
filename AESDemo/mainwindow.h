#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<qaesencryption.h>
#include<QMessageBox>
#include<QLabel>
#include<QDateTime>
#include<QTimer>
#include<algorithm>
#include<QtPrintSupport/QPrintDialog>
#include<QtPrintSupport/QPrinter>
#include<QFileDialog>
#include<QDebug>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QSqlError>
#include<QSqlDriver>
#include<QSqlRecord>
#include"infodialog.h"
#include"tablewindow.h"
#include"calibrationdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnEncryption_clicked();         //加密按钮
    void on_btnGenerateCode_clicked();
    void on_btnClearCode_clicked();
    void on_btnClearEncry_clicked();
    void on_btnSave_clicked();
    void on_btnPrint_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_btnSaveAs_clicked();
    void on_actionInfo_triggered();
    void on_actionCheck_triggered();

//    void on_btnGenerateBoxCode_clicked();
    void on_actioncode_triggered();

    void on_cBoxCategory1_currentIndexChanged(int index);

    void on_cBoxCategory0_currentIndexChanged(int index);

protected:
    void systemTimeUpdate();
    QString decTo48(qint64 src, int nMinLen);

private:
    Ui::MainWindow *ui;

    QString keyString;
    QString changKey;
    QByteArray oldHashKey, hashKey;

    QDateTime currentDataTime;
    QSqlQueryModel sqlModel;

    QLabel *dbinfoLabel;
    QLabel *systemTimeLabel;
    QString category, machine, codeCategory, codeNomal;
    QStringList generCode;

};

#endif // MAINWINDOW_H
