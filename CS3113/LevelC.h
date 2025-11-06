/**
* Author: [Your name here]
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef LEVEL_C_H
#define LEVEL_C_H

#include "Scene.h"

class LevelC : public Scene
{
private:
    static unsigned int levelData[];

public:
    LevelC();
    ~LevelC();

    void initialize();
    void update(float deltaTime);
    void render();
};

#endif // LEVEL_C_H
