#include "towersolver.h"

#include <QMutexLocker>

TowerSolver::TowerSolver(Tower *tower, QObject *parent) :
    QThread { parent },
    m_mutex { QMutex::NonRecursive },
    m_condition { },
    m_stopRequested { false },
    m_stepsRequested { 0 },
    m_tower { tower }
{
    connect(this, &TowerSolver::moveDisk, tower, &Tower::moveDisk, Qt::QueuedConnection);
}

void TowerSolver::start()
{
    QMutexLocker locker { &m_mutex };
    m_stopRequested = false;
    m_stepsRequested = 0;
    locker.unlock();

    QThread::start();
}

void TowerSolver::step()
{
    QMutexLocker locker { &m_mutex };
    ++m_stepsRequested;
    m_condition.wakeOne();
}

void TowerSolver::stop()
{
    if (!isRunning())
        return;

    QMutexLocker locker { &m_mutex };
    m_stopRequested = true;
    m_condition.wakeOne();
    locker.unlock();

    wait();
}

void TowerSolver::run()
{
    moveTower(m_tower->ndisks(), TowerStack::LEFT, TowerStack::RIGHT, TowerStack::MIDDLE);

    // Pause after last step so that stack trace is shown for final disk move
    {
        QMutexLocker locker { &m_mutex };

        if (m_stopRequested)
            return;

        if (m_stepsRequested == 0)
            m_condition.wait(&m_mutex);
    }
}


// Don't move these, for linking to moveTower() source from stack trace window:
extern const char * const kMoveTowerFile = "towersolver.cpp";
extern const int          kMoveTowerLine = __LINE__ + 2;

void TowerSolver::moveTower(int n, TowerStack from, TowerStack to, TowerStack spare, int sub)
{
    emit moveTowerCalled(n, from, to, spare, sub, __builtin_frame_address(0));

    /*
     * This is a recursive solution to the Tower of Hanoi.
     *
     * For detailed explanation:
     * https://en.wikipedia.org/wiki/Tower_of_Hanoi#Recursive_solution
     */

    // First, move all except bottom disk to the spare stack
    if (n > 1) {
        moveTower(n-1, from, spare, to, 0);

        QMutexLocker locker { &m_mutex };
        if (m_stopRequested) return;
    }

    // (Synchronize with UI thread)
    {
        QMutexLocker locker { &m_mutex };

        if (m_stepsRequested == 0) {
            m_condition.wait(&m_mutex);
        }
        if (m_stopRequested) return;
        --m_stepsRequested;
    }

    // Second, move the bottom disk to the target stack
    emit moveDisk(from, to);

    // Third, move all except bottom disk from spare to the target stack
    if (n > 1) {
        moveTower(n-1, spare, to, from, 1);

        QMutexLocker locker { &m_mutex };
        if (m_stopRequested) return;
    }

    emit moveTowerReturned();
}
