#include "Pickup.h"

Pickup::Pickup() { }
Pickup::~Pickup() { }

void Pickup::Init(Engine2D::GLTexture texture, Type type, glm::vec2 position) {
	m_Type = type;
	m_texture = texture;
	m_position = position;
}
