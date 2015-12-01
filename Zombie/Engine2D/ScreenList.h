#pragma once

#include <vector>

namespace Engine2D {

	class IMainGame;
	class IGameScreen;

	class ScreenList
	{
	public:
		ScreenList(IMainGame* game);
		~ScreenList();
		IGameScreen* moveNext();
		IGameScreen* movePrev();

		IGameScreen* getCurrentScreen();
		void setScreen(int nextScreen) { m_currentScreenIndex = nextScreen; }
		void addScreen(IGameScreen* newScreen);

		void Destory();

	protected:
		IMainGame* m_game = nullptr;
		std::vector<IGameScreen*> m_screens;
		int m_currentScreenIndex = -1;
	};
}