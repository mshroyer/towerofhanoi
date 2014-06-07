#ifndef TOWERSOLVER_H
#define TOWERSOLVER_H

#include "datatypes.h"
#include "tower.h"

#include <QThread>
#include <QSemaphore>

#include <atomic>

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
    void step();
    void stop();

protected:
    virtual void run() override;

private:
    void moveTower(int n, TowerStack from, TowerStack to, TowerStack spare,
                   MoveTowerRecursion recursion = MoveTowerRecursion::ROOT);

    std::atomic<bool> m_stopRequested;
    QSemaphore m_semaphore;
    Tower *m_tower;
};

#endif // TOWERSOLVER_H
