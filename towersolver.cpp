#include "towersolver.h"

TowerSolver::TowerSolver(Tower *tower, QObject *parent) :
    QThread { parent },
    m_tower { tower }
{
    connect(this, &TowerSolver::moveDisk, tower, &Tower::moveDisk, Qt::QueuedConnection);
}

void TowerSolver::stop()
{
    // Don't do this in real code. And if you won't take my word for it, please
    // visit #qt on freenode so that thiago can set you straight.
    terminate();
    wait(1000);
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
     * This is a recursive solution to the Tower of Hanoi.
     *
     * For detailed explanation:
     * https://en.wikipedia.org/wiki/Tower_of_Hanoi#Recursive_solution
     */

    _moveTowerCalled(n, from, to, spare, recursion, __builtin_frame_address(0));

    // First, move all except bottom disk to the spare stack
    if (n > 1) {
        moveTower(n-1, from, spare, to, MoveTowerRecursion::LEFT);
    }

    // Second, move the bottom disk to the target stack
    _moveDisk(from, to);
    QThread::msleep(350);

    // Third, move all except bottom disk from spare to the target stack
    if (n > 1) {
        moveTower(n-1, spare, to, from, MoveTowerRecursion::RIGHT);
    }

    _moveTowerReturned();
}


/*
 * The following are wrappers to prevent corrupting Qt data structures due to
 * thread termination while queueing signals for activation in the UI thread.
 * We're using QThread::terminate() to stop the solver rather than voluntary
 * termination so that we can keep the moveTower() function relatively free of
 * ancillary logic; this program is for illustrative purposes.
 */

void TowerSolver::_moveTowerCalled(int n, TowerStack from, TowerStack to, TowerStack spare,
                                        MoveTowerRecursion recursion, void *frame)
{
    setTerminationEnabled(false);
    emit moveTowerCalled(n, from, to, spare, recursion, frame);
    setTerminationEnabled(true);
}

void TowerSolver::_moveTowerReturned()
{
    setTerminationEnabled(false);
    emit moveTowerReturned();
    setTerminationEnabled(true);
}

void TowerSolver::_moveDisk(TowerStack from, TowerStack to)
{
    setTerminationEnabled(false);
    emit moveDisk(from, to);
    setTerminationEnabled(true);
}
