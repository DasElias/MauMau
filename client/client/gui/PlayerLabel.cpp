#include "PlayerLabel.h"
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <shared/utils/MathUtils.h>
#include <iostream>

namespace card {
	float const PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN = 0.05f;
	int const PlayerLabel::SKIP_IMAGE_WIDTH_ADDITION = 25;

	PlayerLabel::PlayerLabel() :
			UnorganizedParentElement() {
		rootElement = std::make_shared<egui::VBox>();
		this->addChildElement(rootElement);

		imageElement = std::make_shared<egui::AspectRatioElement>(1.0f);
		rootElement->addChildElement(imageElement);
		imageElement->setMaxWidth({IMAGE_WIDTH_RELATIVE_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN});

		skipImageElement = std::make_shared<egui::AspectRatioElement>(1.0f);
		this->addChildElement(skipImageElement);
		

		int const TEXT_WIDTH_PIXEL_ADDITION = 100;
		playerNameLabel = std::make_shared<egui::Label>();
		playerNameLabel->getTextComponent()->setColor(egui::Color(1, 1, 1, 1));
		playerNameLabel->getTextComponent()->setForceOneLine(true);
		playerNameLabel->getTextComponent()->setFontSize(25, false);
		playerNameLabel->getTextComponent()->setHorizontalAlignment(egui::Text::HorizontalAlignment::CENTER);
		playerNameLabel->setXTranslation(-TEXT_WIDTH_PIXEL_ADDITION/2);
		playerNameLabel->setStretchX(TEXT_WIDTH_PIXEL_ADDITION);
		rootElement->addChildElement(playerNameLabel);
		playerNameLabel->setMaxWidth({IMAGE_WIDTH_RELATIVE_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN});


		this->setVisible(false);
		this->setMaxWidth({IMAGE_WIDTH_RELATIVE_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN});
		positioning = std::make_shared<egui::RelativePositioningOnScreen>();
		this->setOwnPositioning(positioning);
	}
	void PlayerLabel::set(std::string playerName, std::optional<float> percentSkipAnimationOrNone) {
		this->setVisible(true);
		playerNameLabel->setText(playerName);

		bool isSkipAnimationActive = percentSkipAnimationOrNone.has_value();
		skipImageElement->setVisible(isSkipAnimationActive);

		int computedSkipImageWidthAddition = SKIP_IMAGE_WIDTH_ADDITION;
		int computedSkipImageYTranslation = 5;
		if(isSkipAnimationActive) {
			if(* percentSkipAnimationOrNone > 0.5f) percentSkipAnimationOrNone = (* percentSkipAnimationOrNone - 1) * (-2);
			else percentSkipAnimationOrNone = (*percentSkipAnimationOrNone) * 2;
			std::cout << (*percentSkipAnimationOrNone) << std::endl;

			computedSkipImageWidthAddition += 10 * (*percentSkipAnimationOrNone);
			computedSkipImageYTranslation += 10 * (*percentSkipAnimationOrNone);
		} 

		skipImageElement->setMaxWidth({{IMAGE_WIDTH_RELATIVE_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN}, {2.0f * computedSkipImageWidthAddition, egui::RelativityMode::ABSOLUTE_VALUE}});
		skipImageElement->setXTranslation(-computedSkipImageWidthAddition);
		skipImageElement->setYTranslation(-computedSkipImageYTranslation);

	}
	void PlayerLabel::clear() {
		this->setVisible(false);
	}
	void PlayerLabel::setPositionOnScreen(float x, float y) {
		positioning->setX(x);
		positioning->setY(y);
	}
	std::shared_ptr<egui::AspectRatioElement> PlayerLabel::getImageElement() {
		return imageElement;
	}
	std::shared_ptr<egui::AspectRatioElement> PlayerLabel::getSkipElementIfVisible() {
		if(skipImageElement->isVisible()) return skipImageElement;
		return nullptr;
	}
	void PlayerLabel::setOwnPositioning(std::shared_ptr<egui::Positioning> positioning) {
		UnorganizedParentElement::setOwnPositioning(positioning);
		rootElement->setOwnPositioning(positioning);
		skipImageElement->setOwnPositioning(positioning);
	}
}