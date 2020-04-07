const char* cards_vertex = R"(
#version 400 core

in vec3 position;
in vec2 texCoords;
in mat4 modelViewProjectionMatrix;
in float zIndex;
in uint textureId;

out vec3 pass_texCoords;
out float pass_zIndex;
flat out uint pass_shouldRenderInGrayScale;

void main(void) {
	uint GREY_SCALE_BIT_MASK = 1u << 31;

	pass_texCoords = vec3(texCoords, textureId & ~GREY_SCALE_BIT_MASK);
	pass_shouldRenderInGrayScale = textureId & GREY_SCALE_BIT_MASK;
	pass_zIndex = zIndex;

	gl_Position = modelViewProjectionMatrix * vec4(position.xyz, 1.0);
	
}

)";