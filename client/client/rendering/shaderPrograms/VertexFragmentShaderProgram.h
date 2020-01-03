#pragma once

#include <string>
#include <functional>
#include "ShaderProgram.h"

namespace card {
	class VertexFragmentShaderProgram : public ShaderProgram {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			uint32_t vertexShaderId;
			uint32_t fragmentShaderId;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			VertexFragmentShaderProgram(const char* vertexShaderCode, const char* fragmentShaderCode, std::function<void(void)> bindShaderVariables);


	};
}
