#include "CreateLocalRoomElement.h"
#include <egui/model/nodes/HBox.h>
#include "ContinueOptionsBackButtonBar.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "LabeledInputField.h"
#include <algorithm>

namespace card {
	CreateLocalRoomElement::CreateLocalRoomElement(AvatarTextures& avatarTextures, std::size_t maxFieldLength) :
			BasicRoomCreationElement("Einzelspieler-Raum erstellen", "Spiel starten") {

		avatarChooser = std::make_shared<AvatarChooser>(avatarTextures, 0);
		contentBox->addChildElement(avatarChooser);

		auto usernameCharVerification = [](char c) {
			return isalnum(c) || c == '.' || c == '_' || c == '-';
		};
		usernameInputField = std::make_shared<LabeledInputField>("Dein Benutzername", egui::Color(1.0f, 1.0f, 1.0f));
		usernameInputField->getInputFieldImpl()->setCharFilterFunction([this, maxFieldLength, usernameCharVerification](std::string entered) {
			return usernameInputField->getText().size() + entered.size() > maxFieldLength || std::find_if(entered.begin(), entered.end(), usernameCharVerification) == entered.end();
		});
		contentBox->addChildElement(usernameInputField);
		
		amountOfOpponentsInputField = std::make_shared<LabeledInputField>("Anzahl Gegner", egui::Color(1.0f, 1.0f, 1.0f));
		amountOfOpponentsInputField->getInputFieldImpl()->setCharFilterToNumericWithMaxLength(maxFieldLength);
		contentBox->addChildElement(amountOfOpponentsInputField);

		optionsElement = std::make_shared<EditOptionsElement>();
		addChildElement(optionsElement);
		optionsElement->setVisible(false);
		optionsElement->setConsumeAllMouseEvents(true);
		BasicRoomCreationElement::addOptionsBtnEventHandler({[this](egui::ActionEvent&) {
			optionsElement->loadOptions(options);
			optionsElement->setVisible(true);
		}});
		optionsElement->addBackBtnEventHandler({[this](egui::ActionEvent&) {
			optionsElement->setVisible(false);
		}});
		optionsElement->addContinueBtnEventHandler({[this](egui::ActionEvent&) {
			optionsElement->setVisible(false);
			this->options = optionsElement->getOptions();
		}});
	}

	std::string CreateLocalRoomElement::getUsernameInput() const {
		return usernameInputField->getText();
	}

	std::string CreateLocalRoomElement::getAmountOfOpponentsInput() const {
		return amountOfOpponentsInputField->getText();
	}

	Avatar CreateLocalRoomElement::getSelectedAvatar() const {
		return avatarChooser->getSelectedAvatar();
	}

	RoomOptions CreateLocalRoomElement::getOptions() const {
		return options;
	}
	
}