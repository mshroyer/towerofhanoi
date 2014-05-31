#include "towersolver.h"

TowerSolver::TowerSolver(Tower *tower, QObject *parent) :
    QThread { parent },
    m_tower { tower }
{
    connect(this, &TowerSolver::moveDisk, tower, &Tower::moveDisk, Qt::QueuedConnection);
}

void TowerSolver::run()
{
    step(m_tower->ndisks(), Tower::Stack::LEFT, Tower::Stack::MIDDLE, Tower::Stack::RIGHT);
}

void TowerSolver::step(int n, Tower::Stack from, Tower::Stack spare, Tower::Stack to, bool rightmost)
{
    if (n == 0)
        return;

    step(n-1, from, to, spare, false);

    emit moveDisk(from, to);
    if (n > 1 || !rightmost)
        QThread::msleep(350);

    step(n-1, spare, from, to, rightmost);
}
