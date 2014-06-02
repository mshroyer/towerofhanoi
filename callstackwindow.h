#ifndef CALLSTACKWINDOW_H
#define CALLSTACKWINDOW_H

#include <QWidget>

class TowerOfHanoi;

namespace Ui {
class CallStackWindow;
}

class CallStackWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CallStackWindow(TowerOfHanoi *parent = nullptr);
    ~CallStackWindow();

private slots:
    void updateCallStack();

private:
    Ui::CallStackWindow *ui;
};

#endif // CALLSTACKWINDOW_H
