//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_GAMEOBJECTCREATOR_H
#define BREAKOUT_GAMEOBJECTCREATOR_H

#include <Brick.h>

class GameObjectCreator
{
public:
    static Brick createBrick( const uint32_t& brickId
                            , const Vec2d& position );

    static GameObject createPerson();
};


#endif //BREAKOUT_GAMEOBJECTCREATOR_H
