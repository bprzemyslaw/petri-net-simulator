#ifndef PLACE_H
#define PLACE_H

#include "node.h"
#include <QtGui>

// x to jest x środka, tak samo y
class Place : public Node
{
    Q_OBJECT

public:
    Place(int x, int y, int liveness);
    Place(int id, int x, int y, int liveness);

    void draw(QPainter &painter);

    int liveness;
private:
    int radius;
};

#endif // PLACE_H
