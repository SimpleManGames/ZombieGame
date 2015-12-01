#include "Zombie.h"


Zombie::Zombie() { 
}

Zombie::~Zombie()
{
}

void Zombie::Init(float speed, glm::vec2 pos, bool isplayer) {
	_speed = speed;
	_position = pos;
	_health = 100;
	// Set Green Color
	_color = Engine2D::ColorRGBA8(255, 255, 255, 255);
	_isPlayer = isplayer;

	m_id = Engine2D::ResourceManager::getTexture("Images/Zombie16x16.png").id;
}

void Zombie::Update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies, float deltaTime) {

	// Find the closest human
	Human* closestHuman = getNearestHuman(humans);

	// If we found a human, move towards him
	if (closestHuman != nullptr) {
		// Get the direction vector twoards the player
		_direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += _direction * _speed * deltaTime;
	}

	// Do collision
	collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans) {
	Human* closestHuman = nullptr;
	float smallestDistance = 9999999.0f;

	for (int i = 0; i < humans.size(); i++) {
		// Get the direction vector
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		// Get distance
		float distance = glm::length(distVec);

		// If this person is closer than the closest person, this is the new closest
		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}

	return closestHuman;
}