#include "mplayerwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MPlayerWidget w;
    w.show();

    return a.exec();
}
