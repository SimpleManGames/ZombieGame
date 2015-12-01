#pragma once

#define SCREEN_INDEX_NO_SCREEN -1

namespace Engine2D {

	class IMainGame;

	enum class ScreenState {
		NONE,
		RUNNING,
		EXIT_APP,
		CHANGE_NEXT,
		CHANGE_PREV
	};

	class IGameScreen
	{
	public:
		friend class ScreenList;
		IGameScreen() {
			// Empty
		}
		virtual ~IGameScreen() {
			// Empty
		}

		virtual int getNextScreenIndex() const = 0;
		virtual int getPrevScreenIndex() const = 0;

		virtual void Build() = 0;
		virtual void Destory() = 0;

		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		virtual void Update() = 0;
		virtual void Draw() = 0;

		int getScreenIndex() const { return m_screenIndex; }

		void setParentGame(IMainGame* game) { m_game = game; }

		void setRunning() { m_currentState = ScreenState::RUNNING; }
		ScreenState getState() const { return m_currentState; }

	protected:
		ScreenState m_currentState = ScreenState::NONE;
		IMainGame* m_game = nullptr;

		int m_screenIndex = -1;

	};
}
