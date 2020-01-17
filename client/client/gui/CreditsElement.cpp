#include "CreditsElement.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/nodeComponents/border/Border.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <array>
#include "../utils/FileUtils.h"

using namespace egui;

namespace card {
	CreditsElement::CreditsElement() {
		int const TITLE_FONT_SIZE_PX = 75;
		int const SUBTITLE_FONT_SIZE_PX = 40;
		int const CONTENT_FONT_SIZE_PX = 20;

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

		appendContent("Karten", "www.svg-cards.sourceforge.net");
		appendContent("Avatare", "www.getavataaars.com");
		appendContent("Textur fuer Tisch", "Petr Kovar - www.freeimages.com");
		appendContent("Icons", "Freepik - www.flaticon.com");

	}
	void CreditsElement::appendContent(std::string titleText, std::string contentText) {
		this->content->setText(this->content->getText() + titleText + "\n" + contentText + "\n\n");
	}
}