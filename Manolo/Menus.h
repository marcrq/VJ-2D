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
    void setMaxPoints(int p);

private:
    Sprite* sprite1, * sprite2, * sprite3, * spriteInstrucciones, * spriteCredits, * spritePointsUnidad, * spritePointsDecena, *spritePointsCentena;
    TileMap* map;
    Texture level1, level2, instr, instrucciones, credits, numbers;
    int opt, maxPoints;
    bool changedRecently, insideInstrucciones, releasedUp, releasedDown, showingCredits;
    float timerChangeOption;

    void actualizarPoints();
    void asignarSpriteNumber(Sprite* s, int n);
};
