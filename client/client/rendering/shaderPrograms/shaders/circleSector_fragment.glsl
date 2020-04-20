const char* circleSector_fragment = R"(
#version 330 core

out vec4 out_Color;
uniform vec4 color;

void main(void) {
	out_Color = color;
}
)";