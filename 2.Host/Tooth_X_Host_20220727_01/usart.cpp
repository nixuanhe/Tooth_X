#include "usart.h"



//构造函数
Usart::Usart()
{
    recBytesLenth = 0;
}
Usart::Usart(uint8_t frameHeader,uint8_t frameEnder)
{
    recBytesLenth = 0;
    serialFrameHeader = frameHeader;
    serialFrameEnder = frameEnder;
}

Usart::~Usart()
{
    ;
}

/*
 * find_serialport函数实现
 * 这个函数好像没什么用，实在不行后面就删掉了
*/
//void Usart::find_SerialPort()
//{
//    //查找可用的串口
//    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
//    {
//        mySerial.setPort(info);   //设置串口
//        if(mySerial.open(QIODevice::ReadWrite))
//        {
//            usart_ui->comNumComBox->addItem(mySerial.portName());        //显示串口name
//            mySerial.close();
//        }
//    }
//}

/*
 * 串口接受数据 函数实现
*/
void Usart::serialRecData()
{
    QByteArray bytes = mySerial.readAll();
    QString dataStr;

    recBytesLenth +=bytes.length();

    serialDistinguishRecData(bytes);
    //    if(usart_ui->timestampcheckBox->isChecked())//如果勾选了时间戳
    //    {
    //        dataStr.append("<font coor=\"#FF0000\">");
    //        dataStr.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"));
    //        dataStr.append("</font>");
    //    }



}
/*
 * 对串口接收数据进行识别处理
*/
QByteArray Usart::serialDistinguishRecData(QByteArray& RecData)
{
    QByteArray dataBuff; //要把这个返回
    dataBuff = RecData;  //先复制一份

    //验证帧头帧尾，数据是否正确
    if((serialFrameHeader == RecData[headerIndex])&&(serialFrameEnder == RecData[enderIndex]))
    {
        switch(int(RecData[deviceIndex]))
        {
            case temperatureSensor:
            {
                dataBuff = RecData;
                dataBuff.remove(0,2);//去掉帧头和设备号
                dataBuff.chop(1);    //去掉帧尾
                cout<<dataBuff;
                return dataBuff;
            }
            default:
            {
                dataBuff.append("读取设备号出错");
                cout<<"读取设备号出错"<<dataBuff;
                return dataBuff;
            }
        }
    }
    else
    {
        dataBuff.append("未接收正确数据");
        cout<<"未接收正确数据"<<dataBuff;
        return dataBuff;
    }
}

/*
 * 串口发送数据 函数实现
 * 还没写！！！后面补！！！
*/
void Usart::serialSendData()
{
    QString val ;//发送的数据

    if(val.length()<=0)
        return;

}

