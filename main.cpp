/**
 * Author: [Your name here]
 * Assignment: Rise of the AI
 * Date due: 2025-11-08, 11:59pm
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 **/

#include "CS3113/cs3113.h"
#include "CS3113/Scene.h"
#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/LevelBoss.h"

enum AppState { MENU, LEVEL_A, LEVEL_B, LEVEL_C, LEVEL_BOSS, GAME_OVER_WIN, GAME_OVER_LOSE };

// ────────── GLOBAL STATE ────────────
int g_currentState = MENU;
int g_lives = 3;
float g_accumulator = 0.0f;
const float FIXED_TIMESTEP = 1.0f / 60.0f;

// ────────── SCENES ────────────
Scene *g_currentScene = nullptr;
LevelA *g_levelA = nullptr;
LevelB *g_levelB = nullptr;
LevelC *g_levelC = nullptr;
LevelBoss *g_levelBoss = nullptr;

// ────────── AUDIO ────────────
Music g_backgroundMusic;
Sound g_jumpSound;
Sound g_hitSound;
Sound g_winSound;

// ────────── FUNCTION DECLARATIONS ────────────
void initializeScene(int sceneType);
void cleanupScene();
void updateScene(float deltaTime);
void renderScene();
bool checkEnemyCollisions();

// ────────── MAIN ────────────
int main(void)
{
    InitWindow(1280, 720, "Rise of the AI");
    InitAudioDevice();

    g_backgroundMusic = LoadMusicStream("assets/background.ogg");
    g_jumpSound = LoadSound("assets/jump.wav");
    g_hitSound = LoadSound("assets/hit.wav");
    g_winSound = LoadSound("assets/win.wav");

    if (g_backgroundMusic.ctxData != nullptr)
    {
        PlayMusicStream(g_backgroundMusic);
        SetMusicVolume(g_backgroundMusic, 0.5f);
    }

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // ────────── UPDATE ────────────
        float deltaTime = GetFrameTime();
        g_accumulator += deltaTime;

        if (g_backgroundMusic.ctxData != nullptr)
        {
            UpdateMusicStream(g_backgroundMusic);

            if (!IsMusicStreamPlaying(g_backgroundMusic)) {
                SeekMusicStream(g_backgroundMusic, 0.0f);
                PlayMusicStream(g_backgroundMusic);
}

        }

        while (g_accumulator >= FIXED_TIMESTEP)
        {
            // Menu Logic
            if (g_currentState == MENU)
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    g_currentState = LEVEL_A;
                    g_lives = 3;
                    initializeScene(LEVEL_A);
                }
            }
            // Level A Logic
            else if (g_currentState == LEVEL_A)
            {
                updateScene(FIXED_TIMESTEP);

                // Check enemy collisions
                if (checkEnemyCollisions())
                {
                    if (g_hitSound.frameCount > 0) PlaySound(g_hitSound);
                    g_lives--;
                    if (g_lives <= 0)
                    {
                        g_currentState = GAME_OVER_LOSE;
                        cleanupScene();
                    }
                    else
                    {
                        initializeScene(LEVEL_A);
                    }
                }
                // Check level completion
                else if (g_currentScene->getPlayer()->getPosition().x > 640 + (10 * 64))
                {
                    // Don't play win sound yet - only on final victory
                    g_currentState = LEVEL_B;
                    cleanupScene();
                    initializeScene(LEVEL_B);
                }
                // Check if player fell off
                else if (g_currentScene->getPlayer()->getPosition().y > 720)
                {
                    if (g_hitSound.frameCount > 0) PlaySound(g_hitSound);
                    g_lives--;
                    if (g_lives <= 0)
                    {
                        g_currentState = GAME_OVER_LOSE;
                        cleanupScene();
                    }
                    else
                    {
                        initializeScene(LEVEL_A);
                    }
                }
            }
            // Level B Logic
            else if (g_currentState == LEVEL_B)
            {
                updateScene(FIXED_TIMESTEP);

                if (checkEnemyCollisions())
                {
                    if (g_hitSound.frameCount > 0) PlaySound(g_hitSound);
                    g_lives--;
                    if (g_lives <= 0)
                    {
                        g_currentState = GAME_OVER_LOSE;
                        cleanupScene();
                    }
                    else
                    {
                        initializeScene(LEVEL_B);
                    }
                }
                else if (g_currentScene->getPlayer()->getPosition().x > 640 + (10 * 64))
                {
                    g_currentState = LEVEL_C;
                    cleanupScene();
                    initializeScene(LEVEL_C);
                }
                else if (g_currentScene->getPlayer()->getPosition().y > 720)
                {
                    if (g_hitSound.frameCount > 0) PlaySound(g_hitSound);
                    g_lives--;
                    if (g_lives <= 0)
                    {
                        g_currentState = GAME_OVER_LOSE;
                        cleanupScene();
                    }
                    else
                    {
                        initializeScene(LEVEL_B);
                    }
                }
            }
            // Level C Logic
            else if (g_currentState == LEVEL_C)
            {
                updateScene(FIXED_TIMESTEP);

                if (checkEnemyCollisions())
                {
                    if (g_hitSound.frameCount > 0) PlaySound(g_hitSound);
                    g_lives--;
                    if (g_lives <= 0)
                    {
                        g_currentState = GAME_OVER_LOSE;
                        cleanupScene();
                    }
                    else
                    {
                        initializeScene(LEVEL_C);
                    }
                }
                else if (g_currentScene->getPlayer()->getPosition().x > 640 + (10 * 64))
                {
                    g_currentState = LEVEL_BOSS;
                    cleanupScene();
                    initializeScene(LEVEL_BOSS);
                }
                else if (g_currentScene->getPlayer()->getPosition().y > 720)
                {
                    if (g_hitSound.frameCount > 0) PlaySound(g_hitSound);
                    g_lives--;
                    if (g_lives <= 0)
                    {
                        g_currentState = GAME_OVER_LOSE;
                        cleanupScene();
                    }
                    else
                    {
                        initializeScene(LEVEL_C);
                    }
                }
            }
            // Boss Level Logic
            else if (g_currentState == LEVEL_BOSS)
            {
                updateScene(FIXED_TIMESTEP);

                if (checkEnemyCollisions())
                {
                    if (g_hitSound.frameCount > 0) PlaySound(g_hitSound);
                    g_lives--;
                    if (g_lives <= 0)
                    {
                        g_currentState = GAME_OVER_LOSE;
                        cleanupScene();
                    }
                    else
                    {
                        initializeScene(LEVEL_BOSS);
                    }
                }
                else if (g_currentScene->getPlayer()->getPosition().x > 640 + (10 * 64))
                {
                    // NOW play win sound - game completed!
                    if (g_winSound.frameCount > 0) PlaySound(g_winSound);
                    g_currentState = GAME_OVER_WIN;
                    cleanupScene();
                }
                else if (g_currentScene->getPlayer()->getPosition().y > 720)
                {
                    if (g_hitSound.frameCount > 0) PlaySound(g_hitSound);
                    g_lives--;
                    if (g_lives <= 0)
                    {
                        g_currentState = GAME_OVER_LOSE;
                        cleanupScene();
                    }
                    else
                    {
                        initializeScene(LEVEL_BOSS);
                    }
                }
            }
            // Game Over - Win
            else if (g_currentState == GAME_OVER_WIN)
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    g_currentState = MENU;
                    g_lives = 3;
                    cleanupScene();
                }
            }
            // Game Over - Lose
            else if (g_currentState == GAME_OVER_LOSE)
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    g_currentState = MENU;
                    g_lives = 3;
                    cleanupScene();
                }
            }

            g_accumulator -= FIXED_TIMESTEP;
        }

        // ────────── RENDER ────────────
        BeginDrawing();
        ClearBackground({65, 105, 225, 255});

        if (g_currentState == MENU)
        {
            DrawText("RISE OF THE AI", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 - 100, 40, YELLOW);
            DrawText("Press ENTER to start", GetScreenWidth() / 2 - 120, GetScreenHeight() / 2, 20, WHITE);
        }
        else if (g_currentState == LEVEL_A || g_currentState == LEVEL_B ||
                 g_currentState == LEVEL_C || g_currentState == LEVEL_BOSS)
        {
            renderScene();

            // Draw HUD
            DrawText(TextFormat("Lives: %d", g_lives), 10, 10, 20, WHITE);
        }
        else if (g_currentState == GAME_OVER_WIN)
        {
            DrawText("YOU WIN!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 50, 60, GREEN);
            DrawText("Press ENTER to return to menu", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 + 50, 20, WHITE);
        }
        else if (g_currentState == GAME_OVER_LOSE)
        {
            DrawText("YOU LOSE!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 50, 60, RED);
            DrawText("Press ENTER to return to menu", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 + 50, 20, WHITE);
        }

        EndDrawing();
    }

    // Cleanup
    cleanupScene();
    if (g_levelA != nullptr) delete g_levelA;
    if (g_levelB != nullptr) delete g_levelB;
    if (g_levelC != nullptr) delete g_levelC;
    if (g_levelBoss != nullptr) delete g_levelBoss;

    UnloadMusicStream(g_backgroundMusic);
    UnloadSound(g_jumpSound);
    UnloadSound(g_hitSound);
    UnloadSound(g_winSound);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

// ────────── FUNCTION IMPLEMENTATIONS ────────────
void initializeScene(int sceneType)
{
    cleanupScene();

    if (sceneType == LEVEL_A)
    {
        g_levelA = new LevelA();
        g_currentScene = g_levelA;
    }
    else if (sceneType == LEVEL_B)
    {
        g_levelB = new LevelB();
        g_currentScene = g_levelB;
    }
    else if (sceneType == LEVEL_C)
    {
        g_levelC = new LevelC();
        g_currentScene = g_levelC;
    }
    else if (sceneType == LEVEL_BOSS)
    {
        g_levelBoss = new LevelBoss();
        g_currentScene = g_levelBoss;
    }
}

void cleanupScene()
{
    if (g_currentScene != nullptr)
    {
        g_currentScene = nullptr;
    }
}

void updateScene(float deltaTime)
{
    if (g_currentScene == nullptr) return;

    Scene *scene = g_currentScene;
    Entity *player = scene->getPlayer();
    Entity *enemies = scene->getEnemies();
    int enemyCount = scene->getEnemyCount();
    Map *map = scene->getMap();

    // Player input
    player->resetMovement();
    if (IsKeyDown(KEY_LEFT)) player->moveLeft();
    if (IsKeyDown(KEY_RIGHT)) player->moveRight();
    
    // Play jump sound when jumping
    if (IsKeyPressed(KEY_SPACE))
    {
        if (player->isCollidingBottom())  // Only jump if on ground
        {
            player->jump();
            if (g_jumpSound.frameCount > 0) PlaySound(g_jumpSound);
        }
    }

    // Update player
    if (enemyCount > 0)
    {
        player->update(deltaTime, player, map, enemies, enemyCount);
    }
    else
    {
        player->update(deltaTime, player, map, nullptr, 0);
    }

    // Update enemies
    for (int i = 0; i < enemyCount; i++)
    {
        if (enemies[i].isActive())
        {
            enemies[i].update(deltaTime, player, map, enemies, enemyCount);
            
            // Keep enemies within map bounds
            Vector2 enemyPos = enemies[i].getPosition();
            float leftBound = map->getLeftBoundary() + 32;
            float rightBound = map->getRightBoundary() - 32;
            
            // Reverse direction if hitting boundaries
            if (enemyPos.x <= leftBound)
            {
                enemies[i].setPosition({leftBound + 1, enemyPos.y});
                // Force direction change for wanderers
                if (enemies[i].getAIType() == WANDERER)
                {
                    enemies[i].moveRight();
                }
            }
            else if (enemyPos.x >= rightBound)
            {
                enemies[i].setPosition({rightBound - 1, enemyPos.y});
                // Force direction change for wanderers
                if (enemies[i].getAIType() == WANDERER)
                {
                    enemies[i].moveLeft();
                }
            }
        }
    }

    // Update scene
    scene->update(deltaTime);
}

bool checkEnemyCollisions()
{
    if (g_currentScene == nullptr) return false;
    
    Entity *player = g_currentScene->getPlayer();
    Entity *enemies = g_currentScene->getEnemies();
    int enemyCount = g_currentScene->getEnemyCount();
    
    for (int i = 0; i < enemyCount; i++)
    {
        if (!enemies[i].isActive()) continue;
        
        // Manual collision check since isColliding is private
        Vector2 playerPos = player->getPosition();
        Vector2 enemyPos = enemies[i].getPosition();
        Vector2 playerCollider = player->getColliderDimensions();
        Vector2 enemyCollider = enemies[i].getColliderDimensions();
        
        float xDistance = fabs(playerPos.x - enemyPos.x) - 
                         ((playerCollider.x + enemyCollider.x) / 2.0f);
        float yDistance = fabs(playerPos.y - enemyPos.y) - 
                         ((playerCollider.y + enemyCollider.y) / 2.0f);
        
        if (xDistance < 0.0f && yDistance < 0.0f)
        {
            return true;  // Collision detected!
        }
    }
    
    return false;
}

void renderScene()
{
    if (g_currentScene == nullptr) return;
    g_currentScene->render();
}