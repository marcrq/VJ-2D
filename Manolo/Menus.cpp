// Menus.cpp

#include "Menus.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define TIME_FOR_CHANGE 0.15

Menus::Menus() {
    engine = createIrrKlangDevice();
    soundGameClear = engine->addSoundSourceFromFile("audio/smb_world_clear.wav");
}

Menus::~Menus() {
    // Destructor code, if needed
}

enum PlayerAnims
{
    CERO, UNO, DOS, TRES, CUATRO, CINCO, SEIS, SIETE, OCHO, NUEVE
};

void Menus::init(ShaderProgram& shaderProgram) {
    opt = 0;
    insideInstrucciones = false;
    changedRecently = false;
    timerChangeOption = 0.f;
    releasedUp = true;
    releasedDown = true;
    showingCredits = false;
    maxPoints = 0;
    musicCredits = true;
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

    numbers.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
    /////////////////POINTS//////////////////
    spritePointsUnidad = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &shaderProgram);
    spritePointsUnidad->setNumberAnimations(10);
    spritePointsUnidad->setAnimationSpeed(CERO, 8);
    spritePointsUnidad->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
    spritePointsUnidad->setAnimationSpeed(UNO, 8);
    spritePointsUnidad->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
    spritePointsUnidad->setAnimationSpeed(DOS, 8);
    spritePointsUnidad->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
    spritePointsUnidad->setAnimationSpeed(TRES, 8);
    spritePointsUnidad->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
    spritePointsUnidad->setAnimationSpeed(CUATRO, 8);
    spritePointsUnidad->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
    spritePointsUnidad->setAnimationSpeed(CINCO, 8);
    spritePointsUnidad->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
    spritePointsUnidad->setAnimationSpeed(SEIS, 8);
    spritePointsUnidad->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
    spritePointsUnidad->setAnimationSpeed(SIETE, 8);
    spritePointsUnidad->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
    spritePointsUnidad->setAnimationSpeed(OCHO, 8);
    spritePointsUnidad->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
    spritePointsUnidad->setAnimationSpeed(NUEVE, 8);
    spritePointsUnidad->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
    spritePointsUnidad->setPosition(glm::vec2((126), (270)));

    spritePointsDecena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &shaderProgram);
    spritePointsDecena->setNumberAnimations(10);
    spritePointsDecena->setAnimationSpeed(CERO, 8);
    spritePointsDecena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
    spritePointsDecena->setAnimationSpeed(UNO, 8);
    spritePointsDecena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
    spritePointsDecena->setAnimationSpeed(DOS, 8);
    spritePointsDecena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
    spritePointsDecena->setAnimationSpeed(TRES, 8);
    spritePointsDecena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
    spritePointsDecena->setAnimationSpeed(CUATRO, 8);
    spritePointsDecena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
    spritePointsDecena->setAnimationSpeed(CINCO, 8);
    spritePointsDecena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
    spritePointsDecena->setAnimationSpeed(SEIS, 8);
    spritePointsDecena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
    spritePointsDecena->setAnimationSpeed(SIETE, 8);
    spritePointsDecena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
    spritePointsDecena->setAnimationSpeed(OCHO, 8);
    spritePointsDecena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
    spritePointsDecena->setAnimationSpeed(NUEVE, 8);
    spritePointsDecena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
    spritePointsDecena->setPosition(glm::vec2((100), (270)));

    spritePointsCentena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &shaderProgram);
    spritePointsCentena->setNumberAnimations(10);
    spritePointsCentena->setAnimationSpeed(CERO, 8);
    spritePointsCentena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
    spritePointsCentena->setAnimationSpeed(UNO, 8);
    spritePointsCentena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
    spritePointsCentena->setAnimationSpeed(DOS, 8);
    spritePointsCentena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
    spritePointsCentena->setAnimationSpeed(TRES, 8);
    spritePointsCentena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
    spritePointsCentena->setAnimationSpeed(CUATRO, 8);
    spritePointsCentena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
    spritePointsCentena->setAnimationSpeed(CINCO, 8);
    spritePointsCentena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
    spritePointsCentena->setAnimationSpeed(SEIS, 8);
    spritePointsCentena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
    spritePointsCentena->setAnimationSpeed(SIETE, 8);
    spritePointsCentena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
    spritePointsCentena->setAnimationSpeed(OCHO, 8);
    spritePointsCentena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
    spritePointsCentena->setAnimationSpeed(NUEVE, 8);
    spritePointsCentena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
    spritePointsCentena->setPosition(glm::vec2((74), (270)));
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
        if (musicCredits) {
            engine->play2D(soundGameClear);
            musicCredits = false;
        }
        if (Game::instance().getKey('b')) {
            showingCredits = false;
            musicCredits = true;
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
        if (opt == 0 || opt == 1 || opt == 2) {
            spritePointsUnidad->render();
            spritePointsDecena->render();
            spritePointsCentena->render();
        }

        if (insideInstrucciones) spriteInstrucciones->render();
    }
}

void Menus::showingCreditsFunc() {
    showingCredits = true;
}

void Menus::setMaxPoints(int p) {
    maxPoints = p;
    actualizarPoints();
}

void Menus::asignarSpriteNumber(Sprite* s, int n) {
    if (n < 0 || n > 9) s->changeAnimation(0);
    else s->changeAnimation(n);
}

void Menus::actualizarPoints() {
    int aux = maxPoints;
    asignarSpriteNumber(spritePointsUnidad, maxPoints % 10);
    maxPoints = maxPoints / 10;
    asignarSpriteNumber(spritePointsDecena, maxPoints % 10);
    maxPoints = maxPoints / 10;
    asignarSpriteNumber(spritePointsCentena, maxPoints % 10);
    maxPoints = aux;
}
