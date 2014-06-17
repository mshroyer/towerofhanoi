#ifndef DATATYPES_H
#define DATATYPES_H

enum class TowerStack : char
{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

inline TowerStack otherStack(TowerStack a, TowerStack b)
{
    return static_cast<TowerStack>(3 - static_cast<int>(a) - static_cast<int>(b));
}

struct StackFrame
{
    int n;
    TowerStack to;
    int sub;
    void *fp;
};

#endif // DATATYPES_H
