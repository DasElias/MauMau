#include "CardSceneBackgroundRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "../utils/FileUtils.h"
#include "../renderingModel/SimpleTextureFactory.h"

namespace card {
	float const CardSceneBackgroundRenderer::TABLE_MAX_X = 3;
	float const Y_TOP = 0;
	float const CardSceneBackgroundRenderer::TABLE_POS_MAX_Z = 2;
	float const CardSceneBackgroundRenderer::TABLE_NEG_MAX_Z = -2.5;

	std::vector<float> const CardSceneBackgroundRenderer::TABLE_VERTICES = {
		// top
		TABLE_MAX_X, Y_TOP, TABLE_POS_MAX_Z,
		TABLE_MAX_X, Y_TOP, TABLE_NEG_MAX_Z,
		-TABLE_MAX_X, Y_TOP, TABLE_NEG_MAX_Z,
		-TABLE_MAX_X, Y_TOP, TABLE_NEG_MAX_Z,
		-TABLE_MAX_X, Y_TOP, TABLE_POS_MAX_Z,
		TABLE_MAX_X, Y_TOP, TABLE_POS_MAX_Z,

	
	};

	std::vector<float> const CardSceneBackgroundRenderer::TABLE_NORMALS = {
		// top
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,

		
	};

	std::vector<float> const CardSceneBackgroundRenderer::TABLE_TEXTURE_COORDS = {
		1, 1,
		1, 0,
		0, 0,
		0, 0,
		0, 1,
		1, 1,
	};



	CardSceneBackgroundRenderer::CardSceneBackgroundRenderer(Renderer2D& rendererImpl2d, Renderer3D& renderImpl3d) :
			backgroundElement(std::make_shared<egui::UnorganizedParentElement>()),
			renderImpl2d(rendererImpl2d),
			renderImpl3d(renderImpl3d),
			tableVao(VertexArrayObject::RenderMode::TRIANGLES, 3, TABLE_VERTICES, TABLE_TEXTURE_COORDS, TABLE_NORMALS),
			backgroundTexture(SimpleTextureFactory().loadFromFile(getApplicationFolder() + "\\resources\\ingamebackground.png")),
			tableTopTexture(SimpleTextureFactory()
				.setMagFilter(TextureMagFilter::LINEAR)
				.setMinFilter(TextureMinFilter::NEAREST_MIPMAP_LINEAR)
				.setWrapS(TextureWrap::CLAMP_TO_EDGE)
				.setWrapT(TextureWrap::CLAMP_TO_EDGE)
				.setAnisotropicFiltering(2)
				.loadFromFile(getApplicationFolder() + "\\resources\\tabletop.png")
			) {
	}
	void CardSceneBackgroundRenderer::render(ProjectionMatrix& projectionMatrix, Viewport& viewport) {
		glActiveTexture(GL_TEXTURE0);

	/*	glDepthMask(GL_FALSE);
		glBindTexture(GL_TEXTURE_2D, backgroundTexture.getTexId());
		renderImpl2d.render(backgroundElement, false);
		glDepthMask(GL_TRUE);*/

		glBindTexture(GL_TEXTURE_2D, tableTopTexture.getTexId());
		tableRenderer.render(tableVao, projectionMatrix, viewport);

	}
}