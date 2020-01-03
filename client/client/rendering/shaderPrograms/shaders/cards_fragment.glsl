const char* cards_fragment = R"(
#version 400 core

in vec3 pass_texCoords;

out vec4 out_Color;

uniform sampler2DArray tex;

void main(void) {
	out_Color = texture(tex, pass_texCoords);
	if(out_Color.a < 0.5) discard;

}
)";