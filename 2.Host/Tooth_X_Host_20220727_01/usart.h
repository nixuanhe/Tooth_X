#ifndef USART_H
#define USART_H


#include "ui_widget.h" //usart调用UI -1

#include <QWidget>
#include <QDebug>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>


#define cout qDebug() << "[" << __FILE__ <<":" << __LINE__ << "]"

#define headerIndex 0
#define deviceIndex 1
#define enderIndex 7

#define temperatureSensor 0x01



class Usart
{
public:
    Ui::Widget *usart_ui; //usart调用UI -2   
    QSerialPort mySerial; //给串口类声明容器

    uint32_t recBytesLenth;  //总的串口接受数据长度，点击清除的时候再清零
    uint8_t  serialFrameHeader;
    uint8_t  serialFrameEnder;
    char* usartData;

    /*
     * 构造、析构函数
    */
    Usart();
    Usart(uint8_t frameHeader,uint8_t frameEnder);
    ~Usart();

    /*
     * 函数声明
    */
//    void find_SerialPort(void); //读取串口号
    void serialRecData(void);   //串口接受数据
    void serialSendData(void);  //串口发送数据
    QByteArray serialDistinguishRecData(QByteArray& RecData); //对接收数据进行识别



};

#endif // USART_H
