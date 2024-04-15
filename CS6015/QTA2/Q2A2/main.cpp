#include <QApplication>
#include "game1scene.hpp"
#include <QGraphicsView>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    game1scene *scene = new game1scene(); // Instantiate the game scene
    QGraphicsView *view = new QGraphicsView(scene); // Create the view

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(910, 512);
    view->show(); // Display the view

    return a.exec();
}
