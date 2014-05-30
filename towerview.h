#ifndef TOWERSVIEW_H
#define TOWERSVIEW_H

#include <QWidget>

class Tower;

class TowerView : public QWidget
{
    Q_OBJECT

public:
    explicit TowerView(QWidget *parent = nullptr);
    void setTower(Tower *tower);

signals:

protected:
    virtual void paintEvent(QPaintEvent *);

private slots:
    void debug(QString msg);

private:
    Tower *m_tower = nullptr;
};

#endif // TOWERSVIEW_H
