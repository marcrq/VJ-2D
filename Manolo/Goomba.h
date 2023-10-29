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

    bool vivo;
    float timeSinceDead;
    int getPoints();

private:
    Texture goomba;
};

#endif // _GOOMBA_INCLUDE
