#ifndef TOWERSOFHANOI_H
#define TOWERSOFHANOI_H

#include <QMainWindow>

#include "tower.h"
#include "towersolver.h"

class CallStackWindow;

namespace Ui {
class TowerOfHanoi;
}

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
    void callGraphWindow();
    void done();

private:
    Tower *m_tower;
    TowerSolver *m_towerSolver;
    CallStackWindow *m_callStackWindow;
    Ui::TowerOfHanoi *ui;
};

#endif // TOWERSOFHANOI_H
