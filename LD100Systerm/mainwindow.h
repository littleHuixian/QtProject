#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"header.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void find_port();       //查找可用串口
    void sleep(int msec);      //延时函数
    void initTable1();
    void initTable2();
    void getFormatData(QString a);
    void receivePortSetData(QStringList data);   //接收打印设置传递过来的数据的槽
    //void receivePrintSetData(QStringList data);   //接收打印设置传递过来的数据的槽
    void printPreviewSlot(QPrinter *printerPixmap);
    void previewSlot(QPrinter *printerPixmap);
    void on_actionLink_triggered();
    void on_actionSet_triggered();
    void on_btnSave_clicked();
    void on_btnSearch_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_btnBack_clicked();
    void on_btnPreview_clicked();
    void on_btnPrint_clicked();
    void on_actionPreview_triggered();
    void on_actionPrint_triggered();
    void on_actionClear_triggered();
    void on_actionDelete_triggered();

protected:
    void systemTimeUpdate();
    void setTable1Data(const QModelIndex &index);
    void Read_Date();
    void visualData(const QModelIndex &index);
    void on_tableView2_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QLabel *systemTimeLabel, *infoLabel, *tableLabel;
    QPrinter *printer;
    QPrintPreviewWidget *preview;
    bool portState;
    QSerialPort *serialport;
    QString portName;
    QString dataStream;

    choosePortForm *portWindow;
    editDataDialog *editWindow;

    QString usePortName;
    QString useBaud;

    QStandardItemModel* model1;
    QSqlDatabase database;

};

#endif // MAINWINDOW_H
