#pragma once

#include "Human.h"
#include "Player.h"
#include "Agent.h"
#include "Pathing.h"

class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();

	void Init(float speed, glm::vec2 pos, bool isplayer = false);

	virtual void Update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies, float deltaTime) override;
private:
	Human* getNearestHuman(std::vector<Human*>& humans);
	glm::vec2 _velocity;
};