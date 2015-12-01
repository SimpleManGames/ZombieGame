#include "GameSystem.h"
#include "MainGame.h"

int main(int argc, char** argv) {
	MainGame mainGame;

	mainGame.run(1, 50, 50);

	return 0;
}

