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
    void stackTraceChanged();

public slots:
    void about();
    void playPause();
    void singleStep();
    void reset();

public:
    const QStack<StackFrame> &stackTrace() const;
    int maxMoves() const;
    int numMoves() const;

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
    void progressWindow();
    void stackTraceWindow();
    void spinBoxChanged(int value);
    void dialChanged(int value);
    void moveTowerCalled(int n, TowerStack from, TowerStack to, TowerStack spare,
                         MoveTowerRecursion recursion, void *frame);
    void moveTowerReturned();
    void moveDiskCalled(TowerStack from, TowerStack to);
    void step();
    void done();

private:
    void numMovesReset();
    void stackTraceReset();

    int m_delay = 0;
    bool m_playing = false;
    int m_maxMoves = 0;
    int m_numMoves = 0;
    QStack<StackFrame> m_stackTrace;
    Tower *m_tower;
    TowerSolver *m_towerSolver;
    QTimer *m_towerTimer;
    ProgressWindow *m_progressWindow = nullptr;
    StackTraceWindow *m_stackTraceWindow = nullptr;
    Ui::TowerOfHanoi *ui;
};

#endif // TOWERSOFHANOI_H
