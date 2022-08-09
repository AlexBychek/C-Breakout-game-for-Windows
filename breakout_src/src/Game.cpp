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

template<typename T>
T lengthVector( Vec2_<T> a )
{
    return std::sqrt( std::pow(a.x, 2) + std::pow(a.y, 2) );
}

Vec2f normalize( Vec2f a )
{
    float locLength = lengthVector(a);
    float invLength = (1 / locLength);
    a.x *= invLength;
    a.y *= invLength;
    return a;
}

Game::Game( HWND* consoleWindow )
    : renderer_( new Renderer( consoleWindow ) )
    , initTime_( getMills() )
    , gameStatus_( STATUS_WAIT )
{
    initializeGameObjects();
}

Game::~Game()
{
    renderer_ = nullptr;
    player_   = nullptr;
    ball_     = nullptr;
}

void Game::initializeGameObjects()
{
    levels_ = GameLevelsCreator::createGameLevels();

    player_ = std::make_unique< GameObject >(GameObjectCreator::createPerson() );

    ball_ = std::make_unique< BallObject >(
            Vec2d( ::PMATRIX_WIDTH / 2 - 10
                , (::PMATRIX_HEIGHT - ::START_Y) / 2 )
            , Vec2f( 0.0f, 20.0f )
            , Color( 255, 255, 255 )
    );
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

void Game::objectsPositionUpdate()
{
    for (auto& object : levels_[statistic_.levelNo].getBricks())
    {
        if (!object.isDestroy())
        {
            objectPositionUpdate(object);
        }
    }

    objectPositionUpdate(*player_);
    objectPositionUpdate(*ball_);
}

void Game::draw()
{
    resetMainDigitalMatrix();
    healthIconDraw();
    objectsPositionUpdate();

    renderer_->draw();
}

void Game::handleInput()
{
    int predictPosition;
    if (GetAsyncKeyState(VK_LEFT))
    {
        predictPosition = player_->getPosition().x - player_->getVelocity().x;

        if (predictPosition >= ::START_X)
        {
            player_->setXPosition( predictPosition );
        }
    }
    else if (GetAsyncKeyState(VK_RIGHT))
    {
        predictPosition = player_->getPosition().x + player_->getVelocity().x;

        if (predictPosition < ::PMATRIX_WIDTH - ::START_X - player_->getSize().x)
        {
            player_->setXPosition( predictPosition );
        }
    }
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

void Game::healthChecker()
{
    if (ball_->getPosition().y + ball_->getSize().x >= ::PMATRIX_HEIGHT)
    {
        --statistic_.healthPoint;

        ball_->reset();
        player_->reset();
        initTime_ = getMills();

        if (statistic_.healthPoint == 0)
        {
            gameStatus_ = STATUS_END;
            statistic_.lose = true;
        }
    }
}

bool Game::timeChecker()
{
    uint64_t now = getMills();

    if (now - initTime_ >= ::TIMEOUT_MS)
    {
        if (gameStatus_ == STATUS_WAIT)
        {
            startTime_ = now;
            gameStatus_ = STATUS_START;
        }

        statistic_.time = ((now - startTime_) / 1000);
    }
    else
    {
        statistic_.time = 3 - ((now - initTime_) / 1000);
        renderer_->setTimer(statistic_.time);
        return false;
    }

    renderer_->setTimer(statistic_.time);

    return true;
}

void Game::levelChecker()
{
    if (!levels_[statistic_.levelNo].getActiveBricks())
    {
        if (statistic_.levelNo < levels_.size() - 1)
        {
            ++statistic_.levelNo;
            player_->reset();
            ball_->reset();
            initTime_ = getMills();
        }
        else
        {
            gameStatus_ = STATUS_END;
        }
    }
}

void Game::updateFrame()
{
    handleInput();
    handleLogic();
    draw();
}

void Game::handleLogic()
{
    renderer_->setScores(statistic_.scores);
    renderer_->setLevel(statistic_.levelNo + 1);

    healthChecker();

    if (timeChecker())
    {
        levelChecker();

        ball_->move();

        for (auto& object : levels_[statistic_.levelNo].getBricks())
        {
            if (!object.isDestroy())
            {
                std::tuple< bool, Directions, Vec2f > collision = checkCollision(*ball_, object);
                Directions dir = std::get<1>(collision);
                Vec2f diffVector = std::get<2>(collision);

                if (std::get<0>( collision ))
                {
                    if (object.getHealthPoint() > 1)
                    {
                        if (dir == DIRECTION_LEFT || dir == DIRECTION_RIGHT)
                        {
                            if (object.getHealthPoint() > 2)
                            {
                                ball_->setYVelocity( -ball_->getVelocity().y );
                            }
                            ball_->setXVelocity( -ball_->getVelocity().x );
                            float penetration = ball_->getRadius() - std::abs(diffVector.x);

                            if (dir == DIRECTION_LEFT)
                            {
                                ball_->setXPosition( ball_->getPosition().x + penetration );
                            }
                            else
                            {
                                ball_->setXPosition( ball_->getPosition().x - penetration );
                            }
                        }
                        else
                        {
                            if (object.getHealthPoint() > 2)
                            {
                                ball_->setXVelocity( -ball_->getVelocity().x );
                            }
                            ball_->setYVelocity( -ball_->getVelocity().y );
                            float penetration = ball_->getRadius() - std::abs(diffVector.y);
                            if (dir == DIRECTION_UP)
                            {
                                ball_->setYPosition( ball_->getPosition().y - penetration );
                            }
                            else
                            {
                                ball_->setYPosition( ball_->getPosition().y + penetration );
                            }
                        }
                    }

                    if (!object.isSolid())
                    {
                        object.getDamage();

                        object.colorUpdate();

                        if (object.getHealthPoint() == 0)
                        {
                            object.destroy();
                            statistic_.scores += object.getScores();
                            --levels_[statistic_.levelNo];
                        }
                    }
                }
            }
        }

        std::tuple< bool, Directions, Vec2f > result = checkCollision(*ball_, *player_);

        if (std::get<0>(result))
        {
            float centerBoard = player_->getPosition().x + player_->getSize().x / 2.0f;
            float distance = (ball_->getPosition().x + ball_->getRadius()) - centerBoard;
            float percentage = distance / (FLOAT(player_->getSize().x) / 2.0f);

            float strength = 2.0f;
            Vec2f oldVelocity = ball_->getVelocity();

            ball_->setXVelocity( FLOAT(::INITIAL_BALL_VELOCITY.x) * percentage * strength );
            ball_->setYVelocity( -ball_->getVelocity().y );
            ball_->setVelocity( normalize(ball_->getVelocity() ) );
            ball_->setXVelocity( ball_->getVelocity().x * lengthVector(oldVelocity) );
            ball_->setYVelocity( ball_->getVelocity().y * lengthVector(oldVelocity) );
            ball_->setYVelocity( -1.0f * abs(ball_->getVelocity().y) );
        }
    }
}

void Game::healthIconDraw()
{
    for (int i = 0; i < statistic_.healthPoint; ++i)
    {
        int healthStartPositionX = 20 + i * 20;

        for (int y = 0; y < 20; ++y)
        {
            for (int x = 0; x < 20; ++x)
            {
                if (healthIconPatternArray[20 * y + x ] != 0)
                {
                    renderer_->mainDigitalMatrix[(y + 20) * ::PMATRIX_WIDTH + healthStartPositionX + x] = RGB(255, 0, 0);
                }
            }
        }
    }
}


void Game::resetMainDigitalMatrix()
{
    for (int y = 0; y < ::PMATRIX_HEIGHT; ++y)
    {
        for (int x = 0; x < ::PMATRIX_WIDTH; ++x)
        {
            if ( (x < ::PMATRIX_BORDER || x >= ::PMATRIX_WIDTH - ::PMATRIX_BORDER) ||
                (y < ::PMATRIX_BORDER || (y > ::DASHBOARD_HEIGHT && y < ::DASHBOARD_HEIGHT + ::PMATRIX_BORDER) ) )
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

Statistic Game::checkGameEnd( bool* game )
{
    if (gameStatus_ == STATUS_END)
    {
        *game = false;
        draw();
    }

    statistic_.time = ((getMills() - startTime_) / 1000);
    return statistic_;
}