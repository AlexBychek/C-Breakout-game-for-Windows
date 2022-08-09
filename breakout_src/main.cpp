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

#include <Game.h>
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

    std::optional<Statistic> statistic;

    bool exit = false;

    while (!exit)
    {
        switch (showMenu())
        {
            case 1:
            {
                clearScreen();
                showGameTitle();

                bool game = true;
                std::unique_ptr<Game> aGame( new Game(&consoleWindow) );

                while (game)
                {
                    aGame->updateFrame();
                    statistic = Statistic(aGame->checkGameEnd(&game));
                    this_thread::sleep_for( 50ms );
                }

                if (statistic)
                {
                    clearScreen();
                    writeGameResult(*statistic);
                    statistic = nullopt;
                }
            } break;
            case 2:
            {
                clearScreen();
                showStatistic();
            } break;
            case 3:
            {
                exit = true;
            } break;
        };

        clearScreen();
    }
}
