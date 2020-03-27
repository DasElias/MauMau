#include "PlayerLabel.h"
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <shared/utils/MathUtils.h>
#include <iostream>
#include "CombinedPositioning.h"
#include <egui\input\IOHandler.h>

namespace card {
	float const PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN = 0.045f;
	int const PlayerLabel::SKIP_IMAGE_WIDTH_ADDITION = 30;
	int const PlayerLabel::SPACE_BETWEEN_IMAGE_AND_LABEL_PX = 3;
	float const PlayerLabel::AVATAR_IMAGE_ASPECT_RATIO = 128 / float(128);
	int const PlayerLabel::LABEL_FONT_SIZE_PX = 25;

	PlayerLabel::PlayerLabel() :
			UnorganizedParentElement() {
		rootElement = std::make_shared<egui::VBox>();
		rootElement->setSpaceBetweenElements({SPACE_BETWEEN_IMAGE_AND_LABEL_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		this->addChildElement(rootElement);

		imageElement = std::make_shared<egui::AspectRatioElement>(AVATAR_IMAGE_ASPECT_RATIO);
		rootElement->addChildElement(imageElement);
		imageElement->setMaxWidth({IMAGE_WIDTH_RELATIVE_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN});

		int const TEXT_WIDTH_PIXEL_ADDITION = 100;
		playerNameLabel = std::make_shared<egui::Label>();
		playerNameLabel->getTextComponent()->setColor(egui::Color(1, 1, 1, 1));
		playerNameLabel->getTextComponent()->setForceOneLine(true);
		playerNameLabel->getTextComponent()->setFontSize(LABEL_FONT_SIZE_PX, false);
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
	float PlayerLabel::getHeightInPercent() {
		float height = IMAGE_WIDTH_RELATIVE_ON_SCREEN / AVATAR_IMAGE_ASPECT_RATIO * (egui::getDisplayHandler().getWidth() / float(egui::getDisplayHandler().getHeight()));
		height += egui::y_pixelToPercent(SPACE_BETWEEN_IMAGE_AND_LABEL_PX);
		height += egui::y_pixelToPercent(LABEL_FONT_SIZE_PX);
		return height;
	}
	void PlayerLabel::set(std::string playerName) {
		this->setVisible(true);
		playerNameLabel->setText(playerName);

	}
	void PlayerLabel::clear() {
		this->setVisible(false);
	}
	void PlayerLabel::setPositionOnScreen(float x, float y) {
		positioning->setX(x);
		positioning->setY(y);
	}

	void PlayerLabel::setPositionOnScreen(glm::vec2 pos) {
		setPositionOnScreen(pos.x, pos.y);
	}

	std::shared_ptr<egui::AspectRatioElement> PlayerLabel::getImageElement() {
		return imageElement;
	}

	void PlayerLabel::setOwnPositioning(std::shared_ptr<egui::Positioning> positioning) {
		UnorganizedParentElement::setOwnPositioning(positioning);
		rootElement->setOwnPositioning(positioning);
	}
}