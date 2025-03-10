#pragma once
#include <array>
#include <cstdint>
#include "../renderingModel/DataVertexArrayObject.h"

namespace card {
	class PostProcessor {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::vector<float> const POSITIONS;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			DataVertexArrayObject quad;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			PostProcessor();

		public:
			PostProcessor(const PostProcessor&) = delete;
			virtual ~PostProcessor() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void render(uint32_t textureId, bool updateFbos);
	};
}
