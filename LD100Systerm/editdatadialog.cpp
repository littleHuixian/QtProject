#include "editdatadialog.h"
#include "ui_editdatadialog.h"

extern QSqlQueryModel sqlModel;
extern QString reportTitle;
extern double lowValue, highValue;
extern int paperChoose;

editDataDialog::editDataDialog(QWidget *parent) : QDialog(parent), ui(new Ui::editDataDialog)
{
    ui->setupUi(this);

    QDateTime reportTime = QDateTime::currentDateTime();
    QString searchTime = reportTime.toString("yyyy-MM-dd");
    //报告时间
    ui->lineEdit14->setText(searchTime);
}

editDataDialog::~editDataDialog()
{
    delete ui;
}

void editDataDialog::on_btnUpdata_clicked()
{
//    "insert into saveTable(ID, 日期, 姓名,性别,年龄,病例号,床号,样本条码, 样本编号,科室,医生,"
//                                  "检测机器,HbA1c, 检验者,审核者)"
//                                  " values ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15')"
    QString nums = ui->lineEdit0->text();
    QString name            = ui->lineEdit1->text();
    QString gender          = ui->comboBox2->currentText();
    QString age               = ui->lineEdit3->text();
    QString sickId            = ui->lineEdit4->text();
    QString bedId            = ui->lineEdit5->text();
    QString sampleCode  = ui->lineEdit6->text();
    QString sampleNum   = ui->lineEdit7->text();
    QString department   = ui->lineEdit8->text();
    QString doctor           = ui->lineEdit9->text();
    QString result             = ui->lineEdit17->text();
    QString diagnosis       = ui->lineEdit10->text();
    QString note              = ui->lineEdit11->text();

    QString reportTime   = ui->lineEdit14->text();
    QString checker =ui->lineEdit15->text();
    QString auditor        = ui->lineEdit16->text();

    QSqlQuery query;
    QString execStr=QString("UPDATE  saveTable  SET 姓名='%1',性别='%2',年龄='%3',病例号='%4',床号='%5',样本条码='%6',"
                            "样本编号='%7',科室='%8',医生='%9',HbA1c='%10',检验者='%11',审核者='%12'  WHERE  ID=%13 ")
            .arg(name).arg(gender).arg(age).arg(sickId).arg(bedId).arg(sampleCode).arg(sampleNum).arg(department).arg(doctor)
            .arg(result).arg(checker).arg(auditor).arg(nums);

    bool success = query.exec(execStr);
    if(success) {
        qDebug()<<"数据跟心成功";
    } else {
        QMessageBox::information(this, tr("提示"), tr("数据保存失败！"), QMessageBox::Ok);

        return;
    }
}

void editDataDialog::on_btnPreview_clicked()
{
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(30, 30, 30, 30, QPrinter::DevicePixel);
     // 自定义纸张大小
    //printer.setPageSize(QPrinter::Custom);
    if(paperChoose == 0){
        printer.setPaperSize(QPrinter::A4);

        QPrintPreviewDialog preview(&printer, this);
        connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreviewSlot(QPrinter*)));
        preview.exec ();
    } else {
        printer.setPaperSize(QPrinter::A5);
        printer.setOrientation(QPrinter::Landscape);  //设置纸张横向
        QPrintPreviewDialog preview(&printer, this);
        connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(previewSlot(QPrinter*)));
        preview.exec ();
    }
}

void editDataDialog::on_btnPrint_clicked()
{
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(30, 30, 30, 30, QPrinter::DevicePixel);  //设置页边距
    if(paperChoose == 0){
        printer.setPaperSize(QPrinter::A4);     //定义纸张大小 A4纸规格 210mm×297mm  793pixel ×1123pixel
        if(QPrintDialog(&printer).exec() == QDialog::Accepted){
            printPreviewSlot(&printer);
        }
    } else {
        printer.setPaperSize(QPrinter::A5);
        printer.setOrientation(QPrinter::Landscape);  //设置纸张横向
        if(QPrintDialog(&printer).exec() == QDialog::Accepted){
            previewSlot(&printer);
        }
    }
}
void editDataDialog::printPreviewSlot(QPrinter *printerPixmap)
{
    QPainter painter;
    painter.begin(printerPixmap);
    QRectF pageRect = printerPixmap->pageRect(QPrinter::Millimeter);
    int widthPixle = int(printerPixmap->resolution() * (pageRect.width() / 25.4));
    int heightPixle = int(printerPixmap->resolution() * (pageRect.height() / 25.4));
    painter.setViewport(0, 0, widthPixle, heightPixle);
    qDebug()<<"width"<<"height"<<widthPixle<<heightPixle;
    QPen penBlack(Qt::black);  //画笔颜色
    painter.setPen(penBlack);
//    painter.drawRoundedRect(0, 0, widthPixle, heightPixle, 30, 30);

    QFont font;
    //标题
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(32);
    painter.setFont(font);
    painter.drawText(0, 0, widthPixle, 100, Qt::AlignCenter, reportTitle);

    font.setFamily("Bahnschrift");     //Microsoft YaHei
    font.setPixelSize(16);
    painter.setFont(font);

    painter.drawText(  20, 140, "姓名："+ui->lineEdit1->text());
    painter.drawText(200, 140, "性别："+ui->comboBox2->currentText());
    painter.drawText(400, 140, "年龄："+ui->lineEdit3->text());

    painter.drawText(20, 170, "病例号："+ui->lineEdit4->text());
    painter.drawText(200, 170, "床号："+ui->lineEdit5->text());
    painter.drawText(400, 170, "样本条码："+ui->lineEdit6->text());

    painter.drawText(20, 200, "样本编号："+ui->lineEdit7->text());
    painter.drawText(200, 200, "科室："+ui->lineEdit8->text());
    painter.drawText(400, 200, "医生："+ui->lineEdit9->text());

    painter.drawText(20, 230, "临床诊断："+ui->lineEdit10->text());
    painter.drawText(20, 260, "备注："+ui->lineEdit11->text());

    painter.drawLine(20, 270, 700, 270);    //分割线

    //painter.drawText(60, 330, "序号");

    painter.drawLine(20, 780, 700, 780);     //分割线
    painter.drawText(20, 800, "检验机器："+ui->lineEdit12->text());
    painter.drawText(250, 800, "检验时间："+ui->lineEdit13->text());
    painter.drawText(500, 800, "报告时间："+ui->lineEdit14->text());

    painter.drawText(20, 830, "检验者："+ui->lineEdit15->text());
    painter.drawText(250, 830, "审核者："+ui->lineEdit16->text());

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

void editDataDialog::previewSlot(QPrinter *printerPixmap)
{
    QPainter painter;
    painter.begin(printerPixmap);
    QRectF pageRect = printerPixmap->pageRect(QPrinter::Millimeter);
    int widthPixle = int(printerPixmap->resolution() * (pageRect.width() / 25.4));
    int heightPixle = int(printerPixmap->resolution() * (pageRect.height() / 25.4));
    qDebug()<<"width"<<"height"<<widthPixle<<heightPixle;
    painter.setViewport(0, 0, widthPixle, heightPixle);

    QPen penBlack(Qt::black);  //画笔颜色
    painter.setPen(penBlack);
//    painter.drawRoundedRect(0, 0, widthPixle, heightPixle, 30, 30);

    QFont font;
    //标题
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(28);
    painter.setFont(font);
    painter.drawText(0, 0, widthPixle, 50, Qt::AlignCenter, reportTitle);

    font.setFamily("Bahnschrift");     //Microsoft YaHei
    font.setPixelSize(12);
    painter.setFont(font);

    painter.drawText(  20, 80, "姓名："+ui->lineEdit1->text());
    painter.drawText(200, 80, "性别："+ui->comboBox2->currentText());
    painter.drawText(400, 80, "年龄："+ui->lineEdit3->text());

    painter.drawText(20, 100, "病例号："+ui->lineEdit4->text());
    painter.drawText(200, 100, "床号："+ui->lineEdit5->text());
    painter.drawText(400, 100, "样本条码："+ui->lineEdit6->text());

    painter.drawText(20, 120, "样本编号："+ui->lineEdit7->text());
    painter.drawText(200, 120, "科室："+ui->lineEdit8->text());
    painter.drawText(400, 120, "医生："+ui->lineEdit9->text());

    painter.drawText(20, 140, "临床诊断："+ui->lineEdit10->text());
    painter.drawText(20, 160, "备注："+ui->lineEdit11->text());

    painter.drawLine(20, 170, 700, 170);    //分割线

    //painter.drawText(60, 330, "序号");

    painter.drawLine(20, 400, 700, 400);     //分割线
    painter.drawText(20, 420, "检验机器："+ui->lineEdit12->text());
    painter.drawText(250, 420, "检验时间："+ui->lineEdit13->text());
    painter.drawText(500, 420, "报告时间："+ui->lineEdit14->text());

    painter.drawText(20, 440, "检验者："+ui->lineEdit15->text());
    painter.drawText(250, 440, "审核者："+ui->lineEdit16->text());

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

void editDataDialog::getSelectIndex(const QModelIndex &index)
{
    //qDebug()<<"aaaaaa"<<index.data().toString();
    selectRow = index.row();

    //数据序号
    QModelIndex indexPosition0 = sqlModel.index(selectRow, 0);
    ui->lineEdit0->setText(sqlModel.data(indexPosition0, 0).toString());
    //姓名
    QModelIndex indexPosition1 = sqlModel.index(selectRow, 2);
    ui->lineEdit1->setText(sqlModel.data(indexPosition1, 0).toString());
    //性别
    QModelIndex indexPosition2 = sqlModel.index(selectRow, 3);
    if (sqlModel.data(indexPosition2, 0).toString()=="男") {
        ui->comboBox2->setCurrentIndex(0);
    } else {
        ui->comboBox2->setCurrentIndex(1);
    }
    //年龄
    QModelIndex indexPosition3 = sqlModel.index(selectRow, 4);
    ui->lineEdit3->setText(sqlModel.data(indexPosition3, 0).toString());
    //病例号
    QModelIndex indexPosition4 = sqlModel.index(selectRow, 5);
    ui->lineEdit4->setText(sqlModel.data(indexPosition4, 0).toString());
    //床号
    QModelIndex indexPosition5 = sqlModel.index(selectRow, 6);
    ui->lineEdit5->setText(sqlModel.data(indexPosition5, 0).toString());
    //样本条码
    QModelIndex indexPosition6 = sqlModel.index(selectRow, 9);
    ui->lineEdit6->setText(sqlModel.data(indexPosition6, 0).toString());
    //样本编号
    QModelIndex indexPosition7 = sqlModel.index(selectRow, 10);
    ui->lineEdit7->setText(sqlModel.data(indexPosition7, 0).toString());
    //科室
    QModelIndex indexPosition8 = sqlModel.index(selectRow, 7);
    ui->lineEdit8->setText(sqlModel.data(indexPosition8, 0).toString());
    //医生
    QModelIndex indexPosition9 = sqlModel.index(selectRow, 8);
    ui->lineEdit9->setText(sqlModel.data(indexPosition9, 0).toString());
    //HbA1c
    QModelIndex indexPosition10 = sqlModel.index(selectRow, 12);
    ui->lineEdit17->setText(sqlModel.data(indexPosition10, 0).toString());
    //检验机器
    QModelIndex indexPosition11 = sqlModel.index(selectRow, 11);
    ui->lineEdit12->setText(sqlModel.data(indexPosition11, 0).toString());
    //检验时间
    QModelIndex indexPosition13 = sqlModel.index(selectRow, 1);
    ui->lineEdit13->setText(sqlModel.data(indexPosition13, 0).toString());
    //检验者
    QModelIndex indexPosition15 = sqlModel.index(selectRow, 13);
    ui->lineEdit15->setText(sqlModel.data(indexPosition15, 0).toString());
}
