#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "SamplerObject.h"

namespace card {
	SamplerObject::SamplerObject(uint32_t samplerId) :
			samplerId(samplerId) {
	}
	void SamplerObject::bind() const {
		glBindSampler(0, samplerId);
	}
	void SamplerObject::unbind() const {
		glBindSampler(0, 0);
	}
	void SamplerObject::cleanUp() {
		glDeleteSamplers(1, &samplerId);
	}
}