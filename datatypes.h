#ifndef STACKTRACE_H
#define STACKTRACE_H

enum class Stack : int {
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

enum class StepRecursion : int
{
    ROOT = 0,
    LEFT = 1,
    RIGHT = 2
};

struct StepCall
{
    int n;
    Stack from;
    Stack to;
    Stack spare;
    StepRecursion recursion;
    void *frame;
};

#endif // STACKTRACE_H
