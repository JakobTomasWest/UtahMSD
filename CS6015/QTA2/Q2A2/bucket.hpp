#ifndef BUCKET_HPP
#define BUCKET_HPP

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

class bucket : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit bucket(QObject *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // BUCKET_HPP
