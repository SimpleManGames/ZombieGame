#include "Human.h"



Human::Human() : 
	_frames(0)
{
}


Human::~Human()
{
}

void Human::Init(float speed, glm::vec2 pos, bool isplayer/* = false*/) {
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> rd(-1.0f, 1.0f);
	
	m_id = Engine2D::ResourceManager::getTexture("Images/Player16x16.png").id;

	_health = 100;
	_isPlayer = isplayer;
	_color = Engine2D::ColorRGBA8(255, 255, 255, 255);

	_speed = speed;
	_position = pos;
	_direction = glm::vec2(rd(randomEngine), rd(randomEngine));
	
	if (_direction.length() == 0) 
		_direction = glm::vec2(1.0f, 0.0f);
	
	_direction = glm::normalize(_direction);
}

void Human::Update(const std::vector<std::string>& levelData,
					std::vector<Human*>& humans,
					std::vector<Zombie*>& zombies, float deltaTime) {
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRot(-45.0f, 45.0f);

	_position += _direction * _speed * deltaTime;
	if (_frames == 100) {
		_direction = glm::rotate(_direction, randRot(randomEngine));
		_frames = 0;
	}
	else
		_frames++;

	if (collideWithLevel(levelData)) {
		_direction = glm::rotate(_direction, randRot(randomEngine));
	}
}
