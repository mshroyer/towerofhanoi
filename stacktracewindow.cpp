#include "stacktracewindow.h"
#include "ui_stacktracewindow.h"

#include "datatypes.h"
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
const char * const kPaddingEnd = kPadding + sizeof(kPadding) - 1;

const char * const kCallFormat32 = "0x%08lX %c%s step(%d, %s, %s, %s)\n";
const char * const kCallFormat64 = "0x%016lX %c%s step(%d, %s, %s, %s)\n";

const char *towerStackName(TowerStack stack)
{
    switch (stack) {
    case TowerStack::LEFT:
        return "LEFT";
    case TowerStack::MIDDLE:
        return "MIDDLE";
    case TowerStack::RIGHT:
        return "RIGHT";
    }
    return nullptr;
}

char recursionLabel(StepRecursion recursion)
{
    switch (recursion) {
    case StepRecursion::ROOT:
        return ' ';
    case StepRecursion::LEFT:
        return '0';
    case StepRecursion::RIGHT:
        return '1';
    }
    return ' ';
}

} // namespace

extern const char * const kStepFunctionFile;
extern const int kStepFunctionLine;

StackTraceWindow::StackTraceWindow(TowerOfHanoi *parent) :
    QWidget { parent, Qt::Dialog },
    m_textbuf { new char[kBufSize], [](char *buf) { delete[] buf; } },
    ui { new Ui::StackTraceWindow }
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

    const QString labelFormat = "See <a href='%3/src/%4/towersolver.cpp#cl-%2'>%1:%2</a> for algorithm implementation";
    const QString labelText = labelFormat.arg(kStepFunctionFile).arg(kStepFunctionLine).arg(REPOSITORY_URL).arg(COMMIT_ID);
    ui->label->setText(labelText);
}

StackTraceWindow::~StackTraceWindow()
{
    delete ui;
}

void StackTraceWindow::showEvent(QShowEvent *)
{
    connect(TOWEROFHANOI, &TowerOfHanoi::stackTraceChanged, this, &StackTraceWindow::updateStackTrace);
    updateStackTrace();
}

void StackTraceWindow::hideEvent(QHideEvent *)
{
    disconnect(TOWEROFHANOI, &TowerOfHanoi::stackTraceChanged, this, &StackTraceWindow::updateStackTrace);
}

void StackTraceWindow::updateStackTrace()
{
    const char *frameFormat = sizeof(void *) > 4 ? kCallFormat64 : kCallFormat32;
    const auto &stackTrace = TOWEROFHANOI->stackTrace();
    char *buf = m_textbuf.data();
    int i = 0, line = 0, ret;

    *buf = '\0';
    for (const StackFrame call : stackTrace) {
        ret = snprintf(buf + i, kBufLineLength + 1, frameFormat, call.fp, recursionLabel(call.recursion),
                       kPaddingEnd - line, call.n, towerStackName(call.from), towerStackName(call.to),
                       towerStackName(call.spare));

        if (ret < 0) {
            qWarning("Error formatting call stack frame");
            return;
        }

        i += ret;
        line++;
    }

    ui->textEdit->setText(buf);
}
