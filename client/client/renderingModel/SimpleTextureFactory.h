#pragma once
#include "TextureProperties.h"
#include "SimpleTexture.h"

namespace card {
	class SimpleTextureFactory {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::string path;
            TextureMinFilter minFilter;
            TextureMagFilter magFilter;
            TextureWrap wrapS;
            TextureWrap wrapT;
            float anisotropicFiltering;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public: 
            SimpleTextureFactory(std::string path);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            SimpleTextureFactory& setMinFilter(TextureMinFilter minFilter);
            SimpleTextureFactory& setMagFilter(TextureMagFilter magFilter);
            SimpleTextureFactory& setWrapS(TextureWrap wrap);
            SimpleTextureFactory& setWrapT(TextureWrap wrap);
            SimpleTextureFactory& setAnisotropicFiltering(float a);

            SimpleTexture generateTexture();

	};
}