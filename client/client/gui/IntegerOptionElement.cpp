#include "IntegerOptionElement.h"
#include <egui/model/nodes/Label.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>

namespace card {
	IntegerOptionElement::IntegerOptionElement(std::string labelText, int defaultValue, int minValue, int maxValue) :
			HBox() {
		int const HEIGHT_PX = 30;
		int const SLIDER_WIDTH_PX = 100;

		auto label = std::make_shared<egui::Label>(labelText, float(HEIGHT_PX), false, egui::Text::HorizontalAlignment::LEFT, egui::Text::VerticalAlignment::MIDDLE, egui::Color(1, 1, 1));
		addChildElement(label);
		label->setPreferredWidth({{1, egui::RelativityMode::RELATIVE_IN_PARENT}, {-SLIDER_WIDTH_PX, egui::RelativityMode::ABSOLUTE_VALUE}});

		selector = std::make_shared<IntegerSelector>(defaultValue, minValue, maxValue, false);
		selector->setFontSize(0.75f, true);
		addChildElement(selector);
		selector->setPreferredWidth({SLIDER_WIDTH_PX, egui::RelativityMode::ABSOLUTE_VALUE});

		setPreferredHeight({HEIGHT_PX, egui::RelativityMode::ABSOLUTE_VALUE});
	}

	int IntegerOptionElement::getValue() const {
		return selector->getValue();
	}

	void IntegerOptionElement::setValue(int value) {
		selector->setValue(value);
	}
	
}