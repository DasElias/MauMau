#include "AvatarGrid.h"
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/positioning/RelativePositioningInParent.h>
#include <egui/model/positioning/FunctionalPositioningInParent.h>
#include <egui/model/utils/PositioningUtils.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>

namespace card {
	float const AvatarGrid::WIDTH_PER_AVATAR = 1.0f / AvatarGrid::AVATARS_PER_ROW;

	AvatarGrid::AvatarGrid(AvatarTextures& avatarTextures, std::vector<Avatar> avatarsInGrid, Avatar defaultSelectedAvatar) :
			avatarTextures(avatarTextures),
			selectedAvatar(std::nullopt) {

		int amountOfRows = (avatarsInGrid.size() / AVATARS_PER_ROW) + 1;
		egui::Value avatarMaxWidth = {WIDTH_PER_AVATAR, egui::RelativityMode::RELATIVE_IN_PARENT};
		egui::Value avatarMaxHeight = {1.0f / amountOfRows, egui::RelativityMode::RELATIVE_IN_PARENT};

		hover = std::make_shared<egui::AspectRatioElement>(avatarTextures.getAspectRatio());
		addChildElement(hover);
		hover->setVisible(false);
		hover->setMaxWidth(avatarMaxWidth);
		hover->setMaxHeight(avatarMaxHeight);
		hover->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0.82f, 0.82f, 0.82f)));
		hover->setRadius(0.1f, true);

		selection = std::make_shared<egui::AspectRatioElement>(avatarTextures.getAspectRatio());
		addChildElement(selection);
		selection->setVisible(false);
		selection->setMaxWidth(avatarMaxWidth);
		selection->setMaxHeight(avatarMaxHeight);
		selection->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0.62f, 0.62f, 0.62f)));
		selection->setRadius(0.1f, true);
		if(std::find(avatarsInGrid.begin(), avatarsInGrid.end(), defaultSelectedAvatar) == avatarsInGrid.end()) {
			updateOverlay(std::nullopt, selection);
		} else {
			selectedAvatar = defaultSelectedAvatar;
			updateOverlay(selectedAvatar, selection);

		}
		
		for(int i = 0; i < avatarsInGrid.size(); i++) {
			auto avatarElem = std::make_shared<egui::AspectRatioElement>(avatarTextures.getAspectRatio());
			auto avatar = avatarsInGrid[i];
			avatarElem->setBackground(std::make_shared<egui::TexturedBackground>(
				avatarTextures.getImage(avatar)
			));
			images.push_back(std::make_pair(avatar, avatarElem));

			addChildElement(avatarElem);
			avatarElem->setMaxWidth(avatarMaxWidth);
			avatarElem->setMaxHeight(avatarMaxHeight);

			avatarElem->setOwnPositioning(genPositioningForAvatar(i));

			avatarElem->getMouseClickedEventManager().addEventHandler({[this, avatar](egui::MouseEvent&) {
				selectedAvatar = avatar;
				onSelectHandler(avatar);
				updateOverlay(selectedAvatar, selection);
			}});
			avatarElem->getHoverEventManager().addEventHandler({[this, avatar](egui::HoverEvent& evt) {
				std::optional<Avatar> hoveredAvatar;
				if(evt.isHovered()) hoveredAvatar = avatar;
				updateOverlay(hoveredAvatar, hover);
			}});
		}

	}
	float AvatarGrid::getHeightPerAvatar() const {
		float singleAvatarWidthInY = egui::y_pixelToPercent(egui::x_percentToPixel(WIDTH_PER_AVATAR));
		return singleAvatarWidthInY / avatarTextures.getAspectRatio();
	}
	std::optional<Avatar> AvatarGrid::getSelectedAvatar() const {
		return selectedAvatar;
	}
	void AvatarGrid::clearSelectedAvatar() {
		selectedAvatar = std::nullopt;
		updateOverlay(std::nullopt, selection);
		updateOverlay(std::nullopt, hover);
	}
	void AvatarGrid::setOnSelectHandler(std::function<void(Avatar)> handler) {
		onSelectHandler = handler;
	}
	void AvatarGrid::updateOverlay(std::optional<Avatar> avatar, std::shared_ptr<egui::AspectRatioElement> overlay) {
		bool shouldShow = avatar.has_value();
		overlay->setVisible(shouldShow);

		if(shouldShow) {
			auto it = std::find_if(images.begin(), images.end(), [this, avatar](std::pair<Avatar, std::shared_ptr<egui::AspectRatioElement>> value) {
				auto avatarValue = value.first;
				return avatarValue == *avatar;
			});
			std::size_t index = std::distance(images.begin(), it);
			overlay->setOwnPositioning(genPositioningForAvatar(index));
		}
	}
	std::shared_ptr<egui::Positioning> AvatarGrid::genPositioningForAvatar(std::size_t i) {
		return std::make_shared<egui::FunctionalPositioningInParent>(
			[i](PositionableElement& target) {
				float offsetInParentX = (i % AVATARS_PER_ROW) / float(AVATARS_PER_ROW);		// the purpose of the second division is to convert the value to [0; 1]
				return offsetInParentX;
			},
			[i, this](PositionableElement& target) {
				float const bottomSpace = 0.015f;

				int row = i / AVATARS_PER_ROW;
				float offsetInParentY = row * getHeightPerAvatar();
				offsetInParentY += bottomSpace * row;

				return offsetInParentY;
			}
		);
	}
}