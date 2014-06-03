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

const char kPadding[] = "                ";
const char *kPaddingEnd = kPadding + sizeof(kPadding) - 1;

const char *kCallFormat32 = "0x%08lX%s  step(%d, %s, %s, %s)\n";
const char *kCallFormat64 = "0x%016lX%s  step(%d, %s, %s, %s)\n";

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

#if defined Q_OS_MAC
    QFont font { "Monaco" };
    font.setPointSize(12);
#elif defined Q_OS_WIN
    QFont font { "Consolas" };
    font.setPointSize(10);
#else
    QFont font { "Monospace" };
    font.setPointSize(10);
#endif
    font.setStyleHint(QFont::Monospace);
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
    const char *callFormat = sizeof(void *) > 4 ? kCallFormat64 : kCallFormat32;
    const auto &callStack = TOWEROFHANOI->callStack();
    char *buf = m_textbuf.data();
    int i = 0, line = 0, ret;

    *buf = '\0';
    for (const StepCall call : callStack) {
        ret = snprintf(buf + i, kBufLineLength + 1, callFormat, call.frame, kPaddingEnd - line,
                       call.n, stackName(call.from), stackName(call.to), stackName(call.spare));
        if (ret < 0) {
            qWarning("Error formatting call stack frame");
            return;
        }

        i += ret;
        line++;
    }

    ui->textEdit->setText(buf);
}
