#ifndef _KTROOPA_INCLUDE
#define _KTROOPA_INCLUDE

#include "Personaje.h"

class Ktroopa : public Personaje {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
    void update(int deltaTime) override;
    void render() override;
    void hit() override;
    bool isAlive() override;
    string myType() override;

    bool vivo;

    void killed();
    bool isShellMovingFunc();
    void moveShellToRight(bool toRight);
    int getPoints();
    void changeVelocitiesScroll(bool thereIsScroll, int v);

private:
    Texture ktroopa, shell;
    int conteoCambiosAnimacion;
    bool isDead, isInAnimacionAlternarModo, isShellMoving;
    float timeSinceDead, timerAnimacionAlternarModo;
    Sprite* ktroopaSprite;
    Sprite* shellSprite;

    Sprite* getCorrectSprite();
    void animationResurrection();
};

#endif // _KTROOPA_INCLUDE

