#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ColorRenderer3D.h"

#include "shaderPrograms/shaders/3d_color_vertex.glsl"
#include "shaderPrograms/shaders/3d_color_fragment.glsl"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


namespace card {
	ColorRenderer3D::ColorRenderer3DShader::ColorRenderer3DShader() :
		VertexFragmentShaderProgram(
			threed_color_vertex,
			threed_color_fragment,
			[this]() {
				this->createShaderVariable(0, "position");
				this->createShaderVariable(1, "color");
			}) {

		this->location_projectionViewMatrix = createUniformLocation("projectionViewMatrix");
	}

	void ColorRenderer3D::ColorRenderer3DShader::loadProjectionViewMatrix(glm::mat4x4 projectionViewMatrix) {
		loadMatrix4f(location_projectionViewMatrix, projectionViewMatrix);
	}


	void ColorRenderer3D::render(const DataColorVertexArrayObject& vertexArrayObject, const ProjectionMatrix& projectionMatrix, const Viewport& viewport, glm::mat4x4 modelMatrix) {
		shader.startProgram();
		shader.loadProjectionViewMatrix(projectionMatrix.getProjectionMatrix() * viewport.getViewMatrix() * modelMatrix);

		glBindVertexArray(vertexArrayObject.getVertexArrayObjectId());
		glDrawArrays(vertexArrayObject.getRenderMode(), 0, vertexArrayObject.getIndiciesCount());

		shader.stopProgram();
	}

}
