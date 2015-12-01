#pragma once

#define GLM_FORCE_RADIANS

#include <Engine2D/Window.h>
#include <Engine2D/GLSLProgram.h>
#include <Engine2D/Camera2D.h>
#include <Engine2D/InputManager.h>
#include <Engine2D/Timing.h>
#include <Engine2D/SpriteBatch.h>
#include <Engine2D/SpriteFont.h>
#include <Engine2D/AudioEngine.h>
#include <Engine2D/ParticleEngine2D.h>
#include <Engine2D/ParticleBatch2D.h>

#include <GL/glew.h>
#include <glm/gtx/rotate_vector.hpp>

#include "GameSystem.h"

#include "Player.h"
#include "Zombie.h"
#include "Level.h"
#include "Gun.h"
#include "Bullet.h"
#include "Human.h"
#include "Pickup.h"

class Zombie;

enum class GameState {
	PLAY,
	EXIT
};

class MainGame : public GameSystem
{
public:
	MainGame();
	~MainGame();

	/// Runs the game
	void run(int levelCount, int dungeonX, int dungeonY);

private:
	/// Initializes the core systems
	void initGame();

	/// Initializes the shaders
	void initShaders();

	/// Creates levels
	void generateLevels(int levelCount, int dungeonX, int dungeonY, int humanCount);

	/// Main game loop for the program
	void gameLoop();

	/// Updates Agents
	void updateAgents(float deltaTime);

	void updateBullets(float deltaTime);

	/// Handles input processing
	void processInput();

	/// Renders the game
	void drawGame();

	/// Draws the hud
	void drawHud();

	void addParticle(const glm::vec2& position, int numParticles);

	/// Member Variables

	Engine2D::GLSLProgram _textureProgram; ///< The shader program

	Engine2D::Camera2D _camera; ///< Main Camera
	Engine2D::Camera2D _hudCamera; ///< Hud Camera
	std::vector<Level*> _levels; /// Vector of levels
	int _currentLevel;

	Engine2D::InputManager _inputManager; ///< Handles input
	Engine2D::SpriteBatch _spriteBatch;
	Engine2D::SpriteFont* _spriteFont;
	Engine2D::AudioEngine m_audioEngine;

	Engine2D::SpriteBatch _agentSpriteBatch;
	Engine2D::SpriteBatch _hudSpriteBatch;
	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;
	std::vector<Bullet> _bullets;
	std::vector<Pickup*> _pickups;
	Player* _player;

	Engine2D::ParticleEngine2D m_particleEngine;
	Engine2D::ParticleBatch2D* m_bloodparticleBatch;

	GameState _gameState;

	int _fps;
};

