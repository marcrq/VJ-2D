#ifndef _SHELL_INCLUDE
#define _SHELL_INCLUDE

#include "Personaje.h"

class Shell : public Personaje {
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void render() override;
	void hit() override;
	bool isAlive() override;
	string myType() override;

	bool vivo;

private:
	Texture shell;
	bool vaIzq, isJumping, isGrounded, isMoving;
};


#endif // _PLAYER_INCLUDE


