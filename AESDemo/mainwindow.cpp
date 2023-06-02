#include "mainwindow.h"
#include "ui_mainwindow.h"

QSqlDatabase db;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    keyString = "LabChinacomLd600";      //内置的密钥，16位
    oldHashKey = keyString.toLatin1();
    ui->lineEditKey->setText(keyString);
    ui->lineEditKey->setEnabled(false);
    ui->textEncryption->setEnabled(false);

    ui->lineEdit2->setEnabled(false);   //

    currentDataTime = QDateTime::currentDateTime();
    ui->lineEdit3->setText(currentDataTime.toString("yyyyMMdd"));
    //ui->lineEditBoxData->setText(currentDataTime.toString("yyyyMMdd"));

    //系统时间显示
    systemTimeLabel = new QLabel;
    systemTimeLabel->setMinimumWidth(100);
    ui->statusBar->addWidget(systemTimeLabel);
    QTimer *systemTime = new QTimer(this);
    systemTime->start(1000);
    connect(systemTime, &QTimer::timeout, this, &MainWindow::systemTimeUpdate);

    //数据库状态显示
    dbinfoLabel = new QLabel;
    dbinfoLabel->setMinimumWidth(150);
    ui->statusBar->addWidget(dbinfoLabel);
    dbinfoLabel->setText("数据库未连接！");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("LNHCODEDB.db");

    if( db.open() ) {
        dbinfoLabel->setText(tr("数据库连接成功！"));
        //成功打开数据库，则建表
        //建表 编码时间(codeTime) 名称(name) 条码类别(category)
        QSqlQuery query;
        QString excStr = QString("CREATE TABLE codesTable (id INT PRIMARY KEY NOT NULL,"
                                 "编码时间 VARCHAR, 名称 VARCHAR, 条码类别 VARCHAR, 层析柱 VARCHAR,"
                                 "试剂批号 VARCHAR, 试剂规格 VARCHAR, 销售区域 VARCHAR, 编码数量 VARCHAR, 原始号码 VARCHAR,"
                                 "转换号码 VARCHAR)");
        query.exec(excStr);

    } else {
        //如果数据库打开失败，会弹出一个警告窗口
        QMessageBox::warning(this, "警告", "数据库连接失败，请及时向技术人员反馈修复！");
        return;

    }

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

//生成条码按钮
void MainWindow::on_btnGenerateCode_clicked()
{
    //层析柱
    if (ui->lineEdit2->text().isEmpty() || ui->lineEdit2->text().length() != 1) {
        QMessageBox::information(this, "提示", "层析柱输入错误或长度错误！");
        return;
    }

    //试剂批号
    if (ui->lineEdit3->text().isEmpty() || ui->lineEdit3->text().length() != 8) {
        QMessageBox::information(this, "提示", "试剂批号输入错误或长度错误！");
        return;
    }

    //试剂规格
    if(ui->lineEdit4->text().isEmpty() || ui->lineEdit4->text().length() != 2){
        QMessageBox::information(this, "提示", "规格输入错误或长度错误！");
        return;
    }

    //销售区域
    if(ui->lineEdit5->text().isEmpty() || ui->lineEdit5->text().length() != 3){
        QMessageBox::information(this, "提示", "销售区域输入错误！");
        return;
    }

    if (ui->lineEdit6->text().isEmpty()) {
        QMessageBox::information(this, "提示", "请输入条码数量！");
        return;
    }

    if(ui->cBoxCategory0->currentIndex() == 0){
        QMessageBox::information(this, "提示", "请选择仪器型号！");
        return;
    }
    if(ui->cBoxCategory1->currentIndex() == 0){
        QMessageBox::information(this, "提示", "请选择条码类别！");
        return;
    }
    codeCategory = QString("%1%2").arg(machine).arg(category);
    qDebug()<<"machine code"<<codeCategory;

    ui->textEditEncry->clear();      //清空初始条码区域
    ui->textEncryption->clear();    //清空加密条码区域
    generCode.clear();

//    QString strNum1 = ui->lineEdit1->text();      //类别
//    QString strNum2 = ui->lineEdit2->text();      //层析柱
//    QString strNum3 = ui->lineEdit3->text();      //试剂批号
//    QString strNum4 = ui->lineEdit4->text();      //起始编号
//    QString strNum5 = ui->lineEdit5->text();      //销售区域
//    QString strNum6 = ui->lineEdit6->text();      //条码数目

//    QString category  = QString("%1").arg(strNum1.toInt(), 2, 10, QLatin1Char('0'));
//    QString column = strNum2.toUpper();
//    int endNum = strNum4.toInt()+strNum6.toInt() - 1;
//    QString reagentCode = QString("%1%2%3%4").arg(category).arg(strNum2.toUpper()).arg(strNum3).arg(strNum5);

     //层析柱
    QString column = ui->lineEdit2->text().toUpper();

    //操作生成条码时间
    QDateTime curOperateTime = QDateTime::currentDateTime();
    QString operateTime = curOperateTime.toString("yyyy-MM-dd hh:mm:ss");

    //试剂批号 + 试剂规格
    QString tempCode = ui->lineEdit3->text() + ui->lineEdit4->text();
    QString reagentCode = decTo48(tempCode.toLongLong(), 6);            //将10位试剂转换成6位

    //销售区域
//    QString strNum5 = ui->lineEdit5->text();
//    QString salesArea = QString("%1").arg(strNum5.toInt(), 3, 10, QLatin1Char('0'));
    QString salesArea = ui->lineEdit5->text();

    //编码数量
    QString strNum6 = ui->lineEdit6->text();
    int codeNumbers = strNum6.toInt();

    //原始号码
    QString originalCode = QString("%1%2%3%4").arg(codeCategory).arg(column).arg(tempCode).arg(salesArea);
    //转换号码
    QString convertCode = QString("%1%2%3%4").arg(codeCategory).arg(column).arg(reagentCode).arg(salesArea);

    QString str = QString("SELECT * FROM codesTable");
    sqlModel.setQuery(str);
    int codeid = sqlModel.rowCount();
//    qDebug()<<codeid;
    //从数据库中查询对比，比对原始号码
    QString sqlStr = QString("select * from codesTable where 原始号码 = '%1'").arg(originalCode);
    QSqlQuery query;

    query.exec(sqlStr);

    QString result;
    int totalNum;
    while (query.next()) {
        totalNum = query.value(8).toInt();                //编码数量
        result =  query.value(9).toString();                //原始号码
    }
//    qDebug()<<"qqq"<<totalNum<<result<<codeid;

    if ( result == NULL ) {  //与数据库比对，数据库不存在该条码，表示可做新条码使用
        for (int i = 1; i < codeNumbers + 1; i++) {
    //        QString number = QString("%1").arg(strNum4.toInt() + i, 4, 10, QLatin1Char('0'));
            QString number = QString("%1").arg( i, 4, 10, QLatin1Char('0'));
            ui->textEditEncry->append(QString("%1%2%3%4%5").arg(codeCategory).arg(column).arg(tempCode).arg(number).arg(salesArea));
            generCode.append(QString("%1%2%3%4%5").arg(codeCategory).arg(column).arg(reagentCode).arg(number).arg(salesArea));
            //textEditEncry为原始条码
        }

        //插入数据
        QString insertStr = QString("INSERT INTO codesTable (id, 编码时间, 名称, 条码类别, 层析柱, 试剂批号, 试剂规格, 销售区域, 编码数量, 原始号码, 转换号码)"
                              " VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11')").arg(codeid)
                .arg(operateTime).arg(codeNomal).arg(codeCategory).arg(column).arg(ui->lineEdit3->text()).arg(ui->lineEdit4->text())
                .arg(salesArea).arg(strNum6).arg(originalCode).arg(convertCode);
//        QString insertStr = QString("INSERT INTO codesTable ('编码时间') VALUES('%1')").arg(operateTime);

        bool success = query.exec(insertStr);
//        qDebug()<<insertStr<<success;
        if (success) {
            dbinfoLabel->setText("数据库添加成功！");

        } else {
            dbinfoLabel->setText("数据库添加失败！");
            QMessageBox::warning(this, "警告", "数据库添加失败！");
            return;

        }

    } else {  //与数据库比对，数据库存在该条码，表示将余下条码编码
        if ( totalNum + codeNumbers <= 9999 ) {
            for (int i = totalNum + 1; i < totalNum + codeNumbers + 1; i++) {
                QString number = QString("%1").arg( i, 4, 10, QLatin1Char('0'));
                ui->textEditEncry->append(QString("%1%2%3%4%5").arg(codeCategory).arg(column).arg(tempCode).arg(number).arg(salesArea));
                generCode.append(QString("%1%2%3%4%5").arg(codeCategory).arg(column).arg(reagentCode).arg(number).arg(salesArea));

            }

            //编码时间和最大值更新，并替换
            totalNum = totalNum + codeNumbers;
            QString execStr = QString("update  codesTable  set 编码时间='%1', 编码数量='%2'  where  原始号码='%3' ").arg(operateTime).arg(totalNum).arg(originalCode);
            bool success = query.exec(execStr);
            if (success) {
                dbinfoLabel->setText("数据库更新成功！");

            } else {
                dbinfoLabel->setText("数据库更新失败！");
                QMessageBox::warning(this, "警告", "数据库更新失败！");
                return;
            }

        } else {
            dbinfoLabel->setText("注意：条码已使用！");
            QString messageStr = QString("该条码1-%1已使用！\n无法再生成%2个条码 \n请重新选择编号！").arg(totalNum).arg(codeNumbers);
            QMessageBox::warning(this, "警告", messageStr);

            return;
        }
    }
}

//加密按钮encryption
void MainWindow::on_btnEncryption_clicked()
{
    if (ui->textEditEncry->toPlainText().isEmpty()) {
        QMessageBox::information(this, "提示", "条码为空，请先生成条码！");
        return;
    }

    if ( ui->lineEditKey->text().isEmpty() ) {
        QMessageBox::information(this, "提示", "请输入密钥！");
        return;
    }

    if ( ui->lineEditKey->text().length() != 16 ) {
        QMessageBox::information(this, "提示", "秘钥长度错误，请重新输入密钥！");
        return;
    }

    ui->textEncryption->clear();            //清空加密框结果

    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);

    //选择密钥，秘钥加密
    if (ui->comboBox->currentIndex()==0) {
        //获取内置密钥
        hashKey = ui->lineEditKey->text().toLatin1();

    } else {
        //自定义密钥
        hashKey = ui->lineEditKey->text().toLatin1();
        QByteArray tempText = encryption.encode(hashKey, oldHashKey);
        changKey = tempText.toBase64();
    }

    //条码加密
    for (int i = 0; i < generCode.length(); i++) {
        QByteArray tempByte = QString("%1").arg(generCode.at(i)).toLatin1();
        QByteArray encodeText = encryption.encode(tempByte, hashKey);
        //显示加密内容
        ui->textEncryption->appendPlainText(encodeText.toBase64());
    }

    dbinfoLabel->setText("条码加密成功！");
//    if (   ) {
//        dbinfoLabel->setText("数据库添加成功！");
//    } else {
//        dbinfoLabel->setText("数据库添加失败！");
//        QMessageBox::warning(this, "警告", "数据库添加失败！");
//    }

}

void MainWindow::on_btnClearCode_clicked()
{
    ui->textEditEncry->clear();
    ui->textEncryption->clear();

    ui->lineEdit3->setText(currentDataTime.toString("yyyyMMdd"));
    ui->cBoxCategory1->setCurrentIndex(0);
}

//10进制转化成的x进制长度，不足nMinLen长度的话高位补0，否则不变
QString MainWindow::decTo48(qint64 src, int nMinLen)
{
    QString initWord = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKL";     //48位长度
    QByteArray charSet = initWord.toLatin1();

    QString dst;
    int m_nDigital = initWord.length();
    while(src != 0) {
        int nMod = src % m_nDigital;
        char szTmp[2];
        szTmp[0] = charSet[nMod];
        szTmp[1] = 0;
        dst.append(szTmp);

        src = src / m_nDigital;
    }

    std::reverse(dst.begin(), dst.end());
    int nAddLen = nMinLen - dst.length();

    QString strTmp;
    for (int i = 0; i < nAddLen; i++) {
        strTmp += "0";
    }
    QString str = strTmp + dst;
    return str;
}

void MainWindow::on_btnClearEncry_clicked()
{
    ui->textEncryption->clear();
    //IVL9zXfQdoBnhEEytw5/zg==
}

//保存文本
void MainWindow::on_btnSave_clicked()
{
    if (ui->textEncryption->toPlainText().isEmpty()) {
        QMessageBox::information(this, "提示", "内容为空！");
        return;
    }

    QDateTime todayDate = QDateTime::currentDateTime();
    QString dateString = todayDate.toString("yyyyMMddHHmmss");
    QString aFileName = QString("%1.txt").arg(dateString);

    //保存自定义密钥
    if (ui->comboBox->currentIndex()==1) {
        QFile keyFile("LD600key.txt");
        if (!keyFile.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream keyStream(&keyFile);
        keyStream<<QString("密钥更新时间：%1\n").arg(dateString);
        keyStream<<changKey;            //写入文本流
        keyFile.close();                             //关闭文件
    }

    //保存加密文本
    QFile aFile(aFileName);
    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream aStream(&aFile);                               //用文本流读取文件
    QString str=ui->textEncryption->toPlainText(); //条码转换为字符串
    aStream<<str;      //写入文本流
    aFile.close();          //关闭文件

    QMessageBox::information(this, "提示", "保存成功！");
}

//文本另存为
void MainWindow::on_btnSaveAs_clicked()
{
    if (ui->textEncryption->toPlainText().isEmpty()) {
        QMessageBox::information(this, "提示", "内容为空！");
        return;
    }

    QDateTime todayDate = QDateTime::currentDateTime();
    QString dateString = todayDate.toString("yyyyMMddHHmmss");
    QString aFileName = QString("%1.txt").arg(dateString);

    QFileDialog fileDialog;
    QString fileName = fileDialog.getSaveFileName(this, "Open File", aFileName, "Text File(*.txt)");
    if(fileName == "") {
        return;
    }

    //保存自定义密钥
    if (ui->comboBox->currentIndex()==1) {
        QFile keyFile("LD600key.txt");
        if (!keyFile.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream keyStream(&keyFile);
        keyStream<<QString("密钥更新时间：%1\n").arg(dateString);
        keyStream<<changKey;            //写入文本流
        keyFile.close();                             //关闭文件
    }

    //保存加密文本
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "打开文件失败！");
        return;
    } else {
        QTextStream textStream(&file);
        QString str = ui->textEncryption->toPlainText();            //条码转换为字符串
        textStream<<str;
        file.close();
        QMessageBox::warning(this, "提示", "保存文件成功！");
    }
}

//打印按钮
void MainWindow::on_btnPrint_clicked()
{
    // 创建打印机对象
    QPrinter printer;
    // 创建打印对话框
    QPrintDialog dlg(&printer, this);

    // 如果在对话框中按下了打印按钮，则执行打印操作
    if (dlg.exec() == QDialog::Accepted) {
        ui->textEncryption->print(&printer);
    }
}
//密钥选择
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
        case 0:
            ui->lineEditKey->clear();
            ui->lineEditKey->setText(keyString);
            ui->lineEditKey->setEnabled(false);
            break;
        case 1:
            ui->lineEditKey->clear();
            ui->lineEditKey->setEnabled(true);
            break;
        }
}

void MainWindow::on_actionInfo_triggered()
{
    infoDialog *infoWin = new infoDialog;
    infoWin->setWindowTitle("关于");
    infoWin->show();
}

void MainWindow::on_actionCheck_triggered()
{
    TableWindow *tableWin = new TableWindow;
    tableWin->setWindowTitle("已生成条码");
    tableWin->show();
}

void MainWindow::on_actioncode_triggered()
{
    calibrationDialog *calibrationWin = new calibrationDialog;
    calibrationWin->setWindowTitle("校准条码");
    calibrationWin->setWindowModality(Qt::ApplicationModal);
    calibrationWin->show();
}

void MainWindow::on_cBoxCategory0_currentIndexChanged(int index)
{
    //仪器型号 0代表LD600 1代表LD560
    switch (index) {
    case 0:
        machine.clear();
        break;

    case 1:        //LD560
        machine = "1";
        break;

    case 2:        //LD600
        machine = "0";
        break;

//    default:
//        machine.clear();
//        ui->lineEdit2->setEnabled(false);
//        ui->lineEdit2->clear();
//        break;
    }
}

void MainWindow::on_cBoxCategory1_currentIndexChanged(int index)
{
    // 条码类别
    switch (index) {
    case 0:
        category.clear();
        codeNomal.clear();
        ui->lineEdit2->setEnabled(false);
        ui->lineEdit2->clear();
        break;

    case 1:          //试剂A液
        category = "1";
        codeNomal = "A液";
        ui->lineEdit2->setText("A");
        ui->lineEdit2->setEnabled(false);
        break;

    case 2:          //试剂B液
        category = "2";
        codeNomal = "B液";
        ui->lineEdit2->setText("B");
        ui->lineEdit2->setEnabled(false);
        break;

    case 3:          //试剂C液
        category = "3";
        codeNomal = "C液";
        ui->lineEdit2->setText("C");
        ui->lineEdit2->setEnabled(false);
        break;

    case 4:         //试剂H液
        category = "5";
        codeNomal = "H液";
        ui->lineEdit2->setText("H");
        ui->lineEdit2->setEnabled(false);
        break;

    case 5:          //层析柱
        category = "6";
        codeNomal = "层析柱";
        ui->lineEdit2->setEnabled(true);
        ui->lineEdit2->clear();
        break;

//    default:
//        codeCategory.clear();
//        codeNomal.clear();
//        ui->lineEdit2->setEnabled(false);
//        ui->lineEdit2->clear();
//        break;
    }
}




