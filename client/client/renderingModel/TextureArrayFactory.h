#pragma once
#include "TextureProperties.h"
#include "TextureArray.h"

namespace card {
    class TextureArrayFactory {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::vector<std::string> textures;
            TextureMinFilter minFilter;
            TextureMagFilter magFilter;
            TextureWrap wrapS;
            TextureWrap wrapT;
            float anisotropicFiltering;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            TextureArrayFactory(std::vector<std::string> path);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            TextureArrayFactory& setMinFilter(TextureMinFilter minFilter);
            TextureArrayFactory& setMagFilter(TextureMagFilter magFilter);
            TextureArrayFactory& setWrapS(TextureWrap wrap);
            TextureArrayFactory& setWrapT(TextureWrap wrap);
            TextureArrayFactory& setAnisotropicFiltering(float a);

            TextureArray generateTexture();

    };
}