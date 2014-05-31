#include "towerofhanoi.h"
#include "ui_towerofhanoi.h"

#include <QThread>
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
    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(pushButton()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), m_tower, SLOT(reset(int)));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

TowerOfHanoi::~TowerOfHanoi()
{
    delete m_towerSolver;
    delete ui;
}

void TowerOfHanoi::about()
{
    QMessageBox aboutBox { this };
    aboutBox.setWindowTitle("About Tower of Hanoi");
    aboutBox.setTextFormat(Qt::RichText);
    aboutBox.setText(
                "Tower of Hanoi recursive solution<br><br>"
                "Mark Shroyer &lt;<a href='mailto:code@markshroyer.com'>code@markshroyer.com</a>&gt;<br>"
                "<a href='https://bitbucket.org/markshroyer/towerofhanoi/'>https://bitbucket.org/markshroyer/towerofhanoi/</a>"
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
        m_tower->reset(m_tower->ndisks());
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

void TowerOfHanoi::done()
{
    ui->pushButton->setText("Reset");
}
