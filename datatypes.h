#ifndef DATATYPES_H
#define DATATYPES_H

enum class TowerStack : int
{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

enum class MoveTowerRecursion : int
{
    ROOT = 0,
    LEFT = 1,
    RIGHT = 2
};

struct StackFrame
{
    int n;
    TowerStack from;
    TowerStack to;
    TowerStack spare;
    MoveTowerRecursion recursion;
    void *fp;
};

#endif // DATATYPES_H
