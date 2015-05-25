#ifndef ARROW_H
#define ARROW_H

#include <QObject>
#include "place.h"
#include "transition.h"
#include <QPainter>

class Arrow : public QObject
{
    Q_OBJECT
public:
    explicit Arrow(Place *place, Transition *transition, bool fromPlaceToTransition, QObject *parent = 0);
    explicit Arrow(QObject *parent = 0);
    Arrow(const Arrow &arrow);
    void clear();
    Place *place;
    Transition *transition;

    bool fromPlaceToTransition;

    void draw(QPainter &painter) const;
signals:

public slots:

};

#endif // ARROW_H
