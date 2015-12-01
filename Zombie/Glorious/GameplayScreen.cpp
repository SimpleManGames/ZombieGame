#include "GameplayScreen.h"

#include <iostream>


GameplayScreen::GameplayScreen() {
}

GameplayScreen::~GameplayScreen() {
}

int GameplayScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

void GameplayScreen::Build() {
}

void GameplayScreen::Destory() {
}

void GameplayScreen::onEntry() { 
}

void GameplayScreen::onExit() {
}

void GameplayScreen::Update() {
	checkInput();
}

void GameplayScreen::Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

void GameplayScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}
