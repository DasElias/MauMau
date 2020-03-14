#include "JoinOnlineRoomElement.h"
#include <egui/model/nodes/HBox.h>
#include "ContinueOptionsBackButtonBar.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "LabeledInputField.h"
#include <algorithm>
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include "UsernameInputFieldCharacterFilter.h"

namespace card {
	JoinOnlineRoomElement::JoinOnlineRoomElement(AvatarTextures& avatarTextures, std::size_t roomCodeMaxLength) :
			BasicRoomJoinElement("Online-Raum beitreten", "Beitreten") {
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
		
		roomCodeInputField = std::make_shared<LabeledInputField>("Raum-Code", egui::Color(1, 1, 1));
		roomCodeInputField->getInputFieldImpl()->setCharFilterToNumericWithMaxLength(roomCodeMaxLength);
		contentBox->addChildElement(roomCodeInputField);
	}

	std::string JoinOnlineRoomElement::getUsernameInput() const {
		return usernameInputField->getText();
	}

	std::string JoinOnlineRoomElement::getRoomCodeInput() const {
		return roomCodeInputField->getText();
	}

	Avatar JoinOnlineRoomElement::getSelectedAvatar() const {
		return avatarChooser->getSelectedAvatar();
	}

	void JoinOnlineRoomElement::lockInput(bool shouldLock) {
		lockElement->setVisible(shouldLock);
	}
	
}