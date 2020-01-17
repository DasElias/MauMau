#pragma once
#include <egui/model/utils/Image.h>
#include <array>
#include <shared/model/CardIndex.h>
#include "../renderingModel/SimpleTexture.h"

namespace card {
	class CardIndexTextures {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::array<SimpleTexture, 4> textures;
            std::array<std::shared_ptr<egui::Image>, 4> images;
            float aspectRatio;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
              CardIndexTextures();
              CardIndexTextures(const CardIndexTextures&) = delete;

        // ----------------------------------------------------------------------
        // ---------------------------STATIC-METHODS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static SimpleTexture loadCardIndexTex(std::string texName);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            std::shared_ptr<egui::Image> getImage(CardIndex cardIndex) const;
            SimpleTexture getTexture(CardIndex cardIndex) const;
            float getAspectRatio() const;

	};
}