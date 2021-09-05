#include "stacktracewindow.h"
#include "ui_stacktracewindow.h"

#include "datatypes.h"
#include "towerofhanoi.h"

#include <QDebug>

#include <cstdint>
#include <cstdio>

#define IS32BIT (sizeof(void *) == 4)
#define TOWEROFHANOI qobject_cast<TowerOfHanoi *>(parent())

namespace {

constexpr size_t kBufLineLength = 96;
constexpr size_t kBufNumLines = 2 + 16;
constexpr size_t kBufSize = kBufLineLength * kBufNumLines + 1;

const char kPadding[] = "                ";
const char * const kPaddingEnd = kPadding + sizeof(kPadding) - 1;

const char kCallHeader32[] = "Frame address  Sub  Call graph\n"
                             "--------------------------------------------------------------------\n";
const char kCallHeader64[] = "Frame address     Sub  Call graph\n"
                             "-----------------------------------------------------------------------\n";

const char * const kCallHeader = IS32BIT ? kCallHeader32 : kCallHeader64;
const size_t kCallHeaderSize = IS32BIT ? sizeof(kCallHeader32) : sizeof(kCallHeader64);

const char * const kCallFormat32 = "%08lX       %c%s    moveTower(%d, %s, %s, %s)\n";
const char * const kCallFormat64 = "%016lX  %c%s    moveTower(%d, %s, %s, %s)\n";

const char * const kCallFormat = IS32BIT ? kCallFormat32 : kCallFormat64;

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

char subLabel(int sub)
{
    switch (sub) {
    case 0:
        return '0';
    case 1:
        return '1';
    default:
        return ' ';
    }
}

} // namespace

extern const char * const kMoveTowerFile;
extern const int kMoveTowerLine;

StackTraceWindow::StackTraceWindow(TowerOfHanoi *parent) :
    QDialog { parent },
    m_textbuf { new char[kBufSize], [](char *buf) { delete[] buf; } },
    ui { new Ui::StackTraceWindow }
{
    ui->setupUi(this);

    ui->actionClose->setShortcut(QKeySequence::Close);
    addAction(ui->actionClose);

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

    const QString labelFormat = "See <a href='%3/blob/%4/%1#L%2'>%1:%2</a> for algorithm implementation";
    const QString labelText = labelFormat.arg(kMoveTowerFile).arg(kMoveTowerLine).arg(REPOSITORY_URL).arg(REV_ID);
    ui->label->setText(labelText);

    strncpy(m_textbuf.data(), kCallHeader, kBufSize);
}

StackTraceWindow::~StackTraceWindow()
{
    delete ui;
}

void StackTraceWindow::showEvent(QShowEvent *)
{
    connect(TOWEROFHANOI, &TowerOfHanoi::stackChanged, this, &StackTraceWindow::updateStackTrace);
    updateStackTrace();
}

void StackTraceWindow::hideEvent(QHideEvent *)
{
    disconnect(TOWEROFHANOI, &TowerOfHanoi::stackChanged, this, &StackTraceWindow::updateStackTrace);
}

void StackTraceWindow::updateStackTrace()
{
    const auto &stack = TOWEROFHANOI->stack();
    char *buf = m_textbuf.data() + kCallHeaderSize - 1;
    int i = 0, line = 0, ret;

    *buf = '\0';
    for (const StackFrame call : stack) {
        ret = snprintf(buf + i, kBufLineLength + 1, kCallFormat, reinterpret_cast<uintptr_t>(call.fp),
                       subLabel(call.sub), kPaddingEnd - line, call.n,
                       towerStackName(call.from), towerStackName(call.to), towerStackName(call.spare));

        if (ret < 0) {
            qCritical("Error formatting call stack frame");
            return;
        }

        i += ret;
        ++line;
    }

    ui->textEdit->setText(m_textbuf.data());
}
