#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <Engine2D/AudioEngine.h>

#include "Bullet.h"

class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed, Engine2D::SoundEffect fireEffect, bool isUnlocked = false);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime);
	bool isUnlocked() { return m_Unlocked; }

private:
	void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets);

	Engine2D::SoundEffect m_fireEffect;
	std::string _name;
	int _fireRate; ///< Fire rate in terms of frames
	int _bulletsPerShot; ///< How many bullets are fired at at time
	float _spread; ///< Accuracy
	float _bulletSpeed;
	int _bulletDamage;
	float _frameCounter; ///< Counts frames so we know when to shoot bullets
	bool m_Unlocked;

};

