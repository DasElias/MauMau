#include "BasicRoomJoinElement.h"
#include "BasicRoomJoinElement.h"
#include <egui/model/positioning/ValuedPositioningOnScreen.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>


namespace card {
	BasicRoomJoinElement::BasicRoomJoinElement(std::string title, std::string continueBtnText) :
		TitledMenuElement(title) {

		buttonBar = std::make_shared<ContinueBackButtonBar>(u8"Zurück", continueBtnText);
		addChildElement(buttonBar);
		buttonBar->setOwnPositioning(
			std::make_shared<egui::CenterXInParentWrapper>(
				std::make_shared<egui::RelativePositioningOnScreen>(
					0.0f, 0.9f
				)
			)
		);
	}
	void BasicRoomJoinElement::addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		buttonBar->addBackBtnEventHandler(handler);
	}
	void BasicRoomJoinElement::addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		buttonBar->addContinueBtnEventHandler(handler);
	}
}