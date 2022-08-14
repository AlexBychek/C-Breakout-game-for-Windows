//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_PROJECT_PONG_H
#define BREAKOUT_PROJECT_PONG_H

#include <Game.h>

class Pong : public Game
{
private:
    std::unique_ptr<GameObject> player2_;
public:
    Pong() = default;
    ~Pong();

    Pong( HWND* consoleWindow );

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
    void levelChecker() override;
};


#endif //BREAKOUT_PROJECT_PONG_H
