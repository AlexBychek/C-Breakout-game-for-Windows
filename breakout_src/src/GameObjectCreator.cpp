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

GameObject GameObjectCreator::createPerson()
{
    Vec2d position( ( PMATRIX_WIDTH - PERSON_WIDTH) / 2, PMATRIX_HEIGHT - START_Y );

    auto person = GameObject( position, Vec2d( PERSON_WIDTH, PERSON_HEIGHT ), Vec2f( PLAYER_VELOCITY, 0.0f ), Color(0, 255, 255), 0, 0, true );

    return person;
}