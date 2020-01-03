#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "PostProcessor.h"

namespace card {
	// ----------------------------------------------------------------------
	// ----------------------------STATIC-FIELDS-----------------------------
	// ----------------------------------------------------------------------
	std::vector<float> const PostProcessor::POSITIONS = {{-1, 1, -1, -1, 1, 1, 1, -1}};
	PostProcessor::PostProcessor() :
			quad(VertexArrayObject::TRIANGLE_STRIP, 2, POSITIONS) {
	}

	void PostProcessor::render(uint32_t textureId, bool updateFbos) {
		glBindTexture(GL_TEXTURE_2D, textureId);
		glBindVertexArray(quad.getVertexArrayObjectId());
		glDrawArrays(quad.getRenderMode(), 0, 4);
	}

}

