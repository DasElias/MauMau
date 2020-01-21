#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ParticleRenderer.h"
#include "shaderPrograms/shaders/particle_vertex.glsl"
#include "shaderPrograms/shaders/particle_fragment.glsl"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <shared/utils/Logger.h>
#include <egui/input/IOHandler.h>

namespace card {
	std::vector<float> const ParticleRenderer::VERTICES = {{-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f}};

	ParticleRenderer::ParticleShader::ParticleShader() :
			VertexFragmentShaderProgram(
				particle_vertex, 
				particle_fragment, 
				[this](){
					createShaderVariable(0, "position");
					createShaderVariable(1, "modelViewMatrix");
					createShaderVariable(5, "texOffsets");
					createShaderVariable(6, "blendFactor");
					createShaderVariable(7, "displacementOnScreen");
				}) {

		location_textureRowsCount = createUniformLocation("textureRowsCount");
		location_projectionMatrix = createUniformLocation("projectionMatrix");
	}
	void ParticleRenderer::ParticleShader::loadProjectionMatrix(glm::mat4x4 projectionMatrix) {
		loadMatrix4f(location_projectionMatrix, projectionMatrix);
	}
	void ParticleRenderer::ParticleShader::loadTextureRowsCount(int rows) {
		loadInt(location_textureRowsCount, rows);
	}



	ParticleRenderer::ParticleRenderer() :
			quadVao(DataVertexArrayObject::TRIANGLE_STRIP, 2, VERTICES),
			vbo(quadVao, DATA_SIZE_PER_PARTICLE * MAX_PARTICLES) {

		// column 1 of model view matrix
		vbo.addInstancedAttribute(1, 4, DATA_SIZE_PER_PARTICLE * sizeof(GLfloat), 0 * 4 * sizeof(GLfloat));
		// column 2 of model view matrix
		vbo.addInstancedAttribute(2, 4, DATA_SIZE_PER_PARTICLE * sizeof(GLfloat), 1 * 4 * sizeof(GLfloat));
		// column 3 of model view matrix
		vbo.addInstancedAttribute(3, 4, DATA_SIZE_PER_PARTICLE * sizeof(GLfloat), 2 * 4 * sizeof(GLfloat));
		// column 4 of model view matrix
		vbo.addInstancedAttribute(4, 4, DATA_SIZE_PER_PARTICLE * sizeof(GLfloat), 3 * 4 * sizeof(GLfloat));
		// texture offset
		vbo.addInstancedAttribute(5, 4, DATA_SIZE_PER_PARTICLE * sizeof(GLfloat), 4 * 4 * sizeof(GLfloat));
		// texture blend factor
		vbo.addInstancedAttribute(6, 1, DATA_SIZE_PER_PARTICLE * sizeof(GLfloat), 5 * 4 * sizeof(GLfloat));
		// displacement on screen
		vbo.addInstancedAttribute(7, 2, DATA_SIZE_PER_PARTICLE * sizeof(GLfloat), 5 * 4 * sizeof(GLfloat) + 4);

}

	void ParticleRenderer::render(const ParticleList& particles, Viewport& viewport, ProjectionMatrix& projectionMatrixObj) {
		glm::mat4 projectionMatrix = projectionMatrixObj.getProjectionMatrix();
		
		shader.startProgram();
		shader.loadProjectionMatrix(glm::perspective<float>(45, float(egui::getDisplayHandler().getWidth()) / egui::getDisplayHandler().getHeight(), 1, 100));
		glBindVertexArray(quadVao.getVertexArrayObjectId());
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		glActiveTexture(GL_TEXTURE0);
		for(auto& textureAndParticles : particles) {
			ParticleTexture particleTexture = textureAndParticles.first;
			particleTexture.bind();
			glBlendFunc(GL_SRC_ALPHA, (particleTexture.useAdditiveBlending()) ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);

			shader.loadTextureRowsCount(particleTexture.getStages());

			const std::vector<Particle>& particleList = textureAndParticles.second;
			this->updateVboData(particleList, viewport);

			glDrawArraysInstanced(quadVao.getRenderMode(), 0, quadVao.getIndiciesCount(), particleList.size());
		}

		glDisable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);
		glDepthMask(GL_TRUE);
		shader.stopProgram();
	}

	void ParticleRenderer::updateVboData(const std::vector<Particle>& particles, Viewport& viewport) {
		glm::mat4 viewMatrix = viewport.getViewMatrix();

		std::vector<float> vboData;
		vboData.reserve(particles.size() * DATA_SIZE_PER_PARTICLE);

		for(int i = 0; i < particles.size(); i++) {
			if(i >= MAX_PARTICLES) {
				log(LogSeverity::WARNING, "Tried to render more than " + std::to_string(MAX_PARTICLES) + "particles.");
				break;
			}

			auto& p = particles[i];

			glm::mat4 modelMatrix = p.generateModelMatrix(viewMatrix);
			glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

			pushMatrixDataInVector(vboData, modelViewMatrix);

			vboData.push_back(p.getOffsetTex1().x);
			vboData.push_back(p.getOffsetTex1().y);
			vboData.push_back(p.getOffsetTex2().x);
			vboData.push_back(p.getOffsetTex2().y);
			vboData.push_back(p.getTexBlendFactor());
			vboData.push_back(p.getDisplacementOnScreen().x);
			vboData.push_back(p.getDisplacementOnScreen().y);
		}

		this->vbo.updateVbo(vboData);
	}

	void ParticleRenderer::pushMatrixDataInVector(std::vector<float>& data, glm::mat4 matrix) {
		const float* matrixSource = glm::value_ptr(matrix);
		for(int i = 0; i < 16; i++) {
			data.push_back(matrixSource[i]);
		}
	}
}