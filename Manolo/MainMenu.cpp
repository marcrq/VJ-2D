// MainMenu.cpp

#include "MainMenu.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define TIME_FOR_CHANGE 0.15

MainMenu::MainMenu() {
    // Constructor code, if needed
}

MainMenu::~MainMenu() {
    // Destructor code, if needed
}

void MainMenu::init(ShaderProgram& shaderProgram) {
    opt = 0;
    insideInstrucciones = false;
    changedRecently = false;
    timerChangeOption = 0.f;
    releasedUp = true;
    releasedDown = true;
    level1.loadFromFile("images/test1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite1 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &level1, &shaderProgram);
    level2.loadFromFile("images/test2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite2 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &level2, &shaderProgram);
    instr.loadFromFile("images/test3.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite3 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &instr, &shaderProgram);
    instrucciones.loadFromFile("images/instrucciones.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spriteInstrucciones = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &instrucciones, &shaderProgram);

    sprite1->setPosition(glm::vec2(float(0), float(0)));
    sprite2->setPosition(glm::vec2(float(0), float(0)));
    sprite3->setPosition(glm::vec2(float(0), float(0)));
    spriteInstrucciones->setPosition(glm::vec2(float(0), float(0)));
}

void MainMenu::update(int deltaTime, int &chosed) {
    chosed = 0;
    if (changedRecently) {
        timerChangeOption += deltaTime / 1000.0;
        if (timerChangeOption >= TIME_FOR_CHANGE) changedRecently = false;
    } //(!changedRecently && !releasedDown) || (changedRecently && releasedDown))
    if (!changedRecently && Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
        ++opt;
        if (opt == 3) opt = 0;
        timerChangeOption = 0;
        changedRecently = true;
        releasedDown = false;
    }
    else if (!changedRecently && Game::instance().getSpecialKey(GLUT_KEY_UP)) {
        --opt;
        if (opt == -1) opt = 2;
        timerChangeOption = 0;
        changedRecently = true;
        releasedUp = false;
    }
    else if(Game::instance().getKey(13)){
        if (opt == 0) chosed = 1;
        else if(opt == 1) chosed = 2;
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

void MainMenu::render() {
    if(opt == 0 && !insideInstrucciones) sprite1->render();
    else if (opt == 1 && !insideInstrucciones) sprite2->render();
    else if(opt == 2 && !insideInstrucciones) sprite3->render();

    if (insideInstrucciones) spriteInstrucciones->render();
}
