#include "widget.h"
#include "ui_widget.h"
#include<QHostAddress>
#include<QByteArray>
#include<QString>
#include <QAudioInput>
#include <QAudioOutput>
#include<QDebug>
#include<QAction>
#include<QPalette>
#include<QShortcut>
#include <QTextCodec>
#include <QTextToSpeech>
#include <QLoggingCategory>
#include <QThread>
#include "form_settings.h"
#include "formdate.h"
#include <QDialog>
#include <QUrl>
#include <QDesktopServices>

//#include <QWebEngineView>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    baiduvop("1NCcvMhMMafRhLvyNg6kePou", "G606YPDBNMWUIKe8cN9CLwITlmrXXjpl")
// 上面是百度的API Key和Secret Key


{
    ui->setupUi(this);
    this->setWindowTitle("client");


   // QTextToSpeech *tts = new QTextToSpeech(this);

    // List the available engines.
       QStringList engines = QTextToSpeech::availableEngines();
       qDebug() <<"Available engines:";
       for (auto engine : engines) {
           qDebug()<< "  " <<engine;
       }

       // Create an instance using the default engine/plugin.
       QTextToSpeech *speech = new QTextToSpeech();



       // List the available locales.
       qDebug() << "Available locales:";
       for (auto locale : speech->availableLocales())
       {
           qDebug()<< "  "<<locale;

    }

       // Set locale.设置地区语言
          speech->setLocale(QLocale(QLocale::Chinese, QLocale::SimplifiedHanScript, QLocale::China));


              qDebug() << "Available voices:";
              for (auto voice : speech->availableVoices()) {
                  qDebug()<< "  "<<voice.name();
              }

              // Display properties.
              qDebug() << "Locale:" << speech->locale();
              qDebug() << "Pitch:" << speech->pitch();
              qDebug() << "Rate:"<< speech->rate();
              qDebug()<< "Voice:"<< speech->voice().name();
              qDebug() << "Volume:" << speech->volume();
              qDebug() << "State:" << speech->state();


    //**************************************************************
    QHostAddress hostaddress;
    hostaddress.setAddress("192.168.2.102");         //设置IP地址
   // tcpsocket.connectToHost(hostaddress,9999);

    connect(&tcpsocket,SIGNAL(readyRead()),this,SLOT(read_data()));
    connect(&tcpsocket2,SIGNAL(readyRead()),this,SLOT(read_data2()));
    /******************************************************/
    //Button设计
    QString button_style="QPushButton{background-color:rgb(113, 190, 237);color:white; border-radius:10px; border:2px groove gray; border-style: outset;}"
                         "QPushButton:hover{background-color:white; color: black;}"
                         "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset; }";
    ui->pushButton_connect->setStyleSheet(button_style);
    //ui->pushButton_start->setStyleSheet(button_style);



   //设置背景图片
   this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
//   QPalette palette = this->palette();
//   palette.setBrush(QPalette::Window,
//           QBrush(QPixmap("../a.jpg").scaled(// 缩放背景图.
//               this->size(),
//               Qt::IgnoreAspectRatio,
//               Qt::SmoothTransformation)));             // 使用平滑的缩放方式
//   this->setPalette(palette);                           // 给widget加上背景图
   /*************************************************************/

   //lineEdit设置
   ui->lineEdit_ip->setPlaceholderText(QStringLiteral("IP地址"));



   //lineEdit回车键发送
   QShortcut *key=new QShortcut(QKeySequence(Qt::Key_Return),this);//创建一个快捷键"Key_Return"键
   connect(key,SIGNAL(activated()),this,SLOT(on_pushButton_start_clicked()));//连接到指定槽函数




   //录制语音 与停止录制语音 按钮
     connect(ui->pushButton,&QPushButton::clicked,[=](const bool  &checked)
        {

         if(checked)
         {
            // ui->pushButton->setText("stop");

             this->startInput();
             qDebug()<<"please speaking";

         }

         else {
          //   ui->pushButton->setText("start");
             this->inputFinish();
             qDebug()<<"speaking over";

         }
      });


     //开关风扇
     connect(ui->pushButton_fenshan,&QPushButton::clicked,[=](const bool  &checked)
        {
         if(checked)
         {
           //  QString str="turn on the fan";
           //  QString str="2";
             QString str="turn on the fan";
            // ui->lineEdit->setText(str);

           //  QString temp=ui->lineEdit->text();
            // ui->lineEdit->clear();
            //****************************************************
            // tcpsocket.write(str.toLocal8Bit());  //qt之间用这句
            tcpsocket2.write(str.toUtf8());          //与Linux下的服务器交互用这句
           //***************************************************

            ui->textBrowser_send->append("turn on the fan");

            // this->startInput();
             qDebug()<<"turn on the fan";

         }


         else {
             QString str="turn off the fan";
            //****************************************************
            // tcpsocket.write(str.toLocal8Bit());  //qt之间用这句
            tcpsocket2.write(str.toUtf8());          //与Linux下的服务器交互用这句
           //***************************************************
            ui->textBrowser_send->append("turn off the fan");
            // this->startInput();
             qDebug()<<"turn off the fan";
         }
      });




     //开关灯
     connect(ui->pushButton_deng,&QPushButton::clicked,[=](const bool  &checked)
        {
         if(checked)
         {
             QString str="turn on the light";
            // ui->lineEdit->setText(str);

           //  QString temp=ui->lineEdit->text();
            // ui->lineEdit->clear();
            //****************************************************
            // tcpsocket.write(str.toLocal8Bit());  //qt之间用这句
            tcpsocket2.write(str.toUtf8());          //与Linux下的服务器交互用这句
           //***************************************************

            ui->textBrowser_send->append("turn on the light");

             qDebug()<<"trun on the light";

         }


         else {
             QString str="turn off the light";
            //****************************************************
            // tcpsocket.write(str.toLocal8Bit());  //qt之间用这句
            tcpsocket2.write(str.toUtf8());          //与Linux下的服务器交互用这句
           //***************************************************
            ui->textBrowser_send->append("turn off the light");
            // this->startInput();
             qDebug()<<"turn off the light";
         }
      });



//开关空调
connect(ui->pushButton_kong,&QPushButton::clicked,[=](const bool  &checked)
   {
    if(checked)
    {
        QString str="turn on the air conditioner";
       // ui->lineEdit->setText(str);

      //  QString temp=ui->lineEdit->text();
       // ui->lineEdit->clear();
       //****************************************************
       // tcpsocket.write(str.toLocal8Bit());  //qt之间用这句
       tcpsocket2.write(str.toUtf8());          //与Linux下的服务器交互用这句
      //***************************************************

       ui->textBrowser_send->append("turn on the air conditioner");

        qDebug()<<"turn on the air conditioner";

    }


    else {
        QString str="turn off air conditioner";
       //****************************************************
       // tcpsocket.write(str.toLocal8Bit());  //qt之间用这句
       tcpsocket2.write(str.toUtf8());          //与Linux下的服务器交互用这句
      //***************************************************
       ui->textBrowser_send->append("turn off air conditioner");
       // this->startInput();
        qDebug()<<"turn off air conditioner";
    }
 });


//开关电视
connect(ui->pushButton_tv,&QPushButton::clicked,[=](const bool  &checked)
   {
    if(checked)
    {
        QString str="turn on the buzz";
       // ui->lineEdit->setText(str);

      //  QString temp=ui->lineEdit->text();
       // ui->lineEdit->clear();
       //****************************************************
       // tcpsocket.write(str.toLocal8Bit());  //qt之间用这句
       tcpsocket2.write(str.toUtf8());          //与Linux下的服务器交互用这句
      //***************************************************

       ui->textBrowser_send->append("turn on the buzz");

        qDebug()<<"turn on the buzz";

    }


    else {
        QString str="turn off the buzz";
       //****************************************************
       // tcpsocket.write(str.toLocal8Bit());  //qt之间用这句
       tcpsocket2
               .write(str.toUtf8());          //与Linux下的服务器交互用这句
      //***************************************************
       ui->textBrowser_send->append("turn off the buzz");
       // this->startInput();
        qDebug()<<"turn off the buzz";
    }
 });

qDebug()<<"refresh token :"<<baiduvop.refreshToken();

}


//读取服务器发来的数据
void Widget::read_data()

{

    QByteArray array=tcpsocket.readAll();   //原来是read(32);
 //  printf("read=%s",array);


    QString str(array);
  //   printf("str=%s",str);
    ui->textBrowser_receive->append(str);
    qDebug()<<str;
     QTextToSpeech *speech = new QTextToSpeech();
     speech->say(str);       //语音播报发来的内容

}


//读取服务器2发来的数据
void Widget::read_data2()

{

    QByteArray array=tcpsocket2.readAll();   //原来是read(32);
 //  printf("read=%s",array);


    QString str(array);
  //   printf("str=%s",str);
    ui->textBrowser_receive->append(str);
    qDebug()<<str;
     QTextToSpeech *speech = new QTextToSpeech();
     speech->say(str);       //语音播报发来的内容

}

Widget::~Widget()
{
    delete ui;
}


void Widget::startInput(void)
{

    baiduvop.start();


}



void Widget::inputFinish(void)
{
    const auto &&data=baiduvop.finish();
    if(data.first)
    {
      //  ui->textBrowser_send->append(data.second);
       //    ui->lineEdit->setText(data.second);
           QString str=data.second;
       //   tcpsocket.write(str.toLocal8Bit());    //qt之间用这种
       tcpsocket.write(str.toUtf8());     //与Linux下的服务器交互用这种
     //  int len=strlen(data.second);
       int len = str.length(); //length = 4;
        tcpsocket.write(str.toUtf8(),len);
         ui->textBrowser_send->append(str);

    }
    else {
        ui->textBrowser_send->setText("识别有误");
    }

}



//发送按钮
/*void Widget::on_pushButton_start_clicked()
{
    QString str=ui->lineEdit->text();

 //           tcpsocket.write(s);
    //****************************************************
    // tcpsocket.write(str.toLocal8Bit());  //qt之间用这句
    tcpsocket.write(str.toUtf8());          //与Linux下的服务器交互用这句
    //***************************************************
    ui->textBrowser_send->append(ui->lineEdit->text());
     QTextToSpeech *speech = new QTextToSpeech();
     speech->say(ui->lineEdit->text());
    ui->lineEdit->clear();
}

*/

//连接服务器按钮
void Widget::on_pushButton_connect_clicked()
{

    QHostAddress hostaddress;
    //hostaddress=ui->lineEdit_ip->text();
    QHostAddress temp;
    temp=ui->lineEdit_ip->text();
    hostaddress.setAddress("192.168.43.237");         //设置默认IP地址
    hostaddress.setAddress(ui->lineEdit_ip->text());  //根据输入到lineedit的IP地址来设定ip地址
    tcpsocket.connectToHost(hostaddress,9999);




}
void Widget::on_pushButton_connect_2_clicked()
{
    //192.168.137.7   7777
    QHostAddress hostaddress2;
    hostaddress2.setAddress("192.168.137.210");         //设置默认IP地址
    hostaddress2.setAddress(ui->lineEdit_ip_2->text());  //根据输入到lineedit的IP地址来设定ip地址
    tcpsocket2.connectToHost(hostaddress2,7777);
}

void Widget::on_pushButton_settings_clicked()
{
    Form_settings *form_settings=new Form_settings;
    form_settings->show();
    // Form_settings *form_settings=new Form_settings;   //新建一个窗口用于管理数据库
    // form_settings->show();                              //show出新窗口
 //   QDialog * dialog = new QDialog(this);   //创建一个新的会话窗口

  //  dialog -> show();
//}

//Dialog *dlg = new Dialog;
   //关联信号和槽函数
 //connect(form_settings,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));
  // dlg->setModal(true); 不论是模态或者非模态都可以正常传值
}



//void Widget::on_pushButton_date_clicked()
//{
//    Formdate *formdate=new Formdate;
//    formdate->show();

//}

void Widget::on_pushButton_webnew_clicked()
{
    const QUrl regUrl(QLatin1String("http://www.people.com.cn/"));
    QDesktopServices::openUrl(regUrl);

}



void Widget::on_pushButton_2_clicked()
{
    Formdate *formdate=new Formdate;
    formdate->show();
}

void Widget::on_pushButton_weather_clicked()
{
   QString str="今天天气怎么样";

    //QString strMessage = QString::fromLocal8Bit("我是UTF8编码的文件："));
   // ui->lineEdit->setText(str);

  //  QString temp=ui->lineEdit->text();
   // ui->lineEdit->clear();
   //****************************************************.toStdString().data()
   // tcpsocket.write(str.toLocal8Bit());
 //   tcpsocket.write(str.toStdString().data());  //qt之间用这句
  // tcpsocket.write(str.toUtf8());          //与Linux下的服务器交互用这句

  //  QString str = QString::fromLocal8Bit("今天天气怎么样");

    tcpsocket.write(str.toUtf8());          //,str.toUtf8().size());
  //***************************************************

   ui->textBrowser_send->append(str);


    qDebug()<<"weather";
}



void Widget::on_pushButton_send_clicked()
{
    QString str=ui->lineEdit_question->text();
 //   tcpsocket.write(str.toLocal8Bit());    //qt之间用这种
    tcpsocket.write(str.toUtf8());          //与Linux下的服务器交互用这种
    ui->textBrowser_send->append(str);
}

