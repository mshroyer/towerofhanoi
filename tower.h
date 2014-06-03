#ifndef TOWERS_H
#define TOWERS_H

#include <QObject>

#include "datatypes.h"

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
    int ndisks(void) const;
    const QList<int> &stack(TowerStack name) const;

private:
    QList<int> &getStack(TowerStack name);
    int m_ndisks;
    QList<int> m_stacks[3];
};

#endif // TOWERS_H
