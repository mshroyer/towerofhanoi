#include "datatypes.h"
#include "towerofhanoi.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TowerOfHanoi w;

    qRegisterMetaType<TowerStack>("TowerStack");

    w.show();
    return a.exec();
}
