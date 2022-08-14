//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_BALLOBJECT_H
#define BREAKOUT_BALLOBJECT_H

#include <GameObject.h>
#include <gtest/gtest.h>

class BallObject : public GameObject
{
    uint32_t radius_;
public:

    BallObject() = default;
    ~BallObject() = default;

    BallObject( const Vec2d& position
              , const Vec2f& velocity
              , const Color& color );

    void reset() override;
    void move();
    uint32_t getRadius() const;
};

class TestBallObject : public ::testing::Test
{
protected:
    void SetUp()
    {
        gameBall.reset( new BallObject( Vec2d( 100, 100 ), Vec2f( 10.0f, 0.0f ), Color( 255, 255, 255) ) );
    }
    void TearDown()
    {
        gameBall = nullptr;
    }

    std::unique_ptr<BallObject> gameBall;
};

#endif //BREAKOUT_BALLOBJECT_H
