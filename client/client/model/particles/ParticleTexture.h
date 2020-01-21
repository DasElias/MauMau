#pragma once
#include "../../renderingModel/SimpleTexture.h"

namespace card {
	class ParticleTexture : public Cleanable {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			SimpleTexture backboneTexture;
			int stages;
			bool field_useAdditiveBlending;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ParticleTexture(SimpleTexture tex, int stages = 1, bool isAdditive = false);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void bind() const;
			uint32_t getTexId() const;
			int32_t getStages() const;
			int32_t getWidth() const;
			int32_t getHeight() const;
			bool useAdditiveBlending() const;
			bool operator<(const ParticleTexture&) const;

			void cleanUp() override;
	};
}