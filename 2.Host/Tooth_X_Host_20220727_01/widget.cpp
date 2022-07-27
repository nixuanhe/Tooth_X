#include "widget.h"
#include "ui_widget.h"



//总的窗口类构造函数
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    myUsart->usart_ui = ui;     //usart调用UI -4

    initAll(); //初始化

    connect(&timerForCommon, &QTimer::timeout, this, &Widget::getComNumBox);
    connect(&timerForCommon, &QTimer::timeout, this, &Widget::time_updata);
    timerForCommon.start(500);  //通用定时器每500ms触发

    ui->open_close_serialPort->setCheckable(true); //clicked(bool)


}

//总的窗口类析构函数
Widget::~Widget()
{
    delete ui;
}




/****************************************************************
 * 初始化
*****************************************************************/

/*
*初始化函数
*/
void Widget::initAll()
{
    windowsInit();
    getComNumBox();
    initSerialBandrateComBox();
    initSerialDataBitComBox();
    initSerialVerifyBitComBox();
    initSerialStopBitComBox();
    initTemperaturePlot();
}
/*
*主窗口初始化
*/
void Widget::windowsInit()
{
    //lcd初始化
    ui->lcdClock->setDigitCount(8);                  //显示位数
    ui->lcdClock->setMode(QLCDNumber::Dec);          //十进制
    ui->lcdClock->setSegmentStyle(QLCDNumber::Flat); //显示方式
}
/*
 *获取串口设备信息
*/
void Widget::getComNumBox()
{
    // 获取本机所有串口设备信息
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    static  int infoListLen;

    if(infoList.length()==infoListLen)
        return;
    infoListLen = infoList.length();
    // 为了在下拉选项中显示更多信息，采用模型形式添加设备列表
    QStandardItemModel *model = new QStandardItemModel(ui->comNumComBox);
    for (auto var : infoList){
        QStandardItem *item = new QStandardItem(var.portName());
        item->setToolTip(var.portName() + " " + var.description());
        model->appendRow(item);
    }

    // 将设备信息添加至设备选择框中，添加前清除原有信息
    ui->comNumComBox->clear();
    ui->comNumComBox->setModel(model);
}
/*
 *
*/
void Widget::initSerialBandrateComBox()
{
    ui->serialBandrateComBox->clear();
    QList<qint32> bdList = QSerialPortInfo::standardBaudRates();
    for (auto var : bdList){
        ui->serialBandrateComBox->addItem(QString::number(var), QVariant::fromValue(var));
    }
    ui->serialBandrateComBox->setCurrentText("115200");
}
/*
 *初始化数据位下拉框
*/
void Widget::initSerialDataBitComBox()
{
    ui->serialDataBitComBox->clear();
    /*
     * addItem();第一个参数是要显示的文本，第二个是文本背后的数据
    */
    ui->serialDataBitComBox->addItem(QString::number(QSerialPort::Data5,10),QVariant::fromValue(QSerialPort::Data5));
    ui->serialDataBitComBox->addItem(QString::number(QSerialPort::Data6,10),QVariant::fromValue(QSerialPort::Data6));
    ui->serialDataBitComBox->addItem(QString::number(QSerialPort::Data7,10),QVariant::fromValue(QSerialPort::Data7));
    ui->serialDataBitComBox->addItem(QString::number(QSerialPort::Data8,10),QVariant::fromValue(QSerialPort::Data8));
    ui->serialDataBitComBox->setCurrentText(QString::number(QSerialPort::Data8,10));
}
/*
 *初始化校验位下拉框
*/
void Widget::initSerialVerifyBitComBox()
{
    ui->serialVerifyBitComBox->clear();
    ui->serialVerifyBitComBox->addItem("None",QVariant::fromValue(QSerialPort::NoParity));
    ui->serialVerifyBitComBox->addItem("Even",QVariant::fromValue(QSerialPort::EvenParity));
    ui->serialVerifyBitComBox->addItem("Odd",QVariant::fromValue(QSerialPort::OddParity));
    ui->serialVerifyBitComBox->addItem("Space",QVariant::fromValue(QSerialPort::SpaceParity));
    ui->serialVerifyBitComBox->addItem("Mark",QVariant::fromValue(QSerialPort::MarkParity));
    ui->serialVerifyBitComBox->setCurrentText("None");
}
/*
 *初始化停止位下拉框
*/
void Widget::initSerialStopBitComBox()
{
    ui->serialStopBitComBox->clear();
    ui->serialStopBitComBox->addItem("1",QVariant::fromValue(QSerialPort::OneStop));
    ui->serialStopBitComBox->addItem("1.5",QVariant::fromValue(QSerialPort::OneAndHalfStop));
    ui->serialStopBitComBox->addItem("2",QVariant::fromValue(QSerialPort::TwoStop));
}

/*
 *初始化温度图表
*/
void Widget::initTemperaturePlot()
{
    //添加曲线
    ui->plotTemperature->addGraph();
    ui->plotTemperature->graph(0)->setPen(QPen(Qt::blue));
    ui->plotTemperature->graph(0)->setBrush(QBrush(QColor(0,0,255,20))); // 设置图层画刷颜色
    ui->plotTemperature->graph(0)->setAntialiasedFill(false);            // 设置图层反锯齿：关闭

    // 添加点
    ui->plotTemperature->addGraph();
    ui->plotTemperature->graph(1)->setPen(QPen(Qt::red,3));                   // 设置笔的颜色
    ui->plotTemperature->graph(1)->setLineStyle(QCPGraph::lsNone);            // 不画线条
    ui->plotTemperature->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);  // 设置点的形状

    //坐标轴1设置
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");                  //时间格式
    ui->plotTemperature->xAxis->setTicker(timeTicker);   // 横坐标显示时间
    ui->plotTemperature->axisRect()->setupFullAxesBox(); //轴矩形
    ui->plotTemperature->xAxis->setLabel("时间(s)");      // 设置x轴标签

    ui->plotTemperature->yAxis->setRange(-10,150);       //设置温度范围
    ui->plotTemperature->yAxis->setLabel("温度(℃)");     //设置y轴标签
    //ui->plotTemperature->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);    //设置属性可缩放，移动
    ui->plotTemperature->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);    //允许可缩放，移动

    //用滚轮缩放X轴
    QList < QCPAxis *>  xis;
    xis.append(ui->plotTemperature->xAxis);
    ui->plotTemperature->axisRect()->setRangeZoomAxes(xis);

    // 轴随动
    connect(ui->plotTemperature->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotTemperature->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plotTemperature->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotTemperature->yAxis2, SLOT(setRange(QCPRange)));

    // 实时更新槽函数
    connect(&timerForCommon ,&QTimer::timeout, this, &Widget::temperaturePlotUpdata);
}

/*
 *更新温度图表
*/
void Widget::temperaturePlotUpdata()
{
    ;
}



/*
 * ui按键使能控制
*/
void Widget::uiControlEnable(bool isEnable)
{
    ui->comNumComBox->setEnabled(isEnable);
    ui->serialBandrateComBox->setEnabled(isEnable);
    ui->serialDataBitComBox->setEnabled(isEnable);
    ui->serialVerifyBitComBox->setEnabled(isEnable);
    ui->serialStopBitComBox->setEnabled(isEnable);
}








/*
 * 实现槽函数，显示系统时间
*/
void Widget::time_updata()
{
    QDateTime dataTime = QDateTime::currentDateTime();
    QString datatimeStr = dataTime.toString("hh:mm:ss");
    ui->lcdClock->display(datatimeStr);
}
















/***************************************************************
 * 按键功能
***************************************************************/
/*
 * 打开、关闭串口
*/
void Widget::on_open_close_serialPort_clicked(bool checked)
{
    if(checked)
    {
        //按下打开串口后，此时串口是打开的
        //进行打开串口操作

        if(false == myUsart->mySerial.isOpen())
        {
            //把ui上的串口配置保存到后台
            myUsart->mySerial.setPortName(ui->comNumComBox->currentText());
            myUsart->mySerial.setBaudRate(static_cast<QSerialPort::Parity>(ui->serialBandrateComBox->currentData().toInt()));
            myUsart->mySerial.setParity(static_cast<QSerialPort::Parity>(ui->serialVerifyBitComBox->currentData().toInt()));
            myUsart->mySerial.setDataBits(static_cast<QSerialPort::DataBits>(ui->serialDataBitComBox->currentData().toInt()));
            myUsart->mySerial.setStopBits(static_cast<QSerialPort::StopBits>(ui->serialStopBitComBox->currentData().toInt()));

            if(myUsart->mySerial.open(QSerialPort::ReadWrite))
            {
                //尝试打开串口，如果以读写方式打开串口成功
                uiControlEnable(false);                          //关闭ui控制使能
                ui->open_close_serialPort->setText("关闭串口");   //更改按键文字
                //QObject::connect(serialport, &QSerialPort::readyRead, this, &Widget::read_serialport);    //读数据
                QObject::connect(&(myUsart->mySerial), SIGNAL(QSerialPort::readyRead()), &(myUsart->mySerial), SLOT(Usart::serialRecData()));
            }
            else
            {
                //如果打开串口失败，发送错误信息
                QMessageBox::warning(this,"错误",myUsart->mySerial.errorString());
            }
        }
    }
    else
    {
        //按下关闭串口后，此时串口是关闭的
        if(true == myUsart->mySerial.isOpen())
        {
            //如果串口是开着的，执行关闭串口的操作
            myUsart->mySerial.clear();                      //擦除串口
            myUsart->mySerial.close();                      //关闭串口
            ui->open_close_serialPort->setText("打开串口"); //更改按键文字
            uiControlEnable(true);                         //允许控制按键
        }

    }

}





