#include "FrameBufferObject.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <egui/input/IOHandler.h>


namespace card {
	FrameBufferObject::FrameBufferObject(int width, int height, int samples) :
			width(width),
			height(height),
			multisamples(samples) {

		// generate framebuffer
		glGenFramebuffers(1, &frameBufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

		setupColorTexture();
		setupDepthBuffer();

		// unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	FrameBufferObject::~FrameBufferObject() {
		glDeleteFramebuffers(1, &frameBufferId);
		glDeleteTextures(1, &colorTextureId);
		glDeleteRenderbuffers(1, &depthRenderBufferId);
	}

	void FrameBufferObject::setupColorTexture() {
		glGenTextures(1, &colorTextureId);

		if(multisamples != 0) {
			//TODO shouldn't target be GL_TEXTURE_2D_MULTISAMPLE?
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorTextureId);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisamples, GL_RGBA8, width, height, true);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorTextureId, 0);
		} else {
			glBindTexture(GL_TEXTURE_2D, colorTextureId);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_INT, nullptr);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureId, 0);
		}
	}

	void FrameBufferObject::setupDepthBuffer() {
		glGenRenderbuffers(1, &depthRenderBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferId);

		if(multisamples != 0) {
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisamples, GL_DEPTH_COMPONENT24, width, height);
		} else {
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		}

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferId);
	}

	void FrameBufferObject::bind() const {
		glViewport(0, 0, width, height);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	}

	void FrameBufferObject::unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, egui::getDisplayHandler().getWidth(), egui::getDisplayHandler().getHeight());
	}

	void FrameBufferObject::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void FrameBufferObject::clear(glm::vec4 color) {
		clear();
		glClearColor(color.x, color.y, color.z, color.a);
	}

	int FrameBufferObject::getColorTexture() const {
		return colorTextureId;
	}

	int FrameBufferObject::getWidth() const {
		return width;
	}

	int FrameBufferObject::getHeight() const {
		return height;
	}

	void FrameBufferObject::resolveToFbo(FrameBufferObject& outputFbo) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, outputFbo.frameBufferId);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->frameBufferId);

		glBlitFramebuffer(0, 0, width, height, 0, 0, outputFbo.width, outputFbo.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		unbind();
	}

	void FrameBufferObject::resolveToScreen() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->frameBufferId);
		glDrawBuffer(GL_BACK);

		glBlitFramebuffer(0, 0, width, height, 0, 0, egui::getDisplayHandler().getWidth(), egui::getDisplayHandler().getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
		unbind();
	}

}
