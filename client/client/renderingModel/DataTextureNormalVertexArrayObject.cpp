#include "DataTextureNormalVertexArrayObject.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include "DataTextureNormalVertexArrayObject.h"


namespace card {
	DataTextureNormalVertexArrayObject::DataTextureNormalVertexArrayObject(RenderMode mode, int dimensions, const std::vector<float>& data, const std::vector<float>& textures, const std::vector<float>& normals) :
			VertexArrayObject(mode) {
		this->dimensions = dimensions;
		
		glGenVertexArrays(1, &vertexArrayObjectId);
		glBindVertexArray(vertexArrayObjectId);
		indiciesCount = data.size() / dimensions;

		this->dataVboId = genFloatVbo(0, dimensions, data);
		this->textureVboId = genFloatVbo(1, 2, textures);
		this->normalVboId = genFloatVbo(2, dimensions, normals);

		glBindVertexArray(0);
	}
	int DataTextureNormalVertexArrayObject::getDimensions() const {
		return dimensions;
	}
	void DataTextureNormalVertexArrayObject::updateData(const std::vector<float>& data) {
		this->indiciesCount = data.size() / dimensions;
		updateFloatVbo(dataVboId, data);
	}
	void DataTextureNormalVertexArrayObject::updateTexture(const std::vector<float>& textures) {
		updateFloatVbo(textureVboId, textures);
	}
	void DataTextureNormalVertexArrayObject::updateNormals(const std::vector<float>& normals) {
		updateFloatVbo(normalVboId, normals);
	}
}