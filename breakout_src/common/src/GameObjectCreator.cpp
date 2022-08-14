//
// Created by https://github.com/AlexBychek
//

#include <GameObjectCreator.h>
#include <Config.h>

Brick GameObjectCreator::createBrick( const uint32_t& brickId
                                    , const Vec2d& position )
{
    return Brick(brickId, position );
}

GameObject GameObjectCreator::createPerson( const Vec2d& position, const Vec2d& size  )
{
    auto person = GameObject( position, size, Vec2f( PLAYER_VELOCITY, PLAYER_VELOCITY ), Color(0, 255, 255), 0, 0, true );

    return person;
}