#ifndef TOWERSOFHANOI_H
#define TOWERSOFHANOI_H

#include <QMainWindow>
#include <QStack>

#include "tower.h"
#include "towersolver.h"

class CallStackWindow;

namespace Ui {
class TowerOfHanoi;
}

struct StepCall
{
    int n;
    Tower::Stack from;
    Tower::Stack spare;
    Tower::Stack to;
};

class TowerOfHanoi : public QMainWindow
{
    Q_OBJECT

public:
    explicit TowerOfHanoi(QWidget *parent = nullptr);
    ~TowerOfHanoi();

public slots:
    void about();
    void pushButton();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
    void callStackWindow();
    void done();

private:
    //QList<StepCall> m_callStack;
    Tower *m_tower;
    TowerSolver *m_towerSolver;
    CallStackWindow *m_callStackWindow;
    Ui::TowerOfHanoi *ui;
    //int m_foo;
};

#endif // TOWERSOFHANOI_H
