#include "DataVertexArrayObject.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>


namespace card {
	DataVertexArrayObject::DataVertexArrayObject(RenderMode renderMode, int dimensions, const std::vector<float>& data) : 
			VertexArrayObject(renderMode) {
		this->dimensions = dimensions;
		
		glGenVertexArrays(1, &vertexArrayObjectId);
		glBindVertexArray(vertexArrayObjectId);
		indiciesCount = data.size() / dimensions;

		this->dataVboId = genFloatVbo(0, dimensions, data);

		glBindVertexArray(0);
	}
	int DataVertexArrayObject::getDimensions() const {
		return dimensions;
	}
	void DataVertexArrayObject::updateData(const std::vector<float>& data) {
		this->indiciesCount = data.size() / dimensions;
		updateFloatVbo(dataVboId, data);
	}
}