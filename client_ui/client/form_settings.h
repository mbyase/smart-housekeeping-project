#ifndef FORM_SETTINGS_H
#define FORM_SETTINGS_H

#include <QWidget>
#include<QTcpSocket>

namespace Ui {
class Form_settings;
}

class Form_settings : public QWidget
{
    Q_OBJECT

public:
    explicit Form_settings(QWidget *parent = 0);
    ~Form_settings();

private slots:
//    void on_pushButton_connect_clicked();

   // void on_pushButton_close_clicked();

    void read_imagedata();

    void on_pushButton_connect_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_clicked();

 //  void myrecv();   //****



private:
    Ui::Form_settings *ui;
     QTcpSocket tcpsocket;
     QPixmap pixmap;



};

#endif // FORM_SETTINGS_H
