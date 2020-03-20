#include "Viewport.h"
#include "Viewport.h"
#include "Viewport.h"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace card {
	Viewport::Viewport(glm::vec3 position, glm::vec3 rotation, float scale) :
			position(position),
			rotation(rotation),
			scale(scale) {

		update();
	}
	void Viewport::addPosition(glm::vec3 posAddition) {
		this->position += posAddition;
		update();
	}
	void Viewport::setPosition(glm::vec3 pos) {
		this->position = pos;
		update();
	}
	void Viewport::setRotation(glm::vec3 rotation) {
		this->rotation = rotation;
		update();
	}
	void Viewport::setScale(float scale) {
		this->scale = scale;
		update();
	}
	glm::vec3 Viewport::getPosition() const {
		return position;
	}
	glm::vec3 Viewport::getRotation() const {
		return rotation;
	}
	float Viewport::getScale() const {
		return scale;
	}
	glm::mat4x4 Viewport::getViewMatrix() const {
		return viewMatrix;
	}
	void Viewport::update() {
		viewMatrix = glm::mat4x4(1);
		viewMatrix = glm::translate(viewMatrix, position * glm::vec3(-1, -1, -1));

		viewMatrix = glm::rotate(viewMatrix, -rotation.x, {1, 0, 0});
		viewMatrix = glm::rotate(viewMatrix, -rotation.y, {0, 1, 0});
		viewMatrix = glm::rotate(viewMatrix, -rotation.z, {0, 0, 1});
		viewMatrix = glm::scale(viewMatrix, {scale, scale, scale});
	}
}