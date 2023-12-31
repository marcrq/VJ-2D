#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"



#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 5
#define INIT_PLAYER_Y_TILES 24

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
	soundMenu = engine->play2D("audio/menu-mario.mp3", true, true, true);
	soundGame = engine->play2D("audio/SuperMarioBros.mp3", true, true, true);
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
		soundMenu->setIsPaused(false);
		level = 0;
		coins = 0;
		lives = 3;
		points = 0;
		maxPoints = 0;
		firstTimeInGameShowScreenDead = true;
		timerAnimationDying = -1.0;
		sumarPuntosTimer = false;
		timerAnimationEndLevel = -1.0;
		initShaders();
		menus = new Menus();
		menus->init(texProgram);
		projection = glm::ortho(0.0f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

		//pantallas de info
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

		//moneda animation
		coin.loadFromFile("images/coin.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteCoin = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.166667, 1.0), &coin, &texProgram);
		spriteCoin->setNumberAnimations(1);
		spriteCoin->setAnimationSpeed(0, 10);
		spriteCoin->addKeyframe(0, glm::vec2(0.f, 1.0f));
		spriteCoin->addKeyframe(0, glm::vec2(0.166667, 1.0f));
		spriteCoin->addKeyframe(0, glm::vec2(0.333333, 1.0f));
		spriteCoin->addKeyframe(0, glm::vec2(0.499999, 1.0f));
		spriteCoin->addKeyframe(0, glm::vec2(0.666666, 1.0f));
		spriteCoin->addKeyframe(0, glm::vec2(0.833333, 1.0f));
		spriteCoin->changeAnimation(0);

		//tema de numeros
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

		mininumbers.loadFromFile("images/mininumbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spritePointsAchivedUnidad = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &mininumbers, &texProgram);
		spritePointsAchivedUnidad->setNumberAnimations(10);
		spritePointsAchivedUnidad->setAnimationSpeed(CERO, 8);
		spritePointsAchivedUnidad->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsAchivedUnidad->setAnimationSpeed(UNO, 8);
		spritePointsAchivedUnidad->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsAchivedUnidad->setAnimationSpeed(DOS, 8);
		spritePointsAchivedUnidad->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsAchivedUnidad->setAnimationSpeed(TRES, 8);
		spritePointsAchivedUnidad->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsAchivedUnidad->setAnimationSpeed(CUATRO, 8);
		spritePointsAchivedUnidad->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsAchivedUnidad->setAnimationSpeed(CINCO, 8);
		spritePointsAchivedUnidad->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsAchivedUnidad->setAnimationSpeed(SEIS, 8);
		spritePointsAchivedUnidad->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsAchivedUnidad->setAnimationSpeed(SIETE, 8);
		spritePointsAchivedUnidad->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsAchivedUnidad->setAnimationSpeed(OCHO, 8);
		spritePointsAchivedUnidad->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsAchivedUnidad->setAnimationSpeed(NUEVE, 8);
		spritePointsAchivedUnidad->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsAchivedUnidad->setPosition(glm::vec2((33) * 16, (1) * 16));

		spritePointsAchivedDecena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &mininumbers, &texProgram);
		spritePointsAchivedDecena->setNumberAnimations(10);
		spritePointsAchivedDecena->setAnimationSpeed(CERO, 8);
		spritePointsAchivedDecena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsAchivedDecena->setAnimationSpeed(UNO, 8);
		spritePointsAchivedDecena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsAchivedDecena->setAnimationSpeed(DOS, 8);
		spritePointsAchivedDecena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsAchivedDecena->setAnimationSpeed(TRES, 8);
		spritePointsAchivedDecena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsAchivedDecena->setAnimationSpeed(CUATRO, 8);
		spritePointsAchivedDecena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsAchivedDecena->setAnimationSpeed(CINCO, 8);
		spritePointsAchivedDecena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsAchivedDecena->setAnimationSpeed(SEIS, 8);
		spritePointsAchivedDecena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsAchivedDecena->setAnimationSpeed(SIETE, 8);
		spritePointsAchivedDecena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsAchivedDecena->setAnimationSpeed(OCHO, 8);
		spritePointsAchivedDecena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsAchivedDecena->setAnimationSpeed(NUEVE, 8);
		spritePointsAchivedDecena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsAchivedDecena->setPosition(glm::vec2((33) * 16, (1) * 16));

		////////////////////COINS////////////////////////////
		spriteCoinsUnidad = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteCoinsUnidad->setNumberAnimations(10);
		spriteCoinsUnidad->setAnimationSpeed(CERO, 8);
		spriteCoinsUnidad->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteCoinsUnidad->setAnimationSpeed(UNO, 8);
		spriteCoinsUnidad->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteCoinsUnidad->setAnimationSpeed(DOS, 8);
		spriteCoinsUnidad->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteCoinsUnidad->setAnimationSpeed(TRES, 8);
		spriteCoinsUnidad->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteCoinsUnidad->setAnimationSpeed(CUATRO, 8);
		spriteCoinsUnidad->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteCoinsUnidad->setAnimationSpeed(CINCO, 8);
		spriteCoinsUnidad->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteCoinsUnidad->setAnimationSpeed(SEIS, 8);
		spriteCoinsUnidad->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteCoinsUnidad->setAnimationSpeed(SIETE, 8);
		spriteCoinsUnidad->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteCoinsUnidad->setAnimationSpeed(OCHO, 8);
		spriteCoinsUnidad->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteCoinsUnidad->setAnimationSpeed(NUEVE, 8);
		spriteCoinsUnidad->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteCoinsUnidad->setPosition(glm::vec2((17) * 16, (1) * 16));

		spriteCoinsDecena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteCoinsDecena->setNumberAnimations(10);
		spriteCoinsDecena->setAnimationSpeed(CERO, 8);
		spriteCoinsDecena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteCoinsDecena->setAnimationSpeed(UNO, 8);
		spriteCoinsDecena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteCoinsDecena->setAnimationSpeed(DOS, 8);
		spriteCoinsDecena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteCoinsDecena->setAnimationSpeed(TRES, 8);
		spriteCoinsDecena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteCoinsDecena->setAnimationSpeed(CUATRO, 8);
		spriteCoinsDecena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteCoinsDecena->setAnimationSpeed(CINCO, 8);
		spriteCoinsDecena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteCoinsDecena->setAnimationSpeed(SEIS, 8);
		spriteCoinsDecena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteCoinsDecena->setAnimationSpeed(SIETE, 8);
		spriteCoinsDecena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteCoinsDecena->setAnimationSpeed(OCHO, 8);
		spriteCoinsDecena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteCoinsDecena->setAnimationSpeed(NUEVE, 8);
		spriteCoinsDecena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteCoinsDecena->setPosition(glm::vec2((16) * 16, (1) * 16));

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

		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	}
	else if (lev == 1) {
		soundMenu->setIsPaused(true);
		if(!firstTimeInGameShowScreenDead) soundGame->setIsPaused(false);
		level = 1;
		showScreenDeadPlayer = false;
		timerScreenDead = 0.f;
		timerLevel = 500;
		personajes.clear();
		endedLevel = false;
		timerAnimationDying = -1.0;
		paintCoin = false;
		showPointsAchived = false;
		timerPointsAchived = 0.0;
		initShaders();
		map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 16, INIT_PLAYER_Y_TILES * 16));
		player->setTileMap(map);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(42 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(77 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(93 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(95 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(142 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(145 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(220 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(223 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(240 * 16, INIT_PLAYER_Y_TILES * 16));
		ktroopa->setTileMap(map);
		personajes.push_back(ktroopa);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(257 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(260 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(263 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(266 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(353 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(356 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		palo_bandera = new ObjetoEntorno();
		palo_bandera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(32, 320), "palo_bandera");
		palo_bandera->setPosition(glm::vec2(198 * 32, 2 * 32));
		palo_bandera->setTileMap(map);

		bandera = new ObjetoEntorno();
		bandera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(64, 32), "bandera");
		bandera->setPosition(glm::vec2(197 * 32, 11 * 32));
		bandera->setTileMap(map);

		currentTime = 0.0f;
		personajes.push_back(nullptr); //necesario para que no pete al hacer desaparecer al ultimo elementod de la lista, comentar para probar
	}
	else if (lev == 2) {
		soundMenu->setIsPaused(true);
		if (!firstTimeInGameShowScreenDead) {
			soundGame->setIsPaused(false);
		}
		//soundGame->setPlayPosition(0.0);
		level = 2;
		showScreenDeadPlayer = false;
		timerScreenDead = 0.f;
		timerLevel = 500;
		personajes.clear();
		endedLevel = false;
		timerAnimationDying = -1.0;
		paintCoin = false;
		showPointsAchived = false;
		timerPointsAchived = 0.0;
		initShaders();
		map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 16, INIT_PLAYER_Y_TILES * 16));
		player->setTileMap(map);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(30 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(33 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(60 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(90 * 16, INIT_PLAYER_Y_TILES * 16));
		ktroopa->setTileMap(map);
		personajes.push_back(ktroopa);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(93 * 16, INIT_PLAYER_Y_TILES * 16));
		ktroopa->setTileMap(map);
		personajes.push_back(ktroopa);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(110 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(116 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(122 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(125 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(140 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(146 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(176 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(205 * 16, INIT_PLAYER_Y_TILES * 16));
		ktroopa->setTileMap(map);
		personajes.push_back(ktroopa);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(220 * 16, INIT_PLAYER_Y_TILES * 16));
		ktroopa->setTileMap(map);
		personajes.push_back(ktroopa);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(250 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(270 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(330 * 16, INIT_PLAYER_Y_TILES * 16));
		ktroopa->setTileMap(map);
		personajes.push_back(ktroopa);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(360 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(370 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(376 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(380 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(386 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);
		
		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(390 * 16, INIT_PLAYER_Y_TILES * 16));
		goomba->setTileMap(map);
		personajes.push_back(goomba);

		palo_bandera = new ObjetoEntorno();
		palo_bandera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(32, 320), "palo_bandera");
		palo_bandera->setPosition(glm::vec2(198 * 32, 2 * 32));
		palo_bandera->setTileMap(map);

		bandera = new ObjetoEntorno();
		bandera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(64, 32), "bandera");
		bandera->setPosition(glm::vec2(197 * 32, 11 * 32));
		bandera->setTileMap(map);

		currentTime = 0.0f;
		personajes.push_back(nullptr); //necesario para que no pete al hacer desaparecer al ultimo elementod de la lista, comentar para probar
	}


}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

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

	if (level != 0) {
		player->update(deltaTime);

		for (Personaje* personaje : personajes) {
			if (personaje != nullptr) {
				personaje->update(deltaTime);
			}
		}

		for (int i = 0; i < map->rewardsLevel.size(); ++i) {
			bool pulsado = std::get<1>(map->rewardsLevel[i]);
			bool consumido = std::get<2>(map->rewardsLevel[i]);
			if (pulsado && !consumido) {
				std::get<2>(map->rewardsLevel[i]) = true;
				if (i % 5 == 0) {
					if (player->isBigFunc()) {
						star = new Star();
						star->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
						star->setPosition(glm::vec2(player->getPosition().x, player->getPosition().y - player->getAltura() - 32));
						star->setTileMap(map);
						personajes.pop_back();
						personajes.push_back(star);
						personajes.push_back(nullptr);
					}
					else {
						seta2 = new Seta();
						seta2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
						seta2->setPosition(glm::vec2(player->getPosition().x, player->getPosition().y - player->getAltura() - 32));
						seta2->setTileMap(map);
						personajes.pop_back();
						personajes.push_back(seta2);
						personajes.push_back(nullptr);
					}
				}
				//sino, +1 moneda
				else {
					++coins;
					actualizarCoins(); 
					engine->play2D(soundCoin);
					paintCoin = true;
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

		palo_bandera->setPosition(player->getRelativePosition());
		bandera->setPosition(player->getRelativePosition());
		bandera->update(deltaTime);

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
					actualizarPointsAchived(p);
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
						actualizarPointsAchived(p);
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
							actualizarPointsAchived(p);
						}
						else {
							player->hit();
							++it;
						}
					}
				}
				else if (tipo == "Ktroopa") {
					if (player->isInvencibleFunc()) { //muere el enemigo
						Ktroopa* kt = dynamic_cast<Ktroopa*>(personaje);
						int p = kt->getPoints();
						points += p;
						actualizarPoints();
						actualizarPointsAchived(p);
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
							actualizarPointsAchived(p);
							player->kickShell();
						}
						else { 
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
					actualizarPointsAchived(p);
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
					actualizarPointsAchived(p);
					it = personajes.erase(it);
					delete personaje;
				}
			}
			++it;
		}

		if ((Game::instance().getKey('f') || Game::instance().getKey('F')) && !showScreenDeadPlayer) {
			borrarPersonajes();
			player->setPosition(glm::vec2(257, 3 * 32), 190 * 32);
		}

		if (player->getPosition().x >= palo_bandera->getPosition().x) { //siguiente nivel
			if (!player->isChangingLevel() && level == 1) {
				borrarPersonajes();
				soundGame->setPlayPosition(0.0);
				soundGame->setIsPaused(true);
				engine->play2D(soundFlapPole);
				engine->play2D(soundComplete);
				player->animacionEndLevelFunc();
				bandera->animacionEndLevelFunc(player->getPosition().y);
				++level;
				endedLevel = true;

				float altura = 401 - player->getPosition().y;
				int pointsAltura = altura / 10;
				if (pointsAltura > 30) pointsAltura = 30;
				points += pointsAltura;
				actualizarPoints();
				actualizarPointsAchived(pointsAltura);
				sumarPuntosTimer = true;
				timerAnimationEndLevel = 0.0;
				
			}
			else if ((timerAnimationEndLevel >= 0 && !player->isChangingLevel() && level == 2 && endedLevel) || sumarPuntosTimer) {
				timerAnimationEndLevel += deltaTime / 1000.0;
				if (timerAnimationEndLevel >= 2.0 && sumarPuntosTimer) {
					timerLevel -= 2;
					points += 2;
					actualizarPoints();
					actualizarTimer();
					if (timerLevel <= 0) {
						sumarPuntosTimer = false;
					}
				}
				if (timerAnimationEndLevel >= 6.0) timerAnimationEndLevel = -1.0;
			}
			else if (!player->isChangingLevel() && level == 2 && endedLevel) { //cambiamos de nivel, se cambia el mapa
				init(2);
				firstTimeInGameShowScreenDead = true;
			}
			else if (!player->isChangingLevel() && level == 2) {
				if (level == 2) {
					borrarPersonajes();
					soundGame->setPlayPosition(0.0);
					soundGame->setIsPaused(true);
					engine->play2D(soundFlapPole);
					engine->play2D(soundComplete);
					player->animacionEndLevelFunc();
					bandera->animacionEndLevelFunc(player->getPosition().y);
					++level;
					endedLevel = true;

					float altura = 401 - player->getPosition().y;
					int pointsAltura = altura / 10;
					if (pointsAltura > 30) pointsAltura = 30;
					points += pointsAltura;
					actualizarPoints();
					actualizarPointsAchived(pointsAltura);
					sumarPuntosTimer = true;
					timerAnimationEndLevel = 0.0;
				}
			}
			else if ((timerAnimationEndLevel >= 0 && !player->isChangingLevel() && level == 3 && endedLevel) || sumarPuntosTimer) {
				timerAnimationEndLevel += deltaTime / 1000.0;
				if (timerAnimationEndLevel >= 2.0 && sumarPuntosTimer) {
					timerLevel -= 2;
					points += 2;
					actualizarPoints();
					actualizarTimer();
					if (timerLevel <= 0) {
						sumarPuntosTimer = false;
					}
				}
				if (timerAnimationEndLevel >= 6.0) timerAnimationEndLevel = -1.0;
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
		if (paintCoin) {
			spriteCoin->setPosition(glm::vec2(player->getPosition().x, player->getPosition().y - player->getAltura()));
			spriteCoin->update(deltaTime);
			timerPaintCoin += deltaTime / 1000.0;
			if (timerPaintCoin >= 1) {
				paintCoin = false;
				timerPaintCoin = 0.f;
			}
		}


		if (showPointsAchived) {
			spritePointsAchivedDecena->setPosition(glm::vec2(player->getPosition().x, player->getPosition().y - player->getAltura()));
			spritePointsAchivedUnidad->setPosition(glm::vec2(player->getPosition().x+10, player->getPosition().y - player->getAltura()));
			timerPointsAchived += deltaTime / 1000.0;
			if (timerPointsAchived >= 1.5) {
				timerPointsAchived = 0.0;
				showPointsAchived = false;
			}
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
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(float)player->getRelativePosition(), 0.0, 0.0)); // +32, posici�n original

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
		if (paintCoin) spriteCoin->render();
		palo_bandera->render();
		bandera->render();
		player->render();

		if (showPointsAchived) {
			if (spritePointsAchivedDecena->animation() != 0) spritePointsAchivedDecena->render();
			spritePointsAchivedUnidad->render();
		}

		spriteTimerCentena->render();
		spriteTimerDecena->render();
		spriteTimerUnidad->render();
		actualizarPoints();
		spritePointsUnidad->render();
		spritePointsDecena->render();
		spritePointsCentena->render();
		
		spriteCoinsUnidad->render();
		spriteCoinsDecena->render();

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
	float right1 = posPlayer.x + 32;
	float left2 = posEnemy.x;
	float right2 = posEnemy.x + 32; 

	float top1 = posPlayer.y - alturaPlayer;
	float bottom1 = posPlayer.y;
	float top2 = posEnemy.y - alturaEnemy;
	float bottom2 = posEnemy.y;

	bool collisionX = (left1 < right2) && (right1 > left2);
	bool collisionY = (top1 < bottom2) && (bottom1 > top2);

	return collisionX && collisionY;
}

bool Scene::esMuerte(glm::vec2 posPlayer, glm::vec2 posEnemy, int alturaPlayer, int alturaEnemy) {
	float left1 = posPlayer.x;
	float right1 = posPlayer.x + 32; 
	float left2 = posEnemy.x;
	float right2 = posEnemy.x + 32; 

	float top1 = posPlayer.y - 32;
	float bottom1 = posPlayer.y;
	float top2 = posEnemy.y - 32;
	float bottom2 = posEnemy.y;

	if (((bottom1 - 4 >= top2 && bottom1 <= top2 + 4) || (bottom1 >= top2 && bottom1-2 <= top2)) && (left1 < right2 && right1 > left2)) return true; //el 37 es porq es dif�cil q coincida justo a la misma altura, as� q le pongo una dif de 5 p�xeles (altura 32)
	else return false;
}

bool Scene::isCollisionLeft(glm::vec2 posPlayer, glm::vec2 posEnemy) {
	float left1 = posPlayer.x;
	float right1 = posPlayer.x + 32; 
	float left2 = posEnemy.x;
	float right2 = posEnemy.x + 32; 

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
	asignarSpriteNumber(spriteCoinsUnidad, coins % 10);
	coins = coins / 10;
	asignarSpriteNumber(spriteCoinsDecena, coins % 10);
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
				if (!g->isPisado()) {
					g->hit();
					int p = g->getPoints();
					points += p;
					actualizarPoints();
					actualizarPointsAchived(p);
				}
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

void Scene::actualizarPointsAchived(int p) {
	asignarSpriteNumber(spritePointsAchivedUnidad, p % 10);
	p = p / 10;
	asignarSpriteNumber(spritePointsAchivedDecena, p % 10);
	showPointsAchived = true;
	int posxDecena = player->getPosition().x + player->getRelativePosition() % 3; //para que tenga un factor de aleatoriedad
	int posy = player->getPosition().x + player->getAltura();
	spritePointsAchivedDecena->setPosition(glm::vec2(posxDecena, posy));
	spritePointsAchivedUnidad->setPosition(glm::vec2(posxDecena+12, posy));
}
