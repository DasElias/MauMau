#include "AvatarChooser.h"
#include "ColoredButton.h"
#include <egui/model/nodes/Label.h>
#include "../utils/FileUtils.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <res/menu/userhover.png.h>
#include "ClickableCursorUtils.h"

namespace card {
	AvatarChooser::AvatarChooser(AvatarTextures& avatarTextures_local, Avatar defaultAvatar) :
			VBox(),
			avatarTextures(avatarTextures_local),
			chosenAvatar(defaultAvatar) {

		setPreferredHeight({0.175f, egui::RelativityMode::RELATIVE_IN_PARENT});
		int const LABEL_FONT_SIZE_PX = 22;

		auto backgroundElementWrapper = std::make_shared<egui::HBox>();
		backgroundElementWrapper->setShouldCenterElements(true);
		addChildElement(backgroundElementWrapper);
		backgroundElementWrapper->setPreferredHeight({{1, egui::RelativityMode::RELATIVE_IN_PARENT}, {-LABEL_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE}});

		auto backgroundElement = std::make_shared<egui::AspectRatioElement>(avatarTextures_local.getAspectRatio());
		backgroundElementWrapper->addChildElement(backgroundElement);
		backgroundElement->setMaxHeight({1, egui::RelativityMode::RELATIVE_IN_PARENT});
		static std::shared_ptr<egui::Image> hoveredImage = egui::Image::loadFromMemory(tex_userHover, tex_userHover_size);
		backgroundElement->setHoveredBackground(std::make_shared<egui::TexturedBackground>(
			hoveredImage
		), egui::RENDER_EXCLUSIVELY);
		this->avatarBackground = std::make_shared<egui::TexturedBackground>(avatarTextures.getImage(chosenAvatar));
		backgroundElement->setBackground(avatarBackground);

		auto labelWrapper = std::make_shared<egui::HBox>();
		labelWrapper->setShouldCenterElements(true);
		addChildElement(labelWrapper);
		labelWrapper->setPreferredHeight({LABEL_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE});

		auto label = std::make_shared<egui::Label>("Avatar bearbeiten", LABEL_FONT_SIZE_PX, false, egui::Text::HorizontalAlignment::CENTER, egui::Text::VerticalAlignment::MIDDLE);
		labelWrapper->addChildElement(label);
		// magic constant for width, depends on LABEL_FONT_SIZE_PX
		label->setPreferredWidth({180, egui::RelativityMode::ABSOLUTE_VALUE});	
		label->getTextComponent()->setColor(egui::Color(0.36f, 0.6f, 0.83f));
		label->getTextComponent()->setFont(egui::Font::getFont("NotoSans Medium"));

		popup = std::make_shared<AvatarChoosePopup>(avatarTextures);
		popup->setZIndex(100);
		popup->setVisible(false);
		Node::addChildElement(popup);
		popup->setPreferredDimension({1, egui::RelativityMode::RELATIVE_ON_SCREEN}, {1, egui::RelativityMode::RELATIVE_ON_SCREEN});
		egui::FunctionWrapper<egui::MouseEvent> clickHandler = {[this](egui::MouseEvent&) {
			popup->setVisible(true);
		}};
		label->getMouseClickedEventManager().addEventHandler(clickHandler);
		backgroundElement->getMouseClickedEventManager().addEventHandler(clickHandler);
		popup->setOnCloseFunction([this]() {
			popup->setVisible(false);
			chosenAvatar = popup->getSelectedAvatar();
			avatarBackground->setImage(avatarTextures.getImage(chosenAvatar));
		});

		setCursorToHandOnHoverAndResetOnClick(*label);
		setCursorToHandOnHoverAndResetOnClick(*backgroundElement);
	}
	Avatar AvatarChooser::getSelectedAvatar() const {
		return chosenAvatar;
	}
	void AvatarChooser::setSelectedAvatar(Avatar a) {
		chosenAvatar = a;
		avatarBackground->setImage(avatarTextures.getImage(chosenAvatar));
		popup->setSelectedAvatar(a);
	}
}