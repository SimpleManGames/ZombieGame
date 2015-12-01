#include "ScreenList.h"

#include "IGameScreen.h"

namespace Engine2D {

	ScreenList::ScreenList(IMainGame* game) :
	m_game(game) {

	}
	ScreenList::~ScreenList() {
		Destory();
	}

	IGameScreen * ScreenList::moveNext() {
		IGameScreen* currentScreen = getCurrentScreen();

		if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
			m_currentScreenIndex = currentScreen->getNextScreenIndex();
		}
		return getCurrentScreen();
	}

	IGameScreen * ScreenList::movePrev() {
		IGameScreen* currentScreen = getCurrentScreen();

		if (currentScreen->getPrevScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
			m_currentScreenIndex = currentScreen->getPrevScreenIndex();
		}
		return getCurrentScreen();
	}

	IGameScreen * ScreenList::getCurrentScreen() { 
		if (m_currentScreenIndex != SCREEN_INDEX_NO_SCREEN) 
			return m_screens[m_currentScreenIndex]; 

		return nullptr;
	}

	void ScreenList::addScreen(IGameScreen* newScreen) {
		newScreen->m_screenIndex = m_screens.size();
		m_screens.push_back(newScreen);
		newScreen->Build();
		newScreen->setParentGame(m_game);
	}

	void ScreenList::Destory() {
		for (size_t i = 0; i < m_screens.size(); i++) {
			m_screens[i]->Destory();
		}
		m_screens.resize(0);
		m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
	}


}