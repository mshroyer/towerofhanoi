#include "towerview.h"
#include "tower.h"

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QThread>

namespace {

const QVector<QColor> disk_colors {
    QColor { 148, 0, 211 },
    QColor { 75, 0, 130 },
    QColor { 0, 0, 255 },
    QColor { 0, 128, 0 },
    QColor { 255, 255, 0 },
    QColor { 255, 128, 0 },
    QColor { 255, 0, 0 }
};

QColor diskColor(int disk)
{
    return disk_colors[disk % disk_colors.size()];
}

int diskWidth(int disk_min, int disk_max, int ndisks, int i)
{
    return disk_min + (i - 1) * (disk_max - disk_min) / ndisks;
}

} // namespace

TowerView::TowerView(QWidget *parent) :
    QWidget { parent }
{
}

void TowerView::setTower(Tower *tower)
{
    if (m_tower)
        return;

    m_tower = tower;
    connect(m_tower, &Tower::moveError, this, &TowerView::debug);
    connect(m_tower, SIGNAL(moved()), this, SLOT(update()));
    //m_tower->moveDisk(Tower::Stack::LEFT, Tower::Stack::RIGHT);
}

void TowerView::paintEvent(QPaintEvent *)
{
    QPainter painter { this };

    const int ndisks         = m_tower->ndisks();
    const int stroke         = 10;

    // Container dimensions
    const int width          = 9 * (rect().width() / 10);
    const int x_left         = rect().width() / 20;
    const int x_right        = x_left + width;
    const int height         = stroke * (1 + m_tower->ndisks());
    const int y_top          = (rect().height() - height) / 2;
    const int y_bottom       = y_top + height;

    // Tower positions
    const int x_tower_left   = x_left + width / 6;
    const int x_tower_middle = x_left + width / 2;
    const int x_tower_right  = x_left + 5 * width / 6;

    // Disks
    const int disk_max       = 9 * (width / 3) / 10;
    const int disk_min       = disk_max / 4;

    painter.setPen(QPen(Qt::black, stroke, Qt::SolidLine, Qt::SquareCap));
    painter.drawLine(QPoint(x_left, y_bottom), QPoint(x_right, y_bottom));
    painter.drawLine(QPoint(x_tower_left, y_top), QPoint(x_tower_left, y_bottom));
    painter.drawLine(QPoint(x_tower_middle, y_top), QPoint(x_tower_middle, y_bottom));
    painter.drawLine(QPoint(x_tower_right, y_top), QPoint(x_tower_right, y_bottom));

    int disk_position;

    int tower_x_offsets[] = { x_left, x_left + width / 3, x_left + 2 * width / 3 };
    const QList<int> towers[] = {
        m_tower->stack(Tower::Stack::LEFT),
        m_tower->stack(Tower::Stack::MIDDLE),
        m_tower->stack(Tower::Stack::RIGHT)
    };
    for (int i = 0; i < 3; ++i) {
        const QList<int> &tower = towers[i];
        disk_position = ndisks - tower.size();
        for (int disk : tower) {
            const int y = y_top + stroke * (1 + disk_position++);
            const int disk_width = diskWidth(disk_min, disk_max, ndisks, disk);
            const int disk_offset = (width / 3 - disk_width) / 2;
            const int x = tower_x_offsets[i] + disk_offset;

            painter.setPen(QPen(diskColor(disk), stroke, Qt::SolidLine, Qt::FlatCap));
            painter.drawLine(QPoint(x, y), QPoint(x + disk_width, y));
        }
    }
}

void TowerView::debug(QString msg)
{
    qDebug() << qPrintable(msg);
}
