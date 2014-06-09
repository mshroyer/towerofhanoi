#ifndef TOWERSOLVER_H
#define TOWERSOLVER_H

#include "datatypes.h"
#include "tower.h"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class TowerSolver : public QThread
{
    Q_OBJECT

public:
    explicit TowerSolver(Tower *tower, QObject *parent = nullptr);

signals:
    void moveTowerCalled(int n, TowerStack from, TowerStack to, TowerStack spare,
                         int sub, void *frame);
    void moveTowerReturned();
    void moveDisk(TowerStack from, TowerStack to);

public:
    void start();
    void step();
    void stop();

protected:
    virtual void run() override;

private:
    void moveTower(int n, TowerStack from, TowerStack to, TowerStack spare,
                   int sub = -1);

    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_stopRequested;
    int m_stepsRequested;

    Tower *m_tower;
};

#endif // TOWERSOLVER_H
