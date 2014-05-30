#include "towerofhanoi.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TowerOfHanoi w;
    w.show();

    return a.exec();
}
