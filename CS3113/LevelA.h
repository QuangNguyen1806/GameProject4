/**
* Author: [Your name here]
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef LEVEL_A_H
#define LEVEL_A_H

#include "Scene.h"

class LevelA : public Scene
{
private:
    static unsigned int levelData[];

public:
    LevelA();
    ~LevelA();

    void initialize();
    void update(float deltaTime);
    void render();
};

#endif // LEVEL_A_H
