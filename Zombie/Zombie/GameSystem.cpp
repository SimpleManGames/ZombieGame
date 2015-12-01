#include "GameSystem.h"



GameSystem::GameSystem() {
	_screenWidth = 1028;
	_screenHeight = 728;
}


GameSystem::~GameSystem()
{
}

void GameSystem::initSystems(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	Engine2D::init();

	//_window.create("Zombie", _screenWidth, _screenHeight, 0);
	glClearColor(0.6f, 0.6f, 0.6f, 1);

}
