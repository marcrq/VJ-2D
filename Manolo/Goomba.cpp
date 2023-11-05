#include "Goomba.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ACCELERATION 0.002f
#define MAX_VEL 3.f
#define TIME_UNTIL_ELIMINATION 3.f
#define POINTS 10
#define VEL 1.0

enum GoombaAnims
{
	MOVE, DEAD
};

void Goomba::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	velocity = 1.f;
	timeSinceDead = 0.f;
	vaIzq = true;
	vivo = true;
	pisado = false;
	goomba.loadFromFile("images/goomba.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.33333, 1.0), &goomba, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE, 8);
	sprite->addKeyframe(MOVE, glm::vec2(0.f, 1.0f));
	sprite->addKeyframe(MOVE, glm::vec2(0.33333f, 1.0f));

	sprite->setAnimationSpeed(DEAD, 8);
	sprite->addKeyframe(DEAD, glm::vec2(0.66666f, 1.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Goomba::update(int deltaTime) {
	sprite->update(deltaTime);

	if (pisado && vivo) {
		timeSinceDead += deltaTime / 1000.0;
		if (timeSinceDead >= TIME_UNTIL_ELIMINATION) vivo = false;
	}
	else {
		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y).first);

		if (vaIzq && !pisado) {
			posPlayer.x -= velocity;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)).first)
			{
				//posPlayer.x += 2;
				posPlayer.x += velocity;
				vaIzq = false;
			}
		}
		else if (!pisado) {
			posPlayer.x += velocity;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)).first)
			{
				//posPlayer.x += 2;
				posPlayer.x -= velocity;
				vaIzq = true;
			}
		}
	}

	

	/*posPlayer.y += 1.5; //estas 2 lineas es para controlar la caida
	bool isGrounded = map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y).first;

	if (pisado && vivo) {
		timeSinceDead += deltaTime / 1000.0;
		if (timeSinceDead >= TIME_UNTIL_ELIMINATION) vivo = false;
	}

	else if (vaIzq && isGrounded && !pisado) {
		posPlayer.x -= velocity;
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)).first)
		{
			posPlayer.x += velocity;
			vaIzq = false;
		}
	}
	else if (!vaIzq && isGrounded && !pisado) {
		posPlayer.x += velocity;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)).first)
		{
			posPlayer.x -= velocity;
			vaIzq = true;
		}
	}*/
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Goomba::render() {
	sprite->render();
}

void Goomba::hit() {
	pisado = true;
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

bool Goomba::isPisado() {
	return pisado;
}

void Goomba::changeVelocitiesScroll(bool thereIsScroll, int v) {
	if (thereIsScroll) {
		if (vaIzq) {
			velocity = VEL + v;
		}
		else {
			velocity = VEL - v;
		}
		if (pisado) {
			posPlayer.x -= v;
		}
	}
	else {
		if (vaIzq && !pisado) {
			velocity = VEL;
		}
		else {
			velocity = VEL;
		}
	}
}