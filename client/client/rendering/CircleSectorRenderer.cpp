#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CircleSectorRenderer.h"
#include <vector>
#include "shaderPrograms/shaders/circleSector_vertex.glsl"
#include "shaderPrograms/shaders/circleSector_fragment.glsl"
#include <egui/input/IOHandler.h>

namespace card {
	CircleSectorRenderer::CircleSectorShader::CircleSectorShader() :
			VertexFragmentShaderProgram(
				circleSector_vertex, 
				circleSector_fragment,
				[this]() {
					createShaderVariable(0, "position");
				}) {

		this->location_color = createUniformLocation("color");
	}
	void CircleSectorRenderer::CircleSectorShader::loadColor(glm::vec4 color) {
		loadVector4f(location_color, color);
	}

	static std::vector<float> const INITIAL_DATA = {0};
	CircleSectorRenderer::CircleSectorRenderer() :
			vao(VertexArrayObject::TRIANGLE_FAN, 2, INITIAL_DATA) {
	}

	void CircleSectorRenderer::renderSector_xDiameter(glm::vec2 center, float xDiameter, float angleStart, float angleEnd, int segments, glm::vec4 color) {
		float yDiameter = (xDiameter / egui::getDisplayHandler().getHeight()) * egui::getDisplayHandler().getWidth();
		center.x = (center.x * 2) - 1;
		center.y = ((center.y * 2) - 1) * (-1);	// TODO

		updateVao(center, xDiameter, yDiameter, angleStart, angleEnd, segments);
		renderImpl(color);
	}

	void CircleSectorRenderer::updateVao(glm::vec2 center, float xDiameter, float yDiameter, float angleStart, float angleEnd, int segments) {
		float const angleStep = (angleEnd - angleStart) / segments;

		std::vector<float> vertices;
		vertices.resize(2 * (segments + 2));
		vertices[0] = center.x;
		vertices[1] = center.y;

		for(int i = 0; i <= segments; i++) {
			vertices[2 + 2 * i] = center.x + xDiameter * cos(angleStart + i * angleStep);
			vertices[2 + 2 * i + 1] = center.y + yDiameter * sin(angleStart + i * angleStep);
		}

		vao.updateData(vertices);
	}

	void CircleSectorRenderer::renderImpl(glm::vec4 color) {
		shader.startProgram();
		glBindVertexArray(vao.getVertexArrayObjectId());
		shader.loadColor(color);
		glDrawArrays(vao.getRenderMode(), 0, vao.getIndiciesCount());
		glBindVertexArray(0);
		shader.stopProgram();
	}

	

}