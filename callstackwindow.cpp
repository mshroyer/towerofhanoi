#include "callstackwindow.h"
#include "ui_callstackwindow.h"

#include "towerofhanoi.h"

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
    connect(parent, &TowerOfHanoi::callStackChanged, this, &CallStackWindow::updateCallStack);
    ui->setupUi(this);
}

CallStackWindow::~CallStackWindow()
{
    delete ui;
}

void CallStackWindow::updateCallStack()
{
    QString callStackText;

    const QStack<StepCall> &callStack = qobject_cast<TowerOfHanoi *>(parent())->callStack();
    for (const StepCall call : callStack) {
        callStackText += QString("step(%1, %2, %3, %4)\n").arg(call.n).arg(stackName(call.from)).arg(stackName(call.spare)).arg(stackName(call.to));
    }

    ui->textEdit->setText(callStackText);
}
