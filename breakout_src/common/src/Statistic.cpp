//
// Created by https://github.com/AlexBychek
//

#include <Statistic.h>
#include <DateTime.h>

#include <string>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>

using namespace std;

Statistic::Statistic()
        : lose( false )
        , scores( 0 )
        , healthPoint( 3 )
        , time( 0 )
        , levelNo( 0 )
        , player1Scores( 0 )
        , player2Scores( 0 )
{
}

void StatisticBreakout::writeGameResult()
{
    string game_status = (lose == true ? "lose" : "win");
    cout << " status: " << game_status << endl;
    cout << " level:  " << to_string(levelNo + 1) << endl;
    cout << " scores: " << to_string(scores) <<  endl;
    cout << " health: " << to_string(healthPoint) << endl;
    cout << " time:   " << to_string(time) <<  endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();

    string results = "results.txt";

    ofstream vmdelet_out;
    vmdelet_out.open(results, ios::app);
    vmdelet_out << getDateTime()
                << "\n    " << game_status
                << "\n    levels completed: "  << levelNo
                << "\n    points scored:    "  << scores
                << "\n    game time:        "  << time << endl;
    vmdelet_out.close();
}

void StatisticPong::writeGameResult()
{
    string game_status = player1Scores > player2Scores ? "player 1 Win" : " player 2 Win";
    cout << " status:   " << game_status << endl;
    cout << " player 1: " << to_string(player1Scores) << " scores" <<  endl;
    cout << " player 2: " << to_string(player2Scores) << " scores" <<  endl;
    cout << " time:     " << to_string(time) <<  endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();

    string results = "results.txt";

    ofstream vmdelet_out;
    vmdelet_out.open(results, ios::app);
    vmdelet_out << getDateTime()
                << "\n    " << game_status
                << "\n    player 1 scores:  "  << player1Scores
                << "\n    player 2 scores:  "  << player2Scores
                << "\n    game time:        "  << time << endl;
    vmdelet_out.close();

}
