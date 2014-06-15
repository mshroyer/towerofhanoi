#include "towerofhanoi.h"
#include "ui_towerofhanoi.h"

#include "progresswindow.h"
#include "stacktracewindow.h"

#include <QDialog>
#include <QDialogButtonBox>

#include <cmath>

TowerOfHanoi::TowerOfHanoi(QWidget *parent) :
    QMainWindow { parent },
    m_tower { new Tower { 6, this } },
    m_towerSolver { new TowerSolver { m_tower, this } },
    m_towerTimer { new QTimer { this } },
    ui { new Ui::TowerOfHanoi }
{
    ui->setupUi(this);
    ui->towerView->setTower(m_tower);

    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);

    connect(m_towerSolver, &QThread::finished, this, &TowerOfHanoi::finished);
    connect(m_towerSolver, &TowerSolver::moveTowerCalled, this, &TowerOfHanoi::moveTowerCalled);
    connect(m_towerSolver, &TowerSolver::moveTowerReturned, this, &TowerOfHanoi::moveTowerReturned);
    connect(m_tower, &Tower::moved, this, &TowerOfHanoi::moveCalled);

    m_towerTimer->setSingleShot(true);
    connect(m_towerTimer, &QTimer::timeout, this, &TowerOfHanoi::step);

    spinBoxChanged(ui->spinBox->value());
    dialChanged(ui->dial->value());
}

TowerOfHanoi::~TowerOfHanoi()
{
    delete ui;
}

const QStack<StackFrame> &TowerOfHanoi::stack() const
{
    return m_stack;
}

int TowerOfHanoi::maxMoves() const
{
    return m_maxMoves;
}

int TowerOfHanoi::numMoves() const
{
    return m_numMoves;
}

void TowerOfHanoi::closeEvent(QCloseEvent *event)
{
    m_towerTimer->stop();
    m_towerSolver->stop();
    QWidget::closeEvent(event);
}

void TowerOfHanoi::about()
{
    QDialog aboutBox { this };

    const QString format =
        "<p><b>About Tower of Hanoi %2</b><p>"
        "<p>This program illustrates a recursive solution to the Tower of Hanoi.  For more information "
        "see the Wikipedia article and source code repository linked below.</p>"
        "<p><a href='https://en.wikipedia.org/wiki/Tower_of_hanoi'>https://en.wikipedia.org/wiki/Tower_of_hanoi</a><br>"
        "<a href='https://bitbucket.org/markshroyer/towerofhanoi/'>https://bitbucket.org/markshroyer/towerofhanoi/</a></p>"
        "<p>This build was created from revision <a href='%1/commits/%3'>%4</a>.</p>"

        "<p><br>Copyright (c) 2014 Mark Shroyer &lt;code@markshroyer.com&gt;</p>"

        "<p>Permission to use, copy, modify, and/or distribute this software for any "
        "purpose with or without fee is hereby granted, provided that the above "
        "copyright notice and this permission notice appear in all copies.</p>"

        "<p>THE SOFTWARE IS PROVIDED &quot;AS IS&quot; AND THE AUTHOR DISCLAIMS ALL WARRANTIES "
        "WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF "
        "MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY "
        "SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES "
        "WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN "
        "ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR "
        "IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.</p>";

    QString shortRevId { REV_ID };
    shortRevId.truncate(12);
    const auto aboutText = format.arg(REPOSITORY_URL).arg(ABOUT_VERSION).arg(REV_ID).arg(shortRevId);

    auto *label = new QLabel { aboutText };
    label->setTextFormat(Qt::RichText);
    label->setWordWrap(true);
    label->setOpenExternalLinks(true);

    auto *buttonBox = new QDialogButtonBox { QDialogButtonBox::Ok, Qt::Horizontal };
    connect(buttonBox, &QDialogButtonBox::accepted, &aboutBox, &QDialog::accept);

    auto *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(buttonBox);

    aboutBox.setLayout(layout);
    aboutBox.setWindowTitle("About Tower of Hanoi");
    aboutBox.exec();
}

void TowerOfHanoi::progressWindow()
{
    if (!m_progressWindow) {
        m_progressWindow = new ProgressWindow { this };
        m_progressWindow->move(x()+60, y()+60);
    }
    m_progressWindow->show();
    m_progressWindow->raise();
}

void TowerOfHanoi::stackTraceWindow()
{
    if (!m_stackTraceWindow) {
        m_stackTraceWindow = new StackTraceWindow { this };
        m_stackTraceWindow->move(x()+120, y()+120);
    }
    m_stackTraceWindow->show();
    m_stackTraceWindow->raise();
}

void TowerOfHanoi::spinBoxChanged(int value)
{
    m_tower->reset(value);

    m_maxMoves = (1 << value) - 1;
    emit maxMovesChanged(m_maxMoves);
}

void TowerOfHanoi::dialChanged(int value)
{
    const double diskRate = std::exp(static_cast<double>(value) * std::log(100) / 99);
    const int intRate = static_cast<int>(diskRate);
    const auto text = intRate == 1 ? QString("1 move / s") : QString("%1 moves / s").arg(intRate);

    m_delay = static_cast<int>(1000 / diskRate);
    emit diskRateChanged(text);
}

void TowerOfHanoi::playPause()
{
    if (m_playing) {
        // Pause
        setPlaying(false);
    } else {
        // Play
        setPlaying(true);
        singleStep();
    }
}

void TowerOfHanoi::singleStep()
{
    if (!m_towerSolver->isRunning()) {
        m_towerSolver->start();
    }
    ui->spinBox->setEnabled(false);
    ui->resetButton->setEnabled(true);
    step();
}

void TowerOfHanoi::reset()
{
    setPlaying(false);

    m_towerSolver->stop();
    m_tower->reset();
    resetNumMoves();
    resetStack();

    ui->spinBox->setEnabled(true);
    ui->playPauseButton->setEnabled(true);
    ui->singleStepButton->setEnabled(true);
    ui->resetButton->setEnabled(false);
}

void TowerOfHanoi::step()
{
    // Allow one more step to be sent beyond the maximum number of moves so
    // that we can also step past the wait condition at the end of
    // TowerSolver::run()
    if (m_numSteps > m_maxMoves)
        return;

    if (m_numSteps == m_maxMoves) {
        setPlaying(false);
        ui->playPauseButton->setEnabled(false);
        ui->singleStepButton->setEnabled(false);
    }
    m_numSteps++;
    m_towerSolver->step();
}

void TowerOfHanoi::finished()
{
    m_stack = m_stackNext;
    emit stackChanged();
}

void TowerOfHanoi::moveTowerCalled(int n, TowerStack to, int sub, void *frame)
{
    m_stackNext.push({ n, to, sub, frame });
}

void TowerOfHanoi::moveTowerReturned()
{
    m_stackNext.pop();
}

void TowerOfHanoi::moveCalled()
{
    if (m_playing)
        m_towerTimer->start(m_delay);

    ++m_numMoves;
    m_stack = m_stackNext;

    emit numMovesChanged(m_numMoves);
    emit stackChanged();
}

void TowerOfHanoi::setPlaying(bool playing)
{
    m_playing = playing;
    ui->singleStepButton->setEnabled(!playing);

    if (playing) {
        ui->playPauseButton->setIcon(QIcon { ":/icons/pause.svg" });
        ui->playPauseButton->setToolTip("Pause");
    } else {
        ui->playPauseButton->setIcon(QIcon { ":/icons/play.svg" });
        ui->playPauseButton->setToolTip("Solve");
        m_towerTimer->stop();
    }
}

void TowerOfHanoi::resetNumMoves()
{
    m_numMoves = 0;
    m_numSteps = 0;
    emit numMovesChanged(0);
}

void TowerOfHanoi::resetStack()
{
    m_stackNext.clear();
    m_stack.clear();
    emit stackChanged();
}
