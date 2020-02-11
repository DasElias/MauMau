#include "ContinueOptionsBackButtonBar.h"

namespace card {
	ContinueOptionsBackButtonBar::ContinueOptionsBackButtonBar(std::string backText, std::string optionsText, std::string continueText) :
			HBox(){
		float const BUTTON_WIDTH = 0.2f;
		float const SPACE_BETWEEN_ELEMS = 0.03f;

		this->backBtn = std::make_shared<ColoredButton>(ColoredButtonType::RED, backText);
		this->optionsBtn = std::make_shared<ColoredButton>(ColoredButtonType::BLUE, optionsText);
		this->continueBtn = std::make_shared<ColoredButton>(ColoredButtonType::GREEN, continueText);
		setPreferredHeight(backBtn->getPreferredHeight());
		addChildElement(backBtn);
		addChildElement(optionsBtn);
		addChildElement(continueBtn);
		backBtn->setPreferredWidth({BUTTON_WIDTH, egui::RelativityMode::RELATIVE_ON_SCREEN});
		optionsBtn->setPreferredWidth({BUTTON_WIDTH, egui::RelativityMode::RELATIVE_ON_SCREEN});
		continueBtn->setPreferredWidth({BUTTON_WIDTH, egui::RelativityMode::RELATIVE_ON_SCREEN});

		this->setShouldCenterElements(true);
		this->setSpaceBetweenElements({SPACE_BETWEEN_ELEMS, egui::RelativityMode::RELATIVE_ON_SCREEN});
	}
	void ContinueOptionsBackButtonBar::addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		backBtn->getActionEventManager().addEventHandler(handler);
	}
	void ContinueOptionsBackButtonBar::addOptionsBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		optionsBtn->getActionEventManager().addEventHandler(handler);
	}
	void ContinueOptionsBackButtonBar::addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		continueBtn->getActionEventManager().addEventHandler(handler);
	}
}