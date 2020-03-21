#include "IntegerSelector.h"
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
	IntegerSelector::IntegerSelector(int value, int minValue, int maxValue, bool renderButtonsVertically) :
			HBox(),
			value(value),
			minValue(minValue),
			maxValue(maxValue) {

		if(minValue > maxValue) throw std::logic_error("minValue mustn't be greater than maxValue");
		if(value < minValue) throw std::logic_error("value mustn't be smaller than minValue");
		if(value > maxValue) throw std::logic_error("value mustn't be greater than maxValue");
	

		init(renderButtonsVertically);
		update();
	}
	void IntegerSelector::init(bool renderButtonsVertically) {
		using namespace egui;

		// apply theme
		ThemeManager::getInstance().apply(this);

		setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {0.0575f, RelativityMode::RELATIVE_ON_SCREEN});
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

		createBtn(incrementButton);
		createBtn(decrementButton);

		if(renderButtonsVertically) {
			float const BUTTONS_WIDTH = 0.05f;
			setSpaceBetweenElements({5, egui::RelativityMode::ABSOLUTE_VALUE});

			auto buttonBox = std::make_shared<egui::VBox>();
			addChildElement(buttonBox);
			buttonBox->addChildElement(incrementButton);
			buttonBox->addChildElement(decrementButton);
			buttonBox->setPreferredWidth({{BUTTONS_WIDTH, egui::RelativityMode::RELATIVE_IN_PARENT}, {-BORDER_WIDTH/2, egui::RelativityMode::ABSOLUTE_VALUE}});
			buttonBox->setXTranslation(BORDER_WIDTH/2);
			incrementButton->setPreferredDimension({1, egui::RelativityMode::RELATIVE_IN_PARENT}, {0.5f, egui::RelativityMode::RELATIVE_IN_PARENT});
			decrementButton->setPreferredDimension({1, egui::RelativityMode::RELATIVE_IN_PARENT}, {0.5f, egui::RelativityMode::RELATIVE_IN_PARENT});
		} else {
			float const BUTTONS_WIDTH = 0.25f;
			setSpaceBetweenElements({10, egui::RelativityMode::ABSOLUTE_VALUE});

			auto buttonBox = std::make_shared<egui::HBox>();
			addChildElement(buttonBox);
			buttonBox->addChildElement(incrementButton);
			buttonBox->addChildElement(decrementButton);
			buttonBox->setPreferredWidth({2 * BUTTONS_WIDTH, egui::RelativityMode::RELATIVE_IN_PARENT});

			buttonBox->setSpaceBetweenElements({2, egui::RelativityMode::ABSOLUTE_VALUE});
			buttonBox->setMaxWidth({35, egui::RelativityMode::ABSOLUTE_VALUE});
			buttonBox->setXTranslation(BORDER_WIDTH / 2);
			buttonBox->setYTranslation(BORDER_WIDTH / 2);
			incrementButton->setPreferredDimension({0.5, egui::RelativityMode::RELATIVE_IN_PARENT}, {{1, egui::RelativityMode::RELATIVE_IN_PARENT}, {-BORDER_WIDTH / 2, egui::RelativityMode::ABSOLUTE_VALUE}});
			decrementButton->setPreferredDimension({0.5, egui::RelativityMode::RELATIVE_IN_PARENT}, {{1, egui::RelativityMode::RELATIVE_IN_PARENT}, {-BORDER_WIDTH / 2, egui::RelativityMode::ABSOLUTE_VALUE}});

		}

		incrementButton->getMouseClickedEventManager().addEventHandler({[this](egui::MouseEvent&) {
			this->increment();
		}});
		decrementButton->getMouseClickedEventManager().addEventHandler({[this](egui::MouseEvent&) {
			this->decrement();
		}});
		incrementButton->setBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_integerSelector_topTriangle, tex_integerSelector_topTriangle_size)));
		incrementButton->setHoveredBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_integerSelector_topTriangleHover, tex_integerSelector_topTriangleHover_size)), egui::RenderMode::RENDER_EXCLUSIVELY);
		incrementButton->setPressedBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_integerSelector_topTriangleClick, tex_integerSelector_topTriangleClick_size)), egui::RenderMode::RENDER_EXCLUSIVELY);
		incrementButton->setDisabledBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_integerSelector_topTriangle, tex_integerSelector_topTriangle_size), 0.85f), egui::RenderMode::RENDER_EXCLUSIVELY);
		decrementButton->setBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_integerSelector_bottomTriangle, tex_integerSelector_bottomTriangle_size)));
		decrementButton->setHoveredBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_integerSelector_bottomTriangleHover, tex_integerSelector_bottomTriangleHover_size)), egui::RenderMode::RENDER_EXCLUSIVELY);
		decrementButton->setPressedBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_integerSelector_bottomTriangleClick, tex_integerSelector_bottomTriangleClick_size)), egui::RenderMode::RENDER_EXCLUSIVELY);
		decrementButton->setDisabledBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_integerSelector_bottomTriangle, tex_integerSelector_bottomTriangle_size), 0.85f), egui::RenderMode::RENDER_EXCLUSIVELY);


	}

	void IntegerSelector::update() {
		label->setText(std::to_string(value));
		incrementButton->setDisabled(value == maxValue);
		decrementButton->setDisabled(value == minValue);
	}
	std::string IntegerSelector::getClassName_static() {
		return "EGUI_IntegerSelector";
	}
	bool IntegerSelector::increment() {
		if(value == maxValue) return false;
		value++;
		update();
		return true;
	}
	bool IntegerSelector::decrement() {
		if(value == minValue) return false;
		value--;
		update();
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
		update();
	}
	void IntegerSelector::setFontSize(float fontSize, bool isRelative) const {
		label->getTextComponent()->setFontSize(fontSize, isRelative);
	}
	void IntegerSelector::impl_setDisabled(bool isDisabled) {
		HBox::impl_setDisabled(isDisabled);
		incrementButton->setVisible(! isDisabled);
		decrementButton->setVisible(! isDisabled);
	}
	void IntegerSelector::createBtn(std::shared_ptr<egui::Button>& button) {
		button = std::make_shared<egui::Button>();
		button->setBorder(nullptr);
		button->setRadius(0, false);
	}
}