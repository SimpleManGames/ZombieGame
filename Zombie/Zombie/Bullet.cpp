#include "Bullet.h"



Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed) :
	_position(position), _direction(direction), _damage(damage), _speed(speed)
{
}


Bullet::~Bullet()
{
}

bool Bullet::Update(const std::vector<std::string>& levelData, float deltaTime) {
	_position += _direction * _speed * deltaTime;
	return collideWithWorld(levelData);
}

void Bullet::Draw(Engine2D::SpriteBatch & spriteBatch) {
	glm::vec4 destRect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Engine2D::ColorRGBA8 c;
	c.r = 64;
	c.b = 64;
	c.g = 64;
	c.a = 255;
	spriteBatch.Draw(destRect, uvRect, Engine2D::ResourceManager::getTexture("Images/Player16x16.png").id, 0.0f, c);
}

bool Bullet::collideWithAgent(Agent* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0 && !agent->isPlayer()) {
		return true;
	}
	return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData) {
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / (float)TILE_SIZE);
	gridPosition.y = floor(_position.y / (float)TILE_SIZE);

	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].length() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
		return true;
	}
	return (levelData[gridPosition.y][gridPosition.x] != '.');
}
