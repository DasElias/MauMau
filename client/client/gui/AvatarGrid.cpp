#include "AvatarGrid.h"
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/positioning/RelativePositioningInParent.h>

namespace card {
	AvatarGrid::AvatarGrid(AvatarTextures& avatarTextures, std::vector<Avatar> avatarsInGrid) {
		float const WIDTH_PER_AVATAR = 1.0f / AVATARS_PER_ROW;
		float const HEIGHT_PER_AVATAR = WIDTH_PER_AVATAR / avatarTextures.getAspectRatio();
		
		for(int i = 0; i < avatarsInGrid.size(); i++) {
			auto avatarElem = std::make_shared<egui::AspectRatioElement>(avatarTextures.getAspectRatio());
			auto avatar = avatarsInGrid[i];
			avatarElem->setBackground(std::make_shared<egui::TexturedBackground>(
				avatarTextures.getImage(avatar)
			));
			images.push_back(avatarElem);

			addChildElement(avatarElem);
			avatarElem->setMaxDimension({WIDTH_PER_AVATAR, egui::RelativityMode::RELATIVE_IN_PARENT}, {HEIGHT_PER_AVATAR, egui::RelativityMode::RELATIVE_IN_PARENT});

			float offsetInParentX = (i % AVATARS_PER_ROW) / float(AVATARS_PER_ROW);		// the purpose of the second division is to convert the value to [0; 1]
			float offsetInParentY = (i / AVATARS_PER_ROW) / float(AVATARS_PER_ROW);
			avatarElem->setOwnPositioning(std::make_shared<egui::RelativePositioningInParent>(
				offsetInParentX, offsetInParentY
			));

		}

	}
}