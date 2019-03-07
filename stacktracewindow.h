#ifndef STACKTRACEWINDOW_H
#define STACKTRACEWINDOW_H

#include <QDialog>

class TowerOfHanoi;

namespace Ui {
class StackTraceWindow;
}

class StackTraceWindow : public QDialog
{
    Q_OBJECT


public:
    explicit StackTraceWindow(TowerOfHanoi *parent = nullptr);
    ~StackTraceWindow() override;

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
