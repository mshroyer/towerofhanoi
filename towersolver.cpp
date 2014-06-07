#include "towersolver.h"

#include <QMutexLocker>

TowerSolver::TowerSolver(Tower *tower, QObject *parent) :
    QThread { parent },
    m_stopRequested { false },
    m_semaphore { 0 },
    m_tower { tower }
{
    connect(this, &TowerSolver::moveDisk, tower, &Tower::moveDisk, Qt::QueuedConnection);
}

void TowerSolver::step()
{
    m_semaphore.release(1);
}

void TowerSolver::stop()
{
    if (isRunning()) {
        m_stopRequested.store(true);
        m_semaphore.release(1);
        wait();
        m_stopRequested.store(false);
    }
}

void TowerSolver::run()
{
    moveTower(m_tower->ndisks(), TowerStack::LEFT, TowerStack::RIGHT, TowerStack::MIDDLE);
}


// Don't move these, for linking to moveTower() from stack trace window:
extern const char * const kMoveTowerFile = "towersolver.cpp";
extern const int          kMoveTowerLine = __LINE__ + 2;

void TowerSolver::moveTower(int n, TowerStack from, TowerStack to, TowerStack spare,
                            MoveTowerRecursion recursion)
{
    emit moveTowerCalled(n, from, to, spare, recursion, __builtin_frame_address(0));

    /*
     * This is a recursive solution to the Tower of Hanoi.
     *
     * For detailed explanation:
     * https://en.wikipedia.org/wiki/Tower_of_Hanoi#Recursive_solution
     */

    // First, move all except bottom disk to the spare stack
    if (n > 1) {
        moveTower(n-1, from, spare, to, MoveTowerRecursion::LEFT);
        if (m_stopRequested.load()) return;
    }

    // (Synchronize with UI thread)
    m_semaphore.acquire(1);
    if (m_stopRequested.load()) return;

    // Second, move the bottom disk to the target stack
    emit moveDisk(from, to);

    // Third, move all except bottom disk from spare to the target stack
    if (n > 1) {
        moveTower(n-1, spare, to, from, MoveTowerRecursion::RIGHT);
        if (m_stopRequested.load()) return;
    }

    emit moveTowerReturned();
}
