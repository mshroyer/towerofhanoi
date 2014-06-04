#include "towersolver.h"

#define CHECK_STOP do { if (m_sem.available()) return; } while (0)

TowerSolver::TowerSolver(Tower *tower, QObject *parent) :
    QThread { parent },
    m_sem { 1 },
    m_tower { tower }
{
    connect(this, &TowerSolver::moveDisk, tower, &Tower::moveDisk, Qt::QueuedConnection);
}

void TowerSolver::stopSolver()
{
    // TODO handle multiple invocations
    m_sem.release(1);
}

void TowerSolver::run()
{
    m_sem.acquire(1);
    moveTower(m_tower->ndisks(), TowerStack::LEFT, TowerStack::RIGHT, TowerStack::MIDDLE);
}

// Don't move these, for linking to moveTower() from stack trace window:
extern const char * const kMoveTowerFile = "towersolver.cpp";
extern const int          kMoveTowerLine = __LINE__ + 2;

void TowerSolver::moveTower(int n, TowerStack from, TowerStack to, TowerStack spare,
                            MoveTowerRecursion recursion)
{
    /*
     * This is a recursive solution to the Tower of Hanoi. For explanation:
     * https://en.wikipedia.org/wiki/Tower_of_Hanoi#Recursive_solution
     */

    CHECK_STOP;

    emit moveTowerCalled(n, from, to, spare, recursion, __builtin_frame_address(0));

    // First, move all except bottom disk to the spare stack
    if (n > 1) {
        moveTower(n-1, from, spare, to, MoveTowerRecursion::LEFT);
        CHECK_STOP;
    }

    // Second, move the bottom disk to the target stack
    emit moveDisk(from, to);
    QThread::msleep(350);
    CHECK_STOP;

    // Third, move all except bottom disk from spare to the target stack
    if (n > 1) {
        moveTower(n-1, spare, to, from, MoveTowerRecursion::RIGHT);
        CHECK_STOP;
    }

    emit moveTowerReturned();
}

void TowerSolver::interruptibleSleep(int ms)
{
    if (m_sem.tryAcquire(1, ms)) {
        m_sem.release(1);
    }
}
