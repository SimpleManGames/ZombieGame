#pragma once
#include <Engine2D/SpriteBatch.h>

class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	void run();
private:
	void initTextures(Engine2D::SpriteBatch* spriteBatch);
	void draw();
	void processInput();


};

