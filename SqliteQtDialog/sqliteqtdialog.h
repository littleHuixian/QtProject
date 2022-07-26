#ifndef SQLITEQTDIALOG_H
#define SQLITEQTDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QsqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QDebug>

namespace Ui {
class SqliteQtDialog;
}

class SqliteQtDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SqliteQtDialog(QWidget *parent = 0);
    ~SqliteQtDialog();
private slots:
    void on_insertButton_clicked();
    void on_deleteButton_clicked();
    void on_modifyButton_clicked();
    void on_sortButton_clicked();
private:
    void createDB(void);//创建数据库
    void createTable(void);//创建学生成绩表
    void queryTable(void);//查询数据表

private:
    Ui::SqliteQtDialog *ui;
    QSqlDatabase db;//建立和qt与数据库连接
    QSqlQueryModel model;//保存和遍历select结果
};

#endif // SQLITEQTDIALOG_H
