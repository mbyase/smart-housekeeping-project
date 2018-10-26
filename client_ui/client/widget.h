#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTcpSocket>
#include<QPainter>
#include"jasonqt_vop.h"
#include <QTextToSpeech>
#include "form_settings.h"
#include "form_date.h"
namespace Ui
{
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void read_data();
    void read_data2();
    void startInput(void);
    void inputFinish(void);


  //  void on_pushButton_start_clicked();

    void on_pushButton_connect_clicked();



    void on_pushButton_settings_clicked();





  //  void on_pushButton_date_clicked();

    void on_pushButton_webnew_clicked();



    void on_pushButton_2_clicked();

    void on_pushButton_weather_clicked();



    void on_pushButton_send_clicked();

    void on_pushButton_connect_2_clicked();

private:
    Ui::Widget *ui;                     //
    QTcpSocket tcpsocket;
    QTcpSocket tcpsocket2;
    JasonQt_Vop::BaiduVop baiduvop;    //


    QTextToSpeech *m_speech;
    QVector<QVoice> m_voices;   //文字转语音


};

#endif // WIDGET_H
