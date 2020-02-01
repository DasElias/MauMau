#include "PlayerLabel.h"
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <shared/utils/MathUtils.h>
#include <iostream>

namespace card {
	float const PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN = 0.045f;
	int const PlayerLabel::SKIP_IMAGE_WIDTH_ADDITION = 30;

	PlayerLabel::PlayerLabel(float imageAspectRatio) :
			UnorganizedParentElement() {
		rootElement = std::make_shared<egui::VBox>();
		this->addChildElement(rootElement);

		imageElement = std::make_shared<egui::AspectRatioElement>(imageAspectRatio);
		rootElement->addChildElement(imageElement);
		imageElement->setMaxWidth({IMAGE_WIDTH_RELATIVE_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN});

		animationOverlayElement = std::make_shared<egui::AspectRatioElement>(1.0f);
		this->addChildElement(animationOverlayElement);
	

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
	void PlayerLabel::set(std::string playerName, std::optional<float> percentOverlayAnimationOrNone) {
		this->setVisible(true);
		playerNameLabel->setText(playerName);

		updateAnimationOverlay(percentOverlayAnimationOrNone);
	}
	void PlayerLabel::updateAnimationOverlay(std::optional<float> percentAnimationOverlayOrNone) {
		bool isSkipAnimationActive = percentAnimationOverlayOrNone.has_value();
		animationOverlayElement->setVisible(isSkipAnimationActive);

		int computedSkipImageWidthAddition = SKIP_IMAGE_WIDTH_ADDITION;
		int computedSkipImageYTranslation = 5;
		if(isSkipAnimationActive) {
			if(*percentAnimationOverlayOrNone > 0.5f) percentAnimationOverlayOrNone = (*percentAnimationOverlayOrNone - 1) * (-2);
			else percentAnimationOverlayOrNone = (*percentAnimationOverlayOrNone) * 2;

			computedSkipImageWidthAddition += 10 * (*percentAnimationOverlayOrNone);
			computedSkipImageYTranslation += 10 * (*percentAnimationOverlayOrNone);
		}

		animationOverlayElement->setMaxWidth({{IMAGE_WIDTH_RELATIVE_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN}, {2.0f * computedSkipImageWidthAddition, egui::RelativityMode::ABSOLUTE_VALUE}});
		animationOverlayElement->setXTranslation(-computedSkipImageWidthAddition);
		animationOverlayElement->setYTranslation(-computedSkipImageYTranslation);
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
	std::shared_ptr<egui::AspectRatioElement> PlayerLabel::getAnimationOverlayElementIfVisible() {
		if(animationOverlayElement->isVisible()) return animationOverlayElement;
		return nullptr;
	}
	void PlayerLabel::setOwnPositioning(std::shared_ptr<egui::Positioning> positioning) {
		UnorganizedParentElement::setOwnPositioning(positioning);
		rootElement->setOwnPositioning(positioning);
		animationOverlayElement->setOwnPositioning(positioning);
	}
}