//
// Created by https://github.com/AlexBychek
//

#include "Pong.h"
#include <GameObjectCreator.h>
#include <DateTime.h>

Pong::Pong( HWND* consoleWindow )
        : Game( consoleWindow, PONG )
{
    statistic_.reset( new StatisticPong() );
    renderer_->setStatistic( statistic_ );
    initializeGameObjects();
}

Pong::~Pong()
{
    player2_ = nullptr;
}

void Pong::initializeGameObjects()
{
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
    if ( GetAsyncKeyState(VK_NUMPAD8) )
    {
        playerMoveUp( player_ );
    }
    else if ( GetAsyncKeyState(VK_NUMPAD2) )
    {
        playerMoveDown( player_ );
    }
    if ( GetAsyncKeyState(VK_UP) )
    {
        playerMoveUp( player2_ );
    }
    else if ( GetAsyncKeyState(VK_DOWN) )
    {
        playerMoveDown( player2_ );
    }
}

void Pong::healthChecker()
{
    if ( ball_->getPosition().x + ball_->getSize().x >= ::PMATRIX_WIDTH - ::PMATRIX_BORDER )
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
        ball_->setXVelocity( -ball_->getVelocity().x );
        player2_->reset();
        initTime_ = getMills();
        ++statistic_->player2Scores;
    }

    if ( statistic_->player2Scores >= PONG_WIN_COUNT || statistic_->player1Scores >= PONG_WIN_COUNT )
    {
        gameStatus_ = STATUS_END;
    }
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

void updateBallVelocity( std::unique_ptr<GameObject>& player, std::unique_ptr<BallObject>& ball )
{
    float centerBoard = player->getPosition().y + player->getSize().y / 2.0f;
    float distance = (ball->getPosition().y + ball->getRadius()) - centerBoard;
    float percentage = distance / (FLOAT(player->getSize().y) / 2.0f);

    float strength = 2.0f;
    Vec2f oldVelocity = ball->getVelocity();

    ball->setXVelocity( -ball->getVelocity().x );
    ball->setYVelocity( FLOAT(-::INITIAL_BALL_VELOCITY.y) * percentage * strength );
    ball->setYVelocity( normalize(ball->getVelocity()).y * lengthVector(oldVelocity) );
}

void Pong::handleLogic()
{
    healthChecker();

    if ( gameStatus_ == STATUS_END )
    {
        return;
    }

    if ( timeChecker() )
    {
        ball_->move();

        auto [ pCollision, pDirections, pDifference  ] = checkCollision(*ball_, *player_);

        if ( pCollision )
        {
            updateBallVelocity(player_, ball_);
        }

        auto [ p2Collision2, p2Directions2, p2Difference ] = checkCollision(*ball_, *player2_);

        if ( p2Collision2 )
        {
            updateBallVelocity(player2_, ball_);
        }
    }
}

void Pong::healthIconDraw()
{
}

void Pong::checkGameEnd( bool* game )
{
    if ( gameStatus_ == STATUS_END )
    {
        *game = false;
        draw();
    }
}