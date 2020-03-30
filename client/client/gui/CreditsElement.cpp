#include "CreditsElement.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/nodeComponents/border/Border.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <array>
#include "../utils/FileUtils.h"
#include "ColoredButton.h"

using namespace egui;

namespace card {
	CreditsElement::CreditsElement() {
		int const TITLE_FONT_SIZE_PX = 77.5f;
		int const SUBTITLE_FONT_SIZE_PX = 42.5;
		int const CONTENT_FONT_SIZE_PX = 25.5f;

		auto title = std::make_shared<egui::Label>(
			"MauMau",
			TITLE_FONT_SIZE_PX,
			false,
			Text::HorizontalAlignment::CENTER,
			Text::VerticalAlignment::MIDDLE,
			Color(1, 1, 1)
		);
		auto subtitle = std::make_shared<egui::Label>(
			"by DasElias",
			SUBTITLE_FONT_SIZE_PX,
			false,
			Text::HorizontalAlignment::CENTER,	
			Text::VerticalAlignment::TOP,
			Color(1, 1, 1)
		);
		this->content = std::make_shared<egui::Label>(
			"",
			CONTENT_FONT_SIZE_PX,
			false,
			Text::HorizontalAlignment::CENTER,
			Text::VerticalAlignment::TOP,
			Color(1, 1, 1)
		);
		auto box = std::make_shared<egui::VBox>();
		box->addChildElement(title);
		box->addChildElement(subtitle);
		box->addChildElement(content);
		this->addChildElement(box);

		title->setPreferredHeight({TITLE_FONT_SIZE_PX, RelativityMode::ABSOLUTE_VALUE});
		subtitle->setPreferredHeight({SUBTITLE_FONT_SIZE_PX + 20, RelativityMode::ABSOLUTE_VALUE});

		appendContent("Karten", "svg-cards\nDavid Bellot - www.svg-cards.sourceforge.net\nLGPL-Lizenz (www.gnu.org/licenses/lgpl-3.0.en.html)");
		appendContent("Avatare", "Pablo Stanley - www.getavataaars.com");
		appendContent(u8"Textur für Tisch", "Petr Kovar - www.freeimages.com");
		appendContent("Icons", "Freepik - www.flaticon.com");
		appendContent("Verwendete Libraries", "GLFW/GLAD\nNanoVG\nBoost.Asio\nLoguru");

		backButton = std::make_shared<ColoredButton>(ColoredButtonType::BLUE, u8"Zurück");
		this->addChildElement(backButton);
		backButton->setOwnPositioning(std::make_shared<egui::CenterXInParentWrapper>(
			std::make_shared<egui::RelativePositioningOnScreen>(
				0.0f, 0.9f
			)
		));
		backButton->setPreferredWidth({400, egui::RelativityMode::ABSOLUTE_VALUE});

	}
	void CreditsElement::setOnBackBtn(egui::FunctionWrapper<ActionEvent> handler) {
		backButton->getActionEventManager().addEventHandler(handler);
	}
	void CreditsElement::appendContent(std::string titleText, std::string contentText) {
		this->content->setText(this->content->getText() + titleText + "\n" + contentText + "\n\n");
	}
}