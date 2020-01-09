#pragma once

#include <string>
#include "Texture.h"

namespace card {
	class SimpleTexture : public Texture {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			uint32_t texId;
			int32_t width;
			int32_t height;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			SimpleTexture(uint32_t texId, int32_t width, int32_t height);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			uint32_t getTexId() const override;
			void bind() const override;
			int32_t getWidth() const;
			int32_t getHeight() const;

			void cleanUp() override;
	};
}
