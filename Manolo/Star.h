#ifndef _STAR_INCLUDE
#define _STAR_INCLUDE

#include "Personaje.h"

class Star : public Personaje {
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void render() override;
	void hit() override;
	bool isAlive() override;
	string myType() override;
	int getPoints();
	void changeVelocitiesScroll(bool thereIsScroll, int v);


private:
	Texture star;
	bool vaIzq, isJumping, isGrounded, vivo;
	float timerStopped;
};


#endif // _PLAYER_INCLUDE


