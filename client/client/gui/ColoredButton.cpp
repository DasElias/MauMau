#include "ColoredButton.h"
#include <egui/themes/ThemeManager.h>

namespace card {
	ColoredButton::ColoredButton(ColoredButtonType buttonType, std::string text) :
			Button(text),
			buttonType(buttonType) {

		egui::ThemeManager::getInstance().apply(this);
	}
	std::string ColoredButton::getClassName_static() {
		return "MauMau_ColoredButton";
	}
	std::string ColoredButton::getClassName() const {
		return getClassName_static();
	}
	ColoredButtonType ColoredButton::getButtonType() const {
		return buttonType;
	}
}