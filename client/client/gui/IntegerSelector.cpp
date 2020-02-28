#include "IntegerSelector.h"
#include <stdexcept>
#include <egui/model/positioning/RelativePositioningInParent.h>
#include <egui/model/positioning/ValuedPositioningInParent.h>
#include <egui/themes/ThemeManager.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>

namespace card {
	float const IntegerSelector::BUTTONS_WIDTH = 0.1f;

	IntegerSelector::IntegerSelector(int value, int minValue, int maxValue) :
			value(value),
			minValue(minValue),
			maxValue(maxValue) {

		if(minValue > maxValue) throw std::logic_error("minValue mustn't be greater than maxValue");
		if(value < minValue) throw std::logic_error("value mustn't be smaller than minValue");
		if(value > maxValue) throw std::logic_error("value mustn't be greater than maxValue");
	

		init();
	}
	void IntegerSelector::init() {
		using namespace egui;

		// apply theme
		ThemeManager::getInstance().apply(this);

		setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {0.065f, RelativityMode::RELATIVE_ON_SCREEN});

		getTextComponent()->setForceOneLine(true);
		getTextComponent()->setFontSize(0.5f, true);
		getTextComponent()->setColor(Color(1, 1, 1));
		getTextComponent()->setVerticalAlignment(Text::VerticalAlignment::MIDDLE);
		getTextComponent()->setHorizontalAlignment(Text::HorizontalAlignment::CENTER);

		setBackground(std::make_shared<ColoredBackground>(Color()));
		int const BORDER_WIDTH = 2;
		setBorder(std::make_shared<Border>(Color(0.627f, 0.627f, 0.627f), SOLID, BORDER_WIDTH));

		createBtn(incrementButton, nullptr);
		incrementButton->setOwnPositioning(std::make_shared<ValuedPositioningInParent>(egui::Value(1 - BUTTONS_WIDTH, RelativityMode::RELATIVE_IN_PARENT), egui::Value(BORDER_WIDTH/2, RelativityMode::ABSOLUTE_VALUE)));
		incrementButton->setPreferredDimension({{BUTTONS_WIDTH, egui::RelativityMode::RELATIVE_IN_PARENT}, {-BORDER_WIDTH/2, egui::RelativityMode::ABSOLUTE_VALUE}}, {{0.5, egui::RelativityMode::RELATIVE_IN_PARENT}, {-BORDER_WIDTH/2, egui::RelativityMode::ABSOLUTE_VALUE}});
		createBtn(decrementButton, nullptr);
		decrementButton->setOwnPositioning(std::make_shared<ValuedPositioningInParent>(egui::Value(1 - BUTTONS_WIDTH, RelativityMode::RELATIVE_IN_PARENT), egui::Value(0.5f, RelativityMode::RELATIVE_IN_PARENT)));
		decrementButton->setPreferredDimension({{BUTTONS_WIDTH, egui::RelativityMode::RELATIVE_IN_PARENT}, {-BORDER_WIDTH/2, egui::RelativityMode::ABSOLUTE_VALUE}}, {{0.5, egui::RelativityMode::RELATIVE_IN_PARENT}, {-BORDER_WIDTH/2, egui::RelativityMode::ABSOLUTE_VALUE}});

	}
	std::string IntegerSelector::getClassName_static() {
		return "EGUI_IntegerSelector";
	}
	bool IntegerSelector::increment() {
		if(value == maxValue) return false;
		value++;
		return true;
	}
	bool IntegerSelector::decrement() {
		if(value == minValue) return false;
		value--;
		return true;
	}
	std::string IntegerSelector::getClassName() const {
		return getClassName_static();
	}
	int IntegerSelector::getValue() const {
		return value;
	}
	void IntegerSelector::createBtn(std::shared_ptr<egui::Button>& button, std::shared_ptr<egui::Image> bgImage) {
		button = std::make_shared<egui::Button>();
		addChildElement(button);
		button->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(1)));
		button->setBorder(nullptr);
		button->setRadius(0, false);
	}
}