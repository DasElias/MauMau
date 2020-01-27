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
	float const CardSceneBackgroundRenderer::TABLE_MAX_X = 2;
	float const Y_TOP = 0;
	float const Y_BOTTOM = -0.1f;
	float const Y_BOTTOMEST = -1;
	float const CardSceneBackgroundRenderer::TABLE_MAX_Z = 1;

	float const TABLE_BASE_OUTER_X = 1.5f;
	float const TABLE_BASE_WIDTH = 0.3f;
	float const TABLE_BASE_Z_DISTANCE = 0.1f;
	float const TABLE_BASE_DEPTH = 0.2f;

	std::vector<float> const CardSceneBackgroundRenderer::TABLE_VERTICES = {
		// top
		TABLE_MAX_X, Y_TOP, TABLE_MAX_Z,		
		TABLE_MAX_X, Y_TOP, -TABLE_MAX_Z,
		-TABLE_MAX_X, Y_TOP, -TABLE_MAX_Z,
		-TABLE_MAX_X, Y_TOP, -TABLE_MAX_Z,
		-TABLE_MAX_X, Y_TOP, TABLE_MAX_Z,
		TABLE_MAX_X, Y_TOP, TABLE_MAX_Z,

		// front
		-TABLE_MAX_X, Y_TOP, TABLE_MAX_Z,
		-TABLE_MAX_X, Y_BOTTOM, TABLE_MAX_Z,
		TABLE_MAX_X, Y_BOTTOM, TABLE_MAX_Z,
		-TABLE_MAX_X, Y_TOP, TABLE_MAX_Z,
		TABLE_MAX_X, Y_BOTTOM, TABLE_MAX_Z,
		TABLE_MAX_X, Y_TOP, TABLE_MAX_Z,

		// table base right (front)
		TABLE_BASE_OUTER_X, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		TABLE_BASE_OUTER_X - TABLE_BASE_WIDTH, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		TABLE_BASE_OUTER_X, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		TABLE_BASE_OUTER_X, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		TABLE_BASE_OUTER_X - TABLE_BASE_WIDTH, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		TABLE_BASE_OUTER_X - TABLE_BASE_WIDTH, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,

		// table base right (side)
		TABLE_BASE_OUTER_X - TABLE_BASE_WIDTH, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE - TABLE_BASE_DEPTH,
		TABLE_BASE_OUTER_X - TABLE_BASE_WIDTH, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE - TABLE_BASE_DEPTH,
		TABLE_BASE_OUTER_X - TABLE_BASE_WIDTH, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		TABLE_BASE_OUTER_X - TABLE_BASE_WIDTH, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE - TABLE_BASE_DEPTH,
		TABLE_BASE_OUTER_X - TABLE_BASE_WIDTH, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		TABLE_BASE_OUTER_X - TABLE_BASE_WIDTH, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,

		// table base left (front)
		-TABLE_BASE_OUTER_X, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		-TABLE_BASE_OUTER_X, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		-TABLE_BASE_OUTER_X + TABLE_BASE_WIDTH, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		-TABLE_BASE_OUTER_X, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		-TABLE_BASE_OUTER_X + TABLE_BASE_WIDTH, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		-TABLE_BASE_OUTER_X + TABLE_BASE_WIDTH, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,

		// table base left (side)
		-TABLE_BASE_OUTER_X + TABLE_BASE_WIDTH, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE - TABLE_BASE_DEPTH,
		-TABLE_BASE_OUTER_X + TABLE_BASE_WIDTH, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		-TABLE_BASE_OUTER_X + TABLE_BASE_WIDTH, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE - TABLE_BASE_DEPTH,
		-TABLE_BASE_OUTER_X + TABLE_BASE_WIDTH, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE - TABLE_BASE_DEPTH,
		-TABLE_BASE_OUTER_X + TABLE_BASE_WIDTH, Y_BOTTOM, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
		-TABLE_BASE_OUTER_X + TABLE_BASE_WIDTH, Y_BOTTOMEST, TABLE_MAX_Z - TABLE_BASE_Z_DISTANCE,
	};


	std::vector<float> const CardSceneBackgroundRenderer::TABLE_TEXTURE_COORDS = {
		1, 1,
		1, 0,
		0, 0,
		0, 0,
		0, 1,
		1, 1,

		0, 1,
		0, 0,
		1, 0,
		0, 1,
		1, 0,
		1, 1,

		0, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0,

		0, 1,
		0, 0,
		1, 0,
		0, 1,
		1, 0,
		1, 1,

		0, 1,
		0, 0,
		1, 0,
		0, 1,
		1, 0,
		1, 1,

		0, 1,
		0, 0,
		1, 0,
		0, 1,
		1, 1,
		1, 0,
	};



	CardSceneBackgroundRenderer::CardSceneBackgroundRenderer(Renderer2D& rendererImpl2d, Renderer3D& renderImpl3d) :
			backgroundElement(std::make_shared<egui::UnorganizedParentElement>()),
			renderImpl2d(rendererImpl2d),
			renderImpl3d(renderImpl3d),
			tableVao(VertexArrayObject::RenderMode::TRIANGLES, 3, TABLE_VERTICES, TABLE_TEXTURE_COORDS),
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
		renderImpl3d.render(tableVao, projectionMatrix, viewport);

	}
}