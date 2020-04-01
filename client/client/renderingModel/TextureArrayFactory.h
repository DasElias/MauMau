#pragma once
#include "TextureProperties.h"
#include "TextureArray.h"

namespace card {
    class TextureArrayFactory {
        struct TextureArrayData {
           unsigned char* imgData;
           int32_t width;
           int32_t height;
        };

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
            TextureArrayFactory();

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            TextureArrayFactory& setMinFilter(TextureMinFilter minFilter);
            TextureArrayFactory& setMagFilter(TextureMagFilter magFilter);
            TextureArrayFactory& setWrapS(TextureWrap wrap);
            TextureArrayFactory& setWrapT(TextureWrap wrap);
            TextureArrayFactory& setAnisotropicFiltering(float a);

            TextureArray loadFromFiles(std::vector<std::string> paths) const;
            TextureArray loadFromMemory(std::vector<const unsigned char*> imageData, std::vector<std::size_t> imageDataLength) const;

        private:
            TextureArray load(const std::vector<TextureArrayData>& texData) const;
            void free(const std::vector<TextureArrayData>& texData) const;


    };
}