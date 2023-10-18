#pragma once
#define _MAINMENU_INCLUDE
#define _MAINMENU_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class MainMenu {
public:
    MainMenu();
    ~MainMenu();

    Sprite* sprite1, *sprite2, *sprite3, *spriteInstrucciones;
    TileMap* map;
    Texture level1, level2, instr, instrucciones;
    int opt;
    bool changedRecently, insideInstrucciones, releasedUp, releasedDown;
    float timerChangeOption;

    void init(ShaderProgram& shaderProgram);
    void render();
    void update(int deltaTime, int &chosed);
};
