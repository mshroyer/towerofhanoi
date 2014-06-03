#ifndef TOWERSOFHANOI_H
#define TOWERSOFHANOI_H

#include <QMainWindow>
#include <QStack>

#include "datatypes.h"
#include "tower.h"
#include "towersolver.h"

class ProgressWindow;
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
    void numMovesChanged(int);
    void stackTraceChanged();

public slots:
    void about();
    void pushButton();

public:
    const QStack<StackFrame> &stackTrace() const;

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
    void progressWindow();
    void stackTraceWindow();
    void moveTowerCalled(int n, TowerStack from, TowerStack to, TowerStack spare,
                         StepRecursion recursion, void *frame);
    void moveTowerReturned();
    void moveDiskCalled(TowerStack from, TowerStack to);
    void done();

private:
    void numMovesReset();
    void stackTraceReset();

    int m_numMoves = 0;
    QStack<StackFrame> m_stackTrace;
    Tower *m_tower;
    TowerSolver *m_towerSolver = nullptr;
    ProgressWindow *m_progressWindow = nullptr;
    StackTraceWindow *m_stackTraceWindow = nullptr;
    Ui::TowerOfHanoi *ui;
};

#endif // TOWERSOFHANOI_H
