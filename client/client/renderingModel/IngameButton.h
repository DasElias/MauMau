#pragma once
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/nodes/UnorganizedParentElement.h>

namespace card {
	class IngameButton : public egui::UnorganizedParentElement {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<egui::AspectRatioElement> mauMauButtonBgElement;
            std::shared_ptr<egui::AspectRatioElement> mauMauButtonClickable;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            IngameButton(std::shared_ptr<egui::Background> background, std::shared_ptr<egui::Background> disabledBackground, float relativeWidthOnScreen, std::function<void(void)> clickHandler);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void setOwnPositioning(std::shared_ptr<egui::Positioning> positioning) override;

	};
}