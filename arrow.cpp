#include "arrow.h"
#include <QDebug>
#include <QRect>

Arrow::Arrow(Place *place, Transition *transition, bool fromPlaceToTransition, QObject *parent):
    QObject(parent), place(place), transition(transition), fromPlaceToTransition(fromPlaceToTransition)
{
    arrowheadImage.load(":/images/images/arrowhead.png");
}

Arrow::Arrow(QObject *parent): QObject(parent), fromPlaceToTransition(true)
{
    arrowheadImage.load(":/images/images/arrowhead.png");
}

Arrow::Arrow(const Arrow &arrow)
{
    arrowheadImage.load(":/images/images/arrowhead.png");

    this->setParent(arrow.parent());
    this->place = arrow.place;
    this->transition = arrow.transition;
    this->fromPlaceToTransition = arrow.fromPlaceToTransition;
}

void Arrow::clear()
{
    place = NULL;
    transition = NULL;
    fromPlaceToTransition = true;
}

void Arrow::draw(QPainter &painter) const {
    QPoint placeMiddle = QPoint(this->place->x() + this->place->width()/2, this->place->y() + this->place->height()/2);
    QPoint transitionMiddle = QPoint(this->transition->x() + this->transition->width()/2, this->transition->y() + this->transition->height()/2);
    QPoint *middle1, *middle2;
    int height1, height2, width1, width2;
    if (this->fromPlaceToTransition) {
        middle1 = &placeMiddle;
        height1 = this->place->height();
        width1 = this->place->width();
        middle2 = &transitionMiddle;
        height2 = this->transition->height();
        width2 = this->transition->width();
    }
    else {
        middle1 = &transitionMiddle;
        height1 = this->transition->height();
        width1 = this->transition->width();
        middle2 = &placeMiddle;
        height2 = this->place->height();
        width2 = this->place->width();
    }

    int dy = middle2->y() - middle1->y();
    int dx = middle2->x() - middle1->x();
    int y1Offset = dy > 0 ? height1/2 : -height1/2;
    int y2Offset = dy > 0 ? height2/2 : -height2/2;
    int x1Offset = dx > 0 ? width1/2 : -width1/2;
    int x2Offset = dx > 0 ? width2/2 : -width2/2;

    bool vertical = true;
    if (abs(dx) > abs(dy)) {
        vertical = false;
    }

    // Identify points
    QVector<QPoint> checkPoints(6);
    if (vertical) {
        bool overlapsX = middle1->x() >= (middle2->x() - width2/2) && middle1->x() <= (middle2->x() + width2/2);
        if (overlapsX) {
            int yDistance = dy - y1Offset - y2Offset;
            checkPoints.append(QPoint(middle1->x(), middle1->y() + y1Offset));
            checkPoints.append(QPoint(checkPoints.last().x(), checkPoints.last().y() + yDistance/2));

            checkPoints.append(checkPoints.last());
            checkPoints.append(QPoint(checkPoints.last().x() + dx, checkPoints.last().y()));

            checkPoints.append(checkPoints.last());
            checkPoints.append(QPoint(checkPoints.last().x(), checkPoints.last().y() + (yDistance - yDistance/2)));
        }
        else {
            checkPoints.append(QPoint(middle1->x(), middle1->y() + y1Offset));
            checkPoints.append(QPoint(checkPoints.last().x(), checkPoints.last().y() + (dy - y1Offset)));

            checkPoints.append(checkPoints.last());
            checkPoints.append(QPoint(checkPoints.last().x() + (dx - x2Offset), checkPoints.last().y()));
        }
    }
    else {
        bool overlapsY = middle1->y() >= (middle2->y() - height2/2) && middle1->y() <= (middle2->y() + height2/2);
        if (overlapsY) {
            int xDistance = dx - x1Offset - x2Offset;
            checkPoints.append(QPoint(middle1->x() + x1Offset, middle1->y()));
            checkPoints.append(QPoint(checkPoints.last().x() + xDistance/2, checkPoints.last().y()));

            checkPoints.append(checkPoints.last());
            checkPoints.append(QPoint(checkPoints.last().x(), checkPoints.last().y() + dy));

            checkPoints.append(checkPoints.last());
            checkPoints.append(QPoint(checkPoints.last().x() + (xDistance - xDistance/2), checkPoints.last().y()));
        }
        else {
            checkPoints.append(QPoint(middle1->x() + x1Offset, middle1->y()));
            checkPoints.append(QPoint(checkPoints.last().x() + (dx - x1Offset), checkPoints.last().y()));

            checkPoints.append(checkPoints.last());
            checkPoints.append(QPoint(checkPoints.last().x(), checkPoints.last().y() + (dy - y2Offset)));
        }
    }

    // Draw lines
    painter.drawLines(checkPoints);

    // Rotate and move arrow
    QPoint secondLast = checkPoints[checkPoints.count() - 2];
    QPoint last = checkPoints.last();
    QTransform transformation;
    QImage arrowhead = this->arrowheadImage;
    QPoint imagePosition(last.x() - arrowhead.width()/2, last.y() - arrowhead.height()/2);
    bool verticalDirection = last.x() - secondLast.x() == 0;
    if (verticalDirection) {
        bool up = last.y() - secondLast.y() > 0 ? false : true;
        if (up) {
            transformation.rotate(90, Qt::ZAxis);
            arrowhead = arrowhead.transformed(transformation);
            imagePosition.setY(imagePosition.y() + arrowhead.height()/2);
        }
        else {
            transformation.rotate(-90, Qt::ZAxis);
            arrowhead = arrowhead.transformed(transformation);
            imagePosition.setY(imagePosition.y() - arrowhead.height()/2);
        }
    }
    else {
        bool right = last.x() - secondLast.x() > 0 ? true : false;
        if (right) {
            transformation.rotate(180, Qt::YAxis);
            arrowhead = arrowhead.transformed(transformation);
            imagePosition.setX(imagePosition.x() - arrowhead.width()/2);
        } else {
            imagePosition.setX(imagePosition.x() + arrowhead.width()/2);
        }
    }

    // Draw arrow
    painter.drawImage(imagePosition, arrowhead);
}

