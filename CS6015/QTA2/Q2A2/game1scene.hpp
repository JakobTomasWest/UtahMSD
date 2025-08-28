#ifndef GAME1SCENE_HPP
#define GAME1SCENE_HPP

#include <QGraphicsScene>
#include "bucket.hpp"
#include "droplet.hpp"
class game1scene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit game1scene(QObject *parent = nullptr);
private:
    bucket *bucketItem;
    QTimer *dropletTimer;
private slots:
    void spawnDroplet();
};



#endif // GAME1SCENE_HPP
