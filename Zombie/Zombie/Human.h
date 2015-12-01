#pragma once

#include "Agent.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>

class Human : public Agent
{
public:
	Human();
	virtual ~Human();

	void Init(float speed, glm::vec2 pos, bool isplayer = false);

	virtual void Update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies, float deltaTime) override;

private:
	int _frames;

};

