#ifndef EDITDATADIALOG_H
#define EDITDATADIALOG_H

#include <QDialog>
#include"header.h"

namespace Ui {
class editDataDialog;
}

class editDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editDataDialog(QWidget *parent = nullptr);
    ~editDataDialog();

    void getSelectIndex(const QModelIndex &index);

private slots:
    void on_btnUpdata_clicked();
    void on_btnPreview_clicked();
    void on_btnPrint_clicked();
    void printPreviewSlot(QPrinter *printerPixmap);
    void previewSlot(QPrinter *printerPixmap);

private:
    Ui::editDataDialog *ui;

    int selectRow;
};

#endif // EDITDATADIALOG_H
