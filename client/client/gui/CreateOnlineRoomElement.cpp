#include "CreateOnlineRoomElement.h"
#include <egui/model/nodes/HBox.h>
#include "ContinueOptionsBackButtonBar.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "LabeledInputField.h"
#include <algorithm>

namespace card {
	CreateOnlineRoomElement::CreateOnlineRoomElement(AvatarTextures& avatarTextures, std::size_t maxFieldLength) :
			BasicRoomCreationElement("Online-Raum erstellen", "Raum erstellen") {
		lockElement = std::make_shared<egui::UnorganizedParentElement>();
		lockElement->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0, 0, 0, 0.5f)));
		addChildElement(lockElement);
		lockElement->setVisible(false);

		avatarChooser = std::make_shared<AvatarChooser>(avatarTextures, 1);
		contentBox->addChildElement(avatarChooser);

		auto usernameCharVerification = [](char c) {
			return isalnum(c) || c == '.' || c == '_' || c == '-';
		};
		usernameInputField = std::make_shared<LabeledInputField>("Dein Benutzername", egui::Color(1.0f, 1.0f, 1.0f));
		usernameInputField->getInputFieldImpl()->setCharFilterFunction([this, maxFieldLength, usernameCharVerification](std::string entered) {
			return usernameInputField->getText().size() + entered.size() > maxFieldLength || std::find_if(entered.begin(), entered.end(), usernameCharVerification) == entered.end();
		});
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