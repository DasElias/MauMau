#pragma once
#include "VertexArrayObject.h"
#include <boost/optional.hpp>

namespace card {
	class DataTextureVertexArrayObject : public VertexArrayObject {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int dimensions;
			std::uint32_t dataVboId;
			std::uint32_t textureVboId;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			DataTextureVertexArrayObject(RenderMode mode, int dimensions, const std::vector<float>& data, const std::vector<float>& textures);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			int getDimensions() const;
			void updateData(const std::vector<float>& data);
			void updateTexture(const std::vector<float>& textures);
			
	};
}