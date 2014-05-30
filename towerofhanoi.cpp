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
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(solve()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), m_tower, SLOT(reset(int)));
    connect(m_towerSolver, SIGNAL(finished()), this, SLOT(done()));
}

TowerOfHanoi::~TowerOfHanoi()
{
    delete ui;
}

void TowerOfHanoi::solve()
{
    ui->spinBox->setEnabled(false);
    ui->pushButton->setEnabled(false);

    delete m_towerSolver;
    m_towerSolver = new TowerSolver { m_tower };
    connect(m_towerSolver, SIGNAL(finished()), this, SLOT(done()));
    m_towerSolver->start();
}

void TowerOfHanoi::done()
{
    disconnect(this, SLOT(solve()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(reset()));
    ui->pushButton->setText("Reset");
    ui->pushButton->setEnabled(true);
}

void TowerOfHanoi::reset()
{
    m_tower->reset(ui->spinBox->value());

    disconnect(this, SLOT(reset()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(solve()));
    ui->pushButton->setText("Solve");
    ui->spinBox->setEnabled(true);
}
