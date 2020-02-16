#include "ParticipantsOverviewElement.h"
#include <egui/model/nodes/VBox.h>
#include <egui/model/nodes/HBox.h>
#include <egui/model/nodes/Label.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>

#include "ParticipantEntry.h"
#include <egui/model/positioning/RelativePositioningInParent.h>

namespace card {
	ParticipantsOverviewElement::ParticipantsOverviewElement() {
		int const TITLE_FONT_SIZE_PX = 20;
		int const ROOM_CODE_FONT_SIZE_PX = 70;
		

		float const MARGIN_LEFT_RIGHT = 0.015f;
		float const MARGIN_TOP = 0.01f;
		auto pageContentBox = std::make_shared<egui::VBox>();
		addChildElement(pageContentBox);
		pageContentBox->setPreferredWidth({1 - 2*MARGIN_LEFT_RIGHT, egui::RelativityMode::RELATIVE_IN_PARENT});
		pageContentBox->setOwnPositioning(std::make_shared<egui::RelativePositioningInParent>(MARGIN_LEFT_RIGHT, MARGIN_TOP));
		auto titleBox = std::make_shared<egui::VBox>();
		pageContentBox->addChildElement(titleBox);

		auto managementContentBox = std::make_shared<egui::HBox>();
		auto buttonBox = std::make_shared<egui::VBox>();
		auto participantsBox = std::make_shared<egui::VBox>();
		pageContentBox->addChildElement(managementContentBox);
		managementContentBox->addChildElement(buttonBox);
		managementContentBox->addChildElement(participantsBox);

		float const SPACE_BETWEEN_BUTTON_BOX_AND_PARTICIPANTS = 0.06f;
		managementContentBox->setSpaceBetweenElements({SPACE_BETWEEN_BUTTON_BOX_AND_PARTICIPANTS, egui::RelativityMode::RELATIVE_IN_PARENT});
		float const BUTTON_BOX_WIDTH = 0.1f;
		buttonBox->setPreferredWidth({BUTTON_BOX_WIDTH, egui::RelativityMode::RELATIVE_IN_PARENT});
		participantsBox->setPreferredWidth({1 - BUTTON_BOX_WIDTH - SPACE_BETWEEN_BUTTON_BOX_AND_PARTICIPANTS, egui::RelativityMode::RELATIVE_IN_PARENT});
		float const SPACE_BETWEEN_TITLE_AND_CONTENT = 0.01f;
		titleBox->setPreferredHeight({{TITLE_FONT_SIZE_PX + ROOM_CODE_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE}, {SPACE_BETWEEN_TITLE_AND_CONTENT, egui::RelativityMode::RELATIVE_IN_PARENT}});
		managementContentBox->setPreferredHeight({{1 - MARGIN_TOP - SPACE_BETWEEN_TITLE_AND_CONTENT, egui::RelativityMode::RELATIVE_IN_PARENT}, {-TITLE_FONT_SIZE_PX - ROOM_CODE_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE}});

		auto title = std::make_shared<egui::Label>("Raum-Code", TITLE_FONT_SIZE_PX, false, egui::Text::HorizontalAlignment::CENTER, egui::Text::VerticalAlignment::MIDDLE, egui::Color(1, 1, 1));
		title->setPreferredHeight({TITLE_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		titleBox->addChildElement(title);
		auto roomCode = std::make_shared<egui::Label>("210", ROOM_CODE_FONT_SIZE_PX, false, egui::Text::HorizontalAlignment::CENTER, egui::Text::VerticalAlignment::MIDDLE, egui::Color(1, 1, 1));
		roomCode->setPreferredHeight({ROOM_CODE_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		titleBox->addChildElement(roomCode);


		auto startGameBtn = std::make_shared<egui::AspectRatioElement>(1.0f);
		buttonBox->addChildElement(startGameBtn);
		startGameBtn->setMaxWidth({1.0f, egui::RelativityMode::RELATIVE_IN_PARENT});
		startGameBtn->setBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadTexture("C:\\Users\\Elias\\Downloads\\startGame.png")));
		startGameBtn->setRadius(0.1f, true);
		startGameBtn->setBorder(std::make_shared<egui::Border>(egui::Color(0.71f, 0.71f, 0.71f), egui::BorderType::SOLID, 1.0f));

		auto editOptionsBtn = std::make_shared<egui::AspectRatioElement>(1.0f);
		buttonBox->addChildElement(editOptionsBtn);
		editOptionsBtn->setMaxWidth({1.0f, egui::RelativityMode::RELATIVE_IN_PARENT});
		editOptionsBtn->setBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadTexture("C:\\Users\\Elias\\Downloads\\editOptions.png")));
		editOptionsBtn->setRadius(0.1f, true);
		editOptionsBtn->setBorder(std::make_shared<egui::Border>(egui::Color(0.71f, 0.71f, 0.71f), egui::BorderType::SOLID, 1.0f));

		auto leaveGameBtn = std::make_shared<egui::AspectRatioElement>(1.0f);
		buttonBox->addChildElement(leaveGameBtn);
		leaveGameBtn->setMaxWidth({1.0f, egui::RelativityMode::RELATIVE_IN_PARENT});
		leaveGameBtn->setBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadTexture("C:\\Users\\Elias\\Downloads\\quitGame.png")));
		leaveGameBtn->setRadius(0.1f, true);
		leaveGameBtn->setBorder(std::make_shared<egui::Border>(egui::Color(0.71f, 0.71f, 0.71f), egui::BorderType::SOLID, 1.0f));

		
	//	buttonBox->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0, 1)));

		participantsBox->addChildElement(std::make_shared<ParticipantEntry>("Hallo Welt!", true, true));
		participantsBox->addChildElement(std::make_shared<ParticipantEntry>("Hallo Welt!", true, true));
		participantsBox->setSpaceBetweenElements({5, egui::RelativityMode::ABSOLUTE_VALUE});
		buttonBox->setSpaceBetweenElements({10, egui::RelativityMode::ABSOLUTE_VALUE});

		auto joinAiPlayerBtn = std::make_shared<ColoredButton>(ColoredButtonType::GREEN, u8"Computer-Spieler hinzufügen");
		participantsBox->addChildElement(joinAiPlayerBtn);
		joinAiPlayerBtn->setYTranslation(50);
	}
}