#pragma once
#include "TextureProperties.h"
#include "SimpleTexture.h"

namespace card {
	class SimpleTextureFactory {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            TextureMinFilter minFilter;
            TextureMagFilter magFilter;
            TextureWrap wrapS;
            TextureWrap wrapT;
            float anisotropicFiltering;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public: 
            SimpleTextureFactory();

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            SimpleTextureFactory& setMinFilter(TextureMinFilter minFilter);
            SimpleTextureFactory& setMagFilter(TextureMagFilter magFilter);
            SimpleTextureFactory& setWrapS(TextureWrap wrap);
            SimpleTextureFactory& setWrapT(TextureWrap wrap);
            SimpleTextureFactory& setAnisotropicFiltering(float a);

            SimpleTexture loadFromFile(std::string path) const;
            SimpleTexture loadFromMemory(const unsigned char* imgData, std::size_t imgDataLength) const;

        private:
            SimpleTexture load(unsigned char* stbiHandle, int32_t width, int32_t height) const;
            void free(unsigned char* stbiHandle) const;
	};
}