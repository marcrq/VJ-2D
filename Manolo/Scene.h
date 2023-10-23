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
	Seta* seta;
	Ktroopa* ktroopa;
	ObjetoEntorno* bandera;
	ObjetoEntorno* palo_bandera;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int level;
	bool endedLevel;
	Menus* menus;
	std::vector<Personaje*> personajes;
	Texture resumenLevel1, resumenLevel2;
	Sprite* spriteResumenLevel1, *spriteResumenLevel2;

	void nextLevel();
	void borrarPersonajes();
};


#endif // _SCENE_INCLUDE

