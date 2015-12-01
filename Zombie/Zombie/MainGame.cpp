#include "MainGame.h"

#include <Engine2D/Engine2D.h>
#include <Dungeon/Dungeon.h>

#include <SDL/SDL.h>
#include <iostream>

#include <cassert>

const float HUMAN_SPEED = 2.0f;
const float ZOMBIE_SPEED = 2.5f;

const float CAMERA_SCALE = 0.5f;

/// Sets basic value of the main game
MainGame::MainGame() : 
_gameState(GameState::PLAY),
_fps(0.0f),
_currentLevel(0),
_player(nullptr) { 
	// Empty
}

/// Delete the levels
MainGame::~MainGame() {
	for (int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
}

/// Inits everything and then runs the game
void MainGame::run(int levelCount, int dungeonX, int dungeonY) {
	initGame();
	generateLevels(1, dungeonX, dungeonY, 10);
	
	// Inits the player and guns
	_player = new Player();
	_player->init(5.0f, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets, true);
	_humans.push_back(_player);
	_player->addGun(new Gun("Pistol", 15, 1, 5.0f, 15, 20.0f, m_audioEngine.loadSoundEffect("Sounds/pistol.wav"), true));
	_player->addGun(new Gun("Rifle", 5, 1, 10.0f, 30, 20.0f, m_audioEngine.loadSoundEffect("Sounds/cg1.wav"), true));
	_player->addGun(new Gun("ShotGun", 25, 7, 25.0f, 10, 20.0f, m_audioEngine.loadSoundEffect("Sounds/shotgun.wav"), true));

	// Init the music and sound effects
	Engine2D::Music music = m_audioEngine.loadMusic("Sounds/Al salir El Sol.mp3");
	m_audioEngine.SetMusicVolume(32);
	music.Play(-1);
	m_audioEngine.SetEffectVolume("Sounds/pistol.wav", 32);
	m_audioEngine.SetEffectVolume("Sounds/cg1.wav", 32);
	m_audioEngine.SetEffectVolume("Sounds/shotgun.wav", 32);

	// Run the game
	gameLoop();
}

/// Inits the window, audio, spritebatches and fonts
void MainGame::initGame() {
	m_audioEngine.Init();

	_window.create("Zombie", _screenWidth, _screenHeight, 0);
	glClearColor(0.6f, 0.6f, 0.6f, 1);

	_agentSpriteBatch.Init();
	_hudSpriteBatch.Init();

	_camera.Init(_screenWidth, _screenHeight, CAMERA_SCALE);
	_hudCamera.Init(_screenWidth / 2, _screenHeight / 2, CAMERA_SCALE);
	_hudCamera.setPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));

	initShaders();

	m_bloodparticleBatch = new Engine2D::ParticleBatch2D();
	m_bloodparticleBatch->Init(
		1000, 
		0.1f, 
		Engine2D::ResourceManager::getTexture("Images/particle.png"), 
		[](Engine2D::Particle2D& particle, float deltaTime) { 
		particle.position += particle.velocity * deltaTime;	
		particle.color.a = (GLubyte)(particle.lifeTime * 255.0f) * deltaTime; });

	m_particleEngine.addParticleBatch(m_bloodparticleBatch);

	_spriteFont = new Engine2D::SpriteFont("Fonts/B52.ttf", 64);
}

/// Inits the shaders
void MainGame::initShaders() {
	// Compile our color shader
	_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

/// Generate the levels and adds them to a vector
void MainGame::generateLevels(int levelCount, int dungeonX, int dungeonY, int humanCount) {
	for (int i = 1; i <= levelCount; i++) {
		Dungeon::Dungeon d(dungeonX, dungeonY);
		d.generate(100);
		d.save("Levels/level" + std::to_string(levelCount) + ".txt");
		_levels.push_back(new Level("Levels/level" + std::to_string(i) + ".txt"));
	}
	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(1, dungeonX - 2);
	std::uniform_int_distribution<int> randY(1, dungeonY - 2);


	std::vector<std::string> levelData = _levels[_currentLevel]->getLevelData();

	for (int i = 0; i < humanCount; i++) {
		glm::vec2 pos(randX(randomEngine), randY(randomEngine));
		if (levelData[pos.y][pos.x] == '.') {
			_humans.push_back(new Human);
			_humans.back()->Init(HUMAN_SPEED, glm::vec2(pos.x * TILE_SIZE, pos.y * TILE_SIZE), false);
		}
		else {
			i--;
		}
	}
	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getZombieStartPositions();
	for (int i = 0; i < zombiePositions.size(); i++) {
		_zombies.push_back(new Zombie);
		_zombies.back()->Init(ZOMBIE_SPEED, zombiePositions[i], false);
	}
	/*const std::vector<glm::vec2>& ammoPositions = _levels[_currentLevel]->getammoPositions();
	for (int i = 0; i < ammoPositions.size(); i++) {
		_pickups.push_back(new Pickup);
		_pickups.back()->Init(Engine2D::ResourceManager::getTexture("Images/Ammo16x16"), Type::AMMO, ammoPositions[i]);
	}*/

}

/// Runs the game
void MainGame::gameLoop() {
	Engine2D::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);

	const int MAX_STEPS = 6;

	const float MS_PER_SECOND = 1000;
	const float DESIRED_FPS = 60.0f;
	const float DESIRED_FRAME_TIME = MS_PER_SECOND / DESIRED_FPS;

	const float MAX_DELTATIME = 1.0f;

	float prevTicks = SDL_GetTicks();

	while (_gameState != GameState::EXIT) { 
		fpsLimiter.Begin();

		Uint32 newTicks = SDL_GetTicks();
		Uint32 frameTime = newTicks - prevTicks;
		prevTicks = newTicks;

		float totalDeltaTime = frameTime / DESIRED_FRAME_TIME;

		_inputManager.Update();

		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTATIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			m_particleEngine.Update(deltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}
		_camera.setPosition(_player->getPosition());
		_levels[_currentLevel]->_playerPos
			= glm::vec2(_player->getPosition().x / TILE_SIZE, 
					    _player->getPosition().y / TILE_SIZE);

		_camera.Update();
		_hudCamera.Update();

		drawGame();

		_fps = fpsLimiter.End();
	}
}

/// Update all the agents(Player, humans and zombies)
void MainGame::updateAgents(float deltaTime) {
	// Update all humans
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->Update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies, deltaTime);
	}

	// Update all zombies
	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->Update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies, deltaTime);
	}

	// Update Zombie collisions
	for (int i = 0; i < _zombies.size(); i++) {
		// Collide with other zombies
		for (int j = i + 1; j < _zombies.size(); j++) {
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		// Collide with humans
		for (int j = 0; j < _humans.size(); j++) {
			if (_zombies[i]->collideWithAgent(_humans[j])) {
				// Add the new zombie
				_zombies.push_back(new Zombie);
				_zombies.back()->Init(ZOMBIE_SPEED, _humans[j]->getPosition());
				// Delete the human
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}

		// Collide with player
		if (_zombies[i]->collideWithAgent(_player)) {
			Engine2D::fatalError("YOU LOSE");
		}
	}

	// Update Human collisions
	for (int i = 0; i < _humans.size(); i++) {
		// Collide with other humans
		for (int j = i + 1; j < _humans.size(); j++) {
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}

	// Dont forget to update zombies
}

/// Update the bullets and checks for collision
void MainGame::updateBullets(float deltaTime) {
	// Update and collide with world
	for (int i = 0; i < _bullets.size(); ) {
		// If update returns true, the bullet collided with a wall
		if (_bullets[i].Update(_levels[_currentLevel]->getLevelData(), deltaTime)) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool wasBulletRemoved;

	// Collide with humans and zombies
	for (int i = 0; i < _bullets.size(); i++) {
		wasBulletRemoved = false;
		// Loop through zombies
		for (int j = 0; j < _zombies.size(); ) {
			// Check collision
			if (_bullets[i].collideWithAgent(_zombies[j])) {
				addParticle(_bullets[i].getPosition(), 5);

				// Damage zombie, and kill it if its out of health
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
					// If the zombie died, remove him
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
				}
				else {
					j++;
				}

				// Remove the bullet
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemoved = true;
				i--; // Make sure we don't skip a bullet
					 // Since the bullet died, no need to loop through any more zombies
				break;
			}
			else {
				j++;
			}
		}
		// Loop through humans
		if (wasBulletRemoved == false) {
			for (int j = 0; j < _humans.size(); ) {
				// Check collision
				if (_bullets[i].collideWithAgent(_humans[j])) {
					addParticle(_bullets[i].getPosition(), 5);
					// Damage human, and kill it if its out of health
					if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
						// If the human died, remove him
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
					}
					else {
						j++;
					}

					// Remove the bullet
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					i--; // Make sure we don't skip a bullet
						 // Since the bullet died, no need to loop through any more zombies
					break;
				}
				else {
					j++;
				}
			}
		}
	}
}

/// Gets user input and process it with a Input Manager
void MainGame::processInput() {
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			// Exit the game here!
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

/// Draws what is happening in the game
void MainGame::drawGame() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);

	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = _camera.getcamMatrix();

	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_levels[_currentLevel]->Draw();

	_agentSpriteBatch.Begin();
	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	for (int i = 0; i < _humans.size(); i++) {
		if (_camera.isBoxInView(_humans[i]->getPosition(), agentDims))
			_humans[i]->Draw(_agentSpriteBatch);
	}
	for (int i = 0; i < _zombies.size(); i++) {
		if (_camera.isBoxInView(_zombies[i]->getPosition(), agentDims))
			_zombies[i]->Draw(_agentSpriteBatch);
	}
	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].Draw(_agentSpriteBatch);
	}
	

	_agentSpriteBatch.End();
	_agentSpriteBatch.RenderBatch();

	m_particleEngine.Draw(&_agentSpriteBatch);

	drawHud();

	_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}

/// Draws the hud
void MainGame::drawHud() {
	char buffer[256];

	glm::mat4 projectionMatrix = _hudCamera.getcamMatrix();

	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_hudSpriteBatch.Begin();

	sprintf_s(buffer, "Humans: %d", _humans.size());

	_spriteFont->draw(
		_hudSpriteBatch, 
		buffer, 
		glm::vec2(0, 0),
		glm::vec2(CAMERA_SCALE),
		0.0f, 
		Engine2D::ColorRGBA8(255, 255, 255, 255));

	sprintf_s(buffer, "Zombies: %d", _zombies.size());

	_spriteFont->draw(
		_hudSpriteBatch,
		buffer,
		glm::vec2(_screenWidth, 0),
		glm::vec2(CAMERA_SCALE),
		0.0f,
		Engine2D::ColorRGBA8(255, 255, 255, 255),
		Engine2D::Justification::RIGHT);

	_hudSpriteBatch.End();
	_hudSpriteBatch.RenderBatch();
}

/// Add effects
void MainGame::addParticle(const glm::vec2 & position, int numParticles) {
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);
	
	glm::vec2 vel(1.0f, 0.0f);
	Engine2D::ColorRGBA8 color(255, 0, 0, 255);
	
	for (int i = 0; i < numParticles; i++)
		m_bloodparticleBatch->addParticles(position, glm::rotate(vel, randAngle(randEngine)), color, 10.0f);
}
