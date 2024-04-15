#include "bucket.h"

bucket::bucket(QObject *parent) : QObject(parent), QGraphicsPixmapItem() {
    setPixmap(QPixmap(":/images/bucket.png").scaled(150, 150));
    setPixmap(QPixmap(":/images/cloud.png").scaled(100, 100));

}

void bucket::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Right) {
        setPos(x() + 10, y());
    } else if (event->key() == Qt::Key_Left) {
        setPos(x() - 10, y());
    }
}
