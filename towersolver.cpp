#include "towersolver.h"

TowerSolver::TowerSolver(Tower *tower, QObject *parent) :
    QThread { parent },
    m_tower { tower }
{
}

void TowerSolver::run()
{
    step(m_tower->ndisks(), Tower::Stack::LEFT, Tower::Stack::MIDDLE, Tower::Stack::RIGHT);
}

void TowerSolver::step(int n, Tower::Stack from, Tower::Stack spare, Tower::Stack to, bool right)
{
    if (n == 0)
        return;

    step(n-1, from, to, spare, false);
    m_tower->moveDisk(from, to);
    if (n > 1 || !right)
        QThread::msleep(350);
    step(n-1, spare, from, to, right);
}
