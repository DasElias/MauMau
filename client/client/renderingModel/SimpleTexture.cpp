#define NOMINMAX
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SimpleTexture.h"

#include <algorithm>
#include <stdexcept>

namespace card {
	SimpleTexture::SimpleTexture(uint32_t texId, int32_t width, int32_t height) :
			texId(texId),
			width(width),
			height(height) {
		
	}

	uint32_t SimpleTexture::getTexId() const {
		return texId;
	}

	void SimpleTexture::bind() const {
		glBindTexture(GL_TEXTURE_2D, texId);
	}

	int32_t SimpleTexture::getWidth() const {
		return width;
	}

	int32_t SimpleTexture::getHeight() const {
		return height;
	}

	float SimpleTexture::getAspectRatio() const {
		return float(width) / height;
	}

	void SimpleTexture::cleanUp() {
		glDeleteTextures(1, &texId);
	}


}

