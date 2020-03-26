#pragma once
#include "VertexArrayObject.h"
#include <boost/optional.hpp>

namespace card {
	class DataColorVertexArrayObject : public VertexArrayObject {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int dimensions;
			std::uint32_t dataVboId;
			std::uint32_t colorVboId;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			DataColorVertexArrayObject(RenderMode mode, int dimensions, const std::vector<float>& data, const std::vector<float>& colorData);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			int getDimensions() const;
			void updateData(const std::vector<float>& data);
			void updateColor(const std::vector<float>& colorData);
			
	};
}