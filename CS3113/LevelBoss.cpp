/**
* Author: [Your name here]
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "LevelBoss.h"

unsigned int LevelBoss::levelData[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

LevelBoss::LevelBoss()
{
    initialize();
}

LevelBoss::~LevelBoss() {}

void LevelBoss::initialize()
{
    mMap = new Map(20, 10, levelData, "assets/tileset.png", 64.0f, 4, 4, {640, 360});

    mPlayer = Entity({320, 300}, {64, 64}, "assets/player.png", PLAYER);
    mPlayer.setSpeed(200);
    mPlayer.setJumpingPower(450.0f);
    mPlayer.setColliderDimensions({50, 50});
    mPlayer.setAcceleration({0.0f, 800.0f});

    // Boss: Large enemy with unique behavior
    mBoss = Entity({900, 300}, {100, 100}, "assets/boss.png", NPC);
    mBoss.setAIType(WANDERER);
    mBoss.setSpeed(150);
    mBoss.setColliderDimensions({80, 80});
    mBoss.setAcceleration({0.0f, 600.0f});

    mEnemyCount = 1;

    mCamera = {0};
    mCamera.target = mPlayer.getPosition();
    mCamera.offset = {(float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f};
    mCamera.zoom = 1.0f;

    mIsComplete = false;
}

void LevelBoss::update(float deltaTime)
{
    // Player input
    mPlayer.resetMovement();
    if (IsKeyDown(KEY_LEFT)) mPlayer.moveLeft();
    if (IsKeyDown(KEY_RIGHT)) mPlayer.moveRight();
    if (IsKeyPressed(KEY_SPACE)) mPlayer.jump();

    // Update player
    mPlayer.update(deltaTime, &mPlayer, mMap, &mBoss, 1);

    // Update boss
    if (mBoss.isActive())
    {
        mBoss.update(deltaTime, &mPlayer, mMap, nullptr, 0);
    }

    // Pan camera
    Vector2 playerPos = mPlayer.getPosition();
    panCamera(&mCamera, &playerPos);
}

void LevelBoss::render()
{
    BeginMode2D(mCamera);
    mMap->render();
    mPlayer.render();
    mBoss.render();
    EndMode2D();
}