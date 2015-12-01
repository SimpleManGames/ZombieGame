#pragma once
#include <glm/glm.hpp>
#include <Engine2D/SpriteBatch.h>
#include <Engine2D/ResourceManager.h>

#include <algorithm>

#include "Level.h"

const float AGENT_SIZE = 56;
const float AGENT_RADIUS = AGENT_SIZE / 2.0f;

class Zombie;
class Human;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void Update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies, float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);
	void collideWithTile(glm::vec2 tilePos);
	bool collideWithAgent(Agent* agent);
	bool applyDamage(float damage);
	void Draw(Engine2D::SpriteBatch& _spriteBatch);

	glm::vec2 getPosition() const { return _position; }
	bool isPlayer() const { return _isPlayer; }

protected:
	glm::vec2 _position;
	glm::vec2 _direction = glm::vec2(1.0f, 0.0f);
	Engine2D::ColorRGBA8 _color;
	GLuint m_id;
	float _speed;
	float _health;
	bool _isPlayer;
	void checkTilePosition(const std::vector<std::string>& levelData, 
							std::vector<glm::vec2>& collideTilePositions,
							float x, float y);
};

