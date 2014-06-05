#ifndef TOWERSOLVER_H
#define TOWERSOLVER_H

#include <QThread>

#include "datatypes.h"
#include "tower.h"

class TowerSolver : public QThread
{
    Q_OBJECT

public:
    explicit TowerSolver(Tower *tower, QObject *parent = nullptr);

signals:
    void moveTowerCalled(int n, TowerStack from, TowerStack to, TowerStack spare,
                         MoveTowerRecursion recursion, void *frame);
    void moveTowerReturned();
    void moveDisk(TowerStack from, TowerStack to);

public slots:

protected:
    virtual void run() override;

private:
    void moveTower(int n, TowerStack from, TowerStack to, TowerStack spare,
                   MoveTowerRecursion recursion = MoveTowerRecursion::ROOT);
    void signalMoveTowerCalled(int n, TowerStack from, TowerStack to, TowerStack spare,
                               MoveTowerRecursion recursion, void *frame);
    void signalMoveTowerReturned();
    void signalMoveDisk(TowerStack from, TowerStack to);

    Tower *m_tower;
};

#endif // TOWERSOLVER_H
