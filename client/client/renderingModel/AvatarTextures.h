#pragma once
#include "SimpleTexture.h"
#include <map>
#include <shared/model/Avatar.h>
#include <egui/model/utils/Image.h>
namespace card {
	class AvatarTextures {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::map<Avatar, SimpleTexture> textures;
            std::map<Avatar, std::shared_ptr<egui::Image>> texturesAsImages;
            SimpleTexture avatarNotFoundTexture;
            std::shared_ptr<egui::Image> avatarNotFoundImage;
            std::int32_t width;
            std::int32_t height;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public: 
            AvatarTextures();
            AvatarTextures(const AvatarTextures&) = delete;

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void bind(Avatar avatar) const;
            std::shared_ptr<egui::Image> getImage(Avatar avatar) const;

            std::int32_t getWidth() const;
            std::int32_t getHeight() const;
            float getAspectRatio() const;
            void cleanUp();

        private:   
            void initTextures();

	};
}