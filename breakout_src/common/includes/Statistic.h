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

    uint32_t player1Scores;
    uint32_t player2Scores;

    Statistic();
    ~Statistic() = default;

    virtual void writeGameResult() = 0;
};

class StatisticBreakout : public Statistic
{
public:
    void writeGameResult() override;
};

class StatisticPong : public Statistic
{
public:
    void writeGameResult() override;
};

#endif //BREAKOUT_STATISTIC_H
