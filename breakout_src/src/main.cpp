//
// Created by https://github.com/AlexBychek
//

#define __WIN32_WINNT 0x0A00

#include <iostream>

#include <Windows.h>
#include <thread>
#include <optional>

#include <Breakout.h>
#include <Pong.h>
#include <UI.h>

int paddingWidth = 74;
int paddingHeight = 130;

using namespace std;

void gameProcess( GameType type, HWND* consoleWindow )
{
    clearScreen();
    showPongTitle();

    bool game = true;
    std::unique_ptr<Game> aGame;

    if ( type == PONG )
    {
        aGame.reset( new Pong(consoleWindow) );
    }
    else
    {
        aGame.reset( new Breakout(consoleWindow) );
    }

    while (game)
    {
        aGame->updateFrame();
        aGame->checkGameEnd(&game);
        this_thread::sleep_for( 50ms );
    }

    auto statistic = aGame->getStatistic();

    if (statistic)
    {
        clearScreen();
        statistic->writeGameResult();
    }

    clearScreen();
    showStatistic();
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HWND consoleWindow = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(consoleWindow, &ConsoleRect);
    MoveWindow(consoleWindow, ConsoleRect.left, ConsoleRect.top, PMATRIX_WIDTH + paddingWidth, PMATRIX_HEIGHT + paddingHeight, TRUE);

    std::shared_ptr<Statistic> statistic;

    bool exit = false;

    while (!exit)
    {
        switch (showMenu())
        {
            case 1:
            {
                gameProcess( PONG, &consoleWindow );
            } break;
            case 2:
            {
                gameProcess( BREAKOUT, &consoleWindow );
            } break;
            case 3:
            {
                clearScreen();
                showStatistic();
            } break;
            case 4:
            {
                exit = true;
            } break;
        };

        clearScreen();
    }
}
