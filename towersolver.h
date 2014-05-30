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

public slots:

protected:
    virtual void run() override;

private:
    void step(int n, Tower::Stack from, Tower::Stack spare, Tower::Stack to);

    Tower *m_tower;
};

#endif // TOWERSOLVER_H
