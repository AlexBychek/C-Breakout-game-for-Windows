//
// Created by https://github.com/AlexBychek
//

#include <GameObject.h>

GameObject::GameObject( const Vec2d& positionObject
                      , const Vec2d& sizeObject
                      , const Vec2f& velocity
                      , const Color& colorObject )
        : position_(positionObject )
        , initPosition_(positionObject )
        , size_(sizeObject )
        , color_(colorObject )
        , velocity_(velocity )
        , initVelocity_(velocity )
        , healthPoints_(1 )
        , scores_(0 )
        , isDestroyed_(false )
        , isSolid_(false )
{
}

GameObject::GameObject( const Vec2d& positionObject
                      , const Vec2d& sizeObject
                      , const Vec2f& velocity
                      , const Color& colorObject
                      , const uint32_t& healthPoints
                      , const uint32_t& scores
                      , const bool& isSolid)
        : position_(positionObject )
        , initPosition_(positionObject )
        , size_(sizeObject )
        , initVelocity_(velocity )
        , velocity_(velocity )
        , color_(colorObject )
        , healthPoints_(healthPoints )
        , scores_(scores )
        , isDestroyed_(false )
        , isSolid_(isSolid )
{
}

void GameObject::reset()
{
    position_ = initPosition_;
}

bool GameObject::isDestroy() const
{
    return isDestroyed_;
}

bool GameObject::isSolid() const
{
    return isSolid_;
}

Vec2d GameObject::getPosition() const
{
    return position_;
}

Vec2f GameObject::getVelocity() const
{
    return velocity_;
}

void GameObject::setXPosition( const int& position )
{
    position_.x = position;
}

void GameObject::setYPosition( const int& position )
{
    position_.y = position;
}

void GameObject::setXVelocity( const float& velocity )
{
    velocity_.x = velocity;
}

void GameObject::setYVelocity( const float& velocity )
{
    velocity_.y = velocity;

}

void GameObject::setVelocity( const Vec2f& velocity )
{
    velocity_ = velocity;
}

Vec2d GameObject::getSize() const
{
    return size_;
}

Color GameObject::getColor() const
{
    return color_;
}

uint32_t GameObject::getScores() const
{
    return scores_;
}

uint32_t GameObject::getHealthPoint() const
{
    return healthPoints_;
}

void GameObject::destroy()
{
    isDestroyed_ = true;
}

void GameObject::getDamage()
{
    --healthPoints_;
}

TEST_F(TestGameObject, test_game_object)
{
    ASSERT_EQ(gameObject1->getPosition().x, 100);
    ASSERT_EQ(gameObject1->getPosition().y, 100);
    ASSERT_EQ(gameObject1->getVelocity().x, 10.0f);
    ASSERT_EQ(gameObject1->getVelocity().y, 0.0f);
    ASSERT_EQ(gameObject1->getSize().x, 10);
    ASSERT_EQ(gameObject1->getSize().y, 10);
    ASSERT_EQ(gameObject1->getColor().red,   255);
    ASSERT_EQ(gameObject1->getColor().green, 255);
    ASSERT_EQ(gameObject1->getColor().blue,  255);

    gameObject1->setXPosition( 150 );
    gameObject1->setYPosition( 150 );
    ASSERT_EQ(gameObject1->getPosition().y, 150);
    ASSERT_EQ(gameObject1->getPosition().y, 150);
    gameObject1->setXVelocity( 110.0f );
    gameObject1->setYVelocity( 10.0f );
    ASSERT_EQ(gameObject1->getVelocity().x, 110.0f);
    ASSERT_EQ(gameObject1->getVelocity().y, 10.0f);
    gameObject1->setVelocity( Vec2f( 0.0f, 0.0f ) );
    ASSERT_EQ(gameObject1->getVelocity().x, 0.0f);
    ASSERT_EQ(gameObject1->getVelocity().y, 0.0f);

    ASSERT_EQ(gameObject2->getPosition().x, 200);
    ASSERT_EQ(gameObject2->getPosition().y, 150);
    ASSERT_EQ(gameObject2->getVelocity().x, 20.0f);
    ASSERT_EQ(gameObject2->getVelocity().y, 10.0f);
    ASSERT_EQ(gameObject2->getSize().x, 20);
    ASSERT_EQ(gameObject2->getSize().y, 20);
    ASSERT_EQ(gameObject2->getColor().red,   128);
    ASSERT_EQ(gameObject2->getColor().green, 128);
    ASSERT_EQ(gameObject2->getColor().blue,  128);
    ASSERT_EQ(gameObject2->getScores(), 10);
    ASSERT_EQ(gameObject2->getHealthPoint(), 10);
    ASSERT_EQ(gameObject2->isSolid(),   true);
    ASSERT_EQ(gameObject2->isDestroy(), false);
}
