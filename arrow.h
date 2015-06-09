#ifndef ARROW_H
#define ARROW_H

#include <QObject>
#include "pnsglobal.h"
#include "place.h"
#include "transition.h"

class Arrow : public QObject
{
    Q_OBJECT
public:
    explicit Arrow(Element *place, Element *transition, bool fromPlaceToTransition, QObject *parent = 0);
    explicit Arrow(QObject *parent = 0);
    Arrow(const Arrow &arrow);
    void clear();
    Element *place;
    Element *transition;
    bool fromPlaceToTransition;
    void draw(QPainter &painter) const;

    int weight() const;

signals:

public slots:
    void setWeight(int weight);

private:
    QImage arrowheadImage;
    int _weight;
};

#endif // ARROW_H
