#include "tower.h"

#include <QReadLocker>
#include <QWriteLocker>

QStack<int> &TowerState::stack(TowerStack name)
{
    return stacks[static_cast<int>(name)];
}

TowerStack TowerState::stackContainingDisk(int n)
{
    for (int i = 0; i < 3; ++i) {
        const auto &stack = stacks[i];
        for (int j = stack.size() - 1; j >= 0; --j) {
            int disk = stack[j];

            if (disk == n)
                return static_cast<TowerStack>(i);

            if (disk > n)
                break;
        }
    }

    return TowerStack::LEFT;
}

Tower::Tower(int ndisks, QObject *parent) :
    QObject { parent },
    m_lock { QReadWriteLock::Recursive }
{
    reset(ndisks);
}

void Tower::reset()
{
    QWriteLocker locker { &m_lock };

    auto &left = m_state.stack(TowerStack::LEFT);
    left.clear();
    for (int i = m_state.ndisks; i > 0; --i)
        left.push(i);

    m_state.stack(TowerStack::MIDDLE).clear();
    m_state.stack(TowerStack::RIGHT).clear();

    emit moved();
}

void Tower::reset(int ndisks)
{
    QWriteLocker locker { &m_lock };

    m_state.ndisks = ndisks;
    reset();
}

void Tower::moveDisk(TowerStack from, TowerStack to)
{
    QWriteLocker locker { &m_lock };

    auto &t_from = m_state.stack(from);
    auto &t_to   = m_state.stack(to);

    if (t_from.isEmpty()) {
        emit moveError("Cannot move from empty tower");
        return;
    }

    if (!t_to.isEmpty() && t_to.top() < t_from.top()) {
        emit moveError("Cannot move onto a smaller disk");
        return;
    }

    auto disk = t_from.pop();
    t_to.push(disk);

    emit moved();
}

TowerState Tower::state() const
{
    QReadLocker locker { &m_lock };

    // Safe even though QStack<int> is implicitly shared
    return m_state;
}
