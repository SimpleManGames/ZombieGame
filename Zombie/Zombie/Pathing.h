#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

class Pathing
{
public:
	Pathing();
	~Pathing();

	glm::vec2 aStar(glm::vec2 source, glm::vec2 target, const std::vector<std::string>& levelData);
};

