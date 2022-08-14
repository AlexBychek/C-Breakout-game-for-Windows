//
// Created by https://github.com/AlexBychek
//

#include "Pong.h"

#include <GameObjectCreator.h>
#include <GameLevelsCreator.h>
#include <HealthIcon.h>
#include <DateTime.h>

Pong::Pong( HWND* consoleWindow )
        : Game( consoleWindow, PONG )
{
    statistic_.reset( new StatisticPong() );
    initializeGameObjects();
}

Pong::~Pong()
{
    player2_ = nullptr;
}

void Pong::initializeGameObjects()
{
    levels_ = GameLevelsCreator::createGameLevels();

    player_ = std::make_unique< GameObject >(GameObjectCreator::createPerson( Vec2d( ( PMATRIX_WIDTH - 50), (PMATRIX_HEIGHT - START_Y) / 2 ), Vec2d( 20, 100 ) ) );

    player2_ = std::make_unique< GameObject >(GameObjectCreator::createPerson( Vec2d( 50 - 20, (PMATRIX_HEIGHT - START_Y) / 2 ), Vec2d( 20, 100 ) ) );

    ball_ = std::make_unique< BallObject >(
              Vec2d( ::PMATRIX_WIDTH / 2 - 10, player_->getPosition().y + (player_->getSize().y / 2) - ::BALL_RADIUS )
            , Vec2f( 20.0f, 0.0f )
            , Color( 255, 255, 255 )
    );
}

void Pong::objectsPositionUpdate()
{
    objectPositionUpdate(*player_);
    objectPositionUpdate(*player2_);
    objectPositionUpdate(*ball_);
}

void Pong::draw()
{
    resetMainDigitalMatrix();
    objectsPositionUpdate();

    renderer_->draw();
}

void Pong::handleInput()
{
    int predictPosition;
    if ( GetAsyncKeyState(VK_NUMPAD8) )
    {
        predictPosition = player_->getPosition().y - player_->getVelocity().y;

        if (predictPosition >= ::START_Y)
        {
            player_->setYPosition( predictPosition );
        }
    }
    else if ( GetAsyncKeyState(VK_NUMPAD2) )
    {
        predictPosition = (player_->getPosition().y) + player_->getVelocity().y;

        if (predictPosition <= (::PMATRIX_HEIGHT - 30) - player_->getSize().y)
        {
            player_->setYPosition( predictPosition );
        }
    }

    if ( GetAsyncKeyState(VK_UP) )
    {
        predictPosition = player2_->getPosition().y - player2_->getVelocity().y;

        if (predictPosition >= ::START_Y)
        {
            player2_->setYPosition( predictPosition );
        }
    }
    else if ( GetAsyncKeyState(VK_DOWN) )
    {
        predictPosition = (player2_->getPosition().y) + player2_->getVelocity().y;

        if (predictPosition <= (::PMATRIX_HEIGHT - 30) - player2_->getSize().y)
        {
            player2_->setYPosition( predictPosition );
        }
    }

}

void Pong::healthChecker()
{

    if (ball_->getPosition().x + ball_->getSize().x >= ::PMATRIX_WIDTH - ::PMATRIX_BORDER)
    {
        ball_->reset();
        player_->reset();
        player2_->reset();
        initTime_ = getMills();
        ++statistic_->player1Scores;
    }
    else if ( ball_->getPosition().x <= ::PMATRIX_BORDER )
    {
        ball_->reset();
        player_->reset();
        player2_->reset();
        initTime_ = getMills();
        ++statistic_->player2Scores;
    }

    if (statistic_->player2Scores >= 2 || statistic_->player1Scores >= 2)
    {
        gameStatus_ = STATUS_END;
    }
}

bool Pong::timeChecker()
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

void Pong::levelChecker()
{
}

void Pong::updateFrame()
{
    handleInput();
    handleLogic();
    draw();
}

void Pong::handleLogic()
{
    healthChecker();

    renderer_->setPlayer1Scores( statistic_->player1Scores );
    renderer_->setPlayer2Scores( statistic_->player2Scores );

    if (timeChecker())
    {
        ball_->move();

        std::tuple< bool, Directions, Vec2f > result = checkCollision(*ball_, *player_);

        if (std::get<0>(result))
        {
            float centerBoard = player_->getPosition().y + player_->getSize().y / 2.0f;
            float distance = (ball_->getPosition().y + ball_->getRadius()) - centerBoard;
            float percentage = distance / (FLOAT(player_->getSize().y) / 2.0f);

            float strength = 2.0f;
            Vec2f oldVelocity = ball_->getVelocity();

            ball_->setXVelocity( -ball_->getVelocity().x );
            ball_->setYVelocity( FLOAT(-::INITIAL_BALL_VELOCITY.y) * percentage * strength );
            ball_->setYVelocity( normalize(ball_->getVelocity()).y * lengthVector(oldVelocity) );
        }

        std::tuple< bool, Directions, Vec2f > result2 = checkCollision(*ball_, *player2_);

        if (std::get<0>(result2))
        {
            float centerBoard = player2_->getPosition().y + player2_->getSize().y / 2.0f;
            float distance = (ball_->getPosition().y + ball_->getRadius()) - centerBoard;
            float percentage = distance / (FLOAT(player2_->getSize().y) / 2.0f);

            float strength = 2.0f;
            Vec2f oldVelocity = ball_->getVelocity();

            ball_->setXVelocity( -ball_->getVelocity().x );
            ball_->setYVelocity( FLOAT(-::INITIAL_BALL_VELOCITY.y) * percentage * strength );
            ball_->setYVelocity( normalize(ball_->getVelocity()).y * lengthVector(oldVelocity) );
        }
    }
}

void Pong::healthIconDraw()
{
}

void Pong::checkGameEnd( bool* game )
{
    if (gameStatus_ == STATUS_END)
    {
        *game = false;
        draw();
    }
}