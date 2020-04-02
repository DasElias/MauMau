#include "ClickableCursorUtils.h"
#include <egui/input/IOHandler.h>

namespace card {
	void setCursorToHandOnHover(egui::Node& node) {
		node.getHoverEventManager().addEventHandler({[](egui::HoverEvent& e) {
			egui::getCursorHandler().setCursorType(e.isHovered() ? egui::CursorType::HAND : egui::CursorType::ARROW);
		}});
	}
	void setCursorToHandOnHoverAndResetOnClick(egui::Node& node) {
		setCursorToHandOnHover(node);
		node.getMouseClickedEventManager().addEventHandler({[](egui::MouseEvent& e) {
			egui::getCursorHandler().setCursorType(egui::CursorType::ARROW);
		}});
	}
}