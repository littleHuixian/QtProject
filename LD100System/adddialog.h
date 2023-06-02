#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include"header.h"

namespace Ui {
class addDialog;
}

class addDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDialog(QWidget *parent = nullptr);
    ~addDialog();

public:
    void getDataString(QStringList a, QStringList b, QStringList c);


signals:
    void sendString(QString exceStr);   //用来传递数据的信号

private slots:
    void on_btnAdd_clicked();


private:
    Ui::addDialog *ui;
};

#endif // ADDDIALOG_H
