#include "LabeledInputField.h"

namespace card {
	LabeledInputField::LabeledInputField(std::string titleString, egui::Color fontColor) :
			VBox(){
		inputField = std::make_shared<egui::InputField>();
		title = std::make_shared<egui::Label>(titleString, TITLE_FONT_SIZE_PX, false);
		title->getTextComponent()->setColor(fontColor);
		title->getTextComponent()->setFont(egui::Font::getFont("NotoSans Regular"));
		title->setPreferredHeight({TITLE_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		setPreferredHeight({{inputField->getPreferredHeight(), title->getPreferredHeight()}});

		addChildElement(title);
		addChildElement(inputField);

	}
	std::shared_ptr<egui::InputField> LabeledInputField::getInputFieldImpl() {
		return inputField;
	}
	std::string LabeledInputField::getText() const {
		return inputField->getText();
	}
	void LabeledInputField::setText(std::string text) {
		inputField->setText(text);
	}
}