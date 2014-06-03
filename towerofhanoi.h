#ifndef TOWERSOFHANOI_H
#define TOWERSOFHANOI_H

#include <QMainWindow>
#include <QStack>

#include "datatypes.h"
#include "tower.h"
#include "towersolver.h"

class StackTraceWindow;

namespace Ui {
class TowerOfHanoi;
}

class TowerOfHanoi : public QMainWindow
{
    Q_OBJECT

public:
    explicit TowerOfHanoi(QWidget *parent = nullptr);
    ~TowerOfHanoi();

signals:
    void callStackChanged();

public slots:
    void about();
    void pushButton();

public:
    const QStack<StackFrame> &callStack() const;

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
    void callStackWindow();
    void stepCall(int n, TowerStack from, TowerStack to, TowerStack spare, StepRecursion recursion, void *frame);
    void stepReturn();
    void moveDisk(TowerStack from, TowerStack to);
    void done();

private:
    void callStackReset();

    QStack<StackFrame> m_callStack;
    Tower *m_tower;
    TowerSolver *m_towerSolver = nullptr;
    StackTraceWindow *m_callStackWindow = nullptr;
    Ui::TowerOfHanoi *ui;
};

#endif // TOWERSOFHANOI_H
