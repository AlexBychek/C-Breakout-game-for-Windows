//
// Created by https://github.com/AlexBychek
//

#include <UI.h>
#include <Game.h>
#include <DateTime.h>

#include <iostream>
#include <fstream>
#include <conio.h>

#define ASCII_ZERO 48

using namespace std;

int showMenu()
{
    cout << " ***** Breakout **** " << endl;
    cout << " * 1 New Game      * " << endl;
    cout << " * 2 Statistic     * " << endl;
    cout << " * 3 Exit          * " << endl;
    cout << " ******************* " << endl;

    int choise = _getch();
    return choise - ASCII_ZERO;
}

void clearScreen()
{
    system("cls");
}

void showGameTitle()
{
    cout << endl << "   Breakout" << endl << endl;
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

void writeGameResult( const Statistic& statistic )
{
    string game_status = (statistic.lose == true ? "lose" : "win");
    cout << " status: " << game_status << endl;
    cout << " level:  " << to_string(statistic.levelNo + 1) << endl;
    cout << " scores: " << to_string(statistic.scores) <<  endl;
    cout << " health: " << to_string(statistic.healthPoint) << endl;
    cout << " time:   " << to_string(statistic.time) <<  endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();

    string results = "results.txt";

    ofstream vmdelet_out;
    vmdelet_out.open(results, ios::app);
    vmdelet_out << getDateTime()
    << "\n    " << game_status
    << "\n    levels completed: "<< statistic.levelNo
    << "\n    points scored:    "  << statistic.scores
    << "\n    game time:        " << statistic.time << endl;
    vmdelet_out.close();
}