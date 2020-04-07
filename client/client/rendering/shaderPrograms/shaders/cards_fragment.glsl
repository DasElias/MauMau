const char* cards_fragment = R"(
#version 400 core

in vec3 pass_texCoords;
flat in uint pass_shouldRenderInGrayScale;

out vec4 out_Color;

uniform sampler2DArray tex;

void main(void) {
	vec4 texColor = texture(tex, pass_texCoords);
	if(texColor.a < 0.5) discard;

	if(bool(pass_shouldRenderInGrayScale)) {
		out_Color = mix(texColor, vec4(vec3(0.925f), 1.0f), 0.45f);
	} else {
		out_Color = texColor;
	}

}
)";