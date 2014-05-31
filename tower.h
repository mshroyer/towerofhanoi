#ifndef TOWERS_H
#define TOWERS_H

#include <QObject>

class Tower : public QObject
{
    Q_OBJECT

public:
    explicit Tower(int ndisks = 6, QObject *parent = nullptr);

    enum class Stack : int {
        LEFT = 0,
        MIDDLE = 1,
        RIGHT = 2
    };

signals:
    void moved(void);
    void moveError(QString errmsg);

public slots:
    void reset();
    void reset(int ndisks);
    void moveDisk(Stack from, Stack to);

public:
    int ndisks(void) const;
    const QList<int> &stack(Stack name) const;

private:
    QList<int> &getStack(Stack name);
    int m_ndisks;
    QList<int> m_stacks[3];
};

#endif // TOWERS_H
