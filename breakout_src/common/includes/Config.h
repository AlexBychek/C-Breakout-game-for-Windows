//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_CONFIG_H
#define BREAKOUT_CONFIG_H

#include <Common.h>

#include <vector>
#include <string>

static constexpr int PMATRIX_WIDTH    = 800;
static constexpr int PMATRIX_HEIGHT   = 600;

static constexpr int BRICK_WIDTH      = 50;
static constexpr int BRICK_HEIGHT     = 20;
static constexpr int BRICK_BORDER     = 5;
static constexpr int BRICK_MAX_WIDTH  = 14;
static constexpr int BRICK_MAX_HEIGHT = 7;

static constexpr int PERSON_WIDTH     = 100;
static constexpr int PERSON_HEIGHT    = 20;

static constexpr int BALL_RADIUS      = 10;

static constexpr int TIMEOUT_MS       = 3000;

static constexpr int DASHBOARD_HEIGHT = 50;
static constexpr int PMATRIX_BORDER   = 10;

static constexpr int START_X          = PMATRIX_BORDER;
static constexpr int START_Y          = DASHBOARD_HEIGHT + PMATRIX_BORDER * 2;

static constexpr int OFFSET_LEFT      = 20;
static constexpr int OFFSET_TOP       = 40;
static constexpr int BYTES_PER_PIXEL  = 4;

static constexpr int PONG_WIN_COUNT   = 5;

static const Vec2f INITIAL_BALL_VELOCITY(10.0f, -20.0f);
static const int PLAYER_VELOCITY(25);

static std::vector< std::string > BASIC_LEVELS = { "0\n0 1 0\n0"
                                                 , "1 0 1\n0 1 0\n1 0 1"
                                                 , "2 2\n2 1 1 2\n2 1 1 2\n2 2" };

#endif //BREAKOUT_CONFIG_H
