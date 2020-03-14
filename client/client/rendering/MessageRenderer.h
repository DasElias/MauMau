#pragma once
#include <egui/model/scene/Scene.h>
#include <egui/model/nodes/Button.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/nodes/Label.h>
#include "../model/MessageQueue.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <array>

namespace card {
    class MessageRenderer {
        class TextToRender : public egui::UnorganizedParentElement {
            private:
                std::shared_ptr<egui::Label> text;
                std::shared_ptr<egui::Label> shadow;
                std::shared_ptr<egui::Background> coloredBackground;
                std::shared_ptr<egui::Background> linearGradientBackground;

            public:
                TextToRender(int fontSizePx);
                void setText(std::string text);
                void updateBackground(bool isLastElement);
        };

        typedef std::array<std::shared_ptr<TextToRender>, MessageQueue::MAX_MSG_AMOUNT> TextBoxesArray;
        
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            egui::MasterRenderer& eguiRenderer;
            TextBoxesArray textBoxesArray;
            egui::Scene scene;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            MessageRenderer(egui::MasterRenderer& eguiRenderer);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void render(const MessageQueue& msgQueue);

        private:
            void makeAllTextBoxesInvisible();

    };
}