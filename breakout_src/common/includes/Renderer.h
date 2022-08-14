//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_RENDERER_H
#define BREAKOUT_RENDERER_H

#include <Config.h>
#include <Statistic.h>

#include <memory>
#include <Windows.h>
#include <stdint.h>

enum GameType { PONG, BREAKOUT };

class Renderer
{
    GameType                   gameType_;
    std::shared_ptr<Statistic> statistic_;
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
    void setStatistic( std::shared_ptr<Statistic> statistic );
};


#endif //BREAKOUT_RENDERER_H
