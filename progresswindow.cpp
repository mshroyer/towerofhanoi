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

    ui->actionClose->setShortcut(QKeySequence::Close);
    addAction(ui->actionClose);
}

ProgressWindow::~ProgressWindow()
{
    delete ui;
}

void ProgressWindow::showEvent(QShowEvent *)
{
    connect(TOWEROFHANOI, &TowerOfHanoi::maxMovesChanged, this, &ProgressWindow::setMaximum);
    connect(TOWEROFHANOI, &TowerOfHanoi::numMovesChanged, this, &ProgressWindow::setValue);

    ui->progressBar->setMaximum(TOWEROFHANOI->maxMoves());
    ui->progressBar->setValue(TOWEROFHANOI->numMoves());
    updateLabel();
}

void ProgressWindow::hideEvent(QHideEvent *)
{
    disconnect(TOWEROFHANOI, &TowerOfHanoi::maxMovesChanged, this, &ProgressWindow::setMaximum);
    disconnect(TOWEROFHANOI, &TowerOfHanoi::numMovesChanged, this, &ProgressWindow::setValue);
}

void ProgressWindow::setMaximum(int max)
{
    ui->progressBar->setMaximum(max);
    updateLabel();
}

void ProgressWindow::setValue(int max)
{
    ui->progressBar->setValue(max);
    updateLabel();
}

void ProgressWindow::updateLabel()
{
    const auto *bar = ui->progressBar;
    const auto text = QString("Moves: %1 / %2").arg(bar->value()).arg(bar->maximum());

    ui->label->setText(text);
}
