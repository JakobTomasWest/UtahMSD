#ifndef CLOUD_H
#define CLOUD_H

#include <QGraphicsPixmapItem>

class cloud : public QGraphicsPixmapItem {
public:

    explicit cloud(QGraphicsItem *parent = nullptr);


    void setupGraphics(const QString &imagePath, int width, int height);
};

#endif // CLOUD_H
