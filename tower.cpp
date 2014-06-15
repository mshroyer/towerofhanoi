#include "tower.h"

#include <QReadLocker>
#include <QWriteLocker>

QVector<int> &TowerState::stack(TowerStack name)
{
    return stacks[static_cast<int>(name)];
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
        left.insert(0, i);

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

    if (!t_to.isEmpty() && t_to.first() < t_from.first()) {
        emit moveError("Cannot move onto a smaller disk");
        return;
    }

    auto disk = t_from.first();
    t_from.removeFirst();
    t_to.insert(0, disk);

    emit moved();
}

TowerState Tower::state() const
{
    QReadLocker locker { &m_lock };
    return m_state;
}
