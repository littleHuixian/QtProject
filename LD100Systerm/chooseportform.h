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

private slots:
    void on_btnCertain_clicked();

    void on_btnDefaultPrint_clicked();

    void on_btnPrintSet_clicked();

private:
    Ui::choosePortForm *ui;
    QString usePortName;
    QString useBaud;

};

#endif // CHOOSEPORTFORM_H
