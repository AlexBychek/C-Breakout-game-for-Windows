//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_BRICK_H
#define BREAKOUT_BRICK_H

#include <GameObject.h>
#include <gtest/gtest.h>

class Brick : public GameObject
{
    uint32_t brickId_;
public:

    Brick() = default;
    ~Brick() = default;

    Brick( const uint32_t& brickId
         , const Vec2d& position );

    void colorUpdate();
    uint32_t getBrickId() const;
};

typedef std::vector<Brick> Bricks;

class TestBrick : public ::testing::Test
{
protected:
    void SetUp()
    {
        brick.reset( new Brick( 1, Vec2d(100,100) ) );
    }
    void TearDown()
    {
        brick = nullptr;
    }

    std::unique_ptr<Brick> brick;
};

#endif //BREAKOUT_BRICK_H
