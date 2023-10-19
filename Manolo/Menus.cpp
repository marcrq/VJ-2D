// Menus.cpp

#include "Menus.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define TIME_FOR_CHANGE 0.15

Menus::Menus() {
    // Constructor code, if needed
}

Menus::~Menus() {
    // Destructor code, if needed
}

void Menus::init(ShaderProgram& shaderProgram) {
    opt = 0;
    insideInstrucciones = false;
    changedRecently = false;
    timerChangeOption = 0.f;
    releasedUp = true;
    releasedDown = true;
    showingCredits = false;
    level1.loadFromFile("images/test1v2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite1 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &level1, &shaderProgram);
    level2.loadFromFile("images/test2v2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite2 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &level2, &shaderProgram);
    instr.loadFromFile("images/test3v2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite3 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &instr, &shaderProgram);
    instrucciones.loadFromFile("images/instrv2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spriteInstrucciones = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &instrucciones, &shaderProgram);
    credits.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spriteCredits = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &credits, &shaderProgram);

    sprite1->setPosition(glm::vec2(float(0), float(0)));
    sprite2->setPosition(glm::vec2(float(0), float(0)));
    sprite3->setPosition(glm::vec2(float(0), float(0)));
    spriteInstrucciones->setPosition(glm::vec2(float(0), float(0)));
    spriteCredits->setPosition(glm::vec2(float(0), float(0)));
}

void Menus::update(int deltaTime, int &chosed) {
    chosed = 0;
    if (!showingCredits) {
        if (changedRecently) {
            timerChangeOption += deltaTime / 1000.0;
            if (timerChangeOption >= TIME_FOR_CHANGE) changedRecently = false;
        }
        if ((!changedRecently || (changedRecently && releasedDown)) && Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
            ++opt;
            if (opt == 3) opt = 0;
            timerChangeOption = 0;
            changedRecently = true;
            releasedDown = false;
        }
        else if ((!changedRecently || (changedRecently && releasedUp)) && Game::instance().getSpecialKey(GLUT_KEY_UP)) {
            --opt;
            if (opt == -1) opt = 2;
            timerChangeOption = 0;
            changedRecently = true;
            releasedUp = false;
        }
        else if (Game::instance().getKey(13)) {
            if (opt == 0) chosed = 1;
            else if (opt == 1) chosed = 2;
            else {
                insideInstrucciones = true;
            }
        }
        else if (insideInstrucciones && Game::instance().getKey('b')) {
            insideInstrucciones = false;
        }

        if (!releasedDown && !Game::instance().getSpecialKey(GLUT_KEY_DOWN)) releasedDown = true;
        if (!releasedUp && !Game::instance().getSpecialKey(GLUT_KEY_UP)) releasedUp = true;
    }
    else {
        if (Game::instance().getKey('b')) {
            showingCredits = false;
        }
    }
    
}

void Menus::render() {
    if (showingCredits) {
        spriteCredits->render();
    }
    else {
        if (opt == 0 && !insideInstrucciones) sprite1->render();
        else if (opt == 1 && !insideInstrucciones) sprite2->render();
        else if (opt == 2 && !insideInstrucciones) sprite3->render();

        if (insideInstrucciones) spriteInstrucciones->render();
    }
}

void Menus::showingCreditsFunc() {
    showingCredits = true;
}