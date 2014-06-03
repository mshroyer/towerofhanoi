#ifndef STACKTRACEWINDOW_H
#define STACKTRACEWINDOW_H

#include <QWidget>

class TowerOfHanoi;

namespace Ui {
class StackTraceWindow;
}

class StackTraceWindow : public QWidget
{
    Q_OBJECT


public:
    explicit StackTraceWindow(TowerOfHanoi *parent = nullptr);
    ~StackTraceWindow();

protected:
    virtual void showEvent(QShowEvent *) override;
    virtual void hideEvent(QHideEvent *) override;

private slots:
    void updateStackTrace();

private:
    QSharedPointer<char> m_textbuf;
    Ui::StackTraceWindow *ui;
};

#endif // STACKTRACEWINDOW_H
