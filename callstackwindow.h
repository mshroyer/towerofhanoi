#ifndef CALLSTACKWINDOW_H
#define CALLSTACKWINDOW_H

#include <QWidget>

namespace Ui {
class CallStackWindow;
}

class CallStackWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CallStackWindow(QWidget *parent = 0);
    ~CallStackWindow();

private:
    Ui::CallStackWindow *ui;
};

#endif // CALLSTACKWINDOW_H
