#include "towersolver.h"

TowerSolver::TowerSolver(Tower *tower, QObject *parent) :
    QThread { parent },
    m_tower { tower }
{
    connect(this, &TowerSolver::moveDisk, tower, &Tower::moveDisk, Qt::QueuedConnection);
}

void TowerSolver::run()
{
    step(m_tower->ndisks(), Stack::LEFT, Stack::RIGHT, Stack::MIDDLE);
}

void TowerSolver::step(int n, Stack from, Stack to, Stack spare, StepRecursion recursion)
{
    if (n > 0) {
        emit stepCall(n, from, to, spare, recursion, __builtin_frame_address(0));
        step(n-1, from, spare, to, StepRecursion::LEFT);

        emit moveDisk(from, to);
        QThread::msleep(350);

        step(n-1, spare, to, from, StepRecursion::RIGHT);
        emit stepReturn();
    }
}
