#pragma once
#include <egui/model/scene/Node.h>

namespace card {
	void setCursorToHandOnHover(egui::Node& node);
	void setCursorToHandOnHoverAndResetOnClick(egui::Node& node);
}