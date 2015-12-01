#include "Player.h"
#include <SDL/SDL.h>

#include "Gun.h"

Player::Player() :
	_currentGunIndex(-1) {
	// Empty
}

Player::~Player() {
	// Empty
}

void Player::init(float speed, glm::vec2 pos, Engine2D::InputManager* inputManager, Engine2D::Camera2D* camera, std::vector<Bullet>* bullets, bool isplayer) {
	_speed = speed;
	_position = pos;
	_inputManager = inputManager;
	_bullets = bullets;
	_camera = camera;
	_color = Engine2D::ColorRGBA8(255, 255, 255, 255);
	m_id = Engine2D::ResourceManager::getTexture("Images/Player16x16.png").id;
	_isPlayer = isplayer;
	_currentGunIndex = 0;
	_health = 250;
}

void Player::addGun(Gun* gun) {
	// Add the gun to his inventory
	_guns.push_back(gun);

	// If no gun equipped, equip gun.
	if (_currentGunIndex == -1) {
		_currentGunIndex = 0;
	}
}

void Player::Update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies, float deltaTime) {

	if (_inputManager->isKeyDown(SDLK_w)) {
		_position.y += _speed * deltaTime;
	}
	else if (_inputManager->isKeyDown(SDLK_s)) {
		_position.y -= _speed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_a)) {
		_position.x -= _speed * deltaTime;
	}
	else if (_inputManager->isKeyDown(SDLK_d)) {
		_position.x += _speed * deltaTime;
	}

	if (_inputManager->isKeyDown(SDLK_1) && _guns.size() >= 0 && _guns[0]->isUnlocked()) {
		_currentGunIndex = 0;
	}
	else if (_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1 && _guns[1]->isUnlocked()) {
		_currentGunIndex = 1;
	}
	else if (_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2 && _guns[2]->isUnlocked()) {
		_currentGunIndex = 2;
	}
	glm::vec2 mouseCoords = _inputManager->getMouseCoords();
	mouseCoords = _camera->convertScreenToWorld(mouseCoords);
	glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
	_direction = glm::normalize(mouseCoords - centerPosition);

	if (_currentGunIndex != -1) {
		_guns[_currentGunIndex]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT),
			centerPosition,
			_direction,
			*_bullets, deltaTime);
	}

	collideWithLevel(levelData);
}
