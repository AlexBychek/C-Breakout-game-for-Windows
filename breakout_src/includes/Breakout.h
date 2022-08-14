//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_PROJECT_BREAKOUT_H
#define BREAKOUT_PROJECT_BREAKOUT_H

#include <Game.h>

class Breakout : public Game
{
public:
    Breakout() = default;
    ~Breakout() = default;

    Breakout( HWND* consoleWindow );

    void updateFrame() override;
    void checkGameEnd( bool* game ) override;
protected:
    void draw() override;
    void handleInput() override;
    void handleLogic() override;

    void initializeGameObjects() override;
    void objectsPositionUpdate() override;

    void healthIconDraw() override;
    void healthChecker() override;
    bool timeChecker() override;
    void levelChecker() override;
};


#endif //BREAKOUT_PROJECT_BREAKOUT_H
