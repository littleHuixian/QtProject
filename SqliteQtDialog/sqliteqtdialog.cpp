#include "sqliteqtdialog.h"
#include "ui_sqliteqtdialog.h"

SqliteQtDialog::SqliteQtDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SqliteQtDialog)
{
    ui->setupUi(this);

    connect(ui->insertButton,SIGNAL(triggered()),this,SLOT(on_insertButton_clicked()));
    connect(ui->deleteButton,SIGNAL(triggered()),this,SLOT(on_deleteButton_clicked()));
    connect(ui->modifyButton,SIGNAL(triggered()),this,SLOT(on_modifyButton_clicked()));
    connect(ui->sortButton,SIGNAL(triggered()),this,SLOT(on_sortButton_clicked()));

    createDB();
    createTable();
    //查询和显示
    queryTable();
}

SqliteQtDialog::~SqliteQtDialog()
{
    delete ui;
}
void SqliteQtDialog::createDB(void){//创建数据库
    //建立和sqlite数据库连接
    db=QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库文件的名字
    db.setDatabaseName("menu.db");
    //打开数据库
    if(db.open()==false){
        QMessageBox::critical(this,"DateError",db.lastError().text());
    }
}
void SqliteQtDialog::createTable(void){//创建表
    //构建创建表的SQL语句
    QString createSql=QString("CREATE TABLE student(\
                        id INT PRIMARY KEY NOT NULL,\
                        name TEXT NOT NULL,\
                        score REAL NOT NULL");
    QSqlQuery query;
    query.exec(createSql);
}
void SqliteQtDialog::queryTable(void){//查询表
    QString str=QString("SELECT * FROM Student");
    model.setQuery(str);
    ui->tableView->setModel(&model);
}

void SqliteQtDialog::on_insertButton_clicked()
{
    QSqlQuery query;
    //获取学号
    int id=ui->idEdit->text().toInt();
    //获取姓名
    QString name=ui->nameEdit->text();
    //获取成绩
    double score=ui->scoreEdit->text().toDouble();
    //构造SQL语句
    QString insertSql=QString("INSERT INTO Student VALUES(%1,'%2',%3)").arg(id).arg(name).arg(score);
    qDebug()<<insertSql;
    //执行SQL语句
    query.exec(insertSql);
    query.exec("commit");
    //查询和显示
    queryTable();
}
//删除，通过ID匹配
void SqliteQtDialog::on_deleteButton_clicked()
{
    int id=ui->idEdit->text().toInt();

    //构造删除SQL语句
    QString deleteSql=QString("DELETE FROM Student WHRE id=%1").arg(id);
    QSqlQuery query;
    query.exec(deleteSql);
    //查询和显示
    queryTable();
}
//修改成绩
void SqliteQtDialog::on_modifyButton_clicked()
{
    QSqlQuery query;
    int id=ui->idEdit->text().toInt();
    double score=ui->scoreEdit->text().toDouble();

    QString modifySql=QString("UPDATE Student SET score=%1 WHERE id=%2").arg(score).arg(id);
    qDebug()<<modifySql;
    query.exec(modifySql);
    //查询和显示
    queryTable();
}
//排序
void SqliteQtDialog::on_sortButton_clicked()
{
    //获取排序关键字
    QString value=ui->ValueComboBox->currentText();

    //获取排序条件
    QString condition;
    if(ui->CondComboBox->currentIndex()){
        condition="DESC";
    } else {
        condition="ASC";
    }

    QString sortSql=QString("SELECT * FROM Student ORDER BY %1 %2").arg(value).arg(condition);
    model.setQuery(sortSql);
    ui->tableView->setModel(&model);
}
