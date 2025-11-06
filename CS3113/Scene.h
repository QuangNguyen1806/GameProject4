/**
* Author: [Your name here]
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef SCENE_H
#define SCENE_H

#include "cs3113.h"
#include "Entity.h"
#include "Map.h"

class Scene
{
protected:
    Map *mMap;
    Entity mPlayer;
    Entity mEnemies[3];
    Camera2D mCamera;
    int mEnemyCount;
    bool mIsComplete;

public:
    Scene();
    virtual ~Scene();

    virtual void initialize() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;

    bool isComplete() const { return mIsComplete; }
    void setComplete(bool complete) { mIsComplete = complete; }

    Entity *getPlayer() { return &mPlayer; }
    Entity *getEnemies() { return mEnemies; }
    int getEnemyCount() const { return mEnemyCount; }
    Map *getMap() { return mMap; }
    Camera2D *getCamera() { return &mCamera; }
};

#endif // SCENE_H