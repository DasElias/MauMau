#include "BasicRoomCreationElement.h"
#include <egui/model/positioning/ValuedPositioningOnScreen.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>


namespace card {
	BasicRoomCreationElement::BasicRoomCreationElement(std::string title) :
		TitledMenuElement(title) {

		buttonBar = std::make_shared<ContinueOptionsBackButtonBar>(u8"Zurück", u8"Tischregeln bearbeiten", "Spiel starten");
		addChildElement(buttonBar);
		buttonBar->setOwnPositioning(
			std::make_shared<egui::CenterXInParentWrapper>(
				std::make_shared<egui::RelativePositioningOnScreen>(
					0.0f, 0.9f
				)
			)
		);

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
	void BasicRoomCreationElement::addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		buttonBar->addBackBtnEventHandler(handler);
	}
	void BasicRoomCreationElement::addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		buttonBar->addContinueBtnEventHandler(handler);
	}
}