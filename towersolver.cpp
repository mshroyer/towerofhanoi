#include "towersolver.h"

TowerSolver::TowerSolver(Tower *tower, QObject *parent) :
    QThread { parent },
    m_tower { tower }
{
    connect(this, &TowerSolver::moveDisk, tower, &Tower::moveDisk, Qt::QueuedConnection);
}

void TowerSolver::run()
{
    step(m_tower->ndisks(), Tower::Stack::LEFT, Tower::Stack::RIGHT, Tower::Stack::MIDDLE);
}

void TowerSolver::step(int n, Tower::Stack from, Tower::Stack to, Tower::Stack spare, bool rightmost)
{
    if (n == 0)
        return;

    emit stepCall(n, from, to, spare);

    step(n-1, from, spare, to, false);

    emit moveDisk(from, to);
    if (n > 1 || !rightmost)
        QThread::msleep(350);

    step(n-1, spare, to, from, rightmost);

    emit stepReturn();
}
