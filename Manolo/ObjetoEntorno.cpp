#include "ObjetoEntorno.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ACCELERATION 0.002f
#define MAX_VEL 3.f


enum ObjetoEntornoAnims
{
	STAND
};


void ObjetoEntorno::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::vec2 size, string nameSprite) {
	texture.loadFromFile("images/" + nameSprite + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(size.x, size.y), glm::vec2(1.f, 1.f), &texture, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STAND, 8);
	sprite->addKeyframe(STAND, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;

	finalPos = 0;

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void ObjetoEntorno::update(int deltaTime) {
	sprite->update(deltaTime);
	
	if (finalPos != 0) {
		if (pos.y > finalPos)
			pos.y -= 1.f;
	}

	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void ObjetoEntorno::render() {
	sprite->render();
}

void ObjetoEntorno::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void ObjetoEntorno::setPosition(const glm::vec2& pos) {
	this->pos = pos;
	initialPos = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + this->pos.x), float(tileMapDispl.y + this->pos.y)));
}

void ObjetoEntorno::setPosition(int dist) {
	pos.x = initialPos.x - dist;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

glm::vec2 ObjetoEntorno::getPosition() {
	return pos;
}

void ObjetoEntorno::animacionEndLevelFunc(float posY) {
	finalPos = posY;
}