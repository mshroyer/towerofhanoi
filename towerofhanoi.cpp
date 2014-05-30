#include "towerofhanoi.h"
#include "ui_towerofhanoi.h"

#include <QThread>

TowerOfHanoi::TowerOfHanoi(QWidget *parent) :
    QMainWindow { parent },
    m_tower { new Tower { 6, this } },
    m_towerSolver { nullptr },
    ui { new Ui::TowerOfHanoi }
{
    ui->setupUi(this);
    ui->towerView->setTower(m_tower);
    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(pushButton()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), m_tower, SLOT(reset(int)));
}

TowerOfHanoi::~TowerOfHanoi()
{
    delete ui;
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
        ui->pushButton->setText("Start");
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

void TowerOfHanoi::done()
{
    ui->pushButton->setText("Reset");
}
