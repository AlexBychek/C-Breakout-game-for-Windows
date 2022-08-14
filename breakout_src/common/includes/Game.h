//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include <GameLevel.h>
#include <Renderer.h>
#include <BallObject.h>
#include <Statistic.h>

#include <memory>

enum Directions { DIRECTION_UP, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT };

class Game
{
protected:
    enum Status { STATUS_START, STATUS_END, STATUS_WAIT };

    Status                      gameStatus_;

    std::unique_ptr<Renderer>   renderer_;
    std::shared_ptr<Statistic>  statistic_;

    std::unique_ptr<GameObject> player_;
    std::unique_ptr<BallObject> ball_;
    GameLevels                  levels_;

    uint64_t                    initTime_;
    uint64_t                    startTime_;

public:
    Game() = default;
    virtual ~Game();

    Game( HWND* consoleWindow, const GameType& gameType );

    virtual void updateFrame() = 0;
    virtual void checkGameEnd( bool* game ) = 0;
    virtual std::shared_ptr<Statistic> getStatistic();
protected:
    virtual void draw() = 0;
    virtual void handleInput() = 0;
    virtual void handleLogic() = 0;

    virtual void playerMoveUp( std::unique_ptr<GameObject>& player );
    virtual void playerMoveDown( std::unique_ptr<GameObject>& player );
    virtual void playerMoveLeft( std::unique_ptr<GameObject>& player );
    virtual void playerMoveRight( std::unique_ptr<GameObject>& player );

    virtual void initializeGameObjects() = 0;
    virtual void resetMainDigitalMatrix();

    virtual void objectPositionUpdate( GameObject& obj );
    virtual void objectsPositionUpdate() = 0;

    virtual void healthIconDraw() = 0;
    virtual void healthChecker() = 0;
    virtual bool timeChecker();
    virtual void levelChecker() = 0;

    virtual std::tuple< bool, Directions, Vec2f > checkCollision(BallObject& one, GameObject& two );
};

Vec2f normalize( Vec2f a );
Directions VectorDirection( Vec2f target );

#endif //BREAKOUT_GAME_H
