#pragma once

#include <Engine2D/IGameScreen.h>
#include <SDL/SDL.h>
#include <Engine2D/IMainGame.h>

class GameplayScreen : public Engine2D::IGameScreen
{
public:
	GameplayScreen();
	~GameplayScreen();

	// Inherited via IGameScreen
	virtual int getNextScreenIndex() const override;
	virtual int getPrevScreenIndex() const override;

	virtual void Build() override;
	virtual void Destory() override;
	virtual void onEntry() override;
	virtual void onExit() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	void checkInput();

};

