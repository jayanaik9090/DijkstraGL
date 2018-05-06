//
// Created by ghostship on 4/30/2018.
//
#include <array>
#ifndef DIJKSTRA_COLOR_H
#define DIJKSTRA_COLOR_H

//typedef float *COLOR;
typedef std::array<int,3> COLOR;

namespace color {

    static const float RED[] = {1,0,0};
    static const float GREEN[] = {0,1,0};
    static const float BLUE[] = {0,0,1};
    static const float WHITE[] = {1,1,1};
    static const float YELLOW[] = {1,1,0};
    static const float BLACK[] = {0,0,0};
};


#endif //DIJKSTRA_COLOR_H
