#include "game1scene.hpp"
#include <QBrush>
#include <QTimer>
#include "droplet.hpp"
#include "cloud.h"
#include "QGraphicsPixmapItem"
game1scene::game1scene(QObject *parent) : QGraphicsScene(parent) {
    setBackgroundBrush(QBrush(QImage(":/images/background.jpg").scaledToHeight(512).scaledToWidth(910)));
    setSceneRect(0, 0, 908, 510);



    bucket *bucketItem = new bucket();
    bucketItem->setPixmap(QPixmap(":/images/bucket.png").scaled(150, 150));
    bucketItem->setPos(400, 365);
    bucketItem->setFlag(QGraphicsItem::ItemIsFocusable);
    bucketItem->setFocus();
    addItem(bucketItem);


    cloud *cloud1 = new cloud();
    cloud1->setupGraphics(":/images/cloud.png", 200, 50);
    cloud1->setPos(50,0);
    addItem(cloud1);

    dropletTimer = new QTimer(this);
    connect(dropletTimer, &QTimer::timeout, this, &game1scene::spawnDroplet);
    dropletTimer->start(1000);

}

void game1scene::spawnDroplet(){
    droplet* newDroplet = new droplet(bucketItem);
    int randomXnum = arc4random() % static_cast<int>(sceneRect().width() - 30);
    newDroplet->setPos(randomXnum, 50);
    addItem(newDroplet);
}
