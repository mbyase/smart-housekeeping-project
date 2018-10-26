#include "form_settings.h"
#include "ui_form_settings.h"
#include<QHostAddress>
#include<QDebug>
#include<QString>
#include <QTcpSocket>
#include<qbuffer.h>
#include<qimage.h>
#include<qimagereader.h>

#include <QPixmap>
//#include ""

Form_settings::Form_settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_settings)
{
    ui->setupUi(this);



    QHostAddress hostaddress;
    QString add;
    //**************************************************************


    hostaddress.setAddress("192.168.2.102");         //设置IP地址
    // tcpsocket.connectToHost(hostaddress,9999);

    connect(&tcpsocket,SIGNAL(readyRead()),this,SLOT(read_imagedata()));

   //  connect(&tcpsocket,SIGNAL(readyRead()),this,SLOT(myrecv()));   //************


}

Form_settings::~Form_settings()
{
    delete ui;
}
/*
void  Form_settings::myrecv()
{
    int piclen;
    char data[1024*1024];
   piclen = tcpsocket.read(20).toInt(); //接收图片长度
    //piclen=tcpsocket.readAll().toInt();
    qDebug()<<"piclen="<<piclen;

    int count = 0;
    while(count < piclen)
    {
        tcpsocket.waitForReadyRead(50);
        int ret = tcpsocket.read(data+count,piclen-count);
        if(-1 == ret)
        {
            qDebug()<<"read pic data error";
            return;
        }
        else if(0 == ret)
        {
            tcpsocket.waitForReadyRead(~0);
            qDebug()<<"read zero byte";
        }
        else
            count += ret;
            //qDebug()<<"count0="<<count;
    }
    qDebug()<<"count1="<<count;

    pixmap.loadFromData((uchar *)data, piclen, "JPEG");
    ui->label_movie->setPixmap(pixmap);
}
*/


//读取服务器发来的数据
void Form_settings::read_imagedata()

{


   // QByteArray array=tcpsocket.readAll();   //原来是read(32);
    QByteArray array=tcpsocket.readAll();   //原来是read(32);
    QString str(array);


    QBuffer buffer(&array);
    buffer.open(QIODevice::ReadOnly);

    QImageReader reader(&buffer,"JPG");
    QImage img = reader.read();


   // QByteArray array=tcpsocket.readAll();   //原来是read(32);
   // printf("%s",array);
    //342*240*3

  //  QString str(array);
   // ui->label->setPixmap(img);
    if(!img.isNull()){
        qDebug()<<"right"<<endl;
        ui->label_movie->setPixmap(QPixmap::fromImage(img));  //显示在label

    } else {
        qDebug()<<"error"<<endl;

   // ui->label->setPicture();

    // ui->textBrowser_receive->append(str);
   //  QTextToSpeech *speech = new QTextToSpeech();
    // speech->say(str);       //语音播报发来的内容



}
}



//void Form_settings::on_pushButton_connect_clicked()
//{

//    //hostaddress=ui->lineEdit_ip->text();
//    QHostAddress temp;
//    temp=ui->lineEdit_ip->text();
// //   hostaddress.setAddress("192.168.2.104");         //设置默认IP地址
// //   hostaddress.setAddress(ui->lineEdit_ip->text());  //根据输入到lineedit的IP地址来设定ip地址
//  add=ui->lineEdit_ip->text();

////tcpsocket.connectToHost(hostaddress,9999);
//}




//void Form_settings::on_pushButton_close_clicked()
//{
// //   tcpsocket.connectToHost(hostaddress,9999);
//    this->close();
//}

//开始传输视频
void Form_settings::on_pushButton_connect_clicked()
{
   // ui->label();

    tcpsocket.write("pic",32);
    /*
    QHostAddress hostaddress;
    //hostaddress=ui->lineEdit_ip->text();
    QHostAddress temp;
    temp=ui->lineEdit_ip->text();
    hostaddress.setAddress("192.168.43.237");         //设置默认IP地址
    hostaddress.setAddress(ui->lineEdit_ip->text());  //根据输入到lineedit的IP地址来设定ip地址
    tcpsocket.connectToHost(hostaddress,8888);*/



  //   QTextToSpeech *speech = new QTextToSpeech();
 //    speech->say(str);       //语音播报发来的内容

}

//结束传输视频
void Form_settings::on_pushButton_close_clicked()
{
    tcpsocket.close();

}

//连接子服务器
void Form_settings::on_pushButton_clicked()
{
    QHostAddress hostaddress;
    //hostaddress=ui->lineEdit_ip->text();
    QHostAddress temp;
    temp=ui->lineEdit_ip->text();
    hostaddress.setAddress("192.168.43.237");         //设置默认IP地址
    hostaddress.setAddress(ui->lineEdit_ip->text());  //根据输入到lineedit的IP地址来设定ip地址
    tcpsocket.connectToHost(hostaddress,8888);

}
