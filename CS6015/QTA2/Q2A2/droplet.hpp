#ifndef DROPLET_HPP
#define DROPLET_HPP

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class droplet : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit droplet(QObject *parent = nullptr);
    ~droplet();

public slots:
    void drop();

private:
    QTimer *timer;
};

#endif // DROPLET_HPP
