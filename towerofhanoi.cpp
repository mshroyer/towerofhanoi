#include "towerofhanoi.h"
#include "ui_towerofhanoi.h"

#include "stacktracewindow.h"

#include <QMessageBox>

TowerOfHanoi::TowerOfHanoi(QWidget *parent) :
    QMainWindow { parent },
    m_tower { new Tower { 6, this } },
    ui { new Ui::TowerOfHanoi }
{
    ui->setupUi(this);
    ui->towerView->setTower(m_tower);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButton()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), m_tower, SLOT(reset(int)));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionStackTrace, SIGNAL(triggered()), this, SLOT(stackTraceWindow()));
}

TowerOfHanoi::~TowerOfHanoi()
{
    delete m_towerSolver;
    delete ui;
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
        "31 May 2014</p>";

    QString shortCommitId { COMMIT_ID };
    shortCommitId.truncate(12);
    const auto aboutText = format.arg(REPOSITORY_URL).arg(VERSION).arg(COMMIT_ID).arg(shortCommitId);

    aboutBox.setWindowTitle("About Tower of Hanoi");
    aboutBox.setTextFormat(Qt::RichText);
    aboutBox.setText(aboutText);
    aboutBox.exec();
}

void TowerOfHanoi::pushButton()
{
    if (m_towerSolver && m_towerSolver->isRunning()) {
        // Stop

        m_towerSolver->terminate();
        ui->pushButton->setText("Reset");
    } else if (m_towerSolver) {
        // Reset

        delete m_towerSolver;
        m_towerSolver = nullptr;
        m_tower->reset();
        ui->pushButton->setText("Solve");
        ui->spinBox->setEnabled(true);
        stackTraceReset();
    } else {
        // Start

        m_towerSolver = new TowerSolver { m_tower };
        connect(m_towerSolver, &QThread::finished, this, &TowerOfHanoi::done);
        connect(m_towerSolver, &TowerSolver::stepCall, this, &TowerOfHanoi::stepCall);
        connect(m_towerSolver, &TowerSolver::stepReturn, this, &TowerOfHanoi::stepReturn);
        connect(m_towerSolver, &TowerSolver::moveDisk, this, &TowerOfHanoi::moveDisk);
        ui->pushButton->setText("Stop");
        ui->spinBox->setEnabled(false);
        m_towerSolver->start();
    }
}

const QStack<StackFrame> &TowerOfHanoi::stackTrace() const
{
    return m_stackTrace;
}

void TowerOfHanoi::closeEvent(QCloseEvent *event)
{
    if (m_towerSolver && m_towerSolver->isRunning()) {
        m_towerSolver->terminate();
        m_towerSolver->wait(1000);
    }

    QWidget::closeEvent(event);
}

void TowerOfHanoi::stackTraceWindow()
{
    if (!m_stackTraceWindow) {
        m_stackTraceWindow = new StackTraceWindow { this };
        m_stackTraceWindow->move(x()+70, y()+70);
    }

    m_stackTraceWindow->show();
    m_stackTraceWindow->raise();
}

void TowerOfHanoi::stepCall(int n, TowerStack from, TowerStack to, TowerStack spare, StepRecursion recursion, void *frame)
{
    m_stackTrace.push({ n, from, to, spare, recursion, frame });
}

void TowerOfHanoi::stepReturn()
{
    m_stackTrace.pop();
}

void TowerOfHanoi::moveDisk(TowerStack, TowerStack)
{
    emit stackTraceChanged();
}

void TowerOfHanoi::stackTraceReset()
{
    m_stackTrace.clear();
    emit stackTraceChanged();
}

void TowerOfHanoi::done()
{
    ui->pushButton->setText("Reset");
    emit stackTraceChanged();
}
