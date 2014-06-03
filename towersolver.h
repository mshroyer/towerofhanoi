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
    void stepCall(int n, Stack from, Stack spare, Stack to, StepRecursion recursion, void *frame);
    void stepReturn();
    void moveDisk(Stack from, Stack to);

public slots:

protected:
    virtual void run() override;

private:
    void step(int n, Stack from, Stack to, Stack spare, StepRecursion recursion = StepRecursion::ROOT);

    Tower *m_tower;
};

#endif // TOWERSOLVER_H
