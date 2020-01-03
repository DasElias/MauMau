#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "ShaderProgram.h"

#include <fstream>
#include <vector>

#include <shared/utils/Logger.h>

namespace card {
	ShaderProgram::ShaderProgram() {
		programId = glCreateProgram();
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(programId);
	}

	uint32_t ShaderProgram::createUniformLocation(std::string variableName) {
		return glGetUniformLocation(programId, variableName.c_str());
	}

	void ShaderProgram::createShaderVariable(uint32_t position, std::string variableName) {
		glBindAttribLocation(programId, position, variableName.c_str());
	}

	void ShaderProgram::loadFloat(uint32_t uniformLocation, float value) {
		glUniform1f(uniformLocation, value);
	}

	void ShaderProgram::loadInt(uint32_t uniformLocation, int value) {
		glUniform1i(uniformLocation, value);
	}

	void ShaderProgram::loadVector4f(uint32_t uniformLocation, glm::vec4 value) {
		glUniform4f(uniformLocation, value.x, value.y, value.z, value.a);
	}

	void ShaderProgram::loadVector3f(uint32_t uniformLocation, glm::vec3 value) {
		glUniform3f(uniformLocation, value.x, value.y, value.z);
	}

	void ShaderProgram::loadVector2f(uint32_t uniformLocation, glm::vec2 value) {
		glUniform2f(uniformLocation, value.x, value.y);
	}

	void ShaderProgram::loadBool(uint32_t uniformLocation, bool value) {
		glUniform1i(uniformLocation, (value ? 1 : 0));
	}

	void ShaderProgram::loadMatrix4f(uint32_t uniformLocation, glm::mat4x4 value) {
		float* matrix = glm::value_ptr(value);
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, matrix);
	}

	uint32_t ShaderProgram::loadShader(uint32_t type, const char* shaderCode) {
	    // send shader to GPU
	    GLuint shader = glCreateShader(type);
	    glShaderSource(shader, 1, &shaderCode, 0);

	    // compile shader
	    glCompileShader(shader);
	    GLint compiled = 0;
	    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	    if(compiled == GL_FALSE) {
	    	GLint maxLength = 0;
	    	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	    	std::vector<GLchar> infoLog(maxLength);
	    	glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			log(LogSeverity::FATAL, "Couldn't compile shader!");
			log(LogSeverity::FATAL, &infoLog[0]);
	    	glDeleteShader(shader);

			exit(-1);
	    }

	    return shader;
	}

	void ShaderProgram::startProgram() {
		glUseProgram(programId);
	}


	void ShaderProgram::stopProgram() {
		glUseProgram(0);
	}


}
