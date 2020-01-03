#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer2D.h"

#include "shaderPrograms/shaders/2d_vertex.glsl"
#include "shaderPrograms/shaders/2d_fragment.glsl"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


namespace card {
	std::vector<float> const Renderer2D::POSITIONS = { {-1, 1, -1, -1, 1, 1, 1, -1} };
	std::vector<float> const Renderer2D::TEXTURE_COORDS = {{0, 0, 0, 1, 1, 0, 1, 1}};

	Renderer2D::Renderer2DShader::Renderer2DShader(bool discardTransparentFragments) :
			VertexFragmentShaderProgram(
				twod_vertex,
				(discardTransparentFragments) ? twod_fragment_discardInvisible : twod_fragment,
				[this]() {
					this->createShaderVariable(0, "position");
					this->createShaderVariable(1, "texCoords");
			}) {
		
		this->location_transformationMatrix = createUniformLocation("transformationMatrix");
	}

	void Renderer2D::Renderer2DShader::loadTransformationMatrix(glm::mat4x4 transformationMatrix) {

		loadMatrix4f(location_transformationMatrix, transformationMatrix);
	}

	Renderer2D::Renderer2DVertexArrayObject::Renderer2DVertexArrayObject(const std::vector<float>& data) :
			VertexArrayObject(VertexArrayObject::TRIANGLE_STRIP) {

		glGenVertexArrays(1, &vertexArrayObjectId);
		glBindVertexArray(vertexArrayObjectId);

		glGenBuffers(1, &vertexVbo);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), &data[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

		glGenBuffers(1, &textureVbo);
		glBindBuffer(GL_ARRAY_BUFFER, textureVbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);

		this->indiciesCount = data.size() / 2;
	}

	void Renderer2D::Renderer2DVertexArrayObject::loadTexData(const std::array<float, 8> & texData) {
		glBindBuffer(GL_ARRAY_BUFFER, textureVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, texData.size() * sizeof(texData[0]), &texData[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	Renderer2D::Renderer2D(bool discardInvisibleFragments) :
			shader(discardInvisibleFragments),
			quad(POSITIONS) {
	}

	glm::mat4x4 Renderer2D::computeTransformationMatrix(const std::shared_ptr<egui::PositionableElement>& elem) const {
		glm::mat4x4 mat(1);
		float absXMargin = elem->getAbsXMargin() + elem->getComputedWidth() / 2;
		float absYMargin = elem->getAbsYMargin() + elem->getComputedHeight() / 2;
		mat = glm::translate(mat, {(absXMargin * 2) - 1, ((absYMargin * 2) - 1) * -1, 0 });

		mat = glm::scale(mat, { elem->getComputedWidth(), elem->getComputedHeight(), 0 });

		

		return mat;
	}

	void Renderer2D::render(std::shared_ptr<egui::PositionableElement> elem, bool hasTransparency, glm::vec2 topLeftTexture, glm::vec2 topRightTexture, glm::vec2 bottomRightTexture, glm::vec2 bottomLeftTexture) {
		if(hasTransparency) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		
		shader.startProgram();

		shader.loadTransformationMatrix(computeTransformationMatrix(elem));
		glBindVertexArray(quad.getVertexArrayObjectId());

		std::array<float, 8> tex = {
			topLeftTexture.x, topLeftTexture.y, bottomLeftTexture.x, bottomLeftTexture.y, topRightTexture.x, topRightTexture.y, bottomRightTexture.x, bottomRightTexture.y
		};
		quad.loadTexData(tex);

		glDrawArrays(quad.getRenderMode(), 0, quad.getIndiciesCount());
		shader.stopProgram();
		
		if(hasTransparency) {
			glDisable(GL_BLEND);
		}
	}
	
}
