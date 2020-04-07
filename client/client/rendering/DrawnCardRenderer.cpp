#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DrawnCardRenderer.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodes/HBox.h>
#include <egui/model/nodes/vBox.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include "../renderingModel/ZIndicies.h"

namespace card {
	glm::vec3 const DrawnCardRenderer::POSITION = {0, 0, 0.25f};
	glm::vec3 const DrawnCardRenderer::ROTATION = {0, 0, 0};

	DrawnCardRenderer::DrawnCardRenderer(CardRenderer& cardRenderer, egui::MasterRenderer& eguiRenderer, 
			ProjectionMatrix& projectionMatrix, Viewport& viewport) :
				cardRenderer(cardRenderer),
				eguiRenderer(eguiRenderer),
				projectionMatrix(projectionMatrix),
				viewport(viewport),
				worldToScreenConverter(projectionMatrix, viewport),
				buttonBarPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.0f, 0.65f)) {

		this->playCardButton = std::make_shared<ColoredButton>(ColoredButtonType::BLUE, "Karte spielen");
		this->takeIntoHandCardsButton = std::make_shared<ColoredButton>(ColoredButtonType::BLUE, "Karte in die Hand nehmen");

		float const BUTTON_WIDTH = 0.2f;
		float const BUTTON_HEIGHT = 0.07f;
		float const DISTANCE_BETWEEN_BUTTONS = 0.1f;
		this->playCardButton->setPreferredDimension({{BUTTON_WIDTH, egui::RelativityMode::RELATIVE_ON_SCREEN}}, {{BUTTON_HEIGHT, egui::RelativityMode::RELATIVE_ON_SCREEN}});
		this->takeIntoHandCardsButton->setPreferredDimension({{BUTTON_WIDTH, egui::RelativityMode::RELATIVE_ON_SCREEN}}, {{BUTTON_HEIGHT, egui::RelativityMode::RELATIVE_ON_SCREEN}});

		std::shared_ptr<egui::HBox> buttonBar(new egui::HBox({this->playCardButton, this->takeIntoHandCardsButton}));
		buttonBar->setOwnPositioning(std::make_shared<egui::CenterXInParentWrapper>(buttonBarPositioning));
		buttonBar->setPreferredDimension({{2 * BUTTON_WIDTH + DISTANCE_BETWEEN_BUTTONS, egui::RelativityMode::RELATIVE_ON_SCREEN}}, {{BUTTON_HEIGHT, egui::RelativityMode::RELATIVE_ON_SCREEN}});
		buttonBar->setSpaceBetweenElements({{DISTANCE_BETWEEN_BUTTONS, egui::RelativityMode::RELATIVE_ON_SCREEN}});

		std::shared_ptr<egui::UnorganizedParentElement> rootElem(new egui::UnorganizedParentElement({buttonBar}));
		rootElem->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0, 0, 0, 0.55f)));
		this->scene.setRootElement(rootElem);
	}
	void DrawnCardRenderer::clearPreviousMouseEvents() {
		scene.discardMouseEvents();
	}
	void DrawnCardRenderer::render(Card card) {
		float centerYOfCard = worldToScreenConverter.convertWorldToScreen_percent(POSITION).y;
		centerYOfCard -= playCardButton->getComputedHeight() * 0.5f;	// we can use any of the buttons
		buttonBarPositioning->setY(centerYOfCard);

		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();

		renderOnlyCard(card);
	}
	void DrawnCardRenderer::renderOnlyCard(Card card) {
		PositionedCard positionedCard(card, POSITION, ROTATION, TEMP_LOCAL_START_Z_INDEX);
		cardRenderer.renderInNextPass(positionedCard, projectionMatrix, viewport);
	}
	void DrawnCardRenderer::addPlayHandler(std::function<void(void)> handler) {
		this->playCardButton->getActionEventManager().addEventHandler({[handler](egui::ActionEvent&) {
			handler();
		}});
	}
	void DrawnCardRenderer::addDrawHandler(std::function<void(void)> handler) {
		this->takeIntoHandCardsButton->getActionEventManager().addEventHandler({[handler](egui::ActionEvent&) {
			handler();
		}});
	}
}