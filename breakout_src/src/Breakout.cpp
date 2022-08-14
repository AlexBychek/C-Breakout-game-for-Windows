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
    renderer_->setStatistic( statistic_ );
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
    for ( auto& object : levels_[statistic_->levelNo].getBricks() )
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
    if ( GetAsyncKeyState(VK_LEFT) )
    {
        playerMoveLeft( player_ );
    }
    else if ( GetAsyncKeyState(VK_RIGHT) )
    {
        playerMoveRight( player_ );
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

        if ( statistic_->healthPoint == 0 )
        {
            gameStatus_ = STATUS_END;
            statistic_->lose = true;
        }
    }
}

void Breakout::levelChecker()
{
    if (!levels_[statistic_->levelNo].getActiveBricks())
    {
        if ( statistic_->levelNo < levels_.size() - 1 )
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
    healthChecker();

    if ( gameStatus_ == STATUS_END )
    {
        return;
    }

    if ( timeChecker() )
    {
        levelChecker();

        ball_->move();

        for (auto& object : levels_[statistic_->levelNo].getBricks())
        {
            if ( !object.isDestroy() )
            {
                std::tuple< bool, Directions, Vec2f > collision = checkCollision(*ball_, object);

                auto [ oCollision, oDirections, oDifference ] = checkCollision(*ball_, object);

                if ( oCollision )
                {
                    if (object.getHealthPoint() > 1)
                    {
                        if ( oDirections == DIRECTION_LEFT || oDirections == DIRECTION_RIGHT )
                        {
                            if (object.getHealthPoint() > 2)
                            {
                                ball_->setYVelocity( -ball_->getVelocity().y );
                            }
                            ball_->setXVelocity( -ball_->getVelocity().x );
                            float penetration = ball_->getRadius() - std::abs(oDifference.x);

                            if ( oDirections == DIRECTION_LEFT )
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
                            float penetration = ball_->getRadius() - std::abs(oDifference.y);
                            if ( oDirections == DIRECTION_UP )
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

        auto [ pCollision, pDirections, pDifference  ] = checkCollision(*ball_, *player_);

        if ( pCollision )
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
    for ( int i = 0; i < statistic_->healthPoint; ++i )
    {
        int healthStartPositionX = 20 + i * 20;

        for ( int y = 0; y < 20; ++y )
        {
            for ( int x = 0; x < 20; ++x )
            {
                if ( healthIconPatternArray[20 * y + x ] != 0 )
                {
                    renderer_->mainDigitalMatrix[(y + 20) * ::PMATRIX_WIDTH + healthStartPositionX + x] = RGB(255, 0, 0);
                }
            }
        }
    }
}

void Breakout::checkGameEnd( bool* game )
{
    if ( gameStatus_ == STATUS_END )
    {
        *game = false;
        draw();
    }
}
