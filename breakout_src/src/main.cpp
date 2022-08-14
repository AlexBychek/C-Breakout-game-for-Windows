//
// Created by https://github.com/AlexBychek
//

#define __WIN32_WINNT 0x0A00

#include <iostream>

#include <stdio.h>
#include <Windows.h>
#include <thread>
#include <optional>
#include <ctype.h>

#include <Breakout.h>
#include <Pong.h>
#include <UI.h>

int paddingWidth = 74;
int paddingHeight = 130;

using namespace std;

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
                clearScreen();
                showPongTitle();

                bool game = true;
                std::unique_ptr<Game> aGame( new Pong(&consoleWindow) );

                while (game)
                {
                    aGame->updateFrame();
                    aGame->checkGameEnd(&game);
                    this_thread::sleep_for( 50ms );
                }

                statistic = aGame->getStatistic();

                if (statistic)
                {
                    clearScreen();
                    statistic->writeGameResult();
                }
            } break;
            case 2:
            {
                clearScreen();
                showBreakoutTitle();

                bool game = true;
                std::unique_ptr<Game> aGame( new Breakout(&consoleWindow) );

                while (game)
                {
                    aGame->updateFrame();
                    aGame->checkGameEnd(&game);
                    this_thread::sleep_for( 50ms );
                }

                statistic = aGame->getStatistic();

                if (statistic)
                {
                    clearScreen();
                    statistic->writeGameResult();
                }
            }
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
