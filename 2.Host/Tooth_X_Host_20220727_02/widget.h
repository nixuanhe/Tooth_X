#ifndef WIDGET_H
#define WIDGET_H

//头文件
#include <QWidget>
#include <QLCDNumber>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QStringList>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "qcustomplot.h"

//调试宏定义
#define cout qDebug() << "[" << __FILE__ <<":" << __LINE__ << "]"


#define headerIndex         0
#define deviceIndex         1
#define enderIndex          7
#define serialframeLenth    8

#define temperatureSensor 0x01


//命名空间
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


//总的窗口类
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);  //构造函数
    ~Widget();  //析构函数

    uint32_t recBytesLenth;                 //总的串口接受数据长度，点击清除的时候再清零
    uint8_t  serialRecFrameHeader;   //接收帧头
    uint8_t  serialRecFrameEnder;    //接收帧尾
    uint8_t temperature;             //温度
    uint8_t controlCmd[8] = {0xbb,0x01,0x02,0x03,0x04,0x05,0x0d,0x0a};




    char* usartData;

    void initAll(void);                      //初始化函数
    void windowsInit(void);
    void getComNumBox(void);                 //初始化串口号下拉框
    void initSerialBandrateComBox(void);     //初始化波特率下拉框
    void initSerialDataBitComBox(void);      //初始化数据位下拉框
    void initSerialVerifyBitComBox(void);    //初始化校验位下拉框
    void initSerialStopBitComBox(void);      //初始化停止位下拉框
    void initTemperaturePlot(void);          //初始化温度图    
    void uiControlEnable(bool isEnable);

    void serialRecData(void);                //串口接受数据
    void serialSendCmd(char* cmd);               //串口发送数据
    QByteArray serialDeviceRecData(QByteArray& RecData); //对接收数据进行识别

    void temperaturePlotUpdata(void);        //更新温度图表
    void startDrawTempCSP();                 //开始绘图温度
    void pauseDrawTempCSP();                 //暂停绘图温度



//声明槽函数
public slots:
    void uiDisplayUpdata(void);

private slots:
    void on_open_close_serialPort_clicked(bool checked);
    void on_open_close_drawTemp_clicked(bool checked);

private:
    Ui::Widget *ui;
    QSerialPort *mySerial; //串口类
    QTimer timerForCommon; //通用定时器
    QTimer *timerForCSP;   //绘图定时器


};




#endif // WIDGET_H
