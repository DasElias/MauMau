#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "SamplerObjectFactory.h"
#include "TexturePropertiesConverter.h"
#include <algorithm>

namespace card {
	SamplerObjectFactory::SamplerObjectFactory() :
		minFilter(TextureMinFilter::NEAREST_MIPMAP_LINEAR),
		magFilter(TextureMagFilter::LINEAR),
		wrapS(TextureWrap::REPEAT),
		wrapT(TextureWrap::REPEAT),
		anisotropicFiltering(1) {
	}
	SamplerObjectFactory& SamplerObjectFactory::setMinFilter(TextureMinFilter minFilter) {
		this->minFilter = minFilter;
		return *this;
	}
	SamplerObjectFactory& SamplerObjectFactory::setMagFilter(TextureMagFilter magFilter) {
		this->magFilter = magFilter;
		return *this;
	}
	SamplerObjectFactory& SamplerObjectFactory::setWrapS(TextureWrap wrap) {
		this->wrapS = wrap;
		return *this;
	}
	SamplerObjectFactory& SamplerObjectFactory::setWrapT(TextureWrap wrap) {
		this->wrapT = wrap;
		return *this;
	}
	SamplerObjectFactory& SamplerObjectFactory::setAnisotropicFiltering(float a) {
		this->anisotropicFiltering = a;
		return *this;
	}
	SamplerObject SamplerObjectFactory::generateSampler() {
		uint32_t samplerId;
		glGenSamplers(1, &samplerId);

		glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, convertWrapValue(wrapS));
		glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, convertWrapValue(wrapT));
		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, convertMinFilter(minFilter));
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, convertMagFilter(magFilter));

		// set anisotropic filtering
		if(glfwExtensionSupported("GL_ARB_texture_filter_anisotropic")) {
			GLfloat max;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max);
			max = std::min(max, anisotropicFiltering);
			glSamplerParameterf(samplerId, GL_TEXTURE_MAX_ANISOTROPY, max);
		}

		return SamplerObject(samplerId);
	}

}