const char* twod_vertex = R"(
#version 330 core

in vec2 position;
in vec2 texCoords;

out vec2 pass_texCoords;

uniform mat4 transformationMatrix;

void main(void) {
	pass_texCoords = texCoords;
	
	gl_Position = transformationMatrix * vec4(position, 0.0, 1.0);
}

)";