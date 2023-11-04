#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <irrKlang.h>
using namespace irrklang;


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPosition(const glm::vec2& pos, int dist);

	void hit();
	void creceMario();
	void invencibility();
	bool isInvencibleFunc();
	void sameAnimationBeetwenModes(int animation);
	glm::vec2 getPosition();
	void hasMadeKill();
	bool isInAnimacionDeadFunc();
	void kickShell(); 
	int getAltura();
	void nextLevel();
	void animacionEndLevelFunc();
	bool isInAnimacionEndLevel();
	bool isChangingLevel();
	void instaKill();
	bool isBigFunc();
	int getRelativePosition();

	float velocity;
	bool thereIsScroll;

private:
	bool bJumping, saltoQuieto, realesedJump, isBig, realesedBig, isInAnimacionAlternarModo, isInvencible, isInvulnerable, realesedInvencible, isInAnimacionDead, kickedAShell, animationEndLevel, running;
	float timerAnimacionAlternarModo, timerIsInvencible, timerHasBeenHitted, timerAnimationDead, timerKickedAShell;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY, conteoCambiosAnimacion, walkedBeyondLimit, movementSafeZone, alturaSprite;
	Texture minimario, bigmario, starmario, starbigmario;
	Sprite* spriteMini;
	Sprite* spriteBig;
	Sprite* spriteMarioStar;
	Sprite* spriteBigMarioStar;
	TileMap* map;

	ISoundEngine* engine;
	ISoundSource* soundMarioDie, *soundMarioGrows, *soundMarioBigJump, * soundMarioSmallJump, *soundMarioKick, *soundMarioCabezazo, *soundMarioKill, * soundMarioPowerDown;

	void animacionAlternarModo();
	Sprite* getCorrectSprite();
};


#endif // _PLAYER_INCLUDE


