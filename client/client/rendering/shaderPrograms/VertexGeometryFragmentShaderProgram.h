#pragma once
#include <string>
#include <functional>
#include "ShaderProgram.h"

namespace card {
	class VertexGeometryFragmentShaderProgram : public ShaderProgram {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			uint32_t vertexShaderId;
			uint32_t geometryShaderId;
			uint32_t fragmentShaderId;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			VertexGeometryFragmentShaderProgram(const char* vertexShaderCode, const char* geometryShaderCode, const char* fragmentShaderCode, std::function<void(void)> bindShaderVariables);


	};
}
