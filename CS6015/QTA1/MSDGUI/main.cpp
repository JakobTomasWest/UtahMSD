#include "mainwidget.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // mainWidget w;
    // w.show();

    mainWidget* main = new mainWidget();
    main->show();
    return app.exec();
}
