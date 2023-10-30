#include "Seta.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ACCELERATION 0.002f
#define MAX_VEL 3.f
#define POINTS 1


enum SetaAnims
{
	MOVE
};


void Seta::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	velocity = 2.f;
	seta.loadFromFile("images/seta.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.0, 1.0), &seta, &shaderProgram);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	vaIzq = true;
}

void Seta::update(int deltaTime)
{
	sprite->update(deltaTime);

	posPlayer.y += FALL_STEP;
	if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y));

	if (vaIzq) {
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			//posPlayer.x += 2;
			posPlayer.x += velocity;
			vaIzq = false;
		}
		else {
			posPlayer.x -= velocity;
		}
	}
	else {
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			//posPlayer.x += 2;
			posPlayer.x -= velocity;
			vaIzq = true;
		}
		else {
			posPlayer.x += velocity;
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Seta::render()
{
	sprite->render();
}

void Seta::hit()
{
	velocity = 0;
}

bool Seta::isAlive() {
	return true;
}

string Seta::myType() {
	return "Seta";
}

int Seta::getPoints() {
	return POINTS;
}