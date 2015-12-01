#pragma once

#include <Engine2D/IMainGame.h>
#include "GameplayScreen.h"
#include <Engine2D/ScreenList.h>

class App : public Engine2D::IMainGame {
public:
	App();
	~App();

	virtual void onInit() override;
	virtual void addScreens() override;
	virtual void onExit() override;

private:
	std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
};

