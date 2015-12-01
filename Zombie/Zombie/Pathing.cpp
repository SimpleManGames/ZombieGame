#include "Pathing.h"



Pathing::Pathing()  { }
Pathing::~Pathing() { }

glm::vec2 Pathing::aStar(glm::vec2 source, 
						 glm::vec2 target, 
						 const std::vector<std::string>& levelData) {
	int mapWidth = levelData[0].size();
	int mapHeight = levelData.size();
	std::vector<glm::vec3> pathGrid;
	bool ** checkedPath = new bool*[mapWidth];
	for (int i = 0; i < mapWidth; i++) {
		checkedPath[i] = new bool[mapHeight];
	}
	for (int y = 0; y < mapHeight; y++)
		for (int x = 0; x < mapWidth; x++)
			checkedPath[x][y] = false;

	pathGrid.emplace_back(glm::vec3(target.x, target.y, 0));
	bool pathFound = false;
	int i = 0;

	while (i < pathGrid.size()) {
		if (pathGrid[i].y > 0) {
			if (pathGrid[i].y - 1 == source.y && pathGrid[i].x == source.x) {
				checkedPath[(int)pathGrid[i].x][(int)pathGrid[i].y - 1] = true;
				pathFound = true;
				return glm::vec2(0, 1);
			}
			else if (i > 0 && levelData[(int)pathGrid[i].y - 1][(int)pathGrid[i].x] == '.' && !checkedPath[(int)pathGrid[i].x][(int)pathGrid[i].y - 1]) {
				pathGrid.emplace_back(glm::vec3(pathGrid[i].x, pathGrid[i].y - 1, pathGrid[i].z));
				checkedPath[(int)pathGrid[i].x][(int)pathGrid[i].y - 1] = true;
			}
		}
		if (pathGrid[i].y < mapHeight - 1 && !pathFound) {
			if (pathGrid[i].y + 1 == source.y && pathGrid[i].x == source.x) {
				checkedPath[(int)pathGrid[i].x][(int)pathGrid[i].y + 1] = true;
				pathFound = true;
				return glm::vec2(0, -1);
			}
			else if (levelData[(int)pathGrid[i].y + 1][(int)pathGrid[i].x] == '.' && !checkedPath[(int)pathGrid[i].x][(int)pathGrid[i].y + 1]) {
				pathGrid.emplace_back(glm::vec3(pathGrid[i].x, pathGrid[i].y + 1, pathGrid[i].z));
				checkedPath[(int)pathGrid[i].x][(int)pathGrid[i].y + 1] = true;
			}
		}
		if (pathGrid[i].x > 0 && !pathFound) {
			if (pathGrid[i].y == source.y && pathGrid[i].x - 1 == source.x) {
				checkedPath[(int)pathGrid[i].x - 1][(int)pathGrid[i].y] = true;
				pathFound = true;
				return glm::vec2(1, 0);
			}
			else if (i > 0 && levelData[(int)pathGrid[i].y][(int)pathGrid[i].x - 1] == '.' && !checkedPath[(int)pathGrid[i].x - 1][(int)pathGrid[i].y]) {
				pathGrid.emplace_back(glm::vec3(pathGrid[i].x - 1, pathGrid[i].y, pathGrid[i].z));
				checkedPath[(int)pathGrid[i].x - 1][(int)pathGrid[i].y] = true;
			}
		}
		if (pathGrid[i].x < mapWidth - 1 && !pathFound) {
			if (pathGrid[i].y == source.y && pathGrid[i].x + 1 == source.x) {
				checkedPath[(int)pathGrid[i].x + 1][(int)pathGrid[i].y] = true;
				pathFound = true;
				return glm::vec2(-1, 0);
			}
			else if (levelData[(int)pathGrid[i].y][(int)pathGrid[i].x + 1] == '.' && !checkedPath[(int)pathGrid[i].x + 1][(int)pathGrid[i].y]) {
				pathGrid.emplace_back(glm::vec3(pathGrid[i].x + 1, pathGrid[i].y, pathGrid[i].z));
				checkedPath[(int)pathGrid[i].x + 1][(int)pathGrid[i].y] = true;
			}
		}
		i++;
	}
	return glm::vec2(0.0f);
}
