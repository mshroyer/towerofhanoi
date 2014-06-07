#include "towerofhanoi.h"
#include "ui_towerofhanoi.h"

#include "progresswindow.h"
#include "stacktracewindow.h"

#include <QMessageBox>

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
    connect(ui->playPauseButton, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(ui->singleStepButton, SIGNAL(clicked()), this, SLOT(singleStep()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), m_tower, SLOT(reset(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxChanged(int)));
    connect(ui->dial, SIGNAL(valueChanged(int)), this, SLOT(dialChanged(int)));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionProgress, SIGNAL(triggered()), this, SLOT(progressWindow()));
    connect(ui->actionStackTrace, SIGNAL(triggered()), this, SLOT(stackTraceWindow()));

    connect(m_towerSolver, &QThread::finished, this, &TowerOfHanoi::finished);
    connect(m_towerSolver, &TowerSolver::moveTowerCalled, this, &TowerOfHanoi::moveTowerCalled);
    connect(m_towerSolver, &TowerSolver::moveTowerReturned, this, &TowerOfHanoi::moveTowerReturned);
    connect(m_towerSolver, &TowerSolver::moveDisk, this, &TowerOfHanoi::moveDiskCalled);

    m_towerTimer->setSingleShot(true);
    connect(m_towerTimer, &QTimer::timeout, this, &TowerOfHanoi::step);

    spinBoxChanged(ui->spinBox->value());
    dialChanged(ui->dial->value());
}

TowerOfHanoi::~TowerOfHanoi()
{
    delete ui;
}

const QStack<StackFrame> &TowerOfHanoi::stackTrace() const
{
    return m_stackTrace;
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
    m_towerSolver->stop();
    QWidget::closeEvent(event);
}

void TowerOfHanoi::about()
{
    QMessageBox aboutBox { this };

    const QString format =
        "<p><b>About Tower of Hanoi %2</b><p>"
        "<p>This program implements a recursive solution to the Tower of Hanoi.  For more information "
        "see the Wikipedia article and source code repository linked below.</p>"
        "<p><a href='https://en.wikipedia.org/wiki/Tower_of_hanoi'>https://en.wikipedia.org/wiki/Tower_of_hanoi</a><br>"
        "<a href='https://bitbucket.org/markshroyer/towerofhanoi/'>https://bitbucket.org/markshroyer/towerofhanoi/</a></p>"
        "<p>This build was created from revision <a href='%1/commits/%3'>%4</a></p>"
        "<p>Mark Shroyer<br>"
        "5 June 2014</p>";

    QString shortRevId { REV_ID };
    shortRevId.truncate(12);
    const auto aboutText = format.arg(REPOSITORY_URL).arg(ABOUT_VERSION).arg(REV_ID).arg(shortRevId);

    aboutBox.setWindowTitle("About Tower of Hanoi");
    aboutBox.setTextFormat(Qt::RichText);
    aboutBox.setText(aboutText);
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
    m_maxMoves = (1 << value) - 1;
    emit maxMovesChanged(m_maxMoves);
}

void TowerOfHanoi::dialChanged(int value)
{
    double exponent = static_cast<double>(value) * std::log(100) / 99;
    double diskRate = std::exp(exponent);
    int iDiskRate = static_cast<int>(diskRate);

    const auto labelText = iDiskRate == 1 ? QString("1 disk/sec") : QString("%1 disks/sec").arg(iDiskRate);
    ui->diskRateLabel->setText(labelText);
    m_delay = static_cast<int>(1000 / diskRate);
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
    resetStackTrace();

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
    emit stackTraceChanged();
}

void TowerOfHanoi::moveTowerCalled(int n, TowerStack from, TowerStack to, TowerStack spare,
                                   MoveTowerRecursion recursion, void *frame)
{
    m_stackTrace.push({ n, from, to, spare, recursion, frame });
}

void TowerOfHanoi::moveTowerReturned()
{
    m_stackTrace.pop();
}

void TowerOfHanoi::moveDiskCalled(TowerStack, TowerStack)
{
    if (m_playing)
        m_towerTimer->start(m_delay);

    emit numMovesChanged(++m_numMoves);
    emit stackTraceChanged();
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

void TowerOfHanoi::resetStackTrace()
{
    m_stackTrace.clear();
    emit stackTraceChanged();
}
