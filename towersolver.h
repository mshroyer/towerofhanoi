#ifndef TOWERSOLVER_H
#define TOWERSOLVER_H

#include <QThread>
#include <QSemaphore>

#include "datatypes.h"
#include "tower.h"

class TowerSolver : public QThread
{
    Q_OBJECT

public:
    explicit TowerSolver(Tower *tower, QObject *parent = nullptr);

signals:
    void moveTowerCalled(int n, TowerStack from, TowerStack spare, TowerStack to,
                         MoveTowerRecursion recursion, void *frame);
    void moveTowerReturned();
    void moveDisk(TowerStack from, TowerStack to);

public:
    void stopSolver();

protected:
    virtual void run() override;

private:
    void moveTower(int n, TowerStack from, TowerStack to, TowerStack spare,
                   MoveTowerRecursion recursion = MoveTowerRecursion::ROOT);
    void interruptibleSleep(int ms);

    QSemaphore m_sem;
    Tower *m_tower;
};

#endif // TOWERSOLVER_H
