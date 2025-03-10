#pragma once
#include <egui/model/nodes/VBox.h>
#include "BoolOptionElement.h"

namespace card {
    class OptionGroup : public egui::VBox {
        public:
            // the paramter is whether the root/parent element is toggled,
            // the return value is wheter the child should be disabled or not
            typedef std::function<bool(bool)> ShouldDisableChildFunction;


        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        public:
            static ShouldDisableChildFunction const DISABLE_NEVER;
            static ShouldDisableChildFunction const DISABLE_WHEN_PARENT_IS_TOGGLED;
            static ShouldDisableChildFunction const DISABLE_WHEN_PARENT_IS_NOT_TOGGLED;

        private:
            static int const SPACE_BETWEEN_ELEMS_PX;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<BoolOptionElement> rootOption;
            ShouldDisableChildFunction shouldDisableChildFunc;
            std::vector<std::shared_ptr<BoolOptionElement>> children;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            OptionGroup(std::shared_ptr<BoolOptionElement> rootOption, ShouldDisableChildFunction shouldDisableChildFunc);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void addChildOption(std::shared_ptr<BoolOptionElement> childOption);

        private:
            void updateDisabledStateOfChild(std::shared_ptr<BoolOptionElement> childOption);
            void updateDisabledStateOfChild(std::shared_ptr<BoolOptionElement> childOption, bool isToggled);
            void updatePreferredHeight();

	};
}