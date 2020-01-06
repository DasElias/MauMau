#include "PlayerLabel.h"
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>

namespace card {
	float const PlayerLabel::WIDTH_RELATIVE_ON_SCREEN = 0.05f;

	PlayerLabel::PlayerLabel() :
			VBox() {
		imageElement = std::make_shared<egui::AspectRatioElement>(1.0f);
		this->addChildElement(imageElement);
		imageElement->setMaxWidth({WIDTH_RELATIVE_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN});

		playerNameLabel = std::make_shared<egui::Label>();
		playerNameLabel->getTextComponent()->setColor(egui::Color(1, 1, 1, 1));
		playerNameLabel->getTextComponent()->setForceOneLine(true);
		playerNameLabel->getTextComponent()->setFontSize(20, false);
		playerNameLabel->getTextComponent()->setHorizontalAlignment(egui::Text::HorizontalAlignment::CENTER);
		this->addChildElement(playerNameLabel);
		playerNameLabel->setMaxWidth({WIDTH_RELATIVE_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN});


		this->setVisible(false);
		this->setMaxWidth({WIDTH_RELATIVE_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN});
		positioning = std::make_shared<egui::RelativePositioningOnScreen>();
		this->setOwnPositioning(positioning);
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
	std::shared_ptr<egui::AspectRatioElement> PlayerLabel::getImageElement() {
		return imageElement;
	}
}