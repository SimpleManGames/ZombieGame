#pragma once

#include "Engine2D.h"
#include "Window.h"
#include "InputManager.h"

#include <memory>

namespace Engine2D {

	class ScreenList;
	class IGameScreen;

	class IMainGame
	{
	public:
		IMainGame();
		virtual ~IMainGame();

		void Run();
		void Exit();


		virtual void onInit() = 0;
		virtual void addScreens() = 0;
		virtual void onExit() = 0;

		void onSDLEvent(SDL_Event& evnt);

		const float getFps() const { return m_fps; }

	protected:
		virtual void Update();
		virtual void Draw();

		bool Init();
		bool InitSystems();

		std::unique_ptr<ScreenList> m_screenList = nullptr;
		IGameScreen* m_currentScreen = nullptr;
		bool m_isRunning;
		float m_fps = 0;
		Window m_window;
		InputManager m_inputManager;
	};
}
