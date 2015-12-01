#include "Agent.h"

Agent::Agent()
{
}


Agent::~Agent()
{
}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePositions;

	// Check four corners
	checkTilePosition(levelData, collideTilePositions, _position.x, _position.y);
	checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_SIZE, _position.y);
	checkTilePosition(levelData, collideTilePositions, _position.x, _position.y + AGENT_SIZE);
	checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_SIZE, _position.y + AGENT_SIZE);

	if (collideTilePositions.size() == 0) { return false; }

	for (int i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}
	return true;
}

void Agent::collideWithTile(glm::vec2 tilePos) {
	const float TILE_RADIUS = (float)TILE_SIZE / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos;
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if (xDepth > 0 && yDepth > 0) {
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			if (distVec.x < 0) { _position.x -= xDepth; }
			else _position.x += xDepth;
				
		}
		else
			if (distVec.y < 0) { _position.y -= yDepth; }
			else _position.y += yDepth;
	}
}

bool Agent::collideWithAgent(Agent* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS * 2;

	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {

		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		_position += collisionDepthVec / 2.0f;
		agent->_position -= collisionDepthVec / 2.0f;
		return true;
	}
	return false;
}

bool Agent::applyDamage(float damage) {
	_health -= damage;
	if (_health <= 0) return true;
	return false;
}

void Agent::Draw(Engine2D::SpriteBatch& _spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect(_position.x, _position.y, AGENT_SIZE, AGENT_SIZE);
	_spriteBatch.Draw(destRect, uvRect, m_id, 0, _color, _direction);
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y) {
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_SIZE), floor(y / (float)TILE_SIZE));
	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].length() ||
		cornerPos.y < 0 || cornerPos.y >= levelData.size()) {
		return;
	}
	if (levelData[cornerPos.y][cornerPos.x] != '.') {
		collideTilePositions.push_back(cornerPos  * (float)TILE_SIZE + glm::vec2(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f));
	}
}
