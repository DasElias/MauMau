#pragma once
#include "VertexArrayObject.h"

namespace card {
	class DataVertexArrayObject : public VertexArrayObject {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int dimensions;
			std::uint32_t dataVboId;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			DataVertexArrayObject(RenderMode mode, int dimensions, const std::vector<float>& data);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			int getDimensions() const;
			void updateData(const std::vector<float>& data);
			
	};
}