#ifndef USEDIALOG_H
#define USEDIALOG_H

#include <QDialog>

namespace Ui {
class UseDialog;
}

class UseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UseDialog(QWidget *parent = nullptr);
    ~UseDialog();

private:
    Ui::UseDialog *ui;
};

#endif // USEDIALOG_H
