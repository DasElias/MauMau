#include "LabeledIntegerSelector.h"

namespace card {
	LabeledIntegerSelector::LabeledIntegerSelector(std::string titleString, int defaultValue, int minValue, int maxValue, egui::Color fontColor) :
			VBox(){
		inputField = std::make_shared<IntegerSelector>(defaultValue, minValue, maxValue);
		title = std::make_shared<egui::Label>(titleString, TITLE_FONT_SIZE_PX, false);
		title->getTextComponent()->setColor(fontColor);
		title->getTextComponent()->setFont(egui::Font::getFont("NotoSans Regular"));
		title->setPreferredHeight({TITLE_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		setPreferredHeight({{inputField->getPreferredHeight(), title->getPreferredHeight()}});

		addChildElement(title);
		addChildElement(inputField);

	}
	std::shared_ptr<IntegerSelector> LabeledIntegerSelector::getIntegerSelectorImpl() {
		return inputField;
	}
	int LabeledIntegerSelector::getValue() const {
		return inputField->getValue();
	}

}