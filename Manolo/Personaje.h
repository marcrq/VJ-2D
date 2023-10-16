#pragma once
#ifndef _PERSONAJE_INCLUDE
#define _PERSONAJE_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Personaje {
public:
    enum class PersonajeType {
        Goomba,
        Ktroopa,
        Seta,
        Star,
        Shell        
    };

    virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
    virtual void update(int deltaTime) = 0;
    virtual void render() = 0;
    virtual void hit() = 0;
    virtual bool isAlive() = 0;
    virtual string myType() = 0;

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    

    glm::vec2 getPosition();

protected:
    bool vaIzq;
    float velocity;
    glm::ivec2 tileMapDispl, posPlayer;
    int jumpAngle, startY;
    Sprite* sprite;
    TileMap* map;
};

#endif // _PERSONAJE_INCLUDE

