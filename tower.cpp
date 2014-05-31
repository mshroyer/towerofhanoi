#include "tower.h"

Tower::Tower(int ndisks, QObject *parent) :
    QObject{ parent }
{
    reset(ndisks);
}

int Tower::ndisks(void) const
{
    return m_ndisks;
}

const QList<int> &Tower::stack(Stack name) const
{
    return m_stacks[static_cast<int>(name)];
}

QList<int> &Tower::getStack(Stack name)
{
    return const_cast<QList<int> &>(static_cast<const Tower *>(this)->stack(name));
}

void Tower::reset(int ndisks)
{
    m_ndisks = ndisks;

    auto &left = getStack(Stack::LEFT);
    left.clear();
    for (int i = ndisks; i > 0; --i)
        left.insert(0, i);

    getStack(Stack::MIDDLE).clear();
    getStack(Stack::RIGHT).clear();

    emit moved();
}

void Tower::moveDisk(Stack from, Stack to)
{
    auto &t_from = getStack(from);
    auto &t_to   = getStack(to);

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
