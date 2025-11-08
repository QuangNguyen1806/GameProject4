#include "LevelB.h"

LevelB::LevelB() : Scene { {0.0f} } {}

LevelB::LevelB(Vector2 origin) : Scene { origin } {}

LevelB::~LevelB() { shutdown(); }

void LevelB::initialise() {
    mGameState.nextSceneID = 0;

    // Load audio
    mGameState.bgm = LoadMusicStream("assets/background.wav");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);
    mGameState.deathSound = LoadSound("assets/hit.wav");
    mGameState.jumpSound = LoadSound("assets/jump.wav");
    mGameState.sceneChangeSound = LoadSound("assets/win.wav");

    // Create map
    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT,
        (unsigned int *) mLevelData,
        "assets/tileset.png",
        TILE_DIMENSION, 2, 2,
        mOrigin
    );

    // Create player
    mGameState.soldier = new Entity(
        {100.0f, 520.0f},
        {32.0f, 32.0f},
        "assets/player.png",
        SINGLE,
        {1, 1},
        {},
        PLAYER
    );
    mGameState.soldier->setSpeed(300);
    mGameState.soldier->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.soldier->setJumpingPower(400.0f);

    // Create enemy - FOLLOWER type (enemy3.png)
    // Follower chases the player
    mEnemyEntity = new Entity(
        {500.0f, 480.0f},
        {32.0f, 32.0f},
        "assets/enemy3.png",
        SINGLE,
        {1, 1},
        {},
        NPC
    );
    mEnemyEntity->setAIType(FOLLOWER);
    mEnemyEntity->setSpeed(120);
    mEnemyEntity->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mEnemyEntity->activate();

    // Initialize camera
    mGameState.camera = { 0 };
    mGameState.camera.target = mGameState.soldier->getPosition();
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void LevelB::update(float deltaTime) {
    
    // Update player
    mGameState.soldier->update(
        deltaTime,
        nullptr,
        mGameState.map,
        mEnemyEntity,
        1
    );

    Vector2 playerPos = mGameState.soldier->getPosition();
    panCamera(&mGameState.camera, &playerPos);

    // Update enemy (FOLLOWER chases player)
    mEnemyEntity->update(
        deltaTime,
        mGameState.soldier,
        mGameState.map,
        nullptr,
        0
    );

    // ===== SOUND EFFECTS =====
    if (mGameState.soldier->isJumping() && mGameState.soldier->isCollidingBottom()) {
        PlaySound(mGameState.jumpSound);
    }

        // ===== COLLISION DETECTION: Player touched enemy =====
    // Use CheckCollisionRecs for precise rectangle collision detection
    Rectangle playerRect = {
        mGameState.soldier->getPosition().x - mGameState.soldier->mScale.x / 2,
        mGameState.soldier->getPosition().y - mGameState.soldier->mScale.y / 2,
        mGameState.soldier->mScale.x,
        mGameState.soldier->mScale.y
    };
    
    Rectangle enemyRect = {
        mEnemyEntity->getPosition().x - mEnemyEntity->mScale.x / 2,
        mEnemyEntity->getPosition().y - mEnemyEntity->mScale.y / 2,
        mEnemyEntity->mScale.x,
        mEnemyEntity->mScale.y
    };
    
    if (CheckCollisionRecs(playerRect, enemyRect)) {
        PlaySound(mGameState.deathSound);
        mGameState.lives--;
        if (mGameState.lives <= 0) {
            mGameState.gameOver = true;
        } else {
            mGameState.nextSceneID = 1; // Restart LevelA
        }
        return;
    }

    // ===== FALL OFF WORLD CHECK =====
    if (mGameState.soldier->getPosition().y > 650.0f) {
        PlaySound(mGameState.deathSound);
        mGameState.lives--;
        if (mGameState.lives <= 0) {
            mGameState.gameOver = true;
        } else {
            mGameState.nextSceneID = 2; // Restart LevelC
        }
        return;
    }

    // ===== LEVEL COMPLETION CHECK =====
    if (mGameState.soldier->getPosition().x > 850.0f) {
        mGameState.nextSceneID = 3; // Go to LevelC
    }
}

void LevelB::render() {
    ClearBackground(BLACK);
    BeginMode2D(mGameState.camera);
    mGameState.map->render();
    mGameState.soldier->render();
    mEnemyEntity->render();
    EndMode2D();
}

void LevelB::shutdown() {
    delete mGameState.soldier;
    delete mGameState.map;
    delete mEnemyEntity;
    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.deathSound);
    UnloadSound(mGameState.jumpSound);
    UnloadSound(mGameState.sceneChangeSound);
}