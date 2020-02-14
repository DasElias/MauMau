#include "BasicRoomCreationElement.h"
#include "BasicRoomCreationElement.h"
#include <egui/model/positioning/ValuedPositioningOnScreen.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>


namespace card {
	BasicRoomCreationElement::BasicRoomCreationElement(std::string title, std::string continueBtnText) :
		TitledMenuElement(title) {

		buttonBar = std::make_shared<ContinueOptionsBackButtonBar>(u8"Zurück", u8"Tischregeln bearbeiten", continueBtnText);
		addChildElement(buttonBar);
		buttonBar->setOwnPositioning(
			std::make_shared<egui::CenterXInParentWrapper>(
				std::make_shared<egui::RelativePositioningOnScreen>(
					0.0f, 0.9f
				)
			)
		);
	}
	void BasicRoomCreationElement::addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		buttonBar->addBackBtnEventHandler(handler);
	}
	void BasicRoomCreationElement::addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		buttonBar->addContinueBtnEventHandler(handler);
	}
	void BasicRoomCreationElement::addOptionsBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		buttonBar->addOptionsBtnEventHandler(handler);
	}
}