//
// Created by https://github.com/AlexBychek
//

#include <Brick.h>
#include <Config.h>

static std::vector<Color> BRICK_COLOR_ARRAY =
{
    Color( 255, 255,   0 ), // YELLOW
    Color( 255, 128,   0 ), // ORANGE
    Color( 255,   0,   0 ), // RED
    Color( 102,   0,   0 ), // MAROON
    Color( 128, 128, 128 )  // GREY
};

Brick::Brick( const uint32_t& brickId
            , const Vec2d& position )
            : brickId_( brickId )
            , GameObject( position, Vec2d(::BRICK_WIDTH, ::BRICK_HEIGHT ), Vec2f(0.0f, 0.0f ), ::BRICK_COLOR_ARRAY[ (brickId >= ::BRICK_COLOR_ARRAY.size() ? ::BRICK_COLOR_ARRAY.size() - 1 : brickId ) ] )
{
    if (brickId_ >= 4)
    {
        healthPoints_ = 9;
        scores_ = 0;
        isSolid_ = true;
    }
    else
    {
        healthPoints_ = (brickId_ + 1);
        scores_ = (brickId_ + 1) * 10;
        isSolid_ = false;
    }
}

void Brick::colorUpdate()
{
    if ( !healthPoints_ )
    {
        return;
    }
    color_ = ::BRICK_COLOR_ARRAY[healthPoints_ - 1];
}

uint32_t Brick::getBrickId() const
{
    return brickId_;
}

TEST_F(TestBrick, test_brick)
{
    ASSERT_EQ(brick->getBrickId(),     1);
    ASSERT_EQ(brick->getHealthPoint(), 2);

    brick->getDamage();
    ASSERT_EQ(brick->getHealthPoint(), 1);

    ASSERT_EQ(brick->getScores(),      20);
    ASSERT_EQ(brick->getPosition().x,  100);
    ASSERT_EQ(brick->getPosition().y,  100);
    ASSERT_EQ(brick->isSolid(),        false);
    ASSERT_EQ(brick->isDestroy(),      false);

    brick->getDamage();
    ASSERT_EQ(brick->getHealthPoint(), 0);
    brick->destroy();
    ASSERT_EQ(brick->isDestroy(),      true);


    ASSERT_EQ(brick->getColor().red,    ::BRICK_COLOR_ARRAY[ brick->getBrickId() ].red);
    ASSERT_EQ(brick->getColor().green,  ::BRICK_COLOR_ARRAY[ brick->getBrickId() ].green);
    ASSERT_EQ(brick->getColor().blue,   ::BRICK_COLOR_ARRAY[ brick->getBrickId() ].blue);
}