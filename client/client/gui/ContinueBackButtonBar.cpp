#include "ContinueBackButtonBar.h"

namespace card {
	ContinueBackButtonBar::ContinueBackButtonBar(std::string backBtnText, std::string continueBtnText) :
			HBox(){
		int const BUTTON_WIDTH_PX = 400;
		float const SPACE_BETWEEN_ELEMS = 0.03f;

		this->backBtn = std::make_shared<ColoredButton>(ColoredButtonType::RED, backBtnText);
		this->continueBtn = std::make_shared<ColoredButton>(ColoredButtonType::GREEN, continueBtnText);
		setPreferredHeight(backBtn->getPreferredHeight());
		addChildElement(backBtn);
		addChildElement(continueBtn);
		backBtn->setPreferredWidth({BUTTON_WIDTH_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		continueBtn->setPreferredWidth({BUTTON_WIDTH_PX, egui::RelativityMode::ABSOLUTE_VALUE});

		this->setShouldCenterElements(true);
		this->setSpaceBetweenElements({SPACE_BETWEEN_ELEMS, egui::RelativityMode::RELATIVE_ON_SCREEN});
		this->setMaxWidth({
			{2 * BUTTON_WIDTH_PX, egui::RelativityMode::ABSOLUTE_VALUE},
			{SPACE_BETWEEN_ELEMS, egui::RelativityMode::RELATIVE_ON_SCREEN}
		});

	}
	void ContinueBackButtonBar::addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		backBtn->getActionEventManager().addEventHandler(handler);
	}
	void ContinueBackButtonBar::addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		continueBtn->getActionEventManager().addEventHandler(handler);
	}
}