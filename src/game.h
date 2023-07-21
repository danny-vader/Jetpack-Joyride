#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <bits/stdc++.h>
using namespace std;

#include "game_object.h"

enum GameState
{
    GAME_MENU,
    GAME_ACTIVE,
    GAME_WIN,
    GAME_LOSE
};

static glm::vec2 CHARACTER_SIZE(100.0f, 100.0f);
static int CHARACTER_VELOCITY_UP(1.0f);
static int CHARACTER_VELOCITY_DOWN(1.0f);
static int CHARACTER_ACCELERATION_UP(1.0f);
static int CHARACTER_ACCELERATION_DOWN(1.0f);
static int BG_OFFSET = 0;
static glm::vec2 COIN_SIZE(50.0f, 50.0f);
static float COIN_VELOCITY(1.0f);
static glm::vec2 ZAPPER_SIZE(50.0f, 250.0f);
static float ZAPPER_VELOCITY(1.0f);
static int CoinsCollected = 0;
static int DistanceTravelled = 0;
static int Level = 1;

class Game
{
public:
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    Game(unsigned int width, unsigned int height);
    ~Game();
    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisionsCoins();
    void DoCollisionsZappers();
};

#endif