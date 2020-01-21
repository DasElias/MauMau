#include "ParticleTexture.h"

namespace card {
	ParticleTexture::ParticleTexture(SimpleTexture tex, int stages, bool isAdditive) :
		backboneTexture(tex),
		stages(stages),
		field_useAdditiveBlending(isAdditive) {
	}
	void ParticleTexture::bind() const {
		backboneTexture.bind();
	}
	uint32_t ParticleTexture::getTexId() const {
		return backboneTexture.getTexId();
	}
	int32_t ParticleTexture::getStages() const {
		return stages;
	}
	int32_t ParticleTexture::getWidth() const {
		return backboneTexture.getWidth();
	}
	int32_t ParticleTexture::getHeight() const {
		return backboneTexture.getHeight();
	}
	bool ParticleTexture::useAdditiveBlending() const {
		return field_useAdditiveBlending;
	}
	bool ParticleTexture::operator<(const ParticleTexture& other) const {
		return getTexId() < other.getTexId();
	}
	void ParticleTexture::cleanUp() {
		backboneTexture.cleanUp();
	}
}