#include "BoolOptionElement.h"
#include <egui/model/nodes/Label.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>

namespace card {
	BoolOptionElement::BoolOptionElement(std::string labelText) :
			HBox() {
		int const HEIGHT_PX = 30;
		int const SLIDER_WIDTH_PX = 100;

		auto label = std::make_shared<egui::Label>(labelText, float(HEIGHT_PX), false, egui::Text::HorizontalAlignment::LEFT, egui::Text::VerticalAlignment::MIDDLE, egui::Color(1, 1, 1));
		addChildElement(label);
		label->setPreferredWidth({{1, egui::RelativityMode::RELATIVE_IN_PARENT}, {-SLIDER_WIDTH_PX, egui::RelativityMode::ABSOLUTE_VALUE}});

		switchElement = std::make_shared<ToggleSwitch>();
		addChildElement(switchElement);
		switchElement->setPreferredWidth({SLIDER_WIDTH_PX, egui::RelativityMode::ABSOLUTE_VALUE});

		setPreferredHeight({HEIGHT_PX, egui::RelativityMode::ABSOLUTE_VALUE});
	}
	bool BoolOptionElement::isToggled() const {
		return switchElement->isToggled();
	}
	void BoolOptionElement::set(bool isToggled) {
		switchElement->set(isToggled);
	}
	egui::EventManager<egui::ToggleEvent>& BoolOptionElement::getToggleEventManager() {
		return switchElement->getToggleEventManager();
	}
}