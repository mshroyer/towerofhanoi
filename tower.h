#ifndef TOWERS_H
#define TOWERS_H

#include <QObject>
#include <QStack>
#include <QReadWriteLock>

#include "datatypes.h"

// Maximum number of disks supported
constexpr int kMaxDisks = 16;

class Tower;

class TowerState
{
public:
    friend class Tower;

    const QStack<int> &stack(TowerStack name) const;
    QStack<int> &stack(TowerStack name);
    TowerStack findDisk(int n) const;
    int ndisks() const;
    int movesNeeded() const;

private:
    QStack<int> m_stacks[3];
    TowerStack m_diskStacks[kMaxDisks];
    int m_ndisks;
};

class Tower : public QObject
{
    Q_OBJECT

public:
    explicit Tower(int ndisks = 6, QObject *parent = nullptr);

signals:
    void moved(void);
    void moveError(QString errmsg);

public slots:
    void reset();
    void reset(int ndisks);
    void moveDisk(TowerStack from, TowerStack to);

public:
    TowerState state() const;

private:
    mutable QReadWriteLock m_lock;
    TowerState m_state;
};

#endif // TOWERS_H
