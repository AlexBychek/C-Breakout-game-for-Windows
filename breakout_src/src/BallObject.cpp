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
    this->position_.x += INT(this->velocity_.x);
    this->position_.y += INT(this->velocity_.y);

    if (this->position_.x <= ::START_X)
    {
        this->velocity_.x = -this->velocity_.x;
        this->position_.x = ::START_X;
    }
    else if (this->position_.x + this->size_.x >= ::PMATRIX_WIDTH - ::START_X)
    {
        this->velocity_.x = -this->velocity_.x;
        this->position_.x = (::PMATRIX_WIDTH - ::START_X) - this->size_.x;
    }
    if (this->position_.y <= ::START_Y)
    {
        this->velocity_.y = -this->velocity_.y;
        this->position_.y = ::START_Y;
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