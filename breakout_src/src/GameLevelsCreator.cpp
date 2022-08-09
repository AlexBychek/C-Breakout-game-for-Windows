//
// Created by https://github.com/AlexBychek
//

#include <GameLevelsCreator.h>
#include <Config.h>

#include <filesystem>

using namespace std;

GameLevels GameLevelsCreator::createGameLevels()
{
    GameLevels levels;

    std::string levelsDirectory = std::filesystem::current_path().string() + "\\levels\\";

    if (std::filesystem::exists( levelsDirectory ))
    {
        uint32_t levelId = 1;
        std::string currentLevelPath = levelsDirectory + "level" + std::to_string(levelId ) + ".txt";

        while (std::filesystem::exists( currentLevelPath ))
        {
            GameLevel lvl;
            lvl.loadLevel(currentLevelPath, true);
            levels.emplace_back( lvl );
            currentLevelPath = levelsDirectory + "level" + std::to_string(++levelId ) + ".txt";
        }
    }

    if ( levels.empty() )
    {
        for (auto& stringLvl : BASIC_LEVELS)
        {
            GameLevel lvl;
            lvl.loadLevel(stringLvl, false);
            levels.emplace_back( lvl );
        }
    }

    return levels;
}
