#pragma once
#include <egui/model/nodes/Labeled.h>
#include <egui/model/nodes/Button.h>
#include <egui/model/utils/Image.h>

namespace card {
	class IntegerSelector : public egui::Labeled {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static float const BUTTONS_WIDTH;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int value;
			int minValue;
			int maxValue;

			std::shared_ptr<egui::Button> incrementButton;
			std::shared_ptr<egui::Button> decrementButton;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			IntegerSelector(int value = 0, int minValue = std::numeric_limits<int>::min(), int maxValue = std::numeric_limits<int>::max());

		// ----------------------------------------------------------------------
		// ---------------------------STATIC-METHODS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static std::string getClassName_static();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool increment();
			bool decrement();
			std::string getClassName() const override;
			int getValue() const;

		private:
			void init();
			void createBtn(std::shared_ptr<egui::Button>& button, std::shared_ptr<egui::Image> bgImage);
	};
}