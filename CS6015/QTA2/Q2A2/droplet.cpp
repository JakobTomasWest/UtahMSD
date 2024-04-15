#include "droplet.hpp"
#include "bucket.hpp"
#include "game1scene.cpp"
#include <QGraphicsItem>
#include <QGraphicsScene>

droplet::droplet( QObject *parent) : QObject(parent), QGraphicsPixmapItem() {
    setPixmap(QPixmap(":/images/water.gif").scaled(30, 30));


    //create timer for droplet jobject
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &droplet::drop);
    timer->start(50);

    connect(this, &QObject::destroyed, this, &droplet::deleteLater);

}
droplet::~droplet() {
    timer->stop();
}

void droplet::drop(){
    //drop 5 pixels per second
    setPos(x(),y()+ 5);

    //If the droplet is out of bounds of the scene or collides with  the bucket delete it
    if(y() >500){
        scene()->removeItem(this);
        delete this;
    } else {

        QList<QGraphicsItem*> collidingItems = scene()->collidingItems(this);
        for (QGraphicsItem *item :collidingItems) {
            if (item->type() == droplet::Type or item->type() == bucket::Type) {
                scene()->removeItem(this);
                delete this;
                break;
            }
        }
    }

}
