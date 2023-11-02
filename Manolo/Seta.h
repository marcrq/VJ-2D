#pragma once
#define _SETA_INCLUDE
#define _SETA_INCLUDE


#include "Personaje.h"


class Seta : public Personaje {
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void render() override;
	void hit() override;
	bool isAlive() override;
	string myType() override;
	int getPoints();

	bool vivo;
	void changeVelocitiesScroll(bool thereIsScroll, int v);

private:
	Texture seta;
	bool vaIzq, isJumping, isGrounded;
};



