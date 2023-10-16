#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 5

#define INIT_GOOMBA_X_TILES 26
#define INIT_GOOMBA_Y_TILES 15
//25

#define INIT_STAR_X_TILES 25
#define INIT_STAR_Y_TILES 25


#define INIT_SETA_X_TILES 30
#define INIT_SETA_Y_TILES 15

#define INIT_KTROOPA_X_TILES 15
#define INIT_KTROOPA_Y_TILES 15


Scene::Scene()
{
	map = NULL;
	player = NULL;
	goomba = NULL;
	star = NULL;
	seta = NULL;
	ktroopa = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (goomba != NULL)
		delete goomba;
	if (star != NULL)
		delete star;
	if (seta != NULL)
		delete seta;
	if (ktroopa != NULL)
		delete ktroopa;
}


void Scene::init(int lev)
{
	if (lev == 1) {
		level = 1;
		endedLevel = false;
		initShaders();
		map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(INIT_GOOMBA_X_TILES * map->getTileSize(), INIT_GOOMBA_Y_TILES * map->getTileSize()));
		goomba->setTileMap(map);

		star = new Star();
		star->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		star->setPosition(glm::vec2(INIT_STAR_X_TILES * map->getTileSize(), INIT_STAR_Y_TILES * map->getTileSize()));
		star->setTileMap(map);

		seta = new Seta();
		seta->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		seta->setPosition(glm::vec2(INIT_SETA_X_TILES * map->getTileSize(), INIT_SETA_Y_TILES * map->getTileSize()));
		seta->setTileMap(map);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(INIT_KTROOPA_X_TILES * map->getTileSize(), INIT_KTROOPA_Y_TILES * map->getTileSize()));
		ktroopa->setTileMap(map);

		palo_bandera = new ObjetoEntorno();
		palo_bandera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(32, 320), "palo_bandera");
		palo_bandera->setPosition(glm::vec2((INIT_SETA_X_TILES - 1) * map->getTileSize(), (INIT_SETA_Y_TILES - 15) * map->getTileSize()));
		palo_bandera->setTileMap(map);

		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
		currentTime = 0.0f;

		//personajes.push_back(goomba);
		personajes.push_back(ktroopa);
		//personajes.push_back(star);
		personajes.push_back(seta);
		personajes.push_back(nullptr); //necesario para que no pete al hacer desaparecer al ultimo elementod de la lista, comentar para probar
	}
	else {
		personajes.clear();
		endedLevel = false;

		initShaders();
		map = TileMap::createTileMap("levels/level03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(INIT_GOOMBA_X_TILES * map->getTileSize(), INIT_GOOMBA_Y_TILES * map->getTileSize()));
		goomba->setTileMap(map);

		star = new Star();
		star->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		star->setPosition(glm::vec2(INIT_STAR_X_TILES * map->getTileSize(), INIT_STAR_Y_TILES * map->getTileSize()));
		star->setTileMap(map);

		seta = new Seta();
		seta->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		seta->setPosition(glm::vec2(INIT_SETA_X_TILES * map->getTileSize(), INIT_SETA_Y_TILES * map->getTileSize()));
		seta->setTileMap(map);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(INIT_KTROOPA_X_TILES * map->getTileSize(), INIT_KTROOPA_Y_TILES * map->getTileSize()));
		ktroopa->setTileMap(map);

		palo_bandera = new ObjetoEntorno();
		palo_bandera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(32, 320), "palo_bandera");
		palo_bandera->setPosition(glm::vec2((INIT_SETA_X_TILES - 1) * map->getTileSize(), (INIT_SETA_Y_TILES - 20) * map->getTileSize()));
		palo_bandera->setTileMap(map);

		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
		currentTime = 0.0f;

		//personajes.push_back(goomba);
		//personajes.push_back(ktroopa);
		personajes.push_back(star);
		personajes.push_back(seta);
		personajes.push_back(nullptr); //necesario para que no pete al hacer desaparecer al ultimo elementod de la lista, comentar para probar

	}
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for (Personaje* personaje : personajes) {
		if (personaje != nullptr) {
			personaje->update(deltaTime);
		}
	}

	vector<Personaje*>::iterator it = personajes.begin();
	while (it != personajes.end()) {
		Personaje* personaje = *it;
		if (!player->isInAnimacionDeadFunc() && personaje != nullptr && checkCollision(player->getPosition(), personaje->getPosition(), player->getAltura(), 32)) {
			string tipo = personaje->myType();
			if (tipo == "Star") {
				player->invencibility();
				it = personajes.erase(it);
				delete personaje;
			}
			else if (tipo == "Goomba") {
				if (player->isInvencibleFunc()) { //muere el enemigo
					//personaje->killed();
					it = personajes.erase(it);
					delete personaje;
				}
				else {
					if (esMuerte(player->getPosition(), personaje->getPosition(), player->getAltura(), 32)) {
						player->hasMadeKill();
						//personaje->hit();
						it = personajes.erase(it);
						/*
						personaje->hit();
						//delete personaje;
						*/
						delete personaje;
					}
					else {
						player->hit();
						++it;
					}
				}
			}
			else if (tipo == "Ktroopa") {
				if (player->isInvencibleFunc()) { //muere el enemigo
					//personaje->hit();
					it = personajes.erase(it);
					delete personaje;
				}
				else {
					Ktroopa* ktroopa = dynamic_cast<Ktroopa*>(personaje);
					if (esMuerte(player->getPosition(), ktroopa->getPosition(), player->getAltura(), 32) && ktroopa->isAlive()) { //de tortuga a shell, le ha saltado en la cabeza
						ktroopa->killed();
						player->hasMadeKill();
					}
					else if (!ktroopa->isAlive() && !ktroopa->isShellMovingFunc()) { //es shell quieta y pasa a moverse, le ha tocado en lado izq o dcha
						if (isCollisionLeft(player->getPosition(), ktroopa->getPosition())) {
							ktroopa->moveShellToRight(true);
						}
						else {
							ktroopa->moveShellToRight(false);
						}
						player->kickShell();
					}
					else { //es shell en movimiento, es hit al player
						player->hit();
					}
					++it;
				}
			}
			else if (tipo == "Seta") {
				player->creceMario();
				it = personajes.erase(it);
				delete personaje;
			}
		}
		++it;
	}
	if (Game::instance().getKey('1')) {
		borrarPersonajes();
		init(1);
	}
	if (Game::instance().getKey('2')) {
		borrarPersonajes();
		init(2);
	}
	if (player->getPosition().x >= palo_bandera->getPosition().x) { //siguiente nivel
		if (!player->isChangingLevel() && level == 1) {
			player->animacionEndLevelFunc();
			++level;
			endedLevel = true;
		}
		else if (!player->isChangingLevel() && level == 2 && endedLevel){ //cambiamos de nivel, se cambia el mapa
			borrarPersonajes();
			nextLevel();
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	for (Personaje* personaje : personajes) {
		if (personaje != nullptr) {
			personaje->render();
		}
	}
	palo_bandera->render();

	player->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

bool Scene::checkCollision(glm::vec2 posPlayer, glm::vec2 posEnemy, int alturaPlayer, int alturaEnemy) {
	float left1 = posPlayer.x;
	float right1 = posPlayer.x + 32; // Debes definir el ancho del objeto
	float left2 = posEnemy.x;
	float right2 = posEnemy.x + 32; // Debes definir el ancho del objeto

	// Calcula los límites de los objetos en el eje Y (teniendo en cuenta la altura)
	float top1 = posPlayer.y - alturaPlayer;
	float bottom1 = posPlayer.y;
	float top2 = posEnemy.y - alturaEnemy;
	float bottom2 = posEnemy.y;

	// Verifica si hay colisión en el eje X
	bool collisionX = (left1 < right2) && (right1 > left2);

	// Verifica si hay colisión en el eje Y (considerando la altura)
	bool collisionY = (top1 < bottom2) && (bottom1 > top2);

	// Si hay colisión en ambos ejes, entonces hay colisión en 2D
	return collisionX && collisionY;
}

bool Scene::esMuerte(glm::vec2 posPlayer, glm::vec2 posEnemy, int alturaPlayer, int alturaEnemy) {
	float left1 = posPlayer.x;
	float right1 = posPlayer.x + 32; // Debes definir el ancho del objeto
	float left2 = posEnemy.x;
	float right2 = posEnemy.x + 32; // Debes definir el ancho del objeto

	// Calcula los límites de los objetos en el eje Y (teniendo en cuenta la altura)
	float top1 = posPlayer.y - alturaPlayer;
	float bottom1 = posPlayer.y;
	float top2 = posEnemy.y - alturaEnemy;
	float bottom2 = posEnemy.y;

	if (bottom1-4 >= top2 && bottom1 <= top2+4 && (left1 < right2 && right1 > left2)) return true; //el 37 es porq es difícil q coincida justo a la misma altura, así q le pongo una dif de 5 píxeles (altura 32)
	else return false;
}

bool Scene::isCollisionLeft(glm::vec2 posPlayer, glm::vec2 posEnemy) {
	float left1 = posPlayer.x;
	float right1 = posPlayer.x + 32; // Debes definir el ancho del objeto
	float left2 = posEnemy.x;
	float right2 = posEnemy.x + 32; // Debes definir el ancho del objeto

	// Si hay colisión en ambos ejes, entonces hay colisión en 2D
	return left1 < left2 && right1 < right2;
}

void Scene::nextLevel() {
	init(2);
}

void Scene::borrarPersonajes() {
	vector<Personaje*>::iterator it2 = personajes.begin();
	while (it2 != personajes.end()) {
		Personaje* personaje = *it2;
		if (personaje != nullptr) {
			it2 = personajes.erase(it2);
			delete personaje;
		}
		++it2;
	}
}


