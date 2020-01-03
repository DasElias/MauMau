#include "DataTextureVertexArrayObject.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>


namespace card {
	DataTextureVertexArrayObject::DataTextureVertexArrayObject(RenderMode mode, int dimensions, const std::vector<float>& data, const std::vector<float>& textures) :
			VertexArrayObject(mode) {
		this->dimensions = dimensions;
		
		glGenVertexArrays(1, &vertexArrayObjectId);
		glBindVertexArray(vertexArrayObjectId);
		indiciesCount = data.size() / dimensions;

		this->dataVboId = genFloatVbo(0, dimensions, data);
		this->textureVboId = genFloatVbo(1, 2, textures);

		glBindVertexArray(0);
	}
	int DataTextureVertexArrayObject::getDimensions() const {
		return dimensions;
	}
	void DataTextureVertexArrayObject::updateData(const std::vector<float>& data) {
		this->indiciesCount = data.size() / dimensions;
		updateFloatVbo(dataVboId, data);
	}
	void DataTextureVertexArrayObject::updateTexture(const std::vector<float>& textures) {
		updateFloatVbo(textureVboId, textures);
	}
}