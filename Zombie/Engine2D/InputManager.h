#pragma once
#include <glm/glm.hpp>
#include <unordered_map>

namespace Engine2D {

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void Update();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		/// Returns true if the key is held down
		bool isKeyDown(unsigned int keyID);
		/// Returns true if key was pressed that frame
		bool isKeyPressed(unsigned int keyID);

		void setMouseCoords(float x, float y);
		glm::vec2 getMouseCoords() const { return _mouseCoords; }

	private:
		bool wasKeyDown(unsigned int keyID);

		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _prevKeyMap;
		glm::vec2 _mouseCoords;
	};
}
