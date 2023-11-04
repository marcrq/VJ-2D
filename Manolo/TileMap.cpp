#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
	relativePosition = 0;
	renderMatrix = new int[mapSize.y * mapSize.x];
	for (int i = 0; i < mapSize.y * mapSize.x; i++) {
		renderMatrix[i] = 1;
	}
	

	pulsado = false;
	rewardsLevel1.clear();
	rewardsLevel1.push_back(make_tuple(514, false, false));// 16 * 32 = 512
	rewardsLevel1.push_back(make_tuple(673, false, false));// 21 * 32 = 672
	//hacer bucle para inicializar, y darle 
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	//glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);

	for (int j = 0; j < mapSize.y; j++){
		for (int i = 0; i < mapSize.x; i++){
			int tileIndex = j * mapSize.x + i;
			if (tileIndex < nTiles) {
				// Verifica la matriz de booleanos para saber si renderizar este bloque.
				if (renderMatrix[j * mapSize.x + i] == 1) {
					// Calcula el índice de inicio para el bloque actual.
					int startIndex = 6 * tileIndex;
					glDrawArrays(GL_TRIANGLES, startIndex, 6);
				}
				//else {
				//	// Renderiza un rectángulo con un color específico
				//	int x0 = i * 32;
				//	int y0 = j * 32;
				//	int x1 = (i + 1) * 32;
				//	int y1 = (j + 1) * 32;

				//	// Define los vértices del rectángulo
				//	float vertices[] = {
				//		x0, y0,
				//		x1, y0,
				//		x1, y1,
				//		x0, y1
				//	};

				//	// Establece el color del rectángulo (azul en este caso)
				//	glColor3f(0.3529, 0.6078, 1.0);

				//	// Habilita los atributos de posición y coordenas de textura (si los usas)
				//	glEnableClientState(GL_VERTEX_ARRAY);
				//	glVertexPointer(2, GL_FLOAT, 0, vertices);

				//	// Renderiza el rectángulo
				//	glDrawArrays(GL_QUADS, 0, 4);

				//	// Deshabilita los atributos de posición
				//	glDisableClientState(GL_VERTEX_ARRAY);
				//}
			}
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

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin >> map[j * mapSize.x + i];
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
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
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
		int tile = map[y * mapSize.x + x];
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
		int tile = map[y * mapSize.x + x];
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
		int tile = map[y * mapSize.x + x];
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
		int tile = map[top * mapSize.x + x];
		if (tile >= 1 && tile <= 8 or tile == 12) {
			/*if (514 >= x * tileSize && 514 <= (x + 1) * tileSize) {
				pulsado = true;
			}*/
			for (auto& reward : rewardsLevel1) {
				if (!std::get<1>(reward) && std::get<0>(reward) >= x * tileSize && std::get<0>(reward) <= (x + 1) * tileSize) {
					std::get<1>(reward) = 0;
				}
			}
			if (isBig && tile == 1) {
				map[top * mapSize.x + x] = -1;
				int a = mapSize.x;
				int b = mapSize.y;
				renderMatrix[top * mapSize.x + x] = false; //lo que debía ser, 9*211+20 = 1919
				renderMatrix[166] = false; //para q se vea cambio visual, es 166
			}
			return pair<bool, int>(true, tile);
		}
	}

	//for (int i = 0; i < 299; ++i) renderMatrix[i] = false;

	return pair<bool, int>(false, 0);
}

void TileMap::setRelativePosition(int r) {
	relativePosition = r;
}

void TileMap::itIsPressed() {
	
}
