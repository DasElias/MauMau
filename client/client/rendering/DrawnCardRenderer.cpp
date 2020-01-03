#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DrawnCardRenderer.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodes/HBox.h>
#include <egui/model/nodes/vBox.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <iostream>

namespace card {
	glm::vec3 const DrawnCardRenderer::POSITION = {0, 0, 0};
	glm::vec3 const DrawnCardRenderer::ROTATION = {0, 0, 0};

	DrawnCardRenderer::DrawnCardRenderer(CardRenderer& cardRenderer, egui::MasterRenderer& eguiRenderer, 
			ProjectionMatrix& projectionMatrix, Viewport& viewport, 
			std::function<void(void)> playCard, std::function<void(void)> addToCardStack) :
				cardRenderer(cardRenderer),
				eguiRenderer(eguiRenderer),
				projectionMatrix(projectionMatrix),
				viewport(viewport),
				playCardFunction(playCard),
				takeIntoHandCardsFunction(addToCardStack) {

		this->playCardButton = std::make_shared<egui::Button>("Karte spielen");
		this->playCardButton->getActionEventManager().addEventHandler({[this](egui::ActionEvent&) {
			playCardFunction();
		}});
		this->takeIntoHandCardsButton = std::make_shared<egui::Button>("Karte in die Hand nehmen");
		this->takeIntoHandCardsButton->getActionEventManager().addEventHandler({[this](egui::ActionEvent&) {
			takeIntoHandCardsFunction();
		}});

		float const BUTTON_WIDTH = 0.1f;
		float const BUTTON_HEIGHT = 0.1f;
		float const DISTANCE_BETWEEN_BUTTONS = 0.1f;
		this->playCardButton->setBackground((std::make_shared<egui::ColoredBackground>(egui::Color(0, 1, 0))));
		this->playCardButton->setPreferredDimension({{BUTTON_WIDTH, egui::RelativityMode::RELATIVE_ON_SCREEN}}, {{BUTTON_HEIGHT, egui::RelativityMode::RELATIVE_ON_SCREEN}});

		this->takeIntoHandCardsButton->setBackground((std::make_shared<egui::ColoredBackground>(egui::Color(1, 0, 0))));
		this->takeIntoHandCardsButton->setPreferredDimension({{BUTTON_WIDTH, egui::RelativityMode::RELATIVE_ON_SCREEN}}, {{BUTTON_HEIGHT, egui::RelativityMode::RELATIVE_ON_SCREEN}});

		std::shared_ptr<egui::HBox> buttonBar(new egui::HBox({this->playCardButton, this->takeIntoHandCardsButton}));
		buttonBar->setOwnPositioning(std::make_shared<egui::CenterXInParentWrapper>(std::make_shared<egui::RelativePositioningOnScreen>(0.0f, 0.5f)));
		buttonBar->setPreferredDimension({{2 * BUTTON_WIDTH + DISTANCE_BETWEEN_BUTTONS, egui::RelativityMode::RELATIVE_ON_SCREEN}}, {{BUTTON_HEIGHT, egui::RelativityMode::RELATIVE_ON_SCREEN}});
		buttonBar->setSpaceBetweenElements({{DISTANCE_BETWEEN_BUTTONS, egui::RelativityMode::RELATIVE_ON_SCREEN}});

		std::shared_ptr<egui::UnorganizedParentElement> rootElem(new egui::UnorganizedParentElement({buttonBar}));
		rootElem->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0, 0, 0, 0.85f)));
		this->scene.setRootElement(rootElem);
	}
	void DrawnCardRenderer::clearPreviousMouseEvents() {
		scene.discardMouseEvents();
	}
	void DrawnCardRenderer::render(Card card, bool canPlay, bool canTakeIntoHandCards) {
		playCardButton->setDisabled(! canPlay);
		takeIntoHandCardsButton->setDisabled(! canTakeIntoHandCards);

		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		std::vector<PositionedCard> positionedCards = {
			PositionedCard(card, POSITION, ROTATION)
		};
		cardRenderer.renderInNextPass(positionedCards, projectionMatrix, viewport);
	}
}