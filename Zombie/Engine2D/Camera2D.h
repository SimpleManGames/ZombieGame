#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine2D {
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void Init(int screenWidth, int screenHeight, float scale);
		void Update();

		bool isBoxInView(const glm::vec2& position, const glm::vec2& dimesions);

		void setPosition(const glm::vec2& newPos) { _position = newPos; _needsUpdate = true; }
		glm::vec2 getPosition() { return _position; }

		void setScale(float newScale) { _scale = newScale; _needsUpdate = true; }
		float getScale() { return _scale; }

		glm::mat4 getcamMatrix() { return _camMatrix; }

		glm::vec2 convertScreenToWorld(glm::vec2 a_screenCoords);

	private:
		bool _needsUpdate;

		float _scale;

		int _screenWidth, _screenHeight;

		glm::vec2 _position;
		glm::mat4 _camMatrix;
		glm::mat4 _orthoMatrix;
	};
}
