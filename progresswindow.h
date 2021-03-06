#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include <QDialog>

namespace Ui {
class ProgressWindow;
}

class ProgressWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressWindow(QWidget *parent = 0);
    ~ProgressWindow();

protected:
    virtual void showEvent(QShowEvent *) override;
    virtual void hideEvent(QHideEvent *) override;

private slots:
    void setValue(int);
    void setMaximum(int);

private:
    void updateLabel();

    Ui::ProgressWindow *ui;
};

#endif // PROGRESSWINDOW_H
