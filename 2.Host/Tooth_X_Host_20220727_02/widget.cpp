#include "widget.h"
#include "ui_widget.h"



//总的窗口类构造函数
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);   


    setFixedSize( this->width(),this->height());    //窗口固定大小

    mySerial = new QSerialPort;
    recBytesLenth = 0;      //接受数据长度一开始为0
    serialRecFrameHeader = 0xAA;  //帧头
    serialRecFrameEnder = 0x55;   //帧尾
    temperature = 0;


    initAll(); //初始化

    connect(&timerForCommon, &QTimer::timeout, this, &Widget::getComNumBox);
    connect(&timerForCommon, &QTimer::timeout, this, &Widget::uiDisplayUpdata);
    timerForCommon.start(500);  //通用定时器每500ms触发

    ui->open_close_serialPort->setCheckable(true); //clicked(bool)
    ui->open_close_drawTemp->setCheckable(true); //clicked(bool)

}

//总的窗口类析构函数
Widget::~Widget()
{
    delete ui;
}



/************************************************************************************************************************************************
 * 串口相关
************************************************************************************************************************************************/
/*
 * 串口接受数据 函数实现
*/
void Widget::serialRecData()
{
    QByteArray bytes = mySerial->readAll();
    QString dataStr;

    if(false == bytes.isEmpty())  //这个if判断挺关键的，非空再去做串的处理，要不然对空串处理没有什么意义
    {
        recBytesLenth +=bytes.length();
        qDebug()<<"已经接收:"<<recBytesLenth;
        serialDeviceRecData(bytes);
    }
    else
    {
        qDebug()<<"empty";
    }

    //    if(usart_ui->timestampcheckBox->isChecked())//如果勾选了时间戳
    //    {
    //        dataStr.append("<font coor=\"#FF0000\">");
    //        dataStr.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"));
    //        dataStr.append("</font>");
    //    }

    bytes.clear();

}
/*
 * 对串口接收不同设备数据进行识别处理
*/
QByteArray Widget::serialDeviceRecData(QByteArray& RecData)
{
    char *pArr = RecData.data();      //转换成char类型

    for(int i=0;i<10;i++)
        qDebug()<<"帧："<<uint8_t(pArr[i]); //调试看看数据对不对

    //验证帧头帧尾，数据是否正确
    if((serialRecFrameHeader == uint8_t(pArr[headerIndex]))&&(serialRecFrameEnder == uint8_t(pArr[enderIndex])))
    {
        qDebug()<<"帧头帧尾验证成功";
        switch(uint8_t(pArr[deviceIndex]))
        {
            case temperatureSensor:
            {
                temperature = pArr[deviceIndex+1];

                RecData.remove(0,2);//去掉帧头和设备号
                RecData.chop(1);    //去掉帧尾

                pArr = RecData.data();
                for(int i=0;i<serialframeLenth-3;i++)
                  qDebug()<<"传出去："<<uint8_t(pArr[i]); //调试看看数据对不对
                qDebug()<<"读取温度成功.RecData:"<<RecData;

                return RecData;
            }
            default:
            {
                qDebug()<<"读取设备号出错.RecData:"<<RecData;
                return RecData;
            }
        }
    }
    else
    {
        qDebug()<<"未接收正确数据.RecData:"<<RecData;
        return RecData;
    }
}

/*
 * 串口发送命令 函数实现
 * 不完善
 * 还没写！！！后面补！！！
*/
void Widget::serialSendCmd(char* cmd)
{
    mySerial->write(cmd,8);
}


/************************************************************************************************************************************************
 * 初始化
************************************************************************************************************************************************/

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

//    ui->currTempLCDNum->setDigitCount(3);                  //显示位数
//    ui->currTempLCDNum->setMode(QLCDNumber::Dec);          //十进制
//    ui->currTempLCDNum->setSegmentStyle(QLCDNumber::Flat); //显示方式
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
    ui->plotTemperatureWidget->addGraph();
    ui->plotTemperatureWidget->graph(0)->setPen(QPen(Qt::blue));
    ui->plotTemperatureWidget->graph(0)->setBrush(QBrush(QColor(0,0,255,20))); // 设置图层画刷颜色
    ui->plotTemperatureWidget->graph(0)->setAntialiasedFill(false);            // 设置图层反锯齿：关闭

    // 添加点
    ui->plotTemperatureWidget->addGraph();
    ui->plotTemperatureWidget->graph(1)->setPen(QPen(QColor(255, 228, 181)));                   // 设置笔的颜色
    ui->plotTemperatureWidget->graph(1)->setLineStyle(QCPGraph::lsNone);                        // 不画线条
    ui->plotTemperatureWidget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,1));  // 设置点的形状,实心点大小为1

    //坐标轴1设置
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");                  //时间格式
    ui->plotTemperatureWidget->xAxis->setTicker(timeTicker);   // 横坐标显示时间
    ui->plotTemperatureWidget->axisRect()->setupFullAxesBox(); //轴矩形,就是右侧和上侧的边
    ui->plotTemperatureWidget->xAxis->setLabel("时间");         // 设置x轴标签

    ui->plotTemperatureWidget->yAxis->setRange(-10,170);       //设置温度范围
    ui->plotTemperatureWidget->yAxis->setLabel("温度(℃)");     //设置y轴标签
    //ui->plotTemperature->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);    //设置属性可缩放，移动
    ui->plotTemperatureWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);    //允许可缩放，移动

    //用滚轮缩放X轴
    QList < QCPAxis *>  xis;
    xis.append(ui->plotTemperatureWidget->xAxis);
    ui->plotTemperatureWidget->axisRect()->setRangeZoomAxes(xis);

    // 轴随动
    connect(ui->plotTemperatureWidget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotTemperatureWidget->xAxis2, SLOT(setRange(QCPRange)));
    //connect(ui->plotTemperatureWidget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotTemperatureWidget->yAxis2, SLOT(setRange(QCPRange)));

    // 实时更新槽函数
    timerForCSP = new QTimer(this);
    connect(timerForCSP ,&QTimer::timeout, this, &Widget::temperaturePlotUpdata);
}

/*
 *更新温度图表
*/
void Widget::temperaturePlotUpdata()
{
    static QTime tempCurrTime(QTime::currentTime());        //获取当前时间
    //计算新的数据点
    double key = tempCurrTime.elapsed()/1000.0;
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.002) //大于2ms添加一个数据
    {
        //add data to lines
        ui->plotTemperatureWidget->graph(0)->addData(key, temperature);      //温度加到数据中
        ui->plotTemperatureWidget->graph(1)->addData(key, temperature);
        //rescale value (key) axis to fit the current data:
        //重绘横轴
        ui->plotTemperatureWidget->graph(0)->rescaleKeyAxis();
        ui->plotTemperatureWidget->graph(1)->rescaleKeyAxis(true);
        lastPointKey = key;
    }

    // make key axis range scroll with the data (at a constant range size of 10):
    ui->plotTemperatureWidget->xAxis->setRange(key, 10, Qt::AlignCenter);      //设置x轴范围
    //ui->plotTemperatureWidget->yAxis->setRange(temperature, 60, Qt::AlignCenter);        //设置y轴范围
    ui->plotTemperatureWidget->replot();                                      //画图


    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->tempFrameRateDisplay->setText(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->plotTemperatureWidget->graph(0)->data()->size()+ui->plotTemperatureWidget->graph(1)->data()->size())
            );
        lastFpsKey = key;
        frameCount = 0;
    }
}
/*
 *开始绘图温度
*/
void Widget::startDrawTempCSP()
{
    timerForCSP->start(20);        //300ms更新一次
    //后面补
}
/*
 *暂停绘图温度
*/
void Widget::pauseDrawTempCSP()
{
    timerForCSP->stop();
    //后面补
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
 * 实现槽函数，显示更新
*/
void Widget::uiDisplayUpdata()
{
    QDateTime dataTime = QDateTime::currentDateTime();
    QString datatimeStr = dataTime.toString("hh:mm:ss");
    ui->lcdClock->display(datatimeStr);
    ui->currTempLCDNum->display(temperature);
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
        //此时串口是关闭的
        //按下打开串口后，执行打开串口
        //进行打开串口操作
        if(false == mySerial->isOpen())
        {
            //把ui上的串口配置保存到后台
            mySerial->setPortName(ui->comNumComBox->currentText());
            mySerial->setBaudRate(static_cast<QSerialPort::Parity>(ui->serialBandrateComBox->currentData().toInt()));
            mySerial->setParity(static_cast<QSerialPort::Parity>(ui->serialVerifyBitComBox->currentData().toInt()));
            mySerial->setDataBits(static_cast<QSerialPort::DataBits>(ui->serialDataBitComBox->currentData().toInt()));
            mySerial->setStopBits(static_cast<QSerialPort::StopBits>(ui->serialStopBitComBox->currentData().toInt()));          

            if(mySerial->open(QSerialPort::ReadWrite))
            {
                //尝试打开串口，如果以读写方式打开串口成功
                uiControlEnable(false);                          //关闭ui控制使能
                ui->open_close_serialPort->setText("关闭串口");   //更改按键文字
                //通过信号与槽机制，实时不断读串口
                QObject::connect(mySerial, &QSerialPort::readyRead, this, &Widget::serialRecData);
            }
            else
            {
                //如果打开串口失败，发送错误信息
                QMessageBox::warning(this,"错误",mySerial->errorString());
            }
        }
        else
        {
            QMessageBox::warning(this,"打开串口失败",mySerial->errorString());
        }
    }
    else
    {
        //按下关闭串口后，此时串口是关闭的
        if(true == mySerial->isOpen())
        {
            //如果串口是开着的，执行关闭串口的操作
            mySerial->clear();                      //擦除串口
            mySerial->close();                      //关闭串口
            ui->open_close_serialPort->setText("打开串口"); //更改按键文字
            uiControlEnable(true);                         //允许控制按键
        }
    }

}


void Widget::on_open_close_drawTemp_clicked(bool checked)
{
    if(checked)
    {
        //此时绘图温度是关闭的
        //按下“显示曲线”，完成打开绘图的动作
        startDrawTempCSP();
        ui->open_close_drawTemp->setText("暂停显示");
    }
    else
    {
        //此时绘图温度是打开的
        //按下“暂停显示”，完成关闭绘图的动作
        pauseDrawTempCSP();
        ui->open_close_drawTemp->setText("显示曲线");
    }

}




