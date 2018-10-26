#-------------------------------------------------
#
# Project created by QtCreator 2018-07-13T16:15:25
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimedia

QT       += widgets texttospeech
#QT += axcontainer
#QT  += webkit
#QT += webenginewidgets


QT       += texttospeech
requires(qtConfig(combobox))

 RC_FILE = logo.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client         #原来是 untitled
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    jasonqt_net.cpp \
    jasonqt_vop.cpp \
    jasonqt_net.cpp \
    jasonqt_vop.cpp \
    main.cpp \
    widget.cpp \
    form_settings.cpp \
    form_date.cpp \
    formdate.cpp

HEADERS += \
        widget.h \
    jasonqt_net.h \
    jasonqt_vop.h \
    jasonqt_net.h \
    jasonqt_vop.h \
    widget.h \
    form_settings.h \
    form_settings.h \
    form_date.h \
    formdate.h

FORMS += \
        widget.ui \
    form_settings.ui \
    formdate.ui

DISTFILES += \
    picture/timg.jpg \
    picture/a.jpg \
    picture/aa.png \
    a.jpg \
    aa.png \
    timg.jpg \
    3.ico \
    1.ico \
    2.ico \
    3.ico \
    logo.ico

RESOURCES += \
    pic.qrc


