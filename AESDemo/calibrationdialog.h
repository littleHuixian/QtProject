#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>
#include<QSqlDatabase>
#include<QDateTime>
#include<QMessageBox>
#include<QFile>
#include<QDebug>
#include<cmath>

namespace Ui {
class calibrationDialog;
}

class calibrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit calibrationDialog(QWidget *parent = 0);
    ~calibrationDialog();

private slots:
    void on_btnCertain_clicked();

    void on_btnSave_clicked();

private:
    Ui::calibrationDialog *ui;
    QDateTime currentTime;
};

#endif // CALIBRATIONDIALOG_H
