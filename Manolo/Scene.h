#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Goomba.h"
#include "Star.h"
#include "Seta.h"
#include "Ktroopa.h"
#include "Personaje.h"
#include "ObjetoEntorno.h"
#include "Menus.h"

#include <irrKlang.h>
using namespace irrklang;

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(int level);
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	bool checkCollision(glm::vec2 posPlayer, glm::vec2 posEnemy, int alturaPlayer, int alturaEnemy);
	bool esMuerte(glm::vec2 posPlayer, glm::vec2 posEnemy, int alturaPlayer, int alturaEnemy);
	bool isCollisionLeft(glm::vec2 posPlayer, glm::vec2 posEnemy);

private:
	TileMap *map;
	Player *player;
	Goomba* goomba;
	Star* star;
	Seta* seta, *seta2;
	Ktroopa* ktroopa, *ktroopa2;
	ObjetoEntorno* bandera;
	ObjetoEntorno* palo_bandera;
	ShaderProgram texProgram;
	float currentTime, timerAnimationDying, timerScreenDead, timerAnimationEndLevel, timerPaintCoin, timerPointsAchived;
	glm::mat4 projection;
	bool endedLevel, showScreenDeadPlayer, firstTimeInGameShowScreenDead, sumarPuntosTimer, paintCoin, showPointsAchived;
	int level, timerLevel, points, maxPoints, coins, lives;
	Menus* menus;
	std::vector<Personaje*> personajes;
	Texture resumenLevel1, resumenLevel2, numbers, screenDeadLevel1, screenDeadLevel2, deadScreenGameOver, timeUp, coin, mininumbers;
	Sprite* spriteResumenLevel1, *spriteResumenLevel2, *spriteTimerCentena, *spriteTimerDecena, *spriteTimerUnidad, * spritePointsUnidad, * spritePointsDecena, * spritePointsCentena, *spriteCoinsUnidad, * spriteCoinsDecena, *spriteNumberOfLives, * spriteScreenDeadLevel1, * spriteScreenDeadLevel2, * spriteDeadScreenGameOver, *spriteTimeUp, *spriteCoin, *spritePointsAchivedUnidad, *spritePointsAchivedDecena;
	//Text textTimerLevel

	ISoundEngine* engine;
	ISound* soundMenu, * soundGame;
	ISoundSource *soundGameOver, * soundTimeUp, * soundFlapPole, * soundComplete, *soundCoin;

	void nextLevel();
	void borrarPersonajes();
	void asignarSpriteNumber(Sprite* s, int n);
	void actualizarTimer();
	void actualizarCoins();
	void actualizarPoints();
	void actualizarPointsAchived(int p);
	void checkCollisionsShell(Personaje* it);
};


#endif // _SCENE_INCLUDE

