//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_GAMELEVEL_H
#define BREAKOUT_GAMELEVEL_H

#include <Brick.h>
#include <string>

class GameLevel
{
    Bricks   bricks_;
    uint32_t activeBricks_;
public:

    GameLevel();
    ~GameLevel() = default;

    void     loadLevel(const std::string& source, const bool& fromFile );
    Bricks&  getBricks();
    uint32_t getActiveBricks() const;

    void operator--();

private:
    void initializeBricks(std::vector<std::vector<uint32_t>>& brickData );
};

typedef std::vector<GameLevel> GameLevels;

class TestGameLevel : public ::testing::Test
{
protected:
    void SetUp()
    {
        gameLevel.reset( new GameLevel() );
    }
    void TearDown()
    {
        gameLevel = nullptr;
    }

    std::unique_ptr<GameLevel> gameLevel;
};

#endif //BREAKOUT_GAMELEVEL_H
