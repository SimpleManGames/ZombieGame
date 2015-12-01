#include "IMainGame.h"
#include "Timing.h"

#include "ScreenList.h"
#include "IGameScreen.h"

namespace Engine2D {

	IMainGame::IMainGame() {
		m_screenList = std::make_unique<ScreenList>(this);
	}

	IMainGame::~IMainGame() {
	
	}

	void IMainGame::Run() {
		if (!Init()) return;
		FpsLimiter limiter;
		limiter.setMaxFPS(60.0f);
		
		m_isRunning = true;
		
		while (m_isRunning) {
			limiter.Begin();

			m_inputManager.Update();
			Update();
			Draw();

			m_fps = limiter.End();
			m_window.swapBuffer();
		}
	}

	void IMainGame::Exit() {
		m_currentScreen->onExit();
		if (m_screenList) {
			m_screenList->Destory();
			m_screenList.reset();
		}
		m_isRunning = false;
	}

	bool IMainGame::Init() {
		Engine2D::init();
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		
		if (!InitSystems()) return false;

		onInit();
		addScreens();

		m_currentScreen = m_screenList->getCurrentScreen();
		m_currentScreen->onEntry();
		m_currentScreen->setRunning();

		return true;
	}
	bool IMainGame::InitSystems() {
		m_window.create("Default", 1024, 728, 0);
		return true;
	}
	void IMainGame::Update() {
		if (m_currentScreen) {
			switch (m_currentScreen->getState()) {
			case ScreenState::RUNNING:
				m_currentScreen->Update();
				break;
			case ScreenState::CHANGE_NEXT:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->moveNext();
				if (m_currentScreen) {
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::CHANGE_PREV:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->movePrev();
				if (m_currentScreen) {
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::EXIT_APP:
				exit(0);
				break;
			default:
				break;
			}
		}
		else {
			exit(0);
		}
	}
	void IMainGame::Draw() {
		glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());
		if (m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING)
			m_currentScreen->Draw();
	}
	void IMainGame::onSDLEvent(SDL_Event & evnt) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_isRunning = false;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}