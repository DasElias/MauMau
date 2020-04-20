const char* threed_color_vertex = R"(
#version 330 core

in vec3 position;
in vec3 color;

out vec3 pass_color;

uniform mat4 projectionViewMatrix;

void main(void) {
	pass_color = color;
	
	gl_Position = projectionViewMatrix * vec4(position, 1.0);
}

)";