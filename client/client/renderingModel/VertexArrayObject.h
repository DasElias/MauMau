#pragma once

#include <cstdint>
#include <vector>
#include "../utils/Cleanable.h"

namespace card {
	class VertexArrayObject : public Cleanable {
		public:
		enum RenderMode {
			POINTS = 0,
			LINES,
			LINE_LOOP,
			LINE_STRIP,
			TRIANGLES,
			TRIANGLE_STRIP,
			TRIANGLE_FAN
		};

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			std::uint32_t vertexArrayObjectId = 0;
			std::size_t indiciesCount = 0;
			RenderMode renderMode;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			VertexArrayObject(RenderMode renderMode);

		public:
			virtual ~VertexArrayObject() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void cleanUp() override;
			std::uint32_t getVertexArrayObjectId() const;
			std::size_t getIndiciesCount() const;
			RenderMode getRenderMode() const;

		protected:
			std::uint32_t genFloatVbo(int indexInVao, int sizePerVertex, const std::vector<float>& data);
			void updateFloatVbo(std::uint32_t vboId, const std::vector<float>& data);
	};
}
