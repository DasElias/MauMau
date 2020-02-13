#pragma once
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/events/EventManager.h>
#include <egui/model/events/ToggleEvent.h>
namespace card {
	class ToggleSwitch : public egui::Node {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static float const SWITCH_ELEM_PADDING;
            static float const SWTICH_ELEM_WIDTH;
            static int const ANIMATION_DURATION_MS = 125;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            egui::EventManager<egui::ToggleEvent> toggleEventManager;
            std::shared_ptr<egui::UnorganizedParentElement> switchElement;
            std::shared_ptr<egui::ColoredBackground> background;

            long long unixToggleTimeMs;
            bool isToggled_field;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ToggleSwitch();

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            bool isToggled() const;
            void set(bool isToggled);
            void toggle();
            void toggleWithAnimation();

            void renderGuiComponents(egui::MasterRenderer& renderer) override;
            egui::EventManager<egui::ToggleEvent>& getToggleEventManager();

        private:
            void updateToggleFieldAndFireEvent(bool newState);
            float getSwitchElementXMargin();
	};
}