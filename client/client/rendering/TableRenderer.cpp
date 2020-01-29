#include "TableRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TableRenderer.h"

#include "shaderPrograms/shaders/table_vertex.glsl"
#include "shaderPrograms/shaders/table_fragment.glsl"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


namespace card {
	TableRenderer::TableRendererShader::TableRendererShader() :
		VertexFragmentShaderProgram(
			table_vertex,
			table_fragment,
			[this]() {
				this->createShaderVariable(0, "position");
				this->createShaderVariable(1, "texCoords");
				this->createShaderVariable(2, "normal");
			}) {

		this->location_projectionViewMatrix = createUniformLocation("projectionViewMatrix");
		this->location_modelMatrix = createUniformLocation("modelMatrix");
	}

	void TableRenderer::TableRendererShader::loadProjectionViewMatrix(glm::mat4x4 projectionViewMatrix) {
		loadMatrix4f(location_projectionViewMatrix, projectionViewMatrix);
	}

	void TableRenderer::TableRendererShader::loadModelMatrix(glm::mat4 modelMatrix) {
		loadMatrix4f(location_modelMatrix, modelMatrix);
	}

	TableRenderer::TableRenderer() :
		shader() {
	}

	void TableRenderer::render(const DataTextureNormalVertexArrayObject& vertexArrayObject, const ProjectionMatrix& projectionMatrix, const Viewport& viewport, glm::mat4x4 modelMatrix) {
		shader.startProgram();
		shader.loadProjectionViewMatrix(projectionMatrix.getProjectionMatrix() * viewport.getViewMatrix());
		shader.loadModelMatrix(modelMatrix);

		glBindVertexArray(vertexArrayObject.getVertexArrayObjectId());
		glDrawArrays(vertexArrayObject.getRenderMode(), 0, vertexArrayObject.getIndiciesCount());

		shader.stopProgram();
	}

}
