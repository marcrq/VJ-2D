#include "Goomba.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ACCELERATION 0.002f
#define MAX_VEL 3.f
#define TIME_UNTIL_ELIMINATION 2.f
#define POINTS 1

enum GoombaAnims
{
	MOVE, DEAD
};

void Goomba::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	velocity = 1.f;
	timeSinceDead = 0.f;
	goomba.loadFromFile("images/goomba.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.33333, 1.0), &goomba, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE, 8);
	sprite->addKeyframe(MOVE, glm::vec2(0.f, 1.0f));
	sprite->addKeyframe(MOVE, glm::vec2(0.33333f, 1.0f));

	sprite->setAnimationSpeed(DEAD, 8);
	sprite->addKeyframe(DEAD, glm::vec2(0.66666f, 1.0f));
	sprite->addKeyframe(DEAD, glm::vec2(0.66666f, 1.0f));
	sprite->addKeyframe(DEAD, glm::vec2(0.66666f, 1.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	vaIzq = true;
	vivo = true;
}

void Goomba::update(int deltaTime) {
	sprite->update(deltaTime);

	posPlayer.y += 1.5; //estas 2 lineas es para controlar la caida
	bool isGrounded = map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);

	if (!vivo) {
		timeSinceDead += deltaTime / 1000.0;
		if (timeSinceDead >= TIME_UNTIL_ELIMINATION) delete this;
	}

	else if (vaIzq && isGrounded) {
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			//posPlayer.x += 2;
			posPlayer.x += 1;
			vaIzq = false;
		}
		else {
			posPlayer.x -= 1;
		}
	}
	else if (!vaIzq && isGrounded) {
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			//posPlayer.x += 2;
			posPlayer.x -= 1;
			vaIzq = true;
		}
		else {
			posPlayer.x += 1;
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Goomba::render() {
	sprite->render();
}

void Goomba::hit() {
	vivo = false;
	sprite->changeAnimation(DEAD);
	timeSinceDead = 0.f;
}

bool Goomba::isAlive() {
	return true;
}

string Goomba::myType() {
	return "Goomba";
}

int Goomba::getPoints() {
	return POINTS;
}