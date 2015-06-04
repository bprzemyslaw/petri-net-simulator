#include "place.h"
#include "ui_place.h"
#include <QMenu>
#include <QMouseEvent>
#include "matejkocanvas.h"

int Place::count = 0;

Place::Place(QPoint &origin, int liveness, QWidget *parent) :
    Element(parent), liveness(liveness),
    ui(new Ui::Place)
{
    ui->setupUi(this);
    this->setGeometry(origin.x() - this->width()/2, origin.y() - this->height()/2, this->width(), this->height());
    this->setChildrenClickable(false);

    this->letter = "P";
    this->basicStyleSheet = this->styleSheet();
    ui->livenessLabel->setText(QString::number(liveness));

    ++Place::count;
    this->setNumber(Place::count);

    this->show();
}

Place::~Place() {
    delete ui;
    --Place::count;
}

void Place::updateNumber() {
    ui->nameLabel->setText(this->letter + QString::number(this->number()));
}


