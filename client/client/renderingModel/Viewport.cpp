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
	}
	void Viewport::setPosition(glm::vec3 pos) {
		this->position = pos;
	}
	void Viewport::setRotation(glm::vec3 rotation) {
		this->rotation = rotation;
	}
	void Viewport::setScale(float scale) {
		this->scale = scale;
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

		glm::mat4x4 mat = glm::mat4x4(1);
		mat = glm::translate(mat, position * glm::vec3(-1, -1, -1));

		mat = glm::rotate(mat, -rotation.x, {1, 0, 0});
		mat = glm::rotate(mat, -rotation.y, {0, 1, 0});
		mat = glm::rotate(mat, -rotation.z, {0, 0, 1});
		mat = glm::scale(mat, {scale, scale, scale});


		return mat;
	}
}