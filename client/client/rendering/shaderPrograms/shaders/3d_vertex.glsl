const char* threed_vertex = R"(
#version 400 core

const float DENSITY = 0.4;

in vec3 position;
in vec2 texCoords;
in vec3 normal;

out vec2 pass_texCoords;
out vec3 pass_position;
out vec3 pass_normal;

uniform vec3 cameraPosition;
uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;

void main(void) {
	pass_texCoords = texCoords;
	pass_position = position;
	pass_normal = normal;

	gl_Position = projectionViewMatrix * modelMatrix * vec4(position, 1.0);
}

)";