#include "AvatarChoosePopup.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/positioning/RelativePositioningInParent.h>
#include <egui/model/positioning/ValuedPositioningOnScreen.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>

#include "CategorizedAvatarGrid.h"
#include <shared/model/AvatarUtils.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include "../utils/FileUtils.h"
#include "ColoredButton.h" 
namespace card {
	AvatarChoosePopup::AvatarChoosePopup(AvatarTextures& avatarTextures) {
		setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0, 0, 0, 0.25f)));
		setConsumeAllMouseEvents(true);

		float const CONTENT_WIDTH = 0.8f;
		float const CONTENT_HEIGHT = 0.95f;
		auto content = std::make_shared<egui::UnorganizedParentElement>();
		content->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(1, 1, 1)));
		addChildElement(content);
		content->setPreferredDimension({CONTENT_WIDTH, egui::RelativityMode::RELATIVE_IN_PARENT}, {CONTENT_HEIGHT, egui::RelativityMode::RELATIVE_IN_PARENT});
		content->setOwnPositioning(std::make_shared<egui::RelativePositioningInParent>((1 - CONTENT_WIDTH) / 2.0f, (1 - CONTENT_HEIGHT) / 2.0f));

		float const GRID_MARGIN_LEFT_RIGHT = 0.05f;
		float const GRID_MARGIN_TOP_BOTTOM = 0.05f;
		grid = std::make_shared<CategorizedAvatarGrid>(avatarTextures, 0);
		content->addChildElement(grid);
		grid->setPreferredDimension({1-GRID_MARGIN_LEFT_RIGHT, egui::RelativityMode::RELATIVE_IN_PARENT}, {1-GRID_MARGIN_TOP_BOTTOM, egui::RelativityMode::RELATIVE_IN_PARENT});
		grid->setOwnPositioning(std::make_shared<egui::RelativePositioningInParent>(GRID_MARGIN_LEFT_RIGHT / 2, GRID_MARGIN_TOP_BOTTOM / 2));


		auto closeBtn = std::make_shared<ColoredButton>(ColoredButtonType::RED, u8"Schließen");
		content->addChildElement(closeBtn);
		closeBtn->setOwnPositioning(std::make_shared<egui::CenterXInParentWrapper>(
			std::make_shared<egui::RelativePositioningInParent>(
				0.0f, 0.9f
			)
		));
		closeBtn->setPreferredWidth({0.33f, egui::RelativityMode::RELATIVE_IN_PARENT});
		closeBtn->getActionEventManager().addEventHandler({[this, closeBtn](egui::ActionEvent&) {
			onCloseFunction();
		}});
	}

	void AvatarChoosePopup::setOnCloseFunction(std::function<void(void)> func) {
		this->onCloseFunction = func;
	}

	Avatar AvatarChoosePopup::getSelectedAvatar() const {
		return grid->getSelectedAvatar();
	}

	void AvatarChoosePopup::setSelectedAvatar(Avatar a) {
		grid->setSelectedAvatar(a);
	}
	
}