#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Personaje.h"
#include "Game.h"

void Personaje::setTileMap(TileMap* tileMap) {
    map = tileMap;
}

void Personaje::setPosition(const glm::vec2& pos) {
    posPlayer = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Personaje::getPosition() {
    return posPlayer;
}


