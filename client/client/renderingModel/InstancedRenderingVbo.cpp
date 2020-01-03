#include "InstancedRenderingVbo.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "BufferOffsetMacro.h"


namespace card {
	InstancedRenderingVbo::InstancedRenderingVbo(VertexArrayObject targetVao, int sizeInBytes) :
			targetVao(targetVao) {

		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeInBytes, nullptr, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	InstancedRenderingVbo::~InstancedRenderingVbo() {
		//TODO delete vbo
	}

	void InstancedRenderingVbo::addInstancedAttribute(std::int32_t dataSize, std::int32_t instancedDataLengthInBytes, std::int32_t offsetInBytes) {
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBindVertexArray(targetVao.getVertexArrayObjectId());

		GLuint indexInVao = maxVboAttributeIndex++;
		glEnableVertexAttribArray(indexInVao);
		glVertexAttribPointer(indexInVao, dataSize, GL_FLOAT, false, instancedDataLengthInBytes, BUFFER_OFFSET(offsetInBytes));

		// indicate that it's a per-instance-vbo
		glVertexAttribDivisor(indexInVao, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void InstancedRenderingVbo::updateVbo(std::vector<float>& data) {
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), &data[0], GL_STATIC_DRAW);
		//TODO check if it is acceptable to remove glBufferSubData here
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
