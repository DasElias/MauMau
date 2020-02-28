#include "IntegerSelector.h"
#include <stdexcept>
#include <egui/model/positioning/RelativePositioningInParent.h>
#include <egui/model/positioning/ValuedPositioningInParent.h>
#include <egui/themes/ThemeManager.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>

#include <res/integerSelector/bottomTriangle.png.h>
#include <res/integerSelector/bottomTriangleClick.png.h>
#include <res/integerSelector/bottomTriangleHover.png.h>
#include <res/integerSelector/topTriangle.png.h>
#include <res/integerSelector/topTriangleClick.png.h>
#include <res/integerSelector/topTriangleHover.png.h>

namespace card {
	float const IntegerSelector::BUTTONS_WIDTH = 0.25f;

	IntegerSelector::IntegerSelector(int value, int minValue, int maxValue) :
			HBox(),
			value(value),
			minValue(minValue),
			maxValue(maxValue) {

		if(minValue > maxValue) throw std::logic_error("minValue mustn't be greater than maxValue");
		if(value < minValue) throw std::logic_error("value mustn't be smaller than minValue");
		if(value > maxValue) throw std::logic_error("value mustn't be greater than maxValue");
	

		init();
		updateText();
	}
	void IntegerSelector::init() {
		using namespace egui;

		// apply theme
		ThemeManager::getInstance().apply(this);

		setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {0.065f, RelativityMode::RELATIVE_ON_SCREEN});
		setSpaceBetweenElements({10, egui::RelativityMode::ABSOLUTE_VALUE});
		label = std::make_shared<egui::Label>();
		addChildElement(label);
		label->setPreferredWidth({1, egui::RelativityMode::RELATIVE_IN_PARENT});
		label->getTextComponent()->setForceOneLine(true);
		label->getTextComponent()->setFontSize(0.5f, true);
		label->getTextComponent()->setColor(Color(1, 1, 1));
		label->getTextComponent()->setVerticalAlignment(Text::VerticalAlignment::MIDDLE);
		label->getTextComponent()->setHorizontalAlignment(Text::HorizontalAlignment::CENTER);

		label->setBackground(std::make_shared<ColoredBackground>(Color()));
		int const BORDER_WIDTH = 2;
		label->setBorder(std::make_shared<Border>(Color(0.627f, 0.627f, 0.627f), SOLID, BORDER_WIDTH));

		auto buttonBox = std::make_shared<egui::HBox>();
		buttonBox->setSpaceBetweenElements({2, egui::RelativityMode::ABSOLUTE_VALUE});
		buttonBox->setPreferredWidth({2 * BUTTONS_WIDTH, egui::RelativityMode::RELATIVE_IN_PARENT});
		buttonBox->setXTranslation(BORDER_WIDTH / 2);
		buttonBox->setYTranslation(BORDER_WIDTH / 2);
		addChildElement(buttonBox);
		createBtn(
			incrementButton, 
			egui::Image::loadFromMemory(tex_integerSelector_topTriangle, tex_integerSelector_topTriangle_size), 
			egui::Image::loadFromMemory(tex_integerSelector_topTriangleHover, tex_integerSelector_topTriangleHover_size),
			egui::Image::loadFromMemory(tex_integerSelector_topTriangleClick, tex_integerSelector_topTriangleClick_size)
		);
		buttonBox->addChildElement(incrementButton);
		incrementButton->setPreferredDimension({0.5, egui::RelativityMode::RELATIVE_IN_PARENT}, {{1, egui::RelativityMode::RELATIVE_IN_PARENT}, {-BORDER_WIDTH/2, egui::RelativityMode::ABSOLUTE_VALUE}});
		createBtn(
			decrementButton,
			egui::Image::loadFromMemory(tex_integerSelector_bottomTriangle, tex_integerSelector_bottomTriangle_size),
			egui::Image::loadFromMemory(tex_integerSelector_bottomTriangleHover, tex_integerSelector_bottomTriangleHover_size),
			egui::Image::loadFromMemory(tex_integerSelector_bottomTriangleClick, tex_integerSelector_bottomTriangleClick_size)
		);
		buttonBox->addChildElement(decrementButton);
		decrementButton->setPreferredDimension({0.5, egui::RelativityMode::RELATIVE_IN_PARENT}, {{1, egui::RelativityMode::RELATIVE_IN_PARENT}, {-BORDER_WIDTH/2, egui::RelativityMode::ABSOLUTE_VALUE}});

		incrementButton->getMouseClickedEventManager().addEventHandler({[this](egui::MouseEvent&) {
			this->increment();
		}});
		decrementButton->getMouseClickedEventManager().addEventHandler({[this](egui::MouseEvent&) {
			this->decrement();
		}});
	}
	void IntegerSelector::updateText() {
		label->setText(std::to_string(value));
	}
	std::string IntegerSelector::getClassName_static() {
		return "EGUI_IntegerSelector";
	}
	bool IntegerSelector::increment() {
		if(value == maxValue) return false;
		value++;
		updateText();
		return true;
	}
	bool IntegerSelector::decrement() {
		if(value == minValue) return false;
		value--;
		updateText();
		return true;
	}
	std::string IntegerSelector::getClassName() const {
		return getClassName_static();
	}
	int IntegerSelector::getValue() const {
		return value;
	}
	void IntegerSelector::setValue(int newValue) {
		if(newValue < minValue || newValue > maxValue) throw std::logic_error("Value must been between minVlaue and maxValue");
		this->value = newValue;
		updateText();
	}
	void IntegerSelector::setFontSize(float fontSize, bool isRelative) const {
		label->getTextComponent()->setFontSize(fontSize, isRelative);
	}
	void IntegerSelector::createBtn(std::shared_ptr<egui::Button>& button, std::shared_ptr<egui::Image> bgImage, std::shared_ptr<egui::Image> hoverImage, std::shared_ptr<egui::Image> clickImage) {
		button = std::make_shared<egui::Button>();
		button->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(1)));
		if(bgImage) button->setBackground(std::make_shared<egui::TexturedBackground>(bgImage));
		button->setHoveredBackground(std::make_shared<egui::TexturedBackground>(hoverImage), egui::RenderMode::RENDER_EXCLUSIVELY);
		button->setPressedBackground(std::make_shared<egui::TexturedBackground>(clickImage), egui::RenderMode::RENDER_EXCLUSIVELY);
		button->setBorder(nullptr);
		button->setRadius(0, false);
	}
}