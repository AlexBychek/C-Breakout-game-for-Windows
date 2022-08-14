//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_GAMEOBJECT_H
#define BREAKOUT_GAMEOBJECT_H

#include <Common.h>
#include <vector>
#include <gtest/gtest.h>

class GameObject
{
protected:
    bool     isDestroyed_;
    bool     isSolid_;
    Vec2d    initPosition_;
    Vec2d    position_;
    Vec2d    size_;
    Vec2f    initVelocity_;
    Vec2f    velocity_;
    Color    color_;
    uint32_t scores_;
    uint32_t healthPoints_;

public:

    GameObject() = default;
    virtual ~GameObject() = default;

    GameObject( const Vec2d& positionObject
              , const Vec2d& sizeObject
              , const Vec2f& velocity
              , const Color& colorObject );

    GameObject( const Vec2d& positionObject
              , const Vec2d& sizeObject
              , const Vec2f& velocity
              , const Color& colorObject
              , const uint32_t& healthPoints
              , const uint32_t& scores
              , const bool& isSolid );

    virtual void reset();

    bool isDestroy() const;
    bool isSolid() const;
    Vec2d getPosition() const;
    Vec2f getVelocity() const;

    void setXPosition( const int& position );
    void setYPosition( const int& position );
    void setXVelocity( const float& velocity );
    void setYVelocity( const float& velocity );
    void setVelocity( const Vec2f& velocity );


    Vec2d getSize() const;
    Color getColor() const;
    uint32_t getScores() const;
    uint32_t getHealthPoint() const;

    void destroy();
    void getDamage();
};

class TestGameObject : public ::testing::Test
{
protected:
    void SetUp()
    {
        gameObject1.reset( new GameObject( Vec2d( 100, 100), Vec2d( 10, 10 ), Vec2f( 10.0f, 0.0f ), Color( 255, 255, 255) ) );
        gameObject2.reset( new GameObject( Vec2d( 200, 150), Vec2d( 20, 20 ), Vec2f( 20.0f, 10.0f ), Color( 128, 128, 128), 10, 10, true ) );
    }
    void TearDown()
    {
        gameObject1 = nullptr;
        gameObject2 = nullptr;
    }

    std::unique_ptr<GameObject> gameObject1;
    std::unique_ptr<GameObject> gameObject2;
};

#endif //BREAKOUT_GAMEOBJECT_H
