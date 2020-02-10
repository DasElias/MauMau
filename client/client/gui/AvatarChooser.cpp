#include "AvatarChooser.h"
#include "AvatarChoosePopup.h"

namespace card {
	AvatarChooser::AvatarChooser(AvatarTextures& avatarTextures_local, Avatar defaultAvatar) :
			AspectRatioElement(avatarTextures_local.getAspectRatio()),
			avatarTextures(avatarTextures_local),
			chosenAvatar(defaultAvatar) {

		this->avatarBackground = std::make_shared<egui::TexturedBackground>(avatarTextures.getImage(chosenAvatar));
		setBackground(avatarBackground);
		
		auto popup = std::make_shared<AvatarChoosePopup>(avatarTextures);
		popup->setZIndex(100);
		popup->setVisible(false);
		addChildElement(popup);
		popup->setPreferredDimension({1, egui::RelativityMode::RELATIVE_ON_SCREEN}, {1, egui::RelativityMode::RELATIVE_ON_SCREEN});
		getMouseClickedEventManager().addEventHandler({[popup](egui::MouseEvent&) {
			popup->setVisible(true);
		}});
		popup->setOnCloseFunction([this, popup]() {
			popup->setVisible(false);
			chosenAvatar = popup->getSelectedAvatar();
			avatarBackground->setImage(avatarTextures.getImage(chosenAvatar));
		});
	}
	Avatar AvatarChooser::getSelectedAvatar() const {
		return chosenAvatar;
	}
}