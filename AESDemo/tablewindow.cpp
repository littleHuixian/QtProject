#include "tablewindow.h"
#include "ui_tablewindow.h"
#include<QDebug>

extern QSqlDatabase db;

TableWindow::TableWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::TableWindow)
{
    ui->setupUi(this);
    this->setFixedWidth(800);

//    db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("labLD600DB.db");                //设置数据库名称

    //数据库状态显示
    dbLabel = new QLabel;
    dbLabel->setMinimumWidth(150);
    ui->statusbar->addWidget(dbLabel);

    tModel = new QSqlQueryModel;
    tModel->setQuery(QString("select 编码时间, 名称, 条码类别, 层析柱, 试剂批号, "
                             "试剂规格, 销售区域, 编码数量, 原始号码, 转换号码 from codesTable"));
    ui->tableView->setModel(tModel);
    ui->tableView->setAlternatingRowColors( true);           //使表格颜色交错功能为真
    ui->tableView->horizontalHeader()->showFullScreen();
    ui->tableView->resizeColumnsToContents();
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//    if( !db.open() ) {
//        dbLabel->setText("数据库打开失败！");
//        return;
//    } else {
//        dbLabel->setText("数据库打开成功！");

//        tModel = new QSqlQueryModel;
//        tModel->setQuery(QString("select 编码时间, 名称, 条码类别, 层析柱, 试剂批号, "
//                                 "试剂规格, 销售区域, 编码数量, 原始号码, 转换号码 from codesTable"));

// //        tModel->setHeaderData(0, Qt::Horizontal, "序号");
// //        tModel->setHeaderData(1, Qt::Horizontal, "编码时间");
// //        tModel->setHeaderData(2, Qt::Horizontal, "条码类别");
// //        tModel->setHeaderData(3, Qt::Horizontal, "层析柱");
// //        tModel->setHeaderData(4, Qt::Horizontal, "试剂批号");
// //        tModel->setHeaderData(6, Qt::Horizontal, "试剂规格");
// //        tModel->setHeaderData(7, Qt::Horizontal, "销售区域");
// //        tModel->setHeaderData(8, Qt::Horizontal, "编码数量");
// //        tModel->setHeaderData(9, Qt::Horizontal, "原始号码");
// //        tModel->setHeaderData(5, Qt::Horizontal, "转换号码");

//        ui->tableView->setModel(tModel);
//        ui->tableView->setAlternatingRowColors( true);           //使表格颜色交错功能为真
//        ui->tableView->horizontalHeader()->showFullScreen();
//        ui->tableView->resizeColumnsToContents();
//    }

}

TableWindow::~TableWindow()
{
    delete ui;
}
//delete button
void TableWindow::on_btnDelete_clicked()
{
    //    QSqlQuery query;
    //    query.exec("DELETE * FROM labLD600DB");
    //
    qDebug()<<"当前行"<< ui->tableView->currentIndex().row();
    int index = ui->tableView->currentIndex().row();
    tModel = new QSqlQueryModel;
    tModel->setQuery(QString("DELETE FROM codesTable WHERE rowid = %1").arg(index+1));

    qDebug()<<"delete finished";
}
// refresh button
void TableWindow::on_btnRefresh_clicked()
{
    tModel = new QSqlQueryModel;
    tModel->setQuery(QString("select 编码时间, 名称, 条码类别, 层析柱, 试剂批号, "
                             "试剂规格, 销售区域, 编码数量, 原始号码, 转换号码 FROM codesTable"));
    ui->tableView->setModel(tModel);
    ui->tableView->setAlternatingRowColors( true);           //使表格颜色交错功能为真
    ui->tableView->horizontalHeader()->showFullScreen();
    ui->tableView->resizeColumnsToContents();
}
// modify button
void TableWindow::on_btnModify_clicked()
{

}
