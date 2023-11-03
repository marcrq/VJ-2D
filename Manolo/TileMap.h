#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <vector>
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
	pair<bool, int> collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, int altura);
	void setRelativePosition(int r);

	bool pulsado;
	std::vector<std::tuple<int, bool, bool>> rewardsLevel1; //posici�n, pulsado, creado/consumida moneda
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	void prepareInteractiveLayer(const glm::vec2& minCoords, ShaderProgram& program);
	void prepareBackgroundLayer(const glm::vec2& minCoords, ShaderProgram& program);
	void itIsPressed();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

	int relativePosition;
	
};


#endif // _TILE_MAP_INCLUDE


