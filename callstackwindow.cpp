#include "callstackwindow.h"
#include "ui_callstackwindow.h"

#include "towerofhanoi.h"

#include <QDebug>
#include <cstdint>
#include <cstdio>

#define TOWEROFHANOI qobject_cast<TowerOfHanoi *>(parent())

namespace {

constexpr size_t kBufLineLength = 96;
constexpr size_t kBufNumLines = 16;
constexpr size_t kBufSize = kBufLineLength * kBufNumLines + 1;

const char kSpaces[] = "                ";
const char *kSpacesEnd = kSpaces + sizeof(kSpaces) - 1;

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
    m_textbuf { new char[kBufSize], [](char *buf) { delete[] buf; } },
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
    const auto &callStack = TOWEROFHANOI->callStack();
    int i = 0, line = 0, ret;

    *m_textbuf.data() = '\0';
    for (const StepCall call : callStack) {
        ret = snprintf(m_textbuf.data() + i, kBufLineLength + 1, "%p%s  step(%d, %s, %s, %s)\n",
                       call.frame, kSpacesEnd - line++, call.n,
                       stackName(call.from), stackName(call.to), stackName(call.spare));
        if (ret < 0) {
            qWarning("Error formatting call stack frame");
            return;
        }
        i += ret;
    }

    ui->textEdit->setText(m_textbuf.data());
}
