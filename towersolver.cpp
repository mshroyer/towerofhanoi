#include "towersolver.h"

#include <QMutexLocker>

TowerSolver::TowerSolver(Tower *tower, QObject *parent) :
    QThread { parent },
    m_mutex { QMutex::NonRecursive },
    m_condition { },
    m_tower { tower }
{
    connect(this, &TowerSolver::moveDisk, tower, &Tower::moveDisk, Qt::QueuedConnection);
}

void TowerSolver::step()
{
    m_condition.wakeOne();
}

void TowerSolver::stop()
{
    if (isRunning()) {
        m_mutex.lock();
        m_stopRequested = true;
        m_mutex.unlock();

        m_condition.wakeOne();
        wait();

        m_mutex.lock();
        m_stopRequested = false;
        m_mutex.unlock();
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
        QMutexLocker locker { &m_mutex };
        if (m_stopRequested) {
            return;
        }
    }

    // Second, move the bottom disk to the target stack
    moveDisk(from, to);

    {
        QMutexLocker locker { &m_mutex };
        m_condition.wait(&m_mutex);
        if (m_stopRequested) {
            return;
        }
    }

    // Third, move all except bottom disk from spare to the target stack
    if (n > 1) {
        moveTower(n-1, spare, to, from, MoveTowerRecursion::RIGHT);
        QMutexLocker locker { &m_mutex };
        if (m_stopRequested) {
            return;
        }
    }

    emit moveTowerReturned();
}
