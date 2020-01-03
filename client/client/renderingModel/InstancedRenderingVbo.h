#pragma once
#include "VertexArrayObject.h"
#include <vector>
#include <cstdint>

namespace card {
	class InstancedRenderingVbo {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			VertexArrayObject const targetVao;
			std::uint32_t vboId;
			int maxVboAttributeIndex = 0;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			InstancedRenderingVbo(VertexArrayObject targetVao, int amountOfFloats);
			InstancedRenderingVbo(const InstancedRenderingVbo&) = delete;
			~InstancedRenderingVbo();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void addInstancedAttribute(std::int32_t dataSize, std::int32_t instancedDataLengthInBytes, std::int32_t offsetInBytes);
			void updateVbo(std::vector<float>& data);


	};

}
