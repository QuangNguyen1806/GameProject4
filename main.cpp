/**
* Author: Quang Nguyen
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Menu.h"
#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/LevelBoss.h"

// Global Constants
constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 600;
constexpr int FPS = 60;
constexpr int NUMBER_OF_LEVELS = 4;
constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f;
float gTimeAccumulator = 0.0f;
int gLives = 3;
bool gGameOver = false;
bool gPlayerWon = false;
Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};
Menu *gMenu = nullptr;
LevelA *gLevelA = nullptr;
LevelB *gLevelB = nullptr;
LevelC *gLevelC = nullptr;
LevelBoss *gLevelBoss = nullptr;

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene) {
    if (scene == nullptr) {
        return;
    }
    gCurrentScene = scene;
    gCurrentScene->initialise();
}

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rise of the AI");
    InitAudioDevice();

    // Create all scenes
    gMenu = new Menu(ORIGIN);
    gLevelA = new LevelA(ORIGIN);
    gLevelB = new LevelB(ORIGIN);
    gLevelC = new LevelC(ORIGIN);
    gLevelBoss = new LevelBoss(ORIGIN);

    // Add to levels vector in order: [0]=Menu, [1]=LevelA, [2]=LevelB, [3]=LevelC, [4]=LevelBoss
    gLevels.push_back(gMenu);       // 0
    gLevels.push_back(gLevelA);     // 1
    gLevels.push_back(gLevelB);     // 2
    gLevels.push_back(gLevelC);     // 3
    gLevels.push_back(gLevelBoss);  // 4

    switchToScene(gLevels[0]); // Start at menu
    SetTargetFPS(FPS);
}

void processInput() {
    if (gCurrentScene == gMenu) {
        if (IsKeyPressed(KEY_ENTER)) {
            gLives = 3; // Reset lives for new game
            gGameOver = false;
            gPlayerWon = false;
            switchToScene(gLevels[1]); // Go to LevelA
        }
    } else if (!gGameOver && !gPlayerWon) {
        // Handle player movement in levels
        Entity *player = gCurrentScene->getState().soldier;
        if (player != nullptr) {
            // Left movement
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                player->moveLeft();
            }
            // Right movement
            else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                player->moveRight();
            }
            
            // Jump (only on key press, not hold)
            if (IsKeyPressed(KEY_SPACE)) {
                player->jump();
            }
        }
    }
}

void update() {
    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    // Clamp deltaTime to prevent spiral of death
    if (deltaTime > 0.05f) deltaTime = 0.05f;
    gTimeAccumulator += deltaTime;

    // Update music stream in main loop
    if (gCurrentScene != nullptr && gCurrentScene->getState().bgm.stream.buffer != nullptr) {
        UpdateMusicStream(gCurrentScene->getState().bgm);
    }

    // Fixed timestep loop
    while (gTimeAccumulator >= FIXED_TIMESTEP) {
        if (gCurrentScene && !gGameOver && !gPlayerWon) {
            gCurrentScene->update(FIXED_TIMESTEP);
        }

        // Handle scene transitions via nextSceneID
        if (gCurrentScene != nullptr) {
            int nextSceneID = gCurrentScene->getState().nextSceneID;
            if (nextSceneID > 0) {
                // Check bounds
                if (nextSceneID < (int)gLevels.size()) {
                    switchToScene(gLevels[nextSceneID]);
                } else if (nextSceneID == 5) {
                    // Win condition - beat all levels
                    gPlayerWon = true;
                }
            }
        }

        gTimeAccumulator -= FIXED_TIMESTEP;
    }
}

void render() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (gCurrentScene == gMenu) {
        gCurrentScene->render();
    } else if (gCurrentScene) {
        // Set camera target to player position (BOTH X AND Y for full centering)
        if (gCurrentScene->getState().soldier != nullptr) {
            Vector2 playerPos = gCurrentScene->getState().soldier->getPosition();
            gCurrentScene->getState().camera.target = playerPos;
        }
        
        // Render level with camera
        BeginMode2D(gCurrentScene->getState().camera);
        gCurrentScene->render();
        EndMode2D();

        // Draw HUD (lives counter)
        DrawText(TextFormat("Lives: %d", gLives), 20, 20, 20, WHITE);

        // Draw game over screen
        if (gGameOver) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{0, 0, 0, 200});
            DrawText("YOU LOSE", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 40, 40, RED);
            DrawText("Press ENTER to return to menu", SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 + 20, 20, WHITE);
            
            if (IsKeyPressed(KEY_ENTER)) {
                gGameOver = false;
                gLives = 3;
                switchToScene(gLevels[0]); // Go back to menu
            }
        }

        // Draw win screen
        if (gPlayerWon) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{0, 0, 0, 200});
            DrawText("YOU WIN!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 40, 40, GREEN);
            DrawText("Press ENTER to return to menu", SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 + 20, 20, WHITE);
            
            if (IsKeyPressed(KEY_ENTER)) {
                gPlayerWon = false;
                gLives = 3;
                switchToScene(gLevels[0]); // Go back to menu
            }
        }
    }

    EndDrawing();
}

void shutdown() {
    if (gMenu) { gMenu->shutdown(); delete gMenu; }
    if (gLevelA) { gLevelA->shutdown(); delete gLevelA; }
    if (gLevelB) { gLevelB->shutdown(); delete gLevelB; }
    if (gLevelC) { gLevelC->shutdown(); delete gLevelC; }
    if (gLevelBoss) { gLevelBoss->shutdown(); delete gLevelBoss; }
    gLevels.clear();
    CloseAudioDevice();
    CloseWindow();
}

int main(void) {
    initialise();
    while (!WindowShouldClose() && gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}