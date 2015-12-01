#include "Camera2D.h"

namespace Engine2D {

	Camera2D::Camera2D() :
		_position(0.0f, 0.0f),
		_camMatrix(1.0f),
		_orthoMatrix(1.0f),
		_scale(1.0f),
		_needsUpdate(true),
		_screenWidth(1024),
		_screenHeight(768) {
	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::Init(int screenWidth, int screenHeight, float scale) {
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_scale = scale;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}

	void Camera2D::Update() {
		if (_needsUpdate) {
			
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_camMatrix = glm::translate(_orthoMatrix, translate);

			glm::vec3 scale(_scale, _scale, 0.0f);
			_camMatrix = glm::scale(glm::mat4(1.0f), scale) * _camMatrix;

			_needsUpdate = false;
		}
	}

	bool Camera2D::isBoxInView(const glm::vec2 & position, const glm::vec2 & dimesions) {
		glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / _scale;

		const float MIN_DISTANCE_X = dimesions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimesions.y / 2.0f + scaledScreenDimensions.y / 2.0f;
		
		glm::vec2 centerPos = position + dimesions / 2.0f;
		glm::vec2 centerCameraPos = _position;
		glm::vec2 distVec = centerPos - centerCameraPos;
		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0) {
			return true;
		}
		return false;
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 a_screenCoords) {
		// Invert Y
		a_screenCoords.y = _screenHeight - a_screenCoords.y;
		// (0,0) is center
		a_screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		// Scale
		a_screenCoords /= _scale;
		a_screenCoords += _position;
		return a_screenCoords;
	}
}