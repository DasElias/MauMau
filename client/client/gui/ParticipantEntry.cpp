#include "ParticipantEntry.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>

namespace card {
	ParticipantEntry::ParticipantEntry(std::string username, bool canPromote, bool canKick) :
			HBox() {	
		int const HEIGHT = 30;
		setPreferredHeight({HEIGHT, egui::RelativityMode::ABSOLUTE_VALUE});

		auto label = std::make_shared<egui::Label>(username, HEIGHT, false, egui::Text::HorizontalAlignment::LEFT, egui::Text::VerticalAlignment::MIDDLE, egui::Color(1, 1, 1));
		addChildElement(label);
		label->setPreferredWidth({{1, egui::RelativityMode::RELATIVE_IN_PARENT}, {-2 * HEIGHT, egui::RelativityMode::ABSOLUTE_VALUE}});

		auto kickBtn = std::make_shared<egui::Button>();
		addChildElement(kickBtn);
		kickBtn->setPreferredWidth({HEIGHT, egui::RelativityMode::ABSOLUTE_VALUE});

		auto promoteBtn = std::make_shared<egui::Button>();
		addChildElement(promoteBtn);
		promoteBtn->setPreferredWidth({HEIGHT, egui::RelativityMode::ABSOLUTE_VALUE});

	}
}