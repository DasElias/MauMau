#include "VertexArrayObject.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "BufferOffsetMacro.h"

namespace card {
	VertexArrayObject::VertexArrayObject(RenderMode renderMode) :
			renderMode(renderMode) {
	}
	void VertexArrayObject::cleanUp() {
		Cleanable::cleanUp();

		glDeleteVertexArrays(1, &vertexArrayObjectId);
	}
	std::uint32_t VertexArrayObject::getVertexArrayObjectId() const {
		return vertexArrayObjectId;
	}
	std::size_t VertexArrayObject::getIndiciesCount() const {
		return this->indiciesCount;
	}
	VertexArrayObject::RenderMode VertexArrayObject::getRenderMode() const {
		return this->renderMode;
	}
	std::uint32_t VertexArrayObject::genFloatVbo(int indexInVao, int sizePerVertex, const std::vector<float>& data) {
		std::uint32_t vboId;
		glGenBuffers(1, &vboId);

		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), &data[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(indexInVao);
		glVertexAttribPointer(indexInVao, sizePerVertex, GL_FLOAT, false, 0, BUFFER_OFFSET(0));

		return vboId;
	}
	void VertexArrayObject::updateFloatVbo(std::uint32_t vboId, const std::vector<float>& data) {
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), &data[0], GL_STATIC_DRAW);
		//TODO check if it is acceptable to remove glBufferSubData here
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}