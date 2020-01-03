#include "MenuElement.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>

namespace card {
	MenuElement::MenuElement() {
		setPreferredDimension({1, egui::RelativityMode::RELATIVE_ON_SCREEN}, {1, egui::RelativityMode::RELATIVE_ON_SCREEN});
		setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0.11f, 0.14f, 0.22f)));
	}
}
