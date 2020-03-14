#include "GameEndElement.h"
#include "GameEndElement.h"
#include "GameEndElement.h"
#include <egui/model/nodes/VBox.h>
#include <shared/utils/MathUtils.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <shared/utils/ThreadUtils.h>
#include <iostream>

namespace card {
	float const GameEndElement::BG_MIN_OPACITY = 0;
	float const GameEndElement::BG_MAX_OPACITY = 0.9f;
	float const GameEndElement::FONT_MIN_OPACITY = 0;
	float const GameEndElement::FONT_MAX_OPACITY = 1.0f;

	GameEndElement::GameEndElement() :
			bgOpacity(BG_MIN_OPACITY),
			fontOpacity(FONT_MIN_OPACITY) {
		// init fonts
		int const TITLE_FONT_SIZE = 110;
		this->titleLabel = std::make_shared<egui::Label>(
			"Gewinner",
			TITLE_FONT_SIZE,
			false,
			egui::Text::HorizontalAlignment::CENTER,
			egui::Text::VerticalAlignment::MIDDLE,
			egui::Color(1, 1, 1),
			egui::Font::getFont("NotoSans Black")
		);
		titleLabel->getTextComponent()->setForceOneLine(true);
		int const WINNER_FONT_SIZE = 50;
		this->winnerLabel = std::make_shared<egui::Label>(
			"Lokaler Spieler",
			WINNER_FONT_SIZE,
			false,
			egui::Text::HorizontalAlignment::CENTER,
			egui::Text::VerticalAlignment::MIDDLE,
			egui::Color(1, 1, 1),
			egui::Font::getFont("NotoSans Light")
		);
		winnerLabel->getTextComponent()->setForceOneLine(true);
		winnerLabel->setYTranslation(-5);

		auto lblContainer = std::make_shared<egui::VBox>();
		lblContainer->addChildElement(titleLabel);
		lblContainer->addChildElement(winnerLabel);
		lblContainer->setOwnPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.0f, 0.3f));
		titleLabel->setPreferredHeight({TITLE_FONT_SIZE, egui::RelativityMode::ABSOLUTE_VALUE});
		winnerLabel->setPreferredHeight({WINNER_FONT_SIZE, egui::RelativityMode::ABSOLUTE_VALUE});

		this->addChildElement(lblContainer);

		// init button bar
		this->buttonBar = std::make_shared<egui::HBox>();
		this->addChildElement(buttonBar);
		buttonBar->setShouldCenterElements(true);
		buttonBar->setSpaceBetweenElements({0.085, egui::RelativityMode::RELATIVE_ON_SCREEN});
		buttonBar->setOwnPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.0f, 0.85f));
		buttonBar->setPreferredHeight({50, egui::RelativityMode::ABSOLUTE_VALUE});

		this->continueBtn = std::make_shared<ColoredButton>(ColoredButtonType::BLUE, "Weiter");
		continueBtn->getTextComponent()->setFontSize(30, false);
	//	continueBtn->setPreferredWidth({0.25f, egui::RelativityMode::RELATIVE_IN_PARENT});
		continueBtn->setPreferredWidth({350, egui::RelativityMode::ABSOLUTE_VALUE});
		buttonBar->addChildElement(continueBtn);

		// init background
		background = std::make_shared<egui::ColoredBackground>();
		this->setBackground(background);

	}
	void GameEndElement::startAnimation() {
		bgOpacity = BG_MIN_OPACITY;
		fontOpacity = FONT_MIN_OPACITY;

		continueBtn->setVisible(false);
		threadUtils_invokeIn(BG_OPACITY_ANIMATION_DURATION_MS, [this]() {
			continueBtn->setVisible(true);
		});
	}
	void GameEndElement::setOnContinueBtn(egui::FunctionWrapper<egui::ActionEvent> evtHandler) {
		continueBtn->getActionEventManager().addEventHandler(evtHandler);
	}
	
	void GameEndElement::update(float deltaSeconds, std::string winnerUsername) {
		float deltaMs = deltaSeconds * 1000;
		updateBg(deltaMs);
		updateFonts(deltaMs);
		winnerLabel->setText(winnerUsername);
	}
	void GameEndElement::updateBg(float deltaMs) {
		bgOpacity += (BG_MAX_OPACITY - BG_MIN_OPACITY) / BG_OPACITY_ANIMATION_DURATION_MS * deltaMs;
		bgOpacity = clamp<float>(bgOpacity, BG_MIN_OPACITY, BG_MAX_OPACITY);

		egui::Color bgColor(0, 0, 0, bgOpacity);
		background->setBackgroundColor(bgColor);
	}
	void GameEndElement::updateFonts(float deltaMs) {
		float const TOLERANCE = 0.01f;
		if(bgOpacity > BG_MAX_OPACITY - TOLERANCE) {
			fontOpacity += (FONT_MAX_OPACITY - FONT_MIN_OPACITY) / FONT_OPACITY_ANIMATION_DURATION_MS * deltaMs;
			fontOpacity = clamp<float>(fontOpacity, FONT_MIN_OPACITY, FONT_MAX_OPACITY);
		}

		egui::Color fontColor(1, 1, 1, fontOpacity);
		titleLabel->getTextComponent()->setColor(fontColor);
		winnerLabel->getTextComponent()->setColor(fontColor);
	}
}