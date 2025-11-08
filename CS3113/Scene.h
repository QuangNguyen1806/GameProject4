#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

struct GameState {
    Entity *soldier;              // Player character
    Map *map;                     // Tilemap
    Music bgm;                    // Background music
    Sound deathSound;             // Enemy collision/death SFX
    Sound sceneChangeSound;       // Level transition SFX
    Sound jumpSound;              // Jump SFX
    Camera2D camera;              // Viewport camera
    int nextSceneID;              // Scene transition flag (0=stay, 1+=goto level)
    int lives = 3;
    bool gameOver = false;
    bool playerWon = false;
};

class Scene {
protected:
    GameState mGameState;
    Vector2 mOrigin;
    
public:
    Scene();
    Scene(Vector2 origin);
    virtual ~Scene();
    
    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;
    
    GameState getState() const { return mGameState; }
    void setState(GameState state) { mGameState = state; }
    Vector2 getOrigin() const { return mOrigin; }
};

#endif // SCENE_H