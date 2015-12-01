#pragma once
#include <SDL/SDL.h>

namespace Engine2D {
	class FpsLimiter
	{
	public:
		FpsLimiter();

		void Init(float a_setMaxFPS);
		void Begin();
		// Return FPS
		float End();

		void setMaxFPS(float a_setMaxFPS);

	private:
		void calculateFPS();

		float _fps;
		float _maxFPS;
		float _frameTime;
		unsigned int _startTicks;


	};
}