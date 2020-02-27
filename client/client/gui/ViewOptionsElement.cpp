#include "ViewOptionsElement.h"
#include <egui/model/positioning/ValuedPositioningOnScreen.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
namespace card {
	ViewOptionsElement::ViewOptionsElement() :
			BasicOptionsElement("Tischregeln bearbeiten") {

		auto overlayElement = std::make_shared<egui::UnorganizedParentElement>();
		addChildElement(overlayElement);
		overlayElement->fitToParentElement();
		overlayElement->setConsumeAllMouseEvents(true);

		backButton = std::make_shared<ColoredButton>(ColoredButtonType::RED, u8"Zurück");
		addChildElement(backButton);
		backButton->setOwnPositioning(
			std::make_shared<egui::CenterXInParentWrapper>(
				std::make_shared<egui::RelativePositioningOnScreen>(
					0.0f, 0.9f
					)
				)
		);

		int const BUTTON_WIDTH_PX = 400;
		backButton->setPreferredWidth({BUTTON_WIDTH_PX, egui::RelativityMode::ABSOLUTE_VALUE});
	}
	void ViewOptionsElement::addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		backButton->getActionEventManager().addEventHandler(handler);
	}
}