#ifndef FORMDATE_H
#define FORMDATE_H

#include <QWidget>

namespace Ui {
class Formdate;
}

class Formdate : public QWidget
{
    Q_OBJECT

public:
    explicit Formdate(QWidget *parent = 0);
    ~Formdate();

private:
    Ui::Formdate *ui;
};

#endif // FORMDATE_H
