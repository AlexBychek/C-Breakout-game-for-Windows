//
// Created by https://github.com/AlexBychek
//

#include <Breakout.h>

#include <GameObjectCreator.h>
#include <GameLevelsCreator.h>
#include <HealthIcon.h>
#include <DateTime.h>

Breakout::Breakout( HWND* consoleWindow )
    : Game( consoleWindow, BREAKOUT )
{
    statistic_.reset( new StatisticBreakout() );
    initializeGameObjects();
}


void Breakout::initializeGameObjects()
{
    levels_ = GameLevelsCreator::createGameLevels();

    player_ = std::make_unique< GameObject >(GameObjectCreator::createPerson( Vec2d( ( PMATRIX_WIDTH - PERSON_WIDTH) / 2, PMATRIX_HEIGHT - START_Y ), Vec2d( PERSON_WIDTH, PERSON_HEIGHT ) ) );

    ball_ = std::make_unique< BallObject >(
            Vec2d( ::PMATRIX_WIDTH / 2 - 10
                    , (::PMATRIX_HEIGHT - ::START_Y) / 2 )
            , Vec2f( 0.0f, 20.0f )
            , Color( 255, 255, 255 )
    );
}

void Breakout::objectsPositionUpdate()
{
    for (auto& object : levels_[statistic_->levelNo].getBricks())
    {
        if (!object.isDestroy())
        {
            objectPositionUpdate(object);
        }
    }

    objectPositionUpdate(*player_);
    objectPositionUpdate(*ball_);
}

void Breakout::draw()
{
    resetMainDigitalMatrix();
    healthIconDraw();
    objectsPositionUpdate();

    renderer_->draw();
}

void Breakout::handleInput()
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

void Breakout::healthChecker()
{
    if ( ball_->getPosition().y + ball_->getSize().x >= ::PMATRIX_HEIGHT - ::PMATRIX_BORDER )
    {
        --statistic_->healthPoint;

        ball_->reset();
        player_->reset();
        initTime_ = getMills();

        if (statistic_->healthPoint == 0)
        {
            gameStatus_ = STATUS_END;
            statistic_->lose = true;
        }
    }
}

bool Breakout::timeChecker()
{
    uint64_t now = getMills();

    if (now - initTime_ >= ::TIMEOUT_MS)
    {
        if (gameStatus_ == STATUS_WAIT)
        {
            startTime_ = now;
            gameStatus_ = STATUS_START;
        }

        statistic_->time = ((now - startTime_) / 1000);
    }
    else
    {
        statistic_->time = 3 - ((now - initTime_) / 1000);
        renderer_->setTimer(statistic_->time);
        return false;
    }

    renderer_->setTimer(statistic_->time);

    return true;
}

void Breakout::levelChecker()
{
    if (!levels_[statistic_->levelNo].getActiveBricks())
    {
        if (statistic_->levelNo < levels_.size() - 1)
        {
            ++statistic_->levelNo;
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

void Breakout::updateFrame()
{
    handleInput();
    handleLogic();
    draw();
}

void Breakout::handleLogic()
{
    renderer_->setScores(statistic_->scores);
    renderer_->setLevel(statistic_->levelNo + 1);

    healthChecker();

    if (timeChecker())
    {
        levelChecker();

        ball_->move();

        for (auto& object : levels_[statistic_->levelNo].getBricks())
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
                            statistic_->scores += object.getScores();
                            --levels_[statistic_->levelNo];
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

void Breakout::healthIconDraw()
{
    for (int i = 0; i < statistic_->healthPoint; ++i)
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

void Breakout::checkGameEnd( bool* game )
{
    if (gameStatus_ == STATUS_END)
    {
        *game = false;
        draw();
    }
}
