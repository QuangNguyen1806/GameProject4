/**
* Author: [Your name here]
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "LevelA.h"

unsigned int LevelA::levelData[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

LevelA::LevelA()
{
    initialize();
}

LevelA::~LevelA() {}

void LevelA::initialize()
{
    mMap = new Map(20, 10, levelData, "assets/tileset.png", 64.0f, 4, 4, {640, 360});

    mPlayer = Entity({320, 300}, {64, 64}, "assets/player.jpg", PLAYER);
    mPlayer.setSpeed(200);
    mPlayer.setJumpingPower(450.0f);
    mPlayer.setColliderDimensions({50, 50});
    mPlayer.setAcceleration({0.0f, 800.0f});

    // Enemy 1: Wanderer
    mEnemies[0] = Entity({200, 300}, {64, 64}, "assets/enemy.png", NPC);
    mEnemies[0].setAIType(WANDERER);
    mEnemies[0].setSpeed(100);
    mEnemies[0].setColliderDimensions({50, 50});
    mEnemies[0].setAcceleration({0.0f, 800.0f});

    // Enemy 2: Follower
    mEnemies[1] = Entity({800, 300}, {64, 64}, "assets/enemy2.png", NPC);
    mEnemies[1].setAIType(FOLLOWER);
    mEnemies[1].setSpeed(80);
    mEnemies[1].setColliderDimensions({50, 50});
    mEnemies[1].setAcceleration({0.0f, 800.0f});

    // Enemy 3: Flyer (Wanderer with no gravity)
    mEnemies[2] = Entity({600, 200}, {64, 64}, "assets/enemy3.png", NPC);
    mEnemies[2].setAIType(WANDERER);
    mEnemies[2].setSpeed(120);
    mEnemies[2].setColliderDimensions({50, 50});
    mEnemies[2].setAcceleration({0.0f, 0.0f});

    mEnemyCount = 3;

    mCamera = {0};
    mCamera.target = mPlayer.getPosition();
    mCamera.offset = {(float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f};
    mCamera.zoom = 1.0f;

    mIsComplete = false;
}

void LevelA::update(float deltaTime)
{
    // Player input
    mPlayer.resetMovement();
    if (IsKeyDown(KEY_LEFT)) mPlayer.moveLeft();
    if (IsKeyDown(KEY_RIGHT)) mPlayer.moveRight();
    if (IsKeyPressed(KEY_SPACE)) mPlayer.jump();

    // Update player
    mPlayer.update(deltaTime, &mPlayer, mMap, mEnemies, mEnemyCount);

    // Update enemies
    for (int i = 0; i < mEnemyCount; i++)
    {
        if (mEnemies[i].isActive())
        {
            mEnemies[i].update(deltaTime, &mPlayer, mMap, mEnemies, mEnemyCount);
        }
    }

    // Pan camera
    Vector2 playerPos = mPlayer.getPosition();
    panCamera(&mCamera, &playerPos);
}

void LevelA::render()
{
    BeginMode2D(mCamera);
    mMap->render();
    mPlayer.render();
    for (int i = 0; i < mEnemyCount; i++)
    {
        mEnemies[i].render();
    }
    EndMode2D();
}