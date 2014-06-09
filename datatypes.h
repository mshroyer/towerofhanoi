#ifndef DATATYPES_H
#define DATATYPES_H

enum class TowerStack : int
{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

struct StackFrame
{
    int n;
    TowerStack from;
    TowerStack to;
    TowerStack spare;
    int sub;
    void *fp;
};

#endif // DATATYPES_H
