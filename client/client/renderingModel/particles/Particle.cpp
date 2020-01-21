#include "Particle.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace card {
	Particle::Particle(glm::vec3 position, glm::vec3 startVelocity, glm::vec3 velocityAdditionPerSecond, float lifeLengthSeconds, float rotation, float scale, ParticleTexture texture, glm::vec2 displacementOnScreen) :
			position(position),
			velocity(startVelocity),
			velocityAdditionPerSecond(velocityAdditionPerSecond),
			lifeLength(lifeLengthSeconds),
			rotation(rotation),
			scale(scale),
			elapsedTime(0),
			texture(texture),
			displacementOnScreen(displacementOnScreen) {
	}
	bool Particle::update(float delta, glm::vec3 playerPosition) {
		velocity.x += velocityAdditionPerSecond.x * delta;
		velocity.y += velocityAdditionPerSecond.y * delta;
		velocity.z += velocityAdditionPerSecond.z * delta;

		glm::vec3 change = velocity;
		change *= delta;
		position += change;
		
		distanceFromCamera = glm::length(playerPosition - this->position);
		updateTex(delta);

		elapsedTime += delta;
		return elapsedTime > lifeLength;
	}
	void Particle::updateTex(float delta) {
		float progress = (elapsedTime / lifeLength) * texture.getStages();

		int texIndex1 = int(floor(progress));
		int texIndex2 = (texIndex1 + 1 >= texture.getStages()) ? texIndex1 : texIndex1 + 1;

		this->texBlendFactor = fmod(progress, 1.0f);
		this->offsetTex1 = getTexOffset(texIndex1);
		this->offsetTex2 = getTexOffset(texIndex2);
	}
	glm::vec2 Particle::getTexOffset(int index) {
		float x = 0;
		float y = (texture.getHeight() / texture.getWidth()) * index;

		x /= texture.getWidth();
		y /= texture.getHeight();

		return {x, y};
	}
	glm::mat4x4 Particle::generateModelMatrix(glm::mat4x4 viewMatrix) const {
		glm::mat4 transformationMatrix = glm::mat4(1.0f);
		transformationMatrix = glm::translate(transformationMatrix, position);

		transformationMatrix[0][0] = viewMatrix[0][0];
		transformationMatrix[0][1] = viewMatrix[1][0];
		transformationMatrix[0][2] = viewMatrix[2][0];
		transformationMatrix[1][0] = viewMatrix[0][1];
		transformationMatrix[1][1] = viewMatrix[1][1];
		transformationMatrix[1][2] = viewMatrix[2][1];
		transformationMatrix[2][0] = viewMatrix[0][2];
		transformationMatrix[2][1] = viewMatrix[1][2];
		transformationMatrix[2][2] = viewMatrix[2][2];

		transformationMatrix = glm::rotate(transformationMatrix, rotation, {0, 0, 1});
		transformationMatrix = glm::scale(transformationMatrix, {scale, scale, scale});

		return transformationMatrix;

	}
	glm::vec3 Particle::getPosition() const {
		return position;
	}
	void Particle::setPosition(glm::vec3 position) {
		this->position = position;
	}
	float Particle::getRotation() const {
		return rotation;
	}
	float Particle::getScale() const {
		return scale;
	}
	ParticleTexture Particle::getTexture() const {
		return texture;
	}
	glm::vec2 Particle::getOffsetTex1() const {
		return offsetTex1;
	}
	glm::vec2 Particle::getOffsetTex2() const {
		return offsetTex2;
	}
	float Particle::getTexBlendFactor() const {
		return texBlendFactor;
	}
	float Particle::getDistanceFromCamera() const {
		return distanceFromCamera;
	}

	glm::vec2 Particle::getDisplacementOnScreen() const {
		return displacementOnScreen;
	}
	
}