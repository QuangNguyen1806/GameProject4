#include "Menu.h"

Menu::Menu() : Scene { {0.0f} } {}

Menu::Menu(Vector2 origin) : Scene { origin } {}

Menu::~Menu() { shutdown(); }

void Menu::initialise() {
    mGameState.bgm = LoadMusicStream("assets/background.wav");
    mGameState.nextSceneID = 0;
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);
    
    // Initialize camera
    mGameState.camera = { 0 };
    mGameState.camera.target = mOrigin;
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void Menu::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);
}

void Menu::render() {
    DrawText(MENU_TEXT, mOrigin.x - OFFSET, mOrigin.y - 200, 50, WHITE);
}

void Menu::shutdown() {
    UnloadMusicStream(mGameState.bgm);
}