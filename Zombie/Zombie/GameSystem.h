#pragma once
#include <Engine2D/Engine2D.h>
#include <Engine2D/Window.h>
#include <Engine2D/InputManager.h>
#include <Engine2D/SpriteBatch.h>
#include <Engine2D/SpriteFont.h>
#include <Engine2D/AudioEngine.h>

class GameSystem
{
public:
	GameSystem();
	~GameSystem();

public:
	void initSystems(int screenWidth, int screenHeight);

protected:
	Engine2D::Window _window; ///< The game window

	int _screenWidth, _screenHeight;
};

