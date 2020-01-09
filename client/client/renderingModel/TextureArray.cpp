#define NOMINMAX
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TextureArray.h"
#include <stdexcept>
#include <algorithm>
#include <nanovg/stb_image.h>

namespace card {
	TextureArray::TextureArray(uint32_t texId, int32_t width, int32_t height) :
			texId(texId),
			width(width),
			height(height) {

	}

	uint32_t TextureArray::getTexId() const {
		return texId;
	}

	void TextureArray::bind() const {
		glBindTexture(GL_TEXTURE_2D_ARRAY, texId);
	}

	int32_t TextureArray::getWidth() const {
		return width;
	}

	int32_t TextureArray::getHeight() const {
		return height;
	}

	void TextureArray::cleanUp() {
		glDeleteTextures(1, &texId);
	}

}
