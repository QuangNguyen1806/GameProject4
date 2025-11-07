#include "Scene.h"

Scene::Scene() {
    // Initialize GameState with defaults
    mGameState.soldier = nullptr;
    mGameState.map = nullptr;
    mGameState.bgm = {0};
    mGameState.deathSound = {0};
    mGameState.jumpSound = {0};
    mGameState.sceneChangeSound = {0};
    mGameState.camera = {0};
    mGameState.nextSceneID = 0;
}

Scene::Scene(Vector2 origin) : mOrigin{origin} {
    // Initialize GameState with defaults
    mGameState.soldier = nullptr;
    mGameState.map = nullptr;
    mGameState.bgm = {0};
    mGameState.deathSound = {0};
    mGameState.jumpSound = {0};
    mGameState.sceneChangeSound = {0};
    mGameState.camera = {0};
    mGameState.nextSceneID = 0;
}

Scene::~Scene() {}