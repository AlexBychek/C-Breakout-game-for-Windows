//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_STATISTIC_H
#define BREAKOUT_STATISTIC_H

#include <stdint.h>

class Statistic
{
public:
    bool     lose;
    uint64_t scores;
    uint32_t healthPoint;
    uint32_t time;
    uint32_t levelNo;

    Statistic();
    ~Statistic() = default;
};


#endif //BREAKOUT_STATISTIC_H
