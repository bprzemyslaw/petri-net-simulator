#ifndef TRANSITION_H
#define TRANSITION_H

#include "element.h"

namespace Ui {
class Transition;
}

class Transition : public Element
{
    Q_OBJECT

public:
    static int count;
    explicit Transition(const QPoint &origin, QWidget *parent = 0);
    virtual ~Transition();

public slots:
    void onTicked();

private:
    virtual void updateNumber();
    Ui::Transition *ui;
};

#endif // TRANSITION_H
