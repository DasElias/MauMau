#include "TitledMenuElement.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/nodes/VBox.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <egui/model/positioning/ValuedPositioningOnScreen.h>


namespace card {
	float const TitledMenuElement::TITLE_Y_MARGIN_PERCENT_ON_SCREEN = 0.025f;

	TitledMenuElement::TitledMenuElement(std::string titleString) :
			title(std::make_shared<egui::Label>(titleString, FONT_SIZE_PX, false, egui::Text::HorizontalAlignment::CENTER, egui::Text::VerticalAlignment::MIDDLE, egui::Color(0.36f, 0.6f, 0.83f), egui::Font::getFont("NotoSans ExtraBold"))) {

		addChildElement(title);
		title->setPreferredHeight({FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		title->setOwnPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.0f, TITLE_Y_MARGIN_PERCENT_ON_SCREEN));
		title->getTextComponent()->setForceOneLine(true);

		contentBox = std::make_shared<egui::VBox>();
		addChildElement(contentBox);
		contentBox->setOwnPositioning(
			std::make_shared<egui::CenterXInParentWrapper>(
				std::make_shared<egui::ValuedPositioningOnScreen>(
					egui::Value(0, egui::RelativityMode::ABSOLUTE_VALUE),
					egui::Value({
						TitledMenuElement::getYEndOfTitle(),
						{0.05f, egui::RelativityMode::RELATIVE_ON_SCREEN}
					})
				)
			)
		);
		contentBox->setSpaceBetweenElements({0.05f, egui::RelativityMode::RELATIVE_ON_SCREEN});
		contentBox->setMaxWidth({800, egui::RelativityMode::ABSOLUTE_VALUE});
	}
	void TitledMenuElement::setTitle(std::string title) {
		this->title->setText(title);
	}
	egui::Value TitledMenuElement::getYEndOfTitle() {
		return {
			{TITLE_Y_MARGIN_PERCENT_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN},
			{FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE}
		};
	}
}