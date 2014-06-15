#ifndef TOWERS_H
#define TOWERS_H

#include <QObject>
#include <QVector>
#include <QReadWriteLock>

#include "datatypes.h"

struct TowerState
{
    QVector<int> &stack(TowerStack name);

    QVector<int> stacks[3];
    int ndisks;
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
