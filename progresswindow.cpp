#include "progresswindow.h"
#include "ui_progresswindow.h"

#include "datatypes.h"
#include "towerofhanoi.h"

#define TOWEROFHANOI qobject_cast<TowerOfHanoi *>(parent())

ProgressWindow::ProgressWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressWindow)
{
    ui->setupUi(this);
}

ProgressWindow::~ProgressWindow()
{
    delete ui;
}

void ProgressWindow::showEvent(QShowEvent *)
{
    connect(TOWEROFHANOI, &TowerOfHanoi::numMovesChanged, ui->progressBar, &QProgressBar::setValue);
}

void ProgressWindow::hideEvent(QHideEvent *)
{
    disconnect(TOWEROFHANOI, &TowerOfHanoi::numMovesChanged, ui->progressBar, &QProgressBar::setValue);
}
