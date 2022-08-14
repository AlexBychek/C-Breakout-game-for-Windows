//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_RENDERER_H
#define BREAKOUT_RENDERER_H

#include <Config.h>

#include <Windows.h>
#include <stdint.h>

enum GameType { PONG, BREAKOUT };

class Renderer
{
    uint32_t   time_;
    uint32_t   scores_;
    uint32_t   level_;

    uint32_t player1Scores_;
    uint32_t player2Scores_;

    GameType gameType_;

public:
    HWND*      mPConsoleWindow;
    HDC        deviceContext;
    BITMAPINFO bitmapInfo;
    void*      m_p_bitmapMemory;

    COLORREF   mainDigitalMatrix[PMATRIX_WIDTH * PMATRIX_HEIGHT];
public:
    Renderer() = default;
    ~Renderer();

    Renderer( HWND* consoleWindow, const GameType& gameType );

    void draw();
    void setTimer(const uint32_t& time );
    void setScores(const uint32_t& scores );
    void setLevel(const uint32_t& level );

    void setPlayer1Scores(const uint32_t& scores );
    void setPlayer2Scores(const uint32_t& scores );
};


#endif //BREAKOUT_RENDERER_H
