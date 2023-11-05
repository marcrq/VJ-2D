#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "TileMap.h"


using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* mapa = new TileMap(levelFile, minCoords, program);

	return mapa;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
	relativePosition = 0;
	

	setRewardsLevel();
	pulsado = false;
}

TileMap::~TileMap()
{
	if (mapa != NULL)
		delete mapa;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);

	for (const auto& entry : renderMatrix) {
		if (entry.second.second) {
			glDrawArrays(GL_TRIANGLES, entry.second.first * 6, 6);
		}
	}

	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;

	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;

	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;

	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;

	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	int numTilesX, numTilesY;
	getline(fin, line);
	sstream.str(line);
	sstream >> numTilesX >> numTilesY;

	mapa = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin >> mapa[j * mapSize.x + i];
		}
	}

	getline(fin, line);

	for (int j = 0; j < numTilesY; j++)
	{
		getline(fin, line);
		sstream.str(line);
		for (int i = 0; i < numTilesX; i++)
		{
			int value;
			sstream >> value;
		}
	}

	fin.close();

	return true;
}


void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	renderMatrix = map<int, pair<int, bool> >();
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = mapa[j * mapSize.x + i];
			if (tile != 0)
			{
				renderMatrix.insert(pair<int, pair<int, bool> >(j * mapSize.x + i, pair<int, bool>(nTiles, true)));
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

pair<bool, int> TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + relativePosition) / tileSize;
	y0 = (pos.y - size.y + 32) / tileSize;
	y1 = (pos.y + 31) / tileSize;

	if (x <= 0) return pair<bool, int>(true, 0);

	for (int y = y0; y <= y1; y++)
	{
		int tile = mapa[y * mapSize.x + x];
		if (tile >= 1 && tile <= 8 or tile == 12)
			return pair<bool, int>(true, tile);
	}

	return pair<bool, int>(false, 0);
}

pair<bool, int> TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + relativePosition + size.x - 1) / tileSize;
	y0 = (pos.y - size.y + 32) / tileSize;
	y1 = (pos.y + 31) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		int tile = mapa[y * mapSize.x + x];
		if (tile >= 1 && tile <= 8 or tile == 12)
			return pair<bool, int>(true, tile);
	}

	return pair<bool, int>(false, 0);
}

pair<bool, int> TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = (pos.x + relativePosition) / tileSize;
	x1 = (pos.x + relativePosition + size.x - 1) / tileSize;
	y = (pos.y + 31) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int tile = mapa[y * mapSize.x + x];
		if (tile >= 1 && tile <= 8 or tile == 12) {
			if (*posY - tileSize * y + 32 <= 4)
			{
				*posY = tileSize * y - 32;
				return pair<bool, int>(true, tile);
			}
			return pair<bool, int>(true, tile);
		}
	}

	return pair<bool, int>(false, 0);
}

pair<bool, int> TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool isBig)
{
	int x0, x1, top;

	x0 = (pos.x + relativePosition) / tileSize;
	x1 = (pos.x + relativePosition + size.x - 1) / tileSize;
	top = (pos.y - size.y + 31) / tileSize;

	for (int x = x0; x <= x1; x++)
	{
		int tile = mapa[top * mapSize.x + x];
		if (tile >= 1 && tile <= 8 or tile == 12) {
			for (auto& reward : rewardsLevel) {
				int mapPos = top * mapSize.x + x;
				if (!std::get<1>(reward) && std::get<0>(reward) == mapPos) 
					std::get<1>(reward) = true;
			}
			if (isBig && tile == 1) {
				mapa[top * mapSize.x + x] = -1;
				renderMatrix.find(top * mapSize.x + x)->second.second = false;
			}
			return pair<bool, int>(true, tile);
		}
	}

	return pair<bool, int>(false, 0);
}

void TileMap::setRelativePosition(int r) {
	relativePosition = r;
}

void TileMap::itIsPressed() {
	
}

void TileMap::setRewardsLevel() {
	rewardsLevel.clear();
	for (int y = 0; y < mapSize.y; y++) {
		for (int x = 0; x < mapSize.x; x++) {
			int posMap = y * mapSize.x + x;
			int tile = mapa[posMap];
			if (tile == 2) {
				rewardsLevel.push_back(std::make_tuple(posMap, false, false));
			}
		}
	}
}