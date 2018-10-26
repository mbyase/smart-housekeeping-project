#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include "form_settings.h"
#include "formdate.h"


int main(int argc, char *argv[])
{



    //中文设置
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
