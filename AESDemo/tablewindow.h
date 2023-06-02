#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QMainWindow>
#include<QSqlDatabase>
#include<QSqlTableModel>
#include<QSqlQueryModel>
#include<QLabel>
#include<QTableView>
#include<QDebug>

namespace Ui {
class TableWindow;
}

class TableWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TableWindow(QWidget *parent = 0);
    ~TableWindow();

private slots:
    void on_btnDelete_clicked();
    void on_btnRefresh_clicked();
    void on_btnModify_clicked();

private:
    Ui::TableWindow *ui;

    QSqlTableModel *tableModel;
    QSqlQueryModel *tModel;
    QLabel *dbLabel;
    QTableView *tView;
};

#endif // TABLEWINDOW_H
