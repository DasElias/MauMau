#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GaussianBlurPostProcessor.h"

#include <egui/input/IOHandler.h>

#include "shaderPrograms/shaders/gaussianBlur_vertical_vertex.glsl"
#include "shaderPrograms/shaders/gaussianBlur_horizontal_vertex.glsl"
#include "shaderPrograms/shaders/gaussianBlur_fragment.glsl"
#include "shaderPrograms/shaders/defaultPostProcessing_vertex.glsl"
#include "shaderPrograms/shaders/defaultPostProcessing_fragment.glsl"


namespace card {
	GaussianBlurPostProcessor::HorizontalBlurShader::HorizontalBlurShader() :
		VertexFragmentShaderProgram(
			gaussianBlur_horizontal_vertex,
			gaussianBlur_fragment,
			[this]() {
				this->createShaderVariable(0, "position");
			}) {

		location_fboWidth = this->createUniformLocation("fboWidth");

	}

	void GaussianBlurPostProcessor::HorizontalBlurShader::setFboWidth(float width) {
		loadFloat(location_fboWidth, width);
	}

	GaussianBlurPostProcessor::VerticalBlurShader::VerticalBlurShader() :
		VertexFragmentShaderProgram(
			gaussianBlur_vertical_vertex,
			gaussianBlur_fragment,
			[this]() {
				this->createShaderVariable(0, "position");
			}) {

		location_fboHeight = this->createUniformLocation("fboHeight");

	}

	void GaussianBlurPostProcessor::VerticalBlurShader::setFboHeight(float height) {
		loadFloat(location_fboHeight, height);
	}

	GaussianBlurPostProcessor::FinalRenderingShader::FinalRenderingShader() :
		VertexFragmentShaderProgram(
			defaultPostProcessing_vertex,
			defaultPostProcessing_fragment,
			[this]() {
				this->createShaderVariable(0, "position");
			}) {
	}

	GaussianBlurPostProcessor::GaussianBlurPostProcessor() :
			horizontalFbo({new FrameBufferObject(egui::getDisplayHandler().getWidth() / 4, egui::getDisplayHandler().getHeight() / 4)}),
			verticalFbo({new FrameBufferObject(egui::getDisplayHandler().getWidth() / 4, egui::getDisplayHandler().getHeight() / 4)}),
			horizontalFbo2({new FrameBufferObject(egui::getDisplayHandler().getWidth() / 8, egui::getDisplayHandler().getHeight() / 8)}),
			verticalFbo2({new FrameBufferObject(egui::getDisplayHandler().getWidth() / 8, egui::getDisplayHandler().getHeight() / 8)}) {

		horizontalShader.startProgram();
		horizontalShader.setFboWidth(float(horizontalFbo->getWidth()));
		horizontalShader.stopProgram();

		verticalShader.startProgram();
		verticalShader.setFboHeight(float(verticalFbo->getHeight()));
		verticalShader.stopProgram();
	}

	GaussianBlurPostProcessor::~GaussianBlurPostProcessor() {
	}

	void GaussianBlurPostProcessor::renderHorizontalBlur(FrameBufferObject& target, uint32_t textureId) {
		target.bind();
		target.clear();
		horizontalShader.startProgram();
		horizontalShader.setFboWidth(float(target.getWidth()));

		PostProcessor::render(textureId, false);

		horizontalShader.stopProgram();
		target.unbind();
	}

	void GaussianBlurPostProcessor::renderVerticalBlur(FrameBufferObject& target, uint32_t textureId) {
		target.bind();
		target.clear();
		verticalShader.startProgram();
		verticalShader.setFboHeight(float(target.getHeight()));

		PostProcessor::render(textureId, false);

		verticalShader.stopProgram();
		target.unbind();
	}

	void GaussianBlurPostProcessor::render(uint32_t textureId, bool updateFbos) {
		if(updateFbos) {
			horizontalFbo.reset(new FrameBufferObject(egui::getDisplayHandler().getWidth() / 4, egui::getDisplayHandler().getHeight() / 4));
			verticalFbo.reset(new FrameBufferObject(egui::getDisplayHandler().getWidth() / 4, egui::getDisplayHandler().getHeight() / 4));
			horizontalFbo2.reset(new FrameBufferObject(egui::getDisplayHandler().getWidth() / 8, egui::getDisplayHandler().getHeight() / 8));
			verticalFbo2.reset(new FrameBufferObject(egui::getDisplayHandler().getWidth() / 8, egui::getDisplayHandler().getHeight() / 8));
		}

		glActiveTexture(GL_TEXTURE0);

		renderHorizontalBlur(* horizontalFbo, textureId);
		renderVerticalBlur(* verticalFbo, horizontalFbo->getColorTexture());
		renderHorizontalBlur(* horizontalFbo2, verticalFbo->getColorTexture());
		renderVerticalBlur(* verticalFbo2, horizontalFbo2->getColorTexture());

		finalRenderingShader.startProgram();
		PostProcessor::render(verticalFbo2->getColorTexture(), false);
		finalRenderingShader.stopProgram();

	}

}

