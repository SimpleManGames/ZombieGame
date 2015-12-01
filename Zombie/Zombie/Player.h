#pragma once

#include "Agent.h"
#include "Human.h"
#include <Engine2D/InputManager.h>
#include <Engine2D/Camera2D.h>
#include "Bullet.h"

class Gun;
class Bullet;

class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, Engine2D::InputManager* inputManager, Engine2D::Camera2D* camera, std::vector<Bullet>* bullets, bool isplayer = true);

	void addGun(Gun* gun);

	void Update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies, float deltaTime);

private:
	Engine2D::InputManager* _inputManager;

	std::vector<Gun*> _guns;
	int _currentGunIndex;

	Engine2D::Camera2D* _camera;
	std::vector<Bullet>* _bullets;

};

