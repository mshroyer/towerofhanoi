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
                            MoveTowerRecursion recursion)
{
    /*
     * This is a recursive solution to the Tower of Hanoi. For explanation:
     * https://en.wikipedia.org/wiki/Tower_of_Hanoi#Recursive_solution
     */

    signalMoveTowerCalled(n, from, to, spare, recursion, __builtin_frame_address(0));

    // First, move all except bottom disk to the spare stack
    if (n > 1) {
        moveTower(n-1, from, spare, to, MoveTowerRecursion::LEFT);
    }

    // Second, move the bottom disk to the target stack
    signalMoveDisk(from, to);
    QThread::msleep(350);

    // Third, move all except bottom disk from spare to the target stack
    if (n > 1) {
        moveTower(n-1, spare, to, from, MoveTowerRecursion::RIGHT);
    }

    signalMoveTowerReturned();
}


/*
 * The following are wrappers to prevent thread termination during queued
 * activation. We're using QThread::terminate() rather than voluntary
 * termination via a semaphore in order to keep the moveTower() function
 * relatively simple, since this program is for illustrative purposes after
 * all...
 */

void TowerSolver::signalMoveTowerCalled(int n, TowerStack from, TowerStack to, TowerStack spare,
                                        MoveTowerRecursion recursion, void *frame)
{
    setTerminationEnabled(false);
    emit moveTowerCalled(n, from, to, spare, recursion, frame);
    setTerminationEnabled(true);
}

void TowerSolver::signalMoveTowerReturned()
{
    setTerminationEnabled(false);
    emit moveTowerReturned();
    setTerminationEnabled(true);
}

void TowerSolver::signalMoveDisk(TowerStack from, TowerStack to)
{
    setTerminationEnabled(false);
    emit moveDisk(from, to);
    setTerminationEnabled(true);
}
