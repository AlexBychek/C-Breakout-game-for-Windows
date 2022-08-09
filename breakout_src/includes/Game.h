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

class Statistic;

class Game
{
    enum Status { STATUS_START, STATUS_END, STATUS_WAIT };

    Status                      gameStatus_;

    std::unique_ptr<Renderer>   renderer_;
    Statistic                   statistic_;

    std::unique_ptr<GameObject> player_;
    std::unique_ptr<BallObject> ball_;
    GameLevels                  levels_;

    uint64_t                    initTime_;
    uint64_t                    startTime_;

public:
    Game() = default;
    ~Game();

    Game( HWND* consoleWindow );

    void updateFrame();
    Statistic checkGameEnd( bool* game );

private:
    void draw();
    void handleInput();
    void handleLogic();

    void initializeGameObjects();
    void resetMainDigitalMatrix();

    void objectPositionUpdate( GameObject& obj );
    void objectsPositionUpdate();

    void healthIconDraw();
    void healthChecker();
    bool timeChecker();
    void levelChecker();

    std::tuple< bool, Directions, Vec2f > checkCollision(BallObject& one, GameObject& two );
};

#endif //BREAKOUT_GAME_H
