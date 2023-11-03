#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"



#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 12

#define INIT_GOOMBA_X_TILES 26
#define INIT_GOOMBA_Y_TILES 15
//25

#define INIT_STAR_X_TILES 25
#define INIT_STAR_Y_TILES 17


#define INIT_SETA_X_TILES 60
#define INIT_SETA_Y_TILES 15

#define INIT_KTROOPA_X_TILES 15
#define INIT_KTROOPA_Y_TILES 15

#define MAX_TIME_ANIMATION_DYING 4.0
#define MAX_TIME_SCREEN_DEAD 2.5//2.5

#define TIME_LEVEL 300

Scene::Scene()
{
	map = NULL;
	player = NULL;
	menus = NULL;
	goomba = NULL;
	star = NULL;
	seta = NULL;
	ktroopa2 = NULL;

	engine = createIrrKlangDevice();
	soundGame = engine->play2D("audio/ringtones-super-mario-bros.mp3", true, true, true);
	soundGameOver = engine->addSoundSourceFromFile("audio/smb_gameover.wav");
	soundTimeUp = engine->addSoundSourceFromFile("audio/smb_warning.wav");
	soundFlapPole = engine->addSoundSourceFromFile("audio/smb_flagpole.wav");
	soundComplete = engine->addSoundSourceFromFile("audio/smb_stage_clear.wav");
	soundCoin = engine->addSoundSourceFromFile("audio/smb_coin.wav");
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (menus != NULL)
		delete menus;
	if (goomba != NULL)
		delete goomba;
	if (star != NULL)
		delete star;
	if (seta != NULL)
		delete seta;
	if (ktroopa != NULL)
		delete ktroopa;
	if (ktroopa2 != NULL)
		delete ktroopa2;
}

enum PlayerAnims
{
	CERO, UNO, DOS, TRES, CUATRO, CINCO, SEIS, SIETE, OCHO, NUEVE
};

void Scene::init(int lev) {
	if (lev == 0) {
		level = 0;
		coins = 0;
		lives = 0;
		points = 0;
		maxPoints = 0;
		firstTimeInGameShowScreenDead = true;
		timerAnimationDying = -1.0;
		sumarPuntosTimer = false;
		timerAnimationEndLevel = -1.0;
		initShaders();
		menus = new Menus();
		menus->init(texProgram);
		projection = glm::ortho(-1.0f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

		resumenLevel1.loadFromFile("images/resumenLevel1v2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteResumenLevel1 = Sprite::createSprite(glm::ivec2(640, 42), glm::vec2(1.0, 1.0), &resumenLevel1, &texProgram);
		resumenLevel2.loadFromFile("images/resumenLevel2v2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteResumenLevel2 = Sprite::createSprite(glm::ivec2(640, 42), glm::vec2(1.0, 1.0), &resumenLevel2, &texProgram);
		spriteResumenLevel1->setPosition(glm::vec2(float(0), float(0)));
		spriteResumenLevel2->setPosition(glm::vec2(float(0), float(0)));
		screenDeadLevel1.loadFromFile("images/deadScreenLevel1.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteScreenDeadLevel1 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &screenDeadLevel1, &texProgram);
		spriteScreenDeadLevel1->setPosition(glm::vec2(float(0), float(0)));
		screenDeadLevel2.loadFromFile("images/deadScreenLevel2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteScreenDeadLevel2 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &screenDeadLevel2, &texProgram);
		spriteScreenDeadLevel2->setPosition(glm::vec2(float(0), float(0)));
		deadScreenGameOver.loadFromFile("images/deadScreenGameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteDeadScreenGameOver = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &deadScreenGameOver, &texProgram);
		spriteDeadScreenGameOver->setPosition(glm::vec2(float(0), float(0)));
		timeUp.loadFromFile("images/timeUp.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteTimeUp = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &timeUp, &texProgram);
		spriteTimeUp->setPosition(glm::vec2(float(0), float(0)));
	}
	else if (lev == 1) {
		if(!firstTimeInGameShowScreenDead) soundGame->setIsPaused(false);
		level = 1;
		timerLevel = 500;
		showScreenDeadPlayer = false;
		timerScreenDead = 0.f;
		personajes.clear();
		endedLevel = false;
		timerAnimationDying = -1.0;
		initShaders();
		map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 16, INIT_PLAYER_Y_TILES * 16));
		player->setTileMap(map);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(INIT_GOOMBA_X_TILES * 16, INIT_GOOMBA_Y_TILES * 16));
		goomba->setTileMap(map);

		/*star = new Star();
		star->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		star->setPosition(glm::vec2(INIT_STAR_X_TILES * 16, INIT_STAR_Y_TILES * 16));
		star->setTileMap(map);*/

		seta = new Seta();
		seta->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		seta->setPosition(glm::vec2(INIT_SETA_X_TILES * 16, INIT_SETA_Y_TILES * 16));
		seta->setTileMap(map);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(INIT_KTROOPA_X_TILES * 16, INIT_KTROOPA_Y_TILES * 16));
		ktroopa->setTileMap(map);

		palo_bandera = new ObjetoEntorno();
		palo_bandera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(32, 320), "palo_bandera");
		palo_bandera->setPosition(glm::vec2((INIT_SETA_X_TILES - 1) * 16, (INIT_SETA_Y_TILES - 15) * 16));
		palo_bandera->setTileMap(map);

		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
		currentTime = 0.0f;

		personajes.push_back(goomba);
		//personajes.push_back(ktroopa);
		//personajes.push_back(star);
		personajes.push_back(seta);
		personajes.push_back(nullptr); //necesario para que no pete al hacer desaparecer al ultimo elementod de la lista, comentar para probar

		/////////////////////////////////////77

		numbers.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteTimerCentena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerCentena->setNumberAnimations(10);
		spriteTimerCentena->setAnimationSpeed(CERO, 8);
		spriteTimerCentena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(UNO, 8);
		spriteTimerCentena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(DOS, 8);
		spriteTimerCentena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(TRES, 8);
		spriteTimerCentena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(CUATRO, 8);
		spriteTimerCentena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(CINCO, 8);
		spriteTimerCentena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(SEIS, 8);
		spriteTimerCentena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(SIETE, 8);
		spriteTimerCentena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(OCHO, 8);
		spriteTimerCentena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(NUEVE, 8);
		spriteTimerCentena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerCentena->setPosition(glm::vec2((33) * 16, (1) * 16));

		spriteTimerDecena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerDecena->setNumberAnimations(10);
		spriteTimerDecena->setAnimationSpeed(CERO, 8);
		spriteTimerDecena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(UNO, 8);
		spriteTimerDecena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(DOS, 8);
		spriteTimerDecena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(TRES, 8);
		spriteTimerDecena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(CUATRO, 8);
		spriteTimerDecena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(CINCO, 8);
		spriteTimerDecena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(SEIS, 8);
		spriteTimerDecena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(SIETE, 8);
		spriteTimerDecena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(OCHO, 8);
		spriteTimerDecena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(NUEVE, 8);
		spriteTimerDecena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerDecena->setPosition(glm::vec2((34) * 16, (1) * 16));

		spriteTimerUnidad = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerUnidad->setNumberAnimations(10);
		spriteTimerUnidad->setAnimationSpeed(CERO, 8);
		spriteTimerUnidad->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(UNO, 8);
		spriteTimerUnidad->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(DOS, 8);
		spriteTimerUnidad->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(TRES, 8);
		spriteTimerUnidad->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(CUATRO, 8);
		spriteTimerUnidad->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(CINCO, 8);
		spriteTimerUnidad->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(SEIS, 8);
		spriteTimerUnidad->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(SIETE, 8);
		spriteTimerUnidad->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(OCHO, 8);
		spriteTimerUnidad->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(NUEVE, 8);
		spriteTimerUnidad->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerUnidad->setPosition(glm::vec2((35) * 16, (1) * 16));

		/////////////////POINTS//////////////////
		spritePointsUnidad = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spritePointsUnidad->setNumberAnimations(10);
		spritePointsUnidad->setAnimationSpeed(CERO, 8);
		spritePointsUnidad->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(UNO, 8);
		spritePointsUnidad->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(DOS, 8);
		spritePointsUnidad->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(TRES, 8);
		spritePointsUnidad->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(CUATRO, 8);
		spritePointsUnidad->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(CINCO, 8);
		spritePointsUnidad->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(SEIS, 8);
		spritePointsUnidad->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(SIETE, 8);
		spritePointsUnidad->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(OCHO, 8);
		spritePointsUnidad->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(NUEVE, 8);
		spritePointsUnidad->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsUnidad->setPosition(glm::vec2((5) * 16, (1) * 16));

		spritePointsDecena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spritePointsDecena->setNumberAnimations(10);
		spritePointsDecena->setAnimationSpeed(CERO, 8);
		spritePointsDecena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsDecena->setAnimationSpeed(UNO, 8);
		spritePointsDecena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsDecena->setAnimationSpeed(DOS, 8);
		spritePointsDecena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsDecena->setAnimationSpeed(TRES, 8);
		spritePointsDecena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsDecena->setAnimationSpeed(CUATRO, 8);
		spritePointsDecena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsDecena->setAnimationSpeed(CINCO, 8);
		spritePointsDecena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsDecena->setAnimationSpeed(SEIS, 8);
		spritePointsDecena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsDecena->setAnimationSpeed(SIETE, 8);
		spritePointsDecena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsDecena->setAnimationSpeed(OCHO, 8);
		spritePointsDecena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsDecena->setAnimationSpeed(NUEVE, 8);
		spritePointsDecena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsDecena->setPosition(glm::vec2((4) * 16, (1) * 16));

		spritePointsCentena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spritePointsCentena->setNumberAnimations(10);
		spritePointsCentena->setAnimationSpeed(CERO, 8);
		spritePointsCentena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsCentena->setAnimationSpeed(UNO, 8);
		spritePointsCentena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsCentena->setAnimationSpeed(DOS, 8);
		spritePointsCentena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsCentena->setAnimationSpeed(TRES, 8);
		spritePointsCentena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsCentena->setAnimationSpeed(CUATRO, 8);
		spritePointsCentena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsCentena->setAnimationSpeed(CINCO, 8);
		spritePointsCentena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsCentena->setAnimationSpeed(SEIS, 8);
		spritePointsCentena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsCentena->setAnimationSpeed(SIETE, 8);
		spritePointsCentena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsCentena->setAnimationSpeed(OCHO, 8);
		spritePointsCentena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsCentena->setAnimationSpeed(NUEVE, 8);
		spritePointsCentena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsCentena->setPosition(glm::vec2((3) * 16, (1) * 16));
		////////////////////COINS////////////////////////////
		spriteCoins = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteCoins->setNumberAnimations(10);
		spriteCoins->setAnimationSpeed(CERO, 8);
		spriteCoins->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteCoins->setAnimationSpeed(UNO, 8);
		spriteCoins->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteCoins->setAnimationSpeed(DOS, 8);
		spriteCoins->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteCoins->setAnimationSpeed(TRES, 8);
		spriteCoins->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteCoins->setAnimationSpeed(CUATRO, 8);
		spriteCoins->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteCoins->setAnimationSpeed(CINCO, 8);
		spriteCoins->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteCoins->setAnimationSpeed(SEIS, 8);
		spriteCoins->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteCoins->setAnimationSpeed(SIETE, 8);
		spriteCoins->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteCoins->setAnimationSpeed(OCHO, 8);
		spriteCoins->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteCoins->setAnimationSpeed(NUEVE, 8);
		spriteCoins->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteCoins->setPosition(glm::vec2((16) * 16, (1) * 16));
		////////////////////LIVES////////////////////////////
		spriteNumberOfLives = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteNumberOfLives->setNumberAnimations(10);
		spriteNumberOfLives->setAnimationSpeed(CERO, 8);
		spriteNumberOfLives->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(UNO, 8);
		spriteNumberOfLives->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(DOS, 8);
		spriteNumberOfLives->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(TRES, 8);
		spriteNumberOfLives->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(CUATRO, 8);
		spriteNumberOfLives->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(CINCO, 8);
		spriteNumberOfLives->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(SEIS, 8);
		spriteNumberOfLives->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(SIETE, 8);
		spriteNumberOfLives->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(OCHO, 8);
		spriteNumberOfLives->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(NUEVE, 8);
		spriteNumberOfLives->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteNumberOfLives->setPosition(glm::vec2((22) * 16, (16) * 16));
	}
	else if (lev == 2) {
		if (!firstTimeInGameShowScreenDead) {
			soundGame->setIsPaused(false);
		}
		//soundGame->setPlayPosition(0.0);
		level = 2;
		timerLevel = 500;
		showScreenDeadPlayer = false;
		timerScreenDead = 0.f;
		personajes.clear();
		endedLevel = false;
		timerAnimationDying = -1.0;
		initShaders();
		map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 16, INIT_PLAYER_Y_TILES * 16));
		player->setTileMap(map);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(INIT_GOOMBA_X_TILES * 16, INIT_GOOMBA_Y_TILES * 16));
		goomba->setTileMap(map);

		star = new Star();
		star->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		star->setPosition(glm::vec2(INIT_STAR_X_TILES * 16, INIT_STAR_Y_TILES * 16));
		star->setTileMap(map);

		seta = new Seta();
		seta->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		seta->setPosition(glm::vec2(INIT_SETA_X_TILES * 16, INIT_SETA_Y_TILES * 16));
		seta->setTileMap(map);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(INIT_KTROOPA_X_TILES * 16, INIT_KTROOPA_Y_TILES * 16));
		ktroopa->setTileMap(map);

		ktroopa2 = new Ktroopa();
		ktroopa2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa2->setPosition(glm::vec2((INIT_KTROOPA_X_TILES + 10) * 16, INIT_KTROOPA_Y_TILES * 16));
		ktroopa2->setTileMap(map);

		palo_bandera = new ObjetoEntorno();
		palo_bandera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(32, 320), "palo_bandera");
		palo_bandera->setPosition(glm::vec2((INIT_SETA_X_TILES - 1) * 16, (INIT_SETA_Y_TILES - 20) * 16));
		palo_bandera->setTileMap(map);

		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
		currentTime = 0.0f;

		personajes.push_back(goomba);
		personajes.push_back(ktroopa);
		personajes.push_back(ktroopa2);
		//personajes.push_back(star);
		//personajes.push_back(seta);
		personajes.push_back(nullptr); //necesario para que no pete al hacer desaparecer al ultimo elementod de la lista, comentar para probar


		////////////////////////////////////////////////7
		numbers.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteTimerCentena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerCentena->setNumberAnimations(10);
		spriteTimerCentena->setAnimationSpeed(CERO, 8);
		spriteTimerCentena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(UNO, 8);
		spriteTimerCentena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(DOS, 8);
		spriteTimerCentena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(TRES, 8);
		spriteTimerCentena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(CUATRO, 8);
		spriteTimerCentena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(CINCO, 8);
		spriteTimerCentena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(SEIS, 8);
		spriteTimerCentena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(SIETE, 8);
		spriteTimerCentena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(OCHO, 8);
		spriteTimerCentena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(NUEVE, 8);
		spriteTimerCentena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerCentena->setPosition(glm::vec2((33) * 16, (1) * 16));

		spriteTimerDecena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerDecena->setNumberAnimations(10);
		spriteTimerDecena->setAnimationSpeed(CERO, 8);
		spriteTimerDecena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(UNO, 8);
		spriteTimerDecena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(DOS, 8);
		spriteTimerDecena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(TRES, 8);
		spriteTimerDecena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(CUATRO, 8);
		spriteTimerDecena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(CINCO, 8);
		spriteTimerDecena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(SEIS, 8);
		spriteTimerDecena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(SIETE, 8);
		spriteTimerDecena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(OCHO, 8);
		spriteTimerDecena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(NUEVE, 8);
		spriteTimerDecena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerDecena->setPosition(glm::vec2((34) * 16, (1) * 16));

		spriteTimerUnidad = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerUnidad->setNumberAnimations(10);
		spriteTimerUnidad->setAnimationSpeed(CERO, 8);
		spriteTimerUnidad->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(UNO, 8);
		spriteTimerUnidad->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(DOS, 8);
		spriteTimerUnidad->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(TRES, 8);
		spriteTimerUnidad->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(CUATRO, 8);
		spriteTimerUnidad->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(CINCO, 8);
		spriteTimerUnidad->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(SEIS, 8);
		spriteTimerUnidad->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(SIETE, 8);
		spriteTimerUnidad->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(OCHO, 8);
		spriteTimerUnidad->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(NUEVE, 8);
		spriteTimerUnidad->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerUnidad->setPosition(glm::vec2((35) * 16, (1) * 16));

		/////////////////POINTS//////////////////
		spritePointsUnidad = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spritePointsUnidad->setNumberAnimations(10);
		spritePointsUnidad->setAnimationSpeed(CERO, 8);
		spritePointsUnidad->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(UNO, 8);
		spritePointsUnidad->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(DOS, 8);
		spritePointsUnidad->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(TRES, 8);
		spritePointsUnidad->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(CUATRO, 8);
		spritePointsUnidad->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(CINCO, 8);
		spritePointsUnidad->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(SEIS, 8);
		spritePointsUnidad->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(SIETE, 8);
		spritePointsUnidad->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(OCHO, 8);
		spritePointsUnidad->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(NUEVE, 8);
		spritePointsUnidad->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsUnidad->setPosition(glm::vec2((5) * 16, (1) * 16));

		spritePointsDecena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spritePointsDecena->setNumberAnimations(10);
		spritePointsDecena->setAnimationSpeed(CERO, 8);
		spritePointsDecena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsDecena->setAnimationSpeed(UNO, 8);
		spritePointsDecena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsDecena->setAnimationSpeed(DOS, 8);
		spritePointsDecena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsDecena->setAnimationSpeed(TRES, 8);
		spritePointsDecena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsDecena->setAnimationSpeed(CUATRO, 8);
		spritePointsDecena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsDecena->setAnimationSpeed(CINCO, 8);
		spritePointsDecena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsDecena->setAnimationSpeed(SEIS, 8);
		spritePointsDecena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsDecena->setAnimationSpeed(SIETE, 8);
		spritePointsDecena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsDecena->setAnimationSpeed(OCHO, 8);
		spritePointsDecena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsDecena->setAnimationSpeed(NUEVE, 8);
		spritePointsDecena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsDecena->setPosition(glm::vec2((4) * 16, (1) * 16));

		spritePointsCentena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spritePointsCentena->setNumberAnimations(10);
		spritePointsCentena->setAnimationSpeed(CERO, 8);
		spritePointsCentena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsCentena->setAnimationSpeed(UNO, 8);
		spritePointsCentena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsCentena->setAnimationSpeed(DOS, 8);
		spritePointsCentena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsCentena->setAnimationSpeed(TRES, 8);
		spritePointsCentena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsCentena->setAnimationSpeed(CUATRO, 8);
		spritePointsCentena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsCentena->setAnimationSpeed(CINCO, 8);
		spritePointsCentena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsCentena->setAnimationSpeed(SEIS, 8);
		spritePointsCentena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsCentena->setAnimationSpeed(SIETE, 8);
		spritePointsCentena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsCentena->setAnimationSpeed(OCHO, 8);
		spritePointsCentena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsCentena->setAnimationSpeed(NUEVE, 8);
		spritePointsCentena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsCentena->setPosition(glm::vec2((3) * 16, (1) * 16));
		////////////////////COINS////////////////////////////
		spriteCoins = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteCoins->setNumberAnimations(10);
		spriteCoins->setAnimationSpeed(CERO, 8);
		spriteCoins->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteCoins->setAnimationSpeed(UNO, 8);
		spriteCoins->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteCoins->setAnimationSpeed(DOS, 8);
		spriteCoins->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteCoins->setAnimationSpeed(TRES, 8);
		spriteCoins->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteCoins->setAnimationSpeed(CUATRO, 8);
		spriteCoins->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteCoins->setAnimationSpeed(CINCO, 8);
		spriteCoins->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteCoins->setAnimationSpeed(SEIS, 8);
		spriteCoins->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteCoins->setAnimationSpeed(SIETE, 8);
		spriteCoins->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteCoins->setAnimationSpeed(OCHO, 8);
		spriteCoins->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteCoins->setAnimationSpeed(NUEVE, 8);
		spriteCoins->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteCoins->setPosition(glm::vec2((16) * 16, (1) * 16));
		////////////////////LIVES////////////////////////////
		spriteNumberOfLives = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteNumberOfLives->setNumberAnimations(10);
		spriteNumberOfLives->setAnimationSpeed(CERO, 8);
		spriteNumberOfLives->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(UNO, 8);
		spriteNumberOfLives->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(DOS, 8);
		spriteNumberOfLives->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(TRES, 8);
		spriteNumberOfLives->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(CUATRO, 8);
		spriteNumberOfLives->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(CINCO, 8);
		spriteNumberOfLives->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(SEIS, 8);
		spriteNumberOfLives->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(SIETE, 8);
		spriteNumberOfLives->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(OCHO, 8);
		spriteNumberOfLives->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteNumberOfLives->setAnimationSpeed(NUEVE, 8);
		spriteNumberOfLives->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteNumberOfLives->setPosition(glm::vec2((22) * 16, (16) * 16));
	}


}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (level != 0) {
		player->update(deltaTime);

		for (Personaje* personaje : personajes) {
			if (personaje != nullptr) {
				personaje->update(deltaTime);
			}
		}

		if (level == 1) {
			if (map->pulsado) {
				/*seta2 = new Seta();
				seta2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				seta2->setPosition(glm::vec2(player->getPosition().x, 16 * 16));
				seta2->setTileMap(map);*/
				/*star = new Star();
				star->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				star->setPosition(glm::vec2(player->getPosition().x, 16 * 16));
				star->setTileMap(map);
				personajes.pop_back();
				personajes.push_back(star);
				personajes.push_back(nullptr);*/
			}
			for (auto& reward : map->rewardsLevel1) {
				bool pulsado = std::get<1>(reward);
				bool consumido = std::get<2>(reward);
				if (pulsado && !consumido) {
 					std::get<2>(reward) = true;
					if (std::get<0>(reward) == 514) {
						seta2 = new Seta();
						seta2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
						seta2->setPosition(glm::vec2(player->getPosition().x, 16 * 16));
						seta2->setTileMap(map);
						personajes.pop_back();
						personajes.push_back(seta2);
						personajes.push_back(nullptr);
					}
					//aquí poner otras posiciones donde crear setas o estrellas
					else if (std::get<0>(reward) == 99999) {

					}
					//sinó, +1 moneda
					else {
						++coins;
						engine->play2D(soundCoin);
					}
				}
			}
		}

		if (player->isInAnimacionDeadFunc() && timerAnimationDying == -1.0) {
			soundGame->setIsPaused(true);
			timerAnimationDying = 0.;
		}
		if (timerAnimationDying != -1.0) {
			timerAnimationDying += deltaTime / 1000.0;
		}
		if ((timerAnimationDying >= MAX_TIME_ANIMATION_DYING && !showScreenDeadPlayer) || firstTimeInGameShowScreenDead) {
			if (!firstTimeInGameShowScreenDead) lives -= 1;
			showScreenDeadPlayer = true;
			if (timerLevel <= 0 && lives != -1) engine->play2D(soundTimeUp);
			else if(lives <= -1) engine->play2D(soundGameOver);
			timerAnimationDying = -1.0;
		}
		if (showScreenDeadPlayer) {
			timerScreenDead += deltaTime / 1000.0;

			if (timerScreenDead >= MAX_TIME_SCREEN_DEAD) {
				showScreenDeadPlayer = false;
				firstTimeInGameShowScreenDead = false;
				timerScreenDead = 0.f;
				borrarPersonajes();
				if(lives != -1) init(level);
				else init(0);
			}
		}

		//CONTROL DE ERRORES
		if (Game::instance().getKey('e')) {
			lives = lives;
		}

		//ajustar velocidades
		vector<Personaje*>::iterator it = personajes.begin();
		while (it != personajes.end()) {
			Personaje* personaje = *it;
			string tipo;
			if (personaje != nullptr) tipo = personaje->myType();
			if (tipo == "Seta") {
				Seta* s = dynamic_cast<Seta*>(personaje);
				s->changeVelocitiesScroll(player->thereIsScroll, player->velocity);
			}
			else if (tipo == "Goomba") {
				Goomba* s = dynamic_cast<Goomba*>(personaje);
				s->changeVelocitiesScroll(player->thereIsScroll, player->velocity);
			}
			else if (tipo == "Ktroopa") {
				Ktroopa* s = dynamic_cast<Ktroopa*>(personaje);
				s->changeVelocitiesScroll(player->thereIsScroll, player->velocity);
			}
			else if (tipo == "Star") {
				Star* s = dynamic_cast<Star*>(personaje);
				s->changeVelocitiesScroll(player->thereIsScroll, player->velocity);
			}
			++it;
		}

		/////////////////7
		it = personajes.begin(); //vector<Personaje*>::iterator
		while (it != personajes.end()) {
			Personaje* personaje = *it;
			string tipo;
			if (personaje != nullptr) tipo = personaje->myType();
			if (!firstTimeInGameShowScreenDead && !player->isInAnimacionEndLevel() && !player->isInAnimacionDeadFunc() && personaje != nullptr && checkCollision(player->getPosition(), personaje->getPosition(), player->getAltura(), 32)) {
				if (tipo == "Star") {
					player->invencibility();
					Star* s = dynamic_cast<Star*>(personaje);
					int p = s->getPoints();
					points += p;
					actualizarPoints();
					it = personajes.erase(it);
					delete personaje;
				}
				else if (tipo == "Goomba") {
					Goomba* g = dynamic_cast<Goomba*>(personaje);
					if (player->isInvencibleFunc()) { //muere el enemigo
						//personaje->killed();
						int p = g->getPoints();
						points += p;
						actualizarPoints();
						it = personajes.erase(it);
						delete personaje;
					}
					else if(!g->isPisado()) {
						if (esMuerte(player->getPosition(), personaje->getPosition(), player->getAltura(), 32)) {
							player->hasMadeKill();
							g->hit();
							int p = g->getPoints();
							points += p;
							actualizarPoints();
						}
						else {
							//bool isBig = player->isBigFunc();
							/*if (!isBig) {
								soundGame->setIsPaused(true);
							}*/
							player->hit();
							++it;
						}
					}
				}
				else if (tipo == "Ktroopa") {
					if (player->isInvencibleFunc()) { //muere el enemigo
						//personaje->hit();
						Ktroopa* kt = dynamic_cast<Ktroopa*>(personaje);
						int p = kt->getPoints();
						points += p;
						actualizarPoints();
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
							Ktroopa* kt = dynamic_cast<Ktroopa*>(personaje);
							int p = kt->getPoints();
							points += p;
							actualizarPoints();
							player->kickShell();
						}
						else { //es shell en movimiento, es hit al player
							//if (!player->isBigFunc()) soundGame->setIsPaused(true);
							player->hit();
						}
						++it;
					}
				}
				else if (tipo == "Seta") {
					player->creceMario();
					Seta* s = dynamic_cast<Seta*>(personaje);
					int p = s->getPoints();
					points += p;
					actualizarPoints();
					it = personajes.erase(it);
					delete personaje;
				}
			}

			++it;
		}

		//para todas las shells en movimiento, miro si colisionan
		it = personajes.begin();
		while (it != personajes.end()) {
			Personaje* personaje = *it;
			string tipo;
			if (personaje != nullptr) tipo = personaje->myType();
			if (tipo == "Ktroopa") {
				Ktroopa* kt = dynamic_cast<Ktroopa*>(personaje);
				if (kt->isShellMovingFunc()) {
					checkCollisionsShell(personaje);
				}
			}
			++it;
		}

		//eliminar los tocados por shell en movimiento, o los goombas pisados
		it = personajes.begin();
		while (it != personajes.end()) {
			Personaje* personaje = *it;
			string tipo;
			if (personaje != nullptr) tipo = personaje->myType();
			if (tipo == "Goomba") {
				Goomba* g = dynamic_cast<Goomba*>(personaje);
				if (!g->vivo) {
					int p = g->getPoints();
					points += p;
					actualizarPoints();
					it = personajes.erase(it);
					delete personaje;
				}
			}
			else if (tipo == "Ktroopa") {
				Ktroopa* kt = dynamic_cast<Ktroopa*>(personaje);
				if (!kt->vivo) {
					int p = kt->getPoints();
					points += p;
					actualizarPoints();
					it = personajes.erase(it);
					delete personaje;
				}
			}
			++it;
		}

		if (Game::instance().getKey('1')) {
			lives = 3;
			borrarPersonajes();
			init(1);
		}
		if (Game::instance().getKey('2')) {
			lives = 3;
			borrarPersonajes();
			init(2);
		}
		if (player->getPosition().x >= palo_bandera->getPosition().x) { //siguiente nivel
			if ((!player->isChangingLevel() && level == 1) || (level == 2 && sumarPuntosTimer)) {
				if (level == 1) {
					soundGame->setPlayPosition(0.0);
					soundGame->setIsPaused(true);
					engine->play2D(soundFlapPole);
					engine->play2D(soundComplete);
					player->animacionEndLevelFunc();
					++level;
					endedLevel = true;

					float altura = 401 - player->getPosition().y;
					int pointsAltura = altura / 10;
					points += pointsAltura;
					actualizarPoints();
					sumarPuntosTimer = true;
					timerAnimationEndLevel = 0.0;
				}
				if (sumarPuntosTimer) {
					--timerLevel;
					++points;
					actualizarPoints();
					actualizarTimer();
					if (timerLevel == 0) sumarPuntosTimer = false;
				}
			}
			else if (timerAnimationEndLevel >= 0 && !player->isChangingLevel() && level == 2 && endedLevel) {
				timerAnimationEndLevel += deltaTime / 1000.0;
				if (timerAnimationEndLevel >= 5.0) timerAnimationEndLevel = -1.0;
			}
			else if (!player->isChangingLevel() && level == 2 && endedLevel) { //cambiamos de nivel, se cambia el mapa
				borrarPersonajes();
				init(2);
				firstTimeInGameShowScreenDead = true;
			}
			else if ((!player->isChangingLevel() && level == 2) || (level == 3 && sumarPuntosTimer)) {
				if (level == 2) {
					soundGame->setPlayPosition(0.0);
					soundGame->setIsPaused(true);
					engine->play2D(soundFlapPole);
					engine->play2D(soundComplete);
					player->animacionEndLevelFunc();
					++level;
					endedLevel = true;

					float altura = 401 - player->getPosition().y;
					int pointsAltura = altura / 10;
					points += pointsAltura;
					actualizarPoints();
					sumarPuntosTimer = true;
					timerAnimationEndLevel = 0.0;
				}
				if (sumarPuntosTimer) {
					--timerLevel;
					++points;
					actualizarPoints();
					actualizarTimer();
					if (timerLevel == 0) sumarPuntosTimer = false;
				}
			}
			else if (timerAnimationEndLevel >= 0 && !player->isChangingLevel() && level == 3 && endedLevel) {
				timerAnimationEndLevel += deltaTime / 1000.0;
				if (timerAnimationEndLevel >= 5.0) timerAnimationEndLevel = -1.0;
			}
			else if (!player->isChangingLevel() && level == 3 && endedLevel) { //cambiamos de nivel, se cambia a los creditos
				menus->showingCreditsFunc();
				if (points > maxPoints) maxPoints = points;
				menus->setMaxPoints(maxPoints);
				points = 0;
				level = 0;
			}
		}

		if (player->getPosition().x < palo_bandera->getPosition().x && !player->isInAnimacionDeadFunc() && !showScreenDeadPlayer) {
			timerLevel = TIME_LEVEL - static_cast<int>(currentTime) / 1000;
			if (timerLevel == 0) {
				player->instaKill();
				soundGame->setIsPaused(true);
			}
			actualizarTimer();
		}
	}
	else {
		int chosed;
		menus->update(deltaTime, chosed);
		if (chosed != 0) {
			init(chosed);
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	if (level != 0) {
		// Create a translation matrix and translate it by a certain amount
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(float)player->getRelativePosition(), 0.0, 0.0)); // +32, posición original

		// Multiply the translation matrix with the modelview matrix
		modelview = translationMatrix;

		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

		map->setRelativePosition(player->getRelativePosition());
		map->render();
		if (level == 1 || (level == 2 && player->isChangingLevel()) || (level == 2 && sumarPuntosTimer) || (level == 2 && timerAnimationEndLevel != -1)) spriteResumenLevel1->render();
		else if (level == 2 || (level == 3 && player->isChangingLevel()) || (level == 3 && sumarPuntosTimer) || (level == 3 && timerAnimationEndLevel != -1)) spriteResumenLevel2->render();
		for (Personaje* personaje : personajes) {
			if (personaje != nullptr) {
				modelview = glm::mat4(1.0f);
				texProgram.setUniformMatrix4f("modelview", modelview);
				personaje->render();
			}
		}
		palo_bandera->render();
		player->render();

		spriteTimerCentena->render();
		spriteTimerDecena->render();
		spriteTimerUnidad->render();
		actualizarPoints();
		spritePointsUnidad->render();
		spritePointsDecena->render();
		spritePointsCentena->render();
		actualizarCoins(); //PONER DONDE SE GANA MONEDA Y QUITAR DE AQUÍ
		spriteCoins->render();

		if (showScreenDeadPlayer && lives != -1) {
			if (timerLevel <= 0) spriteTimeUp->render();
			else if (level == 1) spriteScreenDeadLevel1->render();
			else spriteScreenDeadLevel2->render();
			asignarSpriteNumber(spriteNumberOfLives, lives);
			if (timerLevel > 0) spriteNumberOfLives->render();
		}
		else if (showScreenDeadPlayer && lives <= -1) {
			spriteDeadScreenGameOver->render();
		}
	}
	else menus->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
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

	// Calcula los lï¿½mites de los objetos en el eje Y (teniendo en cuenta la altura)
	float top1 = posPlayer.y - alturaPlayer;
	float bottom1 = posPlayer.y;
	float top2 = posEnemy.y - alturaEnemy;
	float bottom2 = posEnemy.y;

	// Verifica si hay colisiï¿½n en el eje X
	bool collisionX = (left1 < right2) && (right1 > left2);

	// Verifica si hay colisiï¿½n en el eje Y (considerando la altura)
	bool collisionY = (top1 < bottom2) && (bottom1 > top2);

	// Si hay colisiï¿½n en ambos ejes, entonces hay colisiï¿½n en 2D
	return collisionX && collisionY;
}

bool Scene::esMuerte(glm::vec2 posPlayer, glm::vec2 posEnemy, int alturaPlayer, int alturaEnemy) {
	float left1 = posPlayer.x;
	float right1 = posPlayer.x + 32; // Debes definir el ancho del objeto
	float left2 = posEnemy.x;
	float right2 = posEnemy.x + 32; // Debes definir el ancho del objeto

	// Calcula los lï¿½mites de los objetos en el eje Y (teniendo en cuenta la altura)
	float top1 = posPlayer.y - 32;
	float bottom1 = posPlayer.y;
	float top2 = posEnemy.y - 32;
	float bottom2 = posEnemy.y;

	if (((bottom1 - 4 >= top2 && bottom1 <= top2 + 4) || (bottom1 >= top2 && bottom1-2 <= top2)) && (left1 < right2 && right1 > left2)) return true; //el 37 es porq es difï¿½cil q coincida justo a la misma altura, asï¿½ q le pongo una dif de 5 pï¿½xeles (altura 32)
	else return false;
}

bool Scene::isCollisionLeft(glm::vec2 posPlayer, glm::vec2 posEnemy) {
	float left1 = posPlayer.x;
	float right1 = posPlayer.x + 32; // Debes definir el ancho del objeto
	float left2 = posEnemy.x;
	float right2 = posEnemy.x + 32; // Debes definir el ancho del objeto

	// Si hay colisiï¿½n en ambos ejes, entonces hay colisiï¿½n en 2D
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

void Scene::asignarSpriteNumber(Sprite* s, int n) {
	if (n < 0 || n > 9) s->changeAnimation(0);
	else s->changeAnimation(n);
}

void Scene::actualizarTimer() {
	int aux = timerLevel;
	asignarSpriteNumber(spriteTimerUnidad, timerLevel % 10);
	timerLevel = timerLevel / 10;
	asignarSpriteNumber(spriteTimerDecena, timerLevel % 10);
	timerLevel = timerLevel / 10;
	asignarSpriteNumber(spriteTimerCentena, timerLevel % 10);
	timerLevel = aux;
}

void Scene::actualizarCoins() {
	int aux = coins;
	asignarSpriteNumber(spriteCoins, coins % 10);
	coins = aux;
}


void Scene::actualizarPoints() {
	int aux = points;
	asignarSpriteNumber(spritePointsUnidad, points % 10);
	points = points / 10;
	asignarSpriteNumber(spritePointsDecena, points % 10);
	points = points / 10;
	asignarSpriteNumber(spritePointsCentena, points % 10);
	points = aux;
}

void Scene::checkCollisionsShell(Personaje* personajeShell) {
	vector<Personaje*>::iterator it2 = personajes.begin();
	Ktroopa* shell = dynamic_cast<Ktroopa*>(personajeShell);
	while (it2 != personajes.end()) {
		Personaje* personaje2 = *it2;
		string tipo;
		if (personaje2 != nullptr) tipo = personaje2->myType();
		if (personaje2 != nullptr && personaje2 != personajeShell && (tipo == "Goomba" || tipo == "Ktroopa") && checkCollision(shell->getPosition(), personaje2->getPosition(), 32, 32)) {
			if (tipo == "Goomba") {
				Goomba* g = dynamic_cast<Goomba*>(personaje2);
				if(!g->isPisado()) g->hit();
			}
			else if (tipo == "Ktroopa" && personajeShell != personaje2) {
				Ktroopa* kt = dynamic_cast<Ktroopa*>(personaje2);
				kt->vivo = false;
			}
		}
		tipo = "null";
		++it2;
	}
}
