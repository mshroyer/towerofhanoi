#ifndef TOWERSOFHANOI_H
#define TOWERSOFHANOI_H

#include <QMainWindow>
#include <QStack>
#include <QTimer>

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
    void maxMovesChanged(int);
    void numMovesChanged(int);
    void stackChanged();
    void diskRateChanged(QString);

public:
    const QStack<StackFrame> &stack() const;
    int maxMoves() const;
    int numMoves() const;

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
    void about();
    void progressWindow();
    void stackTraceWindow();

    void spinBoxChanged(int value);
    void dialChanged(int value);
    void playPause();
    void singleStep();
    void reset();
    void step();
    void finished();

    void moveTowerCalled(int n, TowerStack from, TowerStack to, TowerStack spare,
                         MoveTowerRecursion recursion, void *frame);
    void moveTowerReturned();
    void moveDiskCalled(TowerStack from, TowerStack to);

private:
    void setPlaying(bool playing);
    void resetNumMoves();
    void resetStack();

    int m_delay = 0;
    bool m_playing = false;
    int m_maxMoves = 0;
    int m_numMoves = 0;
    int m_numSteps = 0;

    Tower *m_tower;
    QStack<StackFrame> m_stackNext;
    QStack<StackFrame> m_stack;
    TowerSolver *m_towerSolver;
    QTimer *m_towerTimer;

    ProgressWindow *m_progressWindow = nullptr;
    StackTraceWindow *m_stackTraceWindow = nullptr;
    Ui::TowerOfHanoi *ui;
};

#endif // TOWERSOFHANOI_H
