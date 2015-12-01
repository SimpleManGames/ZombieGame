#pragma once
#include <Engine2D/Errors.h>
#include <Engine2D/SpriteBatch.h>
#include <Engine2D/ResourceManager.h>
#include <Dungeon/Dungeon.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

const int TILE_SIZE = 64;

class Level
{
public:
	/// Build and Load level
	Level(const std::string& fileName);
	~Level();

	void Draw();

	glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
	const std::vector<glm::vec2>& getZombieStartPositions() const { return _zombieStartPositions; }
	const std::vector<glm::vec2>& getammoPositions() const { return _ammoPositions; }
	const glm::vec2& getRiflePosition() const { return _riflePosition; }
	const glm::vec2& getShotgunPosition() const { return _shotgunPosition; }

	const std::vector<std::string>& getLevelData() const { return _levelData; }

	const char getChar(int x, int y) const { return _levelData[y][x]; }

	glm::vec2 _playerPos;

private:
	Dungeon::Dungeon d;
	std::vector<std::string> _levelData;
	int _numHumans;

	Engine2D::SpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPos;
	std::vector<glm::vec2> _zombieStartPositions;
	glm::vec2 _riflePosition;
	glm::vec2 _shotgunPosition;
	std::vector<glm::vec2> _ammoPositions;
};

