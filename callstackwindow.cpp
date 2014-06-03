#include "callstackwindow.h"
#include "ui_callstackwindow.h"

#include "towerofhanoi.h"

#include <QDebug>
#include <cstdint>

#define TOWEROFHANOI qobject_cast<TowerOfHanoi *>(parent())

namespace {

const char *stackName(Tower::Stack stack)
{
    switch (stack) {
    case Tower::Stack::LEFT:
        return "LEFT";
    case Tower::Stack::MIDDLE:
        return "MIDDLE";
    case Tower::Stack::RIGHT:
        return "RIGHT";
    }

    return nullptr;
}

} // namespace

CallStackWindow::CallStackWindow(TowerOfHanoi *parent) :
    QWidget { parent, Qt::Window },
    ui { new Ui::CallStackWindow }
{
    ui->setupUi(this);

    QFont font { "Monospace" };
    font.setStyleHint(QFont::TypeWriter);
    ui->textEdit->setFont(font);
}

CallStackWindow::~CallStackWindow()
{
    delete ui;
}

void CallStackWindow::showEvent(QShowEvent *)
{
    connect(TOWEROFHANOI, &TowerOfHanoi::callStackChanged, this, &CallStackWindow::updateCallStack);
    updateCallStack();
}

void CallStackWindow::hideEvent(QHideEvent *)
{
    disconnect(TOWEROFHANOI, &TowerOfHanoi::callStackChanged, this, &CallStackWindow::updateCallStack);
}

void CallStackWindow::updateCallStack()
{
    QString callStackText;

    const QStack<StepCall> &callStack = TOWEROFHANOI->callStack();
    for (const StepCall call : callStack) {
        callStackText += QString("0x%5   step(%1, %2, %3, %4)\n").arg(call.n, 2).arg(stackName(call.from), 6).arg(stackName(call.to), 6).arg(stackName(call.spare), 6).arg(reinterpret_cast<uintptr_t>(call.frame), 2*sizeof(call.frame), 16, QChar { '0' });
    }

    ui->textEdit->setText(callStackText);
}
