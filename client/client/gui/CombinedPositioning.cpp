#include "CombinedPositioning.h"

namespace card {
	CombinedPositioning::CombinedPositioning(std::initializer_list<std::shared_ptr<egui::Positioning>> parts) :
			parts(parts) {
	}
	float CombinedPositioning::getAbsXMargin(egui::PositionableElement& target) {
		float val = 0;
		for(auto& positioningPart : parts) {
			val += positioningPart->getAbsXMargin(target);
		}
		return val;
	}
	float CombinedPositioning::getAbsYMargin(egui::PositionableElement& target) {
		float val = 0;
		for(auto& positioningPart : parts) {
			val += positioningPart->getAbsYMargin(target);
		}
		return val;
	}
	void CombinedPositioning::addPart(std::shared_ptr<egui::Positioning> positioning) {
		parts.push_back(positioning);
	}
	void CombinedPositioning::addParts(std::initializer_list<std::shared_ptr<Positioning>> positionings) {
		for(auto& p : positionings) {
			addPart(p);
		}
	}
}