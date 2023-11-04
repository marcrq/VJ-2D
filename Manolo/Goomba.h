#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE

#include "Personaje.h"

class Goomba : public Personaje {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
    void update(int deltaTime) override;
    void render() override;
    void hit() override;
    bool isAlive() override;
    string myType() override;
    bool isPisado();

    bool vivo, pisado, isGrounded;
    float timeSinceDead;
    int getPoints();
    void changeVelocitiesScroll(bool thereIsScroll, int v);

private:
    Texture goomba;
};

#endif // _GOOMBA_INCLUDE
