#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>


namespace Ui {
class TableDialog;
}

class TableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableDialog(QWidget *parent = 0);
    ~TableDialog();

private:
    Ui::TableDialog *ui;


};

#endif // TABLEDIALOG_H
