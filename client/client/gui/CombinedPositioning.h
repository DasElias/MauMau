#pragma once
#include <egui/model/positioning/Positioning.h>
#include <initializer_list>
#include <vector>
#include <memory>

namespace card {
	class CombinedPositioning : public egui::Positioning {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::vector<std::shared_ptr<egui::Positioning>> parts;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CombinedPositioning(std::initializer_list<std::shared_ptr<egui::Positioning>> parts = {});

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			float getAbsXMargin(egui::PositionableElement& target);
			float getAbsYMargin(egui::PositionableElement& target);

			void addPart(std::shared_ptr<egui::Positioning> positioning);
			void addParts(std::initializer_list<std::shared_ptr<Positioning>> positionings);
	};
}