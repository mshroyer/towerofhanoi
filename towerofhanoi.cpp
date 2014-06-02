#include "towerofhanoi.h"
#include "ui_towerofhanoi.h"

#include "callstackwindow.h"

#include <QMessageBox>

TowerOfHanoi::TowerOfHanoi(QWidget *parent) :
    QMainWindow { parent },
    m_tower { new Tower { 6, this } },
    m_towerSolver { nullptr },
    ui { new Ui::TowerOfHanoi }
{
    qRegisterMetaType<Tower::Stack>("Tower::Stack");

    ui->setupUi(this);
    ui->towerView->setTower(m_tower);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButton()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), m_tower, SLOT(reset(int)));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionCallGraph, SIGNAL(triggered()), this, SLOT(callGraphWindow()));
}

TowerOfHanoi::~TowerOfHanoi()
{
    if (m_callStackWindow) {
        m_callStackWindow->close();
        delete m_callStackWindow;
    }

    delete m_towerSolver;
    delete ui;
}

void TowerOfHanoi::about()
{
    QMessageBox aboutBox { this };

    aboutBox.setWindowTitle("About Tower of Hanoi");
    aboutBox.setTextFormat(Qt::RichText);
    aboutBox.setText(
                "<b>About Tower of Hanoi v1.0.0</b><br>"
                "<br>"
                "This program implements a recursive solution to the Tower of Hanoi.  For more information "
                "see the Wikipedia article and source code repository linked below.<br>"
                "<br>"
                "<a href='https://en.wikipedia.org/wiki/Tower_of_hanoi'>https://en.wikipedia.org/wiki/Tower_of_hanoi</a><br>"
                "<a href='https://bitbucket.org/markshroyer/towerofhanoi/'>https://bitbucket.org/markshroyer/towerofhanoi/</a><br>"
                "<br>"
                "Mark Shroyer<br>"
                "31 May 2014"
                );

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
    } else {
        // Start

        m_towerSolver = new TowerSolver { m_tower };
        connect(m_towerSolver, &QThread::finished, this, &TowerOfHanoi::done);
        ui->pushButton->setText("Stop");
        ui->spinBox->setEnabled(false);
        m_towerSolver->start();
    }
}

void TowerOfHanoi::closeEvent(QCloseEvent *event)
{
    if (m_towerSolver && m_towerSolver->isRunning())
        m_towerSolver->terminate();

    QWidget::closeEvent(event);
}

void TowerOfHanoi::callGraphWindow()
{
    if (!m_callStackWindow)
        m_callStackWindow = new CallStackWindow { };

    m_callStackWindow->show();
}

void TowerOfHanoi::done()
{
    ui->pushButton->setText("Reset");
}
