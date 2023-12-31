#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <vector>
#include <map>
#include <tuple>  // Necesario para std::pair
#include <iostream>

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	int getMapHeight() const { return mapSize.y; }

	pair<bool, int> collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	pair<bool, int> collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	pair<bool, int> collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	pair<bool, int> collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool isBig);
	void setRelativePosition(int r);
	int getTileType(const glm::ivec2& pos);

	std::vector<std::tuple<int, bool, bool>> rewardsLevel; //posicion, pulsado, creado/consumida moneda

	bool pulsado;
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	void itIsPressed();
	void setRewardsLevel();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *mapa;

	int relativePosition;
	map<int, pair<int, bool> > renderMatrix;
};


#endif // _TILE_MAP_INCLUDE


