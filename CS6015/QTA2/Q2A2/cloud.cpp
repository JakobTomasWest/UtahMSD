#include "cloud.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>

cloud::cloud(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
}

void cloud::setupGraphics(const QString &imagePath, int width, int height) {
    QPixmap pixmap(imagePath);
    this->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio));
}
