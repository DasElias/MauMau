#pragma once
#include <egui/model/nodes/Label.h>
#include <egui/model/nodes/Button.h>
#include <egui/model/utils/Image.h>
#include <egui/model/nodes/HBox.h>
#include <egui/model/nodes/VBox.h>
#include <egui/model/nodes/InputField.h>

namespace card {
	class IntegerSelector : public egui::HBox {
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

			std::shared_ptr<egui::Label> label;
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
			void setValue(int value);

			void setFontSize(float fontSize, bool isRelative) const;

		private:
			void init();
			void updateText();
			void createBtn(std::shared_ptr<egui::Button>& button, std::shared_ptr<egui::Image> bgImage, std::shared_ptr<egui::Image> hoverImage, std::shared_ptr<egui::Image> clickImage);
	};
}