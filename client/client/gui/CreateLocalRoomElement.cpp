#include "CreateLocalRoomElement.h"
#include <egui/model/nodes/HBox.h>
#include "ContinueOptionsBackButtonBar.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "LabeledInputField.h"
#include <algorithm>
#include "UsernameInputFieldCharacterFilter.h"

namespace card {
	CreateLocalRoomElement::CreateLocalRoomElement(AvatarTextures& avatarTextures) :
			BasicRoomCreationElement("Einzelspieler-Raum erstellen", "Spiel starten") {

		avatarChooser = std::make_shared<AvatarChooser>(avatarTextures, 0);
		contentBox->addChildElement(avatarChooser);

		usernameInputField = std::make_shared<LabeledInputField>("Dein Benutzername", egui::Color(1.0f, 1.0f, 1.0f));
		usernameInputField->getInputFieldImpl()->setCharFilterFunction(
			UsernameInputFieldCharacterFilter(usernameInputField->getInputFieldImpl())
		);
		contentBox->addChildElement(usernameInputField);
		
		amountOfOpponentsInputField = std::make_shared<LabeledIntegerSelector>("Anzahl Gegner", 1, 1, 3, egui::Color(1.0f, 1.0f, 1.0f));
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

	int CreateLocalRoomElement::getAmountOfOpponents() const {
		return amountOfOpponentsInputField->getValue();
	}

	Avatar CreateLocalRoomElement::getSelectedAvatar() const {
		return avatarChooser->getSelectedAvatar();
	}

	RoomOptions CreateLocalRoomElement::getOptions() const {
		return options;
	}
	
}