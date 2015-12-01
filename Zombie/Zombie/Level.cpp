#include "Level.h"

Level::Level(const std::string& fileName) {
	std::ifstream file;
	file.open(fileName);

	// Error checking
	if (file.fail()) {
		Engine2D::fatalError("Failed to open " + fileName);
	}

	_spriteBatch.Init();
	_spriteBatch.Begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Engine2D::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	std::string tmp;
	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}

	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData.size(); x++) {
			char tile = _levelData[y][x];

			glm::vec4 destRect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
			
			switch (tile) {
			case ' ':
			case '#':
				_spriteBatch.Draw(destRect, uvRect, 
					Engine2D::ResourceManager::getTexture("Images/Wall16x16.png").id, 0.0f, color);
				break;
			// Door
				//_spriteBatch.Draw(destRect, uvRect,
				//	Engine2D::ResourceManager::getTexture("Images/Door16x16.png").id, 0.0f, color);
				//break;
			case '@':
				_levelData[y][x] = '.';
				_spriteBatch.Draw(destRect, uvRect,
					Engine2D::ResourceManager::getTexture("Images/Blank16x16.png").id, 0.0f, color);
				_startPlayerPos.x = x * TILE_SIZE;
				_startPlayerPos.y = y * TILE_SIZE;
				break;
			case 'Z':
				_levelData[y][x] = '.';
				_spriteBatch.Draw(destRect, uvRect,
					Engine2D::ResourceManager::getTexture("Images/Blank16x16.png").id, 0.0f, color);
				_zombieStartPositions.emplace_back(x * TILE_SIZE, y * TILE_SIZE);
				break;
			case 'R':
				_levelData[y][x] = '.';
				_spriteBatch.Draw(destRect, uvRect,
					Engine2D::ResourceManager::getTexture("Images/Blank16x16.png").id, 0.0f, color);
				_riflePosition = glm::vec2(x, y);
				break;
			case 'S':
				_levelData[y][x] = '.';
				_spriteBatch.Draw(destRect, uvRect,
					Engine2D::ResourceManager::getTexture("Images/Blank16x16.png").id, 0.0f, color);
				_shotgunPosition = glm::vec2(x, y);
				break;
			case 'A':
				_levelData[y][x] = '.';
				_spriteBatch.Draw(destRect, uvRect,
					Engine2D::ResourceManager::getTexture("Images/Blank16x16.png").id, 0.0f, color);
				_ammoPositions.emplace_back(x * TILE_SIZE, y * TILE_SIZE);
				break;
			default:
				_levelData[y][x] = '.';
				_spriteBatch.Draw(destRect, uvRect,
					Engine2D::ResourceManager::getTexture("Images/Blank16x16.png").id, 0.0f, color);
				std::printf("Unknown tile %c at (%d, %d)", tile, x, y);
				break;
			case '<':
			case '>':
			case '+':
			case '.':
			case ',':
				_levelData[y][x] = '.';
				_spriteBatch.Draw(destRect, uvRect,
					Engine2D::ResourceManager::getTexture("Images/Blank16x16.png").id, 0.0f, color);
				break;
			}
		}
	}
	_spriteBatch.End();
}


Level::~Level()
{
}

void Level::Draw() {
	_spriteBatch.RenderBatch();
}
