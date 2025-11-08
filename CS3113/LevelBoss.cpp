#include "LevelBoss.h"

LevelBoss::LevelBoss() : Scene { {0.0f} } {}

LevelBoss::LevelBoss(Vector2 origin) : Scene { origin } {}

LevelBoss::~LevelBoss() { shutdown(); }

void LevelBoss::initialise() {
    mGameState.nextSceneID = 0;
    mBossDefeated = false;
    mBossHealth = 5.0f;

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

    // Create boss - FOLLOWER type that chases player
    mBoss = new Entity(
        {700.0f, 500.0f},
        {48.0f, 48.0f},  // Bigger than normal enemies
        "assets/boss.png",
        SINGLE,
        {1, 1},
        {},
        NPC
    );
    mBoss->setAIType(FOLLOWER);  // Boss follows/chases player
    mBoss->setSpeed(100);
    mBoss->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mBoss->activate();

    // Initialize camera
    mGameState.camera = { 0 };
    mGameState.camera.target = mGameState.soldier->getPosition();
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void LevelBoss::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);
    if (mBossDefeated) return; // Stop updating if boss is defeated

    // Update player
    mGameState.soldier->update(
        deltaTime,
        nullptr,
        mGameState.map,
        mBoss,
        1
    );

    Vector2 playerPos = mGameState.soldier->getPosition();
    panCamera(&mGameState.camera, &playerPos);

    // Update boss
    mBoss->update(
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

    // ===== BOSS DEFEAT CONDITION: Player jumped on boss's head =====
Rectangle playerRect = {
    mGameState.soldier->getPosition().x - mGameState.soldier->mScale.x / 2,
    mGameState.soldier->getPosition().y - mGameState.soldier->mScale.y / 2,
    mGameState.soldier->mScale.x,
    mGameState.soldier->mScale.y
};

Rectangle bossRect = {
    mBoss->getPosition().x - mBoss->mScale.x / 2,
    mBoss->getPosition().y - mBoss->mScale.y / 2,
    mBoss->mScale.x,
    mBoss->mScale.y
};

// Check if rectangles collide AND player is above boss
if (CheckCollisionRecs(playerRect, bossRect)) {
    bool playerAboveBoss = mGameState.soldier->getPosition().y < mBoss->getPosition().y;
    
    // Check if player is moving downward (jumping on it)
    if (playerAboveBoss && mGameState.soldier->getVelocity().y > 50.0f) {
        mBossHealth -= 1.0f;
        PlaySound(mGameState.jumpSound);
        LOG("Boss HP: " << mBossHealth);
        
        if (mBossHealth <= 0) {
            mBossDefeated = true;
            PlaySound(mGameState.sceneChangeSound);
            mGameState.playerWon = true;
            return;
        }
    }
}

// ===== COLLISION DETECTION: Boss touched player from side =====
// Only take damage if player is NOT above the boss
if (mGameState.soldier->getPosition().y >= mBoss->getPosition().y) {
    if (CheckCollisionRecs(playerRect, bossRect)) {
        PlaySound(mGameState.deathSound);
        mGameState.lives--;
        if (mGameState.lives <= 0) {
            mGameState.gameOver = true;
        } else {
            mGameState.nextSceneID = 4; // Restart boss level
        }
        return;
    }
}


    // ===== FALL OFF WORLD CHECK =====
    if (mGameState.soldier->getPosition().y > 650.0f) {
        PlaySound(mGameState.deathSound);
        mGameState.lives--;
        if (mGameState.lives <= 0) {
            mGameState.gameOver = true;
        } else {
            mGameState.nextSceneID = 4; // Restart boss level
        }
        return;
    }
}

void LevelBoss::render() {
    ClearBackground(BLACK);
    
    BeginMode2D(mGameState.camera);
    mGameState.map->render();
    mGameState.soldier->render();
    mBoss->render();
    EndMode2D();
    
    // Draw boss HP bar (world space, follows camera)
    if (mBossHealth > 0 && !mBossDefeated) {
        int barWidth = (int)(100 * (mBossHealth / 5.0f));
        Vector2 bossPos = mBoss->getPosition();
        Vector2 screenPos = GetWorldToScreen2D(bossPos, mGameState.camera);
        DrawRectangle((int)screenPos.x - 50, (int)screenPos.y - 60, barWidth, 15, RED);
    }
}

void LevelBoss::shutdown() {
    delete mGameState.soldier;
    delete mGameState.map;
    delete mBoss;
    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.deathSound);
    UnloadSound(mGameState.jumpSound);
    UnloadSound(mGameState.sceneChangeSound);
}