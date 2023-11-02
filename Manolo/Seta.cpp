#include "Seta.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ACCELERATION 0.002f
#define VEL 2.f
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
		posPlayer.x -= velocity;
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			//posPlayer.x += 2;
			posPlayer.x += velocity;
			vaIzq = false;
		}
	}
	else {
		posPlayer.x += velocity;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			//posPlayer.x += 2;
			posPlayer.x -= velocity;
			vaIzq = true;
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

void Seta::changeVelocitiesScroll(bool thereIsScroll, int v) {
	if (thereIsScroll) {
		if (vaIzq) {
			velocity = VEL + v;
		}
		else {
			velocity = VEL - v;
		}
	}
	else{
		if (vaIzq) {
			velocity = VEL;
		}
		else {
			velocity = VEL;
		}
	}
}