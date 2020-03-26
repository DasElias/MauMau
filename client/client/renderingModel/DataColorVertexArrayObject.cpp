#include "DataColorVertexArrayObject.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>


namespace card {
	DataColorVertexArrayObject::DataColorVertexArrayObject(RenderMode mode, int dimensions, const std::vector<float>& data, const std::vector<float>& colorData) :
			VertexArrayObject(mode) {
		this->dimensions = dimensions;
		
		glGenVertexArrays(1, &vertexArrayObjectId);
		glBindVertexArray(vertexArrayObjectId);
		indiciesCount = data.size() / dimensions;

		this->dataVboId = genFloatVbo(0, dimensions, data);
		this->colorVboId = genFloatVbo(1, 3, colorData);

		glBindVertexArray(0);
	}
	int DataColorVertexArrayObject::getDimensions() const {
		return dimensions;
	}
	void DataColorVertexArrayObject::updateData(const std::vector<float>& data) {
		this->indiciesCount = data.size() / dimensions;
		updateFloatVbo(dataVboId, data);
	}
	void DataColorVertexArrayObject::updateColor(const std::vector<float>& colorData) {
		updateFloatVbo(colorVboId, colorData);
	}
}