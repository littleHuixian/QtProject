﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

QSqlQueryModel sqlModel;
QString reportTitle;
double lowValue, highValue;
int paperSize;

QString configFilePath;
QStringList doctorNames, inspectorNames, reviewerNames;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    configInfo();

    //系统时间显示
    systemTimeLabel = new QLabel;
    systemTimeLabel->setMinimumWidth(150);
    ui->statusBar->addWidget(systemTimeLabel);
    QTimer *systemTime = new QTimer(this);
    systemTime->start(1000);
    connect(systemTime, &QTimer::timeout, this, &MainWindow::systemTimeUpdate);

    infoLabel = new QLabel;
    infoLabel->setMinimumWidth(200);
    ui->statusBar->addWidget(infoLabel);

    tableLabel = new QLabel;
    tableLabel->setMinimumWidth(200);
    ui->statusBar->addWidget(tableLabel);

    QDateTime reportTime = QDateTime::currentDateTime();
    QString searchTime = reportTime.toString("yyyy-MM-dd");
    //报告时间
    ui->lineEdit14->setText(searchTime);

    initTable1();

    ui->tableView1->setContextMenuPolicy(Qt::CustomContextMenu);

    serialport = new QSerialPort;
    portState = false;

    find_port();
    /*
      若未连接串口，则每个五分钟扫描一次串口，并提示是否有可用的串口，提示
      内容为 扫描时间+是否有用串口
    */
    if(!portState) {
        QTimer *timeTick = new QTimer(this);
        timeTick->start(300000);
        connect(timeTick, &QTimer::timeout, this, &MainWindow::find_port);
    }

    //创建数据库
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("resultDB.db");
    if( database.open() ){
        //建表
        QSqlQuery query;
        QString excStr = QString("CREATE TABLE saveTable ("
                                 "ID INT PRIMARY KEY NOT NULL, 日期 VARCHAR, 姓名 VARCHAR, 性别 VARCHAR, 年龄 VARCHAR,"
                                 "病例号 VARCHAR, 床号 VARCHAR, 样本条码 VARCHAR,样本编号 VARCHAR, 科室 VARCHAR,"
                                 " 医生 VARCHAR,检测机器 VARCHAR, HbA1c VARCHAR, 检验者 VARCHAR, 审核者 VARCHAR,"
                                 "备注 VARCHAR, 临床诊断 VARCHAR)");
        query.exec(excStr);

        initTable2();
        tableLabel->setText(tr("历史数据加载完成"));
    } else {
        tableLabel->setText(tr("历史数据加载失败"));
    }

    portWindow = new choosePortForm;
    portWindow->setWindowTitle(tr("设置"));
    portWindow->setWindowIcon(QIcon(":/images/comset.png"));

    editWindow = new editDataDialog;
    editWindow->setWindowTitle(tr("历史数据详情"));
    editWindow->setWindowIcon(QIcon(":/images/connect.png"));

    addNewInfo = new addDialog;
    addNewInfo->setWindowTitle(tr("新建病例"));

    connect(ui->tableView1, &QAbstractItemView::clicked, this, &MainWindow::setTable1Data);
//    connect(ui->tableView2, &QAbstractItemView::clicked, this, &MainWindow::visualData);
    connect(ui->tableView2, &QAbstractItemView::doubleClicked, this, [=](){
        editWindow->show();
//        editWindow->exec();
    });
    connect(ui->tableView2, &QAbstractItemView::doubleClicked, editWindow, &editDataDialog::getSelectIndex);
    //获取串口设置
    connect(portWindow, &choosePortForm::sendPortSetData, this, &MainWindow::receivePortSetData);
    //获取医生名字
    connect(portWindow, &choosePortForm::sendChangeData, this, &MainWindow::receiveName);
    //新建窗口
    connect(addNewInfo, &addDialog::sendString, this, &MainWindow::receiveAddInfo);

    //获取更新
    connect(editWindow, &editDataDialog::sendUpdateexec, this, &MainWindow::receiveUpdate);

}

MainWindow::~MainWindow()
{
    delete ui;
}
//状态栏显示系统时间
void MainWindow::systemTimeUpdate()
{
    QDateTime currentSystemTime = QDateTime::currentDateTime();
    QString timeStr = currentSystemTime.toString("yyyy-MM-dd hh:mm:ss");
    systemTimeLabel->setText(timeStr);
}

void MainWindow::initTable1()
{
    model1 = new QStandardItemModel(this);
    QStringList headList;
    headList<< "机器型号"<< "日期"<< "序号"<<"HbA1c";
    model1->setColumnCount(headList.length());
    for (int i = 0; i <headList.length(); i++) {
        model1->setHeaderData(i, Qt::Horizontal, headList.at(i));
    }

    ui->tableView1->setModel(model1);
    //ui->tableView1->resizeColumnsToContents();
    //对齐方式：居中
    ui->tableView1->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //随ui界面拉伸表
    ui->tableView1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableView1->verticalHeader()->setVisible(false);
    for (int i = 0; i < headList.length(); i++) {
        ui->tableView1->setColumnWidth(i, 60);
    }
    ui->tableView1->setColumnWidth(1, 100);
    ui->tableView1->setColumnWidth(2, 150);
    ui->tableView1->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::initTable2()
{
    // 获取表格视图的水平表头
    QHeaderView* header = ui->tableView2->horizontalHeader();

    // 将列的宽度模式设置为自适应内容
    header->setSectionResizeMode(QHeaderView::ResizeToContents);

    QString str = QString("SELECT * FROM saveTable");
    sqlModel.setQuery(str);
    ui->tableView2->setModel(&sqlModel);
//    ui->tableView2->resizeColumnsToContents();

    for (int i = 0; i < sqlModel.columnCount(); i++) {
        ui->tableView2->setColumnWidth(i, 60);
    }
    ui->tableView2->setColumnWidth(1, 150);
    ui->tableView2->verticalHeader()->setVisible(false);
    ui->tableView2->setSelectionBehavior(QAbstractItemView::SelectRows);

}

void MainWindow::setTable1Data(const QModelIndex &index)
{
//    on_actionClear_triggered();
    int selectIndex = index.row();

    ui->lineEdit7->setText( model1->item(selectIndex, 2)->text());

    ui->lineEdit12->setText( model1->item(selectIndex, 0)->text());
    ui->lineEdit13->setText( model1->item(selectIndex, 1)->text());

}

//void MainWindow::on_tableView2_doubleClicked(const QModelIndex &index)
//{
//    qDebug()<<"doubleClicked"<<index;
//    if(index.isValid()) {
//        editWindow = new editDataDialog;
//        editWindow->setWindowTitle(tr("历史数据详情"));
//        editWindow->setWindowIcon(QIcon(":/images/connect.png"));
//        editWindow->show();
//        editWindow->exec();

//    }
//}

void MainWindow::find_port()
{
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QSerialPort serial;
        serial.setPort(info);   //设置串口
        if( serial.open(QIODevice::ReadWrite) ) {
            //   ui->com->addItem(serial.portName());        //显示串口name
            portName.clear();
            portName = serial.portName();
            QDateTime currentSystemTime = QDateTime::currentDateTime();
            QString timeStr = currentSystemTime.toString("hh:mm:ss");

            infoLabel->setText(timeStr+tr("扫描有可用串口"));
            //qDebug()<<"serial port:"<<portName<<portName.length();
            serial.close();

        }
    }
}

void MainWindow::sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::configInfo()
{
    //创建配置文件，读取配置
    configFilePath = qApp->applicationDirPath()+"/localconfig.ini";
    if(!QFile::exists(configFilePath)){
        QSettings configIniWrite(configFilePath, QSettings::IniFormat);

        configIniWrite.beginGroup("PRINT_SET");
        //0对应A4纸张，1对应A5纸张
        configIniWrite.setValue("PAGE_SIZE", 0);
        configIniWrite.setValue("PRINT_TITLE", "检验报告单");
        configIniWrite.setValue("LOW_VALUE", "4.0");
        configIniWrite.setValue("HIGH_VALUE", "6.0");
        configIniWrite.endGroup();

        configIniWrite.beginGroup("NAME_SET");
        configIniWrite.setValue("DOCTOR_NAMES", "NAME1");
        configIniWrite.setValue("INSPECTOR_NAMES", "NAME2");
        configIniWrite.setValue("REVIEWER_NAMES", "NAME3");
        configIniWrite.endGroup();

    }

    QSettings *configFileRead = new QSettings(configFilePath, QSettings::IniFormat);
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
    paperSize = configFileRead->value("PRINT_SET/PAGE_SIZE").toInt();
    reportTitle = configFileRead->value("PRINT_SET/PRINT_TITLE").toString();
    lowValue = configFileRead->value("PRINT_SET/LOW_VALUE").toDouble();
    highValue = configFileRead->value("PRINT_SET/HIGH_VALUE").toDouble();

    doctorNames = configFileRead->value("NAME_SET/DOCTOR_NAMES").toStringList();
    inspectorNames = configFileRead->value("NAME_SET/INSPECTOR_NAMES").toStringList();
    reviewerNames = configFileRead->value("NAME_SET/REVIEWER_NAMES").toStringList();

    ui->comboBox3->clear();
    ui->comboBox3->addItems(doctorNames);

    ui->comboBox4->clear();
    ui->comboBox4->addItems(inspectorNames);

    ui->comboBox5->clear();
    ui->comboBox5->addItems(reviewerNames);
}

void MainWindow::on_actionLink_triggered()
{
    // qDebug()<<usePortName<<useBaud ;
    if (usePortName.isEmpty()){
        QMessageBox::information(this, tr("提示"), tr("请先选择串口"), QMessageBox::Ok);
        return;
    }

    if (!portState){
        update();
        sleep(100);     //延时100ms
        find_port();     //重新查找com
        //初始化串口
        serialport->setPortName(usePortName);        //设置串口名
        //打开串口成功
        if(serialport->open(QIODevice::ReadWrite)) {
            portState = true;
            ui->actionLink->setIcon(QIcon(":/images/success.png"));
            infoLabel->setText(tr("串口已连接"));
            //设置波特率 setBaudRate(ui->baud->currentText().toInt())
            serialport->setBaudRate(useBaud.toInt());
            //设置数据位数
            serialport->setDataBits(QSerialPort::Data8);
            //设置奇偶校验
            serialport->setParity(QSerialPort::NoParity);
            //设置停止位
            serialport->setStopBits(QSerialPort::OneStop);
            serialport->setFlowControl(QSerialPort::NoFlowControl);     //设置为无流控制

            //连接槽函数
            connect(serialport, &QSerialPort::readyRead, this, &MainWindow::Read_Date);

        } else {   //打开失败提示
            sleep(100);
            QMessageBox::information(this, tr("Erro"), tr("串口打开失败！"), QMessageBox::Ok);
            portState = false;
            infoLabel->setText(tr("串口打开失败！"));
            return;
        }

    } else {
        serialport->clear();        //清空缓存区
        serialport->close();        //关闭串口
        portState = false;
        ui->actionLink->setIcon(QIcon(":/images/failure.png"));
        infoLabel->setText(tr("串口已关闭！"));
        return;
    }
}

void MainWindow::on_actionSet_triggered()
{
    portWindow->show();
}

void MainWindow::Read_Date()
{
    QByteArray buf;
    buf = serialport->readAll();

    // 将数据显示到文本
    if(!buf.isEmpty()) {

        QString str = QString::fromLocal8Bit(buf);
        dataStream = dataStream + str;
        if ( str.length() != 32) {
            qDebug()<<"data:"<<dataStream;
            //处理接收的数据
            getFormatData(dataStream);

            //显示接收的数据信息
            int ReceiveCount=dataStream.length();
            QDateTime currentSystemTime = QDateTime::currentDateTime();
            QString timeStr = currentSystemTime.toString("hh:mm:ss");
            infoLabel->setText(timeStr + tr(" 接收：%1 字节").arg(ReceiveCount));
            dataStream.clear();
        }

    }

}

void MainWindow::visualData(const QModelIndex &index)
{
    int selectRow = index.row();

    ui->lineEdit1->setText( sqlModel.data(sqlModel.index(selectRow, 2), 0).toString() );

    if(sqlModel.data(sqlModel.index(selectRow, 3), 0).toString()=="男"){
        ui->comboBox2->setCurrentIndex(0);
    }
    ui->lineEdit3->setText( sqlModel.data(sqlModel.index(selectRow, 4), 0).toString());
    ui->lineEdit4->setText( sqlModel.data(sqlModel.index(selectRow, 5), 0).toString());
    ui->lineEdit5->setText( sqlModel.data(sqlModel.index(selectRow, 6), 0).toString());
    ui->lineEdit6->setText( sqlModel.data(sqlModel.index(selectRow, 7), 0).toString());
    ui->lineEdit7->setText( sqlModel.data(sqlModel.index(selectRow, 8), 0).toString());
    ui->lineEdit8->setText( sqlModel.data(sqlModel.index(selectRow, 9), 0).toString());
//    ui->lineEdit9->setText( sqlModel.data(sqlModel.index(selectRow, 10), 0).toString());

    ui->lineEdit12->setText( sqlModel.data(sqlModel.index(selectRow, 11), 0).toString());
    ui->lineEdit13->setText( sqlModel.data(sqlModel.index(selectRow, 1), 0).toString());
//    ui->lineEdit15->setText( sqlModel.data(sqlModel.index(selectRow, 13), 0).toString());
//    ui->lineEdit16->setText( sqlModel.data(sqlModel.index(selectRow, 14), 0).toString());
}
void MainWindow::getFormatData(QString a)
{
    if (a.startsWith("<TRANSMIT>") && a.endsWith("</TRANSMIT>")) {
        a = a.remove("<TRANSMIT>");
        a = a.remove("</TRANSMIT>");
        QStringList list = a.split("|");
        qDebug()<<"dataList:"<<list;

        //解析接收数据
        //qDebug()<<"列："<<model1->columnCount()<<"行："<<model1->rowCount();
        int rowNums = model1->rowCount();
        int columnNums = model1->columnCount();
        for (int i = 0; i < columnNums; i++) {
            if (i == 3){

                model1->setItem(rowNums, i, new QStandardItem( list.at(i).mid(5,4)) );

            } else {
                model1->setItem(rowNums, i, new QStandardItem( list.at(i)) );

            }
            model1->item(rowNums, i)->setTextAlignment(Qt::AlignCenter);
        }
        ui->tableView1->setModel(model1);
//        ui->tableView1.setT
//        ui->tableView1->resizeColumnsToContents();
//        ui->tableView1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->lineEdit7->setText(list.at(2));
        ui->lineEdit12->setText( list.at(0) );  //检验机器
        ui->lineEdit13->setText( list.at(1) );  //检验时间

    }

}

void MainWindow::receivePortSetData(QStringList data)
{
    qDebug()<<"portdata"<<data;
    usePortName=data.at(0);
    useBaud=data.at(1);
}

void MainWindow::receiveName(QStringList aList, QStringList bList, QStringList cList)
{
    ui->comboBox3->clear();
    ui->comboBox3->addItems(aList);
    ui->comboBox4->clear();
    ui->comboBox4->addItems(bList);
    ui->comboBox5->clear();
    ui->comboBox5->addItems(cList);
}

void MainWindow::receiveAddInfo(QString infoString)
{
//    int dataRows = sqlModel.rowCount();
//    qDebug()<< infoString<<dataRows;
    QSqlQuery query;
    bool success = query.exec(infoString);

    if(success) {
        initTable2();
        tableLabel->setText(tr("数据保存完成"));

    } else {
        QMessageBox::information(this, tr("提示"), tr("数据保存失败！"), QMessageBox::Ok);
        tableLabel->setText(tr("数据保存失败"));
        return;
    }
}

void MainWindow::receiveUpdate(QString getUpdateString)
{
//    qDebug()<<getUpdateString;
    QSqlQuery query;
    bool success = query.exec(getUpdateString);
    if(success) {
        initTable2();
        tableLabel->setText(tr("数据更新成功"));

    } else {
        QMessageBox::information(this, tr("提示"), tr("数据保存失败！"), QMessageBox::Ok);
        return;
    }
}

void MainWindow::on_btnSave_clicked()
{
    int currentRow = ui->tableView1->currentIndex().row();
    int allRows = sqlModel.rowCount();
    if (currentRow < 0){
        QMessageBox::information(this, tr("提示"), tr("请先选择保存数据"), QMessageBox::Ok);
        return;
    }

    QString name            = ui->lineEdit1->text();
    QString gender          = ui->comboBox2->currentText();
    QString age               = ui->lineEdit3->text();
    QString sickId            = ui->lineEdit4->text();
    QString bedId            = ui->lineEdit5->text();
    QString sampleCode  = ui->lineEdit6->text();
    QString sampleNum   = ui->lineEdit7->text();
    QString department   = ui->lineEdit8->text();
    QString doctor           = ui->comboBox3->currentText();
    QString diagnosis       = ui->lineEdit10->text();
    QString note              = ui->lineEdit11->text();

    QString machin         = ui->lineEdit12->text();
    QString testTime       = ui->lineEdit13->text(); //取日期
    QString reportTime   = ui->lineEdit14->text();
    QString checkName =ui->comboBox4->currentText();
    QString auditor        = ui->comboBox5->currentText();

    QString testResult = model1->item(currentRow, 3)->text();

    QString execSql = QString("insert into saveTable(ID, 日期, 姓名,性别,年龄,病例号,床号,样本条码, 样本编号,科室,医生,"
                              "检测机器,HbA1c, 检验者,审核者)"
                              " values ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15')")
            .arg(allRows+1).arg(testTime).arg(name).arg(gender).arg(age).arg(sickId).arg(bedId).arg(sampleCode)
            .arg(sampleNum).arg(department).arg(doctor).arg(machin).arg(testResult).arg(checkName).arg(auditor);

    QSqlQuery query;
    bool success = query.exec(execSql);

    if(success) {
        initTable2();
        tableLabel->setText(tr("数据保存完成"));

    } else {
        QMessageBox::information(this, tr("提示"), tr("数据保存失败！"), QMessageBox::Ok);
        tableLabel->setText(tr("数据保存失败"));
        return;
    }
}

void MainWindow::on_btnSearch_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
        QMessageBox::information(this, tr("提示"), tr("请输入查询内容"), QMessageBox::Ok);
        return;
    }

    QSqlQuery query;
    //姓名查询
    if (ui->comboBox->currentIndex()==0){
        QString excStr = QString("SELECT * FROM saveTable WHERE 姓名=='%1'").arg(ui->lineEdit->text());

        if(query.exec(excStr)) {
            sqlModel.setQuery(excStr);
            ui->tableView2->setModel(&sqlModel);
            ui->tableView2->verticalHeader()->setVisible(false);
            tableLabel->setText(tr("查询完成"));

        } else {
            tableLabel->setText(tr("查询失败"));
        }
    //ID查询
    } else if(ui->comboBox->currentIndex()==1) {
        QString excStr = QString("SELECT * FROM saveTable WHERE ID=='%1'").arg(ui->lineEdit->text());

        if(query.exec(excStr)) {
            sqlModel.setQuery(excStr);
            ui->tableView2->setModel(&sqlModel);
            ui->tableView2->verticalHeader()->setVisible(false);
            tableLabel->setText(tr("查询完成"));

        }

    //日期查询(模糊查询)
    } else if (ui->comboBox->currentIndex()==2) {

        QString searchContent = ui->lineEdit->text();
        QString excStr = QString("SELECT * FROM saveTable WHERE 日期 LIKE '%1%'").arg(searchContent);
        bool success = query.exec(excStr);
        if (success){
            sqlModel.setQuery(excStr);
            ui->tableView2->setModel(&sqlModel);
            ui->tableView2->verticalHeader()->setVisible(false);
            tableLabel->setText(tr("查询完成"));
        }

    }
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if (index==2) {
        QDateTime reportTime = QDateTime::currentDateTime();
        QString searchTime = reportTime.toString("yyyy-MM-dd");
        ui->lineEdit->setText(searchTime);

    } else {
        ui->lineEdit->clear();

    }
}

void MainWindow::on_btnBack_clicked()
{
    initTable2();
    tableLabel->setText(tr("数据库复原完成"));
}
//打印预览按钮
void MainWindow::on_btnPreview_clicked()
{
    //获取当前行
    int selectRow = ui->tableView2->currentIndex().row();
    if (selectRow < 0) {
        QMessageBox::information(this, tr("警告"), tr("请先选择打印数据"), QMessageBox::Ok);
        return;
    }

    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(30, 30, 30, 30, QPrinter::DevicePixel);
     // 自定义纸张大小
    //printer.setPageSize(QPrinter::Custom);
    if(paperSize == 0){
        printer.setPaperSize(QPrinter::A4);
        printer.setOrientation(QPrinter::Portrait);  //设置纸张纵向

        QPrintPreviewDialog preview(&printer, this);
        preview.setMinimumSize(QSize(900, 1200));

        connect(&preview, &QPrintPreviewDialog::paintRequested, this, &MainWindow::printA4Slot);
        preview.exec ();

    } else {
        printer.setPaperSize(QPrinter::A5);
        printer.setOrientation(QPrinter::Landscape);  //设置纸张横向

        QPrintPreviewDialog preview(&printer, this);
        preview.setMinimumSize(QSize(1000, 800));

        connect(&preview, &QPrintPreviewDialog::paintRequested, this, &MainWindow::printA5Slot);
        //connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printA5Slot(QPrinter*)));
        preview.exec ();

    }
}

void MainWindow::on_btnPrint_clicked()
{
    int selectRow = ui->tableView2->currentIndex().row();
    if (selectRow < 0){
        QMessageBox::information(this, tr("警告"), tr("请先选择打印数据"), QMessageBox::Ok);
        return;
    }

    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(30, 30, 30, 30, QPrinter::DevicePixel);  //设置页边距
    if(paperSize == 0){
        printer.setPaperSize(QPrinter::A4);     //定义纸张大小 A4纸规格 210mm×297mm  793pixel ×1123pixel
        printer.setOrientation(QPrinter::Portrait);  //设置纸张纵向

        if(QPrintDialog(&printer).exec() == QDialog::Accepted){
            printA4Slot(&printer);
        }
    } else {
        printer.setPaperSize(QPrinter::A5);
        printer.setOrientation(QPrinter::Landscape);  //设置纸张横向

        if(QPrintDialog(&printer).exec() == QDialog::Accepted){
            printA5Slot(&printer);
        }
    }

}

void MainWindow::on_actionPreview_triggered()
{
    //获取当前行
    int selectRow = ui->tableView2->currentIndex().row();
    if (selectRow < 0) {
        QMessageBox::information(this, tr("警告"), tr("请先选择打印数据"), QMessageBox::Ok);
        return;
    }

    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(30, 30, 30, 30, QPrinter::DevicePixel);     //设置页边距
     //自定义纸张大小
     //printer.setPageSize(QPrinter::Custom);
    if(paperSize == 0){
        printer.setPaperSize(QPrinter::A4);
        printer.setOrientation(QPrinter::Portrait);  //设置纸张纵向

        QPrintPreviewDialog preview(&printer, this);
        connect(&preview, &QPrintPreviewDialog::paintRequested, this, &MainWindow::printA4Slot);

        preview.exec ();

    } else {
        printer.setPaperSize(QPrinter::A5);
        printer.setOrientation(QPrinter::Landscape);  //设置纸张横向

        QPrintPreviewDialog preview(&printer, this);
        connect(&preview, &QPrintPreviewDialog::paintRequested, this, &MainWindow::printA5Slot);
        preview.exec ();
    }
}

void MainWindow::on_actionPrint_triggered()
{
    int selectRow = ui->tableView2->currentIndex().row();
    if (selectRow < 0){
        QMessageBox::information(this, tr("警告"), tr("请先选择打印数据"), QMessageBox::Ok);
        return;
    }

    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(30, 30, 30, 30, QPrinter::DevicePixel);  //设置页边距

    if(paperSize == 0){
        printer.setPaperSize(QPrinter::A4);
        printer.setOrientation(QPrinter::Portrait);  //设置纸张纵向

        if(QPrintDialog(&printer).exec() == QDialog::Accepted){
            printA4Slot(&printer);
        }

    } else {
        printer.setPaperSize(QPrinter::A5);
        printer.setOrientation(QPrinter::Landscape);  //设置纸张横向
        if(QPrintDialog(&printer).exec() == QDialog::Accepted){
            printA5Slot(&printer);
        }
    }
}

void MainWindow::printA4Slot(QPrinter *printerPixmap)
{
    QPainter painter;
    painter.begin(printerPixmap);
    QRectF pageRect = printerPixmap->pageRect(QPrinter::Millimeter);
    int widthPixle = int(printerPixmap->resolution() * (pageRect.width() / 25.4));
    int heightPixle = int(printerPixmap->resolution() * (pageRect.height() / 25.4));
    painter.setViewport(0, 0, widthPixle, heightPixle);
//    qDebug()<<"width"<<"height"<<widthPixle<<heightPixle;
    QPen penBlack(Qt::black);  //画笔颜色
    painter.setPen(penBlack);

    int selectRow = ui->tableView2->currentIndex().row();
    QFont font;
    //标 题
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(32);
    painter.setFont(font);
    painter.drawText(0, 0, widthPixle, 100, Qt::AlignCenter, reportTitle);

    font.setFamily("Bahnschrift");     //Microsoft YaHei
    font.setPixelSize(18);
    painter.setFont(font);

    painter.drawText(  20, 140, "姓 名："+sqlModel.data(sqlModel.index(selectRow, 2), 0).toString());
    painter.drawText(200, 140, "性 别："+sqlModel.data(sqlModel.index(selectRow, 3), 0).toString());
    painter.drawText(400, 140, "年 龄："+sqlModel.data(sqlModel.index(selectRow, 4), 0).toString());

    painter.drawText(20, 170, "病例号："+sqlModel.data(sqlModel.index(selectRow, 5), 0).toString());
    painter.drawText(200, 170, "床 号："+sqlModel.data(sqlModel.index(selectRow, 6), 0).toString());
    painter.drawText(400, 170, "样本条码："+sqlModel.data(sqlModel.index(selectRow, 7), 0).toString());

    painter.drawText(20, 200, "样本编号："+sqlModel.data(sqlModel.index(selectRow, 8), 0).toString());
    painter.drawText(200, 200, "申请科室："+sqlModel.data(sqlModel.index(selectRow, 9), 0).toString());
    painter.drawText(400, 200, "医 生："+sqlModel.data(sqlModel.index(selectRow, 10), 0).toString());

    painter.drawText(20, 230, "临床诊断："+sqlModel.data(sqlModel.index(selectRow, 16), 0).toString());
    painter.drawText(20, 260, "备 注："+sqlModel.data(sqlModel.index(selectRow, 15), 0).toString());

    painter.drawLine(20, 270, 900, 270);    //分割线

    painter.drawLine(20, 780, 900, 780);     //分割线
    painter.drawText(20, 800, "检验机器："+sqlModel.data(sqlModel.index(selectRow, 11), 0).toString());
    painter.drawText(250, 800, "检验时间："+ui->lineEdit13->text());
    painter.drawText(500, 800, "报告时间："+ui->lineEdit14->text());

    painter.drawText(20, 830, "检验者："+sqlModel.data(sqlModel.index(selectRow, 13), 0).toString());
    painter.drawText(250, 830, "审核者："+sqlModel.data(sqlModel.index(selectRow, 14), 0).toString());

    painter.drawText(450, 880, "*本结果仅供临床参考*");

    //内容标题
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(16);
    painter.drawText(70, 300, "检测项目");
    painter.drawText(220, 300, "检测方法");
    painter.drawText(380, 300, "结果");
    painter.drawText(500, 300, "参考区间");

    painter.drawText(80, 330, "HbA1c");
    painter.drawText(200, 330, "硼酸亲和层析色谱法");

    painter.drawText(500, 330, QString("%1-%2").arg( lowValue).arg(highValue)+"%");

    QModelIndex columnIndex = sqlModel.index(selectRow, 12);
    if(sqlModel.data(columnIndex, 0).toDouble() < lowValue){
        painter.drawText(380, 330, sqlModel.data(columnIndex, 0).toString()+"%"+"    ↓");
    } else if (sqlModel.data(columnIndex, 0).toDouble() > highValue) {
        painter.drawText(380, 330, sqlModel.data(columnIndex, 0).toString()+"%"+"    ↑");
    } else {
        painter.drawText(380, 330, sqlModel.data(columnIndex, 0).toString()+"%");
    }

    painter.end();
}

void MainWindow::printA5Slot(QPrinter *printerPixmap)
{
    QPainter painter;
    painter.begin(printerPixmap);
    QRectF pageRect = printerPixmap->pageRect(QPrinter::Millimeter);
    int widthPixle = int(printerPixmap->resolution() * (pageRect.width() / 25.4));
    int heightPixle = int(printerPixmap->resolution() * (pageRect.height() / 25.4));
//    qDebug()<<"width"<<"height"<<widthPixle<<heightPixle;
    painter.setViewport(0, 0, widthPixle, heightPixle);

    QPen penBlack(Qt::black);  //画笔颜色
    painter.setPen(penBlack);

    int selectRow = ui->tableView2->currentIndex().row();

    QFont font;
    //标题
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(28);
    painter.setFont(font);
    painter.drawText(0, 0, widthPixle, 50, Qt::AlignCenter, reportTitle);

    font.setFamily("Bahnschrift");     //Microsoft YaHei
    font.setPixelSize(12);
    painter.setFont(font);
//    sqlModel.data(indexPosition0, 0).toString()
    painter.drawText(  20, 80, "姓名："+sqlModel.data(sqlModel.index(selectRow, 2), 0).toString());
    painter.drawText(200, 80, "性别："+sqlModel.data(sqlModel.index(selectRow, 3), 0).toString());
    painter.drawText(400, 80, "年龄："+sqlModel.data(sqlModel.index(selectRow, 4), 0).toString());

    painter.drawText(20, 100, "病例号："+sqlModel.data(sqlModel.index(selectRow, 5), 0).toString());
    painter.drawText(200, 100, "床号："+sqlModel.data(sqlModel.index(selectRow, 6), 0).toString());
    painter.drawText(400, 100, "样本条码："+sqlModel.data(sqlModel.index(selectRow, 7), 0).toString());

    painter.drawText(20, 120, "样本编号："+sqlModel.data(sqlModel.index(selectRow, 8), 0).toString());
    painter.drawText(200, 120, "申请科室："+sqlModel.data(sqlModel.index(selectRow, 9), 0).toString());
    painter.drawText(400, 120, "医生："+sqlModel.data(sqlModel.index(selectRow, 10), 0).toString());

    painter.drawText(20, 140, "临床诊断："+sqlModel.data(sqlModel.index(selectRow, 16), 0).toString());
    painter.drawText(20, 160, "备注："+sqlModel.data(sqlModel.index(selectRow, 15), 0).toString());

    painter.drawLine(20, 170, 700, 170);    //分割线

    //painter.drawText(60, 330, "序号");

    painter.drawLine(20, 400, 700, 400);     //分割线
    painter.drawText(20, 420, "检验机器："+sqlModel.data(sqlModel.index(selectRow, 11), 0).toString());
    painter.drawText(250, 420, "检验时间："+ui->lineEdit13->text());
    painter.drawText(500, 420, "报告时间："+ui->lineEdit14->text());

    painter.drawText(20, 440, "检验者："+sqlModel.data(sqlModel.index(selectRow, 13), 0).toString());
    painter.drawText(250, 440, "审核者："+sqlModel.data(sqlModel.index(selectRow, 14), 0).toString());

    painter.drawText(450, 480, "*本结果仅供临床参考*");

    //内容标题
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(16);
    painter.drawText(70, 200, "检测项目");
    painter.drawText(220, 200, "检测方法");
    painter.drawText(380, 200, "结果");
    painter.drawText(500, 200, "参考区间");

    painter.drawText(80, 220, "HbA1c");
    painter.drawText(200, 220, "硼酸亲和层析色谱法");

    painter.drawText(500, 220, QString("%1-%2").arg( lowValue).arg(highValue)+"%");

    QModelIndex columnIndex = sqlModel.index(selectRow, 12);
    if(sqlModel.data(columnIndex, 0).toDouble() < lowValue){
        painter.drawText(380, 220, sqlModel.data(columnIndex, 0).toString()+"%"+"    ↓");
    } else if (sqlModel.data(columnIndex, 0).toDouble() > highValue) {
        painter.drawText(380, 220, sqlModel.data(columnIndex, 0).toString()+"%"+"    ↑");
    } else {
        painter.drawText(380, 220, sqlModel.data(columnIndex, 0).toString()+"%");
    }

    painter.end();
}

void MainWindow::on_actionClear_triggered()
{
    ui->lineEdit1->clear();
    ui->lineEdit3->clear();
    ui->lineEdit4->clear();
    ui->lineEdit5->clear();
    ui->lineEdit6->clear();
    ui->lineEdit7->clear();
    ui->lineEdit8->clear();
//    ui->lineEdit9->clear();

    ui->lineEdit10->clear();
    ui->lineEdit11->clear();

//    ui->lineEdit15->clear();
//    ui->lineEdit16->clear();
}

void MainWindow::on_actionDelete_triggered()
{
    //获取当前行
    int selectIndex = ui->tableView1->currentIndex().row();
    if (selectIndex == -1){
        QMessageBox::information(this, tr("Erro"), tr("请先选择删除行"), QMessageBox::Ok);
        return;

    } else {
        //QMessageBox::information(this, tr("Erro"), tr("请先选择删除行"), QMessageBox::Ok);
        model1->removeRow(selectIndex);
        tableLabel->setText(tr("数据删除完成"));

    }
}

void MainWindow::on_btnAdd_clicked()
{
//    addNewInfo->sen
    addNewInfo->getDataString(doctorNames, inspectorNames, reviewerNames);
    addNewInfo->exec();

}


void MainWindow::on_btnSaveXlsx_clicked()
{
    QDateTime currentSystemTime = QDateTime::currentDateTime();
    QString timeStr = currentSystemTime.toString("yyyy-MM-dd");
    QString fileName = timeStr + "_save_data.xlsx";

    if (QFile::exists(fileName)) {
        // 文件存在
        QMessageBox::StandardButton btn = QMessageBox::warning(this, "Warning", "File already exists, do you want to overwrite it?", QMessageBox::Yes|QMessageBox::No);
        if(btn != QMessageBox::Yes){
            return;
        }
        // 将数据保存到XLSX文件中
        saveToExcel(ui->tableView2, fileName);

    } else {
        // 文件不存在
        saveToExcel(ui->tableView2, fileName);
    }

}

void MainWindow::saveToExcel(QTableView *tableView, const QString &fileName)
{
    QXlsx::Document xlsx(fileName);

    // 写入数据头
    xlsx.write("A1", "序号");
    xlsx.write("B1", "日期");
    xlsx.write("C1", "姓名");
    xlsx.write("D1", "性别");
    xlsx.write("E1", "年龄");
    xlsx.write("F1", "病例号");
    xlsx.write("G1", "床号");
    xlsx.write("H1", "样本条码");
    xlsx.write("I1", "样本编号");
    xlsx.write("J1", "科室");
    xlsx.write("K1", "医生");
    xlsx.write("L1", "检验机器");
    xlsx.write("M1", "HbA1c");
    xlsx.write("N1", "检验者");
    xlsx.write("O1", "审核者");
    xlsx.write("P1", "备注");
    xlsx.write("Q1", "临床诊断");

    // 获取行列数
    int rowCount = tableView->model()->rowCount();
    int colCount = tableView->model()->columnCount();

    // 遍历每一行和每一列
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            // 获取表格中的数据
            QString data = tableView->model()->index(row, col).data().toString();

            // 将数据保存到Excel中
            xlsx.write(row + 2, col + 1, data);
        }
    }

    xlsx.saveAs(fileName);

    QMessageBox::information(this, tr("提示"), tr("数据已保存！"), QMessageBox::Ok);
}

//帮助对话框
void MainWindow::on_actionHelp_triggered()
{
    UseDialog *helpWindow = new UseDialog;
    helpWindow->setWindowTitle(tr("帮助"));
    helpWindow->setWindowIcon(QIcon(":/images/help.png"));
    helpWindow->show();
}

