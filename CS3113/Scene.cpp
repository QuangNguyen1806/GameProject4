/**
* Author: [Your name here]
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Scene.h"

Scene::Scene() : mMap(nullptr), mCamera({0}), mEnemyCount(0), mIsComplete(false) {}

Scene::~Scene()
{
    if (mMap != nullptr)
    {
        delete mMap;
        mMap = nullptr;
    }
}
