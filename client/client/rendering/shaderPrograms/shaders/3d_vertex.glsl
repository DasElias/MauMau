const char* threed_vertex = R"(
#version 400 core

in vec3 position;
in vec2 texCoords;

out vec2 pass_texCoords;

uniform mat4 projectionViewMatrix;

void main(void) {
	pass_texCoords = texCoords;
	
	gl_Position = projectionViewMatrix * vec4(position, 1.0);
	//gl_Position = vec4(position, 1.0);
}

)";