#include "formdate.h"
#include "ui_formdate.h"

Formdate::Formdate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Formdate)
{
    ui->setupUi(this);
}

Formdate::~Formdate()
{
    delete ui;
}
