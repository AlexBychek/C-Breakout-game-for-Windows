//
// Created by https://github.com/AlexBychek
//

#include <Game.h>
#include <GameObjectCreator.h>
#include <GameLevelsCreator.h>
#include <HealthIcon.h>
#include <DateTime.h>

#include <iostream>
#include <filesystem>

Game::Game( HWND* consoleWindow, const GameType& gameType )
    : renderer_( new Renderer( consoleWindow, gameType ) )
    , initTime_( getMills() )
    , gameStatus_( STATUS_WAIT )
{
}

Game::~Game()
{
    renderer_ = nullptr;
    player_   = nullptr;
    ball_     = nullptr;
}

void Game::objectPositionUpdate( GameObject& obj )
{
    int pixMatStartX = obj.getPosition().x;
    int pixMatStartY = obj.getPosition().y;
    int pixMatEndX   = obj.getSize().x + pixMatStartX;
    int pixMatEndY   = obj.getSize().y + pixMatStartY;

    if (pixMatEndX > ::PMATRIX_WIDTH)
    {
        pixMatEndX = ::PMATRIX_WIDTH;
    }

    if (pixMatEndY > ::PMATRIX_HEIGHT)
    {
        pixMatEndY = ::PMATRIX_HEIGHT;
    }

    int objectHeight = 0;
    int objectX;

    for (int y = pixMatStartY; y < pixMatEndY; ++y)
    {
        objectX = ::PMATRIX_BORDER;
        for (int x = pixMatStartX; x < pixMatEndX; ++x)
        {
            renderer_->mainDigitalMatrix[y * ::PMATRIX_WIDTH + x] = RGB(obj.getColor().red, obj.getColor().green, obj.getColor().blue);
            objectX++;
        }
        objectHeight++;
    }
}

void Game::playerMoveUp( std::unique_ptr<GameObject>& player )
{
    int predictPosition = player->getPosition().y - player->getVelocity().y;

    if ( predictPosition >= ::START_Y )
    {
        player->setYPosition( predictPosition );
    }
}

void Game::playerMoveDown( std::unique_ptr<GameObject>& player )
{
    int predictPosition = (player->getPosition().y) + player->getVelocity().y;

    if ( predictPosition <= (::PMATRIX_HEIGHT - 30) - player->getSize().y )
    {
        player->setYPosition( predictPosition );
    }
}

void Game::playerMoveLeft( std::unique_ptr<GameObject>& player )
{
    int predictPosition = player->getPosition().x - player->getVelocity().x;

    if (predictPosition >= ::START_X)
    {
        player->setXPosition( predictPosition );
    }
}

void Game::playerMoveRight( std::unique_ptr<GameObject>& player )
{
    int predictPosition = player->getPosition().x + player->getVelocity().x;

    if (predictPosition < ::PMATRIX_WIDTH - ::START_X - player->getSize().x)
    {
        player->setXPosition( predictPosition );
    }
}

void Game::resetMainDigitalMatrix()
{
    for (int y = 0; y < ::PMATRIX_HEIGHT; ++y)
    {
        for (int x = 0; x < ::PMATRIX_WIDTH; ++x)
        {
            if ( (x < ::PMATRIX_BORDER || x >= ::PMATRIX_WIDTH - ::PMATRIX_BORDER) ||
                 (y < ::PMATRIX_BORDER || (y > ::DASHBOARD_HEIGHT && y < ::DASHBOARD_HEIGHT + ::PMATRIX_BORDER) ) || (y >= ::PMATRIX_HEIGHT - ::PMATRIX_BORDER) )
            {
                renderer_->mainDigitalMatrix[y * ::PMATRIX_WIDTH + x] = RGB(220, 220, 220);
            }
            else
            {
                if ( y <= ::DASHBOARD_HEIGHT )
                {
                    renderer_->mainDigitalMatrix[y * ::PMATRIX_WIDTH + x] = RGB(169, 169, 169);
                }
                else
                {
                    renderer_->mainDigitalMatrix[y * ::PMATRIX_WIDTH + x] = RGB(0, 0, 0);
                }
            }
        }
    }
}

std::tuple< bool, Directions, Vec2f > Game::checkCollision(BallObject& one, GameObject& two )
{
    Vec2d center(one.getPosition().x + one.getRadius(), one.getPosition().y + one.getRadius());
    Vec2f aabbHalf_extents(two.getSize().x / 2.0f, two.getSize().y / 2.0f);
    Vec2f aabbCenter(two.getPosition().x + aabbHalf_extents.x, two.getPosition().y + aabbHalf_extents.y);
    Vec2f difference(center.x - aabbCenter.x, center.y - aabbCenter.y );

    Vec2d clamped(std::min(std::max(difference.x, -aabbHalf_extents.x ), aabbHalf_extents.x ),
                  std::min(std::max( difference.y, -aabbHalf_extents.y ), aabbHalf_extents.y ) );

    Vec2d closest(aabbCenter.x + clamped.x, aabbCenter.y + clamped.y );
    difference = Vec2f(closest.x - center.x, closest.y - center.y);

    if (lengthVector( difference ) < one.getRadius())
    {
        return std::make_tuple( true, VectorDirection( difference ), difference );
    }
    else
    {
        return std::make_tuple(false, DIRECTION_UP, Vec2f(0.0f, 0.0f));
    }
}

std::shared_ptr<Statistic> Game::getStatistic()
{
    return statistic_;
}

bool Game::timeChecker()
{
    uint64_t now = getMills();

    if ( now - initTime_ >= ::TIMEOUT_MS )
    {
        if ( gameStatus_ == STATUS_WAIT )
        {
            startTime_ = now;
            gameStatus_ = STATUS_START;
        }

        statistic_->time = ((now - startTime_) / 1000);
    }
    else
    {
        statistic_->time = 3 - ((now - initTime_) / 1000);
        return false;
    }

    return true;
}

Vec2f normalize( Vec2f a )
{
    float locLength = lengthVector(a);
    float invLength = (1 / locLength);
    a.x *= invLength;
    a.y *= invLength;
    return a;
}

Directions VectorDirection( Vec2f target )
{
    Vec2d compass[] = {
            Vec2d(0.0f, 1.0f),
            Vec2d(1.0f, 0.0f),
            Vec2d(0.0f, -1.0f),
            Vec2d(-1.0f, 0.0f)
    };

    float max = 0.0f;
    unsigned int bestMatch = -1;
    for (unsigned int i = 0; i < 4; ++i)
    {
        auto norm = normalize(target);
        float dotProduct = (norm.x * compass[i].x) + (norm.y * compass[i].y);
        if (dotProduct > max)
        {
            max = dotProduct;
            bestMatch = i;
        }
    }
    return static_cast<Directions>(bestMatch);
}