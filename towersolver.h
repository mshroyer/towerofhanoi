#ifndef TOWERSOLVER_H
#define TOWERSOLVER_H

#include <QThread>

#include "tower.h"

class TowerSolver : public QThread
{
    Q_OBJECT

public:
    explicit TowerSolver(Tower *tower, QObject *parent = nullptr);

signals:
    void stepCall(int n, Tower::Stack from, Tower::Stack spare, Tower::Stack to);
    void stepReturn();
    void moveDisk(Tower::Stack from, Tower::Stack to);

public slots:

protected:
    virtual void run() override;

private:
    void step(int n, Tower::Stack from, Tower::Stack to, Tower::Stack spare, bool rightmost = true);

    Tower *m_tower;
};

#endif // TOWERSOLVER_H
