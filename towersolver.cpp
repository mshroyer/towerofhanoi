#include "towersolver.h"

#define _moveTower(n, from, to, spare, recursion) \
    do { \
        moveTower(n, from, to, spare, recursion); \
        if (m_semaphore.available()) \
            return; \
    } while (0)

#define _moveDisk(from, to) \
    do { \
        emit moveDisk(from, to); \
        if (m_semaphore.tryAcquire(1, 350)) { \
            m_semaphore.release(1); \
            return; \
        } \
    } while (0)

TowerSolver::TowerSolver(Tower *tower, QObject *parent) :
    QThread { parent },
    m_semaphore { 0 },
    m_tower { tower }
{
    connect(this, &TowerSolver::moveDisk, tower, &Tower::moveDisk, Qt::QueuedConnection);
}

void TowerSolver::stop()
{
    m_semaphore.release(1);
    wait();
    m_semaphore.acquire(1);
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
        _moveTower(n-1, from, spare, to, MoveTowerRecursion::LEFT);
    }

    // Second, move the bottom disk to the target stack
    _moveDisk(from, to);

    // Third, move all except bottom disk from spare to the target stack
    if (n > 1) {
        _moveTower(n-1, spare, to, from, MoveTowerRecursion::RIGHT);
    }

    emit moveTowerReturned();
}
