#define NOMINMAX
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SimpleTexture.h"

#include <algorithm>
#include <nanovg/stb_image.h>
#include <stdexcept>

namespace card {
	SimpleTexture::SimpleTexture(std::string path) {
		unsigned char* img = stbi_load(path.c_str(), &width, &height, 0, STBI_rgb_alpha);
		if(! img) {
			throw std::runtime_error("Couldn't load image!");
		}

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//TODO what does this line do`?
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set anisotropic filtering
		if(glfwExtensionSupported("GL_ARB_texture_filter_anisotropic")) {
			GLfloat max;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max);
			max = std::min(max, ANISOTROPIC_FILTERING_AMOUNT);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, max);
		}
		stbi_image_free(img);
	}

	uint32_t SimpleTexture::getTexId() const {
		return texId;
	}

	int32_t SimpleTexture::getWidth() const {
		return width;
	}

	int32_t SimpleTexture::getHeight() const {
		return height;
	}

	void SimpleTexture::cleanUp() {
		glDeleteTextures(1, &texId);
	}


}

