#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <string>

namespace card {
	class ShaderProgram {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			uint32_t programId;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			ShaderProgram();

		public:
			ShaderProgram(const ShaderProgram&) = delete;
			virtual ~ShaderProgram();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			uint32_t createUniformLocation(std::string variableName);
			void createShaderVariable(uint32_t position, std::string variableName);

			void loadFloat(uint32_t uniformLocation, float value);
			void loadInt(uint32_t uniformLocation, int value);
			void loadVector3f(uint32_t uniformLocation, glm::vec3 value);
			void loadVector2f(uint32_t uniformLocation, glm::vec2 value);
			void loadVector4f(uint32_t uniformLocation, glm::vec4 value);
			void loadBool(uint32_t uniformLocation, bool value);
			void loadMatrix4f(uint32_t uniformLocation, glm::mat4x4 value);

			uint32_t loadShader(uint32_t type, const char* shaderCode);

		public:
			void startProgram();
			void stopProgram();
	};
}
