#include "towersolver.h"

TowerSolver::TowerSolver(Tower *tower, QObject *parent) :
    QThread { parent },
    m_tower { tower }
{
    connect(this, &TowerSolver::moveDisk, tower, &Tower::moveDisk, Qt::QueuedConnection);
}

void TowerSolver::run()
{
    moveTower(m_tower->ndisks(), TowerStack::LEFT, TowerStack::RIGHT, TowerStack::MIDDLE);
}

// Don't move these, for linking to moveTower() from stack trace window:
extern const char * const kMoveTowerFile = "towersolver.cpp";
extern const int          kMoveTowerLine = __LINE__ + 2;

void TowerSolver::moveTower(int n, TowerStack from, TowerStack to, TowerStack spare,
                            StepRecursion recursion)
{
    /*
     * This is a recursive solution to the Tower of Hanoi. For explanation:
     * https://en.wikipedia.org/wiki/Tower_of_Hanoi#Recursive_solution
     */

    // Nothing to do for move of zero disks
    if (n == 0)
        return;

    emit moveTowerCalled(n, from, to, spare, recursion, __builtin_frame_address(0));

    // Move all except bottom disk to spare stack
    moveTower(n-1, from, spare, to, StepRecursion::LEFT);

    // Move bottom disk to target stack
    emit moveDisk(from, to);
    QThread::msleep(350);

    // Move all except bottom disk from spare to target stack
    moveTower(n-1, spare, to, from, StepRecursion::RIGHT);

    emit moveTowerReturned();
}
