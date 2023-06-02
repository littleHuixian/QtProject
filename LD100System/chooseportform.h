#ifndef CHOOSEPORTFORM_H
#define CHOOSEPORTFORM_H

#include <QWidget>
#include"header.h"

namespace Ui {
class choosePortForm;
}

class choosePortForm : public QWidget
{
    Q_OBJECT

public:
    explicit choosePortForm(QWidget *parent = nullptr);
    ~choosePortForm();

    void findPort();

signals:
    void sendPortSetData(QStringList);   //用来传递数据的信号
    void sendPrintSetData(QStringList);
//    void sendChangeData(int sIndex, QStringList sendName);
    void sendChangeData(QStringList dNames, QStringList iNames, QStringList rNames);

private slots:
    void on_btnCertain_clicked();
    void on_btnDefaultPrint_clicked();
    void on_btnPrintSet_clicked();
    void on_btnDelete_clicked();
    void on_btnAdd_clicked();

//    void on_comboBox4_currentIndexChanged(int index);

    void on_comboBox5_currentIndexChanged(int index);

private:
    Ui::choosePortForm *ui;
    QString usePortName;
    QString useBaud;

};

#endif // CHOOSEPORTFORM_H
