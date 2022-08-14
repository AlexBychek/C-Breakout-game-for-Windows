//
// Created by https://github.com/AlexBychek
//

#include <GameLevel.h>
#include <GameObjectCreator.h>
#include <Config.h>

#include <fstream>
#include <sstream>

GameLevel::GameLevel()
    : activeBricks_( 0 )
{
}

void GameLevel::loadLevel( const std::string& source, const bool& fromFile )
{
    bricks_.clear();
    uint32_t brickCode;
    std::vector<std::vector<uint32_t>> brickData;
    std::string line;
    std::unique_ptr< std::istream > stream;

    if (fromFile)
    {
        stream.reset( new std::ifstream(source ) );
    }
    else
    {
        stream.reset( new std::istringstream(source ) );
    }

    if (*stream)
    {
        while (std::getline( *stream, line ))
        {
            std::istringstream sstream(line);
            std::vector<uint32_t> row;
            while (sstream >> brickCode)
            {
                row.push_back(brickCode);
            }
            brickData.push_back(row);
        }

        if (!brickData.empty())
        {
            initializeBricks(brickData);
        }

    }

}

void GameLevel::initializeBricks(std::vector<std::vector<uint32_t>>& brickData )
{

    uint32_t dimensionHeight = brickData.size() > BRICK_MAX_HEIGHT ? BRICK_MAX_HEIGHT : brickData.size();
    uint32_t dimensionWidth;

    for (int y = 0; y < dimensionHeight; ++y)
    {
        dimensionWidth  = brickData[y].size() > BRICK_MAX_WIDTH ? BRICK_MAX_WIDTH : brickData[y].size();

        int all_brick_width = dimensionWidth * (BRICK_WIDTH + BRICK_BORDER);
        int offset_w = (PMATRIX_WIDTH - all_brick_width) / 2;

        for (int x = 0; x < dimensionWidth; ++x)
        {
            Vec2d pos( offset_w + (BRICK_WIDTH * x) + x * BRICK_BORDER,
                       START_Y + (BRICK_HEIGHT * y) + y * BRICK_BORDER + 20 );

            auto brick = GameObjectCreator::createBrick(brickData[y][x], pos);

            if (!brick.isSolid())
            {
                ++activeBricks_;
            }

            bricks_.emplace_back(brick );
        }
    }
}

uint32_t GameLevel::getActiveBricks() const
{
    return activeBricks_;
}

Bricks& GameLevel::getBricks()
{
    return bricks_;
}

void GameLevel::operator--()
{
    --activeBricks_;
}

TEST_F(TestGameLevel, test_game_level)
{
    gameLevel->loadLevel( "0 0 0\n0 0 0\n 0 0 0", false );

    ASSERT_EQ(gameLevel->getBricks().size(), 9);
    gameLevel->getBricks()[0].destroy();
    --*gameLevel;
    ASSERT_EQ(gameLevel->getActiveBricks(), 8);
}