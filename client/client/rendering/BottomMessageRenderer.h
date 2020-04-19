#pragma once
#include <egui/model/scene/Scene.h>
#include <egui/model/nodes/Button.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/nodes/Label.h>
#include "../model/MessageQueue.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <array>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>

namespace card {
    class BottomMessageRenderer {
        class TextToRender : public egui::UnorganizedParentElement {
            private:
                static int const X_TRANSLATION = 17;
                static int const Y_TRANSLATION = 20;

                egui::MasterRenderer& eguiRenderer;
                std::shared_ptr<egui::Label> text;
                std::shared_ptr<egui::ColoredBackground> coloredBackground;
                int fontSizePx;

            public:
                TextToRender(int fontSizePx, egui::MasterRenderer& eguiRenderer);
                void setText(std::string text);
                void updateOpacity(long long messageAppendUnixTimeMs);
                void updateXTranslation(long long messageAppendUnixTimeMs);
        };

        typedef std::array<std::shared_ptr<TextToRender>, MessageQueue::MAX_MSG_AMOUNT> TextBoxesArray;

        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static float const MESSAGE_WIDTH_ON_SCREEN;
            static float const MESSAGE_RIGHT_MARGIN_ON_SCREEN;
            static int const PADDING_BETWEEN_MESSAGE_BOXES_PX;
            static int const MESSAGE_ANIMATION_DURATION_MS;
        
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            egui::MasterRenderer& eguiRenderer;
            TextBoxesArray textBoxesArray;
            egui::Scene scene;
            std::shared_ptr<egui::RelativePositioningOnScreen> parentPositioning;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            BottomMessageRenderer(egui::MasterRenderer& eguiRenderer);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void render(const MessageQueue& msgQueue);

        private:
            void makeAllTextBoxesInvisible();

    };
}