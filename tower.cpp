#include "tower.h"

#include <QReadLocker>
#include <QWriteLocker>

const QStack<int> &TowerState::stack(TowerStack name) const
{
    return m_stacks[static_cast<int>(name)];
}

QStack<int> &TowerState::stack(TowerStack name)
{
    return const_cast<QStack<int> &>(static_cast<const TowerState *>(this)->stack(name));
}

TowerStack TowerState::findDisk(int n) const
{
    return m_diskStacks[n-1];
}

int TowerState::ndisks() const
{
    return m_ndisks;
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
    for (int i = m_state.m_ndisks; i > 0; --i) {
        m_state.m_diskStacks[i-1] = TowerStack::LEFT;
        left.push(i);
    }

    m_state.stack(TowerStack::MIDDLE).clear();
    m_state.stack(TowerStack::RIGHT).clear();

    emit moved();
}

void Tower::reset(int ndisks)
{
    QWriteLocker locker { &m_lock };

    m_state.m_ndisks = ndisks;
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
    m_state.m_diskStacks[disk-1] = to;

    emit moved();
}

TowerState Tower::state() const
{
    QReadLocker locker { &m_lock };

    // Safe even though QStack<int> is implicitly shared
    return m_state;
}
