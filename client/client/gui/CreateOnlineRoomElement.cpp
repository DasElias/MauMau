#include "CreateOnlineRoomElement.h"
#include <egui/model/nodes/HBox.h>
#include "ContinueOptionsBackButtonBar.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "LabeledInputField.h"
#include <algorithm>
#include "UsernameInputFieldCharacterFilter.h"

namespace card {
	CreateOnlineRoomElement::CreateOnlineRoomElement(AvatarTextures& avatarTextures) :
			BasicRoomCreationElement("Online-Raum erstellen", "Raum erstellen") {
		lockElement = std::make_shared<egui::UnorganizedParentElement>();
		lockElement->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0, 0, 0, 0.5f)));
		addChildElement(lockElement);
		lockElement->setVisible(false);

		avatarChooser = std::make_shared<AvatarChooser>(avatarTextures, 0);
		contentBox->addChildElement(avatarChooser);

		usernameInputField = std::make_shared<LabeledInputField>("Dein Benutzername", egui::Color(1.0f, 1.0f, 1.0f));
		usernameInputField->getInputFieldImpl()->setCharFilterFunction(
			UsernameInputFieldCharacterFilter(usernameInputField->getInputFieldImpl())
		);
		contentBox->addChildElement(usernameInputField);
		

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

	std::string CreateOnlineRoomElement::getUsernameInput() const {
		return usernameInputField->getText();
	}

	Avatar CreateOnlineRoomElement::getSelectedAvatar() const {
		return avatarChooser->getSelectedAvatar();
	}

	RoomOptions CreateOnlineRoomElement::getOptions() const {
		return options;
	}

	void CreateOnlineRoomElement::lockInput(bool shouldLock) {
		lockElement->setVisible(shouldLock);
	}
	
}