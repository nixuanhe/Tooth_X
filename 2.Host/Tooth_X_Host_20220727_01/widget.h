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



#include "usart.h"
#include "qcustomplot.h"





//调试宏定义
#define cout qDebug() << "[" << __FILE__ <<":" << __LINE__ << "]"


//命名空间
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


//总的窗口类
class Widget : public QWidget
{
    Q_OBJECT

public:


    Widget(QWidget *parent = nullptr);
    ~Widget();

//声明槽函数
public slots:
    void time_updata(void);

private slots:
    void on_open_close_serialPort_clicked(bool checked);

private:
    Ui::Widget *ui;
    Usart *myUsart = new Usart(0xaa,0x55); //实例化串口对象，usart调用UI -3
    QTimer timerForCommon;
    void initAll(void);                      //初始化函数
    void windowsInit(void);
    void getComNumBox(void);                 //初始化串口号下拉框
    void initSerialBandrateComBox(void);     //初始化波特率下拉框
    void initSerialDataBitComBox(void);      //初始化数据位下拉框
    void initSerialVerifyBitComBox(void);    //初始化校验位下拉框
    void initSerialStopBitComBox(void);      //初始化停止位下拉框
    void initTemperaturePlot(void);          //初始化温度图
    void temperaturePlotUpdata(void);        //更新温度图表
    void uiControlEnable(bool isEnable);

};




#endif // WIDGET_H
