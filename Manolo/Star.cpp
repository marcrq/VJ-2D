#include "Star.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ACCELERATION 0.002f
#define MAX_VEL 3.f
#define POINTS 2
#define VEL 2.0
#define TIME_STOPPED 2.f


enum StarAnims
{
	MOVE
};


void Star::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	velocity = 2.f;
	timerStopped = 0.f;
	star.loadFromFile("images/star.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 1.0), &star, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(MOVE, 15);
	sprite->addKeyframe(MOVE, glm::vec2(0.f, 1.0f));
	sprite->addKeyframe(MOVE, glm::vec2(0.5f, 1.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	vaIzq = false;
	isJumping = false;
	vivo = true;
}

void Star::update(int deltaTime)
{
	sprite->update(deltaTime);

	//acaba de aparecer, va a estar un tiempo sin hacer nada
	if (timerStopped <= TIME_STOPPED) {
		timerStopped += deltaTime / 1000.0;
	}
	else {
		if (isJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				posPlayer.y = startY;
				isJumping = false;
			}
			else
			{
				posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90) {
					isGrounded = map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y).first;
					if (isGrounded) {
						isJumping = false;
					}
				}
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y).first)
			{
				isJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}

		if (vaIzq) {
			posPlayer.x -= velocity;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)).first)
			{
				//posPlayer.x += 2;
				posPlayer.x += velocity;
				vaIzq = false;
			}
		}
		else {
			posPlayer.x += velocity;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)).first)
			{
				//posPlayer.x += 2;
				posPlayer.x -= velocity;
				vaIzq = true;
			}
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Star::render() {
	sprite->render();
}

void Star::hit() {
	vaIzq = !vaIzq;
	vivo = false;
}

bool Star::isAlive() {
	return vivo;
}

string Star::myType() {
	return "Star";
}

int Star::getPoints() {
	return POINTS;
}

void Star::changeVelocitiesScroll(bool thereIsScroll, int v) {
	if (thereIsScroll) {
		if (timerStopped <= TIME_STOPPED) {
			posPlayer.x -= v;
		}
		if (vaIzq) {
			velocity = VEL + v;
		}
		else {
			velocity = VEL - v;
		}
	}
	else {
		if (vaIzq) {
			velocity = VEL;
		}
		else {
			velocity = VEL;
		}
	}
}