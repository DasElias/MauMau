const char* cards_vertex = R"(
#version 400 core

in vec3 position;
in vec2 texCoords;
in mat4 modelViewProjectionMatrix;
in uint textureId;

out vec2 pass_texCoords;
flat out uint pass_shouldRenderInGrayScale;

float getTextureCoordAddition_x(uint textureId) {
	const uint TEXTURES_PER_ROW = 13;
	const float IMAGE_WIDTH = 2048.0f;
	const float COORD_ADDITION_PER_TEXTURE = 1.0f / TEXTURES_PER_ROW;

	uint textureX = (textureId - 1) % TEXTURES_PER_ROW;
	return textureX * COORD_ADDITION_PER_TEXTURE;
}

float getTextureCoordAddition_y(uint textureId) {
	const uint TEXTURES_PER_ROW = 13;
	const uint TEXTURES_PER_COLUMN = 5;
	const float USABLE_SPACE_IN_PERCENT_Y = 1142.0f / 2048;
	const float COORD_ADDITION_PER_TEXTURE = USABLE_SPACE_IN_PERCENT_Y / TEXTURES_PER_COLUMN;

	uint textureY = (textureId - 1) / TEXTURES_PER_ROW;
	return textureY * COORD_ADDITION_PER_TEXTURE;
}

void main(void) {
	uint GREY_SCALE_BIT_MASK = 1u << 31;

	uint textureIdWithoutGreyScaleBit = textureId & ~GREY_SCALE_BIT_MASK;
	pass_texCoords = texCoords + vec2(getTextureCoordAddition_x(textureIdWithoutGreyScaleBit), getTextureCoordAddition_y(textureIdWithoutGreyScaleBit));
	pass_shouldRenderInGrayScale = textureId & GREY_SCALE_BIT_MASK;

	gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}

)";