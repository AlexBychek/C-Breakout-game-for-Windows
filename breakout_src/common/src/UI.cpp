//
// Created by https://github.com/AlexBychek
//

#include <UI.h>
#include <Game.h>

#include <iostream>
#include <fstream>
#include <conio.h>

#define ASCII_ZERO 48

using namespace std;

//#define DEBUG

int showMenu()
{
    cout << " ******* Breakout ****** " << endl;
    cout << " * 1 New Game Pong     * " << endl;
    cout << " * 2 New Game Breakout * " << endl;
    cout << " * 3 Statistic         * " << endl;
    cout << " * 4 Exit              * " << endl;
    cout << " *********************** " << endl;

#ifndef DEBUG
    int choise = _getch();
    return choise - ASCII_ZERO;
#else
    int key;
    cin >> key;
    return key;
#endif
}

void clearScreen()
{
    system("cls");
}

void showBreakoutTitle()
{
    cout << endl << "   Breakout" << endl << endl;
}

void showPongTitle()
{
    cout << endl << "   Pong" << endl << endl;
}


void showStatistic()
{
    fstream file;
    file.open("results.txt", ios::in);
    if (file)
    {
        string buf;
        while (getline(file, buf))
        {
            cout << buf << endl;
        }
    }

    _getch();
}