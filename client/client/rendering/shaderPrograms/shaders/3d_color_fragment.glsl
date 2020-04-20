const char* threed_color_fragment = R"(
#version 330 core

in vec3 pass_color;

out vec4 out_Color;

uniform sampler2D tex;

void main(void) {
	out_Color = vec4(pass_color, 1.0f);
}
)";
