#include "PositionedCard.h"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace card {
	PositionedCard::PositionedCard(Card wrappedCard, glm::vec3 position, glm::vec3 rotation, float zIndex, float scale) :
			wrappedCard(wrappedCard),
			position(position),
			rotation(rotation),
			scale(scale),
			zIndex(zIndex) {

		update();
	}
	void PositionedCard::update() {
		modelMatrix = glm::mat4x4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, rotation.x, {1, 0, 0});
		modelMatrix = glm::rotate(modelMatrix, rotation.y, {0, 1, 0});
		modelMatrix = glm::rotate(modelMatrix, rotation.z, {0, 0, 1});
		modelMatrix = glm::scale(modelMatrix, {scale, scale, scale});
	}
	glm::mat4 PositionedCard::getModelMatrix() const {
		return modelMatrix;
	}
	Card PositionedCard::getCard() const {
		return wrappedCard;
	}
	int PositionedCard::getCardNumber() const {
		return wrappedCard.getCardNumber();
	}
	void PositionedCard::setCard(Card c) {
		this->wrappedCard = c;
	}
	void PositionedCard::setPosition(glm::vec3 pos) {
		this->position = pos;
		update();
	}
	void PositionedCard::setRotation(glm::vec3 rot) {
		this->rotation = rot;
		update();
	}
	void PositionedCard::setScale(float scale) {
		this->scale = scale;
		update();
	}
	void PositionedCard::changePosition(glm::vec3 addition) {
		position += addition;
		update();
	}
	void PositionedCard::changeRotation(glm::vec3 addition) {
		rotation += addition;
		update();
	}
	void PositionedCard::changeScale(float addition) {
		scale += addition;
		update();
	}
	glm::vec3 PositionedCard::getPosition() const {
		return position;
	}
	glm::vec3 PositionedCard::getRotation() const {
		return rotation;
	}
	float PositionedCard::getScale() const {
		return scale;
	}

	float PositionedCard::getZIndex() const {
		return zIndex;
	}
	
}