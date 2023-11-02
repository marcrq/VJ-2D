#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ACCELERATION 0.002f
#define MAX_VEL 3.f
#define TIME_INVINCIBILITY 2.f //cuando la estrella 12
#define TIME_INVULNERABILITY 1.0 //cuando te pegan
#define LIGHT_INVULNERABILITY 0.2 //para que no cuente como hit cuando chuto shell

#define SCROLL_LIMIT 192.0



enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_RIGHT, JUMP_LEFT, DEAD
};

bool runningG = false;

//static void keyboardCallback(unsigned char key, int x, int y)
//{
//	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
//		runningG = true;
//	}
//	else {
//		runningG = false;
//	}
//}

static void specialDownCallback(int key, int x, int y)
{
	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		runningG = true;
	}
	else {
		runningG = false;
	}
}

// If a special key is released this callback is called

static void specialUpCallback(int key, int x, int y)
{
	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		runningG = true;
	}
	else {
		runningG = false;
	}
}


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{

	//glutSpecialFunc(specialDownCallback);
	//glutSpecialUpFunc(specialUpCallback);

	
	engine = createIrrKlangDevice();
	// play some sound stream, looped
	//ISound* backgroundMusic = engine->play2D("audio/ringtones-super-mario-bros.mp3", true);
	soundMarioDie = engine->addSoundSourceFromFile("audio/mariodie.wav");
	soundMarioGrows = engine->addSoundSourceFromFile("audio/smb_powerup.wav");
	soundMarioBigJump = engine->addSoundSourceFromFile("audio/smb_jump-super.wav");
	soundMarioSmallJump = engine->addSoundSourceFromFile("audio/smb_jump-small.wav");
	soundMarioKick = engine->addSoundSourceFromFile("audio/smb_kick.wav");
	soundMarioCabezazo = engine->addSoundSourceFromFile("audio/smb_bump.wav");
	soundMarioKill = engine->addSoundSourceFromFile("audio/smb_stomp.wav");
	soundMarioPowerDown = engine->addSoundSourceFromFile("audio/smb_powerdown.wav");

	bJumping = false;
	realesedJump = true;
	velocity = 2.f;
	isBig = false;
	realesedBig = true;
	isInAnimacionAlternarModo = false;
	saltoQuieto = false;
	conteoCambiosAnimacion = 0;
	isInvencible = false;
	isInvulnerable = false;
	timerHasBeenHitted = 0.f;
	realesedInvencible = true;
	timerAnimationDead = 0.f;
	kickedAShell = false;
	timerKickedAShell = 0.f;
	alturaSprite = 32;
	animationEndLevel = false;
	walkedBeyondLimit = 0;
	running = false;
	movementSafeZone = 0;
	//glutKeyboardFunc(keyboardCallback);

	minimario.loadFromFile("images/minimariosTODOS.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteMini = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.33333, 0.25), &minimario, &shaderProgram);
	spriteMini->setNumberAnimations(7);

	spriteMini->setAnimationSpeed(STAND_LEFT, 8);
	spriteMini->addKeyframe(STAND_LEFT, glm::vec2(0.66666f, 0.5f));

	spriteMini->setAnimationSpeed(STAND_RIGHT, 8);
	spriteMini->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	spriteMini->setAnimationSpeed(DEAD, 8);
	spriteMini->addKeyframe(DEAD, glm::vec2(0.66666f, 0.f));
	spriteMini->addKeyframe(DEAD, glm::vec2(0.66666f, 0.f));
	spriteMini->addKeyframe(DEAD, glm::vec2(0.66666f, 0.f));
	spriteMini->addKeyframe(DEAD, glm::vec2(0.66666f, 0.f));
	spriteMini->addKeyframe(DEAD, glm::vec2(0.66666f, 0.f));

	spriteMini->setAnimationSpeed(MOVE_LEFT, 9);
	spriteMini->addKeyframe(MOVE_LEFT, glm::vec2(0.66666f, 0.75f));
	spriteMini->addKeyframe(MOVE_LEFT, glm::vec2(0.33333f, 0.75f));
	spriteMini->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.75f));

	spriteMini->setAnimationSpeed(MOVE_RIGHT, 9);
	spriteMini->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.25f));
	spriteMini->addKeyframe(MOVE_RIGHT, glm::vec2(0.33333, 0.25f));
	spriteMini->addKeyframe(MOVE_RIGHT, glm::vec2(0.66666, 0.25f));

	spriteMini->setAnimationSpeed(JUMP_RIGHT, 8);
	spriteMini->addKeyframe(JUMP_RIGHT, glm::vec2(0.33333, 0.f));

	spriteMini->setAnimationSpeed(JUMP_LEFT, 8);
	spriteMini->addKeyframe(JUMP_LEFT, glm::vec2(0.33333, 0.5f));

	spriteMini->changeAnimation(MOVE_RIGHT);
	tileMapDispl = tileMapPos;
	spriteMini->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//--------------------
	starmario.loadFromFile("images/minimariosTODOSESTRELLA.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteMarioStar = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.33333, 0.25), &starmario, &shaderProgram);
	spriteMarioStar->setNumberAnimations(7);

	spriteMarioStar->setAnimationSpeed(STAND_LEFT, 8);
	spriteMarioStar->addKeyframe(STAND_LEFT, glm::vec2(0.66666f, 0.5f));

	spriteMarioStar->setAnimationSpeed(STAND_RIGHT, 8);
	spriteMarioStar->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	spriteMarioStar->setAnimationSpeed(DEAD, 8);
	spriteMarioStar->addKeyframe(DEAD, glm::vec2(0.66666f, 0.f));
	spriteMarioStar->addKeyframe(DEAD, glm::vec2(0.66666f, 0.f));
	spriteMarioStar->addKeyframe(DEAD, glm::vec2(0.66666f, 0.f));
	spriteMarioStar->addKeyframe(DEAD, glm::vec2(0.66666f, 0.f));
	spriteMarioStar->addKeyframe(DEAD, glm::vec2(0.66666f, 0.f));

	spriteMarioStar->setAnimationSpeed(MOVE_LEFT, 9);
	spriteMarioStar->addKeyframe(MOVE_LEFT, glm::vec2(0.66666f, 0.75f));
	spriteMarioStar->addKeyframe(MOVE_LEFT, glm::vec2(0.33333f, 0.75f));
	spriteMarioStar->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.75f));

	spriteMarioStar->setAnimationSpeed(MOVE_RIGHT, 9);
	spriteMarioStar->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.25f));
	spriteMarioStar->addKeyframe(MOVE_RIGHT, glm::vec2(0.33333, 0.25f));
	spriteMarioStar->addKeyframe(MOVE_RIGHT, glm::vec2(0.66666, 0.25f));

	spriteMarioStar->setAnimationSpeed(JUMP_RIGHT, 8);
	spriteMarioStar->addKeyframe(JUMP_RIGHT, glm::vec2(0.33333, 0.f));

	spriteMarioStar->setAnimationSpeed(JUMP_LEFT, 8);
	spriteMarioStar->addKeyframe(JUMP_LEFT, glm::vec2(0.33333, 0.5f));

	spriteMarioStar->changeAnimation(MOVE_RIGHT);
	spriteMarioStar->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//--------------------
	
	bigmario.loadFromFile("images/mariosTODOS.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteBig = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.2, 0.5), &bigmario, &shaderProgram);
	spriteBig->setNumberAnimations(7);

	spriteBig->setAnimationSpeed(STAND_LEFT, 8);
	spriteBig->addKeyframe(STAND_LEFT, glm::vec2(0.8f, 0.5f));

	spriteBig->setAnimationSpeed(STAND_RIGHT, 8);
	spriteBig->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	spriteBig->setAnimationSpeed(DEAD, 8);
	spriteBig->addKeyframe(DEAD, glm::vec2(0.f, 0.66666f));

	spriteBig->setAnimationSpeed(MOVE_LEFT, 9);
	spriteBig->addKeyframe(MOVE_LEFT, glm::vec2(0.6f, 0.5f));
	spriteBig->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.5f));
	spriteBig->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.5f));

	spriteBig->setAnimationSpeed(MOVE_RIGHT, 9);
	spriteBig->addKeyframe(MOVE_RIGHT, glm::vec2(0.2, 0.f));
	spriteBig->addKeyframe(MOVE_RIGHT, glm::vec2(0.4, 0.f));
	spriteBig->addKeyframe(MOVE_RIGHT, glm::vec2(0.6, 0.f));

	spriteBig->setAnimationSpeed(JUMP_RIGHT, 8);
	spriteBig->addKeyframe(JUMP_RIGHT, glm::vec2(0.8, 0.f));

	spriteBig->setAnimationSpeed(JUMP_LEFT, 8);
	spriteBig->addKeyframe(JUMP_LEFT, glm::vec2(0., 0.5f));

	spriteBig->changeAnimation(1);
	spriteBig->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//--------------------

	starbigmario.loadFromFile("images/mariosTODOSESTRELLA.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteBigMarioStar = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.2, 0.5), &starbigmario, &shaderProgram);
	spriteBigMarioStar->setNumberAnimations(7);

	spriteBigMarioStar->setAnimationSpeed(STAND_LEFT, 8);
	spriteBigMarioStar->addKeyframe(STAND_LEFT, glm::vec2(0.8f, 0.5f));

	spriteBigMarioStar->setAnimationSpeed(STAND_RIGHT, 8);
	spriteBigMarioStar->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	spriteBigMarioStar->setAnimationSpeed(DEAD, 8);
	spriteBigMarioStar->addKeyframe(DEAD, glm::vec2(0.f, 0.66666f));

	spriteBigMarioStar->setAnimationSpeed(MOVE_LEFT, 9);
	spriteBigMarioStar->addKeyframe(MOVE_LEFT, glm::vec2(0.6f, 0.5f));
	spriteBigMarioStar->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.5f));
	spriteBigMarioStar->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.5f));

	spriteBigMarioStar->setAnimationSpeed(MOVE_RIGHT, 9);
	spriteBigMarioStar->addKeyframe(MOVE_RIGHT, glm::vec2(0.2, 0.f));
	spriteBigMarioStar->addKeyframe(MOVE_RIGHT, glm::vec2(0.4, 0.f));
	spriteBigMarioStar->addKeyframe(MOVE_RIGHT, glm::vec2(0.6, 0.f));

	spriteBigMarioStar->setAnimationSpeed(JUMP_RIGHT, 8);
	spriteBigMarioStar->addKeyframe(JUMP_RIGHT, glm::vec2(0.8, 0.f));

	spriteBigMarioStar->setAnimationSpeed(JUMP_LEFT, 8);
	spriteBigMarioStar->addKeyframe(JUMP_LEFT, glm::vec2(0., 0.5f));

	spriteBigMarioStar->changeAnimation(1);
	spriteBigMarioStar->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime)
{
	getCorrectSprite()->update(deltaTime);
	if (isInAnimacionAlternarModo) {
		timerAnimacionAlternarModo += deltaTime / 1000.0;
		if (timerAnimacionAlternarModo > 1.f) {
			isInAnimacionAlternarModo = false;
			if (isBig) { //si ahora es big es porq antes era mini
				if (spriteMini->animation() == MOVE_LEFT || spriteMini->animation() == STAND_LEFT || spriteMini->animation() == JUMP_LEFT) spriteBig->changeAnimation(STAND_LEFT);
				else spriteBig->changeAnimation(STAND_RIGHT);
			}
			else {
				if (spriteBig->animation() == MOVE_LEFT || spriteBig->animation() == STAND_LEFT || spriteBig->animation() == JUMP_LEFT) spriteMini->changeAnimation(STAND_LEFT);
				else spriteMini->changeAnimation(STAND_RIGHT);
			}
		}
		else {
			if (timerAnimacionAlternarModo > 0.05 && timerAnimacionAlternarModo < 0.2 && conteoCambiosAnimacion == 0) {
				isBig = !isBig;
				++conteoCambiosAnimacion;
			}
			else if(timerAnimacionAlternarModo > 0.2 && timerAnimacionAlternarModo < 0.4 && conteoCambiosAnimacion == 1) {
				isBig = !isBig;
				++conteoCambiosAnimacion;
			}
			else if (timerAnimacionAlternarModo > 0.4 &&  timerAnimacionAlternarModo < 0.6f && conteoCambiosAnimacion == 2) {
				isBig = !isBig;
				++conteoCambiosAnimacion;
			}
			else if (timerAnimacionAlternarModo > 0.6 &&  timerAnimacionAlternarModo < 0.8 && conteoCambiosAnimacion == 3) {
				isBig = !isBig;
				++conteoCambiosAnimacion;
			}
		}
	}
	else if (isInAnimacionDead) {
		if (timerAnimationDead == 0.f) {
			spriteMini->changeAnimation(DEAD);
		}
		timerAnimationDead += deltaTime / 1000.0;
		if (timerAnimationDead < 0.5f) {
			posPlayer.y -= 3.f;
		}
		else if (timerAnimationDead < 3.f) {
			posPlayer.y += 3.f;
		}
		else if (timerAnimationDead >= 4.f) {
			isInAnimacionDead = false;
		}

	}
	else if (animationEndLevel) {
		
		if (posPlayer.y < 376) {
			if (getCorrectSprite()->animation() != JUMP_RIGHT) {//se apreta izq 1r vez
				getCorrectSprite()->changeAnimation(JUMP_RIGHT);
			}
			posPlayer.y += 1.f;
		}
		else if (posPlayer.x < 520) {
			if (getCorrectSprite()->animation() != MOVE_RIGHT) {//se apreta izq 1r vez
				getCorrectSprite()->changeAnimation(MOVE_RIGHT);
			}
			posPlayer.x += 1.f;
		}
		else nextLevel();
	}
	else {
		if (realesedBig && (Game::instance().getKey('m'))) { // || Game::instance().getKey('M')
			if (!isBig) {
				if(!isInvencible) sameAnimationBeetwenModes(spriteMini->animation());
				else sameAnimationBeetwenModes(spriteMarioStar->animation());
				alturaSprite = 64;
				isBig = true;
			}
			else {
				if (!isInvencible) sameAnimationBeetwenModes(spriteBig->animation());
				else sameAnimationBeetwenModes(spriteBigMarioStar->animation());
				alturaSprite = 32;
				isBig = false;
			}
			realesedBig = false;
		}

		//cosas de velocidades, run e invencibilidad
		if (Game::instance().getKey('z')) { //Game::instance().getKey('z')
			velocity = 3.f;
		}
		else if (!isInvencible && velocity != 2.f) velocity = 2.f;

		if (isInvencible) {
			timerIsInvencible += deltaTime / 1000.0;
			velocity = 3.f;
			if (timerIsInvencible >= TIME_INVINCIBILITY) {
				isInvencible = false;
				velocity = 2.f;
				if(isBig) sameAnimationBeetwenModes(spriteBigMarioStar->animation());
				else sameAnimationBeetwenModes(spriteMarioStar->animation());
			}
		}

		if (isInvulnerable) { //has been hitted
			timerHasBeenHitted += deltaTime / 1000.0;
			if (timerHasBeenHitted >= TIME_INVULNERABILITY) isInvulnerable = false;
		}

		if (kickedAShell) {
			timerKickedAShell += deltaTime / 1000.0;
			if (timerKickedAShell >= LIGHT_INVULNERABILITY) kickedAShell = false;
		}

		if (Game::instance().getKey('g') || Game::instance().getKey('G')) {
			timerIsInvencible = 0.f;
			isInvencible = true;
			realesedInvencible = false;
			if(!isInvencible && !isBig) sameAnimationBeetwenModes(spriteMini->animation());
			else if(!isInvencible && isBig) sameAnimationBeetwenModes(spriteBig->animation());
			
		}

		if (!realesedJump && !Game::instance().getKey(' ')) realesedJump = true;
		if (!realesedBig && (!Game::instance().getKey('m') && !Game::instance().getKey('M'))) realesedBig = true;
		if (!realesedInvencible && (!Game::instance().getKey('g') && !Game::instance().getKey('G'))) realesedInvencible = true;


		//CHECK DE VARIABLES
		if (Game::instance().getKey('s')) {
			velocity = velocity;
		}
		if ((Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !bJumping) || (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && bJumping && saltoQuieto)) //TODO ESTO ES PARA Q SE PUEDA MOVER SI EST�S HACIENDO UN SALTO EN PARADA
		{
			if (getCorrectSprite()->animation() != MOVE_LEFT && !bJumping) {//se apreta izq 1r vez
				getCorrectSprite()->changeAnimation(MOVE_LEFT);
			}
			/*if (posPlayer.x + walkedBeyondLimit >= SCROLL_LIMIT) {
				movementSafeZone -= velocity;
				posPlayer.x -= velocity;
			}
			else posPlayer.x -= velocity;*/
			posPlayer.x -= velocity;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)) || posPlayer.x <= 20) //poner condición para que no se vaya de la pantalla
			{
				if (posPlayer.x >= SCROLL_LIMIT) {
					posPlayer.x += velocity;
				}
				else posPlayer.x += velocity;
				if (getCorrectSprite()->animation() != STAND_LEFT) getCorrectSprite()->changeAnimation(STAND_LEFT);
			}
		}
		else if ((Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !bJumping) || (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && bJumping && saltoQuieto)) //TODO ESTO ES PARA Q SE PUEDA MOVER SI EST�S HACIENDO UN SALTO EN PARADA
		{
			if (getCorrectSprite()->animation() != MOVE_RIGHT && !bJumping) {
				getCorrectSprite()->changeAnimation(MOVE_RIGHT);
			}
			if(posPlayer.x >= SCROLL_LIMIT) walkedBeyondLimit += velocity;
			else posPlayer.x += velocity;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
			{
				if (posPlayer.x >= SCROLL_LIMIT) walkedBeyondLimit -= velocity;
				else posPlayer.x -= velocity;
				if (getCorrectSprite()->animation() != STAND_RIGHT) getCorrectSprite()->changeAnimation(STAND_RIGHT);
			}
		}
		else
		{
			if (getCorrectSprite()->animation() == MOVE_LEFT)
				getCorrectSprite()->changeAnimation(STAND_LEFT);
			else if (getCorrectSprite()->animation() == MOVE_RIGHT)
				getCorrectSprite()->changeAnimation(STAND_RIGHT);
		}

		if (bJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if (getCorrectSprite()->animation() == JUMP_RIGHT && !saltoQuieto) {
				if (posPlayer.x >= SCROLL_LIMIT) walkedBeyondLimit += velocity;
				else posPlayer.x += velocity;
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
				{
					if (posPlayer.x >= SCROLL_LIMIT) walkedBeyondLimit -= velocity;
					else posPlayer.x -= velocity;
					if (getCorrectSprite()->animation() != STAND_RIGHT) getCorrectSprite()->changeAnimation(STAND_RIGHT);
				}
			}

			else if (getCorrectSprite()->animation() == JUMP_LEFT && !saltoQuieto) {
				posPlayer.x -= velocity;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
				{
					posPlayer.x += velocity;
					if (getCorrectSprite()->animation() != STAND_LEFT) getCorrectSprite()->changeAnimation(STAND_LEFT);
				}
			}

			if (jumpAngle == 180)// || posPlayer.y > startY NECESARIO SI HAGO Q LA 2N PARTE DEL SALTO CAIGA ANTES
			{
				bJumping = false;
				// posPlayer.y = startY; NECESARIO SI HAGO Q LA 2N PARTE DEL SALTO CAIGA ANTES
				saltoQuieto = false;

				if (getCorrectSprite()->animation() == JUMP_LEFT) {//se apreta izq 1r vez
					getCorrectSprite()->changeAnimation(STAND_LEFT);
				}
				if (getCorrectSprite()->animation() == JUMP_RIGHT) {//se apreta izq 1r vez
					getCorrectSprite()->changeAnimation(STAND_RIGHT);
				}
			}
			else
			{
				if (jumpAngle <= 90) {
					posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
					/*if (!Game::instance().getKey(' ')) {
						jumpAngle = 91;
					}*/
					if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y, alturaSprite)) {
						jumpAngle = 91;

						engine->play2D(soundMarioCabezazo);

						if (getCorrectSprite()->animation() == JUMP_LEFT) {//se apreta izq 1r vez
							getCorrectSprite()->changeAnimation(STAND_LEFT);
						}
						if (getCorrectSprite()->animation() == JUMP_RIGHT) {//se apreta izq 1r vez
							getCorrectSprite()->changeAnimation(STAND_RIGHT);
						}
					}
				}
				else {
					//posPlayer.y = int(startY - 96 * sin(1.15 * 3.14159f * jumpAngle / 180.f));
					//posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
					posPlayer.y += FALL_STEP;
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
					if (!bJumping) {
						if (getCorrectSprite()->animation() == JUMP_LEFT) {//se apreta izq 1r vez
							getCorrectSprite()->changeAnimation(STAND_LEFT);
						}
						if (getCorrectSprite()->animation() == JUMP_RIGHT) {//se apreta izq 1r vez
							getCorrectSprite()->changeAnimation(STAND_RIGHT);
						}
					}
				}
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
			{
				if (realesedJump && Game::instance().getKey(' '))
				{
					if (isBig) {
						engine->play2D(soundMarioBigJump);
					}
					else {
						engine->play2D(soundMarioSmallJump);
					}
					realesedJump = false;
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;

					if (getCorrectSprite()->animation() == MOVE_RIGHT || getCorrectSprite()->animation() == STAND_RIGHT) {//se apreta izq 1r vez
						if (getCorrectSprite()->animation() == STAND_RIGHT) saltoQuieto = true;
						getCorrectSprite()->changeAnimation(JUMP_RIGHT);
					}
					else if (getCorrectSprite()->animation() == MOVE_LEFT || getCorrectSprite()->animation() == STAND_LEFT) {//se apreta izq 1r vez
						if (getCorrectSprite()->animation() == STAND_LEFT) saltoQuieto = true;
						getCorrectSprite()->changeAnimation(JUMP_LEFT);
					}
				}

			}
		}
	}
	if (posPlayer.y >= 405 && posPlayer.y < 420 && !isInAnimacionDead && animationEndLevel) {
		instaKill();
	}
	spriteBig->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
	spriteMini->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	spriteMarioStar->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	spriteBigMarioStar->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
}

void Player::render()
{
	getCorrectSprite()->render();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	if (isBig) getCorrectSprite()->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
	else getCorrectSprite()->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosition() { return posPlayer; }

void Player::hit() {
	if (!isInvulnerable && !kickedAShell) {
		if (isBig) {
			engine->play2D(soundMarioPowerDown);
			isInvulnerable = true;
			timerHasBeenHitted = 0.f;
			isBig = false;
			alturaSprite = 32;
			conteoCambiosAnimacion = 0;
			sameAnimationBeetwenModes(spriteBig->animation());
			setPosition(posPlayer);
			animacionAlternarModo();
		}
		else { //muere -1vida
			engine->play2D(soundMarioDie);
			isInAnimacionDead = true;
			timerAnimationDead = 0.f;
		}
	}
}

void Player::creceMario() {
	if (!isBig) {
		isBig = true;
		alturaSprite = 64;
		conteoCambiosAnimacion = 0;
		sameAnimationBeetwenModes(spriteMini->animation());
		setPosition(posPlayer);
		animacionAlternarModo();
		engine->play2D(soundMarioGrows);
	}
}

int Player::getAltura() {
	return alturaSprite;
}

void Player::animacionAlternarModo() {
	isInAnimacionAlternarModo = true;
	timerAnimacionAlternarModo = 0.f;
}

Sprite* Player::getCorrectSprite() {
	if (!isBig && !isInvencible) {
		return spriteMini;
	}
	else if (!isBig && isInvencible) {
		return spriteMarioStar;
	}
	else if (isBig && !isInvencible) {
		return spriteBig;
	}
	else {
		return spriteBigMarioStar;
	}
}

//a esta funcion le paso la animaci�n que quiero, y la aplico a los 4 sprites a la vez
void Player::sameAnimationBeetwenModes(int animation) {
	spriteMini->changeAnimation(animation);
	spriteBig->changeAnimation(animation);
	spriteMarioStar->changeAnimation(animation);
	spriteBigMarioStar->changeAnimation(animation);
}

void Player::invencibility() {
	isInvencible = true;
	timerIsInvencible = 0.f;
}

void Player::kickShell() {
	kickedAShell = true;
	timerKickedAShell = 0.f;
	engine->play2D(soundMarioKick);
}

bool Player::isInvencibleFunc() {
	return isInvencible;
}

void Player::hasMadeKill() {
	bJumping = true;
	jumpAngle = 0;
	startY = posPlayer.y;

	engine->play2D(soundMarioKill);

	if (getCorrectSprite()->animation() == MOVE_RIGHT || getCorrectSprite()->animation() == STAND_RIGHT) {//se apreta izq 1r vez
		if (getCorrectSprite()->animation() == STAND_RIGHT) saltoQuieto = true;
		getCorrectSprite()->changeAnimation(JUMP_RIGHT);
	}
	else if (getCorrectSprite()->animation() == MOVE_LEFT || getCorrectSprite()->animation() == STAND_LEFT) {//se apreta izq 1r vez
		if (getCorrectSprite()->animation() == STAND_LEFT) saltoQuieto = true;
		getCorrectSprite()->changeAnimation(JUMP_LEFT);
	}
}

bool Player::isInAnimacionDeadFunc() {
	return isInAnimacionDead;
}

void Player::animacionEndLevelFunc() {
	animationEndLevel = true;
}

bool Player::isInAnimacionEndLevel() {
	return animationEndLevel;
}

void Player::nextLevel() {
	bJumping = false;
	realesedJump = true;
	velocity = 2.f;
	realesedBig = true;
	saltoQuieto = false;
	conteoCambiosAnimacion = 0;
	isInvencible = false;
	isInvulnerable = false;
	timerHasBeenHitted = 0.f;
	realesedInvencible = true;
	timerAnimationDead = 0.f;
	kickedAShell = false;
	timerKickedAShell = 0.f;
	animationEndLevel = false;
	running = false;
}

bool Player::isChangingLevel() {
	return animationEndLevel;
}

void Player::instaKill() {
	engine->play2D(soundMarioDie);
	isInAnimacionDead = true;
	timerAnimationDead = 0.f;
}

int Player::getRelativePosition() {
	int r = getPosition().x;
	if (walkedBeyondLimit == 0) return 0;
	else return walkedBeyondLimit;
}

//void keyboardCallback(unsigned char key, int x, int y) {
//	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
//		isChangingLevel();
//	}
//}
