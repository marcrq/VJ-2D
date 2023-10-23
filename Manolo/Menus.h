#pragma once
#define _MENUS_INCLUDE
#define _MENUS_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Menus {
public:
    Menus();
    ~Menus();

    void init(ShaderProgram& shaderProgram);
    void render();
    void update(int deltaTime, int &chosed);
    void showingCreditsFunc();

private:
    Sprite* sprite1, * sprite2, * sprite3, * spriteInstrucciones, * spriteCredits;
    TileMap* map;
    Texture level1, level2, instr, instrucciones, credits;
    int opt;
    bool changedRecently, insideInstrucciones, releasedUp, releasedDown, showingCredits;
    float timerChangeOption;
};
