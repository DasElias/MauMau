#include "EditOptionsElement.h"
#include <egui/model/positioning/ValuedPositioningOnScreen.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
namespace card {
	EditOptionsElement::EditOptionsElement() :
			BasicOptionsElement("Tischregeln bearbeiten") {

		buttonBar = std::make_shared<ContinueBackButtonBar>("Abbrechen", u8"Änderungen übernehmen");
		addChildElement(buttonBar);
		buttonBar->setOwnPositioning(
			std::make_shared<egui::CenterXInParentWrapper>(
				std::make_shared<egui::RelativePositioningOnScreen>(
					0.0f, 0.9f
					)
				)
		);
	}
	void EditOptionsElement::addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		buttonBar->addBackBtnEventHandler(handler);
	}
	void EditOptionsElement::addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		buttonBar->addContinueBtnEventHandler(handler);
	}
}