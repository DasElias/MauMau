#pragma once
#include <glm/glm.hpp>
#include <array>
#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include "../renderingModel/DataVertexArrayObject.h"

namespace card {
	class CircleSectorRenderer {
		class CircleSectorShader : public VertexFragmentShaderProgram {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				int location_color;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				CircleSectorShader();

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void loadColor(glm::vec4 color);
		};


        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
			CircleSectorShader shader;
			DataVertexArrayObject vao;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
			CircleSectorRenderer();
			CircleSectorRenderer(const CircleSectorRenderer&) = delete;

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
			// center is the position of the circle center from [0, 1]; 
			// (0/0) means that the circle is in the top left corner of the screen,
			// (1/1) means that the circle is in the bottom right corner of the screen
            void renderSector_xDiameter(glm::vec2 center, float xDiameter, float startArc, float endArc, int segments, glm::vec4 color);

		private:
			void updateVao(glm::vec2 center, float xDiameter, float yDiameter, float startArc, float endArc, int segments);
			void renderImpl(glm::vec4 color);
	};
}