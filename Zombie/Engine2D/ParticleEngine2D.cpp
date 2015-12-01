#include "ParticleEngine2D.h"
#include "ParticleBatch2D.h"
#include "SpriteBatch.h"

namespace Engine2D {

	ParticleEngine2D::ParticleEngine2D()
	{
	}
	ParticleEngine2D::~ParticleEngine2D() {
		for (auto& b : m_batches) {
			delete b;
		}
	}

	void ParticleEngine2D::addParticleBatch(ParticleBatch2D * particleBatch) {
		m_batches.push_back(particleBatch);
	}
	void ParticleEngine2D::Update(float deltaTime) {
		for (auto& b : m_batches) {
			b->Update(deltaTime);
		}
	}
	void ParticleEngine2D::Draw(SpriteBatch* spriteBatch) {
		for (auto& b : m_batches) {
			spriteBatch->Begin();

			b->Draw(spriteBatch);

			spriteBatch->End();
			spriteBatch->RenderBatch();
		}
	}
}