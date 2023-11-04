#pragma once
#ifndef _OBJETOENTORNO_INCLUDE
#define _OBJETOENTORNO_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class ObjetoEntorno {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::vec2 size, string nameSprite);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    void setPosition(int dist);

    glm::vec2 getPosition();
    void animacionEndLevelFunc(float pos);

protected:
    glm::ivec2 tileMapDispl, pos, initialPos;
    float finalPos;
    Texture texture;
    Sprite* sprite;
    TileMap* map;


    void nextLevel();
};

#endif // _PERSONAJE_INCLUDE

