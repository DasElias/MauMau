#pragma once
#include "VertexArrayObject.h"
#include <boost/optional.hpp>

namespace card {
	class DataTextureNormalVertexArrayObject : public VertexArrayObject {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int dimensions;
			std::uint32_t dataVboId;
			std::uint32_t textureVboId;
			std::uint32_t normalVboId;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			DataTextureNormalVertexArrayObject(RenderMode mode, int dimensions, const std::vector<float>& data, const std::vector<float>& textures, const std::vector<float>& normals);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			int getDimensions() const;
			void updateData(const std::vector<float>& data);
			void updateTexture(const std::vector<float>& textures);
			void updateNormals(const std::vector<float>& normals);

	};
}