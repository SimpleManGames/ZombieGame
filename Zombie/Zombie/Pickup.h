#pragma once

#include <Engine2D/GLTexture.h>
#include <glm/glm.hpp>

enum class Type { RIFLE, SHOTGUN, AMMO };

class Pickup
{
public:
	Pickup();
	~Pickup();

	void Init(Engine2D::GLTexture texture, Type type, glm::vec2 position);

	Type getType() { return m_Type; }

private:
	Engine2D::GLTexture m_texture;
	Type m_Type;
	glm::vec2 m_position;
};

