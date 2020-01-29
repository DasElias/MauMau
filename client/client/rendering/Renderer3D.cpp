#include "Renderer3D.h"
#include "Renderer3D.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer3D.h"

#include "shaderPrograms/shaders/3d_vertex.glsl"
#include "shaderPrograms/shaders/3d_fragment.glsl"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


namespace card {
	Renderer3D::Renderer3DShader::Renderer3DShader(bool discardTransparentFragments) :
		VertexFragmentShaderProgram(
			threed_vertex,
			(discardTransparentFragments) ? threed_fragment_discardInvisible : threed_fragment,
			[this]() {
				this->createShaderVariable(0, "position");
				this->createShaderVariable(1, "texCoords");
				this->createShaderVariable(2, "normal");
			}) {

		this->location_projectionViewMatrix = createUniformLocation("projectionViewMatrix");
		this->location_modelMatrix = createUniformLocation("modelMatrix");
		this->location_cameraPosition = createUniformLocation("cameraPosition");
	}

	void Renderer3D::Renderer3DShader::loadCameraPosition(glm::vec3 cameraPosition) {
		loadVector3f(location_cameraPosition, cameraPosition);
	}

	void Renderer3D::Renderer3DShader::loadProjectionViewMatrix(glm::mat4x4 projectionViewMatrix) {
		loadMatrix4f(location_projectionViewMatrix, projectionViewMatrix);
	}

	void Renderer3D::Renderer3DShader::loadModelMatrix(glm::mat4 modelMatrix) {
		loadMatrix4f(location_modelMatrix, modelMatrix);
	}

	Renderer3D::Renderer3D(bool discardInvisibleFragments) :
		shader(discardInvisibleFragments) {
	}

	void Renderer3D::render(const DataTextureNormalVertexArrayObject& vertexArrayObject, const ProjectionMatrix& projectionMatrix, const Viewport& viewport, glm::mat4x4 modelMatrix) {
		shader.startProgram();
		shader.loadProjectionViewMatrix(projectionMatrix.getProjectionMatrix() * viewport.getViewMatrix());
		shader.loadCameraPosition(viewport.getPosition());
		shader.loadModelMatrix(modelMatrix);

		glBindVertexArray(vertexArrayObject.getVertexArrayObjectId());
		glDrawArrays(vertexArrayObject.getRenderMode(), 0, vertexArrayObject.getIndiciesCount());

		shader.stopProgram();
	}

}
