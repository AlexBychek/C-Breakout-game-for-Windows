//
// Created by https://github.com/AlexBychek
//

#include <BallObject.h>
#include <Config.h>

BallObject::BallObject( const Vec2d& position
                      , const Vec2f& velocity
                      , const Color& color )
                      : GameObject(position, {::BALL_RADIUS * 2, ::BALL_RADIUS * 2 }, velocity, color )
                      , radius_(::BALL_RADIUS )
{
}

void BallObject::move()
{
    position_.x += INT(velocity_.x);
    position_.y += INT(velocity_.y);

    if ( position_.x <= ::START_X )
    {
        velocity_.x = -velocity_.x;
        position_.x = ::START_X;
    }
    else if ( position_.x + size_.x >= ::PMATRIX_WIDTH - ::START_X )
    {
        velocity_.x = -velocity_.x;
        position_.x = (::PMATRIX_WIDTH - ::START_X) - size_.x;
    }

    if ( position_.y <= ::START_Y )
    {
        velocity_.y = -velocity_.y;
        position_.y = ::START_Y;
    }
    else if ( position_.y + size_.y >= ::PMATRIX_HEIGHT - ::BRICK_BORDER )
    {
        velocity_.y = -velocity_.y;
        position_.y = ::PMATRIX_HEIGHT - ::BRICK_BORDER - size_.y;
    }
}

void BallObject::reset()
{
    position_ = initPosition_;
    velocity_ = initVelocity_;
}
uint32_t BallObject::getRadius() const
{
    return radius_;
}

TEST_F(TestBallObject, test_ball)
{
    ASSERT_EQ(gameBall->getPosition().x, 100);
    ASSERT_EQ(gameBall->getPosition().y, 100);

    ASSERT_EQ(gameBall->getVelocity().x, 10.0f);
    ASSERT_EQ(gameBall->getVelocity().y, 0.0f);

    ASSERT_EQ(gameBall->getColor().red,   255);
    ASSERT_EQ(gameBall->getColor().green, 255);
    ASSERT_EQ(gameBall->getColor().blue,  255);

    ASSERT_EQ(gameBall->getRadius(), 10);
    ASSERT_EQ(gameBall->getSize().x, 20);
    ASSERT_EQ(gameBall->getSize().y, 20);

    gameBall->move();

    ASSERT_EQ(gameBall->getPosition().x, 110);
    ASSERT_EQ(gameBall->getPosition().y, 100);

    gameBall->setXVelocity(150.0f);
    gameBall->setYVelocity(100.0f);

    ASSERT_EQ(gameBall->getVelocity().x, 150.0f);
    ASSERT_EQ(gameBall->getVelocity().y, 100.0f);
}