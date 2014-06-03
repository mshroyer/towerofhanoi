#include "datatypes.h"
#include "towerofhanoi.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TowerOfHanoi w;

    qRegisterMetaType<Stack>("Stack");
    qRegisterMetaType<StepRecursion>("StepRecursion");

    w.show();
    return a.exec();
}
