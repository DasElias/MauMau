#pragma once
#include "SamplerObject.h"
#include "TextureProperties.h"

namespace card {
	class SamplerObjectFactory {
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
            SamplerObjectFactory();

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            SamplerObjectFactory& setMinFilter(TextureMinFilter minFilter);
            SamplerObjectFactory& setMagFilter(TextureMagFilter magFilter);
            SamplerObjectFactory& setWrapS(TextureWrap wrap);
            SamplerObjectFactory& setWrapT(TextureWrap wrap);
            SamplerObjectFactory& setAnisotropicFiltering(float a);

            SamplerObject generateSampler();
	};
}