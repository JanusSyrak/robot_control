#ifndef STRUCTS_H
#define STRUCTS_H


struct actionvalue
{
    double RIGHT;
    double UP;
    double LEFT;
    double DOWN;
};

struct state
{
    int x;
    int y;
    bool is_outside_environment;
    bool is_visited;
    double reward;
    std::vector<actionvalue> Q;
};

#endif // STRUCTS_H
