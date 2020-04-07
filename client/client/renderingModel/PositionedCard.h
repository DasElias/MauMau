#pragma once
#include <glm/glm.hpp>
#include <shared/model/Card.h>

namespace card {
	class PositionedCard {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			Card wrappedCard;
			glm::vec3 position;
			glm::vec3 rotation;
			float scale;
			float zIndex;


			glm::mat4x4 modelMatrix;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			PositionedCard(Card wrappedCard, glm::vec3 position, glm::vec3 rotation, float zIndex, float scale = 1.0f);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			glm::mat4 getModelMatrix() const;

			Card getCard() const;
			int getCardNumber() const;
			void setCard(Card c);
			void setPosition(glm::vec3 pos);
			void setRotation(glm::vec3 rot);
			void setScale(float scale);
			void changePosition(glm::vec3 addition);
			void changeRotation(glm::vec3 addition);
			void changeScale(float addition);
			glm::vec3 getPosition() const;
			glm::vec3 getRotation() const;
			float getScale() const;
			float getZIndex() const;

		private:
			void update();
	};
}