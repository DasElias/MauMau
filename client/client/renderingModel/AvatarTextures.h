#pragma once
#include "SimpleTexture.h"
#include <map>
#include <shared/model/Avatar.h>

namespace card {
	class AvatarTextures {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::map<Avatar, SimpleTexture> textures;
            SimpleTexture avatarNotFoundTexture;
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
            float getAspectRatio() const;
            void cleanUp();

        private:   
            void initTextures();

	};
}