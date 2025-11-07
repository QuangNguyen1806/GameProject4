#ifndef MENU_H
#define MENU_H

#include "Scene.h"

class Menu : public Scene {
public:
    static constexpr float TILE_DIMENSION = 75.0f;
    static constexpr float ACCELERATION_OF_GRAVITY = 981.0f;
    static constexpr float END_GAME_THRESHOLD = 800.0f;
    static constexpr int OFFSET = 300.0f;
    
    const char * const MENU_TEXT = "RISE OF THE AI\nPress enter to start!";
    
    Menu();
    Menu(Vector2 origin);
    ~Menu();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // MENU_H