#include "searchlineedit.h"
#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
{
    lineEdit = new InnerLineEdit(this);
    lineEdit->move(8,38);
    lineEdit->resize(168,23);
    connect(lineEdit,SIGNAL(textChanged(QString)),this,SLOT(textChange(QString)));
    connect(lineEdit,SIGNAL(pressed()),this,SLOT(editClicked()));

    btn = new QPushButton(this);
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setCursor(Qt::PointingHandCursor);
    btn->resize(14,14);
    btn->move(157,42);
    btn->setStyleSheet("QPushButton{background-color:transparent;border:0px;}");
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setVisible(false);
    connect(btn,SIGNAL(clicked()),this,SLOT(btnClicked()));

    this->setFocus();
    lineEdit->addValue("C");
    lineEdit->addValue("C++");
    lineEdit->addValue("Python");
    lineEdit->addValue("Php");
    lineEdit->addValue("Perl");
    lineEdit->addValue("Qt");
}
SearchLineEdit::~SearchLineEdit()
{

}

void SearchLineEdit::resizeEvent(QResizeEvent *event)
{
    move((QApplication::desktop()->width() - width())/2,  (QApplication::desktop()->height() - height())/2);
    QWidget::resizeEvent(event);
}

void SearchLineEdit::textChange(QString text)
{
    emit searchTextChanged(text);
}

void SearchLineEdit::editClicked()
{
    lineEdit->setSearching();
    btn->setVisible(true);
}

void SearchLineEdit::btnClicked()
{
    lineEdit->setNormal();
    btn->setVisible(false);
    this->setFocus();
}


/*******************************/
InnerLineEdit::InnerLineEdit(QWidget *parent)
   : QLineEdit(parent)
{
    setTextMargins(0,0,this->height(),0);
    setStyleSheet("background-image:url(:/searchInput.png);border:1px solid #536874;");

    QCompleter *completer = new QCompleter(this);
    listModel = new QStringListModel(valueList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setModel(listModel);
    this->setCompleter(completer);
    connect(this, SIGNAL(editingFinished()), this, SLOT(editComplete()));

}

void InnerLineEdit::setNormal()
{
    setText("");
    setTextMargins(-5,0,this->height(),0);    //solve the cursor problem
    setStyleSheet("background-image:url(:/searchInput.png);border:1px solid #536874;");
}

void InnerLineEdit::setSearching()
{
    setTextMargins(0,0,this->height(),0);
    setStyleSheet("background-image:url(:/searchInput_click.png);border:1px solid #536874;");
}

void InnerLineEdit::addValue(const QString &value)
{
    valueList.append(value);
    listModel->setStringList(valueList);
}

void InnerLineEdit::focusInEvent(QFocusEvent *event)
{
    emit pressed();
    QLineEdit::focusInEvent(event);
}

void InnerLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
}

void InnerLineEdit::editComplete()
{
    QString text = this->text();
    if(QString::compare(text, QString("")) != 0) {
        bool flag = valueList.contains(text, Qt::CaseInsensitive);
        if(!flag) {
            addValue(text);
        }
    }
}
