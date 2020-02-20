#include "ParticipantEntry.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodes/HBox.h>

namespace card {
	ParticipantEntry::ParticipantEntry(AvatarTextures& avatarTextures, std::string username, Avatar av, EntryAttributes attributes) :
			VBox() {	
		float const WIDTH = 0.15f;
		setPreferredWidth({WIDTH, egui::RelativityMode::RELATIVE_ON_SCREEN});
		auto avatarWrapper = std::make_shared<egui::HBox>();
		addChildElement(avatarWrapper);
		avatarWrapper->setPreferredHeight({0.3f, egui::RelativityMode::RELATIVE_IN_PARENT});
		avatarWrapper->setShouldCenterElements(true);

		auto avatarElement = std::make_shared<egui::AspectRatioElement>(avatarTextures.getAspectRatio());
		avatarWrapper->addChildElement(avatarElement);
		avatarElement->setMaxHeight({1, egui::RelativityMode::RELATIVE_IN_PARENT});
		this->avatar = std::make_shared<egui::TexturedBackground>(avatarTextures.getImage(av));
		avatarElement->setBackground(avatar);
	//	avatar->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(1)));

		int const LABEL_FONT_SIZE_PX = 20;
		this->usernameLabel = std::make_shared<egui::Label>(username, LABEL_FONT_SIZE_PX, false, egui::Text::HorizontalAlignment::CENTER, egui::Text::VerticalAlignment::MIDDLE, egui::Color(1, 1, 1), egui::Font::getFont("arialbd"));
		addChildElement(usernameLabel);
		usernameLabel->setPreferredHeight({LABEL_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE});

		int const DESCRIPTION_LABEL_FONT_SIZE_PX = LABEL_FONT_SIZE_PX - 3;
		std::string descriptionText = getDescriptionForAttributes(attributes);
		this->descriptionLabel = std::make_shared<egui::Label>(descriptionText, DESCRIPTION_LABEL_FONT_SIZE_PX, false, egui::Text::HorizontalAlignment::CENTER, egui::Text::VerticalAlignment::MIDDLE, egui::Color(1, 1, 1), egui::Font::getFont("ariali"));
		addChildElement(descriptionLabel);
		descriptionLabel->setPreferredHeight({DESCRIPTION_LABEL_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE});

		int BUTTON_HEIGHT_PX = 35;

		auto buttonBoxWrapper = std::make_shared<egui::HBox>();
		buttonBoxWrapper->setYTranslation(20);
		addChildElement(buttonBoxWrapper);
		buttonBoxWrapper->setShouldCenterElements(true);

		auto buttonBox = std::make_shared<egui::VBox>();
		buttonBoxWrapper->addChildElement(buttonBox);
		buttonBox->setPreferredWidth({0.7, egui::RelativityMode::RELATIVE_IN_PARENT});

		this->kickBtn = std::make_shared<ColoredButton>(ColoredButtonType::RED, u8"Entfernen");
		buttonBox->addChildElement(kickBtn);
		kickBtn->setPreferredDimension({1.0f, egui::RelativityMode::RELATIVE_IN_PARENT}, {float(BUTTON_HEIGHT_PX), egui::RelativityMode::ABSOLUTE_VALUE});
		kickBtn->setVisible(attributes & CAN_BE_KICKED);

		this->promoteBtn = std::make_shared<ColoredButton>(ColoredButtonType::BLUE, u8"Zum Raumleiter befördern");
		buttonBox->addChildElement(promoteBtn);
		promoteBtn->setPreferredDimension({1.0f, egui::RelativityMode::RELATIVE_IN_PARENT}, {float(BUTTON_HEIGHT_PX), egui::RelativityMode::ABSOLUTE_VALUE});
		promoteBtn->setVisible(attributes & CAN_BE_PROMOTED);

		buttonBoxWrapper->setPreferredDimension({1.0f, egui::RelativityMode::RELATIVE_IN_PARENT}, {float(buttonBoxWrapper->getChildrenElements().size() * BUTTON_HEIGHT_PX), egui::RelativityMode::ABSOLUTE_VALUE});
	}
	void ParticipantEntry::addKickHandler(std::function<void(void)> handler) {
		kickBtn->getActionEventManager().addEventHandler({[handler](egui::ActionEvent&) {
			handler();
		}});
	}
	void ParticipantEntry::addPromoteHandler(std::function<void(void)> handler) {
		promoteBtn->getActionEventManager().addEventHandler({[handler](egui::ActionEvent&) {
			handler();
		}});
	}
	std::string ParticipantEntry::getDescriptionForAttributes(EntryAttributes attributes) {
		if(attributes & AI_PLAYER) return "(Computer-Gegner)";
		if(attributes & LOCAL_PLAYER && attributes & ROOM_LEADER) return "(Du) (Raum-Leiter)";
		if(attributes & LOCAL_PLAYER) return "(Du)";
		if(attributes & ROOM_LEADER) return "(Raum-Leiter)";
		return "";
	}
}